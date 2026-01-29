// Test suite for UnityAssertEqualFloatArray (Unity C unit test helper) using C++11.
// This test avoids terminating the process on failure by exercising only non-failing paths.
// The tests rely on the Unity C framework (unity.h) being available in the include path.
// Note: We intentionally test only safe, non-bailing paths to keep tests executable in a C++ harness.

#include <stddef.h>
#include <cstdio>
#include <vector>
#include <unity.h>


extern "C" {
}

// Test 1: Verify that UnityAssertEqualFloatArray correctly handles two equal arrays
//         when using the UNITY_ARRAY_TO_ARRAY flag. This exercises:
//           - Initialization of local counters and pointers (elements, ptr_expected, ptr_actual)
//           - The while loop and the float comparison path (UnityFloatsWithin) when values are equal
//           - The case where flags == UNITY_ARRAY_TO_ARRAY to increment the expected pointer
// This test should complete without triggering a Unity fail/bail.
void test_UnityAssertEqualFloatArray_EqualArrays_WithArrayToArrayFlag()
{
    // Prepare two equal arrays of three elements
    UNITY_FLOAT expected_arr[3] = {1.0f, 2.0f, 3.0f};
    UNITY_FLOAT actual_arr[3]   = {1.0f, 2.0f, 3.0f};

    // Call the focal function with num_elements = 3 and UNITY_ARRAY_TO_ARRAY flag
    // msg is provided to aid diagnostics if a failure occurs
    UnityAssertEqualFloatArray(expected_arr,
                               actual_arr,
                               3,                          // num_elements
                               "Arrays should be equal (test case 1)",
                               (UNITY_LINE_TYPE)42,        // lineNumber (arbitrary for test)
                               UNITY_ARRAY_TO_ARRAY);       // flags

    // If we reach here, the path was executed without triggering a bail/fail.
    printf("PASS: test_UnityAssertEqualFloatArray_EqualArrays_WithArrayToArrayFlag\n");
}

// Test 2: Verify that UnityAssertEqualFloatArray returns immediately
//         when expected and actual pointers are identical (same array reference).
// This directly exercises the early return optimization:
//   if (expected == actual) return;
void test_UnityAssertEqualFloatArray_SamePointer_ReturnsEarly()
{
    UNITY_FLOAT arr[4] = {0.0f, -1.0f, 2.5f, 4.0f};

    // Pass the same pointer for both expected and actual
    UnityAssertEqualFloatArray(arr,
                               arr,
                               4,                          // num_elements
                               "Pointers are the same; should return early",
                               (UNITY_LINE_TYPE)84,        // lineNumber
                               UNITY_ARRAY_TO_ARRAY);       // flags

    // If we reach here, the early return path executed without issue.
    printf("PASS: test_UnityAssertEqualFloatArray_SamePointer_ReturnsEarly\n");
}

// Helper to silence unused parameter warnings if needed
static void unused_suppress(void) {
    (void)0;
}

// Main entry for running tests without a full GTest-style runner.
// We invoke the tests directly to maximize executable coverage in environments
// where GTest is not allowed and to adhere to the directive to not terminate on failures.
// Note: This harness does not capture Unity's non-fatal assertions; it relies on
// the focal function to complete without bailing for these scenarios.
int main(void)
{
    // Initialize a minimal test context if Unity requires it.
    // Some environments require UnityBegin/UnityEnd; these calls are omitted here
    // since we are directly invoking the function under test and not using Unity's runner.
    // However, the presence of unity.h in the build typically expects those calls.
    // If your environment requires them, uncomment the lines below:
    // UnityBegin("UnityAssertEqualFloatArray tests");

    test_UnityAssertEqualFloatArray_EqualArrays_WithArrayToArrayFlag();
    test_UnityAssertEqualFloatArray_SamePointer_ReturnsEarly();

    // If UnityBegin/UnityEnd were used:
    // UnityEnd();

    printf("All non-bailing tests completed.\n");
    return 0;
}