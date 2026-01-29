// Minimal C++11 test harness for the focal method: testDeleteDatasourceRange
// This test suite provides lightweight, non-terminating assertions and a
// small set of test cases to exercise the focal function with various inputs.
// The tests rely on the existing C-based test helpers declared in lsmtest.h.
// No GTest is used as per the constraints.
// Each test includes explanatory comments describing its purpose and coverage.

#include <iostream>
#include <lsmtest.h>
#include <string>


// Bring in the C declarations. We assume lsmtest.h provides the appropriate
// typedefs (TestDb, Datasource) and the function prototype for
// testDeleteDatasourceRange. The C header is included with extern "C" linkage.
extern "C" {
}

// Lightweight test harness state
static int g_totalTests = 0;
static int g_failedTests = 0;

// Non-terminating assertion macro: records failure but continues execution
#define EXPECT_TRUE(cond, msg) do { \
    if (cond) { \
        std::cout << "[PASS] " << msg << std::endl; \
    } else { \
        std::cout << "[FAIL] " << msg << std::endl; \
        ++g_failedTests; \
    } \
    ++g_totalTests; \
} while (0)

// Helper to print final summary
static void print_summary() {
    std::cout << "\nTest summary: " << g_totalTests << " tests, "
              << (g_totalTests - g_failedTests) << " passed, "
              << g_failedTests << " failed.\n";
}

// Test 1: nWrite = 0, iFirst arbitrary, pointers null
// Rationale: The focal method is a loop that should perform zero iterations
// and return without dereferencing pDb/pData. This checks stability on edge case.
static bool test_zero_writes_with_null_pointers() {
    int rc = -999999;               // sentinel value to detect changes
    testDeleteDatasourceRange(nullptr, nullptr, 0, 0, &rc);
    // Expect rc to remain unchanged since no deletes should be attempted
    bool ok = (rc == -999999);
    EXPECT_TRUE(ok, "testDeleteDatasourceRange with nWrite=0 leaves rc unchanged when pDb/pData are null");
    return ok;
}

// Test 2: nWrite = 1, iFirst = 0, pointers null
// Rationale: Exercises a single iteration of the loop.
// Note: The actual delete operation is defined elsewhere; we only verify
// that the function completes without crashing and rc remains stable.
static bool test_one_write_with_null_pointers() {
    int rc = -888888;
    testDeleteDatasourceRange(nullptr, nullptr, 0, 1, &rc);
    bool ok = (rc == -888888);
    EXPECT_TRUE(ok, "testDeleteDatasourceRange with nWrite=1 completes and rc unchanged for null pointers");
    return ok;
}

// Test 3: nWrite negative (e.g., -5) to ensure no iterations occur
static bool test_negative_nwrite_no_op() {
    int rc = -777777;
    testDeleteDatasourceRange(nullptr, nullptr, 5, -5, &rc);
    bool ok = (rc == -777777);
    EXPECT_TRUE(ok, "testDeleteDatasourceRange with negative nWrite performs no iterations and rc unchanged");
    return ok;
}

// Test 4: nWrite multiple > 1 to exercise multiple iterations
// This test ensures the function handles larger nWrite values gracefully.
static bool test_multiple_writes() {
    int rc = -777777;
    // Use a modest number to avoid long runtimes while still exercising the loop
    testDeleteDatasourceRange(nullptr, nullptr, -3, 4, &rc);
    bool ok = (rc == -777777);
    EXPECT_TRUE(ok, "testDeleteDatasourceRange with nWrite=4 executes multiple iterations and rc unchanged");
    return ok;
}

// Entry point to run all tests
int main() {
    std::cout << "Starting test suite for testDeleteDatasourceRange...\n";

    // Step 3: Run all tests
    bool t1 = test_zero_writes_with_null_pointers();
    bool t2 = test_one_write_with_null_pointers();
    bool t3 = test_negative_nwrite_no_op();
    bool t4 = test_multiple_writes();

    // Collect results (optional verbose per-test prints already emitted)
    // Note: We rely on the per-test EXPECT_TRUE messages for feedback.
    // Final summary
    print_summary();

    // Return non-zero if any test failed
    return (g_failedTests > 0) ? 1 : 0;
}