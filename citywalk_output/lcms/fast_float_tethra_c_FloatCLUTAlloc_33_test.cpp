// Unit test suite for FloatCLUTAlloc in fast_float_tethra.c
// This test uses a lightweight, non-GTest framework with simple EXPECT-like macros.
// It provides a controlled stub for the external allocator _cmsMallocZero to simulate
// success and failure paths.
// The tests are designed to run under C++11.

#include <cstdio>
#include <fast_float_internal.h>
#include <cstddef>
#include <cstdlib>
#include <cstdint>


// Include the production header/types required by the focal method.
// We rely on the build environment where these symbols are defined by fast_float_internal.h.

// Forward declaration of the function under test (defined in fast_float_tethra.c)
extern "C" FloatCLUTData* FloatCLUTAlloc(cmsContext ContextID, const cmsInterpParams* p);

// ---------------------------------------------------------------------------
// Mock/Override of the external allocator used by FloatCLUTAlloc
// We provide a controlled _cmsMallocZero implementation to test both branches.
// ---------------------------------------------------------------------------

// Global control for the mock allocator behavior
static bool g_forceAllocSuccess = true;

// C linkage to ensure the linker uses this symbol for _cmsMallocZero
extern "C" void* _cmsMallocZero(cmsContext ContextID, size_t size) {
    if (!g_forceAllocSuccess) {
        // Simulate allocation failure
        return NULL;
    }
    // Allocate and zero initialize memory
    void* p = calloc(1, size);
    return p;
}

// Simple lightweight test framework (non-terminating assertions)
static int g_test_failures = 0;

// EXPECT-style macros
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        fprintf(stderr, "EXPECT_TRUE failed at %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        ++g_test_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    if(!((a) == (b))) { \
        fprintf(stderr, "EXPECT_EQ failed at %s:%d: %s == %s (%lld vs %lld)\n", __FILE__, __LINE__, #a, #b, (long long)(a), (long long)(b)); \
        ++g_test_failures; \
    } \
} while(0)

// Helper to access the FloatCLUTData members safely when non-null
static void test_safe_equals(FloatCLUTData* fd, cmsContext expectedCtx, const cmsInterpParams* expectedP) {
    if (fd != NULL) {
        EXPECT_EQ(fd->ContextID, expectedCtx);
        EXPECT_EQ(fd->p, expectedP);
    }
}

// ---------------------------------------------------------------------------
// Test 1: Success path - _cmsMallocZero returns non-NULL and fields are set
// ---------------------------------------------------------------------------

void test_FloatCLUTAlloc_success() {
    // Arrange
    g_forceAllocSuccess = true;

    cmsContext ctx = (cmsContext)0x1234;
    cmsInterpParams dummyP;
    cmsInterpParams* p = &dummyP;

    // Act
    FloatCLUTData* res = FloatCLUTAlloc(ctx, p);

    // Assert
    EXPECT_TRUE(res != NULL);
    test_safe_equals(res, ctx, p);

    // Cleanup is not strictly required for this test since we don't own the memory beyond the test scope.
}

// ---------------------------------------------------------------------------
// Test 2: Null p parameter should be accepted and stored as-is
// ---------------------------------------------------------------------------

void test_FloatCLUTAlloc_null_p() {
    // Arrange
    g_forceAllocSuccess = true;

    cmsContext ctx = (cmsContext)0x5;
    cmsInterpParams* p = NULL;

    // Act
    FloatCLUTData* res = FloatCLUTAlloc(ctx, p);

    // Assert
    EXPECT_TRUE(res != NULL);
    EXPECT_EQ(res->ContextID, ctx);
    EXPECT_EQ(res->p, p);
}

// ---------------------------------------------------------------------------
// Test 3: Failure path - _cmsMallocZero returns NULL and thus the function returns NULL
// ---------------------------------------------------------------------------

void test_FloatCLUTAlloc_failure() {
    // Arrange
    g_forceAllocSuccess = false; // Force allocator to fail

    cmsContext ctx = (cmsContext)0x9;
    cmsInterpParams dummyP;
    cmsInterpParams* p = &dummyP;

    // Act
    FloatCLUTData* res = FloatCLUTAlloc(ctx, p);

    // Assert
    EXPECT_TRUE(res == NULL);

    // Reset allocator behavior for subsequent tests
    g_forceAllocSuccess = true;
}

// ---------------------------------------------------------------------------
// Test 4: Multiple allocations produce distinct pointers (no aliasing)
// ---------------------------------------------------------------------------

void test_FloatCLUTAlloc_multiple() {
    // Arrange
    g_forceAllocSuccess = true;

    cmsContext ctx1 = (cmsContext)1;
    cmsInterpParams p1;
    FloatCLUTData* a = FloatCLUTAlloc(ctx1, &p1);

    cmsContext ctx2 = (cmsContext)2;
    cmsInterpParams p2;
    FloatCLUTData* b = FloatCLUTAlloc(ctx2, &p2);

    // Assert
    EXPECT_TRUE(a != NULL);
    EXPECT_TRUE(b != NULL);
    if (a && b) {
        EXPECT_TRUE(a != b);
        // Also verify individual fields
        EXPECT_EQ(a->ContextID, ctx1);
        EXPECT_EQ(a->p, &p1);
        EXPECT_EQ(b->ContextID, ctx2);
        EXPECT_EQ(b->p, &p2);
    }

    // Note: Memory cleanup is omitted; the test process will end shortly after.
}

// ---------------------------------------------------------------------------
// Main: Run all tests and report results
// ---------------------------------------------------------------------------

int main() {
    // Run tests
    test_FloatCLUTAlloc_success();
    test_FloatCLUTAlloc_null_p();
    test_FloatCLUTAlloc_failure();
    test_FloatCLUTAlloc_multiple();

    // Report overall result
    if (g_test_failures == 0) {
        printf("All tests passed\n");
        return 0;
    } else {
        fprintf(stderr, "Total failures: %d\n", g_test_failures);
        return 1;
    }
}