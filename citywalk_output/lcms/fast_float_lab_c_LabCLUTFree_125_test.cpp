/*
  LabCLUTFree unit test suite
  - Target: LabCLUTFree(cmsContext ContextID, void* v)
  - Approach: Compile test together with the focal C source (fast_float_lab.c) in one translation unit
              and provide a test double for _cmsFree to verify invocation semantics.
  - Notes:
    - This tests only that LabCLUTFree forwards its arguments to _cmsFree; it does not attempt to rely on the
      actual internal allocator semantics of the original library.
    - Uses simple, non-terminating assertions to maximize code-path coverage.
    - No GoogleTest/GMock; run via main() in this file.
*/

/* Domain: Use only C++11 standard library, provide lightweight test harness, override _cmsFree to capture calls,
   and verify LabCLUTFree's behavior by asserting on captured arguments and call counts. */

#include <fast_float_lab.c>
#include <fast_float_internal.h>
#include <cstddef>
#include <iostream>
#include <cstdlib>


// Candidate Keywords derived from the focal method and its dependencies:
// - LabCLUTFree: the method under test
// - cmsContext: context identifier type (opaque pointer-like type)
// - _cmsFree: internal memory free facility invoked by LabCLUTFree
// - v: void* memory reference being freed
// - ContextID: input context passed through to _cmsFree
// - Memory management contract: LabCLUTFree should forward (ContextID, v) to _cmsFree

// Forward declare the focal function prototype to allow direct calling from C++ test code.
// The actual implementation lives in fast_float_lab.c.
extern "C" {
    void LabCLUTFree(void* ContextID, void* v);
    // Enter a placeholder prototype for _cmsFree to enable a test double in this translation unit.
    void _cmsFree(void* ContextID, void* v);
}

/* Global test state to capture calls to _cmsFree when LabCLUTFree is exercised. */
static size_t g_cmsFreeCallCount = 0;
static void* g_lastContextID = nullptr;
static void* g_lastFreedPtr = nullptr;

/* Test double for _cmsFree to intercept and record parameters.
   We deliberately implement this in the same translation unit to ensure it binds to LabCLUTFree's call. */
extern "C" void _cmsFree(void* ContextID, void* v)
{
    ++g_cmsFreeCallCount;
    g_lastContextID = ContextID;
    g_lastFreedPtr  = v;
    // Do not free memory here; this is a test double to observe the call.
}

/* Include the focal C source after declaring the test double so LabCLUTFree resolves to a real function,
   but its internal call to _cmsFree will be resolved to our test double above. */
extern "C" {
}

// Lightweight test macros (non-terminating) to maximize code execution and coverage
static bool g_anyTestFailure = false;

#define EXPECT_TRUE(cond) \
    do { \
        if(!(cond)) { \
            std::cerr << "EXPECT_TRUE failed: " #cond " at " __FILE__ ":" << __LINE__ << std::endl; \
            g_anyTestFailure = true; \
        } \
    } while(false)

#define EXPECT_EQ(a, b) \
    do { \
        if(!((a) == (b))) { \
            std::cerr << "EXPECT_EQ failed: " #a " (" << (a) << ") != " #b " (" << (b) << ")" \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            g_anyTestFailure = true; \
        } \
    } while(false)

// Helper test wrappers to keep main concise
// Test 1: Basic forwarding of non-null pointer and context through LabCLUTFree
void test_LabCLUTFree_ForwardsNonNullPointer()
{
    // Reset captured state
    g_cmsFreeCallCount = 0;
    g_lastContextID = nullptr;
    g_lastFreedPtr  = nullptr;

    // Arrange: arbitrary, but deterministic inputs
    void* ctx = reinterpret_cast<void*>(0xABCD0001);
    void* ptr = reinterpret_cast<void*>(0xBAADF00D);

    // Act: invoke the focal method
    LabCLUTFree(ctx, ptr);

    // Assert: LabCLUTFree should invoke _cmsFree with the same arguments
    EXPECT_EQ(g_cmsFreeCallCount, 1u);
    EXPECT_EQ(g_lastContextID, ctx);
    EXPECT_EQ(g_lastFreedPtr,  ptr);
}

// Test 2: Forwarding when v is nullptr
void test_LabCLUTFree_ForwardsNullPointer()
{
    // Reset captured state
    g_cmsFreeCallCount = 0;
    g_lastContextID = nullptr;
    g_lastFreedPtr  = nullptr;

    void* ctx = reinterpret_cast<void*>(0x12340001);
    void* v   = nullptr;

    LabCLUTFree(ctx, v);

    EXPECT_EQ(g_cmsFreeCallCount, 1u);
    EXPECT_EQ(g_lastContextID, ctx);
    EXPECT_EQ(g_lastFreedPtr,  v);
}

// Test 3: Forwarding with a different context and pointer
void test_LabCLUTFree_ForwardsDifferentInputs()
{
    // Reset captured state
    g_cmsFreeCallCount = 0;
    g_lastContextID = nullptr;
    g_lastFreedPtr  = nullptr;

    void* ctx = reinterpret_cast<void*>(0xDEADBEEF);
    int someData = 42;
    void* v = &someData;

    LabCLUTFree(ctx, v);

    EXPECT_EQ(g_cmsFreeCallCount, 1u);
    EXPECT_EQ(g_lastContextID, ctx);
    EXPECT_EQ(g_lastFreedPtr,  v);
}

int main()
{
    std::cout << "Running LabCLUTFree unit tests (single-translation-unit C test double)..." << std::endl;

    test_LabCLUTFree_ForwardsNonNullPointer();
    test_LabCLUTFree_ForwardsNullPointer();
    test_LabCLUTFree_ForwardsDifferentInputs();

    if (g_anyTestFailure) {
        std::cerr << "Some tests FAILED. See above messages." << std::endl;
        return 1;
    } else {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    }
}