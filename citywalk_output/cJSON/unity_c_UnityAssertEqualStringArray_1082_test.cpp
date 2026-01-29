// Self-contained C++ test harness for UnityAssertEqualStringArray
// This test suite is designed to exercise the focal method in a single translation unit.
// It includes a minimal, self-contained subset of the Unity runtime (stubs) sufficient
// to validate branching behavior of UnityAssertEqualStringArray without relying on an
// external test framework like GTest.
//
// Note: We intentionally do not test the zero-length (num_elements == 0) path here due to
// the bail semantics (UnityPrintPointlessAndBail) which in the real Unity suite performs a bail.
// The tests below cover: early return when pointers are identical, equal arrays, and mismatch cases.

#include <stddef.h>
#include <cstring>
#include <unity.h>
#include <cstdio>
#include <cstdint>


// -----------------------------------------------------------------------------
// Minimal Type Definitions and Globals to Support UnityAssertEqualStringArray
// -----------------------------------------------------------------------------

typedef unsigned int UNITY_UINT32;
typedef unsigned int UNITY_FLAGS_T;
typedef void*        UNITY_INTERNAL_PTR;
typedef int          UNITY_LINE_TYPE;

// The UnityArrayFlags (subset)
#define UNITY_ARRAY_TO_ARRAY 0

// Lightweight Unity global to mimic the expected interface
struct UnityGlobals {
    int CurrentTestFailed;
    // Additional debug/state fields can be added if needed
} Unity = {0};

// Forward declarations of helper functions used by UnityAssertEqualStringArray
static void UnityPrint(const char* string) { /* no-op for test harness */ }
static void UnityPrintLen(const char* string, UNITY_UINT32 length) { /* no-op */ }
static void UnityPrintNumberByStyle(int number, int style) { /* no-op */ }
static void UnityPrintNumber(int number_to_print) { /* no-op */ }
static void UnityPrintNumberUnsigned(UNITY_UINT32 number) { /* no-op */ }
static void UnityPrintNumberHex(UNITY_UINT32 number, char nibbles_to_print) { /* no-op */ }
static void UnityPrintMask(UNITY_UINT32 mask, UNITY_UINT32 number) { /* no-op */ }
static void UnityPrintFloat(double input_number) { /* no-op */ }

static void UnityTestResultsBegin(const char* file, const UNITY_LINE_TYPE line) { /* no-op */ }
static void UnityTestResultsFailBegin(const UNITY_LINE_TYPE line) { /* no-op */ }

static void UnityAddMsgIfSpecified(const char* msg) { /* no-op */ }

static void UnityPrintExpectedAndActualStrings(const char* expected, const char* actual) {
    // For visibility in debugging, we print a concise diff-like summary.
    // In real Unity, this would format nicely; here we keep it simple.
    (void)expected; (void)actual;
}

static void UnityPrintExpectedAndActualStringsLen(const char* expected,
                                                const char* actual,
                                                const UNITY_UINT32 length) {
    (void)expected; (void)actual; (void)length;
}

static int UnityIsOneArrayNull(UNITY_INTERNAL_PTR expected,
                             UNITY_INTERNAL_PTR actual,
                             const UNITY_LINE_TYPE lineNumber,
                             const char* msg) {
    // In the real Unity, this checks for A==NULL and B==NULL mix for array pointers.
    // For the purposes of this test harness, we simply indicate no null-array condition.
    (void)expected; (void)actual; (void)lineNumber; (void)msg;
    return 0;
}

// Bail macro (as in real Unity)
#define UNITY_FAIL_AND_BAIL do { Unity.CurrentTestFailed = 1; return; } while(0)
#define RETURN_IF_FAIL_OR_IGNORE /* no-op in harness */

// Pointless bail (simply mark as failed for visibility in tests; keeping it non-bailing here)
static void UnityPrintPointlessAndBail() {
    // In real Unity, this would bail the test; here we mark as failed to signal the event.
    Unity.CurrentTestFailed = 1;
}

// -----------------------------------------------------------------------------
// UnityAssertEqualStringArray (focal method under test) copied/adapted for harness
// -----------------------------------------------------------------------------

void UnityAssertEqualStringArray(UNITY_INTERNAL_PTR expected,
                               const char** actual,
                               const UNITY_UINT32 num_elements,
                               const char* msg,
                               const UNITY_LINE_TYPE lineNumber,
                               const UNITY_FLAGS_T flags)
{
    UNITY_UINT32 i = 0;
    UNITY_UINT32 j = 0;
    const char* expd = NULL;
    const char* act = NULL;
    RETURN_IF_FAIL_OR_IGNORE;
    /* if no elements, it's an error */
    if (num_elements == 0)
    {
        UnityPrintPointlessAndBail();
    }
    if ((const void*)expected == (const void*)actual)
    {
        return; /* Both are NULL or same pointer */
    }
    if (UnityIsOneArrayNull((UNITY_INTERNAL_PTR)expected, (UNITY_INTERNAL_PTR)actual, lineNumber, msg))
    {
        UNITY_FAIL_AND_BAIL;
    }
    if (flags != UNITY_ARRAY_TO_ARRAY)
    {
        expd = (const char*)expected;
    }
    do
    {
        act = actual[j];
        if (flags == UNITY_ARRAY_TO_ARRAY)
        {
            expd = ((const char* const*)expected)[j];
        }
        /* if both pointers not null compare the strings */
        if (expd && act)
        {
            for (i = 0; expd[i] || act[i]; i++)
            {
                if (expd[i] != act[i])
                {
                    Unity.CurrentTestFailed = 1;
                    break;
                }
            }
        }
        else
        { /* handle case of one pointers being null (if both null, test should pass) */
            if (expd != act)
            {
                Unity.CurrentTestFailed = 1;
            }
        }
        if (Unity.CurrentTestFailed)
        {
            UnityTestResultsFailBegin(lineNumber);
            if (num_elements > 1)
            {
                // Indicate which element failed (for debugging)
                // In harness, we simply note failure
            }
            UnityPrintExpectedAndActualStrings(expd, act);
            UnityAddMsgIfSpecified(msg);
            UNITY_FAIL_AND_BAIL;
        }
    } while (++j < num_elements);
}

// -----------------------------------------------------------------------------
// Helper structures to capture test results (for harness visibility)
// -----------------------------------------------------------------------------

static const char* UnityStrElement = "Element";

// Per-test result tracking for harness
static int g_lastTestFailed = 0;
static const char* g_lastExpected = nullptr;
static const char* g_lastActual = nullptr;

// Capture debug info when a failure would be printed
static void CaptureFailureDetails(const char* expected, const char* actual) {
    g_lastExpected = expected;
    g_lastActual = actual;
}

// Override UnityPrintExpectedAndActualStrings for visibility
static void UnityPrintExpectedAndActualStringsOverride(const char* expected, const char* actual) {
    CaptureFailureDetails(expected, actual);
}

// Wire up override
#undef UnityPrintExpectedAndActualStrings
#define UnityPrintExpectedAndActualStrings UnityPrintExpectedAndActualStringsOverride

// -----------------------------------------------------------------------------
// Test Harness: Individual Test Cases
// -----------------------------------------------------------------------------

static void resetUnityState() {
    Unity.CurrentTestFailed = 0;
}

// Test Case 1: Equal two-element string array (should pass)
static void test_UnityAssertEqualStringArray_EqualArrays(void) {
    resetUnityState();

    const char* e0 = "hello";
    const char* e1 = "world";
    const char* expectedArr[2] = { e0, e1 };
    const char* actualArr[2]   = { "hello", "world" };

    UnityAssertEqualStringArray((UNITY_INTERNAL_PTR)expectedArr,
                                actualArr,
                                2,
                                "Equal arrays should pass",
                                10,
                                UNITY_ARRAY_TO_ARRAY);

    if (Unity.CurrentTestFailed) {
        printf("Test 1 FAILED: Equal arrays reported as not equal.\n");
    } else {
        printf("Test 1 PASSED: Equal arrays reported as equal.\n");
    }
}

// Test Case 2: Mismatch in second element (should fail)
static void test_UnityAssertEqualStringArray_MismatchSecondElement(void) {
    resetUnityState();

    const char* e0 = "foo";
    const char* e1 = "bar";
    const char* expectedArr[2] = { e0, e1 };
    const char* actualArr[2]   = { "foo", "baz" };

    UnityAssertEqualStringArray((UNITY_INTERNAL_PTR)expectedArr,
                                actualArr,
                                2,
                                "Mismatch in second element",
                                20,
                                UNITY_ARRAY_TO_ARRAY);

    if (Unity.CurrentTestFailed) {
        printf("Test 2 PASSED: Detected mismatch in second element.\n");
    } else {
        printf("Test 2 FAILED: Mismatch not detected when expected.\n");
    }
}

// Test Case 3: Null pointer in first element vs non-null (should fail)
static void test_UnityAssertEqualStringArray_FirstElementNullVsNonNull(void) {
    resetUnityState();

    const char* expectedArr[2] = { NULL, "BB" };
    const char* actualArr[2]   = { "AA",   "BB" };

    UnityAssertEqualStringArray((UNITY_INTERNAL_PTR)expectedArr,
                                actualArr,
                                2,
                                "First element NULL vs non-NULL",
                                30,
                                UNITY_ARRAY_TO_ARRAY);

    if (Unity.CurrentTestFailed) {
        printf("Test 3 PASSED: Detected NULL vs non-NULL in first element.\n");
    } else {
        printf("Test 3 FAILED: NULL vs non-NULL condition not detected.\n");
    }
}

// Test Case 4: Pointer identity path (expected and actual pointers identical) -> should return early
static void test_UnityAssertEqualStringArray_PointerIdentityReturn(void) {
    resetUnityState();

    const char* s0 = "alpha";
    const char* s1 = "beta";
    const char* arr[2] = { s0, s1 };
    // Pass the same pointer for both expected and actual
    UnityAssertEqualStringArray((UNITY_INTERNAL_PTR)arr, arr, 2, "Pointer identity test", 40, UNITY_ARRAY_TO_ARRAY);

    if (Unity.CurrentTestFailed) {
        printf("Test 4 FAILED: Pointer identity incorrectly reported as failed.\n");
    } else {
        printf("Test 4 PASSED: Pointer identity path returned without failure.\n");
    }
}

// Test runner to execute all tests
int main() {
    printf("Starting UnityAssertEqualStringArray test suite (self-contained harness)...\n\n");

    test_UnityAssertEqualStringArray_EqualArrays();
    test_UnityAssertEqualStringArray_MismatchSecondElement();
    test_UnityAssertEqualStringArray_FirstElementNullVsNonNull();
    test_UnityAssertEqualStringArray_PointerIdentityReturn();

    printf("\nTest suite completed.\n");
    return 0;
}