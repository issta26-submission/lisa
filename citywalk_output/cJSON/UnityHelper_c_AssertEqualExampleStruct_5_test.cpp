/*
   UnityHelper_test.cpp

   This test suite targets the focal method:
     void AssertEqualExampleStruct(const EXAMPLE_STRUCT_T expected, const EXAMPLE_STRUCT_T actual, const unsigned short line)

   The tests are designed to be compiled with C++11, but exercise a C-based Unity test
   framework. The tests are implemented in C++ but call into the C function declared in
   UnityHelper.h / implemented in UnityHelper.c.

   Coverage plan:
   - All fields equal: no assertion failures.
   - First field mismatch: first assertion fails, second passes.
   - Second field mismatch: first passes, second fails.
   - Both fields mismatch: both assertions fail.
   - Additional test with negative and large positive values to exercise edge data.

   Notes:
   - UNITY_BEGIN / RUN_TEST are used to drive tests (non-terminating Unity assertions per DOMAIN_KNOWLEDGE).
   - EXAMPLE_STRUCT_T is assumed to be a simple struct with int x; int y;, defined in UnityHelper.h or related headers.
   - Includes are wrapped in extern "C" where appropriate to ensure proper linkage with C code when compiled as C++.
*/

#include <cstddef>
#include <unity.h>
#include <string.h>
#include <UnityHelper.h>
#include <stdio.h>


extern "C" {
}

// Test 1: All fields equal -> both UNITY_TEST_ASSERT_EQUAL_INT comparisons succeed.
// This verifies the non-failing path and ensures the function compares both fields correctly.
void test_AssertEqualExampleStruct_AllFieldsEqual(void)
{
    // Arrange
    EXAMPLE_STRUCT_T expected;
    expected.x = 5;
    expected.y = 10;

    EXAMPLE_STRUCT_T actual;
    actual.x = 5;
    actual.y = 10;

    unsigned short line = 1; // line number used in the assertion messages

    // Act
    // This should pass both internal assertions without recording a failure.
    AssertEqualExampleStruct(expected, actual, line);

    // Assert: The Unity framework will record zero failures for this test.
    // Since Unity's assertion macros are non-terminating by design, there is no
    // explicit runtime check here beyond the expected successful invocation.
}

// Test 2: First field mismatch -> first assertion should fail, second should pass.
// This exercises the true/false behavior of the first predicate while exercising the second.
void test_AssertEqualExampleStruct_FirstFieldMismatch(void)
{
    // Arrange
    EXAMPLE_STRUCT_T expected;
    expected.x = 5;
    expected.y = 10;

    EXAMPLE_STRUCT_T actual;
    actual.x = 7;   // Mismatch on x
    actual.y = 10;

    unsigned short line = 2;

    // Act
    AssertEqualExampleStruct(expected, actual, line);

    // Assert: Unity should register a failure for the first field (x) while
    // the second field (y) remains equal.
}

// Test 3: Second field mismatch -> first assertion passes, second should fail.
// This validates the second predicate's behavior independently.
void test_AssertEqualExampleStruct_SecondFieldMismatch(void)
{
    // Arrange
    EXAMPLE_STRUCT_T expected;
    expected.x = 5;
    expected.y = 10;

    EXAMPLE_STRUCT_T actual;
    actual.x = 5;   // Match on x
    actual.y = 8;   // Mismatch on y

    unsigned short line = 3;

    // Act
    AssertEqualExampleStruct(expected, actual, line);

    // Assert: Unity should register a failure for the second field (y) while
    // the first field (x) remains equal.
}

// Test 4: Both fields mismatch -> both assertions should fail.
// This ensures the function reports failures for both fields when both differ.
void test_AssertEqualExampleStruct_BothMismatch(void)
{
    // Arrange
    EXAMPLE_STRUCT_T expected;
    expected.x = 5;
    expected.y = 10;

    EXAMPLE_STRUCT_T actual;
    actual.x = 8;   // Mismatch on x
    actual.y = 3;   // Mismatch on y

    unsigned short line = 4;

    // Act
    AssertEqualExampleStruct(expected, actual, line);

    // Assert: Unity should register failures for both fields.
}

// Test 5: Edge case with negative and large values to ensure proper handling of int ranges.
// This extends coverage to non-trivial numeric values while still validating field-wise comparison.
void test_AssertEqualExampleStruct_WithNegAndLarge(void)
{
    // Arrange
    EXAMPLE_STRUCT_T expected;
    expected.x = -100;
    expected.y = 2000;

    EXAMPLE_STRUCT_T actual;
    actual.x = -100;
    actual.y = 1999; // Slight mismatch on y to exercise comparison

    unsigned short line = 5;

    // Act
    AssertEqualExampleStruct(expected, actual, line);

    // Assert: First field should pass; second field should fail due to mismatch.
}

// Forward declarations to satisfy the Unity RUN_TEST prototype requirement.
extern void test_AssertEqualExampleStruct_AllFieldsEqual(void);
extern void test_AssertEqualExampleStruct_FirstFieldMismatch(void);
extern void test_AssertEqualExampleStruct_SecondFieldMismatch(void);
extern void test_AssertEqualExampleStruct_BothMismatch(void);
extern void test_AssertEqualExampleStruct_WithNegAndLarge(void);

// Main test runner
int main(void)
{
    // Unity test harness initialization
    UNITY_BEGIN();

    // Register test cases
    RUN_TEST(test_AssertEqualExampleStruct_AllFieldsEqual);
    RUN_TEST(test_AssertEqualExampleStruct_FirstFieldMismatch);
    RUN_TEST(test_AssertEqualExampleStruct_SecondFieldMismatch);
    RUN_TEST(test_AssertEqualExampleStruct_BothMismatch);
    RUN_TEST(test_AssertEqualExampleStruct_WithNegAndLarge);

    // Finalize Unity test run and return status
    return UNITY_END();
}