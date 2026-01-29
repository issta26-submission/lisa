/*
  Unit test suite for the focal method: lsm_config_work_hook
  - Target: lsm_config_work_hook in lsm_main.c
  - Language: C++11
  - Testing approach: minimal, custom test harness (no GTest)
  - Assumptions: lsmInt.h provides the lsm_db type used by lsm_config_work_hook
  - Strategy: cover typical usage by asserting that:
      pDb->xWork is assigned to the provided function pointer
      pDb->pWorkCtx is assigned to the provided context pointer
    Additionally validate null-pointer handling by passing null xWork or null pCtx.
  - This test code is self-contained and relies only on the project's headers.
*/

#include <lsmInt.h>
#include <iostream>
#include <cstring>


// Include the project header that defines lsm_db and related API.
// This header is assumed to exist in the project environment.

// Ensure C linkage for the focal function to avoid name mangling
extern "C" void lsm_config_work_hook(
  lsm_db *pDb, 
  void (*xWork)(lsm_db *, void *), 
  void *pCtx
);

/* 
  Dummy work functions used to verify pointer assignments.
  Different functions allow us to verify function-pointer equality semantics.
*/
static void dummyWorkA(lsm_db *db, void *ctx) { (void)db; (void)ctx; }
static void dummyWorkB(lsm_db *db, void *ctx) { (void)db; (void)ctx; }

/* Test 1: Basic assignment
   - Goal: After calling lsm_config_work_hook, verify that xWork and pWorkCtx
     are updated to the provided values.
*/
static bool test_basic_assignment() {
  lsm_db db;
  // Initialize to a known state to avoid relying on default values
  std::memset(&db, 0, sizeof(db));

  void *ctx = (void*)0x1234;
  lsm_config_work_hook(&db, dummyWorkA, ctx);

  bool passed = (db.xWork == dummyWorkA) && (db.pWorkCtx == ctx);
  return passed;
}

/* Test 2: Null xWork handling
   - Goal: When xWork is NULL, ensure the field is set to NULL and pWorkCtx preserved as provided.
*/
static bool test_null_xwork_overwrite() {
  lsm_db db;
  std::memset(&db, 0, sizeof(db));

  // Pre-fill with some values to ensure overwrite actually happens
  db.xWork = dummyWorkB;
  void *ctx = (void*)0xdead;
  lsm_config_work_hook(&db, nullptr, ctx);

  bool passed = (db.xWork == nullptr) && (db.pWorkCtx == ctx);
  return passed;
}

/* Test 3: Null pCtx handling
   - Goal: When pCtx is NULL, ensure the field is set to NULL and xWork updated as provided.
*/
static bool test_null_pctx_overwrite() {
  lsm_db db;
  std::memset(&db, 0, sizeof(db));

  // Pre-fill with a known non-NULL ctx to verify overwrite
  void *ctx = (void*)0xdef;
  db.pWorkCtx = (void*)0xface;
  lsm_config_work_hook(&db, dummyWorkA, nullptr);

  bool passed = (db.xWork == dummyWorkA) && (db.pWorkCtx == nullptr);
  return passed;
}

/* Test 4: Multiple sequential updates
   - Goal: Ensure that successive calls to lsm_config_work_hook reflect the last assigned values.
*/
static bool test_multiple_calls() {
  lsm_db db;
  std::memset(&db, 0, sizeof(db));

  // First update
  lsm_config_work_hook(&db, dummyWorkA, (void*)0x1111);
  if (!(db.xWork == dummyWorkA && db.pWorkCtx == (void*)0x1111)) {
    return false;
  }

  // Second update with a different function and context
  lsm_config_work_hook(&db, dummyWorkB, (void*)0x2222);
  bool ok = (db.xWork == dummyWorkB) && (db.pWorkCtx == (void*)0x2222);
  return ok;
}

int main() {
  int total = 0;
  int passed = 0;

  auto runTest = [&](const char* name, bool (*fn)()) {
    ++total;
    bool r = fn();
    if (r) {
      ++passed;
      std::cout << "[PASS] " << name << "\n";
    } else {
      std::cout << "[FAIL] " << name << "\n";
    }
  };

  // Run all tests
  runTest("test_basic_assignment", test_basic_assignment);
  runTest("test_null_xwork_overwrite", test_null_xwork_overwrite);
  runTest("test_null_pctx_overwrite", test_null_pctx_overwrite);
  runTest("test_multiple_calls", test_multiple_calls);

  std::cout << "Summary: " << passed << " / " << total << "\n";
  // Non-terminating, return non-zero if any test failed
  return (passed == total) ? 0 : 1;
}