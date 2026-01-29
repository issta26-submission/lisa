// Test suite for the focal method testNotEqualMemoryEachEqual1
// This suite focuses on validating the behavior of TEST_ASSERT_EACH_EQUAL_MEMORY
// when a mismatch exists in different blocks of memory. The tests are built to
// run with the Unity framework (C-style) but compiled with a C++11 compiler.

#include <cstdint>
#include <cstddef>
#include <unity.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>


extern "C" {
}

// Lightweight stubs for the spy/output helpers required by Unity's EXPECT_ABORT_BEGIN
// and related macros. In the original project these are defined in testunity.c.
// Providing minimal stubs here ensures the test can compile and link in a
// standalone C++11 environment without modifying the Unity core behavior.

extern "C" void startPutcharSpy(void) { /* no-op stub */ }
extern "C" void endPutcharSpy(void)   { /* no-op stub */ }
extern "C" char* getBufferPutcharSpy(void) { return nullptr; }

// Static state mirrors used by the original macros to track test state.
// They are kept minimal for the standalone test harness.

static int SetToOneToFailInTearDown;
static int SetToOneMeanWeAlreadyCheckedThisGuy;

// Forward declarations for setUp/tearDown hooks (Unity convention)
void setUp(void);
void tearDown(void);

// Minimal implementations to satisfy the test harness when used standalone.
// These are inert but maintain compatibility with the macros' expectations.

void setUp(void)
{
    SetToOneToFailInTearDown = 0;
    SetToOneMeanWeAlreadyCheckedThisGuy = 0;
}

void tearDown(void)
{
    endPutcharSpy();
    // If a failure happened and tearDown wasn't supposed to suppress it, report gracefully.
    if ((SetToOneMeanWeAlreadyCheckedThisGuy == 0) && (Unity.CurrentTestFailed > 0))
    {
        UnityPrint(": [[[[ Test Should Have Passed But Did Not ]]]]");
        UNITY_OUTPUT_CHAR('\n');
    }
}

// The focal test variants (each corresponds to a different block-mismatch scenario).
// We mirror the original testNotEqualMemoryEachEqual1 semantics but provide
// two additional variants to exercise first/second/third block differences.

void testNotEqualMemoryEachEqual1(void)
{
    // Scenario 1: Mismatch in the first block of memory
    // p0 is {1, 8, 987, -2}
    // p1 is {9, 8, 987, -2, 1, 8, 987, -2, 1, 8, 987, -2}
    // The first 4 bytes (first block) differ, so the comparison should fail.
    int p0[] = {1, 8, 987, -2};
    int p1[] = {9, 8, 987, -2, 1, 8, 987, -2, 1, 8, 987, -2};

    // Expect the assertion to fail (non-terminating wrapper) without crashing the test runner.
    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_MEMORY(p0, p1, sizeof(int)*4, 3);
    VERIFY_FAILS_END
}

void testNotEqualMemoryEachEqual2(void)
{
    // Scenario 2: Mismatch in the second block of memory
    // p0 is {1, 8, 987, -2}
    // p1 is {1, 8, 987, -2, 9, 8, 987, -2, 1, 8, 987, -2}
    // Second block (indices 4..7) differs.
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, -2, 9, 8, 987, -2, 1, 8, 987, -2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_MEMORY(p0, p1, sizeof(int)*4, 3);
    VERIFY_FAILS_END
}

void testNotEqualMemoryEachEqual3(void)
{
    // Scenario 3: Mismatch in the third block of memory
    // p0 is {1, 8, 987, -2}
    // p1 is {1, 8, 987, -2, 1, 8, 987, -2, 1, 8, 999, -2}
    // Third block (indices 8..11) differs.
    int p0[] = {1, 8, 987, -2};
    int p1[] = {1, 8, 987, -2, 1, 8, 987, -2, 1, 8, 999, -2};

    EXPECT_ABORT_BEGIN
    TEST_ASSERT_EACH_EQUAL_MEMORY(p0, p1, sizeof(int)*4, 3);
    VERIFY_FAILS_END
}

// Lightweight test runner for the three scenarios above.
// This main is suitable for a standalone build under C++11.

int main(void)
{
    // Unity-specific test runner
    UnityBegin("test_not_equal_memory_each_equal_suite");

    // Register tests
    RUN_TEST(testNotEqualMemoryEachEqual1);
    RUN_TEST(testNotEqualMemoryEachEqual2);
    RUN_TEST(testNotEqualMemoryEachEqual3);

    int testResult = UnityEnd();
    return testResult;
}

// Notes:
// - This test suite reproduces the focal method's intended behavior by executing
//   the same "memory block equality across multiple blocks" scenario using
//   Unity's TEST_ASSERT_EACH_EQUAL_MEMORY, wrapped with EXPECT_ABORT_BEGIN
//   / VERIFY_FAILS_END to capture expected failures without terminating the
//   entire test run.
// - The stubs for startPutcharSpy and endPutcharSpy allow the test to link in
//   environments where the full Unity spy infrastructure is not present.
// - Each test's comments describe which block of the multi-block comparison is
//   being stressed (first, second, or third block), ensuring targeted
//   coverage of the failure pathways.
// - This file is designed to be compiled alongside the Unity framework under C++11
//   without requiring GTest or additional mocking frameworks.