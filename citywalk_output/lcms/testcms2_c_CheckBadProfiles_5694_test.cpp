// Test harness for the focal method CheckBadProfiles (C/C++ interop scenario)
// This test suite is written in C++11 without using GTest. It directly calls
// the focal function and uses a lightweight, non-terminating assertion style
// to maximize code coverage.

// The test relies on the presence of the project test header that defines
// prototypes and the SimultaneousErrors global used by CheckBadProfiles.
#include <iostream>
#include <cstdint>
#include <testcms2.h>


// Include the project's test harness header which provides necessary
// declarations (including CheckBadProfiles and SimultaneousErrors).

// We assume that the project's header defines the proper prototype for
// CheckBadProfiles as cmsInt32Number CheckBadProfiles(void).
// If the typedef is different, this test will still compile given the header
// provides correct type aliases.

static int g_failures = 0;

// Lightweight non-terminating assertion macro.
// On failure, it increments a global counter but does not abort the test flow.
#define EXPECT_EQ(expected, actual, msg) do { \
    if ((expected) != (actual)) { \
        std::cerr << "FAIL: " << (msg) \
                  << " (expected " << (expected) \
                  << ", got " << (actual) << ")\n"; \
        ++g_failures; \
    } else { \
        std::cout << "."; \
    } \
} while(0)

/*
Test Case 1: SimultaneousErrors == 9
- Purpose: Validate the success path where all file/memory opens fail and the
  global SimultaneousErrors counter equals 9, causing CheckBadProfiles to return 1.
- Rationale: This exercises the final return path of the function.
*/
void Test_CheckBadProfiles_AllFails_WithSim9()
{
    // Set the global error counter to the value required for success path.
    // The actual Open calls are expected to fail (no real ICC files present)
    // in the test environment, ensuring the path continues to the final check.
    SimultaneousErrors = 9;

    cmsInt32Number result = CheckBadProfiles();

    // Expect success (return value 1)
    EXPECT_EQ(1, result, "CheckBadProfiles should return 1 when SimultaneousErrors == 9 and all opens fail");
}

/*
Test Case 2: SimultaneousErrors != 9 (e.g., 0)
- Purpose: Validate the failure path when the global SimultaneousErrors counter
  is not equal to 9. The function should return 0 without requiring a passing
  condition for all open attempts.
- Rationale: Exercises the early return due to SimultaneousErrors check.
*/
void Test_CheckBadProfiles_Not9_Returns0()
{
    SimultaneousErrors = 0;

    cmsInt32Number result = CheckBadProfiles();

    // Expect failure (return value 0)
    EXPECT_EQ(0, result, "CheckBadProfiles should return 0 when SimultaneousErrors != 9");
}

/*
Test Case 3: SimultaneousErrors negative (e.g., -1)
- Purpose: Validate an additional non-9 boundary condition to ensure correct
  handling of non-9 values (negative in this case).
- Rationale: Further exercises the predicate controlling the final return path.
*/
void Test_CheckBadProfiles_NegativeNot9_Returns0()
{
    SimultaneousErrors = -1;

    cmsInt32Number result = CheckBadProfiles();

    // Expect failure (return value 0)
    EXPECT_EQ(0, result, "CheckBadProfiles should return 0 when SimultaneousErrors is negative and != 9");
}

int main(int argc, char* argv[])
{
    (void)argc; (void)argv; // Unused, silence warnings

    std::cout << "Running CheckBadProfiles unit tests (C/C++ interop, no GTest):\n";

    // Run tests
    Test_CheckBadProfiles_AllFails_WithSim9();
    Test_CheckBadProfiles_Not9_Returns0();
    Test_CheckBadProfiles_NegativeNot9_Returns0();

    std::cout << "\n";

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0; // success
    } else {
        std::cout << g_failures << " TEST(S) FAILED\n";
        return 1; // failure
    }
}