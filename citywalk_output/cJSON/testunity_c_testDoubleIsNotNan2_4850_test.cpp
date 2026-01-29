/***************************************************************
 Step 1 - Program Understanding and Candidate Keywords
 - Focal Method: testDoubleIsNotNan2
 - Purpose: Validate that a double constant (234.9) is NOT NaN.
 - Key dependencies / keywords likely used in tests:
   Unity framework: UNITY_EXCLUDE_DOUBLE, TEST_IGNORE, TEST_ASSERT_DOUBLE_IS_NOT_NAN
   Type/precision: UNITY_DOUBLE
   Control flow: preprocessor branches for UNITY_EXCLUDE_DOUBLE
   Test framework glue: UNITY_BEGIN, UNITY_END, RUN_TEST, etc.
 - These keywords guide test coverage for both paths (double support enabled vs excluded).
***************************************************************/

/*
 This separate test harness targets the focal method testDoubleIsNotNan2
 defined in testunity.c. It uses the Unity framework (unity.h) to drive
 the tests and is compiled as a separate translation unit to exercise
 both branches: when doubles are included and when they are excluded.

 Notes:
 - The focal function uses a preprocessor branch:
     #ifdef UNITY_EXCLUDE_DOUBLE
         TEST_IGNORE();
       #else
         TEST_ASSERT_DOUBLE_IS_NOT_NAN(234.9);
       #endif
 - This harness creates two tests:
     1) Basic execution when doubles are available (calls testDoubleIsNotNan2).
     2) Branch coverage for the "excluded" path (calls testDoubleIsNotNan2; if UNITY_EXCLUDE_DOUBLE is defined, the focal path becomes a TEST_IGNORE()).
 - The tests are written to be compatible with C++11 compilers (extern "C" for C linkage),
   and do not rely on GoogleTest or any private/private access to the SUT.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>

void testDoubleIsNotNan2(void); // Focal method declared in testunity.c

#ifdef __cplusplus
}
#endif

// ------------------------------------------------------------------
// Test 1: Basic execution path when double support is available.
// This exercises the focal method directly. If the assertion holds,
// the test passes (Unity will report pass since no assertion failed).
// ------------------------------------------------------------------
void testDoubleIsNotNan2_BasicExecution(void)
{
    // Expects that 234.9 is not NaN; when UNITY_EXCLUDE_DOUBLE is not defined,
    // testDoubleIsNotNan2() will perform TEST_ASSERT_DOUBLE_IS_NOT_NAN(234.9)
    // and should pass.
    testDoubleIsNotNan2();
}

// ------------------------------------------------------------------
// Test 2: Branch coverage for the UNITY_EXCLUDE_DOUBLE path.
// If UNITY_EXCLUDE_DOUBLE is defined, the focal method compiles to TEST_IGNORE();
// this test explicitly invokes the focal method to exercise the ignore path.
// If UNITY_EXCLUDE_DOUBLE is not defined, this test also calls the focal method
// to ensure the non-ignored path is exercised.
// ------------------------------------------------------------------
void testDoubleIsNotNan2_BranchCoverage(void)
{
    // Call the focal method to exercise the compiled branch depending on the flag.
    testDoubleIsNotNan2();

#ifdef UNITY_EXCLUDE_DOUBLE
    // Explicitly indicate coverage intent for ignore-branch when doubles are excluded.
    // Note: In Unity, TEST_IGNORE() is typically executed inside a test.
    // If testDoubleIsNotNan2() already triggers TEST_IGNORE(), this is just documentation.
    TEST_IGNORE_MESSAGE("Covered UNITY_EXCLUDE_DOUBLE path where testDoubleIsNotNan2 becomes TEST_IGNORE.");
#else
    // When doubles are available, ensure the non-NaN assertion path is executed.
    // The focal method itself asserts; reaching here means the call did not fail.
#endif
}

// ------------------------------------------------------------------
// Optional linkage check (non-fatal): ensure symbol is linkable.
// This does not interfere with Unity's runtime semantics but helps detect linkage issues early.
// ------------------------------------------------------------------
void testDoubleIsNotNan2_LinkageCheck(void)
{
    // Simply reference the symbol to ensure it is linkable.
    // This is a no-op at runtime.
    (void) testDoubleIsNotNan2;
}

// ------------------------------------------------------------------
// Main runner to execute the added tests.
// This uses Unity's test harness macros and can be compiled as a standalone test runner.
// ------------------------------------------------------------------
int main(void)
{
    UNITY_BEGIN();

    // Register tests
    RUN_TEST(testDoubleIsNotNan2_BasicExecution);
    RUN_TEST(testDoubleIsNotNan2_BranchCoverage);
    RUN_TEST(testDoubleIsNotNan2_LinkageCheck);

    return UNITY_END();
}