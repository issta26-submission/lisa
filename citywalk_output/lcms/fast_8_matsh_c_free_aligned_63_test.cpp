/*
Step 1 - Program Understanding (Candidate Keywords)
Captured core dependencies and elements for generating tests:
- Function under test: free_aligned(XMatShaper8Data* a)
- Dependent operation: _cmsFree(a->ContextID, a->real_ptr)
- Data structure: XMatShaper8Data with members ContextID and real_ptr
- Dependency signature: _cmsFree(cmsContext ContextID, void* ptr)
- Context types: cmsContext (used as a pointer-like handle)
- External linkage: free_aligned is defined in fast_8_matsh.c and linked with the test
- Testing goal: verify that free_aligned forwards the correct values to _cmsFree

These keywords guide the test design to ensure proper behavior under typical, edge, and boundary scenarios.

Step 2 & 3 - Unit Test Generation and Refinement
We implement a lightweight, non-G-test test harness in C++11 that mocks _cmsFree and validates that free_aligned forwards ContextID and real_ptr correctly. We assume the XMatShaper8Data layout used by free_aligned is:
struct XMatShaper8Data {
    cmsContext ContextID;
    void* real_ptr;
};

The test suite uses:
- A minimal, non-terminating EXPECT-like macro set (non-GTest).
- A C linkage mock of _cmsFree to capture parameters.
- Direct calling of free_aligned from the linked C translation unit (fast_8_matsh.c).

The following code file is self-contained and intended to be compiled together with fast_8_matsh.c (or with its corresponding header providing prototypes).

Notes:
- We avoid private methods/fields; we only exercise the public-facing free_aligned API.
- We access function results via captured mock state.
- We use simple integer/pointer comparisons to differentiate addresses (not content semantics).
- All tests are run from main() as per domain knowledge constraints (no GTest).

Now providing the test code (explanatory comments accompany each unit test).
*/

#include <cstddef>
#include <iostream>
#include <cstdint>
#include <fast_float_internal.h>


// ----------------------------------------------------------------------
// Domain-specific scaffolding for the tests
// ----------------------------------------------------------------------

// Candidate Keywords (from Step 1): _cmsFree, ContextID, real_ptr, XMatShaper8Data, cmsContext, free_aligned

// Assumed type aliases to match production code usage
typedef void* cmsContext;

// Forward declaration matching the production function signature.
// We assume the actual C function is defined in fast_8_matsh.c with C linkage.
extern "C" void free_aligned(struct XMatShaper8Data* a);

// Mockable _cmsFree function that the production code calls.
// We provide the mock here with C linkage so that the linker finds it correctly.
extern "C" void _cmsFree(cmsContext ContextID, void* ptr);

// The exact layout of XMatShaper8Data is assumed to be:
// struct XMatShaper8Data {
//     cmsContext ContextID;
//     void* real_ptr;
// };
// We mirror the layout in this test to ensure memory layout compatibility.
struct XMatShaper8Data {
    cmsContext ContextID;
    void* real_ptr;
};

// ----------------------------------------------------------------------
// Mock infrastructure to capture calls to _cmsFree
// ----------------------------------------------------------------------

static cmsContext g_lastContextID = nullptr;
static void*      g_lastPtr       = nullptr;
static int          g_callCount     = 0;

extern "C" void _cmsFree(cmsContext ContextID, void* ptr) {
    // Capture parameters for verification
    g_lastContextID = ContextID;
    g_lastPtr       = ptr;
    ++g_callCount;
}

// ----------------------------------------------------------------------
// Lightweight EXPECT-like assertions (non-terminating)
// These do not abort tests; they accumulate failures for final reporting.
// ----------------------------------------------------------------------

static int g_total_checks = 0;
static int g_failures     = 0;

static void log_failure(const char* file, int line, const char* msg) {
    std::cerr << "[TEST FAIL] " << file << ":" << line << " - " << msg << "\n";
}

#define EXPECT_TRUE(cond) \
    do { \
        ++g_total_checks; \
        if(!(cond)) { log_failure(__FILE__, __LINE__, "EXPECT_TRUE failed: " #cond); ++g_failures; } \
    } while(0)

#define EXPECT_EQ(expected, actual) \
    do { \
        ++g_total_checks; \
        if(!((expected) == (actual))) { \
            std::cerr << "[TEST FAIL] " << __FILE__ << ":" << __LINE__ \
                      << " - EXPECT_EQ failed: expected " << (void*)(expected) \
                      << ", got " << (void*)(actual) << "\n"; \
            ++g_failures; \
        } \
    } while(0)

#define EXPECT_PTR_EQ(expected, actual) \
    do { \
        ++g_total_checks; \
        if((expected) != (actual)) { \
            std::cerr << "[TEST FAIL] " << __FILE__ << ":" << __LINE__ \
                      << " - EXPECT_PTR_EQ failed: expected " << expected \
                      << ", got " << actual << "\n"; \
            ++g_failures; \
        } \
    } while(0)

// Helper to reset the mock state between tests
static void reset_mock() {
    g_lastContextID = nullptr;
    g_lastPtr       = nullptr;
    g_callCount     = 0;
}

// ----------------------------------------------------------------------
// Unit Tests for free_aligned
// ----------------------------------------------------------------------

// Test 1: Basic positive path - ensure correct ContextID and real_ptr are forwarded
void test_free_aligned_basic() {
    reset_mock();

    XMatShaper8Data a;
    a.ContextID = reinterpret_cast<cmsContext>(0x1000);
    a.real_ptr  = reinterpret_cast<void*>(0x2000);

    // Invoke the focal method
    free_aligned(&a);

    // Verify _cmsFree was called exactly once with the same values
    EXPECT_EQ(1, g_callCount);
    EXPECT_PTR_EQ(a.ContextID, g_lastContextID);
    EXPECT_PTR_EQ(a.real_ptr, g_lastPtr);

    // Explanation:
    // - Validates the true path where both forwarded values are non-null
    // - Ensures no additional unintended side effects occur
}

// Test 2: Edge case - forwarding when real_ptr is nullptr
void test_free_aligned_null_ptr() {
    reset_mock();

    XMatShaper8Data a;
    a.ContextID = reinterpret_cast<cmsContext>(0x3000);
    a.real_ptr  = nullptr;

    free_aligned(&a);

    EXPECT_EQ(1, g_callCount);
    EXPECT_PTR_EQ(a.ContextID, g_lastContextID);
    EXPECT_PTR_EQ(a.real_ptr, g_lastPtr); // both should be nullptr
}

// Test 3: Multiple consecutive calls - ensure state is accumulated correctly
void test_free_aligned_multiple_calls() {
    reset_mock();

    XMatShaper8Data a1;
    a1.ContextID = reinterpret_cast<cmsContext>(0x4001);
    a1.real_ptr  = reinterpret_cast<void*>(0xA1);

    XMatShaper8Data a2;
    a2.ContextID = reinterpret_cast<cmsContext>(0x4002);
    a2.real_ptr  = reinterpret_cast<void*>(0xB2);

    free_aligned(&a1);
    free_aligned(&a2);

    // After two calls
    EXPECT_EQ(2, g_callCount);
    // Last call should reflect a2's values
    EXPECT_PTR_EQ(a2.ContextID, g_lastContextID);
    EXPECT_PTR_EQ(a2.real_ptr, g_lastPtr);
}

// ----------------------------------------------------------------------
// Main harness to execute tests
// ----------------------------------------------------------------------

int main() {
    std::cout << "Starting unit tests for free_aligned (fast_8_matsh.c) using lightweight test harness...\n";

    test_free_aligned_basic();
    test_free_aligned_null_ptr();
    test_free_aligned_multiple_calls();

    std::cout << "Tests completed. Total checks: " << g_total_checks
              << ", Failures: " << g_failures << "\n";

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED\n";
        return 1;
    }
}