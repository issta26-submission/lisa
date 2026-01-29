// Unit test suite for EqualLongTag (focal method) with its core dependencies.
// This test suite uses a lightweight in-process harness (no Google Test) and
// relies on test doubles to mock CmpLongTag and FatalError behavior.
// The goal is to exercise true/false branches of the dependency predicates
// and verify that EqualLongTag responds correctly without terminating the test
// run prematurely when possible (non-terminating assertions are preferred in tests).

// The test suite follows the provided FOCAL_METOD and FOCAL_CLASS_DEP structure:
// - EqualLongTag(TIFF* tif1, TIFF* tif2, int tag, const char* Error) calls
//   CmpLongTag and, on mismatch, FatalError("%s is different", Error).
// - We provide mock implementations of CmpLongTag and FatalError to control
//   behavior during testing and to capture error messages for verification.
// - The tests exercise both the "equal" (true branch) and "not equal" (false branch)
//   scenarios.

// Step 1: Candidate Keywords extracted from the focal method and dependencies
// - EqualLongTag
// - CmpLongTag
// - FatalError
// - TIFF
// - tag (long/integer tag being compared)
// (These keywords guide test case intent and coverage focus.)

#include <cstdio>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>
#include <cstdarg>
#include <utils.h>
#include <tiffio.h>


// Forward declaration of the focal method under test is assumed to be provided
// by the production code (tifdiff.c) and linked into the test binary.
// We declare it here so tests can call it directly.
// The exact signature must match the production: void EqualLongTag(TIFF*, TIFF*, int, const char*)
// Note: In a real test environment, ensure that the test binary links against the
// production object file that defines EqualLongTag.
// extern "C" void EqualLongTag(TIFF* tif1, TIFF* tif2, int tag, const char* Error);

// For the purpose of this self-contained test scaffold, we provide a minimal
// dummy TIFF type to stand in for the actual libtiff TIFF structure.
// In a real environment, use #include <tiffio.h> and the real TIFF type.
struct TIFF {};

// Global state for the mock CmpLongTag and FatalError
static int g_mock_cmp_result = 1;           // Controls the return value of CmpLongTag
static bool g_fatal_called = false;         // Recorded if FatalError was invoked
static std::string g_last_error_message;    // Captured error message from FatalError

// Mock implementation of CmpLongTag to be used by EqualLongTag during tests.
// This mock ignores TIFF content and returns a value controlled by g_mock_cmp_result.
extern "C" int CmpLongTag(TIFF* /*tif1*/, TIFF* /*tif2*/, int /*tag*/) {
    return g_mock_cmp_result;
}

// Mock FatalError that simulates the real fatal error handler without terminating tests.
// It formats the message, records it, and then throws an exception to allow
// test harness to catch and verify error conditions gracefully.
extern "C" void FatalError(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, ap);
    va_end(ap);
    g_last_error_message = buffer;
    g_fatal_called = true;

    // Throw a non-terminating exception so tests can continue to run
    // and verify the exact error message produced by EqualLongTag path.
    throw std::runtime_error(buffer);
}

// Prototype of the focal function under test (the production code).
// In a real test, link against the compiled tifdiff.o (or tifdiff.lib) that provides
// EqualLongTag. Here we declare it to enable direct invocation from tests.
extern "C" void EqualLongTag(TIFF* tif1, TIFF* tif2, int tag, const char* Error);

// Lightweight test framework (non-GoogleTest) with non-terminating assertions.
struct TestResult {
    std::string name;
    bool passed;
    std::string message;
};

class TestSuite {
public:
    void add(const std::string& name, bool pass, const std::string& msg = "") {
        results.push_back({name, pass, msg});
    }

    int summary() const {
        int passed = 0;
        for (const auto& r : results) if (r.passed) ++passed;
        return passed;
    }

    int total() const { return static_cast<int>(results.size()); }

    void report() const {
        for (const auto& r : results) {
            std::cout << (r.passed ? "[PASS] " : "[FAIL] ") << r.name;
            if (!r.passed && !r.message.empty()) {
                std::cout << " -- " << r.message;
            }
            std::cout << "\n";
        }
        std::cout << "Summary: " << summary() << " / " << total() << " tests passed.\n";
    }

private:
    std::vector<TestResult> results;
};

// Helper to reset the mock state before each test
static void resetMocks() {
    g_mock_cmp_result = 1;
    g_fatal_called = false;
    g_last_error_message.clear();
}

// Convenience macro for non-terminating assertions
#define ASSERT_TRUE(cond, name, suite, msg) \
    do { \
        if (cond) { suite.add(name, true); } \
        else { suite.add(name, false, msg); } \
    } while (0)

#define ASSERT_FALSE(cond, name, suite, msg) \
    do { \
        if (!(cond)) { suite.add(name, true); } \
        else { suite.add(name, false, msg); } \
    } while (0)

int main() {
    TestSuite suite;

    // Step 3: Test Case Refinement
    // Design goals:
    // - Cover both branches of the predicate: CmpLongTag returns true/false.
    // - Verify non-terminating assertion behavior by catching FatalError via exception.
    // - Validate the error message content when the long tag comparison fails.

    // Test Case 1: True branch (CmpLongTag returns non-zero/true). EqualLongTag should NOT call FatalError.
    // Rationale: When CmpLongTag reports the tag values as equal, EqualLongTag should simply return
    // without invoking the fatal error handler.
    resetMocks();
    g_mock_cmp_result = 1;          // Simulate "tags equal" scenario
    bool test1_passed = true;
    try {
        // The production EqualLongTag would normally have more TIFF objects; our mock ignores them.
        EqualLongTag(nullptr, nullptr, 42, "LongTagMismatch");
        // If no exception is thrown, the path is considered to have taken the true-branch safely.
    } catch (...) {
        test1_passed = false; // Should not throw in the equal-branch scenario
    }
    ASSERT_TRUE(test1_passed && !g_fatal_called, "EqualLongTag_true_branch_no_fatal", suite,
                "FatalError should not be called when CmpLongTag returns true.");

    // Test Case 2: False branch (CmpLongTag returns false). EqualLongTag should call FatalError.
    // Rationale: When the tag values differ, EqualLongTag should signal an error via FatalError with
    // a formatted message containing the provided Error string.
    resetMocks();
    g_mock_cmp_result = 0; // Simulate "tags different" scenario
    bool test2_passed = false;
    try {
        EqualLongTag(nullptr, nullptr, 99, "TagDiffError");
        // If no exception is thrown, that's a failure for the false-branch since FatalError should trigger.
        test2_passed = false;
    } catch (const std::exception& e) {
        // Expected path: FatalError throws; verify the message content matches formatting.
        // The expected message is "<Error> is different" where <Error> is "TagDiffError".
        std::string expected = "TagDiffError is different";
        if (g_fatal_called && g_last_error_message == expected) {
            test2_passed = true;
        } else {
            test2_passed = false;
        }
        // Also optionally verify the exception text matches the captured message (redundant in this harness).
        // If needed: assert(e.what() == expected);
    } catch (...) {
        test2_passed = false; // Unexpected exception type
    }
    ASSERT_TRUE(test2_passed, "EqualLongTag_false_branch_fatalerror_message", suite,
                "FatalError should be invoked with the correctly formatted message when CmpLongTag returns false.");

    // Test Case 3: Ensure the test harness handles multiple sequential runs without state leakage.
    // This test ensures that previous mock state does not affect subsequent runs.
    resetMocks();
    g_mock_cmp_result = 1;
    bool test3_passed = true;
    try {
        EqualLongTag(nullptr, nullptr, 7, "FirstRun");
    } catch (...) {
        test3_passed = false;
    }
    // Now switch to false-branch and ensure we still capture the error properly without affecting prior run results.
    resetMocks();
    g_mock_cmp_result = 0;
    try {
        EqualLongTag(nullptr, nullptr, 7, "SecondRun");
        test3_passed = false;
    } catch (const std::exception&) {
        // Expected, but ensure the message content is captured as expected.
        if (g_last_error_message != "SecondRun is different") {
            test3_passed = false;
        } else {
            test3_passed = true;
        }
    }
    ASSERT_TRUE(test3_passed, "EqualLongTag_sequential_runs_state_isolated", suite,
                "Sequential runs should not leak state between tests and should report correct messages on failure.");

    // Report all results
    suite.report();

    // Return non-zero if any test failed; otherwise success.
    int passed = suite.summary();
    if (passed != suite.total()) {
        return 1;
    }
    return 0;
}