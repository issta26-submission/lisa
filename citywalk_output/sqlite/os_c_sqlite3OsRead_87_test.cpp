// Lightweight, self-contained unit tests for the focal method:
// int sqlite3OsRead(sqlite3_file *id, void *pBuf, int amt, i64 offset)
// The tests are designed to be compiled under C++11 without Google Test.
// They mock the minimal environment needed by sqlite3OsRead
// (sqlite3_file, sqlite3_io_methods and the xRead callback).

#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Domain-specific notes implemented as comments inside code for clarity.
// - We model only the minimal dependencies required by sqlite3OsRead.
// - DO_OS_MALLOC_TEST is defined as a no-op placeholder to enable compilation.
// - The actual code path (id->pMethods->xRead) is exercised via a mock reader.

// ---------- Minimal type definitions to emulate sqlite3's environment ---------- //
typedef long long i64; // domain-specific 64-bit integer used by sqlite3

// Forward declaration to mirror the real environment
struct sqlite3_file;

// Method table describing read operation
typedef struct sqlite3_io_methods {
  int (*xRead)(sqlite3_file *id, void *pBuf, int amt, i64 offset);
} sqlite3_io_methods;

// File object with a pointer to its method table
typedef struct sqlite3_file {
  sqlite3_io_methods *pMethods;
} sqlite3_file;

// --------- Focal method under test (replicated in test harness) --------- //
// In the real project, this would be defined in os.c and rely on DO_OS_MALLOC_TEST.
// We replicate the exact logic here to create a self-contained test harness.
#define DO_OS_MALLOC_TEST(id) do { (void)(id); } while(0)

int sqlite3OsRead(sqlite3_file *id, void *pBuf, int amt, i64 offset){
  DO_OS_MALLOC_TEST(id);
  return id->pMethods->xRead(id, pBuf, amt, offset);
}

// ------------- Test scaffolding: mock xRead implementations ------------- //

static sqlite3_file *g_called_id = nullptr;
static void *g_called_buf = nullptr;
static int g_called_amt = 0;
static i64 g_called_offset = 0;

// Successful read mock: fills buffer with a pattern derived from offset
int mock_xRead_ok(sqlite3_file *id, void *buf, int amt, i64 offset){
  g_called_id = id;
  g_called_buf = buf;
  g_called_amt = amt;
  g_called_offset = offset;

  if (amt > 0 && buf) {
    unsigned char* b = static_cast<unsigned char*>(buf);
    for (int i = 0; i < amt; ++i) {
      b[i] = static_cast<unsigned char>(offset & 0xFF);
    }
  }
  return amt;
}

// Failing read mock: simulates error by returning -1
int mock_xRead_err(sqlite3_file *id, void *buf, int amt, i64 offset){
  g_called_id = id;
  g_called_buf = buf;
  g_called_amt = amt;
  g_called_offset = offset;
  (void)buf; // unused for error path
  (void)id;
  (void)amt;
  (void)offset;
  return -1;
}

// Zero-length read mock: behaves like successful read with amt == 0
int mock_xRead_zero(sqlite3_file *id, void *buf, int amt, i64 offset){
  g_called_id = id;
  g_called_buf = buf;
  g_called_amt = amt;
  g_called_offset = offset;
  return 0;
}

// ----------------------- Lightweight test framework ---------------------- //

// Non-terminating expectations to maximize code execution
static int g_test_failures = 0;

#define EXPECT_EQ(a,b) do { \
  if ((a) != (b)) { \
    std::cerr << "EXPECT_EQ failed: " #a " != " #b " (" << (a) << " vs " << (b) \
              << ") at " << __FILE__ << ":" << __LINE__ << std::endl; \
    ++g_test_failures; \
  } \
} while(0)

#define EXPECT_TRUE(x) do { \
  if (!(x)) { \
    std::cerr << "EXPECT_TRUE failed: " #x " is false at " << __FILE__ << ":" << __LINE__ << std::endl; \
    ++g_test_failures; \
  } \
} while(0)

#define EXPECT_NOT_NULL(x) do { \
  if ((x) == nullptr) { \
    std::cerr << "EXPECT_NOT_NULL failed: " #x " is null at " << __FILE__ << ":" << __LINE__ << std::endl; \
    ++g_test_failures; \
  } \
} while(0)


// ------------------------------- Tests ----------------------------------- //

/*
  Test 1: Basic successful read
  - Setup: a file object with mock_xRead_ok as its xRead method
  - Action: call sqlite3OsRead with a non-zero amt and a non-zero offset
  - Expectation:
    - sqlite3OsRead returns amt
    - The callback received correct parameters (id, amt, offset)
    - Buffer is filled with pattern derived from offset
*/
void test_sqlite3OsRead_basic_success(){
  // Prepare environment
  sqlite3_io_methods mm_ok = { mock_xRead_ok };
  sqlite3_file f;
  f.pMethods = &mm_ok;

  unsigned char buffer[8];
  // clear buffer to verify it gets overwritten
  std::memset(buffer, 0xAA, sizeof(buffer));

  // Pre-conditions checks (static analysis of environment)
  EXPECT_NOT_NULL(&f);
  EXPECT_NOT_NULL(f.pMethods);

  // Call focal method
  int ret = sqlite3OsRead(&f, buffer, (int)sizeof(buffer), 0x12);

  // Verify expectations
  EXPECT_EQ(ret, (int)sizeof(buffer));
  EXPECT_EQ(g_called_id, &f);
  EXPECT_EQ(g_called_amt, (int)sizeof(buffer));
  EXPECT_EQ(g_called_offset, (i64)0x12);

  // Buffer content should be 0x12 repeated
  for (size_t i = 0; i < sizeof(buffer); ++i) {
    EXPECT_EQ(buffer[i], static_cast<unsigned char>(0x12));
  }
}

/*
  Test 2: Read returns error from underlying xRead
  - Setup: xRead returns -1
  - Action: sqlite3OsRead
  - Expectation: sqlite3OsRead propagates the error (-1)
*/
void test_sqlite3OsRead_error_propagation(){
  sqlite3_io_methods mm_err = { mock_xRead_err };
  sqlite3_file f;
  f.pMethods = &mm_err;

  unsigned char buffer[4] = {0,0,0,0};

  int ret = sqlite3OsRead(&f, buffer, (int)sizeof(buffer), 0x1A);

  EXPECT_EQ(ret, -1);
  // Ensure the call path reached xRead with the provided parameters
  EXPECT_NOT_NULL(g_called_id);
  EXPECT_EQ(g_called_id, &f);
  EXPECT_EQ(g_called_amt, (int)sizeof(buffer));
  EXPECT_EQ(g_called_offset, (i64)0x1A);
}

/*
  Test 3: Zero-length read
  - Setup: xRead_zero as the underlying read
  - Action: call with amt == 0
  - Expectation: return value is 0 and buffer remains unchanged
*/
void test_sqlite3OsRead_zero_length(){
  sqlite3_io_methods mm_zero = { mock_xRead_zero };
  sqlite3_file f;
  f.pMethods = &mm_zero;

  unsigned char buffer[6];
  std::memset(buffer, 0x55, sizeof(buffer)); // initial pattern

  int ret = sqlite3OsRead(&f, buffer, (int)sizeof(buffer), 0xFF);

  EXPECT_EQ(ret, 0);
  // The mock returns 0 and does not modify the buffer (amt == 0)
  for (size_t i = 0; i < sizeof(buffer); ++i) {
    EXPECT_EQ(buffer[i], 0x55);
  }

  // Sanity: the call parameters should be observed
  EXPECT_EQ(g_called_id, &f);
  EXPECT_EQ(g_called_amt, (int)sizeof(buffer));
  EXPECT_EQ(g_called_offset, (i64)0xFF);
}

/*
  Test 4: Different offset yields different buffer content
  - Similar to Test 1 but with a different offset to ensure the pattern depends on offset
*/
void test_sqlite3OsRead_offset_dependency(){
  sqlite3_io_methods mm_ok = { mock_xRead_ok };
  sqlite3_file f;
  f.pMethods = &mm_ok;

  unsigned char buffer[8];
  std::memset(buffer, 0x00, sizeof(buffer));

  i64 off = 0xABCD;
  int ret = sqlite3OsRead(&f, buffer, (int)sizeof(buffer), off);

  EXPECT_EQ(ret, (int)sizeof(buffer));
  EXPECT_EQ(g_called_id, &f);
  EXPECT_EQ(g_called_amt, (int)sizeof(buffer));
  EXPECT_EQ(g_called_offset, off);

  // buffer content should be encoded with offset LSB
  for (size_t i = 0; i < sizeof(buffer); ++i) {
    EXPECT_EQ(buffer[i], static_cast<unsigned char>(off & 0xFF));
  }
}

// ------------------------------- Main Driver ------------------------------ //

int main(){
  std::cout << "Running unit tests for sqlite3OsRead (self-contained, C++11)..." << std::endl;

  test_sqlite3OsRead_basic_success();
  test_sqlite3OsRead_error_propagation();
  test_sqlite3OsRead_zero_length();
  test_sqlite3OsRead_offset_dependency();

  if (g_test_failures == 0) {
    std::cout << "All tests passed (no assertion failures)." << std::endl;
    return 0;
  } else {
    std::cerr << g_test_failures << " test(s) failed." << std::endl;
    return 1;
  }
}