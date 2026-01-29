// Test suite for the focal method Prelin8free in cmsopt.c
// Notes:
// - This test uses function interposition to override the internal _cmsFree
//   with a C linkage function defined in this test. This allows us to verify
//   that Prelin8free correctly calls _cmsFree with the provided context and
//   pointer without needing to rely on the internal memory management of the
//   actual cms library.
// - We avoid GTest and implement a small, self-contained test harness.
// - We keep tests minimal yet expressive to cover the primary interaction of
//   Prelin8free with its dependencies.

// Include necessary standard headers
#include <lcms2_internal.h>
#include <iostream>
#include <lcms2.h>
#include <cstdlib>


// Include the public CMS header to obtain type definitions (e.g., cmsContext)

// Forward declaration of the focal function under test (from cmsopt.c)
extern "C" void Prelin8free(cmsContext ContextID, void* ptr);

// Interposed function to capture calls to _cmsFree (memory free hook)
static int g_CmsFreeCallCount = 0;
static cmsContext g_LastCmsFreeContext = nullptr;
static void* g_LastCmsFreePtr = nullptr;

// We define _cmsFree with C linkage so the linker resolves to this symbol
extern "C" void _cmsFree(cmsContext ContextID, void* Ptr)
{
    g_LastCmsFreeContext = ContextID;
    g_LastCmsFreePtr = Ptr;
    ++g_CmsFreeCallCount;
    // Do not actually free memory here to avoid interfering with test objects.
}

// Notes for test maintainers:
// - We intentionally create dummy cmsContext values (casted integers) to avoid
//   requiring actual cmsCreateContext initialization. The interposed _cmsFree
//   does not dereference the context, it only records it, so this is safe for
//   the test harness as long as the function signature matches.
// - The tests focus on ensuring that Prelin8free calls _cmsFree with the given
//   ContextID and ptr, covering both non-null and null ptr scenarios, and that
//   the context is propagated correctly across calls.

static bool test_Prelin8free_calls_with_non_null_ptr() {
    // Reset interposition state
    g_CmsFreeCallCount = 0;
    g_LastCmsFreeContext = nullptr;
    g_LastCmsFreePtr = nullptr;

    // Create a dummy context value (no real cms context needed for this test)
    cmsContext ctxA = (cmsContext)0x1;

    // Allocate a sample block
    void* sample = std::malloc(32);
    if (!sample) {
        std::cerr << "Test failed: malloc() returned NULL for sample\n";
        return false;
    }

    // Call the function under test
    Prelin8free(ctxA, sample);

    // Validate that _cmsFree (interposed) was invoked exactly once
    bool ok = (g_CmsFreeCallCount == 1) && (g_LastCmsFreeContext == ctxA) && (g_LastCmsFreePtr == sample);
    if (!ok) {
        std::cerr << "Test Prelin8free with non-null ptr failed:\n"
                  << "  Expected context: " << ctxA << "\n"
                  << "  Actual context:   " << g_LastCmsFreeContext << "\n"
                  << "  Expected ptr: " << sample << "\n"
                  << "  Actual ptr:   " << g_LastCmsFreePtr << "\n"
                  << "  Free call count: " << g_CmsFreeCallCount << "\n";
    }

    // Clean up the allocated memory (even though _cmsFree didn't actually free it in test)
    std::free(sample);

    return ok;
}

static bool test_Prelin8free_calls_with_null_ptr() {
    // Reset interposition state
    g_CmsFreeCallCount = 0;
    g_LastCmsFreeContext = nullptr;
    g_LastCmsFreePtr = nullptr;

    // Create a dummy context value
    cmsContext ctxA = (cmsContext)0xA;

    // Call with NULL pointer
    Prelin8free(ctxA, nullptr);

    // Validate that _cmsFree was invoked with a NULL pointer
    bool ok = (g_CmsFreeCallCount == 1) && (g_LastCmsFreeContext == ctxA) && (g_LastCmsFreePtr == nullptr);
    if (!ok) {
        std::cerr << "Test Prelin8free with null ptr failed:\n"
                  << "  Expected context: " << ctxA << "\n"
                  << "  Actual context:   " << g_LastCmsFreeContext << "\n"
                  << "  Expected ptr: NULL\n"
                  << "  Actual ptr:   " << g_LastCmsFreePtr << "\n"
                  << "  Free call count: " << g_CmsFreeCallCount << "\n";
    }

    return ok;
}

static bool test_Prelin8free_calls_with_multiple_contexts() {
    // Reset interposition state
    g_CmsFreeCallCount = 0;
    g_LastCmsFreeContext = nullptr;
    g_LastCmsFreePtr = nullptr;

    // Two distinct dummy contexts
    cmsContext ctxA = (cmsContext)0x11;
    cmsContext ctxB = (cmsContext)0x22;

    // First allocation and call
    void* a = std::malloc(24);
    if (!a) {
        std::cerr << "Test failed: malloc() returned NULL for a\n";
        return false;
    }
    Prelin8free(ctxA, a);
    bool firstOk = (g_CmsFreeCallCount == 1) && (g_LastCmsFreeContext == ctxA) && (g_LastCmsFreePtr == a);
    if (!firstOk) {
        std::cerr << "Test step 1 failed: context/pointer mismatch for ctxA\n";
        std::free(a);
        return false;
    }

    // Second allocation and call with different context
    void* b = std::malloc(16);
    if (!b) {
        std::cerr << "Test failed: malloc() returned NULL for b\n";
        std::free(a);
        return false;
    }
    Prelin8free(ctxB, b);
    bool secondOk = (g_CmsFreeCallCount == 2) && (g_LastCmsFreeContext == ctxB) && (g_LastCmsFreePtr == b);
    if (!secondOk) {
        std::cerr << "Test step 2 failed: context/pointer mismatch for ctxB\n";
        std::free(a);
        std::free(b);
        return false;
    }

    // Cleanup
    std::free(a);
    std::free(b);

    return true;
}

static bool run_all_tests() {
    bool all_ok = true;

    // Test 1: Non-null pointer should trigger _cmsFree with the correct context and pointer
    if (!test_Prelin8free_calls_with_non_null_ptr()) {
        std::cerr << "Error: test_Prelin8free_calls_with_non_null_ptr failed.\n";
        all_ok = false;
    } else {
        std::cout << "PASS: test_Prelin8free_calls_with_non_null_ptr\n";
    }

    // Test 2: Null pointer should still trigger _cmsFree with the correct context
    if (!test_Prelin8free_calls_with_null_ptr()) {
        std::cerr << "Error: test_Prelin8free_calls_with_null_ptr failed.\n";
        all_ok = false;
    } else {
        std::cout << "PASS: test_Prelin8free_calls_with_null_ptr\n";
    }

    // Test 3: Interactions with multiple contexts should be tracked independently
    if (!test_Prelin8free_calls_with_multiple_contexts()) {
        std::cerr << "Error: test_Prelin8free_calls_with_multiple_contexts failed.\n";
        all_ok = false;
    } else {
        std::cout << "PASS: test_Prelin8free_calls_with_multiple_contexts\n";
    }

    return all_ok;
}

int main() {
    bool success = run_all_tests();
    if (success) {
        std::cout << "All Prelin8free tests passed.\n";
        return 0;
    } else {
        std::cerr << "Some Prelin8free tests failed.\n";
        return 1;
    }
}