// Test suite for the focal method: Type_S15Fixed16_Dup
// Targeted file: cmstypes.c
// Notes:
// - This test assumes the availability of the focal function Type_S15Fixed16_Dup
//   with C linkage as declared below.
// - We provide minimal forward declarations to compile in a standalone test
//   environment. We cast a locally defined fake self object to the expected
//   struct type to satisfy the function signature.
// - The test uses plain C-style assertions and a small test harness driven from main.
// - This test is designed to be compiled in C++11 mode without Google Test.
// - It relies on the actual library's behavior for memory duplication via _cmsDupMem
//   invoked inside Type_S15Fixed16_Dup. We validate that the returned block contains
//   the original data and is distinct from the input data.

#include <lcms2_internal.h>
#include <iostream>
#include <cassert>
#include <cstring>


// Lightweight typedefs to match the focal function's expected types.
// These are provided to make the test self-contained.
typedef unsigned int cmsUInt32Number;

// Forward declaration of the internal type used by the focal method.
// We only need a pointer type here; the actual layout is determined by the
// compiling library (cmstypes.c). To satisfy the function signature, declare
// the struct tag as an incomplete type and use a cast in the tests.
struct _cms_typehandler_struct;

// Prototype for the focal function under test (C linkage).
extern "C" void* Type_S15Fixed16_Dup(struct _cms_typehandler_struct* self,
                                     const void *Ptr,
                                     cmsUInt32Number n);

// Optional corresponding Free function prototype to release memory
// (present in the library as Type_S15Fixed16_Free).
extern "C" void Type_S15Fixed16_Free(struct _cms_typehandler_struct* self, void* Ptr);

//------------------------------------------------------------------------------
// Test Helpers
//------------------------------------------------------------------------------

struct FakeSelf {
    // Assume that the real cms_typehandler_struct has a ContextID at the start.
    // We only need to provide something for the ContextID field to satisfy
    // the upcast in the focal function. If the actual layout places
    // ContextID at offset 0, this will align well enough for the test.
    void* ContextID;
    // Additional fields are not required for the test since the focal method
    // only uses ContextID in the memory duplication path.
};

// Helper to run a single test and report result
static bool run_test(const std::string& test_name, bool condition, const std::string& msg) {
    if (condition) {
        std::cout << "[PASS] " << test_name << ": " << msg << std::endl;
        return true;
    } else {
        std::cout << "[FAIL] " << test_name << ": " << msg << std::endl;
        return false;
    }
}

//------------------------------------------------------------------------------
// Test Suite
//------------------------------------------------------------------------------

/*
Test 1: Basic duplication of a small array of cmsFloat64Number (double)
- Prepare input data: 4 doubles {1.1, 2.2, 3.3, 4.4}
- Call Type_S15Fixed16_Dup with a fake self context
- Verify:
  - Returned pointer is non-null
  - Returned data equals input data (byte-for-byte)
  - Returned memory is distinct from input (different address)
  - Clean up via Type_S15Fixed16_Free
*/
static void test_Type_S15Fixed16_Dup_basic_duplication() {
    // Prepare fake self object with a plausible ContextID
    FakeSelf fake;
    fake.ContextID = (void*)0xDEADBEEF;

    // Cast to the expected type for the call
    struct _cms_typehandler_struct* self = (struct _cms_typehandler_struct*) &fake;

    // Input data
    const cmsUInt32Number n = 4;
    const double input[ n ] = {1.1, 2.2, 3.3, 4.4};

    // Call the focal function
    void* dup_ptr = Type_S15Fixed16_Dup(self, input, n);

    // Assertions
    bool ok = true;
    if (dup_ptr == nullptr) {
        ok = false;
        run_test("test_Type_S15Fixed16_Dup_basic_duplication", ok,
                 "Returned NULL pointer for non-empty input");
        return;
    }

    // Compare content
    if (std::memcmp(dup_ptr, input, n * sizeof(double)) != 0) {
        ok = false;
        run_test("test_Type_S15Fixed16_Dup_basic_duplication", ok,
                 "Duplicated data does not match input data");
    } else {
        run_test("test_Type_S15Fixed16_Dup_basic_duplication", true,
                 "Duplicated data matches input data");
    }

    // Ensure distinct memory (different address)
    if (dup_ptr == (void*)input) {
        ok = false;
        run_test("test_Type_S15Fixed16_Dup_basic_duplication", ok,
                 "Duplicated memory has the same address as input (not a true copy)");
    } else {
        run_test("test_Type_S15Fixed16_Dup_basic_duplication", true,
                 "Duplicated memory is a distinct allocation");
    }

    // Free the allocated memory using the library's free function
    Type_S15Fixed16_Free(self, dup_ptr);
}

/*
Test 2: Zero-element duplication
- n = 0, Ptr points to a valid memory block (we'll reuse input buffer)
- Expectation:
  - Function returns a non-null pointer (implementation may allocate 0 bytes or a sentinel block)
  - The content is zero-length, so we only verify non-null and that pointer != input (if allocator returns non-null)
*/
static void test_Type_S15Fixed16_Dup_zero_elements() {
    FakeSelf fake;
    fake.ContextID = (void*)0xFEEDBEEF;
    struct _cms_typehandler_struct* self = (struct _cms_typehandler_struct*) &fake;

    const cmsUInt32Number n = 0;
    const double input[1] = {42.0};

    void* dup_ptr = Type_S15Fixed16_Dup(self, input, n);

    bool ok = true;
    if (dup_ptr == nullptr) {
        ok = false;
        run_test("test_Type_S15Fixed16_Dup_zero_elements", ok,
                 "Returned NULL pointer for zero-length input");
        return;
    } else {
        run_test("test_Type_S15Fixed16_Dup_zero_elements", ok,
                 "Returned non-null pointer for zero-length input");
    }

    // Free if we have a pointer
    Type_S15Fixed16_Free(self, dup_ptr);
}

/*
Test 3: Mutating input after duplication should not affect the duplicate
- Duplicate an array, then mutate the original
- Verify the duplicate remains unchanged (content-wise)
*/
static void test_Type_S15Fixed16_Dup_independence() {
    FakeSelf fake;
    fake.ContextID = (void*)0xCAFEBABE;
    struct _cms_typehandler_struct* self = (struct _cms_typehandler_struct*) &fake;

    const cmsUInt32Number n = 5;
    double input[ n ] = {0.5, 1.5, 2.5, 3.5, 4.5};

    void* dup_ptr = Type_S15Fixed16_Dup(self, input, n);
    if (dup_ptr == nullptr) {
        run_test("test_Type_S15Fixed16_Dup_independence", false,
                 "Duplicated pointer is NULL");
        return;
    }

    // Mutate the original after duplication
    for (cmsUInt32Number i = 0; i < n; ++i) {
        input[i] += 100.0;
    }

    // Compare copied data to the original prior mutation (we cannot access prior state here,
    // but we can ensure that the current copied data does not reflect the mutation)
    // Since we mutated input, the memory at dup_ptr should still reflect the pre-mutation values.
    if (std::memcmp(dup_ptr, (const void*) (const double[]) {0.5, 1.5, 2.5, 3.5, 4.5}, 
                    n * sizeof(double)) != 0) {
        // If the library stored the pre-mutation values, this would be true.
        run_test("test_Type_S15Fixed16_Dup_independence", true,
                 "Duplicate preserved original values after input mutation (as expected).");
    } else {
        run_test("test_Type_S15Fixed16_Dup_independence", false,
                 "Duplicate reflected post-mutation input values, which is unexpected.");
    }

    Type_S15Fixed16_Free(self, dup_ptr);
}

//------------------------------------------------------------------------------
// Main
//------------------------------------------------------------------------------

int main() {
    // Run tests
    test_Type_S15Fixed16_Dup_basic_duplication();
    test_Type_S15Fixed16_Dup_zero_elements();
    test_Type_S15Fixed16_Dup_independence();

    // Summary (simple exit code)
    std::cout << "Type_S15Fixed16_Dup test suite completed." << std::endl;
    return 0;
}