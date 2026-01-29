/*
  Unit Test Suite for the focal method:
  cmsInt32Number CheckQuickFloor(void)

  Step 1 - Program Understanding (represented in tests as Candidate Keywords)
  - Core dependencies: _cmsQuickFloor, Die
  - Boundary values used by the focal check: 1.234, 32767.234, -1.234, -32767.1
  - Behavior: If any _cmsQuickFloor(...) result does not match the expected integer, Die(...) is invoked and the function returns 0; otherwise it returns 1.
  - The provided class dependencies (testcms2.h) supply stubs for test infrastructure, including Die, etc.

  Step 2/3 - Test Suite Generation and Refinement
  - Implement a lightweight, non-terminating (non-fatal) assertion framework in C++11 (no GTest).
  - Expose the focal function CheckQuickFloor via extern "C" to avoid name mangling when called from C++.
  - Provide explanatory comments for each unit test.
  - Import dependencies via testcms2.h as per the project’s dependency structure.
  - Coverage focus: ensure that the tested path executes CheckQuickFloor and validates expected behavior under standard environment assumptions. Note: due to environment constraints, triggering the false branch (where Die is invoked) may not be programmatically forced here; the test verifies the normal, expected path returns 1.

  Domain Knowledge considerations followed:
  - Use only C++11 standard library.
  - Do not rely on a GTest framework.
  - Use a minimal test harness that reports results and continues execution after failures.
  - Access to static/file-scope helpers is not required; focus remains on the focal method’s public interface.
  - All tests are executed from main() in this test file.

  The test harness below relies on the provided testcms2.h, which exposes the required symbols (including a stubbed Die function) suitable for unit testing in this environment.
*/

#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>


// Candidate Keywords (for reference/documentation):
// - _cmsQuickFloor
// - Die
// - CMS_DONT_USE_FAST_FLOOR toggle (commented in lcms2.h)
// - 1.234, 32767.234, -1.234, -32767.1
// - cmsInt32Number, cmsUInt16Number, cmsFloat64Number

extern "C" {
    // Forward declaration of the focal function under test.
    // The implementation is provided by the tested project; we call it as a C-style function.
    cmsInt32Number CheckQuickFloor(void);
}

// The testcms2.h dependency provides the test harness functions and stubs required by the focal method.
// It is included here to mirror the project’s Step 2 dependency guidance.

// Lightweight non-terminating test framework (no GTest)
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

// Simple assertion macro: expected == actual
#define EXPECT_EQ_INT(actual, expected, msg) do {                \
    ++g_tests_run;                                                \
    int _act = (int)(actual);                                    \
    int _exp = (int)(expected);                                   \
    if (_act != _exp) {                                          \
        std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__  \
                  << " " << (msg)                                      \
                  << " | expected: " << _exp                          \
                  << ", actual: " << _act << std::endl;             \
        ++g_tests_failed;                                         \
    } else {                                                       \
        ++g_tests_passed;                                         \
    }                                                              \
} while(0)

// Helper macro to mark a test function execution
#define RUN_TEST(fn) do { fn(); } while(0)

/*
  Test 1: CheckQuickFloor_Returns1
  Intent: Validate that CheckQuickFloor() returns 1 under the environment where
  _cmsQuickFloor behaves according to the standard expectations (as used in the
  original code's test scenario).
  Rationale: This covers the "true" branch of the predicate in the focal method.
  Limitations: The ability to force the "false" branch (Die invoked) is highly
  environment-dependent due to macro/inline behavior of _cmsQuickFloor; this test
  captures the expected successful path.
*/
void test_CheckQuickFloor_Returns1(void)
{
    cmsInt32Number result = CheckQuickFloor();
    EXPECT_EQ_INT(result, 1, "CheckQuickFloor should return 1 on standard environment");
}

/*
  Test 2: CheckQuickFloor_NoCrash
  Intent: Ensure that invoking CheckQuickFloor does not lead to a crash or undefined
  behavior in this test harness (i.e., that the function and its dependencies are
  properly linked and callable from C++11 test code).
  Rationale: This test exercises the integration surface and verifies the function
  can be invoked and returns a value without side effects outside the expected contract.
*/
void test_CheckQuickFloor_NoCrash(void)
{
    // Call multiple times to ensure stability across invocations.
    for (int i = 0; i < 3; ++i) {
        cmsInt32Number res = CheckQuickFloor();
        // Expect a valid integer return (0 or 1). The canonical path should be 1;
        // we do not enforce 1 here to confirm function returns consistently.
        if (res != 0 && res != 1) {
            std::cerr << "[FAILED] test_CheckQuickFloor_NoCrash: unexpected return value "
                      << res << std::endl;
            ++g_tests_failed;
        } else {
            ++g_tests_passed;
        }
        ++g_tests_run;
    }
}

/*
  Step 3 Compliance Note:
  The current tests exercise the focal function's public interface and validate
  the expected path when the environment's _cmsQuickFloor semantics are aligned
  with the test design. Due to the dependency on internal macros and potential
  architecture-specific floor semantics, forcing the Die path from CheckQuickFloor
  may not be reliably achievable in this isolated test harness without altering
  the library's build configuration (e.g., CMS_DONT_USE_FAST_FLOOR toggle).
  The included tests are designed to maximize code coverage of the focal path and
  to exercise the test infrastructure and dependency imports as described in the
  problem statement.

  Main
*/
int main(int argc, char* argv[])
{
    // Articulate domain knowledge and candidate keyword mapping in output.
    std::vector<std::string> keywords = {
        "_ cmsQuickFloor", "_ cmsQuickFloor semantics",
        "CheckQuickFloor", "Die", "CMS_DONT_USE_FAST_FLOOR",
        "1.234", "32767.234", "-1.234", "-32767.1"
    };
    std::cout << "Unit Test Suite: CheckQuickFloor (no GTest)\n";
    std::cout << "Candidate Keywords alignment: ";
    for (const auto& k : keywords) std::cout << k << " ";
    std::cout << "\n" << std::endl;

    // Run tests
    RUN_TEST(test_CheckQuickFloor_Returns1);
    RUN_TEST(test_CheckQuickFloor_NoCrash);

    // Summary
    std::cout << "\nTest Summary: " << g_tests_passed << " passed, "
              << g_tests_failed << " failed, "
              << g_tests_run << " total tests." << std::endl;

    // Return non-zero if any test failed
    return g_tests_failed > 0 ? 1 : 0;
}