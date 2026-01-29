/*
  Test Suite: tearDown(void)
  Target: FOCAL_METHOD = void tearDown(void) { { CounterTeardown = 1; } }
  Context:
  - CounterTeardown is the dependent static/global flag (likely declared in the focal codebase).
  - The function under test simply assigns 1 to CounterTeardown.
  - The goal is to validate that tearDown reliably sets CounterTeardown to 1, including edge cases.
  - This test harness is implemented in C++11-compatible style (no GTest), using a minimal, non-terminating assertion approach.
  - We directly link against the focal function tearDown and the variable CounterTeardown via C linkage to ensure compatibility with C code.
  - Competence notes: we treat tearDown as a void function with C linkage, and expose CounterTeardown as extern "C" for linking.

  Candidate Keywords extracted from the focal method and its dependencies:
  - CounterTeardown
  - tearDown
  - extern "C" linkage
  - Defs.h (dependency hint)
  - Unity-like test lifecycle (mimicked via direct invocation)
  - Edge values for int (INT_MAX, INT_MIN)
  - Non-terminating assertions (EXPECT-like)

  Important: This suite uses direct invocation of the focal tearDown function rather than a full Unity harness, matching the instruction to call test methods from main when GTest is not allowed. It provides explanatory comments for each test.
*/

#include <unity.h>
#include <Defs.h>
#include <cstdio>
#include <climits>
#include <stdio.h>


// Declare the focal function and the counter with C linkage so that linking with the C/C++ under-test works properly.
extern "C" {
    void tearDown(void);
    extern int CounterTeardown;
}

// Lightweight, non-terminating assertion macro.
// Reports failures but does not abort test execution, to maximize coverage.
static int g_fail_count = 0;

#define EXPECT_EQ_INT(a, b) do { \
    int _a = (int)(a); \
    int _b = (int)(b); \
    if (_a != _b) { \
        fprintf(stderr, "EXPECT_EQ_INT failed: %d != %d (at %s:%d)\n", _a, _b, __FILE__, __LINE__); \
        ++g_fail_count; \
    } \
} while (0)

// Test 1: Ensure that calling tearDown resets CounterTeardown to 1 from a negative/initialized state.
void test_teardown_sets_counter_to_1(void)
{
    // Arrange: start from a known non-one value
    CounterTeardown = -5;

    // Act: invoke the focal method under test
    tearDown();

    // Assert: CounterTeardown should be exactly 1
    EXPECT_EQ_INT(CounterTeardown, 1);
}

// Test 2: Ensure that repeated invocations of tearDown always reset the counter back to 1 (idempotence-like check).
void test_teardown_idempotent(void)
{
    // First invocation from a non-one value
    CounterTeardown = 0;
    tearDown();
    EXPECT_EQ_INT(CounterTeardown, 1);

    // Second invocation from another non-one value
    CounterTeardown = 999;
    tearDown();
    EXPECT_EQ_INT(CounterTeardown, 1);
}

// Test 3: Boundary value checks to exercise potential integer edge handling.
void test_teardown_boundary_values(void)
{
    // Upper boundary
    CounterTeardown = INT_MAX;
    tearDown();
    EXPECT_EQ_INT(CounterTeardown, 1);

    // Lower boundary
    CounterTeardown = INT_MIN;
    tearDown();
    EXPECT_EQ_INT(CounterTeardown, 1);
}

// Simple runner: execute tests from main and report results.
// This adheres to the directive of calling test methods from main when GTest is not used.
int main(void)
{
    printf("Running tearDown() focused unit tests...\n");

    test_teardown_sets_counter_to_1();
    test_teardown_idempotent();
    test_teardown_boundary_values();

    if (g_fail_count == 0) {
        printf("ALL tests PASSED\n");
        return 0;
    } else {
        printf("%d test(s) FAILED\n", g_fail_count);
        return 1;
    }
}