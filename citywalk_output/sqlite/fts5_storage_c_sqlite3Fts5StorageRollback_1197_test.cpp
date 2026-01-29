// Minimal C++11 test harness for sqlite3Fts5StorageRollback
// This test suite focuses on the focal method:
//   int sqlite3Fts5StorageRollback(Fts5Storage *p)
// which is defined in fts5_storage.c and relies on internal state
// (p->bTotalsValid) and a call to sqlite3Fts5IndexRollback(p->pIndex).
//
// Notes:
// - We import necessary dependencies from the project headers (fts5Int.h).
// - We test only the observable side effects of the function under test:
//   1) bTotalsValid is reset to 0.
//   2) The return value matches the return value of the underlying
//      sqlite3Fts5IndexRollback(pIndex) call for the given index (we compute
//      an expected value by calling sqlite3Fts5IndexRollback in the test as well).
// - We avoid terminating assertions to maximize code coverage by collecting
//   failures in a global container and reporting them from main().
// - Tests are executed by manually invoking test functions from main().

#include <functional>
#include <vector>
#include <fts5Int.h>
#include <string>
#include <iostream>


// Import project dependencies.
// The exact path may vary depending on the build system.
// Ensure this header defines Fts5Storage, Fts5Index, and the signatures
// for sqlite3Fts5StorageRollback and sqlite3Fts5IndexRollback.
extern "C" {
}

// Global collector for test failures (non-terminating assertions)
static std::vector<std::string> gFailures;

// Simple non-terminating assert macro: records a failure message if condition is false
#define ASSERT(cond, msg) do { if(!(cond)) { gFailures.push_back(msg); } } while(0)

// Helper to print a single test result summary
static void printTestResult(const std::string& testName, bool passed) {
    if(passed) {
        std::cout << "[PASS] " << testName << "\n";
    } else {
        std::cout << "[FAIL] " << testName << "\n";
    }
}

// Test 1: Verify that sqlite3Fts5StorageRollback resets bTotalsValid to 0
//        and returns the same value as sqlite3Fts5IndexRollback(pIndex).
//        We use a NULL index to keep the test robust against internal index state.
//        This helps to cover the straightforward path of the focal method.
static void test_sqlite3Fts5StorageRollback_setsTotalsValid_zero_with_null_index() {
    // Prepare a storage object with non-zero bTotalsValid and NULL pIndex
    Fts5Storage s;
    s.bTotalsValid = 5;
    s.pIndex = NULL;

    // Compute the expected return value by directly calling the index rollback
    // with the same index pointer the storage would pass.
    // If NULL is not supported, this test may fail depending on the
    // implementation, but it reflects the relationship we want to verify.
    int expected = sqlite3Fts5IndexRollback(s.pIndex);

    // Call the focal method
    int ret = sqlite3Fts5StorageRollback(&s);

    // Verify side effect and return value
    ASSERT(s.bTotalsValid == 0, "bTotalsValid should be reset to 0 by sqlite3Fts5StorageRollback");
    ASSERT(ret == expected, "Return value should equal sqlite3Fts5IndexRollback(pIndex) result");
}

// Test 2: Verify that sqlite3Fts5StorageRollback handles the case where bTotalsValid
//        is already 0. The function should leave it at 0 and still propagate
//        the underlying index rollback return value.
static void test_sqlite3Fts5StorageRollback_already_zero_totals() {
    Fts5Storage s;
    s.bTotalsValid = 0;
    s.pIndex = NULL;

    int expected = sqlite3Fts5IndexRollback(s.pIndex);
    int ret = sqlite3Fts5StorageRollback(&s);

    ASSERT(s.bTotalsValid == 0, "bTotalsValid should remain 0 when already 0");
    ASSERT(ret == expected, "Return value should equal sqlite3Fts5IndexRollback(pIndex) result");
}

// Entry point
int main() {
    // Clear global failure list
    gFailures.clear();

    // Run tests
    test_sqlite3Fts5StorageRollback_setsTotalsValid_zero_with_null_index();
    test_sqlite3Fts5StorageRollback_already_zero_totals();

    // Report results
    if(gFailures.empty()) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << gFailures.size() << " failure(s) detected:\n";
        for(size_t i = 0; i < gFailures.size(); ++i) {
            std::cout << "  [" << i+1 << "] " << gFailures[i] << "\n";
        }
        return 1;
    }
}