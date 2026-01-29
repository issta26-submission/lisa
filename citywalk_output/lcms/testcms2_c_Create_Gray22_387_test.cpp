// Test suite for cmsHPROFILE Create_Gray22(void)
// Note: This test suite is written for a C++11 project without Google Test.
// It uses a small, self-contained assertion mechanism and calls into the
// focal function directly. The tests are designed to maximize code coverage
// by exercising the success path and repeated invocation.
//
// How to run (conceptual):
// - Compile this test file alongside the project providing testcms2.h and its
//   implementations (including Create_Gray22 from the FOCAL_METHOD).
// - Link against the library that implements cmsCreateGrayProfileTHR, cmsBuildGamma, etc.
// - Run the resulting executable; it will print test results to stdout.

#include <iostream>
#include <string>
#include <testcms2.h>


// Import the C test harness and the focal function from the project under test.
extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_test_failures = 0;

// Report a non-fatal test failure with location info
static void TestFail(const char* message, const char* file, int line) {
    std::cerr << "Test Failure: " << message << " (" << file << ":" << line << ")\n";
    ++g_test_failures;
}

// Convenience assertions
#define EXPECT_TRUE(cond) \
    do { if(!(cond)) TestFail(("Expected true: " #cond), __FILE__, __LINE__); } while(0)

#define EXPECT_NOT_NULL(ptr) \
    do { if((ptr) == nullptr) TestFail(("Expected non-null: " #ptr), __FILE__, __LINE__); } while(0)

//////////////////////////////
// Test 1: Ensure Create_Gray22 returns a non-null profile.
// Rationale: Validates the success path where the ToneCurve is built and the
// Gray profile is created. This covers the main control flow and resource cleanup.
static void Test_CreateGray22_ReturnsNonNull_Profile(void)
{
    // Act
    cmsHPROFILE profile = Create_Gray22();

    // Assert
    EXPECT_NOT_NULL(profile);
    if (profile != nullptr) {
        // Cleanup to avoid leaking in subsequent tests
        cmsCloseProfile(profile);
    }
}

// Test 2: Invoke Create_Gray22 multiple times to exercise repeated usage.
// Rationale: Improves coverage of the function under multiple invocations and
// ensures resources are consistently managed across calls.
static void Test_CreateGray22_RepeatedInvocations(void)
{
    const int iterations = 5;
    for (int i = 0; i < iterations; ++i) {
        cmsHPROFILE profile = Create_Gray22();
        EXPECT_NOT_NULL(profile);
        if (profile != nullptr) {
            cmsCloseProfile(profile);
        }
    }
}

// Test 3: Stress test Create_Gray22 with several quick successive calls.
// Rationale: Enhances branch coverage and memory-management paths by rapid usage.
static void Test_CreateGray22_Stress(void)
{
    const int iterations = 10;
    for (int i = 0; i < iterations; ++i) {
        cmsHPROFILE profile = Create_Gray22();
        EXPECT_NOT_NULL(profile);
        if (profile != nullptr) cmsCloseProfile(profile);
    }
}

// Entry point for running all tests.
// Uses non-terminating assertions so that all tests are exercised even if some fail.
static void RunAllTests(void)
{
    std::cout << "Running Create_Gray22 unit tests...\n";

    Test_CreateGray22_ReturnsNonNull_Profile();
    Test_CreateGray22_RepeatedInvocations();
    Test_CreateGray22_Stress();

    if (g_test_failures == 0) {
        std::cout << "All tests PASSED for Create_Gray22.\n";
    } else {
        std::cout << "Create_Gray22 tests completed with "
                  << g_test_failures << " failure(s).\n";
    }
}

int main(int argc, char* argv[])
{
    // Execute the tests. In a larger suite this could parse args to select tests.
    RunAllTests();
    // Return non-zero if any test failed (helps integrate with CI in a minimal way)
    return (g_test_failures == 0) ? 0 : 1;
}