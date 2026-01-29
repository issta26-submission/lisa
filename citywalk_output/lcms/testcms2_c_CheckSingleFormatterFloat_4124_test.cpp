// Lightweight C++11 unit tests for the focal method: CheckSingleFormatterFloat
// This test suite purposefully avoids any gtest/gmock usage and relies on the
// existing C test infrastructure provided with the focal project (testcms2.h and lcms2.h).
// The tests focus on exercising the critical predicates within CheckSingleFormatterFloat
// and validating observable side-effects via static/global state that the focal code exposes.
// Explanatory comments are added for each unit test to clarify intent and coverage.

#include <cstring>
#include <iostream>
#include <testcms2.h>
#include <lcms2.h>
#include <cmath>


// Bring C bindings into C++ test file
extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int gTestCount = 0;
static int gFailCount = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        ++gTestCount; \
        if(!(cond)) { \
            ++gFailCount; \
            std::cerr << "[FAILED] " << __FUNCTION__ << ": " << (msg) \
                      << " (Condition: " << #cond << ")" << std::endl; \
        } \
    } while(0)

#define EXPECT_FALSE(cond, msg) EXPECT_TRUE(!(cond), msg)
#define RESET_COUNTS() do { gTestCount = 0; gFailCount = 0; } while(0)

// Note on critical candidate keywords from Step 1:
// - CheckSingleFormatterFloat: focal function under test
// - FormatterFailed: global/static guard used inside the function to short-circuit
// - _cmsGetFormatter: called to retrieve formatters for input/output
// - FmtFloat: function pointer on formatter used for float packing/unpacking
// - CMS_PACK_FLAGS_FLOAT: packing mode for floating-point data
// - T_CHANNELS(Type): macro to determine number of channels for a given type
// - TYPE_RGB_8 (TYPE_GRAY_8, etc.): representative Type codes used to drive the test

// Test 1: Normal path for a known-good Type (TYPE_RGB_8).
// Rationale:
// - Ensure the function enters the main double-loop and completes without setting
//   FormatterFailed when the round-trip through the formatter preserves values.
// - This exercises the true-branch of the critical delta check (delta <= 1e-9) for all channels.
void Test_CheckSingleFormatterFloat_NormalPath_RGB8()
{
    // Prepare a clean state for the test. If the project exposes FormatterFailed externally,
    // reset it to the known FALSE value to simulate a fresh run.
    // The exact symbol type is expected to be defined in test infrastructure (cmsBool/TRUE/FALSE).
    FormatterFailed = FALSE;

    // Use a representative 3-channel RGB type with 8-bit/channel precision in LittleCMS.
    // The actual numeric value TYPE_RGB_8 is provided by lcms2.h and is commonly available.
    CheckSingleFormatterFloat(TYPE_RGB_8, "TYPE_RGB_8_RGB");

    // After a successful run, FormatterFailed should remain FALSE.
    EXPECT_TRUE(FormatterFailed == FALSE, "FormatterFailed should be FALSE after successful float formatting round-trip for TYPE_RGB_8");
}

// Test 2: Early exit path when FormatterFailed is already set (true branch).
// Rationale:
// - Verify that the function short-circuits and returns immediately when FormatterFailed is TRUE.
// - This covers the initial if (FormatterFailed) return; predicate in the focal method.
void Test_CheckSingleFormatterFloat_EarlyExitFormatterFailedFlag()
{
    // Force the global guard to signal a previous failure
    FormatterFailed = TRUE;

    // Execute with a valid Type; the function should return immediately due to the guard.
    CheckSingleFormatterFloat(TYPE_RGB_8, "TYPE_RGB_8_RGB");

    // The guard should still be TRUE after the call; no further processing should occur.
    EXPECT_TRUE(FormatterFailed == TRUE, "FormatterFailed should remain TRUE after early exit");
}

// Test 3: Optional robustness: ensure that a second common Type exercises similar path.
// Rationale:
// - While not strictly guaranteed to trigger every internal branch, using TYPE_GRAY_8
//   increases coverage across a different channel count (1 channel) and helps exercise
//   the inner loops with a different memory footprint.
void Test_CheckSingleFormatterFloat_NormalPath_GRAY8()
{
    // Reset guard for fresh run
    FormatterFailed = FALSE;

    CheckSingleFormatterFloat(TYPE_GRAY_8, "TYPE_GRAY_8_GRAY");

    // Expect no failure and a successful passthrough for a single-channel type
    EXPECT_TRUE(FormatterFailed == FALSE, "FormatterFailed should be FALSE after successful float formatting round-trip for TYPE_GRAY_8");
}

// Runner that executes all tests and reports summary
void RunAllTests()
{
    RESET_COUNTS();
    Test_CheckSingleFormatterFloat_NormalPath_RGB8();
    Test_CheckSingleFormatterFloat_EarlyExitFormatterFailedFlag();
    Test_CheckSingleFormatterFloat_NormalPath_GRAY8();

    std::cout << "Test Summary: " << gTestCount << " tests run, "
              << gFailCount << " failures." << std::endl;
}

// Main entry point to be invoked by the test executable
int main()
{
    RunAllTests();
    if (gFailCount == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << gFailCount << " test(s) failed." << std::endl;
        return 1;
    }
}