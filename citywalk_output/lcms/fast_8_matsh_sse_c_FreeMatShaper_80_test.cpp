// Test suite for FreeMatShaper from fast_8_matsh_sse.c
// This test suite follows the given instructions to generate a focused
// unit test harness without using GTest. It relies on the actual
// FreeMatShaper implementation to be linked in at test time.

// Key Candidate Keywords (derived from the focal method and dependencies)
// - FreeMatShaper: the function under test
// - cmsContext ContextID: the context identifier parameter (unused in body)
// - void* Data: the generic data pointer argument
// - free_aligned: helper invoked when Data is non-NULL
// - XMatShaper8Data: type of the Data payload the function frees
// - malloc_aligned: allocator to create an XMatShaper8Data for testing
// - NULL vs non-NULL Data: branches covered by tests
// - UNUSED_PARAMETER: macro used in the function (no effect on test)
// - CMS context/typing: cmsContext aliasing for the test scaffold

#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <string>
#include <cpuid.h>


// Domain knowledge: keep tests self-contained and non-terminating on assertion.
// We implement a lightweight test harness with non-terminating assertions
// to maximize code execution coverage.

extern "C" {
    // Forward declarations to link with the actual C implementation in fast_8_matsh_sse.c
    // These must match the signatures used in the focal file.
    typedef void* cmsContext;

    struct XMatShaper8Data; // Opaque; actual layout unknown to tests

    // Allocator/Deallocator provided by the focal class dependencies
    XMatShaper8Data* malloc_aligned(cmsContext ContextID);
    void free_aligned(XMatShaper8Data* a);

    // Focal method under test (C linkage)
    void FreeMatShaper(cmsContext ContextID, void* Data);
}

// Lightweight test harness helpers
static int g_total_tests = 0;
static int g_passed_tests = 0;

// Record a test result without aborting on failure
static void record_test_result(bool passed, const std::string& test_name, const std::string& message = "")
{
    ++g_total_tests;
    if (passed) {
        ++g_passed_tests;
        std::cout << "[PASS] " << test_name << "\n";
    } else {
        std::cout << "[FAIL] " << test_name;
        if (!message.empty()) {
            std::cout << " - " << message;
        }
        std::cout << "\n";
    }
}

// Test 1: FreeMatShaper with NULL Data should not crash or misbehave
static bool test_FreeMatShaper_NullData()
{
    // Arrange
    cmsContext dummyContext = nullptr;

    // Act & Assert (non-terminating style)
    // If FreeMatShaper crashes, the test will fail (no exception handling in C)
    FreeMatShaper(dummyContext, nullptr);

    // Since we cannot observe internal state, consider this a pass if no crash occurred.
    return true;
}

// Test 2: FreeMatShaper with non-NULL Data should invoke free_aligned
//        We allocate a valid XMatShaper8Data via malloc_aligned and pass it.
//        If the library correctly frees the allocated memory, this should not crash.
//        We also pass a non-NULL ContextID to exercise the unused parameter behavior.
static bool test_FreeMatShaper_NonNullData()
{
    cmsContext dummyContext = reinterpret_cast<cmsContext>(0x12345678);

    // Try to obtain a valid allocated data object
    XMatShaper8Data* data = malloc_aligned(dummyContext);
    if (data == nullptr) {
        // If allocation fails in the test environment, skip gracefully.
        std::cout << "[SKIP] malloc_aligned returned NULL; test environment may not provide SSE-backed allocator.\n";
        return true;
    }

    // Call FreeMatShaper with non-NULL Data
    FreeMatShaper(dummyContext, static_cast<void*>(data));

    // We cannot directly observe free_aligned invocation here, but must assume
    // success if no crash occurs during FreeMatShaper execution.
    return true;
}

// Optional: a third test to exercise non-NULL Data with NULL Context (unused in body) 
// to ensure no crash regardless of Context value.
static bool test_FreeMatShaper_NonNullDataWithNullContext()
{
    XMatShaper8Data* data = malloc_aligned(nullptr);
    if (data == nullptr) {
        std::cout << "[SKIP] malloc_aligned returned NULL; skipping test.\n";
        return true;
    }

    FreeMatShaper(nullptr, static_cast<void*>(data));

    // If no crash occurred, test passes.
    return true;
}

int main()
{
    // Step 2 & 3: Build a small test suite for FreeMatShaper
    // We intentionally keep tests non-terminating and lightweight to maximize coverage.
    std::vector<std::pair<std::string, bool(*)()>> tests = {
        {"test_FreeMatShaper_NullData", test_FreeMatShaper_NullData},
        {"test_FreeMatShaper_NonNullData", test_FreeMatShaper_NonNullData},
        {"test_FreeMatShaper_NonNullDataWithNullContext", test_FreeMatShaper_NonNullDataWithNullContext}
    };

    for (auto &t : tests) {
        bool ok = false;
        try {
            ok = t.second();
        } catch (...) {
            ok = false;
        }
        // A dedicated message for test result (no early exit)
        record_test_result(ok, t.first);
    }

    // Summary
    std::cout << "Test summary: " << g_passed_tests << " / " << g_total_tests << " tests passed.\n";

    // Return non-zero if some tests failed (standard convention)
    return (g_total_tests == g_passed_tests) ? 0 : 1;
}

/*
Notes for maintainers:
- This test suite focuses on the observed behavior of FreeMatShaper:
  - It should gracefully handle NULL Data (no crash).
  - It should handle non-NULL Data by invoking free_aligned (observability is limited in unit isolation).
- We rely on malloc_aligned/free_aligned provided by the project to create deterministic memory
  blocks for testing. If their behavior changes, tests may need to adapt to new APIs or add mocks
  that track free_aligned invocations (requires linker support or weak symbols).
- The tests are designed to compile with C++11 and to be runnable from main() without a test framework.
- Static members and private details are not accessed; only the FreeMatShaper API is exercised.
- Assertions are non-terminating; the test harness prints per-test results and a final summary.
*/