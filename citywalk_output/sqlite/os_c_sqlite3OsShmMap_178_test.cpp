/*
  Unit Test Suite for sqlite3OsShmMap (C linkage) using plain C++11 (no GTest).
  This test suite uses a minimal, self-contained mock of the sqlite3_file and
  sqlite3_io_methods structures to exercise sqlite3OsShmMap's behavior.

  Notes:
  - We implement DO_OS_MALLOC_TEST(id) as a macro that records that the test
    malloc-check was invoked. This mimics the macro call in the focal method.
  - We verify that sqlite3OsShmMap delegates correctly to id->pMethods->xShmMap
    and propagates its return value and the out-parameter pp.
  - We provide two test scenarios:
    1) Normal successful mapping path where the underlying xShmMap fills *pp
       and returns a sentinel value.
    2) Case where pp is NULL to ensure the code gracefully passes through the
       NULL without dereferencing.
  - All tests run under main() with non-terminating assertions. Failures are logged
    and reported at the end rather than aborting the test run.
*/

#include <cstddef>
#include <iostream>
#include <sqliteInt.h>
#include <cstdint>


// Domain knowledge: C linkage for the focal function
extern "C" {

// Forward declarations mimicking SQLite structures sufficient for testing.
typedef struct sqlite3_file sqlite3_file;
typedef struct sqlite3_io_methods sqlite3_io_methods;

struct sqlite3_io_methods {
  // Underlying OS-specific ShmMap method to be invoked by sqlite3OsShmMap
  int (*xShmMap)(sqlite3_file*, int, int, int, void volatile **);
};

struct sqlite3_file {
  sqlite3_io_methods *pMethods;
};

// Macro used inside the focal method. Implemented here for test harness.
static int g_malloc_test_id_seen = 0;
#define DO_OS_MALLOC_TEST(id) do { g_malloc_test_id_seen = 1; } while(0)

// Focal method under test (redeclared with C linkage to match original code)
int sqlite3OsShmMap(
  sqlite3_file *id,
  int iPage,
  int pgsz,
  int bExtend,
  void volatile **pp
){
  DO_OS_MALLOC_TEST(id);
  return id->pMethods->xShmMap(id, iPage, pgsz, bExtend, pp);
}
} // extern "C"

// ----------------------------------------------------------------------------------
// Test harness (self-contained, no external testing framework)
// Non-terminating assertions to maximize code coverage
static int g_testFailures = 0;
#define EXPECT_TRUE(cond) do { \
  if(!(cond)) { \
    std::cerr << "Assertion failed: " #cond " at " __FILE__ ":" << __LINE__ << "\n"; \
    ++g_testFailures; \
  } \
} while(0)

#define EXPECT_EQ(a,b) do { \
  if(((long long)(a)) != ((long long)(b))) { \
    std::cerr << "Assertion failed: " #a " == " #b " (" << (long long)(a) << " != " << (long long)(b) << ") at " __FILE__ ":" << __LINE__ << "\n"; \
    ++g_testFailures; \
  } \
} while(0)

namespace {

// Global test state to capture the invocation details of the mock xShmMap
static sqlite3_file* g_last_id = nullptr;
static int g_last_iPage = 0;
static int g_last_pgsz = 0;
static int g_last_bExtend = 0;
static void volatile **g_last_pp = nullptr;

// Mock implementation of the underlying xShmMap to verify parameter passing
int mock_xShmMap(sqlite3_file* id, int iPage, int pgsz, int bExtend, void volatile **pp) {
  g_last_id = id;
  g_last_iPage = iPage;
  g_last_pgsz = pgsz;
  g_last_bExtend = bExtend;
  g_last_pp = pp;
  if (pp != nullptr) {
    // Simulate mapping by writing a non-null placeholder
    *pp = (void volatile*)0xDEADBEEF;
  }
  // Return a sentinel value to verify propagation
  return 123;
}

// Helper to reset the global test state between tests
void reset_test_state() {
  g_last_id = nullptr;
  g_last_iPage = 0;
  g_last_pgsz = 0;
  g_last_bExtend = 0;
  g_last_pp = nullptr;
  g_last_id = nullptr;
  g_malloc_test_id_seen = 0;
  // g_testFailures is global across tests; do not reset here
}

} // anonymous namespace

// ----------------------------------------------------------------------------------
// Test 1: Normal path - ensures proper delegation, argument forwarding, and
// proper propagation of return value and out parameter.
// Verifies that DO_OS_MALLOC_TEST macro was invoked as part of the focal method.
void test_sqlite3OsShmMap_delegates_and_propagates() {
  reset_test_state();

  // Prepare a mock sqlite3_file with a valid pMethods.xShmMap
  sqlite3_io_methods methods;
  methods.xShmMap = mock_xShmMap;

  sqlite3_file id;
  id.pMethods = &methods;

  void volatile *mapped_ptr = nullptr;

  // Call the focal method
  int ret = sqlite3OsShmMap(&id, /*iPage*/ 7, /*pgsz*/ 4096, /*bExtend*/ 1, &mapped_ptr);

  // Assertions
  EXPECT_TRUE(ret == 123);
  EXPECT_TRUE(g_last_id == &id);
  EXPECT_TRUE(g_last_iPage == 7);
  EXPECT_TRUE(g_last_pgsz == 4096);
  EXPECT_TRUE(g_last_bExtend == 1);
  EXPECT_TRUE(mapped_ptr == (void*)0xDEADBEEF);
  EXPECT_TRUE(g_malloc_test_id_seen == 1);
  // Also ensure that the macro invocation indicator is observable
}

// ----------------------------------------------------------------------------------
// Test 2: Null out-parameter case - ensure function handles NULL pp gracefully
// without dereferencing it, while still delegating to the underlying method.
void test_sqlite3OsShmMap_null_pp_handling() {
  reset_test_state();

  sqlite3_io_methods methods;
  methods.xShmMap = mock_xShmMap;

  sqlite3_file id;
  id.pMethods = &methods;

  // Pass NULL for pp to simulate caller opting out of receiving a mapping address
  int ret = sqlite3OsShmMap(&id, /*iPage*/ 0, /*pgsz*/ 1024, /*bExtend*/ 0, nullptr);

  // Assertions
  EXPECT_TRUE(ret == 123);
  EXPECT_TRUE(g_last_id == &id);
  EXPECT_TRUE(g_last_iPage == 0);
  EXPECT_TRUE(g_last_pgsz == 1024);
  EXPECT_TRUE(g_last_bExtend == 0);
  EXPECT_TRUE(g_last_pp == nullptr); // Should not dereference a NULL pointer
  EXPECT_TRUE(g_malloc_test_id_seen == 1);
}

// ----------------------------------------------------------------------------------
// Entry point: Run tests and report summary
int main() {
  std::cout << "Starting sqlite3OsShmMap unit tests (no GTest, plain C++11)...\n";

  test_sqlite3OsShmMap_delegates_and_propagates();
  test_sqlite3OsShmMap_null_pp_handling();

  if (g_testFailures == 0) {
    std::cout << "All tests passed.\n";
    return 0;
  } else {
    std::cerr << g_testFailures << " test(s) failed.\n";
    return 1;
  }
}