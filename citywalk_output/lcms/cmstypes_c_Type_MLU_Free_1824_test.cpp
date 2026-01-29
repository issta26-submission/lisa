// Test suite for the focal method: Type_MLU_Free in cmstypes.c
// Goal: verify that Type_MLU_Free correctly delegates to cmsMLUfree for the provided Ptr.
// This test avoids GTest and uses a lightweight custom assertion mechanism.
// We provide a local mock for cmsMLUfree to observe the interaction without relying on the actual library.

// Domain-oriented notes applied:
// - Import dependencies in test context as needed by cmstypes.c linkage.
// - Exercise true/false branches by providing null and non-null Ptr.
// - Do not rely on private members; only public-like behavior is exercised.
// - Use the correct namespace and avoid external testing frameworks.
// - Execute test code from main() without GTest.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>


// Forward declarations to mirror the C interfaces used by cmstypes.c
typedef struct cmsMLU cmsMLU;
struct _cms_typehandler_struct;
extern "C" {
    // Prototype of the focal function under test (from cmstypes.c)
    void Type_MLU_Free(struct _cms_typehandler_struct* self, void* Ptr);

    // Mockable cmsMLUfree symbol. We provide our own definition in this translation unit
    // to observe calls without depending on the real library.
    void cmsMLUfree(cmsMLU* mlu);
}

// Global state for the mock to observe calls
static cmsMLU* g_lastPtrPassed = nullptr;
static int g_callCount = 0;

// Mock implementation of cmsMLUfree to intercept calls made by Type_MLU_Free.
// This mock should be sited in the same translation unit and linked with cmstypes.c.
extern "C" void cmsMLUfree(cmsMLU* mlu)
{
    g_lastPtrPassed = mlu;
    ++g_callCount;
}

// Convenience to reset mock state before each test
static void reset_mock_state()
{
    g_lastPtrPassed = nullptr;
    g_callCount = 0;
}

// Lightweight assertion macro (non-terminating) to maximize test coverage
#define ASSERT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "[FAIL] " << (msg) << " (line " << __LINE__ << ")" << std::endl; \
            return false; \
        } \
    } while (0)


// Test 1: Non-null Ptr should cause cmsMLUfree to be called with the same pointer
static bool test_Type_MLU_Free_NonNullPtr()
{
    // Prepare
    reset_mock_state();

    // Allocate a dummy cmsMLU object (only its address is used; no internal semantics required)
    typedef struct cmsMLU_local { int dummy; } cmsMLU_local;
    cmsMLU* mlu = (cmsMLU*)malloc(sizeof(cmsMLU_local));
    if (mlu == nullptr) {
        std::cerr << "Memory allocation for cmsMLU_local failed" << std::endl;
        return false;
    }

    // Exercise
    Type_MLU_Free(nullptr, (void*)mlu);

    // Verify that cmsMLUfree was invoked exactly once with the same pointer
    ASSERT_TRUE(g_callCount == 1, "expected cmsMLUfree to be called exactly once");
    ASSERT_TRUE(g_lastPtrPassed == mlu, "expected cmsMLUfree to receive the original Ptr");

    // Cleanup
    free(mlu);
    return true;
}

// Test 2: Null Ptr should still call cmsMLUfree with NULL
static bool test_Type_MLU_Free_NullPtr()
{
    // Prepare
    reset_mock_state();

    // Exercise
    Type_MLU_Free(nullptr, NULL);

    // Verify
    ASSERT_TRUE(g_callCount == 1, "expected cmsMLUfree to be called once for NULL Ptr");
    ASSERT_TRUE(g_lastPtrPassed == NULL, "expected cmsMLUfree to be called with NULL pointer");

    return true;
}

// Test 3: Non-null self parameter should not crash or misbehave.
// The Type_MLU_Free implementation does not use fields of 'self', only passes Ptr to cmsMLUfree.
static bool test_Type_MLU_Free_SelfParamNonNull()
{
    // Prepare
    reset_mock_state();

    // Create a dummy Ptr (non-null) as in Test 1
    typedef struct cmsMLU_local { int dummy; } cmsMLU_local;
    cmsMLU* mlu = (cmsMLU*)malloc(sizeof(cmsMLU_local));
    if (mlu == nullptr) {
        std::cerr << "Memory allocation for cmsMLU_local failed" << std::endl;
        return false;
    }

    // Provide a non-null dummy self pointer (not used by the focal function)
    // We cast an integer value to the expected struct pointer type to keep it simple.
    struct _cms_typehandler_struct* dummy_self = (struct _cms_typehandler_struct*)0xDEADBEEF;

    // Exercise
    Type_MLU_Free(dummy_self, (void*)mlu);

    // Verify that cmsMLUfree was invoked with the Ptr
    ASSERT_TRUE(g_callCount == 1, "expected cmsMLUfree to be called exactly once");
    ASSERT_TRUE(g_lastPtrPassed == mlu, "expected cmsMLUfree to receive the original Ptr");

    // Cleanup
    free(mlu);
    return true;
}


// Entry point for the test suite
int main()
{
    bool all_passed = true;

    std::cout << "Running tests for Type_MLU_Free..." << std::endl;

    if (!test_Type_MLU_Free_NonNullPtr()) {
        std::cerr << "Test 1 failed: Non-null Ptr path." << std::endl;
        all_passed = false;
    } else {
        std::cout << "[PASS] Test 1: Non-null Ptr path." << std::endl;
    }

    if (!test_Type_MLU_Free_NullPtr()) {
        std::cerr << "Test 2 failed: NULL Ptr path." << std::endl;
        all_passed = false;
    } else {
        std::cout << "[PASS] Test 2: NULL Ptr path." << std::endl;
    }

    if (!test_Type_MLU_Free_SelfParamNonNull()) {
        std::cerr << "Test 3 failed: Non-null self parameter path." << std::endl;
        all_passed = false;
    } else {
        std::cout << "[PASS] Test 3: Non-null self parameter path." << std::endl;
    }

    if (all_passed) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << "Some tests failed." << std::endl;
        return 1;
    }
}