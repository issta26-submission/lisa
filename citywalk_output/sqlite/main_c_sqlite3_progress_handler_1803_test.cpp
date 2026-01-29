// Unit test suite for the focal method: sqlite3_progress_handler
// This test suite is self-contained and does not rely on GTest.
// It re-implements a minimal, test-friendly version of the focal function
// (with C linkage) and exercises true/false branches of its predicates.
// The tests aim to cover: API armoring path, positive nOps path, and reset path.

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>
#include <cassert>


// Domain-directed macros to enable API armor (simulates the original project behavior)
#ifndef SQLITE_ENABLE_API_ARMOR
#define SQLITE_ENABLE_API_ARMOR 1
#endif

// Tiny mock of required SQLite types and APIs to allow isolated testing
// without pulling in the full SQLite source tree.

// Forward declaration of sqlite3 to mirror expected usage in the focal method.
struct sqlite3;

// Global test flags to simulate API armor behavior and monitor mutex usage.
static int g_safety_ok_for_test = 1;           // 1 = safety check passes, 0 = fails
static bool g_mutex_enter_called = false;      // tracks if sqlite3_mutex_enter invoked
static bool g_mutex_leave_called = false;      // tracks if sqlite3_mutex_leave invoked
static sqlite3* g_mutex_enter_db = nullptr;    // last db passed to mutex_enter (for assertion)

// Minimal sqlite3 structure compatible with test usage.
// Only fields accessed by the focal logic are included.
struct sqlite3 {
  void *mutex;                 // pointer used as a stand-in for a real mutex
  int (*xProgress)(void*);     // user-provided progress callback
  unsigned int nProgressOps;   // number of progress operations requested
  void *pProgressArg;          // argument passed to progress callback
};

// Forward declarations of helpers to simulate SQLite internals in tests.
extern "C" int sqlite3SafetyCheckOk(struct sqlite3 *db);
extern "C" void sqlite3_mutex_enter(void *mutex);
extern "C" void sqlite3_mutex_leave(void *mutex);

// The focal method re-implemented with C linkage for testability.
// Note: The original code may reside in main.c; we mirror its logic here for unit testing.
// We keep the semantics faithful to the provided snippet.
extern "C" void sqlite3_progress_handler(
  sqlite3 *db,
  int nOps,
  int (*xProgress)(void*),
  void *pArg
){
#ifdef SQLITE_ENABLE_API_ARMOR
  if( !sqlite3SafetyCheckOk(db) ){
    (void)0; // SQLITE_MISUSE_BKPT placeholder (no-op in test)
    return;
  }
#endif
  sqlite3_mutex_enter(db->mutex);
  if( nOps>0 ){
    db->xProgress = xProgress;
    db->nProgressOps = (unsigned)nOps;
    db->pProgressArg = pArg;
  }else{
    db->xProgress = 0;
    db->nProgressOps = 0;
    db->pProgressArg = 0;
  }
  sqlite3_mutex_leave(db->mutex);
}

// Implementation of API armor behavior in test environment.
extern "C" int sqlite3SafetyCheckOk(struct sqlite3 *db){
  (void)db; // suppress unused warning when not debugging safety checks
  return g_safety_ok_for_test;
}

// Mock implementations of mutex enter/leave to observe call behavior.
extern "C" void sqlite3_mutex_enter(void *mutex){
  // Record the db pointer passed as "mutex" for assertion in tests
  g_mutex_enter_called = true;
  g_mutex_enter_db = static_cast<sqlite3*>(mutex);
}
extern "C" void sqlite3_mutex_leave(void *mutex){
  (void)mutex; // not strictly required for test assertions
  g_mutex_leave_called = true;
}

// Simple progress function used in tests (its behavior isn't critical for assertions)
static int dummy_progress(void* /*ctx*/){
  return 0;
}

// Lightweight test harness (non-terminating assertions)
static int g_total_tests_run = 0;
static int g_total_failures = 0;

static void log_failure(const std::string& test_name, const std::string& message){
  std::cerr << "[FAIL] " << test_name << ": " << message << "\n";
  ++g_total_failures;
}

#define ASSERT_TRUE(cond, test_name, message) \
  do { \
    ++g_total_tests_run; \
    if(!(cond)){ log_failure(test_name, message); } \
  } while(0)

#define ASSERT_EQ(expected, actual, test_name, message) \
  do { \
    ++g_total_tests_run; \
    if((expected) != (actual)){ \
      std::stringstream ss; ss << "expected: " << (expected) << ", actual: " << (actual); \
      log_failure(test_name, (message + " [" + ss.str() + "]")); \
    } \
  } while(0)

// Helper: reset global test state between tests
static void reset_test_state(){
  g_mutex_enter_called = false;
  g_mutex_leave_called = false;
  g_mutex_enter_db = nullptr;
  g_safety_ok_for_test = 1;
}

// Test 1: True branch for nOps > 0 should populate fields and enter/leave mutex
static bool test_positive_nOps_sets_fields(){
  reset_test_state();
  std::string test_name = "test_positive_nOps_sets_fields";

  sqlite3 db;
  db.mutex = static_cast<void*>(&db); // use db address as mutex token (simple sim)
  db.xProgress = nullptr;
  db.nProgressOps = 0;
  db.pProgressArg = nullptr;

  // Call focal method
  sqlite3_progress_handler(&db, 3, dummy_progress, reinterpret_cast<void*>(0x1234));

  // Assertions
  bool ok = true;
  if(db.xProgress != dummy_progress){
    log_failure(test_name, "db.xProgress should be set to the provided xProgress");
    ok = false;
  }
  if(db.nProgressOps != 3){
    log_failure(test_name, "db.nProgressOps should reflect the provided nOps");
    ok = false;
  }
  if(db.pProgressArg != reinterpret_cast<void*>(0x1234)){
    log_failure(test_name, "db.pProgressArg should be set to the provided pArg");
    ok = false;
  }
  if(!g_mutex_enter_called){
    log_failure(test_name, "sqlite3_mutex_enter should have been called (enter path)");
    ok = false;
  }
  if(!g_mutex_leave_called){
    log_failure(test_name, "sqlite3_mutex_leave should have been called (leave path)");
    ok = false;
  }

  return ok;
}

// Test 2: Else branch for nOps <= 0 should reset fields to zero/NULL
static bool test_zero_nOps_resets_fields(){
  reset_test_state();
  std::string test_name = "test_zero_nOps_resets_fields";

  sqlite3 db;
  db.mutex = static_cast<void*>(&db);
  db.xProgress = reinterpret_cast<int(*)(void*)>(0xDEAD);
  db.nProgressOps = 7;
  db.pProgressArg = reinterpret_cast<void*>(0xBEEF);

  // Call focal method with non-positive nOps
  sqlite3_progress_handler(&db, 0, nullptr, nullptr);

  // Assertions
  bool ok = true;
  if(db.xProgress != 0){
    log_failure(test_name, "db.xProgress should be cleared (set to 0)");
    ok = false;
  }
  if(db.nProgressOps != 0){
    log_failure(test_name, "db.nProgressOps should be reset to 0");
    ok = false;
  }
  if(db.pProgressArg != nullptr){
    log_failure(test_name, "db.pProgressArg should be reset to nullptr");
    ok = false;
  }
  if(!g_mutex_enter_called){
    log_failure(test_name, "sqlite3_mutex_enter should have been called (enter path)");
    ok = false;
  }
  if(!g_mutex_leave_called){
    log_failure(test_name, "sqlite3_mutex_leave should have been called (leave path)");
    ok = false;
  }

  return ok;
}

// Test 3: API armor path when safety check fails should bail out before mutex
static bool test_api_armor_safety_false_no_changes(){
  reset_test_state();
  std::string test_name = "test_api_armor_safety_false_no_changes";

  // Simulate API armor failure
  g_safety_ok_for_test = 0;

  sqlite3 db;
  db.mutex = static_cast<void*>(&db);
  db.xProgress = nullptr;
  db.nProgressOps = 0;
  db.pProgressArg = nullptr;

  // Pre-state snapshot
  int pre_xProgress = reinterpret_cast<int>(db.xProgress);
  unsigned int pre_nProgressOps = db.nProgressOps;
  void *pre_pProgressArg = db.pProgressArg;

  // Call focal method with nOps > 0 to exercise the early return
  sqlite3_progress_handler(&db, 5, dummy_progress, reinterpret_cast<void*>(0x1234));

  // Assertions: state should be unchanged due to early return
  bool ok = true;
  if(db.xProgress != reinterpret_cast<int*>(pre_xProgress)){
    // Cast back for comparison convenience in error output
  }
  if(db.xProgress != (void*)pre_xProgress){
    log_failure(test_name, "db.xProgress should remain unchanged after safety check failure");
    ok = false;
  }
  if(db.nProgressOps != pre_nProgressOps){
    log_failure(test_name, "db.nProgressOps should remain unchanged after safety check failure");
    ok = false;
  }
  if(db.pProgressArg != pre_pProgressArg){
    log_failure(test_name, "db.pProgressArg should remain unchanged after safety check failure");
    ok = false;
  }

  // And mutex functions should not have been invoked
  if(g_mutex_enter_called){
    log_failure(test_name, "sqlite3_mutex_enter should NOT have been called when safety check fails");
    ok = false;
  }
  if(g_mutex_leave_called){
    log_failure(test_name, "sqlite3_mutex_leave should NOT have been called when safety check fails");
    ok = false;
  }

  // Restore safety for potential subsequent tests
  g_safety_ok_for_test = 1;

  return ok;
}

// Test 4: Reaffirm that when safety check passes, API armor path behaves as in Test 1
static bool test_api_armor_safety_true_behaves_like_positive(){
  reset_test_state();
  std::string test_name = "test_api_armor_safety_true_behaves_like_positive";

  g_safety_ok_for_test = 1;

  sqlite3 db;
  db.mutex = static_cast<void*>(&db);
  db.xProgress = nullptr;
  db.nProgressOps = 0;
  db.pProgressArg = nullptr;

  sqlite3_progress_handler(&db, 2, dummy_progress, reinterpret_cast<void*>(0xABCD));

  bool ok = true;
  if(db.xProgress != dummy_progress){
    log_failure(test_name, "db.xProgress should be set when safety check passes and nOps>0");
    ok = false;
  }
  if(db.nProgressOps != 2){
    log_failure(test_name, "db.nProgressOps should reflect provided nOps when safety check passes");
    ok = false;
  }
  if(db.pProgressArg != reinterpret_cast<void*>(0xABCD)){
    log_failure(test_name, "db.pProgressArg should reflect provided pArg when safety check passes");
    ok = false;
  }
  if(!g_mutex_enter_called){
    log_failure(test_name, "sqlite3_mutex_enter should have been called in safe path");
    ok = false;
  }
  if(!g_mutex_leave_called){
    log_failure(test_name, "sqlite3_mutex_leave should have been called in safe path");
    ok = false;
  }

  return ok;
}

// Runner: executes all tests and reports results
int main(){
  int total = 0;
  int failures = 0;

  std::cout << "Starting sqlite3_progress_handler unit tests (self-contained, no GTest)...\n";

  // Run tests
  total++;
  if(!test_positive_nOps_sets_fields()) { ++failures; std::cout << "[  ] " << "test_positive_nOps_sets_fields failed\n"; } else { std::cout << "[OK] " << "test_positive_nOps_sets_fields passed\n"; }

  total++;
  if(!test_zero_nOps_resets_fields()) { ++failures; std::cout << "[  ] " << "test_zero_nOps_resets_fields failed\n"; } else { std::cout << "[OK] " << "test_zero_nOps_resets_fields passed\n"; }

  total++;
  if(!test_api_armor_safety_false_no_changes()) { ++failures; std::cout << "[  ] " << "test_api_armor_safety_false_no_changes failed\n"; } else { std::cout << "[OK] " << "test_api_armor_safety_false_no_changes passed\n"; }

  total++;
  if(!test_api_armor_safety_true_behaves_like_positive()) { ++failures; std::cout << "[  ] " << "test_api_armor_safety_true_behaves_like_positive failed\n"; } else { std::cout << "[OK] " << "test_api_armor_safety_true_behaves_like_positive passed\n"; }

  std::cout << "Tests completed: " << (total - failures) << " passed, " << failures << " failed, out of " << total << " tests.\n";

  return failures ? 1 : 0;
}