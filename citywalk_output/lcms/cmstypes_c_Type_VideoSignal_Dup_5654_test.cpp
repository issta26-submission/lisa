// Unit tests for Type_VideoSignal_Dup in cmstypes.c
// Purpose: verify the duplication behavior by exercising the focal method
// and ensuring correctness without relying on GoogleTest.
// The tests are designed to be compiled with a C/C++11 toolchain and linked
// against the lcms2 (Little CMS) library sources that provide the actual
// implementation of Type_VideoSignal_Dup and the related types.

// Notes:
// - We rely on the internal definitions from lcms2_internal.h to access
//   cmsVideoSignalType, _cms_typehandler_struct, and cmsContext.
// - We treat Type_VideoSignal_Dup as a black-box that duplicates the memory
//   pointed to by Ptr with size sizeof(cmsVideoSignalType) using the context
//   from self->ContextID. The 'n' parameter is unused in this focal method.
// - We implement a small, non-terminating test harness that records failures
//   and reports them in main().

#include <vector>
#include <lcms2_internal.h>
#include <iostream>
#include <cstring>



extern "C" {
// Forward declaration of the focal method (C linkage)
void* Type_VideoSignal_Dup(struct _cms_typehandler_struct* self, const void* Ptr, cmsUInt32Number n);
}

// Simple non-terminating assertion helper for internal test use
#define HARD_ASSERT(cond, msg, errors) \
    do { if(!(cond)) { errors.push_back(msg); } } while(0)

static bool test_Type_VideoSignal_Dup_basic_copy()
{
    // Candidate Keywords mapped: ContextID, cmsVideoSignalType, _cmsDupMem, Ptr, n

    std::vector<std::string> errors;

    // Prepare a minimal self object with only the needed member used by the focal method.
    struct _cms_typehandler_struct self;
    // Zero-initialize to simulate a neutral, clean object
    std::memset(&self, 0, sizeof(self));
    // Context field used by _cmsDupMem (type cmsContext). Use null context for determinism.
    ((cmsContext)&self)->ContextID = (cmsContext)0; // If actual layout differs, the cast ensures compilation.

    // Prepare a sample cmsVideoSignalType input with non-trivial content
    cmsVideoSignalType input;
    // Fill with a recognizable pattern to detect exact duplication
    std::memset(&input, 0xAB, sizeof(input));

    // Do duplication with a non-zero n (should be ignored by the function)
    void* dupPtr = Type_VideoSignal_Dup(&self, &input, 42);
    HARD_ASSERT(dupPtr != nullptr, "Type_VideoSignal_Dup returned null", errors);

    cmsVideoSignalType* dup = static_cast<cmsVideoSignalType*>(dupPtr);
    HARD_ASSERT(dup != nullptr, "Duplicated pointer cast failed", errors);

    // Verify that the duplicated memory is identical to the input
    int identical = std::memcmp(dup, &input, sizeof(input)) == 0;
    HARD_ASSERT(identical, "Duplicated data does not match input data", errors);

    // Ensure the duplicated memory is a separate allocation (different address)
    bool distinct_address = dupPtr != static_cast<void*>(&input);
    HARD_ASSERT(distinct_address, "Dup result points to the same memory as input", errors);

    // Mutate input after duplication and ensure dup remains unchanged
    cmsVideoSignalType input_backup = input;
    // Change input bytes to ensure separation
    std::memset(&input, 0x55, sizeof(input));

    bool still_identical = std::memcmp(dup, &input_backup, sizeof(input_backup)) == 0;
    HARD_ASSERT(still_identical, "Duplicated data was altered after input mutation", errors);

    if(!errors.empty()) {
        std::cout << "[FAIL] test_Type_VideoSignal_Dup_basic_copy\n";
        for(const auto& e : errors) std::cout << "  - " << e << "\n";
        return false;
    }

    return true;
}

static bool test_Type_VideoSignal_Dup_nIgnored()
{
    // Candidate Keywords mapped: ContextID, cmsVideoSignalType, _cmsDupMem, Ptr, n

    std::vector<std::string> errors;

    // Prepare a minimal self object
    struct _cms_typehandler_struct self;
    std::memset(&self, 0, sizeof(self));
    ((cmsContext)&self)->ContextID = (cmsContext)0;

    // Prepare input
    cmsVideoSignalType input;
    std::memset(&input, 0xCD, sizeof(input));

    // Duplicate with two different 'n' values; ensure both succeed and data is identical
    void* dup1 = Type_VideoSignal_Dup(&self, &input, 0);
    void* dup2 = Type_VideoSignal_Dup(&self, &input, 9999);

    HARD_ASSERT(dup1 != nullptr, "Type_VideoSignal_Dup (dup1) returned null", errors);
    HARD_ASSERT(dup2 != nullptr, "Type_VideoSignal_Dup (dup2) returned null", errors);

    cmsVideoSignalType* d1 = static_cast<cmsVideoSignalType*>(dup1);
    cmsVideoSignalType* d2 = static_cast<cmsVideoSignalType*>(dup2);

    HARD_ASSERT(d1 != nullptr && d2 != nullptr, "Casting duplicated pointers failed", errors);

    bool eq1 = std::memcmp(d1, &input, sizeof(input)) == 0;
    bool eq2 = std::memcmp(d2, &input, sizeof(input)) == 0;

    HARD_ASSERT(eq1, "dup1 data does not match input", errors);
    HARD_ASSERT(eq2, "dup2 data does not match input", errors);

    // Ensure that dup1 and dup2 are distinct allocations
    HARD_ASSERT(dup1 != dup2, "dup1 and dup2 should be distinct allocations", errors);

    if(!errors.empty()) {
        std::cout << "[FAIL] test_Type_VideoSignal_Dup_nIgnored\n";
        for(const auto& e : errors) std::cout << "  - " << e << "\n";
        return false;
    }

    return true;
}

int main()
{
    // Run tests and report results
    int total = 2;
    int passed = 0;

    std::cout << "Running unit tests for Type_VideoSignal_Dup...\n";

    if(test_Type_VideoSignal_Dup_basic_copy()) {
        std::cout << "PASS: test_Type_VideoSignal_Dup_basic_copy\n";
        ++passed;
    } else {
        std::cout << "FAIL: test_Type_VideoSignal_Dup_basic_copy\n";
    }

    if(test_Type_VideoSignal_Dup_nIgnored()) {
        std::cout << "PASS: test_Type_VideoSignal_Dup_nIgnored\n";
        ++passed;
    } else {
        std::cout << "FAIL: test_Type_VideoSignal_Dup_nIgnored\n";
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";

    // Non-terminating exit status: exit non-zero if any test failed
    return (passed == total) ? 0 : 1;
}