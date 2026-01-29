// This is a lightweight C++11 test harness for the focal function CheckFixedPoint15_16
// and its dependency TestSingleFixed15_16 as provided in testcms2.c/testcms2.h.
// The test suite is designed to compile without Google Test and to run from main().
// It uses simple EXPECT-like assertions that print diagnostic messages but do not terminate
// on failure, enabling multiple tests to run for higher coverage.

#include <iostream>
#include <cmath>
#include <testcms2.h>
#include <iomanip>


// Include the provided CMS test headers which declare the target functions.
// Assumes testcms2.h is available and defines cmsInt32Number, cmsFloat64Number, etc.
extern "C" {
}

// Function declarations for compatibility with C linkage.
// If testcms2.h already provides these as extern "C" prototypes, this is harmless.
extern "C" cmsInt32Number TestSingleFixed15_16(cmsFloat64Number d);
extern "C" cmsInt32Number CheckFixedPoint15_16(void);

// Simple test harness utilities
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_PASS() do { ++g_total_tests; } while(0)
#define TEST_FAIL(msg) do { ++g_total_tests; ++g_failed_tests; std::cerr << "TEST FAILED: " << (msg) << std::endl; } while(0)

#define EXPECT_TRUE(cond, msg) do { if(!(cond)) { TEST_FAIL(msg); } else { TEST_PASS(); } } while(0)

// Test 1: Validate that the public CheckFixedPoint15_16 reports success (returns non-zero).
// This exercises the entire chain of fixed-point checks as implemented in the focal method.
// Rationale: ensures the "happy path" where all embedded tests pass.
bool Test_CheckFixedPoint15_16_AllPass(void)
{
    // Expect the function to return 1 on success (as in the provided code).
    cmsInt32Number res = CheckFixedPoint15_16();
    bool ok = (res == 1);
    if (!ok) {
        std::cerr << "CheckFixedPoint15_16() returned " << res << ", expected 1 (success).\n";
        TEST_FAIL("CheckFixedPoint15_16 did not return 1 on the expected input set.");
        // Count as test done
        TEST_PASS();
    } else {
        TEST_PASS();
    }
    return ok;
}

// Test 2: Validate that TestSingleFixed15_16 returns non-zero for a representative set
// of positive inputs used by CheckFixedPoint15_16. This covers the true-branch execution
// path for these inputs and indirectly exercises conversion logic to Fixed15_16 representation.
bool Test_TestSingleFixed15_16_PositiveInputs(void)
{
    bool all_ok = true;

    // Cover positive representative values
    cmsFloat64Number positives[] = {
        1.0,
        2.0,
        1.23456,
        0.99999,
        0.1234567890123456789099999
    };

    for (size_t i = 0; i < sizeof(positives)/sizeof(positives[0]); ++i) {
        cmsInt32Number r = TestSingleFixed15_16(positives[i]);
        if (r == 0) {
            std::cerr << "TestSingleFixed15_16(" << std::setprecision(20) << positives[i]
                      << ") returned 0 (unexpected failure on positive input).\n";
            TEST_FAIL("TestSingleFixed15_16 failed on positive input.");
            all_ok = false;
        } else {
            // Pass for this input
            TEST_PASS();
        }
    }

    // Aggregate result
    return all_ok;
}

// Test 3: Validate that TestSingleFixed15_16 returns non-zero for negative inputs.
// This ensures proper handling of signed values within the fixed-point conversion routine.
bool Test_TestSingleFixed15_16_NegativeInputs(void)
{
    bool all_ok = true;

    cmsFloat64Number negatives[] = {
        -1.0,
        -2.0,
        -1.23456,
        -1.1234567890123456789099999
    };

    for (size_t i = 0; i < sizeof(negatives)/sizeof(negatives[0]); ++i) {
        cmsInt32Number r = TestSingleFixed15_16(negatives[i]);
        if (r == 0) {
            std::cerr << "TestSingleFixed15_16(" << std::setprecision(20) << negatives[i]
                      << ") returned 0 (unexpected failure on negative input).\n";
            TEST_FAIL("TestSingleFixed15_16 failed on negative input.");
            all_ok = false;
        } else {
            // Pass for this input
            TEST_PASS();
        }
    }

    return all_ok;
}

// Test 4: Validate that TestSingleFixed15_16 handles boundary-like magnitude values
// used in the original test suite (extremely large positive/negative values).
bool Test_TestSingleFixed15_16_MaxMagnitudeInputs(void)
{
    bool all_ok = true;

    cmsFloat64Number magnitudes[] = {
        +32767.1234567890123456789099999,
        -32767.1234567890123456789099999
    };

    for (size_t i = 0; i < sizeof(magnitudes)/sizeof(magnitudes[0]); ++i) {
        cmsInt32Number r = TestSingleFixed15_16(magnitudes[i]);
        if (r == 0) {
            std::cerr << "TestSingleFixed15_16(" << std::setprecision(20) << magnitudes[i]
                      << ") returned 0 (unexpected failure on magnitude input).\n";
            TEST_FAIL("TestSingleFixed15_16 failed on magnitude input.");
            all_ok = false;
        } else {
            // Pass for this input
            TEST_PASS();
        }
    }

    return all_ok;
}

// Main driver: run all tests and report overall success/failure.
// If GTest were allowed, we would use subtests, but here we rely on a simple main-based harness.
int main(int argc, char* argv[])
{
    // Optional: keep I/O fast enough for test output
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    // Reset counters
    g_total_tests = 0;
    g_failed_tests = 0;

    bool all_ok = true;

    // Run test scenarios with explanatory comments
    std::cout << "Starting tests for CheckFixedPoint15_16 and TestSingleFixed15_16...\n";

    // Test 1: happy-path overall function
    std::cout << "Test 1: CheckFixedPoint15_16 should return 1 on the expected input set.\n";
    all_ok = Test_CheckFixedPoint15_16_AllPass();
    if (!all_ok) {
        std::cerr << "Test 1 failed. CheckFixedPoint15_16 did not report success.\n";
    }

    // Test 2: positives
    std::cout << "Test 2: TestSingleFixed15_16 on positive inputs should succeed.\n";
    if (!Test_TestSingleFixed15_16_PositiveInputs()) {
        std::cerr << "Test 2 failed. Some positive inputs failed in TestSingleFixed15_16.\n";
        all_ok = false;
    }

    // Test 3: negatives
    std::cout << "Test 3: TestSingleFixed15_16 on negative inputs should succeed.\n";
    if (!Test_TestSingleFixed15_16_NegativeInputs()) {
        std::cerr << "Test 3 failed. Some negative inputs failed in TestSingleFixed15_16.\n";
        all_ok = false;
    }

    // Test 4: max magnitude inputs
    std::cout << "Test 4: TestSingleFixed15_16 on maximum magnitude inputs should succeed.\n";
    if (!Test_TestSingleFixed15_16_MaxMagnitudeInputs()) {
        std::cerr << "Test 4 failed. Max magnitude inputs failed in TestSingleFixed15_16.\n";
        all_ok = false;
    }

    // Final summary
    std::cout << "\nTest run complete. Total: " << g_total_tests
              << ", Passed: " << (g_total_tests - g_failed_tests)
              << ", Failed: " << g_failed_tests << "\n";

    return (g_failed_tests == 0) ? 0 : 1;
}