// Unit tests for Type_Screening_Dup in cmstypes.c
// Target: ensure Type_Screening_Dup correctly duplicates a cmsScreening object
// without depending on the 'n' parameter and that the duplication is independent
// of the source after duplication.
// Assumptions: The project is buildable with C++11, uses lcms headers, and Type_Screening_Dup
// is exposed with C linkage. We rely on the library's _cmsDupMem implementation to perform
// the actual memory duplication.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// Import necessary dependencies and type definitions.
// Adjust the include path as needed for the build environment.

// Ensure C linkage for the focal function when calling from C++
extern "C" void* Type_Screening_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n);

// Helper macro for lightweight non-terminating assertions
#define EXPECT_TRUE(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "EXPECT_TRUE failed: " << (msg) \
                      << " (at " << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
            tests_passed = false; \
        } \
    } while(0)

static bool tests_passed = true;

// Test 1: Basic duplication correctness and memory equality
// - Create a source cmsScreening with known content.
// - Call Type_Screening_Dup and verify the returned pointer is non-null.
// - Verify that the duplicated memory matches the source exactly.
// - Verify the duplication is independent: modifying the source after duplication does not alter the duplicate.
bool test_Type_Screening_Dup_basic() {
    // Prepare a fake type handler with a ContextID (cmsContext is typically a pointer type)
    struct _cms_typehandler_struct handler;
    std::memset(&handler, 0, sizeof(handler));
    handler.ContextID = (cmsContext)0x1234;

    // Prepare a source cmsScreening with a known pattern
    cmsScreening src;
    std::memset(&src, 0xAB, sizeof(src));  // fill with a recognizable pattern

    // Copy for independence test
    cmsScreening src_before = src;

    // Call the focal function
    void* dup = Type_Screening_Dup(&handler, &src, 0);

    EXPECT_TRUE(dup != nullptr, "dup should not be NULL");

    // The duplicated memory should be identical to the source
    if (dup != nullptr) {
        int cmp_same = std::memcmp(dup, &src, sizeof(src)) == 0;
        EXPECT_TRUE(cmp_same, "duplicated memory should equal source memory");
    }

    // Independence check: modify source after duplication and ensure dup remains unchanged
    std::memset(&src, 0x00, sizeof(src));  // change source post-dup
    int cmp_unchanged = std::memcmp(dup, &src_before, sizeof(src_before)) == 0;
    EXPECT_TRUE(cmp_unchanged, "duplication should be independent of subsequent changes to source");

    return tests_passed;
}

// Test 2: The n parameter is ignored by Type_Screening_Dup
// - Ensure that using different values for n does not affect the result.
// - The content of the duplicated memory should still match the source at the time of duplication.
bool test_Type_Screening_Dup_nParamIgnored() {
    struct _cms_typehandler_struct handler;
    std::memset(&handler, 0, sizeof(handler));
    handler.ContextID = (cmsContext)0x5678;

    cmsScreening src;
    std::memset(&src, 0xCD, sizeof(src));

    // Keep a copy to compare against after the first duplication
    cmsScreening src_copy1 = src;
    void* dup1 = Type_Screening_Dup(&handler, &src, 1234);
    EXPECT_TRUE(dup1 != nullptr, "dup1 should not be NULL");
    if (dup1) {
        int same1 = std::memcmp(dup1, &src, sizeof(src)) == 0;
        EXPECT_TRUE(same1, "dup1 should match source content");
    }

    // Change n to another value and duplicate again
    cmsScreening src_copy2 = src;
    void* dup2 = Type_Screening_Dup(&handler, &src_copy2, 0);
    EXPECT_TRUE(dup2 != nullptr, "dup2 should not be NULL");
    if (dup2) {
        int same2 = std::memcmp(dup2, &src_copy2, sizeof(src_copy2)) == 0;
        EXPECT_TRUE(same2, "dup2 should match source content even with different n");
    }

    return tests_passed;
}

// Entry point for running tests
int main() {
    std::cout << "Running Type_Screening_Dup unit tests...\n";

    bool all_passed = true;

    if (!test_Type_Screening_Dup_basic()) {
        std::cerr << "Test 1 (basic duplication) failed.\n";
        all_passed = false;
    } else {
        std::cout << "Test 1 passed.\n";
    }

    // Reset flag for next test
    if (!test_Type_Screening_Dup_nParamIgnored()) {
        std::cerr << "Test 2 (n parameter ignored) failed.\n";
        all_passed = false;
    } else {
        std::cout << "Test 2 passed.\n";
    }

    if (all_passed) {
        std::cout << "All Type_Screening_Dup tests passed.\n";
        return 0;
    } else {
        std::cerr << "Some Type_Screening_Dup tests failed.\n";
        return 1;
    }
}