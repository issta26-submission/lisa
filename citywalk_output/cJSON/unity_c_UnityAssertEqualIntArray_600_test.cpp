// Self-contained C++11 test suite for UnityAssertEqualIntArray (ported and simplified for testing in C++11 without GTest)
// This file provides a minimal mock of the Unity testing environment sufficient to exercise the
// UnityAssertEqualIntArray logic, along with a small suite of unit tests.
// Note: This is a focused, self-contained test harness and not a full Unity framework installation.

#include <stddef.h>
#include <cstddef>
#include <unity.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <string>


using namespace std;

/*
  Domain notes:
  - We replicate only the necessary parts of Unity's environment to exercise UnityAssertEqualIntArray.
  - We define minimal types/macros to drive the code paths.
  - We implement tests to cover: zero-length arrays, equal pointers, one-array-null, int8/int16/int32/int64 paths,
    display range mask logic, and array-to-array pointer adjustment path.
*/

// ------------------ Minimal Unity-like Environment (Stubs) ------------------

// Types (mocked for test purposes)
typedef void* UNITY_INTERNAL_PTR;
typedef int   UNITY_INT;
typedef unsigned int UNITY_UINT;
typedef unsigned int UNITY_UINT32;
typedef int   UNITY_LINE_TYPE;
typedef int   UNITY_FLAGS_T;
typedef unsigned int UNITY_DISPLAY_STYLE_T;

// Pointer attribute macro (kept for compatibility with the focal code)
#define UNITY_PTR_ATTRIBUTE /* empty */

// Flags and style constants (mocked)
#define UNITY_ARRAY_TO_ARRAY 1
#define UNITY_DISPLAY_RANGE_UINT 0x20

// Optional 64-bit support (simulate UNITY_SUPPORT_64)
#define UNITY_SUPPORT_64 1

// Globals to observe behavior in tests
static bool g_pointlessCalled = false;     // whether UnityPrintPointlessAndBail() was invoked
static bool g_failIssued = false;          // whether UNITY_FAIL_AND_BAIL path was triggered
static int  g_failLine = -1;               // line number captured on failure
static bool g_bailed = false;              // bail state flag (simulated macro effect)
static string g_logBuffer;                   // capture simple textual logs for debugging

// Helpers to log and simulate Unity's printing facilities
static void UnityPrint(const char* str) {
    if (str) g_logBuffer += str;
}
static void UnityPrintNumberUnsigned(const UNITY_UINT number) {
    g_logBuffer += to_string(number);
}
static void UnityPrintNumberByStyle(const UNITY_INT number, const UNITY_DISPLAY_STYLE_T /*style*/) {
    // For test purposes, ignore 'style' and print decimal representation
    g_logBuffer += to_string(number);
}
static void UnityPrintElementIndex(size_t idx) {
    g_logBuffer += "[";
    g_logBuffer += to_string(idx);
    g_logBuffer += "]";
}
static void UnityPrintExpectedAndActualStrings(const char* /*expected*/, const char* /*actual*/) {
    // Placeholder
}
static void UnityAddMsgIfSpecified(const char* msg) {
    if (msg) {
        g_logBuffer += " Msg:";
        g_logBuffer += msg;
    }
}
static void UnityTestResultsFailBegin(const UNITY_LINE_TYPE line) {
    (void)line;
    // In tests, we simply note a failure has begun
}
static void UnityPrintPointlessAndBail() {
    g_pointlessCalled = true;
    // In the real Unity, this would bail later; here we just record the event
}
#define UNITY_FAIL_AND_BAIL do { g_failIssued = true; g_bailed = true; return; } while(0)
#define RETURN_IF_FAIL_OR_IGNORE if (g_bailed) return

// UnityIsOneArrayNull: simple helper to emulate the condition where one array is NULL
static int UnityIsOneArrayNull(UNITY_INTERNAL_PTR expected,
                               UNITY_INTERNAL_PTR actual,
                               const UNITY_LINE_TYPE lineNumber,
                               const char* msg)
{
    (void)lineNumber;
    (void)msg;
    // Treat either pointer NULL while the other is non-NULL as an error
    if ((expected == nullptr) != (actual == nullptr)) {
        return 1;
    }
    return 0;
}

// UnityAssertEqualIntArray declaration (mocked for this test harness)
static void UnityAssertEqualIntArray(UNITY_INTERNAL_PTR expected,
                                   UNITY_INTERNAL_PTR actual,
                                   const UNITY_UINT32 num_elements,
                                   const char* msg,
                                   const UNITY_LINE_TYPE lineNumber,
                                   const UNITY_DISPLAY_STYLE_T style,
                                   const UNITY_FLAGS_T flags);

// Implementation of UnityAssertEqualIntArray (mirrors the focal method's core logic)
static void UnityAssertEqualIntArray(UNITY_INTERNAL_PTR expected,
                                   UNITY_INTERNAL_PTR actual,
                                   const UNITY_UINT32 num_elements,
                                   const char* msg,
                                   const UNITY_LINE_TYPE lineNumber,
                                   const UNITY_DISPLAY_STYLE_T style,
                                   const UNITY_FLAGS_T flags)
{
    UNITY_UINT32 elements = num_elements;
    unsigned int length   = style & 0xF; // length selector from style
    RETURN_IF_FAIL_OR_IGNORE;
    if (num_elements == 0)
    {
        UnityPrintPointlessAndBail();
    }
    if (expected == actual) return; /* Both are NULL or same pointer */
    if (UnityIsOneArrayNull(expected, actual, lineNumber, msg))
        UNITY_FAIL_AND_BAIL;
    while (elements--)
    {
        UNITY_INT expect_val;
        UNITY_INT actual_val;
        switch (length)
        {
            case 1:
                expect_val = *(UNITY_PTR_ATTRIBUTE const UNITY_INT8*)expected;
                actual_val = *(UNITY_PTR_ATTRIBUTE const UNITY_INT8*)actual;
                break;
            case 2:
                expect_val = *(UNITY_PTR_ATTRIBUTE const UNITY_INT16*)expected;
                actual_val = *(UNITY_PTR_ATTRIBUTE const UNITY_INT16*)actual;
                break;
#ifdef UNITY_SUPPORT_64
            case 8:
                expect_val = *(UNITY_PTR_ATTRIBUTE const UNITY_INT64*)expected;
                actual_val = *(UNITY_PTR_ATTRIBUTE const UNITY_INT64*)actual;
                break;
#endif
            default: /* length 4 bytes */
                expect_val = *(UNITY_PTR_ATTRIBUTE const UNITY_INT32*)expected;
                actual_val = *(UNITY_PTR_ATTRIBUTE const UNITY_INT32*)actual;
                length = 4;
                break;
        }
        if (expect_val != actual_val)
        {
            if (style & UNITY_DISPLAY_RANGE_UINT && length < sizeof(expect_val))
            {   /* For UINT, remove sign extension (padding 1's) from signed type casts above */
                UNITY_INT mask = 1;
                mask = (mask << 8 * length) - 1;
                expect_val &= mask;
                actual_val &= mask;
            }
            UnityTestResultsFailBegin(lineNumber);
            UnityPrint("Element");
            UnityPrintNumberUnsigned(num_elements - elements - 1);
            UnityPrint("Expected");
            UnityPrintNumberByStyle(expect_val, style);
            UnityPrint("Was");
            UnityPrintNumberByStyle(actual_val, style);
            UnityAddMsgIfSpecified(msg);
            UNITY_FAIL_AND_BAIL;
        }
        if (flags == UNITY_ARRAY_TO_ARRAY)
        {
            expected = (UNITY_INTERNAL_PTR)((char*)expected + length);
        }
        actual   = (UNITY_INTERNAL_PTR)((char*)actual + length);
    }
}

// ------------------ Test Harness and Test Cases ------------------

// Helper to reset global state before each test
static void ResetTestState() {
    g_pointlessCalled = false;
    g_failIssued = false;
    g_bailed = false;
    g_failLine = -1;
    g_logBuffer.clear();
}

// Test 1: Zero elements
// - Verifies that UnityPrintPointlessAndBail is invoked when num_elements == 0
static bool test_zero_elements() {
    ResetTestState();
    // Prepare distinct non-null pointers; functionality should mark pointless path, not bail by itself
    UNITY_INTERNAL_PTR exp = (void*)0xDEAD;
    UNITY_INTERNAL_PTR act = (void*)0xBEEF;
    UnityAssertEqualIntArray(exp, act, 0, "zero elements test", 123, 1 /* length=1 */, 0);
    if (!g_pointlessCalled) {
        std::cerr << "Test Zero Elements: expected UnityPrintPointlessAndBail to be called.\n";
        return false;
    }
    return true;
}

// Test 2: Equal pointers (no work done)
static bool test_equal_pointers() {
    ResetTestState();
    int dummy;
    UNITY_INTERNAL_PTR exp = &dummy;
    UNITY_INTERNAL_PTR act = &dummy;
    UnityAssertEqualIntArray(exp, act, 3, "equal pointers should return early", 124, 4 /* length=4 */, 0);
    if (g_bailed) {
        std::cerr << "Test Equal Pointers: did not expect a bail.\n";
        return false;
    }
    return true;
}

// Test 3: One array null (should bail)
static bool test_one_array_null() {
    ResetTestState();
    UNITY_INTERNAL_PTR exp = nullptr;
    int x = 5;
    UNITY_INTERNAL_PTR act = &x;
    UnityAssertEqualIntArray(exp, act, 3, "one array null should bail", 125, 4, 0);
    if (!g_failIssued && !g_bailed) {
        std::cerr << "Test One Array Null: expected a bail due to null array.\n";
        return false;
    }
    return true;
}

// Test 4: int8 path with mismatch (length=1) and optional range flag
static bool test_mismatch_int8_with_rangeFlag() {
    ResetTestState();
    // Build two 8-bit arrays; mismatch at index 1
    int8_t expected[] = {10, 20, 30};
    int8_t actual[]   = {10, 99, 30};
    UNITY_UINT32 n = 3;
    UNITY_UINT line = 126;
    UnityAssertEqualIntArray((UNITY_INTERNAL_PTR)expected, (UNITY_INTERNAL_PTR)actual, n,
                             "int8 mismatch with range flag", line,
                             /* style: length=1, with range flag set */ (UNITY_DISPLAY_STYLE_T)(1 | UNITY_DISPLAY_RANGE_UINT),
                             /* flags */ 0);
    if (!g_failIssued && g_bailed) {
        // Bail should occur on mismatch
        return true;
    }
    // Depending on implementation path, bail could occur; ensure we detected a bail
    if (g_bailed) return true;
    std::cerr << "Test Mismatch Int8 With RangeFlag: expected bail on mismatch.\n";
    return false;
}

// Test 5: int32 default path mismatch
static bool test_mismatch_int32() {
    ResetTestState();
    int32_t expected[] = {100, 200, 300};
    int32_t actual[]   = {100, 201, 300};
    UNITY_UINT32 n = 3;
    UNITY_UINT line = 127;
    UnityAssertEqualIntArray((UNITY_INTERNAL_PTR)expected, (UNITY_INTERNAL_PTR)actual, n,
                             "int32 mismatch", line,
                             (UNITY_DISPLAY_STYLE_T)4, // length=4
                             0);
    if (g_bailed) {
        return true;
    } else {
        std::cerr << "Test Mismatch Int32: expected bail on mismatch.\n";
        return false;
    }
}

// Test 6: int64 path (64-bit case) - only compiled if UNITY_SUPPORT_64 defined (we defined 1 above)
static bool test_mismatch_int64() {
    ResetTestState();
    int64_t expected[] = {10000000000LL, 20000000000LL};
    int64_t actual[]   = {10000000000LL, 20000000001LL};
    UNITY_UINT32 n = 2;
    UNITY_UINT line = 128;
    // length=8
    UnityAssertEqualIntArray((UNITY_INTERNAL_PTR)expected, (UNITY_INTERNAL_PTR)actual, n,
                             "int64 mismatch", line,
                             (UNITY_DISPLAY_STYLE_T)8, // length=8
                             0);
    if (g_bailed) return true;
    std::cerr << "Test Mismatch Int64: expected bail on mismatch.\n";
    return false;
}

// Test 7: UNITY_ARRAY_TO_ARRAY path (pointer arithmetic for expected)
static bool test_array_to_array_path() {
    ResetTestState();
    // Two 8-bit elements back-to-back; use array-to-array flag to trigger pointer offset
    int8_t expected[] = {1, 2};
    int8_t actual[]   = {1, 2};
    UNITY_UINT32 n = 2;
    UNITY_UINT line = 129;
    UnityAssertEqualIntArray((UNITY_INTERNAL_PTR)expected, (UNITY_INTERNAL_PTR)actual, n,
                             "array to array path", line,
                             (UNITY_DISPLAY_STYLE_T)1, // length=1
                             UNITY_ARRAY_TO_ARRAY);
    // Expect no bail
    if (g_bailed) {
        std::cerr << "Test Array-To-Array Path: unexpected bail.\n";
        return false;
    }
    return true;
}

// Test 8: Nulls both NULL (should exit without bail)
static bool test_both_null_pointers() {
    ResetTestState();
    UnityAssertEqualIntArray(nullptr, nullptr, 3, "both null pointers", 130, 4, 0);
    if (g_bailed) {
        std::cerr << "Test Both Null Pointers: unexpected bail.\n";
        return false;
    }
    return true;
}

// ------------------ Test Runner ------------------

struct TestCase {
    const char* name;
    bool (*func)();
};

static bool prettyPrintAndRun(const TestCase& tc) {
    bool result = tc.func();
    cout << (result ? "[PASS] " : "[FAIL] ") << tc.name << "\n";
    return result;
}

int main() {
    // List of test cases
    const TestCase tests[] = {
        {"Zero Elements", test_zero_elements},
        {"Equal Pointers", test_equal_pointers},
        {"One Array Null", test_one_array_null},
        {"Mismatch int8 with Range Flag", test_mismatch_int8_with_rangeFlag},
        {"Mismatch int32", test_mismatch_int32},
        {"Mismatch int64", test_mismatch_int64},
        {"Array-To-Array Path", test_array_to_array_path},
        {"Both NULL Pointers", test_both_null_pointers},
    };

    int passed = 0;
    int total  = int(sizeof(tests) / sizeof(TestCase));

    cout << "Running UnityAssertEqualIntArray unit tests (self-contained harness)\n";
    cout << "---------------------------------------------------------------\n";
    for (const auto& tc : tests) {
        if (prettyPrintAndRun(tc)) ++passed;
    }
    cout << "---------------------------------------------------------------\n";
    cout << "Summary: " << passed << " / " << total << " tests passed.\n";

    return (passed == total) ? 0 : 1;
}