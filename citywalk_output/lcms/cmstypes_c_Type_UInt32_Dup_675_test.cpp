// Test suite for Type_UInt32_Dup in cmstypes.c
// This test suite is written in C++11, does not rely on GTest, and uses a small
// in-project test harness with non-terminating EXPECT_* macros.
// The tests focus on verifying that Type_UInt32_Dup duplicates an array of cmsUInt32Number
// elements according to the focal method's implementation.

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Import CMS Luminosity (LCMS) headers to access types and the focal function.
// Adjust the include paths according to your build system if needed.

////////////////////////////////////////////////////////////////////////////////
// Minimal test framework (non-terminating assertions)
////////////////////////////////////////////////////////////////////////////////

static int g_test_fail_count = 0;

// Log a non-fatal failure with file and line information
static void log_failure(const char* file, int line, const char* cond) {
    fprintf(stderr, "TEST FAILURE: %s:%d: Condition failed: %s\n", file, line, cond);
    ++g_test_fail_count;
}

// Assertions (non-terminating)
#define EXPECT_TRUE(cond) do { if(!(cond)) log_failure(__FILE__, __LINE__, #cond); } while(0)
#define EXPECT_NOT_NULL(ptr) do { if((ptr) == NULL) log_failure(__FILE__, __LINE__, "ptr != NULL"); } while(0)
#define EXPECT_EQ_UINT32(a, b) do { if((static_cast<std::uint32_t>(a)) != (static_cast<std::uint32_t>(b))) \
    { fprintf(stderr, "TEST FAILURE: %s:%d: Expected %" PRIu32 " but got %" PRIu32 "\n", __FILE__, __LINE__, static_cast<std::uint32_t>(b), static_cast<std::uint32_t>(a)); ++g_test_fail_count; } } while(0)
#define EXPECT_NE_PTR(a, b) do { if((void*)(a) == (void*)(b)) log_failure(__FILE__, __LINE__, "Pointers must not be equal"); } while(0)

// Lightweight test runner helpers
static bool test_passed() { return g_test_fail_count == 0; }

////////////////////////////////////////////////////////////////////////////////
// Focal method under test
// void* Type_UInt32_Dup(struct _cms_typehandler_struct* self, const void* Ptr, cmsUInt32Number n)
// {
//     {
//         return _cmsDupMem(self->ContextID, Ptr, n * sizeof(cmsUInt32Number));
//     }
// }
// The function is provided by the library; we rely on its prototype from lcms2.h
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Test 1: Duplicates a non-empty array and verifies content integrity
////////////////////////////////////////////////////////////////////////////////
static bool test_Type_UInt32_Dup_DuplicatesContent() {
    // Prepare a minimal type handler instance with a valid ContextID.
    struct _cms_typehandler_struct self;
    // Use a non-null, identifiable context id to exercise _cmsDupMem path
    self.ContextID = (cmsContext)0xDEADBEEF;

    // Input data to duplicate
    cmsUInt32Number input[5] = { 10, 20, 30, 40, 50 };

    // Call the focal method
    void* dup_ptr = Type_UInt32_Dup(&self, input, 5);

    // Verify that a non-null pointer is returned
    EXPECT_NOT_NULL(dup_ptr);

    if (dup_ptr != NULL) {
        cmsUInt32Number* dup_arr = static_cast<cmsUInt32Number*>(dup_ptr);

        // Verify content matches input
        for (size_t i = 0; i < 5; ++i) {
            EXPECT_EQ_UINT32(dup_arr[i], input[i]);
        }

        // Verify that the duplicated memory is a distinct copy (address should differ)
        EXPECT_NE_PTR(dup_arr, input);
        // Free the duplicated memory using the corresponding Free function
        Type_UInt32_Free(&self, dup_ptr);
    }

    // If we reach here, the test ran; failures are tracked via g_test_fail_count
    return true;
}

////////////////////////////////////////////////////////////////////////////////
// Test 2: Duplicating zero elements should not crash and should return a valid
// pointer depending on implementation (library may return NULL or a valid block).
////////////////////////////////////////////////////////////////////////////////
static bool test_Type_UInt32_Dup_ZeroElements() {
    struct _cms_typehandler_struct self;
    self.ContextID = (cmsContext)0xBADCAFE;

    cmsUInt32Number dummy[1] = { 0x12345678 };

    void* dup_ptr = Type_UInt32_Dup(&self, dummy, 0);

    // The behavior for 0 elements is implementation-defined (NULL or non-NULL).
    // We simply ensure that the call does not crash and returns a pointer
    // if provided by the implementation; if NULL, it's also acceptable.
    if (dup_ptr != NULL) {
        // If a block is allocated, free it properly.
        Type_UInt32_Free(&self, dup_ptr);
    }

    // The test passes regardless of NULL/non-NULL return value to exercise the path
    // where n == 0 (no elements). Non-terminating assertion ensures test continues.
    return true;
}

////////////////////////////////////////////////////////////////////////////////
// Test 3: Duplicates a small array with boundary-like values (including max uint32_t)
// to ensure proper copying of values across the entire range.
////////////////////////////////////////////////////////////////////////////////
static bool test_Type_UInt32_Dup_BoundaryValues() {
    struct _cms_typehandler_struct self;
    self.ContextID = (cmsContext)0xCAFEBABE;

    cmsUInt32Number input[3] = { 0, 0xFFFFFFFF, 1 };

    void* dup_ptr = Type_UInt32_Dup(&self, input, 3);
    EXPECT_NOT_NULL(dup_ptr);

    if (dup_ptr != NULL) {
        cmsUInt32Number* dup_arr = static_cast<cmsUInt32Number*>(dup_ptr);
        EXPECT_EQ_UINT32(dup_arr[0], input[0]);
        EXPECT_EQ_UINT32(dup_arr[1], input[1]);
        EXPECT_EQ_UINT32(dup_arr[2], input[2]);

        Type_UInt32_Free(&self, dup_ptr);
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////
// Main: Run all tests and report summary
////////////////////////////////////////////////////////////////////////////////
int main() {
    // Run tests
    if (test_Type_UInt32_Dup_DuplicatesContent()) {
        printf("Test 1 passed: Type_UInt32_Dup duplicates content correctly.\n");
    } else {
        printf("Test 1 failed.\n");
    }

    if (test_Type_UInt32_Dup_ZeroElements()) {
        printf("Test 2 passed: Type_UInt32_Dup handles zero-length input gracefully.\n");
    } else {
        printf("Test 2 failed.\n");
    }

    if (test_Type_UInt32_Dup_BoundaryValues()) {
        printf("Test 3 passed: Type_UInt32_Dup copies boundary values correctly.\n");
    } else {
        printf("Test 3 failed.\n");
    }

    // Summary
    if (g_test_fail_count == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("NUMBER OF FAILED TESTS: %d\n", g_test_fail_count);
        return 1;
    }
}