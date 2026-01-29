/************************************************************
 * Test harness for the focal method: testEqualUChars
 * 
 * This C++11 file builds a small Unity-based test suite
 * that runs the focal testEqualUChars defined in testunity.c
 * and adds two additional variation tests to improve coverage
 * around unsigned char equality scenarios.
 * 
 * Notes:
 * - Unity is a C test framework; we expose C functions with C linkage
 *   to the C++ test runner via extern "C".
 * - The harness uses Unity's RUN_TEST and UnityBegin/UnityEnd.
 * - No GTest is used; tests rely on Unity's assertion macros.
 * - The added tests are designed to exercise similar code paths as
 *   testEqualUChars by using unsigned char semantics and pointer dereferencing.
 ************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// Focal test under test is defined in testunity.c
void testEqualUChars(void);
#ifdef __cplusplus
}
#endif

// Additional variation tests to increase coverage around uchar equality
#ifdef __cplusplus
extern "C" {
#endif
void testEqualUChars_Variation1(void);
void testEqualUChars_Variation2(void);
#ifdef __cplusplus
}
#endif

// Main entry point for the Unity-based test runner
int main(void)
{
    UnityBegin("TestEqualUCharsSuite");

    // Run the focal test as part of the suite
    RUN_TEST(testEqualUChars);

    // Run additional variations to cover more scenarios with uchar equality
    RUN_TEST(testEqualUChars_Variation1);
    RUN_TEST(testEqualUChars_Variation2);

    return UnityEnd();
}

/*
 * Variation 1: Mirror the original focal test's scenarios with a different
 * set of unsigned char values to verify that equality assertions pass
 * for another valid pair of equal values, including pointer dereferences.
 * Rationale: ensures that the assertion macros behave consistently for
 * distinct variables and their addresses.
 */
extern "C" void testEqualUChars_Variation1(void)
{
    unsigned char a, b;
    unsigned char *pa, *pb;

    a = 123; // arbitrary equal value within uchar range
    b = 123;
    pa = &a;
    pb = &b;

    // Basic sanity-check assertion and value equality
    TEST_ASSERT_EQUAL_INT(0, 0);     // trivial true condition to confirm framework is active
    TEST_ASSERT_EQUAL_INT(a, b);     // direct value comparison
    TEST_ASSERT_EQUAL_INT(123, b);   // literal vs variable comparison
    TEST_ASSERT_EQUAL_INT(a, 123);   // variable vs literal comparison
    TEST_ASSERT_EQUAL_INT(*pa, b);   // dereferenced pointer vs value
    TEST_ASSERT_EQUAL_INT(*pa, *pb); // dereferenced pointers equality
    TEST_ASSERT_EQUAL_INT(*pa, 123); // final dereferenced check
}

/*
 * Variation 2: Use another independent pair of equal unsigned chars and
 * validate equality through a pointer and its indirection, including
 * a pointer-to-pointer style comparison via dereferencing.
 * Rationale: exercises the same logic paths with different storage
 * to ensure robustness of equality checks across identities.
 */
extern "C" void testEqualUChars_Variation2(void)
{
    unsigned char x, y;
    unsigned char *px, *py;

    x = 0xFF; // 255
    y = 0xFF; // 255
    px = &x;
    py = &y;

    // Assertions mirroring the focal test's structure
    TEST_ASSERT_EQUAL_INT(42, 42);     // ensure integration with existing constants
    TEST_ASSERT_EQUAL_INT(x, y);         // xi == yi
    TEST_ASSERT_EQUAL_INT(0xFF, y);      // literal comparison
    TEST_ASSERT_EQUAL_INT(x, 0xFF);      // x matches literal
    TEST_ASSERT_EQUAL_INT(*px, y);       // dereferenced equality
    TEST_ASSERT_EQUAL_INT(*px, *py);     // dereferenced pointers equal
    TEST_ASSERT_EQUAL_INT(*px, 0xFF);    // final check against literal
}