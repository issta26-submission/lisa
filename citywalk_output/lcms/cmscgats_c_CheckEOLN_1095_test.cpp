// Automated unit test suite for the focal method cmsBool CheckEOLN(cmsIT8* it8)
// Approach: Use a lightweight, self-contained test harness (no gtest) with a tiny
// mocking layer to replace dependent functions (Check and InSymbol) via preprocessor tricks.
// The tests aim to cover true/false branches of the CheckEOLN logic as described in the focal code.
// Note: This test suite assumes a C-like internal structure for cmsIT8 with at least a `sy` field
// and uses a minimal stand-in for the dependent behavior to exercise the main control flow.

#include <stdio.h>
#include <lcms2_internal.h>
#include <stdlib.h>
#include <stdbool.h>


// Domain adaptation for mocking inside the included cmscgats.c
// We replace the external calls as follows:
// - Check(...)  => TestCheck(...)
// - InSymbol(...) => TestInSymbol(...)
// - SEOLN constant is mocked to a test-specific value

#define SEOLN 9         // Mocked value for the end-of-line separator symbol

#define Check TestCheck   // Redirect calls inside cmscgats.c to our TestCheck
#define InSymbol TestInSymbol // Redirect InSymbol calls to our TestInSymbol

// Minimal type definitions to allow cmscgats.c (the focal C file) to compile in this test harness.
// We declare cmsIT8 with only the fields we need for the test (sy). The real library uses a larger struct,
// but cmsEOLN logic only relies on it8->sy for the branch and InSymbol manipulation.
typedef int cmsBool;
#define TRUE 1
#define FALSE 0

typedef struct cmsIT8 cmsIT8;

// Forward declare the test doubles to satisfy the compiler before including cmscgats.c
extern "C" cmsBool TestCheck(struct cmsIT8* it8, int sy, const char* Err);
extern "C" void TestInSymbol(struct cmsIT8* it8);

// Publicly visible field required by the test doubles
struct cmsIT8 {
    int sy; // current symbolic token type
};

// Global counter to verify InSymbol invocations during the tests
extern int gInSymbolCalls;
int gInSymbolCalls = 0;

// Test double for Check: mimic the expected behavior of the real function.
// The test doubles assume the pattern: Check(it8, sy, Err) returns TRUE if it8->sy == sy, else FALSE.
// This aligns with how the focal code uses Check(it8, SEOLN, ...): it checks if the current symbol matches SEOLN.
extern "C" cmsBool TestCheck(struct cmsIT8* it8, int sy, const char* /*Err*/) {
    if (it8 == NULL) return FALSE;
    return (it8->sy == sy) ? TRUE : FALSE;
}

// Test double for InSymbol: simulate advancing to the next token by mutating it8->sy.
// We want to emulate skipping a single SEOLN token in the test scenario.
extern "C" void TestInSymbol(struct cmsIT8* it8) {
    // Increment the call counter for verification
    ++gInSymbolCalls;

    // Move to a non-SEOLN token after the first call to mimic skipping an EOLN.
    // This is sufficient for the while (it8->sy == SEOLN) loop to terminate in tests.
    it8->sy = 0; // Any value != SEOLN will end the loop
}

// Simple test harness
static int gTotalTests = 0;
static int gFailedTests = 0;

#define RUN_TEST(testFunc, name) do { \
    gTotalTests++; \
    bool ok = (testFunc()); \
    if (!ok) { \
        gFailedTests++; \
        printf("TEST FAILED: %s\n", name); \
    } else { \
        printf("TEST PASSED: %s\n", name); \
    } \
} while(0)

//  Test 1: CheckEOLN should return FALSE when the initial Check fails.
// This covers the false-branch of the first predicate.
bool test_CheckEOLN_ReturnsFalse_When_CheckFails() {
    // Arrange
    cmsIT8 it;
    it.sy = 10; // Not SEOLN so that TestCheck(it, SEOLN, ...) will return FALSE

    // Act
    cmsBool res = CheckEOLN(&it);

    // Assert: Expect FALSE, and InSymbol should not have been invoked
    // Note: InSymbol's side-effect is measured by gInSymbolCalls (which should remain 0)
    return (res == FALSE) && (gInSymbolCalls == 0);
}

//  Test 2: CheckEOLN should return TRUE and invoke InSymbol when initial Check passes.
// This covers the true-branch of the first predicate and the subsequent EOLN-skipping loop.
bool test_CheckEOLN_ReturnsTrue_When_CheckPasses_AndInSymbolAdvances() {
    // Arrange
    cmsIT8 it;
    it.sy = SEOLN; // Set to separator so that TestCheck(it, SEOLN, ...) will return TRUE
    gInSymbolCalls = 0; // reset counter

    // Act
    cmsBool res = CheckEOLN(&it);

    // Assert
    // - The first predicate passes, so res should be TRUE
    // - InSymbol should have been invoked at least once to skip the EOLN
    bool ok = (res == TRUE) && (gInSymbolCalls >= 1);
    // After InSymbol, our TestInSymbol sets it.sy to 0, so the loop should end
    return ok;
}

// Entry point
int main(void) {
    printf("Starting unit tests for cmsBool CheckEOLN(cmsIT8* it8)\n");

    // Run tests
    RUN_TEST(test_CheckEOLN_ReturnsFalse_When_CheckFails, "CheckEOLN false-branch when Check fails");
    RUN_TEST(test_CheckEOLN_ReturnsTrue_When_CheckPasses_AndInSymbolAdvances, "CheckEOLN true-branch with InSymbol advancement");

    // Summary
    printf("Test summary: %d total, %d failed\n", gTotalTests, gFailedTests);
    return (gFailedTests == 0) ? 0 : 1;
}

// NOTE:
// The test harness relies on a preprocessor trick to redirect internal calls inside cmscgats.c.
// Before including cmscgats.c, the test defines:
// - SEOLN as a mock value
// - Check to TestCheck
// - InSymbol to TestInSymbol
// The TestCheck and TestInSymbol implementations provide deterministic, controlled behavior
// so that we can exercise the conditional branches in cmsBool CheckEOLN(cmsIT8* it8) without a full parser.
// This approach ensures non-terminating execution during tests (we never exit early),
// and keeps changes localized to the test suite.