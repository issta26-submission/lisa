// Test suite for Type_XYZ_Dup in cmstypes.c
// Note: This test uses plain C++11 with assertions (no Google Test or mocks).
// It exercises the focal function by supplying concrete cmsCIEXYZ data and
// validating memory duplication behavior via the internal _cmsDupMem pathway.

#include <lcms2_internal.h>
#include <cassert>
#include <cstring>
#include <cstddef>


// Include C headers with C linkage for proper type definitions.
// This relies on the project providing lcms2_internal.h with cmsCIEXYZ and related types.
extern "C" {
}

// Declare the focal function from the compilation unit under test.
extern "C" void* Type_XYZ_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n);

// Test 1: Valid input should produce a non-null duplicate with identical content.
// The returned memory should be a separate copy from the source (address should differ).
void test_Type_XYZ_Dup_valid_copy() {
    // Prepare a dummy type handler with a known ContextID.
    struct _cms_typehandler_struct self;
    self.ContextID = 42; // Arbitrary non-zero context

    // Prepare a cmsCIEXYZ source with known values.
    cmsCIEXYZ src;
    src.X = 1.0;
    src.Y = 2.0;
    src.Z = 3.0;

    // Perform duplication.
    void* dupMem = Type_XYZ_Dup(&self, &src, 1);
    assert(dupMem != NULL && "Type_XYZ_Dup should return non-null pointer for valid input");

    // Interpret the returned memory as cmsCIEXYZ and compare fields.
    cmsCIEXYZ* dup = (cmsCIEXYZ*)dupMem;
    assert(dup->X == src.X && "Duplicated X component should match source");
    assert(dup->Y == src.Y && "Duplicated Y component should match source");
    assert(dup->Z == src.Z && "Duplicated Z component should match source");

    // Ensure a distinct memory location (new copy, not the same address as source)
    assert(dup != &src && "Duplicated memory should be a distinct copy from source");

    // Note: Memory freeing is dependent on the internal CMS allocation API.
    // We avoid explicit free here to prevent mismatches with the library's allocator.
}

// Test 2: Different 'n' values should not affect the correctness of the duplicate (the function ignores 'n').
void test_Type_XYZ_Dup_ignored_n_parameter() {
    struct _cms_typehandler_struct self;
    self.ContextID = 77; // Different context

    cmsCIEXYZ src;
    src.X = -4.5;
    src.Y = 0.0;
    src.Z = 9.25;

    // Duplicate with n = 1
    void* dup1 = Type_XYZ_Dup(&self, &src, 1);
    assert(dup1 != NULL && "Duplicate with n=1 should return non-null");

    // Duplicate with a different n value
    void* dup2 = Type_XYZ_Dup(&self, &src, 999);
    assert(dup2 != NULL && "Duplicate with n=999 should return non-null");

    cmsCIEXYZ* d1 = (cmsCIEXYZ*)dup1;
    cmsCIEXYZ* d2 = (cmsCIEXYZ*)dup2;

    // Both duplicates should have identical content to the source
    assert(d1->X == src.X && d1->Y == src.Y && d1->Z == src.Z);
    assert(d2->X == src.X && d2->Y == src.Y && d2->Z == src.Z);

    // Duplicates should be separate objects
    assert(dup1 != dup2 && "Duplicates produced with different 'n' should reside at different memory addresses");
}

// Test 3: Pointer independence across multiple distinct inputs.
// Duplicating two different cmsCIEXYZ structures should yield two independent copies with correct values.
void test_Type_XYZ_Dup_ptr_independence() {
    struct _cms_typehandler_struct self;
    self.ContextID = 100; // Arbitrary

    cmsCIEXYZ src1;
    src1.X = 1.1; src1.Y = 2.2; src1.Z = 3.3;

    cmsCIEXYZ src2;
    src2.X = 4.4; src2.Y = 5.5; src2.Z = 6.6;

    void* dup1 = Type_XYZ_Dup(&self, &src1, 0);
    void* dup2 = Type_XYZ_Dup(&self, &src2, 0);

    assert(dup1 != NULL && dup2 != NULL);

    cmsCIEXYZ* d1 = (cmsCIEXYZ*)dup1;
    cmsCIEXYZ* d2 = (cmsCIEXYZ*)dup2;

    // Validate first duplicate contents
    assert(d1->X == src1.X && d1->Y == src1.Y && d1->Z == src1.Z);

    // Validate second duplicate contents
    assert(d2->X == src2.X && d2->Y == src2.Y && d2->Z == src2.Z);

    // Ensure distinct memory for both duplicates
    assert(dup1 != dup2);
}

// Entry point for the test runner
int main() {
    // Execute tests sequentially to maximize code coverage.
    test_Type_XYZ_Dup_valid_copy();
    test_Type_XYZ_Dup_ignored_n_parameter();
    test_Type_XYZ_Dup_ptr_independence();

    // If all assertions pass, return 0.
    return 0;
}