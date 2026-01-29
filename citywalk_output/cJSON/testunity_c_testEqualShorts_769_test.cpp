// test_suite_equal_shorts.cpp
// A focused Unity-based test suite to exercise the focal method testEqualShorts
// and validate short-type equality scenarios via a replicated path.
// This file is designed to be compiled alongside the existing Unity framework
// and the focal test file (testunity.c) which provides the symbol `testEqualShorts`.
//
// Notes:
// - We declare testEqualShorts as extern "C" to avoid name mangling when linking
//   with the C file that defines it.
// - The suite includes:
//   1) A test that directly invokes the focal testEqualShorts() to ensure it runs
//      without aborts within the Unity harness.
//   2) A replicated reproduction of the focal test sequence for short variables and
//      pointers to exercise the same comparison paths in a separate test context.
// - Each test is annotated with comments describing its purpose and the related
//   candidate keywords observed in the focal method (short v0, v1, p0, p1, etc.).

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unity.h>


#ifdef __cplusplus
extern "C" {
#endif
// Focal method under test is defined in testunity.c
void testEqualShorts(void);
#ifdef __cplusplus
}
#endif

// -------------------------------------------------------------------------------------------------
// Test 1: Directly invoke the focal test to ensure it runs under the test harness without
// crashing or aborting (true path for the focal test).
// This validates that the existing testEqualShorts() function executes as expected
// when integrated into a broader test suite.
// Candidate Keywords: testEqualShorts, v0, v1, p0, p1, short, equality asserts.
// -------------------------------------------------------------------------------------------------
void test_RunFocalTestEqualShorts_DirectInvocation(void)
{
    // Simply call the focal test. If it aborts, Unity will record a failure.
    // This test does not attempt to modify behavior; it serves as a smoke test.
    testEqualShorts();
}

// -------------------------------------------------------------------------------------------------
// Test 2: Replicated short-equality sequence (standalone) to exercise the same
// comparison paths as the focal method, using explicit declarations and
// Unity assertions in a separate test context.
// Candidate Keywords: short v0, short v1, short *p0, short *p1, *p0, *p1, 19467, 1837, -2987.
// -------------------------------------------------------------------------------------------------
void test_ReplicateShortsEqualityPath(void)
{
    short v0, v1;
    short *p0, *p1;

    // Initialize values to exercise equality checks (short domain).
    v0 = 19467;
    v1 = 19467;

    // Pointers to the two short variables will be dereferenced in assertions.
    p0 = &v0;
    p1 = &v1;

    // Replicate the sequence from the focal method:
    // 1) Basic integer equality assertions
    TEST_ASSERT_EQUAL_INT(1837, 1837);

    // 2) Negative value equality assertion
    TEST_ASSERT_EQUAL_INT(-2987, -2987);

    // 3) Equality between two shorts
    TEST_ASSERT_EQUAL_INT(v0, v1);

    // 4) Direct literal vs. the second short
    TEST_ASSERT_EQUAL_INT(19467, v1);

    // 5) First short vs. literal
    TEST_ASSERT_EQUAL_INT(v0, 19467);

    // 6) Dereferenced pointers vs. the other short
    TEST_ASSERT_EQUAL_INT(*p0, v1);

    // 7) Dereferenced pointers vs. each other
    TEST_ASSERT_EQUAL_INT(*p0, *p1);

    // 8) Dereferenced pointer vs. literal
    TEST_ASSERT_EQUAL_INT(*p0, 19467);
}

// -------------------------------------------------------------------------------------------------
// Main entry point for this standalone suite when invoked as part of a custom runner.
// This main is provided for environments where a single translation unit drives
// the Unity test execution. If your build uses the existing testunity.c main,
// you may integrate these tests by registering them there instead.
// -------------------------------------------------------------------------------------------------
int main(void)
{
    UNITY_BEGIN("test_suite_equal_shorts");

    // Run tests that exercise the focal method and its replicated path.
    RUN_TEST(test_RunFocalTestEqualShorts_DirectInvocation);
    RUN_TEST(test_ReplicateShortsEqualityPath);

    return UNITY_END();
}