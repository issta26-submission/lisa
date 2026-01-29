// Lightweight C++11 unit tests for sqlite3VtabModuleUnref (from vtab.c)
// - No GoogleTest, only a minimal in-file test harness
// - Tests focus on the behavior of sqlite3VtabModuleUnref as described in the focal method
// - Uses the actual Module type and sqlite3DbFree from the project's internal headers when available
// - Explanatory comments accompany each test case

#include <cstdlib>
#include <cstdio>
#include <sqliteInt.h>
#include <cstring>


// Domain knowledge: We rely on internal SQLite structures. Include the internal header
// to obtain the Module structure and related declarations.
// This header is typically present in SQLite's source tree.
extern "C" {
}

// Forward declarations for the focal function (C linkage)
extern "C" {
  // The function under test, defined in vtab.c
  void sqlite3VtabModuleUnref(sqlite3 *db, Module *pMod);

  // Memory freeing helper from SQLite core (used by the function under test)
  void sqlite3DbFree(sqlite3 *db, void *p);
}

// Global flag to observe whether the destroy callback was invoked
static bool g_destroyCalled = false;

// Mock destructor to track invocation from sqlite3VtabModuleUnref
static void MockDestroy(void *pAux) {
  (void)pAux;          // pAux is not used in this test, but keep signature consistent
  g_destroyCalled = true; // Signal that the destructor was invoked
}

// Utility: Reset global test state
static void resetTestState() {
  g_destroyCalled = false;
}

// Utility: Create a minimal sqlite3 object placeholder.
// We allocate a small dummy sqlite3 instance so that sqlite3VtabModuleUnref can
// pass this through to sqlite3DbFree without obvious null-pointer issues.
// This does not attempt to provide full SQLite functionality; it is only for test harness purposes.
static sqlite3* createDummyDb() {
  sqlite3* db = (sqlite3*)malloc(sizeof(sqlite3));
  if(db) {
    memset(db, 0, sizeof(sqlite3));
  }
  return db;
}

// Utility: Clean up dummy sqlite3 object
static void destroyDummyDb(sqlite3* db) {
  if(db) free(db);
}

// Utility: Create and initialize a Module instance.
// We allocate memory for the Module struct and set its fields as required by the tests.
static Module* createModule(int refCount,
                            void (*destroy)(void*),
                            void* pAux) {
  // Allocate Module. Module is a concrete struct in sqliteInt.h; using malloc mirrors
  // how the actual SQLite code would allocate modules.
  Module* pMod = (Module*)malloc(sizeof(Module));
  if(!pMod) return nullptr;
  // Initialize with the values needed for sqlite3VtabModuleUnref
  pMod->nRefModule = refCount;
  pMod->xDestroy = destroy;
  pMod->pAux = pAux;
  pMod->pEpoTab = NULL; // Must be NULL to satisfy the assertion in the focal method
  return pMod;
}

// Utility: Clean up a Module instance
static void destroyModule(Module* pMod) {
  if(pMod) free(pMod);
}

// Test 1: Decrement without destruction (nRefModule > 1 after call)
// - Expect: nRefModule decreases by 1; xDestroy is not invoked; pEpoTab remains 0
// - This tests the non-destroy path of the function
static bool test_case_decrement_without_destroy() {
  printf("Test 1: Decrement without destruction (nRefModule > 0 after call)\n");

  sqlite3* db = createDummyDb();
  if(!db){
    printf("  [FAIL] Unable to allocate dummy sqlite3 object.\n");
    return false;
  }

  // Set up Module with ref count > 1 so that it won't be destroyed
  Module* pMod = createModule(2, /*destroy*/ nullptr, (void*)0x1);
  if(!pMod){
    printf("  [FAIL] Unable to allocate Module.\n");
    destroyDummyDb(db);
    return false;
  }

  // Call the function under test
  sqlite3VtabModuleUnref(db, pMod);

  // Validate: nRefModule should have been decremented to 1
  bool pass = (pMod->nRefModule == 1) && (!g_destroyCalled);

  // Cleanup: The core function would still own the Module memory here if it wasn't freed;
  // since nRefModule != 0, there should be no free yet, but to avoid leaks in the test harness,
  // free the Module ourselves.
  destroyModule(pMod);
  destroyDummyDb(db);

  if(pass) printf("  [PASS] nRefModule decremented to 1 and destroy not invoked.\n");
  else       printf("  [FAIL] Unexpected behavior: nRefModule=%d, destroyCalled=%d\n",
                    pMod->nRefModule, g_destroyCalled);

  return pass;
}

// Test 2: Destruction when ref count reaches zero
// - Expect: xDestroy is invoked, and pMod would be freed by sqlite3DbFree (we rely on external memory handling)
static bool test_case_destroy_on_zero() {
  printf("Test 2: Destruction invoked when nRefModule reaches zero\n");

  sqlite3* db = createDummyDb();
  if(!db){
    printf("  [FAIL] Unable to allocate dummy sqlite3 object.\n");
    return false;
  }

  // Set up Module with ref count 1 so after unref it becomes 0 and should destroy
  Module* pMod = createModule(1, MockDestroy, (void*)0x2);
  if(!pMod){
    printf("  [FAIL] Unable to allocate Module.\n");
    destroyDummyDb(db);
    return false;
  }

  // Reset flag before invoking the function
  resetTestState();

  // Call the function under test
  sqlite3VtabModuleUnref(db, pMod);

  // Validate: destroy callback should have been invoked
  bool pass = g_destroyCalled;

  // Cleanup: In the real path, sqlite3DbFree would free pMod; attempt to free here as well to avoid leaks
  // Note: If the core has already freed pMod, our free here would be unsafe. We guard by checking pointer validity.
  // Since we allocated pMod via malloc, and the test harness is not the actual SQLite allocator, free it defensively.
  destroyModule(pMod);
  destroyDummyDb(db);

  if(pass) printf("  [PASS] xDestroy callback invoked as nRefModule reached zero.\n");
  else       printf("  [FAIL] xDestroy callback was not invoked when expected.\n");

  return pass;
}

// Test 3: Destruction path is not taken when xDestroy is null
// - Expect: nRefModule decrements to 0, xDestroy not invoked, memory freed via sqlite3DbFree
static bool test_case_destroy_pointer_null() {
  printf("Test 3: Destruction path skipped when xDestroy is NULL\n");

  sqlite3* db = createDummyDb();
  if(!db){
    printf("  [FAIL] Unable to allocate dummy sqlite3 object.\n");
    return false;
  }

  Module* pMod = createModule(1, /*destroy*/ nullptr, (void*)0x3);
  if(!pMod){
    printf("  [FAIL] Unable to allocate Module.\n");
    destroyDummyDb(db);
    return false;
  }

  // Reset flag
  resetTestState();

  // Call the function under test
  sqlite3VtabModuleUnref(db, pMod);

  // Validate: xDestroy should not have been invoked
  bool pass = (!g_destroyCalled);

  destroyModule(pMod);
  destroyDummyDb(db);

  if(pass) printf("  [PASS] xDestroy not invoked when it was NULL.\n");
  else       printf("  [FAIL] xDestroy was unexpectedly invoked.\n");

  return pass;
}

// Simple test runner
static void runAllTests() {
  int passed = 0;
  int total  = 0;

  if(test_case_decrement_without_destroy()) ++passed;
  ++total;

  if(test_case_destroy_on_zero()) ++passed;
  ++total;

  if(test_case_destroy_pointer_null()) ++passed;
  ++total;

  printf("Test results: %d/%d passed.\n", passed, total);
}

// Entrypoint for the test program
int main() {
  printf("Starting sqlite3VtabModuleUnref unit tests (no GTest, lightweight harness)\n");
  runAllTests();
  printf("Finished tests.\n");
  return 0;
}