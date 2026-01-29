// C++11 test suite for sqlite3ParserAddCleanup (from prepare.c)
// This test suite does not rely on GTest. It uses a small, non-terminating assertion mechanism
// and a minimal fake SQLite/C runtime environment to exercise the focal method's behavior.

#include <iostream>
#include <cstdlib>
#include <sqliteInt.h>
#include <cstdint>


// Forward declarations to mirror the C API used by sqlite3ParserAddCleanup.
// We provide minimal, test-focused structures that match the expected layout in the focal function.
struct sqlite3;      // Opaque sqlite3 database object (test doubles only)
struct Parse;         // Focal class/structure used by sqlite3ParserAddCleanup
struct ParseCleanup;  // Node in the Parser's cleanup list

// Minimal fake storage for the test environment
struct sqlite3 { int dummy; }; // dummy for type completeness

// Define ParseCleanup layout as expected by sqlite3ParserAddCleanup
struct ParseCleanup {
  ParseCleanup *pNext;
  void *pPtr;
  void (*xCleanup)(sqlite3*, void*);
};

// Define Parse layout used by sqlite3ParserAddCleanup
struct Parse {
  sqlite3 *db;
  ParseCleanup *pCleanup;
  // In the original code, there might be a conditional member (earlyCleanup) under
  // SQLITE_DEBUG/SQLITE_COVERAGE_TEST. We omit it here to keep the layout minimal for tests.
};

// We declare the focal function as C to avoid name mangling issues when linking with prepare.c
extern "C" void *sqlite3ParserAddCleanup(
  Parse *pParse,
  void (*xCleanup)(sqlite3*,void*),
  void *pPtr
);

// ---- Test harness scaffolding ----

// Global state used by test double for cleanup callback
static bool g_cleanup_called = false;
static void *g_cleanup_last_ptr = nullptr;
static sqlite3 *g_cleanup_last_db = nullptr;

// A flag to force sqlite3DbMallocRaw to fail (simulate allocation failure)
static bool g_force_malloc_fail = false;

// A simple in-test memory allocator hook to mimic sqlite3DbMallocRaw behavior.
// We count allocations (by size) and optionally fail to simulate OOM.
static size_t g_allocated_bytes = 0;
extern "C" void *sqlite3DbMallocRaw(sqlite3*, size_t n){
  if(g_force_malloc_fail){
    return nullptr;
  }
  void *p = std::malloc(n);
  if(p) g_allocated_bytes += n;
  return p;
}

// Mock cleanup function to verify that the focal method stores the xCleanup callback
static void test_cleanup(sqlite3* db, void* pPtr){
  g_cleanup_called = true;
  g_cleanup_last_ptr = pPtr;
  g_cleanup_last_db = db;
}

// Simple non-terminating assertion macro.
// It increments a global counter and prints a message on failure, but does not abort tests.
static int g_total_tests = 0;
static int g_failed_tests = 0;
#define TEST_ASSERT(cond, msg) do { \
  ++g_total_tests; \
  if(!(cond)) { \
    std::cerr << "TEST FAILURE: " << msg \
              << " [Condition: " << #cond << "]" \
              << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    ++g_failed_tests; \
  } \
} while(0)

// Helper to reset global state before each test
static void reset_globals(){
  g_cleanup_called = false;
  g_cleanup_last_ptr = nullptr;
  g_cleanup_last_db = nullptr;
  g_force_malloc_fail = false;
  g_allocated_bytes = 0;
}

// ---- Unit Tests ----

// Test 1: Allocation succeeds. Verifies that a new cleanup node is prepended to the list
// and that the node stores the correct pointer and cleanup function.
static void TestAllocSuccess(){
  reset_globals();

  // Prepare parse object
  Parse p;
  p.db = reinterpret_cast<sqlite3*>(0xDEADBEEF);
  p.pCleanup = nullptr;

  void *ptr = reinterpret_cast<void*>(0x1111);

  void *ret = sqlite3ParserAddCleanup(&p, test_cleanup, ptr);

  // True branch: allocation succeeds, should return the original pointer
  TEST_ASSERT(ret == ptr, "sqlite3ParserAddCleanup should return original pointer when allocation succeeds");

  // The cleanup list should have been updated
  TEST_ASSERT(p.pCleanup != nullptr, "ParseCleanup node should be allocated and linked into pParse->pCleanup");
  TEST_ASSERT(p.pCleanup->pPtr == ptr, "Node's pPtr should be the original pointer passed in");
  TEST_ASSERT(p.pCleanup->xCleanup == test_cleanup, "Node should store the provided xCleanup callback");
  TEST_ASSERT(p.pCleanup->pNext == nullptr, "First node's pNext should be NULL (head of list)");
  TEST_ASSERT(g_allocated_bytes >= sizeof(ParseCleanup), "Allocator should have allocated at least size of ParseCleanup");
  TEST_ASSERT(g_cleanup_called == false, "Cleanup callback should not have been invoked in success path");
}

// Test 2: Allocation fails. Verifies that the cleanup function is invoked immediately and
// that the original pointer is cleared (set to NULL/0).
static void TestAllocFail(){
  reset_globals();

  // Force allocation to fail
  g_force_malloc_fail = true;

  Parse p;
  p.db = reinterpret_cast<sqlite3*>(0xBEEFDEAD);
  p.pCleanup = nullptr;

  void *ptr = reinterpret_cast<void*>(0x2222);

  void *ret = sqlite3ParserAddCleanup(&p, test_cleanup, ptr);

  // False branch: allocation fails, function should call xCleanup and return NULL
  TEST_ASSERT(ret == nullptr, "sqlite3ParserAddCleanup should return NULL when allocation fails");
  TEST_ASSERT(p.pCleanup == nullptr, "ParseCleanup should not be created when allocation fails");
  TEST_ASSERT(g_cleanup_called == true, "xCleanup should be invoked when allocation fails");
  TEST_ASSERT(g_cleanup_last_ptr == ptr, "Cleanup callback should receive the original pointer");
  TEST_ASSERT(g_cleanup_last_db == p.db, "Cleanup callback should receive the correct sqlite3* (db) pointer");
}

// Test 3: Multiple allocations on the same Parse object.
// Verifies that successive calls prepend new nodes and that the linked list order is correct.
static void TestMultipleAllocations(){
  reset_globals();

  Parse p;
  p.db = reinterpret_cast<sqlite3*>(0xCAFEBABE);
  p.pCleanup = nullptr;

  // First insertion
  void *ptr1 = reinterpret_cast<void*>(0x100);
  void *ret1 = sqlite3ParserAddCleanup(&p, test_cleanup, ptr1);
  TEST_ASSERT(ret1 == ptr1, "First allocation should return the original pointer");
  TEST_ASSERT(p.pCleanup != nullptr, "First node should be allocated");
  TEST_ASSERT(p.pCleanup->pPtr == ptr1, "First node's pPtr should be ptr1");
  TEST_ASSERT(p.pCleanup->pNext == nullptr, "First node's next should be NULL");

  // Second insertion (should become new head)
  void *ptr2 = reinterpret_cast<void*>(0x200);
  void *ret2 = sqlite3ParserAddCleanup(&p, test_cleanup, ptr2);
  TEST_ASSERT(ret2 == ptr2, "Second allocation should return the original pointer");
  TEST_ASSERT(p.pCleanup != nullptr, "Second node should be allocated and set as head");
  TEST_ASSERT(p.pCleanup->pPtr == ptr2, "Second node (head) should have pPtr equal to ptr2");
  TEST_ASSERT(p.pCleanup->pNext != nullptr, "Head's next should point to the previous node");
  TEST_ASSERT(p.pCleanup->pNext->pPtr == ptr1, "Previous node should still contain ptr1");
  TEST_ASSERT(p.pCleanup->pNext->pNext == nullptr, "Previous node should be the tail with NULL next");
  TEST_ASSERT(p.pCleanup->xCleanup == test_cleanup, "Head node should store the correct cleanup function");
}

// ---- Test Runner ----

// Note: We intentionally expose a minimal main() that runs the tests sequentially.
// Non-terminating assertions ensure all tests are exercised even if some fail.
int main() {
  TestAllocSuccess();
  TestAllocFail();
  TestMultipleAllocations();

  std::cout << "Unit test run complete. "
            << "Total tests: " << g_total_tests
            << ", Failures: " << g_failed_tests
            << std::endl;

  if (g_failed_tests > 0) {
    std::cerr << "Some tests FAILED. See messages above for details." << std::endl;
    return 1;
  }
  return 0;
}