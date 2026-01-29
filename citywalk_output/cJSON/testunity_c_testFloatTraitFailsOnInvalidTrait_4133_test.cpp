// test_float_trait_suite.cpp
// A C++11 test harness for the focal Unity test: testFloatTraitFailsOnInvalidTrait
// This harness executes the focal test via Unity's RUN_TEST mechanism from C++
// and therefore validates the behavior under the Unity test framework.
// It relies on the focal test being defined in testunity.c as:
//   void testFloatTraitFailsOnInvalidTrait(void);
//
// The focal test itself contains two execution paths controlled by UNITY_EXCLUDE_FLOAT.
// By invoking it through a wrapper RUN_TEST, we exercise its integration with Unity.
//
// Note: This harness is designed to be compiled and linked with the rest of the
// Unity-based test suite (including testunity.c). It uses C linkage for the focal
// test function declaration to ensure proper symbol resolution when linking with C code.

#include <unity.h>
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


#ifdef __cplusplus
extern "C" {
#endif
// Declaration of the focal test function located in testunity.c
void testFloatTraitFailsOnInvalidTrait(void);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Wrapper test: delegates to the focal test using Unity's RUN_TEST macro.
//
// This approach ensures that the focal test is executed within Unity's test runner,
// enabling proper setup/teardown, test counts, and abort handling.
// The wrapper itself is a Unity test function but is implemented here in C++
// to satisfy the requirement of a C++11 test suite driver.

void testWrapper_RunFloatTraitFailsOnInvalidTrait(void)
{
    // Execute the focal test. This will internally exercise both code paths
    // depending on whether UNITY_EXCLUDE_FLOAT is defined at compile time.
    // The Unity framework will capture any aborts/failures as part of the suite.
    RUN_TEST(testFloatTraitFailsOnInvalidTrait);
}

#ifdef __cplusplus
}
#endif

// Main harness: initializes Unity, runs the wrapper test (which in turn runs the focal test),
// and then finalizes Unity, reporting the cumulative results.
//
// Rationale:
// - Centralizes execution of the focal test within a C++-driven harness.
// - Keeps the test structure consistent with typical C++ unit-test ergonomics
//   while leveraging the existing Unity-based test under testunity.c.
int main(void)
{
#ifdef __cplusplus
    // Optional: provide a descriptive stdout header for test runs
    std::cout << "Starting Unity test suite for focal method: testFloatTraitFailsOnInvalidTrait" << std::endl;
#endif

    // Begin Unity testing
    UNITY_BEGIN();

    // Run the wrapper which invokes the focal test
    RUN_TEST(testWrapper_RunFloatTraitFailsOnInvalidTrait);

    // End Unity testing and return status
    int result = UNITY_END();
#ifdef __cplusplus
    std::cout << "Unity test suite finished with result: " << (result == 0 ? "PASS" : "FAIL") << std::endl;
#endif
    return result;
}