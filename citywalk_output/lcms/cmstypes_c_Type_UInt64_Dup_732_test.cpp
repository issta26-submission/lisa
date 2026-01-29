// Unit test suite for Type_UInt64_Dup in cmstypes.c
// This test is written for a C++11 environment without Google Test.
// It uses a lightweight, non-terminating assertion framework to maximize code coverage.
// The tests rely on the real cms memory duplication behavior provided by LittleCMS.

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cstdint>


// Include internal dependencies to access the type definitions used by Type_UInt64_Dup
// Adjust the include path if your project layout places lcms2_internal.h elsewhere.

// Declare the focal function with C linkage so C++ code can link against it correctly
extern "C" void* Type_UInt64_Dup(struct _cms_typehandler_struct* self, const void* Ptr, cmsUInt32Number n);

// Lightweight non-terminating assertion framework
static int g_test_failures = 0;

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::fprintf(stderr, "EXPECT_TRUE failed: %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        ++g_test_failures; \
    } \
} while(0)

#define EXPECT_EQ64(a, b) do { \
    if( (static_cast<uint64_t>(a)) != (static_cast<uint64_t>(b)) ) { \
        std::fprintf(stderr, "EXPECT_EQ64 failed: %s:%d: %" PRIu64 " != %" PRIu64 "\n", __FILE__, __LINE__, (uint64_t)(a), (uint64_t)(b)); \
        ++g_test_failures; \
    } \
} while(0)

#define EXPECT_NE_PTR(a, b) do { \
    if((void*)(a) == (void*)(b)) { \
        std::fprintf(stderr, "EXPECT_NE_PTR failed: %s:%d: pointers are equal (%p)\n", __FILE__, __LINE__, (void*)(a)); \
        ++g_test_failures; \
    } \
} while(0)

// Candidate Keywords (Step 1) mapping to observed dependencies in the focal method
// - ContextID: Context handle passed to memory duplication
// - Ptr: Source memory to duplicate
// - n: Number of elements of type cmsUInt64Number to duplicate
// - cmsUInt64Number: Data type being duplicated
// - _cmsDupMem: Underlying memory duplication routine used by Type_UInt64_Dup
// These keywords guide test focus: verify that Type_UInt64_Dup duplicates the memory of n cmsUInt64Number elements
// and that the result is a separate copy (not the same address) and contents are identical.

// Test 1: Basic duplication correctness and memory independence
// - Create a source array of 64-bit integers
// - Call Type_UInt64_Dup and verify:
//   a) The duplicated contents match the source contents
//   b) The returned pointer is different from the source pointer (independence)
//   c) Modifying the source after duplication does not affect the duplicate
static void test_Type_UInt64_Dup_basic_duplication()
{
    // Prepare a minimal self struct with a known ContextID.
    // The real struct is defined in lcms2_internal.h; initialize with zero to use default allocator.
    struct _cms_typehandler_struct self = {};
    self.ContextID = 0; // Default/null context for the test; _cmsDupMem will use this

    // Source data: 5 elements
    cmsUInt64Number source[5] = { 1ULL, 2ULL, 3ULL, 4ULL, 5ULL };

    // Perform duplication
    void* dup_void = Type_UInt64_Dup(&self, source, 5);
    EXPECT_TRUE(dup_void != nullptr);

    cmsUInt64Number* dup = static_cast<cmsUInt64Number*>(dup_void);

    // Validate contents match
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ64(dup[i], source[i]);
    }

    // Validate memory independence: modifying source should not affect duplicate
    source[0] = 0xDEADBEEFDEADBEEFULL;
    EXPECT_EQ64(dup[0], 1ULL);

    // Validate that the returned pointer is a distinct memory region
    EXPECT_NE_PTR(dup, source);
}

// Test 2: Ensure no crash and basic behavior with repeated usage on a larger dataset
static void test_Type_UInt64_Dup_large_dataset()
{
    struct _cms_typehandler_struct self = {};
    self.ContextID = 0;

    const int N = 16;
    cmsUInt64Number source[N];
    for (int i = 0; i < N; ++i) {
        source[i] = static_cast<cmsUInt64Number>(i) + 7;
    }

    void* dup_void = Type_UInt64_Dup(&self, source, N);
    EXPECT_TRUE(dup_void != nullptr);

    cmsUInt64Number* dup = static_cast<cmsUInt64Number*>(dup_void);
    for (int i = 0; i < N; ++i) {
        EXPECT_EQ64(dup[i], source[i]);
    }

    // Sanity: ensure independence even after modifying original array
    source[5] = 0x123456789ABCDEF0ULL;
    EXPECT_EQ64(dup[5],  (static_cast<cmsUInt64Number>(6) + 7)); // original value at index 5

    // Integrity check for a couple more indices
    EXPECT_EQ64(dup[0], 7ULL);
    EXPECT_EQ64(dup[15], 7ULL + 15);
}

// Main test runner
int main()
{
    // Run tests
    test_Type_UInt64_Dup_basic_duplication();
    test_Type_UInt64_Dup_large_dataset();

    if (g_test_failures == 0) {
        std::printf("All Type_UInt64_Dup tests passed.\n");
        return 0;
    } else {
        std::fprintf(stderr, "Type_UInt64_Dup tests finished with %d failure(s).\n", g_test_failures);
        return 1;
    }
}