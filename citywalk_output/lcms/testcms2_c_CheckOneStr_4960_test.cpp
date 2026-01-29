/*
  Unit Test Suite for cmsBool CheckOneStr(cmsMLU* mlu, cmsInt32Number n)

  This test harness is designed to be compiled with a C++11 toolchain
  and linked against the existing test framework (testcms2.h and related
  implementation in the project). It avoids using GTest and instead
  uses a small, non-terminating assertion mechanism to maximize
  code coverage while preserving executable tests.

  Tests cover:
  - True path: en_US string equals "Hello, world %d" and es_ES string equals "Hola, mundo %d"
  - False path variants:
      a) Mismatch in English string
      b) Mismatch in Spanish string
  - A basic sanity check with a representative n value.

  Notes:
  - We rely on SetOneStr to construct a cmsMLU object containing the two localized strings.
  - The test suite assumes that SetOneStr is provided by the test dependencies
    (as indicated in the focal class dependency block). If cmsMLUfree exists, it can be used to cleanup;
    otherwise, leaks are acceptable in a short-lived test run.
  - The tests invoke CheckOneStr and perform non-terminating assertions
    (they accumulate failures and report at the end).
*/

#include <cstddef>
#include <iostream>
#include <cstdio>
#include <testcms2.h>


// Import C dependencies. The project provides testcms2.h with prototypes
// for SetOneStr and CheckOneStr, among other test helpers.
// Use extern "C" to ensure correct linkage in C++11.
extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static bool g_all_tests_passed = true;

#define EXPECT_TRUE(x, msg) \
    do { \
        if(!(x)) { \
            std::cerr << "TEST FAILURE: " << (msg) << std::endl; \
            g_all_tests_passed = false; \
        } \
    } while(0)

#define EXPECT_FALSE(x, msg) \
    do { \
        if((x)) { \
            std::cerr << "TEST FAILURE: " << (msg) << std::endl; \
            g_all_tests_passed = false; \
        } \
    } while(0)

// Test 1: Success path
// Set up a MLU with en_US -> "Hello, world %d" and es_ES -> "Hola, mundo %d"
// Invoke CheckOneStr with n = 7 and expect TRUE.
static void Test_CheckOneStr_Success(void)
{
    cmsMLU* mlu = nullptr;

    // s1 corresponds to English string (en_US) and s2 to Spanish (es_ES)
    const wchar_t* s_en  = L"Hello, world %d";
    const wchar_t* s_es  = L"Hola, mundo %d";

    // Build MLU with proper localized strings
    SetOneStr(&mlu, s_en, s_es);

    // Use a representative n to ensure formatting is applied
    const cmsInt32Number n = 7;

    cmsBool result = CheckOneStr(mlu, n);
    EXPECT_TRUE(result != 0, "CheckOneStr should return TRUE for matching en/es strings.");

    // Optional cleanup if the API provides cmsMLUfree or similar
    // If unavailable, memory leak is acceptable for test duration.
#ifdef cmsMLUfree
    cmsMLUfree(mlu);
#endif
}

// Test 2: English string mismatch (Should fail on first comparison)
static void Test_CheckOneStr_Fail_EnglishMismatch(void)
{
    cmsMLU* mlu = nullptr;

    // Intentionally wrong English string; Spanish stays correct
    const wchar_t* s_en_wrong = L"Greetings, world %d";
    const wchar_t* s_es_correct = L"Hola, mundo %d";

    SetOneStr(&mlu, s_en_wrong, s_es_correct);

    const cmsInt32Number n = 3;

    cmsBool result = CheckOneStr(mlu, n);
    EXPECT_FALSE(result != 0, "CheckOneStr should fail when English (en_US) string mismatches.");

#ifdef cmsMLUfree
    cmsMLUfree(mlu);
#endif
}

// Test 3: Spanish string mismatch (Should fail on second comparison)
static void Test_CheckOneStr_Fail_SpanishMismatch(void)
{
    cmsMLU* mlu = nullptr;

    // English is correct, Spanish is wrong
    const wchar_t* s_en_correct = L"Hello, world %d";
    const wchar_t* s_es_wrong = L"Adios, mundo %d";

    SetOneStr(&mlu, s_en_correct, s_es_wrong);

    const cmsInt32Number n = 42;

    cmsBool result = CheckOneStr(mlu, n);
    EXPECT_FALSE(result != 0, "CheckOneStr should fail when Spanish (es_ES) string mismatches.");

#ifdef cmsMLUfree
    cmsMLUfree(mlu);
#endif
}

// Entry point for the tests
int main(void)
{
    std::cout << "Starting tests for cmsBool CheckOneStr..." << std::endl;

    Test_CheckOneStr_Success();
    Test_CheckOneStr_Fail_EnglishMismatch();
    Test_CheckOneStr_Fail_SpanishMismatch();

    if (g_all_tests_passed) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}