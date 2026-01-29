// Unit test suite for GenericMPEfree in cmstypes.c
// Target: ensure GenericMPEfree properly frees a cmsStage pointer by calling cmsStageFree
// without relying on the full LittleCMS runtime. This test uses a lightweight, non-terminating
// assertion scheme (no GTest) and validates key behaviors.
//
// Notes:
// - We provide a small C-compatible stub for cmsStageFree to capture the argument passed by
//   GenericMPEfree. This allows us to verify that the exact Ptr value is forwarded.
// - We forward-declare necessary types to avoid pulling in full internal headers in the test file.
// - The test calls the focal function via an extern "C" prototype to ensure correct linkage.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdlib>


// Forward declare the focal function (as it would be declared in the project headers)
struct _cms_typehandler_struct; // opaque type used by GenericMPEfree
extern "C" void GenericMPEfree(struct _cms_typehandler_struct* self, void *Ptr);

// Forward declare the cmsStage type and cmsStageFree symbol to allow a compatible stub.
// We only need to provide a definition for cmsStage (incomplete type) and a C-linkage function.
struct cmsStage; // incomplete type to satisfy the prototype
extern "C" void cmsStageFree(struct cmsStage* stage); // symbol overridden by our test stub

// Lightweight test harness: non-terminating assertions
static int g_test_failures = 0;

// We capture the pointer passed to cmsStageFree to verify correct forwarding from GenericMPEfree.
static void* g_last_cmsStage_ptr = nullptr;

// Stub: intercept cmsStageFree to record the pointer value it receives.
extern "C" void cmsStageFree(struct cmsStage* stage)
{
    g_last_cmsStage_ptr = reinterpret_cast<void*>(stage);
    // Do not perform any real freeing; this is a test stub.
}

// A minimal forward declaration for the type passed as 'self' to GenericMPEfree.
// The function under test does not use this parameter (as per its body), but we keep the
// signature consistent for compilation/linking purposes.
struct _cms_typehandler_struct;

// Helper macros for non-terminating assertions
#define TEST_LOG(msg) do { std::cerr << "TEST: " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")\n"; } while (0)
#define EXPECT_PTR_EQ(a, b) \
    do { if ((void*)(a) != (void*)(b)) { \
        TEST_LOG("Pointer values differ: " #a " != " #b); \
        std::cerr << "  Actual: " << (void*)(a) << "  Expected: " << (void*)(b) << "\n"; \
        ++g_test_failures; \
      } \
    } while (0)
#define EXPECT_TRUE(cond) \
    do { if (!(cond)) { \
        TEST_LOG("Expected condition true: " #cond); \
        ++g_test_failures; \
      } \
    } while (0)

// Test 1: Ptr is non-null. Expect cmsStageFree to be called with the exact Ptr.
void test_GenericMPEfree_non_null_ptr() {
    g_last_cmsStage_ptr = reinterpret_cast<void*>(0xDEADBEEF); // ensure different value initially

    void* testPtr = std::malloc(32); // non-null arbitrary pointer
    // self is unused by GenericMPEfree; pass a null pointer for simplicity
    GenericMPEfree(nullptr, testPtr);

    // cmsStageFree should have been invoked with testPtr
    EXPECT_PTR_EQ(g_last_cmsStage_ptr, testPtr);

    // Clean up
    std::free(testPtr);
}

// Test 2: Ptr is null. Expect cmsStageFree to be called with a null pointer.
void test_GenericMPEfree_null_ptr() {
    g_last_cmsStage_ptr = reinterpret_cast<void*>(0xCAFEBABE); // ensure different value initially

    void* testPtr = nullptr;
    GenericMPEfree(nullptr, testPtr);

    // cmsStageFree should have been invoked with nullptr
    EXPECT_PTR_EQ(g_last_cmsStage_ptr, nullptr);
}

// Test 3: The self parameter can be non-null; it should not affect the forwarding of Ptr.
// We pass a dummy object as self and a non-null Ptr, ensuring the pointer forwarding remains correct.
void test_GenericMPEfree_self_param_ignored() {
    g_last_cmsStage_ptr = nullptr;

    // Create a dummy self object
    struct DummySelf { int value; };
    DummySelf dummy;
    dummy.value = 123;

    void* testPtr = reinterpret_cast<void*>(0x12345678);
    GenericMPEfree(reinterpret_cast<struct _cms_typehandler_struct*>(&dummy), testPtr);

    // Ensure Ptr was forwarded correctly to cmsStageFree
    EXPECT_PTR_EQ(g_last_cmsStage_ptr, testPtr);
}

// Entry point
int main() {
    // Execute tests
    test_GenericMPEfree_non_null_ptr();
    test_GenericMPEfree_null_ptr();
    test_GenericMPEfree_self_param_ignored();

    // Report summary
    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_test_failures << " TEST(S) FAILED\n";
        return g_test_failures;
    }
}