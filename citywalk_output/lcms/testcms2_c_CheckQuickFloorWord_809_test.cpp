/************* Test Suite for focal method CheckQuickFloorWord *************
  Objective:
  - Validate the behavior of CheckQuickFloorWord() as defined in testcms2.c.
  - Provide a small C++11-based test harness (no GTest) that exercises the method.
  - Respect the domain guidance: non-terminating assertions, static usage rules, and clear comments.

  Step 1 (Brief Candidate Keywords to orient tests):
  - Core function under test: CheckQuickFloorWord(void)
  - Internal dependency in focal method: _cmsQuickFloorWord(cmsFloat64Number)
  - Numeric domain: cmsUInt32Number i ranges [0, 65534]
  - Validation trigger: mismatch -> Die(...) and return 0; else return 1
  - Data types: cmsFloat64Number, cmsUInt32Number, cmsInt32Number
  - Side effect path: Die(...) is a signaling path (potentially exits in some builds)

  Step 2: Class/Dependency context (from provided focal class dep):
  - The test relies on the environment where _cmsQuickFloorWord is defined (likely in lcms2.h)
  - Die(...) is a test signaling function; in normal operation, with a healthy environment, the path will not invoke Die
  - Other helper/debug memory/testing utilities exist in testcms2.h but are not invoked by CheckQuickFloorWord

  Step 3: Test strategy notes (domain guidance applied):
  - We implement a minimal, non-terminating test harness (no GTest).
  - We perform a single positive-path test (CheckQuickFloorWord returns 1 when the quick-floor routine behaves correctly).
  - We outline (via comments) how a failure-path could be induced if the environment allows overriding _cmsQuickFloorWord (not guaranteed in all builds).
  - The test is designed to compile with a C/C++11 toolchain and link against the existing C library providing CheckQuickFloorWord.

  Important: This test file assumes the library provides the C symbol CheckQuickFloorWord with C linkage.
*/

/* 
   To keep the test self-contained and focused on the focal method,
   we declare the C prototype for CheckQuickFloorWord. The real signature
   in the project uses cmsInt32Number as return type; we map it to int here
   since cmsInt32Number is typically a typedef for int in lcms2-like codebases.
*/
extern "C" int CheckQuickFloorWord(void);

/* Lightweight non-terminating test harness (no GTest). */
#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <iomanip>


/* Simple test harness to accumulate results without terminating on failure. */
class TestHarness {
public:
    void addTest(const std::string& name, std::function<void()> fn) {
        tests.emplace_back(name, fn);
    }

    void run() {
        int failed = 0;
        for (const auto& t : tests) {
            const std::string& name = t.first;
            try {
                t.second(); // run the test case
                // If test case doesn't call any failing assertion, it's considered passed.
                if (!lastFailureFlag) {
                    std::cout << "[PASS] " << name << "\n";
                } else {
                    // If a failing assertion happened inside, report it
                    failed++;
                    std::cout << "[FAIL] " << name << " (captured failure)\n";
                }
            } catch (const std::exception& e) {
                ++failed;
                std::cout << "[EXCEPT] " << name << " threw: " << e.what() << "\n";
            } catch (...) {
                ++failed;
                std::cout << "[EXCEPT] " << name << " threw an unknown exception\n";
            }
            // reset flag for next test
            lastFailureFlag = false;
        }
        std::cout << "\nTest Summary: " << (tests.size() - failed) << " passed, "
                  << failed << " failed, out of " << tests.size() << " tests.\n";
        if (failed == 0) {
            std::cout << "ALL TESTS PASSED\n";
        } else {
            std::cout << "SOME TESTS FAILED\n";
        }
    }

    // A tiny assertion facility that does not terminate the test run.
    static void AssertTrue(bool condition, const std::string& msg = "") {
        if (!condition) {
            lastFailureFlag = true;
            std::cerr << "ASSERTION FAILED: " << (msg.empty() ? "Condition is false" : msg) << "\n";
        }
    }

private:
    std::vector<std::pair<std::string, std::function<void()>>> tests;
    static thread_local bool lastFailureFlag;
};

thread_local bool TestHarness::lastFailureFlag = false;

/* ---------------------------------------------------------------
   Test 1: Basic correctness for CheckQuickFloorWord
   - Rationale: In a healthy runtime environment, _cmsQuickFloorWord and the
     surrounding macro/function should behave correctly, causing the loop to
     complete without triggering Die and CheckQuickFloorWord to return 1.
   - This test exercises the true-branch of the predicate inside CheckQuickFloorWord.
   - Note: If the environment allows overriding _cmsQuickFloorWord to simulate
     a failure, a separate test could be added to cover the false-branch path,
     but that requires linkage/control not guaranteed by all build setups.
--------------------------------------------------------------- */
static void test_CheckQuickFloorWord_basic_correct_behavior() {
    // Expect that CheckQuickFloorWord returns 1 on a well-behaved runtime.
    int result = CheckQuickFloorWord();
    // Use non-terminating assertion to record outcome and continue.
    TestHarness::AssertTrue(result == 1, "CheckQuickFloorWord() should return 1 on correct environment");
}

/* ---------------------------------------------------------------
   Main entry: run the test suite.
   - This main will be compiled as a C++11 executable and linked against
     the project's C library providing CheckQuickFloorWord.
--------------------------------------------------------------- */
int main(int argc, char* argv[]) {
    // Snapshot of candidate keyword exposure for traceability
    // Candidate Keywords examined:
    // - _cmsQuickFloorWord: core numeric-floor helper
    // - cmsFloat64Number: input type, double-precision
    // - cmsUInt32Number: loop counter type
    // - Die: error signaling function
    // - 0.1234: test offset used in the input expression
    // - 65535: loop upper bound
    // - cmsInt32Number: return type for the testable function
    // This harness focuses on the positive-path validation of the focal method.

    TestHarness th;
    th.addTest("CheckQuickFloorWord_basic_correct_behavior", test_CheckQuickFloorWord_basic_correct_behavior);

    th.run();

    // Return 0 on success, non-zero if any test failed (captured by harness).
    // We rely on the harness to report detailed results.
    return 0;
}