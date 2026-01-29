// Lightweight C++11 test harness for sqlite3VdbeMemStringify in vdbemem.c
// This file provides minimal stubs to exercise the focal function without
// depending on external libraries (GTest, GMock). It uses a small, non-terminating
// assertion framework to maximize code coverage across branches.

#include <vector>
#include <cstdio>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <cassert>


// -------------------------
// Minimal SQLite-like dependencies and types (stubs)
// -------------------------

// Result codes (subset)
const int SQLITE_OK = 0;
const int SQLITE_NOMEM_BKPT = -1;

// Encoding constants
const int SQLITE_UTF8 = 1;

// Mem flags (subset)
const int MEM_Zero    = 0x01;
const int MEM_Str     = 0x02;
const int MEM_Blob    = 0x04;
const int MEM_Int     = 0x08;
const int MEM_Real    = 0x10;
const int MEM_IntReal = 0x20;
const int MEM_Term    = 0x40;

// Forward declaration of Mem to align with test scaffolding
struct Mem;
struct Db;

// Simple mutex placeholder (non-functional in this test)
struct sqlite3_mutex_t { };

// Simple Db placeholder with a mutex indicator
struct Db {
  sqlite3_mutex_t* mutex;
};

// Macro to check 8-byte alignment for the Mem object
#define EIGHT_BYTE_ALIGNMENT(p) ( ((uintptr_t)(p) & 7) == 0 )

// Static helpers used by the focal function
static inline int sqlite3Strlen30NN(const char* z) {
  return (int)strlen(z);
}

// Function stubs to satisfy linking with the focal function
static void vdbeMemRenderNum(int sz, char *zBuf, Mem *p);
static int sqlite3VdbeMemClearAndResize(Mem *pMem, int szNew);
static void sqlite3VdbeChangeEncoding(Mem *pMem, int desiredEnc);
static int sqlite3VdbeMemIsRowSet(const Mem *pMem);
static int sqlite3_mutex_held(sqlite3_mutex_t *mutex);

// The focal function under test (embedded as in <FOCAL_METHOD>)
// We provide a direct translation adapted to the test harness.
// Note: We rely on the minimal stubs above to satisfy dependencies.
int sqlite3VdbeMemStringify(Mem *pMem, unsigned char enc, unsigned char bForce){
  const int nByte = 32;
  assert( pMem!=0 );
  assert( pMem->db==0 || sqlite3_mutex_held(pMem->db->mutex) );
  assert( !(pMem->flags&MEM_Zero) );
  assert( !(pMem->flags&(MEM_Str|MEM_Blob)) );
  assert( pMem->flags&(MEM_Int|MEM_Real|MEM_IntReal) );
  assert( !sqlite3VdbeMemIsRowSet(pMem) );
  assert( EIGHT_BYTE_ALIGNMENT(pMem) );

  if( sqlite3VdbeMemClearAndResize(pMem, nByte) ){
    pMem->enc = 0;
    return SQLITE_NOMEM_BKPT;
  }
  vdbeMemRenderNum(nByte, pMem->z, pMem);
  assert( pMem->z!=0 );
  assert( pMem->n==(int)sqlite3Strlen30NN(pMem->z) );
  pMem->enc = SQLITE_UTF8;
  pMem->flags |= MEM_Str|MEM_Term;
  if( bForce ) pMem->flags &= ~(MEM_Int|MEM_Real|MEM_IntReal);
  sqlite3VdbeChangeEncoding(pMem, enc);
  return SQLITE_OK;
}

// -------------------------
// Minimal Mem implementation for testing
// -------------------------

struct Mem {
  int flags;    // bitfield of MEM_* flags
  int enc;      // encoding (SQLITE_UTF8 etc.)
  char *z;      // string/buffer
  int n;        // length of string in z
  Db *db;       // associated database
};

// Simple global to control simulated OOM behavior
static bool g_oomNext = false;

// vdbeMemRenderNum: render the size as a decimal string into zBuf
static void vdbeMemRenderNum(int sz, char *zBuf, Mem *p){
  // Simple decimal rendering
  if (zBuf){
    std::snprintf(zBuf, 32, "%d", sz);
  }
  // Update Mem length placeholder; actual strlen is validated by caller
  if (p){
    p->n = (int)std::strlen(zBuf);
  }
}

// sqlite3VdbeMemClearAndResize: simulate memory resize, can simulate OOM
static int sqlite3VdbeMemClearAndResize(Mem *pMem, int szNew){
  if (g_oomNext){
    // Reset flag to permit subsequent tests
    g_oomNext = false;
    // Simulate OOM: return non-zero
    // Keep existing z untouched (we mimic a failure path)
    return 1;
  }
  // Allocate or resize buffer
  if (szNew <= 0) szNew = 1;
  char *newz = (char*)realloc(pMem->z, (size_t)szNew);
  if (!newz) return 1;
  pMem->z = newz;
  // Ensure a non-null terminator
  pMem->z[0] = '\0';
  // Track the size for use by vdbeMemRenderNum
  // We cannot set pMem->n here precisely; the render function will set it.
  return 0; // success
}

// sqlite3VdbeMemChangeEncoding: set the desired encoding
static void sqlite3VdbeChangeEncoding(Mem *pMem, int desiredEnc){
  pMem->enc = desiredEnc;
}

// sqlite3VdbeMemIsRowSet: test stub, returns false by default
static int sqlite3VdbeMemIsRowSet(const Mem *pMem){
  (void)pMem;
  return 0;
}

// sqlite3_mutex_held: stub; returns true if mutex pointer is non-null
static int sqlite3_mutex_held(sqlite3_mutex_t *mutex){
  return mutex != nullptr;
}

// -------------------------
// Lightweight test framework (non-terminating assertions)
// -------------------------

static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;
static std::vector<std::string> g_logs;

// Non-terminating assertion macro
#define EXPECT_TRUE(cond, msg) \
  do { \
    ++g_tests_run; \
    if(!(cond)) { \
      g_logs.push_back(std::string("FAIL: ") + (msg)); \
      ++g_tests_failed; \
    } else { \
      ++g_tests_passed; \
    } \
  } while(0)

#define EXPECT_EQ(a,b,msg) \
  do { \
    ++g_tests_run; \
    if(!((a) == (b))) { \
      g_logs.push_back(std::string("FAIL: ") + (msg) + " (expected " #a "==" #b ", got " + std::to_string((a)) + "!=" + std::to_string((b)) + ")"); \
      ++g_tests_failed; \
    } else { \
      ++g_tests_passed; \
    } \
  } while(0)

static void log_results() {
  for (const auto &s : g_logs) {
    std::cout << s << std::endl;
  }
  std::cout << "Tests run: " << g_tests_run
            << ", Passed: " << g_tests_passed
            << ", Failed: " << g_tests_failed << std::endl;
}

// Helper to create a Mem object with convenient initial state
static Mem* createMem(int flags, Db* pDb=nullptr){
  Mem* m = new Mem();
  m->flags = flags;
  m->enc = 0;
  m->z = nullptr;
  m->n = 0;
  m->db = pDb;
  return m;
}

// -------------------------
// Test cases
// -------------------------

/*
Test 1: Basic stringify path (bForce = 0)
- pMem has MEM_Int and no MEM_Str/MEM_Blob
- ClearAndResize succeeds
- z becomes "32" representing nByte
- n equals strlen(z)
- enc is set to SQLITE_UTF8, flags include MEM_Str and MEM_Term
- MEM_Int flag remains set
*/
static void test_basic_stringify_success(){
  Mem m;
  m.flags = MEM_Int;     // must include one of MEM_Int|MEM_Real|MEM_IntReal
  m.z = nullptr;
  m.n = 0;
  m.enc = 0;
  m.db = nullptr;

  int rc = sqlite3VdbeMemStringify(&m, 5 /* enc */, 0 /* bForce */);

  EXPECT_EQ(rc, SQLITE_OK, "Basic stringify should return SQLITE_OK");
  // After stringify
  EXPECT_TRUE(m.z != nullptr, "z should be allocated");
  EXPECT_EQ(std::strcmp(m.z, "32"), 0, "z should contain '32'");

  EXPECT_EQ(m.n, 2, "n should be length of z ('32')");
  EXPECT_EQ(m.enc, SQLITE_UTF8, "encoding should be set to SQLITE_UTF8 inside stringify");
  EXPECT_TRUE( (m.flags & MEM_Str) != 0, "flags should include MEM_Str after stringify");
  EXPECT_TRUE( (m.flags & MEM_Term) != 0, "flags should include MEM_Term after stringify");
  EXPECT_TRUE( (m.flags & MEM_Int) != 0, "MEM_Int should remain set when bForce is false");
  // Cleanup
  free(m.z);
}

/*
Test 2: Force zeroing of numeric affinity flags (bForce = 1)
- Start with MEM_Int|MEM_Real
- After stringify, MEM_Int|MEM_Real|MEM_IntReal should be cleared
- Other behavior same as Test 1
*/
static void test_stringify_with_bForce_clear_numeric_flags(){
  Mem m;
  m.flags = MEM_Int | MEM_Real; // numeric affinities set
  m.z = nullptr;
  m.n = 0;
  m.enc = 0;
  m.db = nullptr;

  int rc = sqlite3VdbeMemStringify(&m, 2 /* enc */, 1 /* bForce */);

  EXPECT_EQ(rc, SQLITE_OK, " stringify with bForce should return SQLITE_OK");
  // After stringify
  EXPECT_TRUE(m.z != nullptr, "z should be allocated");
  EXPECT_EQ(std::strcmp(m.z, "32"), 0, "z should contain '32'");

  EXPECT_EQ(m.n, 2, "n should be length of z ('32')");
  EXPECT_EQ(m.enc, SQLITE_UTF8, "encoding should be set to SQLITE_UTF8 inside stringify");
  EXPECT_TRUE( (m.flags & MEM_Str) != 0, "flags should include MEM_Str after stringify");
  EXPECT_TRUE( (m.flags & MEM_Term) != 0, "flags should include MEM_Term after stringify");
  EXPECT_TRUE( (m.flags & MEM_Int) == 0, "MEM_Int should be cleared when bForce is true");
  EXPECT_TRUE( (m.flags & MEM_Real) == 0, "MEM_Real should be cleared when bForce is true");
  EXPECT_TRUE( (m.flags & MEM_IntReal) == 0, "MEM_IntReal should be cleared when bForce is true");
  // Cleanup
  free(m.z);
}

/*
Test 3: OOM path (sqlite3VdbeMemClearAndResize returns non-zero)
- Ensure pMem->enc is set to 0 and function returns SQLITE_NOMEM_BKPT
*/
static void test_oom_path(){
  Mem m;
  m.flags = MEM_Int;
  m.z = nullptr;
  m.n = 0;
  m.enc = 0;
  m.db = nullptr;

  // Enable OOM for next allocation
  g_oomNext = true;

  int rc = sqlite3VdbeMemStringify(&m, 7 /* enc */, 0);

  EXPECT_EQ(rc, SQLITE_NOMEM_BKPT, "OOM path should return SQLITE_NOMEM_BKPT");
  EXPECT_TRUE(m.enc == 0, "On OOM, enc should be reset to 0");
  // No z allocation guaranteed
  // (We do not rely on z in OOM path)
}

/*
Test 4: DB with a mutex held (pMem->db != nullptr and mutex non-null)
- This confirms the assertion path requiring a held mutex is satisfied
- Similar to Test 1 but with a dummy DB present
*/
static void test_with_db_mutex_held(){
  // Prepare a dummy mutex and DB
  sqlite3_mutex_t dummyMutex;
  Db dummyDb;
  dummyDb.mutex = &dummyMutex;

  Mem m;
  m.flags = MEM_Int;
  m.z = nullptr;
  m.n = 0;
  m.enc = 0;
  m.db = &dummyDb;

  int rc = sqlite3VdbeMemStringify(&m, 9 /* enc */, 0);

  EXPECT_EQ(rc, SQLITE_OK, "Stringify with held mutex should succeed");
  EXPECT_TRUE(m.z != nullptr, "z should be allocated");
  EXPECT_EQ(std::strcmp(m.z, "32"), 0, "z should contain '32'");
  // Cleanup
  free(m.z);
}

// -------------------------
// Main: run tests
// -------------------------

int main() {
  test_basic_stringify_success();
  test_stringify_with_bForce_clear_numeric_flags();
  test_oom_path();
  test_with_db_mutex_held();

  log_results();
  std::cout << "Summary: total=" << g_tests_run
            << " passed=" << g_tests_passed
            << " failed=" << g_tests_failed << std::endl;
  return g_tests_failed ? 1 : 0;
}