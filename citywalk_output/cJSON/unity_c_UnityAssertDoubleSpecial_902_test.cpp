/*
  Test Suite for UnityAssertDoubleSpecial in unity.c
  - This test file targets the Unity C framework function:
      void UnityAssertDoubleSpecial(const UNITY_DOUBLE actual,
                                   const char* msg,
                                   const UNITY_LINE_TYPE lineNumber,
                                   const UNITY_FLOAT_TRAIT_T style)
  - The tests are written in C++11 but exercise the C function via extern "C".
  - Tests focus on exercising passing scenarios (INF, NEG_INF, NAN, DET) and an
    invalid style default path to ensure code paths are exercised.
  - Explanatory comments accompany each unit test describing intent and coverage goals.
  - This suite uses the Unity testing framework semantics (UnityBegin, RUN_TEST, UnityEnd).
*/

#include <stddef.h>
#include <limits>
#include <cmath>
#include <unity.h>


// Ensure C linkage for Unity's function and types when called from C++
extern "C" {
}

// Declare the focal function from unity.c with C linkage
extern "C" void UnityAssertDoubleSpecial(const UNITY_DOUBLE actual,
                                        const char* msg,
                                        const UNITY_LINE_TYPE lineNumber,
                                        const UNITY_FLOAT_TRAIT_T style);

// If UNITY_FLOAT_TRAIT_T is defined as a specific type, the above prototype matches
// the Unity framework's typedefs used in unity.c. We rely on unity.h for these typedefs.

/*
  Test 1: Infinitely positive value should be recognized as INF trait when style = UNITY_FLOAT_IS_INF
  Intent: Exercise the INF branch where actual is +infinity and ensure the function handles
          the case where the trait matches (i.e., should_be_trait aligns with is_trait).
*/
void test_UnityAssertDoubleSpecial_INF_Positive_Pass(void)
{
    // Use C++ to produce a reliable +infinity value
    double pos_inf = std::numeric_limits<double>::infinity();

    // We expect this call to pass (i.e., is_trait == should_be_trait) for the INF trait.
    // The message and line number are for diagnostic purposes.
    UnityAssertDoubleSpecial(pos_inf, "POS_INF should be recognized as INF trait", 100, UNITY_FLOAT_IS_INF);
}

/*
  Test 2: Infinitely negative value should be recognized as NEG_INF trait when style = UNITY_FLOAT_IS_NEG_INF
  Intent: Exercise the NEG_INF INF branch with a negative infinity input.
*/
void test_UnityAssertDoubleSpecial_NEG_INF_Pass(void)
{
    double neg_inf = -std::numeric_limits<double>::infinity();

    // Expect the NEG_INF trait to match for a negative infinity value
    UnityAssertDoubleSpecial(neg_inf, "NEG_INF should be recognized as NEG_INF trait", 101, UNITY_FLOAT_IS_NEG_INF);
}

/*
  Test 3: NaN value should be recognized as NAN trait when style = UNITY_FLOAT_IS_NAN
  Intent: Exercise the NAN branch with a NaN input.
*/
void test_UnityAssertDoubleSpecial_NAN_Pass(void)
{
    double nan_val = std::numeric_limits<double>::quiet_NaN();

    // Expect the NAN trait to match for a NaN value
    UnityAssertDoubleSpecial(nan_val, "NAN should be recognized as NAN trait", 102, UNITY_FLOAT_IS_NAN);
}

/*
  Test 4: Determinate finite value should be recognized as DET trait when style = UNITY_FLOAT_IS_DET
  Intent: Exercise the DET branch with a finite, non-inf, non-NaN value.
*/
void test_UnityAssertDoubleSpecial_DET_Pass(void)
{
    double finite_val = 3.14159265358979323846;

    // For a finite, non-inf, non-NaN value, DET trait should evaluate to true
    // and should_be_trait should align with is_trait for DET case.
    UnityAssertDoubleSpecial(finite_val, "DET should be recognized for finite real number", 103, UNITY_FLOAT_IS_DET);
}

/*
  Test 5: Invalid/Unknown style should trigger the default path in the switch-case
  and, given the implementation, should cause a failure path (trait mismatch by design).
  Intent: Exercise the default branch to increase coverage of code-path handling.
  Note: This test is expected to fail if Unity's fail-bail behavior is active.
*/
void test_UnityAssertDoubleSpecial_InvalidStyle_Fail(void)
{
    double some_val = 0.0;

    // Pick an obviously invalid style value by using an unlikely large value
    // The exact numeric value doesn't matter as long as it doesn't match a defined case.
    UnityAssertDoubleSpecial(some_val, "Invalid style should hit default branch", 104, (UNITY_FLOAT_TRAIT_T)0xDEADBEEF);
}

/*
  Test Suite entrypoint and runner
  - Uses Unity's standard macros RUN_TEST. If your environment uses a different runner,
    adapt accordingly.
*/
int main(void)
{
    UnityBegin("unity_double_special_tests.cpp");

    RUN_TEST(test_UnityAssertDoubleSpecial_INF_Positive_Pass);
    RUN_TEST(test_UnityAssertDoubleSpecial_NEG_INF_Pass);
    RUN_TEST(test_UnityAssertDoubleSpecial_NAN_Pass);
    RUN_TEST(test_UnityAssertDoubleSpecial_DET_Pass);
    RUN_TEST(test_UnityAssertDoubleSpecial_InvalidStyle_Fail);

    return UnityEnd();
}