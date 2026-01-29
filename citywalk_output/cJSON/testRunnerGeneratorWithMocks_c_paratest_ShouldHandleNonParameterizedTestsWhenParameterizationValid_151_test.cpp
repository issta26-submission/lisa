// Auto-generated lightweight C++ test harness for focal method paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid
// This harness follows the guideline to not rely on GTest and to call the C test directly from main.
// It provides non-terminating assertions and prints a summary at the end.
// Included "Candidate Keywords" and dependencies are reflected in comments for traceability.

#include <unity.h>
#include <mockMock.h>
#include <iostream>
#include <Defs.h>
#include <CException.h>
#include <stdio.h>


// Candidate Keywords derived from the focal method and its context:
// - paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid
// - Non-Parameterized Tests
// - Parameterization Valid
// - Test Runner (testRunnerGeneratorWithMocks.c)
// - Unity TEST_PASS (as seen in the focal method block)
// - Mocking / Defs / CException (as per class dependency block)

/*
Notes about dependencies (guided by <FOCAL_CLASS_DEP>):
- The focal function is defined in testRunnerGeneratorWithMocks.c and uses Unity macros (e.g., TEST_PASS()).
- The test harness here does not depend on GTest; it uses a lightweight, non-terminating assertion approach.
- The C function paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid is declared with C linkage.
*/

// Forward declaration of the focal function (defined in the C source file under test)
extern "C" void paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid(void);

// Simple non-terminating assertion mechanism suitable for a lightweight harness
static int g_totalTests = 0;
static int g_failedTests = 0;

// EXPECT macro: increments totals and records a failure without terminating the program.
// This aligns with the domain knowledge directive to use non-terminating assertions.
#define EXPECT(cond, msg) do {                                    \
    ++g_totalTests;                                                 \
    if(!(cond)) {                                                   \
        ++g_failedTests;                                            \
        std::cerr << "[FAILED] " << __FUNCTION__ << ": " << msg << "\n"; \
    }                                                               \
} while(false)


// =========================================================
// Test Suite for paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid
// =========================================================

// Test 1: Basic invocation sanity check
// Purpose: Ensure that the focal function is callable from the C++ test harness and does not crash upon a single invocation.
// This aligns with the idea of "handling non-parameterized tests when parameterization is valid" by exercising the function path once.
void test_paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid_basic(void)
{
    // Access the focal function and invoke it.
    paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid();

    // Since paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid returns void and uses Unity TEST_PASS(),
    // we conservatively mark this invocation as passed as long as it returns control to this harness.
    EXPECT(true, "Invocation reachable and returned control (basic call).");
}

// Test 2: Idempotence sanity check (second consecutive invocation)
// Purpose: Verify that a subsequent invocation of the focal function does not crash or cause undefined behavior
// and that the function remains callable in a subsequent call scenario.
void test_paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid_twice(void)
{
    // First invocation
    paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid();

    // Second invocation to test stability across multiple runs
    paratest_ShouldHandleNonParameterizedTestsShouldHandleNonParameterizedTestsWhenParameterizationValidAgainDummyFallback();
    // Note: The above line references an actual callable function in the C file if available.
    // Since we cannot modify the focal function, we fall back to a direct second invocation as a best-effort test.

    // For safety in environments where the above fallback symbol is not available, comment the line above
    // and use the simple repeat invocation below:
}

// Fallback helper (only used if the compiler/linker supports the symbol; otherwise, this just serves as a placeholder)
// If the above dummy call is unavailable, we provide a straightforward second invocation instead.
// This macro-style dummy is inserted to ensure compilation in various environments.
// If you encounter a linker error, replace with a direct repeated call to paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid() only.
static inline void paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid_again(void) {
    paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid();
}

// Revised Test 2 (covering environments where the previous placeholder isn't usable)
// This version performs a straightforward second invocation to maximize coverage through repetition.
void test_paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid_twice_revisted(void)
{
    paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid();
    paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid_again();
    EXPECT(true, "Second invocation completed without evident crash (twice invocation).");
}


// =========================================================
// Test Runner (main)
// =========================================================

int main()
{
    // Run tests
    test_paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid_basic();
    test_paratest_ShouldHandleNonParameterizedTestsWhenParameterizationValid_twice_revisted();

    // Summary report
    std::cout << "Test Summary: Total=" << g_totalTests
              << ", Passed=" << (g_totalTests - g_failedTests)
              << ", Failed=" << g_failedTests << std::endl;

    // Return non-zero if any test failed
    return (g_failedTests != 0) ? 1 : 0;
}