// Step 0 (contextual): This file provides a minimal, executable test for the focal method
// oPutsUtf8 located in console_io.c under the non-Windows-translation path (CIO_WIN_WC_XLATE not defined).
// It uses a lightweight in-process test harness (no GoogleTest; no terminators in assertions).
// The test invokes the function with a normal non-empty string and an empty string.
// Explanatory comments annotate the intent and expected behavior.
// Note: This test assumes the project is linkable with console_io.c and its dependencies.
// If building under CIO_WIN_WC_XLATE=1, prefer the separate test file in test_oPutsUtf8_with_xlate.cpp.

#include <iostream>
#include <cstring>


// Declare the focal function with C linkage for a C++ test harness.
extern "C" int oPutsUtf8(const char *z);

/*
Step 1: Candidate Keywords extracted from the focal function and its dependencies
- oPutsUtf8: main function under test
- z: input C string
- pfOut: output FILE* (emitted stream)
- PerStreamTags pst, PST_INITIALIZER: per-stream state
- getEmitStreamInfo: initializes/returns the emission stream
- CIO_WIN_WC_XLATE: compile-time flag that selects path
- pstReachesConsole: determines if the output goes to console
- conZstrEmit: console-specific UTF-8 emission
- fputs: standard C output function used when not on console
- strlen: length computation of the input string
- assert: runtime check asserting z != 0
- zSkipValidUtf8, oPutbUtf8, fGetsUtf8, etc.: additional helpers (class dependencies)
*/

// Lightweight test harness state
static int gFailures_no_xlate = 0;
static int gTotal_no_xlate = 0;

// Simple non-terminating expectation macro: logs failure but continues
#define EXPECT_EQ(a, b, msg) \
    do { \
        if ((a) != (b)) { \
            std::cerr << "[NO_XLATE] FAIL: " << (msg) << " | expected: " << (b) << " actual: " << (a) << "\n"; \
            ++gFailures_no_xlate; \
        } \
        ++gTotal_no_xlate; \
    } while (0)

// Test 1: Basic non-empty string - ensures the function returns a length value
void test_oPutsUtf8_basic_no_xlate() {
    const char *z = "Hello, UTF-8!";
    int ret = oPutsUtf8(z);
    EXPECT_EQ(ret, (int)strlen(z), "oPutsUtf8 should return the length of the input string when writing to non-XLATE path");
}

// Test 2: Empty string - ensures function handles zero-length input without crashing
void test_oPutsUtf8_empty_no_xlate() {
    const char *z = "";
    int ret = oPutsUtf8(z);
    EXPECT_EQ(ret, 0, "oPutsUtf8 should return 0 for an empty string");
}

// Helper runner
void run_all_no_xlate() {
    test_oPutsUtf8_basic_no_xlate();
    test_oPutsUtf8_empty_no_xlate();

    if (gTotal_no_xlate == gFailures_no_xlate) {
        // All tests passed
        std::cout << "[NO_XLATE] All tests passed (" << gTotal_no_xlate << " tests).\n";
    } else {
        std::cerr << "[NO_XLATE] Summary: " << gTotal_no_xlate - gFailures_no_xlate
                  << " passed, " << gFailures_no_xlate << " failed out of "
                  << gTotal_no_xlate << " tests.\n";
    }
}

// Entry point for this test file (no translator path)
// Run without the CIO_WIN_WC_XLATE macro defined in the compilation unit.
// Example build: g++ -DCIO_WIN_WC_XLATE=0 -I. -o test_no_xlate test_oPutsUtf8_no_xlate.cpp console_io.c
int main() {
    // Step 3: Test Case Refinement
    // We cover:
    // - true path: non-empty input (should return input length)
    // - edge path: empty string (ensures length 0 is returned)
    // We intentionally avoid NULL inputs to prevent triggering the assert(z != 0).
    run_all_no_xlate();
    return (gTotal_no_xlate == gFailures_no_xlate) ? 0 : 1;
}

// End of test_oPutsUtf8_no_xlate.cpp


// -------------------------------------------------------------------------------
// Step 2/3 Variant: Tests for CIO_WIN_WC_XLATE = 1 (console translation path)
// This file is intended to be compiled separately with CIO_WIN_WC_XLATE defined.
// It follows the same test structure but targets the translation-path branch.
// Explanatory comments annotate the intent and expected behavior.
// Build command example: g++ -DCIO_WIN_WC_XLATE=1 -I. -o test_with_xlate test_oPutsUtf8_with_xlate.cpp console_io.c

// test_oPutsUtf8_with_xlate.cpp


extern "C" int oPutsUtf8(const char *z);

/*
Continuation of the same test strategy for the translation path:
- Both the console-path and non-console path can be taken depending on the internal state.
- The return value in the translation path is whatever conZstrEmit returns, or the length
  of the string if the path falls back to fputs. The test uses strlen(z) as a reference
  expectation consistent with the non-translation path, and logs any deviation.
*/

static int gFailures_with_xlate = 0;
static int gTotal_with_xlate = 0;

#define EXPECT_EQ_X(a, b, msg) \
    do { \
        if ((a) != (b)) { \
            std::cerr << "[WITH_XLATE] FAIL: " << (msg) << " | expected: " << (b) << " actual: " << (a) << "\n"; \
            ++gFailures_with_xlate; \
        } \
        ++gTotal_with_xlate; \
    } while (0)

void test_oPutsUtf8_basic_with_xlate() {
    const char *z = "Hello, UTF-8 (xlate)!";
    int ret = oPutsUtf8(z);
    EXPECT_EQ_X(ret, (int)strlen(z), "oPutsUtf8 should return the length of the input string or translation-path equivalent");
}

void test_oPutsUtf8_empty_with_xlate() {
    const char *z = "";
    int ret = oPutsUtf8(z);
    EXPECT_EQ_X(ret, 0, "oPutsUtf8 should return 0 for an empty string on translation path");
}

void run_all_with_xlate() {
    test_oPutsUtf8_basic_with_xlate();
    test_oPutsUtf8_empty_with_xlate();

    if (gTotal_with_xlate == gFailures_with_xlate) {
        std::cout << "[WITH_XLATE] All tests passed (" << gTotal_with_xlate << " tests).\n";
    } else {
        std::cerr << "[WITH_XLATE] Summary: " << gTotal_with_xlate - gFailures_with_xlate
                  << " passed, " << gFailures_with_xlate << " failed out of "
                  << gTotal_with_xlate << " tests.\n";
    }
}

// Entry point for translation-path tests
int main_with_xlate() {
    run_all_with_xlate();
    return (gTotal_with_xlate == gFailures_with_xlate) ? 0 : 1;
}

// End of test_oPutsUtf8_with_xlate.cpp