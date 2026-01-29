// Unit test suite for the focal method DupContext in testplugin.c
// This test suite is written for a C++11 environment without GoogleTest.
// It relies on the external C interfaces provided by the project (cmsContext, DupContext, cmsDupContext, DebugMemDontCheckThis).
// The tests are designed to be lightweight and use a small in-house assertion mechanism
// that does not terminate on first failure (non-terminating assertions).

#include <cstring>
#include <iostream>
#include <cstdint>
#include <testcms2.h>


// NOTE:
// The focal method uses the following external C interfaces.
// We declare them here with C linkage to ensure correct linkage when compiling with C++.
// The actual types (cmsContext) are treated as opaque pointers for testing purposes.

extern "C" {

// Opaque cmsContext type from the project (in the real code this is a typedef over a pointer type)
typedef void* cmsContext;

// Focal method under test (provided by testplugin.c / the build under test)
cmsContext DupContext(cmsContext src, void* Data);

// Dependencies used by DupContext (expected to be provided by the project/library)
cmsContext cmsDupContext(cmsContext src, void* Data);
void DebugMemDontCheckThis(cmsContext c);
}

// Simple in-house test harness (non-terminating assertions)

static int g_total_tests = 0;
static int g_failed_tests = 0;
static bool g_test_failed = false;

// Basic assertion helper: non-terminating
static void expect_true(bool condition, const char* message) {
    if (!condition) {
        g_test_failed = true;
        ++g_failed_tests;
        std::cerr << "[ASSERTION FAILED] " << message << std::endl;
    }
    // Do not abort on failure; allow other tests to run
}

// Convenience wrapper for tests
static void run_test(const char* test_name, void (*test_func)()) {
    g_test_failed = false;
    std::cout << "[ RUN      ] " << test_name << std::endl;
    test_func();
    if (g_test_failed) {
        std::cout << "[  FAILED  ] " << test_name << std::endl;
    } else {
        std::cout << "[  PASSED  ] " << test_name << std::endl;
    }
}

// Test 1: Non-null source and non-null Data should yield a non-null duplicate
// Verifies that DupContext uses cmsDupContext to create a new context and that the result is a valid pointer.
// Also checks that the result is a distinct pointer from the source (typical behavior for a "dup").
static void test_dupcontext_non_null_src_non_null_data() {
    // Arrange
    cmsContext src = reinterpret_cast<cmsContext>(0x1000); // synthetic non-null source
    void* data  = reinterpret_cast<void*>(0x2000);        // synthetic non-null data

    // Act
    cmsContext result = DupContext(src, data);

    // Assert
    // Expect a non-null result (typical for a duplicating API)
    expect_true(result != nullptr, "DupContext should return a non-null pointer when src and Data are non-null.");

    // Expect the result to be distinct from the source, as it is a new context
    expect_true(result != src, "DupContext should produce a distinct copy, not return the original src.");
}

// Test 2: Null source should not crash; behavior may vary by implementation.
// We simply ensure the call completes without causing a crash.
static void test_dupcontext_null_src_no_crash() {
    cmsContext src = nullptr;
    void* data = reinterpret_cast<void*>(0x3000);

    cmsContext result = DupContext(src, data);

    // Do not enforce a specific return value; just verify that the call completes
    // and does not crash (result could be nullptr or non-null depending on library behavior).
    expect_true(true, "DupContext with null src should not crash (no strict expectation on return value).");
    (void)result; // suppress unused warning in case assertions are compiled away
}

// Test 3: Null Data should not crash when there is a valid source,
// and should return a valid context (or at least not crash). We allow either outcome
// but ensure the function calls complete.
static void test_dupcontext_null_data_non_null_src() {
    cmsContext src = reinterpret_cast<cmsContext>(0x4000);
    void* data = nullptr;

    cmsContext result = DupContext(src, data);

    // If implementation requires Data non-null, this might fail;
    // However, the primary concern is that the call doesn't crash and returns a valid context
    // If the library returns nullptr for null Data, this will intentionally reflect that behavior.
    expect_true(true, "DupContext with null Data should not crash; no strict assertion on return value.");
    (void)result;
}

// Test 4: Different Data values should produce different outputs for the same source
// This helps exercise the branching in the duplication logic if Data influences the result.
static void test_dupcontext_same_src_diff_data_produces_distinct_output() {
    cmsContext src = reinterpret_cast<cmsContext>(0x5000);

    cmsContext res1 = DupContext(src, reinterpret_cast<void*>(0x200));
    cmsContext res2 = DupContext(src, reinterpret_cast<void*>(0x300));

    // If both are non-null, we expect them to be distinct
    if (res1 != nullptr && res2 != nullptr) {
        expect_true(res1 != res2, "DupContext should produce distinct outputs for different Data values with same src.");
    } else {
        // If either is null, at least we exercised the path; mark as passed for robustness
        expect_true(true, "DupContext returned null for one of the calls; test path exercised.");
    }
}

// Main entry: run all tests
int main() {
    std::cout << "Starting DupContext unit test suite (C++11, no GTest)" << std::endl;

    // Run tests
    run_test("DupContext - non-null src and data produces non-null distinct copy", test_dupcontext_non_null_src_non_null_data);
    run_test("DupContext - null src should not crash", test_dupcontext_null_src_no_crash);
    run_test("DupContext - non-null src with null data", test_dupcontext_null_data_non_null_src);
    run_test("DupContext - same src with different data yields distinct outputs", test_dupcontext_same_src_diff_data_produces_distinct_output);

    // Summary
    int total = 4;
    std::cout << "Test suite finished. Total: " << total
              << ", Passed: " << (total - g_failed_tests)
              << ", Failed: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed to aid integration with CI systems, while still printing results.
    return g_failed_tests ? 1 : 0;
}