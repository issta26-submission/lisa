/*
  Unit test suite for the focal method testPrngString (C function)
  under testPrngString(unsigned int iVal, char *aOut, int nOut)
  - The implementation of testPrngString relies on testPrngValue(iVal)
  - We provide a deterministic C linkage definition for testPrngValue to ensure
    repeatable tests and link against the C file lsmtest_util.c that contains
    testPrngString.
  - The tests are written in C++11 without any GTest dependency.
  - Each test prints descriptive messages on failure and continues to exercise all tests
    (non-terminating assertions style).
*/

#include <string.h>
#include <lsmtest.h>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <iostream>


// Declarations of the C functions from the focal code.
// Use C linkage to match the implementation in lsmtest_util.c.
extern "C" void testPrngString(unsigned int iVal, char *aOut, int nOut);
extern "C" unsigned int testPrngValue(unsigned int iVal);

// Provide a deterministic, pure function for testPrngValue with C linkage.
// This ensures repeatable test results and aligns with our expectations for outputs.
extern "C" unsigned int testPrngValue(unsigned int iVal) {
    // Simple 32-bit Linear Congruential Generator (parameters from Numerical Recipes)
    unsigned int x = iVal;
    x = 1664525u * x + 1013904223u;
    return x;
}

// Global counter to report failures without aborting tests early.
static int g_failures = 0;

// Helper to report a failing assertion while continuing tests.
static void reportFail(const char* testName, const char* msg) {
    std::cerr << "FAIL: " << testName << " - " << msg << std::endl;
    ++g_failures;
}

/*
  Test 1: Basic sequence for iVal = 0 with nOut = 6 (produces 5 characters).
  Expected string: aOut = "xxxxx\0" where each x is 'a' + (testPrngValue(iVal+i) % 26)
*/
static void test_PrngString_BasicSequence() {
    const unsigned int iVal = 0;
    const int nOut = 6;
    char out[64];
    testPrngString(iVal, out, nOut);

    bool passed = true;
    for (int i = 0; i < nOut - 1; ++i) {
        unsigned int v = testPrngValue(iVal + i);
        char expected = 'a' + (v % 26);
        if (out[i] != expected) {
            reportFail("test_PrngString_BasicSequence", "Mismatch at position while computing expected characters.");
            passed = false;
            break;
        }
    }

    if (out[nOut - 1] != '\0') {
        reportFail("test_PrngString_BasicSequence", "String is not properly null-terminated at index nOut-1.");
        passed = false;
    }

    if (passed) {
        // Optional: success message per test (commented to reduce stdout in large suites)
        // std::cout << "PASS: test_PrngString_BasicSequence" << std::endl;
    }
}

/*
  Test 2: Additional sequence sanity with a non-trivial iVal and larger nOut.
  Ensures the function handles longer strings and consistent output.
*/
static void test_PrngString_LargeSequence() {
    const unsigned int iVal = 5;
    const int nOut = 10;
    char out[128];
    testPrngString(iVal, out, nOut);

    bool passed = true;
    for (int i = 0; i < nOut - 1; ++i) {
        unsigned int v = testPrngValue(iVal + i);
        char expected = 'a' + (v % 26);
        if (out[i] != expected) {
            reportFail("test_PrngString_LargeSequence", "Mismatch in long sequence computation.");
            passed = false;
            break;
        }
    }
    if (out[nOut - 1] != '\0') {
        reportFail("test_PrngString_LargeSequence", "String is not null-terminated for large sequence.");
        passed = false;
    }

    if (passed) {
        // std::cout << "PASS: test_PrngString_LargeSequence" << std::endl;
    }
}

/*
  Test 3: Edge case with nOut = 1 should produce an empty string.
  Verifies that aOut[0] == '\0'.
*/
static void test_PrngString_EmptyOutput() {
    const unsigned int iVal = 42;
    const int nOut = 1;
    char out[16];
    testPrngString(iVal, out, nOut);

    if (out[0] != '\0') {
        reportFail("test_PrngString_EmptyOutput", "Expected empty string when nOut == 1.");
    } else {
        // std::cout << "PASS: test_PrngString_EmptyOutput" << std::endl;
    }
}

/*
  Test 4: Validate that all produced characters lie within the lowercase alphabet range.
  This checks the contract that the output is alphabetic (a-z) for produced positions.
*/
static void test_PrngString_AlphabetRange() {
    const unsigned int iVal = 13;
    const int nOut = 20;
    char out[64];
    testPrngString(iVal, out, nOut);

    bool passed = true;
    for (int i = 0; i < nOut - 1; ++i) {
        char c = out[i];
        if (c < 'a' || c > 'z') {
            char msg[80];
            std::snprintf(msg, sizeof(msg),
                          "Character out of range at index %d: '%c'", i, c);
            reportFail("test_PrngString_AlphabetRange", msg);
            passed = false;
            break;
        }
    }
    if (out[nOut - 1] != '\0') {
        reportFail("test_PrngString_AlphabetRange", "String is not null-terminated as expected.");
        passed = false;
    }

    if (passed) {
        // std::cout << "PASS: test_PrngString_AlphabetRange" << std::endl;
    }
}

/*
  Test 5: Multiple varying iVal values to ensure consistent behavior across calls.
  Verifies each produced string's length and termination irrespective of iVal.
*/
static void test_PrngString_MultipleInputs() {
    struct Case { unsigned int iVal; int nOut; };
    Case cases[] = {
        { 0, 8 },
        { 7, 12 },
        { 123, 15 },
        { 9999, 6 }
    };
    const int count = sizeof(cases) / sizeof(cases[0]);
    bool allPassed = true;

    for (int idx = 0; idx < count; ++idx) {
        unsigned int iVal = cases[idx].iVal;
        int nOut = cases[idx].nOut;
        char out[64];
        testPrngString(iVal, out, nOut);
        for (int i = 0; i < nOut - 1; ++i) {
            unsigned int v = testPrngValue(iVal + i);
            char expected = 'a' + (v % 26);
            if (out[i] != expected) {
                reportFail("test_PrngString_MultipleInputs", "Mismatch in multi-input case.");
                allPassed = false;
                break;
            }
        }
        if (out[nOut - 1] != '\0') {
            reportFail("test_PrngString_MultipleInputs", "Null-termination missing in multi-input case.");
            allPassed = false;
        }
    }

    if (allPassed) {
        // std::cout << "PASS: test_PrngString_MultipleInputs" << std::endl;
    }
}

// Entry point
int main() {
    // Run all test cases. Each test prints its own failures if any.
    test_PrngString_BasicSequence();
    test_PrngString_LargeSequence();
    test_PrngString_EmptyOutput();
    test_PrngString_AlphabetRange();
    test_PrngString_MultipleInputs();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
    } else {
        std::cerr << g_failures << " test(s) FAILED" << std::endl;
    }

    // Return non-zero if any test failed, to integrate with CI that checks exit status.
    return g_failures;
}