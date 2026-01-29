// Test suite for GrowMLUpool (cmsBool GrowMLUpool(cmsMLU* mlu))
// This file provides a self-contained, compilable C++11 test harness
// that mirrors the minimal runtime environment required by the focal method.
// It includes a lightweight reimplementation of the needed types and helper
// functions (_cmsRealloc) to exercise all branches of GrowMLUpool without
// external dependencies. No GoogleTest is used; tests are executed from main.

#include <cstring>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cstdint>


// Domain-specific assumptions tailored for the tests
// Provide minimal type aliases and constants to mirror the original code.

typedef unsigned int cmsUInt32Number;
typedef unsigned short cmsUInt16Number;
typedef int cmsBool;

#define TRUE 1
#define FALSE 0

// Minimal cmsMLU structure used by GrowMLUpool
struct cmsMLU {
    void* ContextID;
    void* MemPool;
    cmsUInt32Number PoolSize;
};

// Forward declaration of _cmsRealloc to match usage in GrowMLUpool
// We'll provide a lightweight implementation suitable for tests.
void* _cmsRealloc(void* ContextID, void* ptr, size_t size);

// Focal method under test (re-implemented here for a self-contained test environment)
cmsBool GrowMLUpool(cmsMLU* mlu)
{
{
    cmsUInt32Number size;
    void *NewPtr;
    // Sanity check
    if (mlu == NULL) return FALSE;
    if (mlu ->PoolSize == 0)
        size = 256;
    else
        size = mlu ->PoolSize * 2;
    // Check for overflow
    if (size < mlu ->PoolSize) return FALSE;
    // Reallocate the pool
    NewPtr = _cmsRealloc(mlu ->ContextID, mlu ->MemPool, size);
    if (NewPtr == NULL) return FALSE;

    mlu ->MemPool  = NewPtr;
    mlu ->PoolSize = size;
    return TRUE;
}
}

// Lightweight allocator-like function used by GrowMLUpool
// - If ContextID equals a designated sentinel, simulate allocation failure
// - Otherwise, behave like realloc/malloc
void* _cmsRealloc(void* ContextID, void* ptr, size_t size)
{
    // Simulate failure path for testing: use a distinct sentinel
    static const void* const FAIL_CTX = (void*)0xDEADBEEF;
    if (ContextID == FAIL_CTX) {
        // Do not allocate; mimic allocator failure
        return NULL;
    }

    // Normal path: allocate if needed, otherwise reallocate
    if (ptr == NULL) {
        return std::malloc(size);
    } else {
        return std::realloc(ptr, size);
    }
}

// Helper for test output
static int g_TotalTests = 0;
static int g_FailedTests = 0;

// Assertion helper
#define ASSERT_TRUE(cond, msg)                                                \
    do {                                                                       \
        ++g_TotalTests;                                                        \
        if (!(cond)) {                                                         \
            ++g_FailedTests;                                                 \
            std::cerr << "TEST FAILURE: " << msg << " (" << __FILE__ << ":" \
                      << __LINE__ << ")\n";                                   \
        } else {                                                             \
            /* Optional: success message could be printed/commented out */   \
        }                                                                    \
    } while (0)

#define ASSERT_EQUALS(expected, actual, msg)                                  \
    do {                                                                       \
        ++g_TotalTests;                                                        \
        if (!((expected) == (actual))) {                                     \
            ++g_FailedTests;                                                 \
            std::cerr << "TEST FAILURE: " << msg << " - expected: "            \
                      << (expected) << ", actual: " << (actual)                 \
                      << " (" << __FILE__ << ":" << __LINE__ << ")\n";        \
        }                                                                    \
    } while (0)


// Test 1: Null pointer input should return FALSE
bool test_GrowMLUpool_null_ptr()
{
    cmsBool res = GrowMLUpool(nullptr);
    ASSERT_TRUE(res == FALSE, "GrowMLUpool should return FALSE when input mlu is NULL");
    return res == FALSE;
}

// Test 2: PoolSize == 0 should allocate a 256-byte pool and set PoolSize to 256
bool test_GrowMLUpool_zero_poolsize_allocation()
{
    cmsMLU mlu;
    mlu.ContextID = (void*)1;      // non-failing context
    mlu.MemPool   = nullptr;
    mlu.PoolSize  = 0;

    cmsBool res = GrowMLUpool(&mlu);
    ASSERT_TRUE(res == TRUE, "GrowMLUpool should succeed when PoolSize is 0 and allocate 256 bytes");
    ASSERT_EQUALS(256u, mlu.PoolSize, "PoolSize should be updated to 256 after grow");
    ASSERT_TRUE(mlu.MemPool != nullptr, "MemPool should be non-NULL after successful grow");

    // cleanup
    if (mlu.MemPool) std::free(mlu.MemPool);
    return res == TRUE;
}

// Test 3: Non-zero PoolSize should double the pool and update pointers accordingly
bool test_GrowMLUpool_double_growth()
{
    cmsMLU mlu;
    mlu.ContextID = (void*)2;
    mlu.MemPool   = std::malloc(128); // initial pool
    if (mlu.MemPool == nullptr) {
        // Allocation failed; treat as test failure
        ASSERT_TRUE(false, "Initial memory allocation failed");
        return false;
    }
    mlu.PoolSize  = 128;

    cmsBool res = GrowMLUpool(&mlu);
    ASSERT_TRUE(res == TRUE, "GrowMLUpool should succeed when PoolSize > 0 (doubling)");
    ASSERT_EQUALS(256u, mlu.PoolSize, "PoolSize should be doubled to 256");
    ASSERT_TRUE(mlu.MemPool != nullptr, "MemPool should be non-NULL after grow");

    // Repeat once more to ensure continued doubling
    res = GrowMLUpool(&mlu);
    ASSERT_TRUE(res == TRUE, "GrowMLUpool should succeed on subsequent growth");
    ASSERT_EQUALS(512u, mlu.PoolSize, "PoolSize should be doubled to 512 on second growth");

    // cleanup
    if (mlu.MemPool) std::free(mlu.MemPool);
    return res == TRUE;
}

// Test 4: Overflow check: if size after doubling overflows such that size < PoolSize, return FALSE
bool test_GrowMLUpool_overflow_detection()
{
    cmsMLU mlu;
    mlu.ContextID = (void*)3;
    mlu.MemPool   = nullptr;
    mlu.PoolSize  = 0xFFFFFFFE; // 4294967294

    cmsBool res = GrowMLUpool(&mlu);
    // Expected to fail due to overflow check
    ASSERT_TRUE(res == FALSE, "GrowMLUpool should fail when doubling overflows (size < PoolSize)");
    // PoolSize should remain unchanged
    ASSERT_EQUALS(0xFFFFFFFE, mlu.PoolSize, "PoolSize should remain unchanged after failed grow");

    return res == FALSE;
}

// Test 5: Realloc failure path: ContextID indicating failure should cause GrowMLUpool to return FALSE
bool test_GrowMLUpool_realloc_failure_path()
{
    cmsMLU mlu;
    mlu.ContextID = (void*)0xDEADBEEF; // sentinel to trigger realloc failure
    mlu.MemPool   = std::malloc(128);
    if (mlu.MemPool == nullptr) {
        ASSERT_TRUE(false, "Initial allocation for test 5 failed");
        return false;
    }
    mlu.PoolSize  = 0; // first allocation path uses 256, but failure is simulated by ContextID

    cmsBool res = GrowMLUpool(&mlu);
    ASSERT_TRUE(res == FALSE, "GrowMLUpool should fail when _cmsRealloc reports NULL (simulated)");
    // Ensure MemPool and PoolSize are untouched (MemPool should remain allocated; PoolSize unchanged)
    ASSERT_EQUALS(0, mlu.PoolSize, "PoolSize should remain 0 after failed grow");
    ASSERT_TRUE(mlu.MemPool != nullptr, "MemPool should remain non-NULL after failed grow (unchanged)");

    // cleanup
    if (mlu.MemPool) std::free(mlu.MemPool);
    return res == FALSE;
}

// Main runner
int main()
{
    std::cout << "Running GrowMLUpool test suite (C++11, no GTest) ..." << std::endl;

    // Execute tests
    test_GrowMLUpool_null_ptr();
    test_GrowMLUpool_zero_poolsize_allocation();
    test_GrowMLUpool_double_growth();
    test_GrowMLUpool_overflow_detection();
    test_GrowMLUpool_realloc_failure_path();

    // Summary
    std::cout << "Tests run: " << g_TotalTests << ", Failures: " << g_FailedTests << std::endl;
    if (g_FailedTests > 0) {
        std::cerr << "Some tests FAILED. See above for details." << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "All tests PASSED." << std::endl;
    return EXIT_SUCCESS;
}