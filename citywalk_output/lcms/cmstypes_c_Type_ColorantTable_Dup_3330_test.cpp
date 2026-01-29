// Test suite for Type_ColorantTable_Dup in cmstypes.c
// This test uses a lightweight, self-contained harness (no GTest) and relies on the
// public LittleCMS API to create a named color list and verify duplication behavior.

#include <cstring>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>


// Include LittleCMS public API. This provides cmsNAMEDCOLORLIST, cmsDupNamedColorList,
// cmsNamedColorListAlloc, cmsFreeNamedColorList, etc.

// Forward declaration of the focal function's signature.
// The focal function is defined in cmstypes.c and uses C linkage.
extern "C" {
    // The structure is forward-declared to match the function's expected parameter type.
    struct _cms_typehandler_struct;
    void* Type_ColorantTable_Dup(struct _cms_typehandler_struct* self, const void* Ptr, cmsUInt32Number n);
}

// Simple test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT(cond, msg) \
    do { \
        ++g_total_tests; \
        if (!(cond)) { \
            std::cerr << "[FAIL] " << msg << " (Line " << __LINE__ << ")" << std::endl; \
            ++g_failed_tests; \
        } else { \
            std::cout << "[PASS] " << msg << std::endl; \
        } \
    } while (0)


// Helper to safely allocate and free a Named Color List.
// We rely on the LittleCMS API: cmsNamedColorListAlloc and cmsFreeNamedColorList.
// If allocation fails, we treat as test failure.
static cmsNAMEDCOLORLIST* AllocNamedColorListSafe(cmsUInt32Number count) {
    cmsNAMEDCOLORLIST* lst = cmsNamedColorListAlloc(count);
    return lst;
}

// Test 1: Duplicating an empty named color list should return a non-null pointer
// and the duplicate should be a distinct object (pointer value different from input).
static bool test_ColorantTableDup_EmptyList() {
    // Create an empty named color list
    cmsNAMEDCOLORLIST* orig = AllocNamedColorListSafe(0);
    if (orig == nullptr) {
        std::cerr << "[ERROR] Failed to allocate empty cmsNAMEDCOLORLIST." << std::endl;
        return false;
    }

    // Call the focal function with a null self and arbitrary n (unused by the function)
    void* dupv = Type_ColorantTable_Dup(nullptr, orig, 0);
    cmsNAMEDCOLORLIST* dup = (cmsNAMEDCOLORLIST*)dupv;

    // Expectations
    bool ok = true;
    if (dup == nullptr) {
        std::cerr << "[ERROR] Type_ColorantTable_Dup returned null for empty list." << std::endl;
        ok = false;
    } else {
        if (dup == orig) {
            std::cerr << "[ERROR] Duplicate has the same pointer as original (not a real copy)." << std::endl;
            ok = false;
        }
    }

    // Cleanup
    cmsFreeNamedColorList(orig);
    if (dup != nullptr) {
        cmsFreeNamedColorList(dup);
    }

    return ok;
}

// Test 2: Ensure that using a non-null self and non-zero n does not alter the outcome.
// This checks that the function ignores these inputs (as intended) and still returns a valid duplicate.
static bool test_ColorantTableDup_WithParamsIgnored() {
    // Prepare an empty list again for a simple, reliable input
    cmsNAMEDCOLORLIST* orig = AllocNamedColorListSafe(0);
    if (orig == nullptr) {
        std::cerr << "[ERROR] Failed to allocate empty cmsNAMEDCOLORLIST for param-ignored test." << std::endl;
        return false;
    }

    // Create a dummy non-null self pointer
    struct _cms_typehandler_struct dummySelf;
    void* dupv = Type_ColorantTable_Dup(&dummySelf, orig, 12345);
    cmsNAMEDCOLORLIST* dup = (cmsNAMEDCOLORLIST*)dupv;

    // Expectations
    bool ok = true;
    if (dup == nullptr) {
        std::cerr << "[ERROR] Type_ColorantTable_Dup returned null when inputs were provided." << std::endl;
        ok = false;
    } else {
        if (dup == orig) {
            std::cerr << "[ERROR] Duplicate has the same pointer as original (not a real copy)." << std::endl;
            ok = false;
        }
    }

    // Cleanup
    cmsFreeNamedColorList(orig);
    if (dup != nullptr) {
        cmsFreeNamedColorList(dup);
    }

    return ok;
}

// Entry point
int main() {
    std::cout << "Running tests for Type_ColorantTable_Dup..." << std::endl;

    if (!test_ColorantTableDup_EmptyList()) {
        std::cerr << "Test 1 failed." << std::endl;
    } else {
        std::cout << "Test 1 passed." << std::endl;
    }

    if (!test_ColorantTableDup_WithParamsIgnored()) {
        std::cerr << "Test 2 failed." << std::endl;
    } else {
        std::cout << "Test 2 passed." << std::endl;
    }

    std::cout << "Summary: " << g_total_tests << " tests, "
              << g_failed_tests << " failures." << std::endl;

    return g_failed_tests ? EXIT_FAILURE : EXIT_SUCCESS;
}