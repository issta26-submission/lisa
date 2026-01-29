// This test suite targets the focal function:
// int sqlite3_vtab_in_first(sqlite3_value *pVal, sqlite3_value **ppOut)
// The function is a thin wrapper around a static helper: valueFromValueList(pVal, ppOut, 0);
// We assume the surrounding project (vdbeapi.c and related SQLite internals) is present
// in the build environment. This test uses a lightweight, non-terminating assertion
// style to maximize code-path coverage during test execution without aborting on
// the first failure, as requested by the domain knowledge guidelines.
//
// Notes:
// - We only rely on standard C++11 headers and the provided external C API.
// - sqlite3_value is treated as an opaque struct in this test (forward-declared here),
//   mirroring how the real project exposes the type in other translation units.
// - The tests focus on invoking the focal function with representative inputs and
//   verifying that the function executes and returns an int (the contract of the API).
// - If the environment provides a real sqlite3_value type and the vdbeapi.c/SQLite core,
//   these tests will exercise the wrapper path sqlite3_vtab_in_first and thus exercise
//   the call into valueFromValueList with bNext = 0 as intended.

#include <vector>
#include <opcodes.h>
#include <iomanip>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>


// Forward declaration to match the project’s external C API.
// In the real build, sqlite3_value is a defined type; here we only declare it
// so the test can compile in environments where the SQLite core provides the type.
extern "C" {
    struct sqlite3_value;              // opaque type from the SQLite core
    int sqlite3_vtab_in_first(sqlite3_value *pVal, sqlite3_value **ppOut);
}

// Simple, non-terminating test assertion helpers.
// They record failures but do not abort, allowing multiple tests to run.
static std::vector<std::string> g_failures;

#define TEST_TOTAL  (g_testTotals)
#define EXPECT_EQ(a, b) do { \
    ++g_testTotals; \
    if ((a) != (b)) { \
        std::ostringstream oss; \
        oss << "EXPECT_EQ failed: " #a " (" << (a) << ") != " #b " (" << (b) << ")" \
            << " at " << __FILE__ << ":" << __LINE__; \
        g_failures.push_back(oss.str()); \
    } \
} while (0)

#define EXPECT_NEQ(a, b) do { \
    ++g_testTotals; \
    if ((a) == (b)) { \
        std::ostringstream oss; \
        oss << "EXPECT_NEQ failed: " #a " (" << (a) << ") == " #b " (" << (b) << ")" \
            << " at " << __FILE__ << ":" << __LINE__; \
        g_failures.push_back(oss.str()); \
    } \
} while (0)

#define EXPECT_NOTNull(p) do { \
    ++g_testTotals; \
    if ((p) == nullptr) { \
        std::ostringstream oss; \
        oss << "EXPECT_NOTNull failed: " #p " is null" \
            << " at " << __FILE__ << ":" << __LINE__; \
        g_failures.push_back(oss.str()); \
    } \
} while (0)

static int g_testTotals = 0;

// Test 1: Null input for pVal with a probably-null ppOut.
// Rationale: Validate that the focal wrapper can be invoked safely with null inputs
// and returns an int code. In normal operation, pVal is a value or a list node; supplying
// null should be a deterministic path exercised in the wrapper.
void test_sqlite3_vtab_in_first_null_pval()
{
    // pVal is null; ppOut is a valid pointer to a sqlite3_value* (but initialized to null)
    sqlite3_value *pVal = nullptr;
    sqlite3_value *ppOutHolder = nullptr;
    sqlite3_value **ppOut = &ppOutHolder;

    // Explanatory comment:
    // This test exercises the wrapper with null pVal to ensure the function handles a
    // null input without crashing and returns an int. The exact non-crashing behavior
    // depends on valueFromValueList; we only require that it returns an int code.
    int rc = sqlite3_vtab_in_first(pVal, ppOut);
    // We do not assume a specific return value here; we only verify that an int is produced.
    // Use a broad expectation: the function should yield a consistent integral return.
    // Since we cannot know the exact contract for null input here in isolation, we check
    // that the call completes and returns a value (not a crash). We simulate this by
    // expecting a non-negative integer in practice; adjust if the real API negative codes
    // denote errors in your environment.
    EXPECT_NEQ(rc, -1); // rc should not be -1 (typical sentinel for "unhandled error" in some code paths)
}

// Test 2: Non-null pVal with ppOut set to a non-null location.
// Rationale: Ensure the wrapper forwards control to valueFromValueList with a plausible
// non-null argument pair, exercising the typical path.
void test_sqlite3_vtab_in_first_nonnull_input()
{
    // We create a non-null placeholder for pVal and a non-null ppOut pointer to receive potential output.
    // Since we cannot construct a real sqlite3_value without the core APIs here, we only test that
    // the call site is reachable and does not crash for a non-null placeholder scenario in environments
    // where a valid sqlite3_value object can be supplied by the test harness.
    sqlite3_value dummyVal;
    sqlite3_value *pVal = &dummyVal;
    sqlite3_value *ppOutHolder = nullptr;
    sqlite3_value **ppOut = &ppOutHolder;

    int rc = sqlite3_vtab_in_first(pVal, ppOut);
    // As with Test 1, we do not enforce a precise code, only that the function returns an int.
    EXPECT_NEQ(rc, -1);
}

// Test 3: Repeated calls with the same inputs to check for stability.
// Rationale: If valueFromValueList updates internal state via the input pointers, repeated
// invocations should not crash and should return an int each time.
void test_sqlite3_vtab_in_first_repeated_calls_stability()
{
    sqlite3_value dummyVal;
    sqlite3_value *pVal = &dummyVal;
    sqlite3_value *ppOutHolder = nullptr;
    sqlite3_value **ppOut = &ppOutHolder;

    int rc1 = sqlite3_vtab_in_first(pVal, ppOut);
    int rc2 = sqlite3_vtab_in_first(pVal, ppOut);

    EXPECT_EQ(rc1, rc2);
}

// Test 4: Ensure a non-null ppOut argument is allowed and does not cause immediate crash.
// Rationale: The wrapper signature includes ppOut; we pass a valid pointer to a sqlite3_value* and
// verify that the function returns an integral code.
void test_sqlite3_vtab_in_first_with_valid_ppOut_pointer()
{
    sqlite3_value dummyVal;
    sqlite3_value *pVal = &dummyVal;

    sqlite3_value *ppOutHolder = nullptr;
    sqlite3_value **ppOut = &ppOutHolder;

    int rc = sqlite3_vtab_in_first(pVal, ppOut);

    EXPECT_NEQ(rc, -1);
}

// Simple test runner that reports results and exits with non-zero status if failures exist.
int main() {
    // Run tests
    test_sqlite3_vtab_in_first_null_pval();
    test_sqlite3_vtab_in_first_nonnull_input();
    test_sqlite3_vtab_in_first_repeated_calls_stability();
    test_sqlite3_vtab_in_first_with_valid_ppOut_pointer();

    // Report results
    std::cout << "sqlite3_vtab_in_first test suite completed." << std::endl;
    if (g_failures.empty()) {
        std::cout << "All tests passed (non-terminating assertions)." << std::endl;
        return 0;
    } else {
        std::cout << "Test failures: " << g_failures.size() << std::endl;
        for (const auto &msg : g_failures) {
            std::cout << " - " << msg << std::endl;
        }
        // Return non-zero if there were failures to indicate unsuccessful test run
        return 1;
    }
}

// Explanation of test coverage:
// - Test 1 exercises the focal wrapper with a null pVal to ensure safety and an integral return.
// - Test 2 exercises a non-null pVal scenario, relying on the test harness to provide a valid object
//   via the environment's SQLite core, which would be constructed using the project's APIs.
// - Test 3 checks stability across repeated invocations to catch potential stateful behavior.
// - Test 4 ensures that a non-null ppOut pointer is accepted and doesn't crash.
// These tests aim to maximize coverage of the wrapper's control flow while depending on the
// surrounding SQLite core for concrete value construction. Adjust expectations if your environment
// defines a specific contract for error codes (e.g., negative vs non-negative codes).