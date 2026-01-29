// File: test_detail_suite_default.cpp
// Test harness for the focal method testThatDetailsCanHandleTestFail when Unity details are not excluded.
// This file should be compiled and linked with the Unity test framework and the original testunity.c.
// The goal is to verify that the focal test correctly triggers a failure with the detail strings "Detail1" and "Detail2".

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// Focal method under test located in testunity.c
void testThatDetailsCanHandleTestFail(void);
#ifdef __cplusplus
}
#endif

// Wrapper test that delegates to the focal test.
// Explanations:
// - This wrapper keeps the test suite minimal and delegates to the focal method
// - The focal method uses Unity's EXPECT_ABORT_BEGIN/END to capture the expected abort
//   and will report a failure that includes the detail strings when DETAILS are enabled.
void testWrapperDetailHandlingDefault(void)
{
    // Call the focal test directly. If UNITY_SET_DETAILS is used inside, details will be
    // reported as part of the failure message by Unity.
    testThatDetailsCanHandleTestFail();
}

// Main runner for the Unity test framework.
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(testWrapperDetailHandlingDefault); // Execute the focal test path with details
    return UNITY_END();
}
 

// File: test_detail_suite_exclude.cpp
// Test harness for the focal method testThatDetailsCanHandleTestFail when Unity details are excluded.
// This file should be compiled and linked with the Unity test framework and the original testunity.c
// with UNITY_EXCLUDE_DETAILS defined (build flag). The goal is to verify that the focal test path
// falls back to TEST_IGNORE() behavior when details are excluded.

#define UNITY_EXCLUDE_DETAILS


#ifdef __cplusplus
extern "C" {
#endif
// Focal method under test located in testunity.c
void testThatDetailsCanHandleTestFail(void);
#ifdef __cplusplus
}
#endif

// Wrapper test that delegates to the focal test.
// Explanations:
// - This wrapper ensures the same external interface is exercised, but under UNITY_EXCLUDE_DETAILS.
// - The focal function will execute the TEST_IGNORE path, resulting in the test being ignored
//   and no abort occurring.
void testWrapperDetailHandlingExcluded(void)
{
    testThatDetailsCanHandleTestFail();
}

// Main runner for the Unity test framework.
int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(testWrapperDetailHandlingExcluded); // Execute the focal test path with details excluded
    return UNITY_END();
}