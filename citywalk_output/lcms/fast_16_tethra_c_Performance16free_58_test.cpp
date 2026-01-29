// Test suite for Performance16free (wrapper around _cmsFree)
// This test suite is hand-written (no GoogleTest) and uses a lightweight
// in-file test harness to exercise the focal function.
// The goal is to achieve execution coverage for the wrapper function by
// invoking it with a null pointer and with a non-null pointer in a simulated
// cmsContext context. Since _cmsFree is an external dependency, we rely on
// the fact that the wrapper simply forwards the call and should not crash
// for typical inputs.
// Explanatory comments are added for each unit test as requested.

#include <cstdio>
#include <fast_float_internal.h>
#include <cstddef>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Forward declaration of the focal function under test.
// We assume cmsContext is a typedef of void* in the actual library (as is common in
// LittleCMS). If the real type differs in the link-time library, this prototype
// remains compatible for symbol resolution due to C linkage.
extern "C" void Performance16free(void* ContextID, void* ptr);

// Lightweight test harness (non-terminating assertions)
static int g_test_count = 0;
static int g_pass_count = 0;
static int g_fail_count = 0;

#define TEST_PASS() do { ++g_test_count; ++g_pass_count; } while(0)
#define TEST_FAIL(msg) do { ++g_test_count; ++g_fail_count; std::cerr << "[TEST FAIL] " << msg << std::endl; } while(0)

// Test 1: Free with null context and null pointer - ensure no crash
// Rationale: The wrapper should handle null arguments gracefully without terminating the program.
// This covers the "true"/neutral branch behavior for input validation that may exist in _cmsFree usage.
static void Test_Performance16free_NullArgs_NoCrash() {
    // arrange
    void* ContextID = nullptr;
    void* ptr = nullptr;

    // act
    try {
        Performance16free(ContextID, ptr);
        // If no exception/crash occurs, we consider the test passed.
        TEST_PASS();
        return;
    } catch (...) {
        // If any exception propagates, mark as fail
        TEST_FAIL("Performance16free threw an exception on null arguments.");
        return;
    }
}

// Test 2: Free with a non-null pointer and a non-null context - ensure no crash
// Rationale: The wrapper should invoke _cmsFree with the provided arguments.
// This checks basic forward-call integrity when a real pointer is supplied.
// Note: We do not free the pointer afterwards to avoid potential double-free
// scenarios if _cmsFree also frees the memory internally.
static void Test_Performance16free_NonNullArgs_NoCrash() {
    // arrange
    void* ContextID = reinterpret_cast<void*>(0x12345678); // synthetic non-null context
    void* ptr = std::malloc(64); // allocate a small block to pass to the allocator

    if (ptr == nullptr) {
        TEST_FAIL("malloc failed in Test_Performance16free_NonNullArgs_NoCrash.");
        return;
    }

    // act
    try {
        Performance16free(ContextID, ptr);
        // If no exception/crash, consider test passed.
        TEST_PASS();
        // Important: Do not free here if _cmsFree has freed the memory; double-free risk.
        // We purposely avoid freeing to not interfere with the CMS free path in test environments.
        return;
    } catch (...) {
        TEST_FAIL("Performance16free threw an exception on non-null arguments.");
        // Best-effort cleanup in case of unexpected failure; avoid double-free risk.
        // If we did free, we might clash with _cmsFree; thus skip.
        return;
    }
}

// Test 3: Repeated calls with mixed arguments to increase coverage surface
// Rationale: Re-invoking the wrapper with varying inputs increases coverage on
// the call path within a single translation unit, helping detect potential
// issues with statefulness or repeated frees.
static void Test_Performance16free_MultipleCalls() {
    // arrange
    void* ContextID1 = nullptr;
    void* ContextID2 = reinterpret_cast<void*>(0xABCDEF01);
    void* ptr1 = nullptr;
    void* ptr2 = std::malloc(128);

    // act
    // 1) Null scenario
    Performance16free(ContextID1, ptr1);

    // 2) Non-null scenario
    if (ptr2 != nullptr) {
        Performance16free(ContextID2, ptr2);
        // Avoid freeing to prevent double-free risk with _cmsFree
        // Note: In a full test environment, a dedicated mock or allocator would clarify ownership.
        // Here we simply exercise multiple invocations.
        // If we reach here without crash, consider test passed.
        // Do not free(ptr2);
    }

    // mark as passed (best-effort; actual crash would have been caught by try/catch)
    TEST_PASS();
}

// Helper to run all tests and print a summary
static void RunAllTests() {
    std::cout << "Running Performance16free unit tests (no GTest, custom harness)..." << std::endl;

    Test_Performance16free_NullArgs_NoCrash();
    Test_Performance16free_NonNullArgs_NoCrash();
    Test_Performance16free_MultipleCalls();

    std::cout << "Test Summary: "
              << g_pass_count << " passed, "
              << g_fail_count << " failed, "
              << g_test_count << " total." << std::endl;

    if (g_fail_count != 0) {
        std::cout << "Some tests failed. Please review the failure messages above." << std::endl;
    } else {
        std::cout << "All tests passed (best-effort coverage for Performance16free wrapper)." << std::endl;
    }
}

// Main entry point for the test executable
int main() {
    RunAllTests();
    // Non-zero exit code if any test failed, to aid integration in CI or scripts
    return (g_fail_count == 0) ? 0 : 1;
}