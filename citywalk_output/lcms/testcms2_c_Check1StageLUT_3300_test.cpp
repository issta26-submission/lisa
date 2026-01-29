// test_check1stagelut.cpp
// A lightweight, non-GTest C++11 test suite for the focal method
// Check1StageLUT() in testcms2.c, plus its dependent components.
// The tests are designed to run in a normal C++ program (no GTest).

#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>


// Bring in the C declarations from the project under test.
// The header is expected to declare all necessary functions used by the tests,
// including cmsPipelineAlloc, cmsPipelineFree, AddIdentityMatrix, CheckFullLUT,
// DbgThread, and the focal function Check1StageLUT.
extern "C" {
}

// Simple non-terminating assertion helpers.
// They log failures but do not abort the test execution, enabling coverage.

// Global storage for test failure messages (for potential extended reporting)
static std::vector<std::string> g_testFailures;

// Record a failed expectation with a message
static void RecordFailure(const std::string& msg) {
    g_testFailures.push_back(msg);
    std::cerr << "TEST FAIL: " << msg << std::endl;
}

// Expectation helpers (non-terminating)
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { RecordFailure(msg); } \
} while(false)

#define EXPECT_EQ(expected, actual, msg) do { \
    if((expected) != (actual)) { \
        std::string s = std::string(msg) + " (expected: " + std::to_string(expected) + ", actual: " + std::to_string(actual) + ")"; \
        RecordFailure(s); \
    } \
} while(false)

// Convenience macro to convert a test to a bool with automatic reporting
#define TEST_EXISTS(cond, msg) do { \
    if(!(cond)) { RecordFailure(msg); } \
} while(false)

// Test 1: Basic sanity - Check1StageLUT should return a success value (non-zero, typically 1)
static bool Test_Check1StageLUT_Returns_Success(void)
{
    // Call the focal function directly
    cmsInt32Number result = Check1StageLUT();
    bool ok = (result != 0);
    EXPECT_TRUE(ok, "Check1StageLUT() did not return a success code (non-zero expected).");
    return ok;
}

// Test 2: Validate the dependencies used by Check1StageLUT (identity matrix path)
static bool Test_AddIdentity_and_CheckFullLUT_identity(void)
{
    // Allocate a 3x3 LUT, apply identity, run full LUT check with expected 1 stage
    cmsPipeline* lut = cmsPipelineAlloc(DbgThread(), 3, 3);
    if (lut == nullptr) {
        RecordFailure("Test_AddIdentity_and_CheckFullLUT_identity: cmsPipelineAlloc returned null.");
        return false;
    }

    AddIdentityMatrix(lut);

    cmsInt32Number res = CheckFullLUT(lut, 1);

    cmsPipelineFree(DbgThread(), lut);

    bool ok = (res == 1);
    EXPECT_TRUE(ok, "CheckFullLUT(identity, 1) should return 1 (success).");
    return ok;
}

// Test 3: Negative path for the same dependencies (wrong expected stage should fail)
// This ensures we cover the false branch where the expected stage count does not match.
static bool Test_AddIdentity_and_CheckFullLUT_wrong_expected(void)
{
    cmsPipeline* lut = cmsPipelineAlloc(DbgThread(), 3, 3);
    if (lut == nullptr) {
        RecordFailure("Test_AddIdentity_and_CheckFullLUT_wrong_expected: cmsPipelineAlloc returned null.");
        return false;
    }

    AddIdentityMatrix(lut);

    cmsInt32Number res = CheckFullLUT(lut, 2); // Deliberately using an incorrect expected stage
    cmsPipelineFree(DbgThread(), lut);

    bool ok = (res != 1); // Expect non-success when the expected stage is incorrect
    EXPECT_TRUE(ok, "CheckFullLUT(identity, 2) should not return 1 (expected fail).");
    return ok;
}

// Optional: Re-run Check1StageLUT multiple times to exercise repeated usage and ensure no crash
static bool Test_Check1StageLUT_RepeatedInvocation(void)
{
    cmsInt32Number r1 = Check1StageLUT();
    cmsInt32Number r2 = Check1StageLUT();
    // Both invocations should succeed independently
    bool ok = (r1 != 0) && (r2 != 0);
    EXPECT_TRUE(ok, "Repeated Check1StageLUT() invocations should both succeed.");
    return ok;
}

// Driver: run all tests and report summary
int main(int argc, char* argv[])
{
    (void)argc; (void)argv; // silence unused-parameter warnings

    int total = 0;
    int passed = 0;

    auto runTest = [&](const char* name, bool (*fn)()) {
        total++;
        bool ok = fn();
        if (ok) {
            passed++;
            std::cout << "[OK]   " << name << std::endl;
        } else {
            std::cout << "[FAILED] " << name << std::endl;
        }
        return ok;
    };

    // Execute tests
    runTest("Test_Check1StageLUT_Returns_Success", Test_Check1StageLUT_Returns_Success);
    runTest("Test_AddIdentity_and_CheckFullLUT_identity", Test_AddIdentity_and_CheckFullLUT_identity);
    runTest("Test_AddIdentity_and_CheckFullLUT_wrong_expected", Test_AddIdentity_and_CheckFullLUT_wrong_expected);
    runTest("Test_Check1StageLUT_RepeatedInvocation", Test_Check1StageLUT_RepeatedInvocation);

    // Summary
    std::cout << "\nTest Summary: " << passed << " / " << total << " tests passed." << std::endl;

    // Return non-zero if any test failed (conventional CI behavior)
    return (passed == total) ? 0 : 1;
}