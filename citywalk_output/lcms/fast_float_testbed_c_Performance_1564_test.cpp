// Test suite for the focal Performance function in fast_float_testbed.c
// This test suite is written in C++11 (no GTest) and uses a lightweight, non-terminating assertion style.
// It relies on the real Performance function and its runtime dependencies present in the project.
// The tests focus on core behavior: ensuring Performance returns the value provided by the perf_fn
// and that it forwards the provided cmsContext (ct) and ICC profile handles to perf_fn.
// Comments explain the intent and the expectations for each test case.

#include <cstring>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <string>
#include <memory.h>
#include <stdlib.h>
#include <lcms2.h>
#include <cstdint>


// Import Little CMS types used by the focal method.
// The actual project depends on the Little CMS headers; we assume their availability in the test environment.
extern "C" {
}

// Forward declaration of the focal Performance function from the production code.
// Signature must match exactly as used in the source (<FOCAL_METHOD>).
typedef cmsFloat64Number (*perf_fn)(cmsContext, cmsHPROFILE, cmsHPROFILE);

extern "C" cmsFloat64Number Performance(const char* Title, perf_fn fn, cmsContext ct,
                                        const char* inICC, const char* outICC,
                                        size_t sz, cmsFloat64Number prev);

// Lightweight non-terminating test framework
static std::vector<std::string> g_failures;
static int g_tests_run = 0;
static int g_tests_passed = 0;

// Helpers for test assertions (non-terminating)
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        g_failures.push_back(std::string("Expectation failed: ") + (msg)); \
    } else { \
        ++g_tests_passed; \
    } \
} while(0)

#define REPORT_SUMMARY() do { \
    std::cout << "Tests executed: " << g_tests_run << "\n"; \
    std::cout << "Tests passed:   " << g_tests_passed << "\n"; \
    if(!g_failures.empty()) { \
        std::cout << "Failures (" << g_failures.size() << "):\n"; \
        for(const auto& s : g_failures) std::cout << "  - " << s << "\n"; \
    } else { \
        std::cout << "All tests passed.\n"; \
    } \
} while(0)

// Global/test-scoped state to capture perf_fn interactions
static cmsContext g_last_ct = nullptr;
static cmsHPROFILE g_last_in = nullptr;
static cmsHPROFILE g_last_out = nullptr;
static cmsFloat64Number g_fn_return_value = 0.0;

// Perf function stub used by tests to control and observe behavior
static cmsFloat64Number testPerfFn(cmsContext ct, cmsHPROFILE in, cmsHPROFILE out)
{
    g_last_ct = ct;
    g_last_in = in;
    g_last_out = out;
    return g_fn_return_value;
}

// Test 1: Basic pass-through - verify that Performance returns the value from perf_fn
void test_Performance_BasicPassThrough()
{
    g_tests_run++;

    // Arrange
    g_fn_return_value = 5.25;          // perf_fn will return this value
    cmsContext testCt = reinterpret_cast<cmsContext>(0x1234);
    const char* inICC = "in_basic.icc";
    const char* outICC = "out_basic.icc";
    size_t sz = 128;
    cmsFloat64Number prev = 0.0;        // triggers the 'no imp' path

    // Act
    cmsFloat64Number result = Performance("Test-BasicPassThrough", testPerfFn, testCt, inICC, outICC, sz, prev);

    // Assert
    EXPECT_TRUE(result == g_fn_return_value, "Performance should forward perf_fn's return value.");
    EXPECT_TRUE(g_last_ct == testCt, "Performance should pass the correct cmsContext to perf_fn.");
    EXPECT_TRUE(g_last_in != nullptr, "loadProfile should provide a valid input profile to perf_fn (non-null).");
    EXPECT_TRUE(g_last_out != nullptr, "loadProfile should provide a valid output profile to perf_fn (non-null).");
}

// Test 2: Prev > 0 path is exercised (inner branch exists); ensures proper forwarding and return value.
// Note: The trace output is a no-op in the test environment, but the focus is on input/output flow.
void test_Performance_PrevPositive_Path()
{
    g_tests_run++;

    // Arrange
    g_fn_return_value = 42.0;          // perf_fn will return this value
    cmsContext testCt = reinterpret_cast<cmsContext>(0xABCD);
    const char* inICC = "in_positive.icc";
    const char* outICC = "out_positive.icc";
    size_t sz = 64;
    cmsFloat64Number prev = 3.0;         // > 0.0 so imp will be computed (n/prev). Inner branch exists.

    // Act
    cmsFloat64Number result = Performance("Test-PrevPositivePath", testPerfFn, testCt, inICC, outICC, sz, prev);

    // Assert
    EXPECT_TRUE(result == g_fn_return_value, "Performance should return perf_fn's value when prev > 0.");
    EXPECT_TRUE(g_last_ct == testCt, "Performance should pass the same cmsContext when prev > 0.");
    EXPECT_TRUE(g_last_in != nullptr, "Input profile should be loaded and passed to perf_fn.");
    EXPECT_TRUE(g_last_out != nullptr, "Output profile should be loaded and passed to perf_fn.");
}

// Test 3: Longer Title string and 0-byte size edge to ensure no crash and correct return propagation.
// This checks that unusual Title and sz values do not break behavior and that perf_fn is still called.
void test_Performance_LongTitleAndEdgeCase()
{
    g_tests_run++;

    // Arrange
    g_fn_return_value = 7.0;
    cmsContext testCt = reinterpret_cast<cmsContext>(0x0F0F);
    // Construct a long title at compile-time
    const char* inICC = "in_longtitle.icc";
    const char* outICC = "out_longtitle.icc";
    size_t sz = 0;                       // edge case: zero size
    cmsFloat64Number prev = 0.0;

    // Act
    cmsFloat64Number result = Performance("Test-Performance-LongTitle-EdgeCase-Case-XYZ- unnecessarily-long-title-to-test-formatting",
                                           testPerfFn, testCt, inICC, outICC, sz, prev);

    // Assert
    EXPECT_TRUE(result == g_fn_return_value, "Performance should return perf_fn's value even with long title and edge sz.");
    EXPECT_TRUE(g_last_ct == testCt, "Performance should propagate the cmsContext through perf_fn.");
    EXPECT_TRUE(g_last_in != nullptr, "Input ICC should be provided to perf_fn.");
    EXPECT_TRUE(g_last_out != nullptr, "Output ICC should be provided to perf_fn.");
}

// Entry point to run all tests
int main()
{
    // Initialize shared state
    g_last_ct = nullptr;
    g_last_in = nullptr;
    g_last_out = nullptr;
    g_fn_return_value = 0.0;

    // Run tests
    test_Performance_BasicPassThrough();
    test_Performance_PrevPositive_Path();
    test_Performance_LongTitleAndEdgeCase();

    // Report
    std::cout << "\nTest results summary:\n";
    REPORT_SUMMARY();

    // Return non-zero if any tests failed (based on collected failures)
    return g_failures.empty() ? 0 : 1;
}

// Notes for maintainers:
// - The tests rely on the actual implementation of Performance and its dependencies (loadProfile, trace, etc.).
//   If the environment cannot provide the expected ICC files, consider adapting the tests to mock loadProfile
//   or supply a lightweight test ICC dataset accessible by the test environment.
// - These tests focus on the observable behavior of Performance (i.e., forwarding to perf_fn and returning its value)
//   rather than on internal formatting/console output, which is handled by the trace function (a no-op in this setup).
// - All assertions are non-terminating: they accumulate failures and allow subsequent tests to run, enabling higher coverage.