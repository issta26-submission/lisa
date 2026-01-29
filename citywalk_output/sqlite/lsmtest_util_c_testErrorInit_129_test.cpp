/*
Unit test suite for the focal method: testErrorInit
Context:
- The function assigns a global g's fields from its arguments.
- We assume g and its fields (argc, argv) are defined in lsmtest.h (as in the provided class dependency section).
- We provide a small C++11 test harness (no GoogleTest) that calls testErrorInit and verifies the effects on g.
- Tests are designed to exercise typical, boundary, and repeated invocations to ensure correct behavior and coverage.

Notes:
- We use a lightweight, non-terminating assertion mechanism to allow all tests to run and report.
- Tests are executed from main() as required when gtest is not allowed.
*/

#include <string.h>
#include <lsmtest.h>
#include <iomanip>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>


/* Bring in the C test utilities and g structure with C linkage. */
extern "C" {
}

/* Simple non-terminating test result reporting infrastructure */
static int g_totalTests = 0;
static int g_failedTests = 0;

/* Integer equality assertion with non-terminating behavior */
#define EXPECT_EQ_INT(actual, expected, label) do { \
    ++g_totalTests; \
    if ((actual) != (expected)) { \
        ++g_failedTests; \
        std::cerr << "[FAIL] " << label \
                  << " | Expected: " << (expected) \
                  << " | Actual: " << (actual) << std::endl; \
    } else { \
        std::cout << "[OK]   " << label << std::endl; \
    } \
} while(0)

/* Pointer equality assertion (addresses) with non-terminating behavior */
#define EXPECT_PTR_EQ(actual, expected, label) do { \
    ++g_totalTests; \
    if ((void*)(actual) != (void*)(expected)) { \
        ++g_failedTests; \
        std::cerr << "[FAIL] " << label \
                  << " | Expected: " << static_cast<void*>(expected) \
                  << " | Actual: " << static_cast<void*>(actual) << std::endl; \
    } else { \
        std::cout << "[OK]   " << label << std::endl; \
    } \
} while(0)

/* Test 1: Call testErrorInit with null argv and zero argc
   Expectation:
   - g.argc should be 0
   - g.argv should be nullptr
*/
static void testErrorInit_withNullArgs(void) {
    // Domain scenario: minimal, zero-arg invocation
    int argc = 0;
    char **argv = nullptr;

    testErrorInit(argc, argv);

    EXPECT_EQ_INT(g.argc, argc, "testErrorInit should store argc correctly (0)");
    EXPECT_PTR_EQ(g.argv, argv, "testErrorInit should store argv pointer correctly (nullptr)");
}

/* Test 2: Call testErrorInit with a non-empty argv array
   Expectation:
   - g.argc should equal provided argc
   - g.argv should point to the exact argv array passed in
*/
static void testErrorInit_withNonNullArgs(void) {
    char *arg0 = const_cast<char*>("program");
    char *arg1 = const_cast<char*>("--option");
    char *argv2[] = { arg0, arg1, nullptr };

    int argc = 2;
    testErrorInit(argc, argv2);

    EXPECT_EQ_INT(g.argc, argc, "testErrorInit should store argc correctly (non-zero)");
    EXPECT_PTR_EQ(g.argv, argv2, "testErrorInit should store argv pointer correctly (non-null)");
}

/* Test 3: Re-invocation to ensure g gets updated with new values
   This checks that subsequent calls overwrite previous state as expected. */
static void testErrorInit_reinvokeUpdatesState(void) {
    char *argA[] = { const_cast<char*>("p"), const_cast<char*>("a"), nullptr };
    int argcA = 1; // simulate single argument

    testErrorInit(argcA, argA);
    EXPECT_EQ_INT(g.argc, argcA, "testErrorInit updates argc on reinvocation (1)");
    EXPECT_PTR_EQ(g.argv, argA, "testErrorInit updates argv pointer on reinvocation");

    // Second reinvocation with different values
    char *argB[] = { const_cast<char*>("program"), const_cast<char*>("b"), const_cast<char*>("c"), nullptr };
    int argcB = 3;
    testErrorInit(argcB, argB);
    EXPECT_EQ_INT(g.argc, argcB, "testErrorInit updates argc on second reinvocation (3)");
    EXPECT_PTR_EQ(g.argv, argB, "testErrorInit updates argv pointer on second reinvocation");
}

/* Runner to execute all tests and summarize results */
static void runAllTests(void) {
    std::cout << "Starting testErrorInit test suite...\n";
    testErrorInit_withNullArgs();
    testErrorInit_withNonNullArgs();
    testErrorInit_reinvokeUpdatesState();
    std::cout << "Finished testErrorInit tests. Total: " << g_totalTests
              << " Failed: " << g_failedTests << "\n";
}

/* Entry point for the test executable */
int main(void) {
    runAllTests();
    // Return non-zero if any test failed to indicate issues during CI runs.
    return (g_failedTests == 0) ? 0 : 1;
}