/*
  This file provides a small C++11 test suite for the focal method:

    sqlite3_mutex *sqlite3Pcache1Mutex(void){
      return pcache1.mutex;
    }

  Step 1: Program Understanding
  - The function simply returns the mutex member from the internal static
    pcache1 object. pcache1 is a file-scope (static) object, so its fields
    are not directly accessible from test code.
  - Therefore, we test observable behavior only: the function should return a
    non-null sqlite3_mutex pointer and successive calls should yield the same
    pointer (i.e., the same mutex object is returned).

  Step 2: Unit Test Generation
  - We implement two tests:
    1) Non-null return value.
    2) Consistency/identity across multiple calls.

  Step 3: Test Case Refinement
  - We provide a lightweight, non-terminating test harness (no GTest required),
    using plain C++ standard library facilities. Static members remain unmocked
    and private implementation details are not touched directly.
  - We rely only on the provided API (via sqliteInt.h) and do not instantiate or
    manipulate internal static state beyond what the API exposes.

  Note: The test relies on the project providing a proper sqlite3Pcache1Mutex()
  implementation linked in. It does not attempt to access internal static state
  (pcache1) directly due to its internal linkage.
*/

#include <iostream>
#include <sqliteInt.h>
#include <string>


extern "C" {
}

// Simple non-terminating test harness
static int g_testCount = 0;
static int g_failCount = 0;

static void logResult(bool pass, const std::string &name) {
    ++g_testCount;
    if (!pass) ++g_failCount;
    std::cout << (pass ? "[OK] " : "[FAIL] ") << name << std::endl;
}

/*
  Test 1: sqlite3Pcache1Mutex should return a non-null pointer.
  Rationale:
  - The focal function exposes a mutex used by the page cache; in a sane runtime
    environment, this should not be null. This test verifies the observable
    contract of the API without relying on internal static state.
*/
static bool test_sqlite3Pcache1Mutex_nonNull() {
    sqlite3_mutex *m = sqlite3Pcache1Mutex();
    return m != nullptr;
}

/*
  Test 2: sqlite3Pcache1Mutex should consistently return the same pointer
  across multiple invocations.
  Rationale:
  - If the internal pcache1.mutex is a single global mutex, repeated calls to
    sqlite3Pcache1Mutex() should yield the same pointer. This checks stability
    of the observable API.
*/
static bool test_sqlite3Pcache1Mutex_consistency() {
    sqlite3_mutex *m1 = sqlite3Pcache1Mutex();
    sqlite3_mutex *m2 = sqlite3Pcache1Mutex();
    if (m1 == nullptr || m2 == nullptr) return false;
    return m1 == m2;
}

int main() {
    // Step 2: Unit Test Generation - execute tests
    logResult(test_sqlite3Pcache1Mutex_nonNull(),
              "sqlite3Pcache1Mutex should return a non-null pointer");

    logResult(test_sqlite3Pcache1Mutex_consistency(),
              "sqlite3Pcache1Mutex should be stable across multiple calls");

    // Step 3: Test Case Refinement - summary
    std::cout << "Total tests: " << g_testCount
              << ", Failures: " << g_failCount << std::endl;

    return g_failCount ? 1 : 0;
}