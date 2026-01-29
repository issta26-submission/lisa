/*
Unit Test Suite for _cmsCallocDefaultFn (cmserr.c)

Notes for test author (Step 3/Domain knowledge alignment):
- We are testing the C function:
  void* _cmsCallocDefaultFn(cmsContext ContextID, cmsUInt32Number num, cmsUInt32Number size)
- We target all conditional branches inside the function:
  1) Total == 0 (returns NULL)
  2) Overflow check: if (num >= UINT_MAX / size) return NULL
  3) Overflow detection: if (Total < num || Total < size) return NULL
  4) Memory cap: if (Total > MAX_MEMORY_FOR_ALLOC) return NULL
  5) Successful allocation via _cmsMallocZero(ContextID, Total)
- The tests will rely on the actual implementation in the linked cmserr.c and the values from the lcms2 types.
- We do not use GTest; instead we implement a small, non-terminating assertion framework to allow all tests to run and report coverage.
- We exercise true/false branches for each condition at least once, including a positive path that exercises memory zero-initialization verification.
- We assume ContextID can be NULL/0 for test purposes.
- Tests are written to be as self-contained as possible, using only the C/C++ standard library and the provided lcms internal headers.
- Explanatory comments are provided for each test case.

This file should be compiled with the existing cmserr.c (or its object) linked, so that _cmsCallocDefaultFn symbol resolves correctly.
*/

#include <cstring>
#include <iostream>
#include <climits>
#include <lcms2_internal.h>
#include <cstdint>


// Import necessary types from the lcms internal header
extern "C" {
}

// Declarations for the function under test and its related free function.
// The function _cmsCallocDefaultFn is defined in cmserr.c and should be linked.
// We declare with C linkage to ensure proper symbol resolution from C++ test code.
extern "C" void* _cmsCallocDefaultFn(cmsContext ContextID, cmsUInt32Number num, cmsUInt32Number size);
extern "C" void _cmsFreeDefaultFn(cmsContext ContextID, void* Ptr);

// Simple non-terminating test framework (EXPECT-style)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT(cond, desc) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "[EXPECT FAIL] " << (desc) << std::endl; \
    } \
} while(0)


// Test 1: Total == 0 should return NULL (num == 0)
void test_zero_total_returns_null() {
    // Candidate Keywords: NUM_ZERO, TOTAL_ZERO, NULL_RETURN
    cmsContext ctx = nullptr;
    void* p1 = _cmsCallocDefaultFn(ctx, 0, 10); // Total == 0
    EXPECT(p1 == nullptr, "Total == 0 (num=0) should return NULL (calloc NULL behavior)");
    void* p2 = _cmsCallocDefaultFn(ctx, 10, 0); // Total == 0
    EXPECT(p2 == nullptr, "Total == 0 (size=0) should return NULL (calloc NULL behavior)");
}

// Test 2: Overflow check: num >= UINT_MAX / size should return NULL
void test_overflow_num_ge_threshold_returns_null() {
    // Candidate Keywords: UINT_MAX, overflow_check, threshold
    cmsContext ctx = nullptr;
    // size = 2; UINT_MAX / size == 2147483647 (for 32-bit)
    void* p = _cmsCallocDefaultFn(ctx, UINT_MAX / 2, 2);
    EXPECT(p == nullptr, "Overflow check (num >= UINT_MAX/size) should return NULL");
}

// Test 3: Overflow detection due to multiplication overflow / small Total
void test_overflow_total_less_than_num_or_size_returns_null() {
    // Candidate Keywords: TOTAL_OVERFLOW, wraparound
    cmsContext ctx = nullptr;
    // Use num = UINT_MAX, size = 2; Total wraps to 4294967294 (less than num)
    void* p = _cmsCallocDefaultFn(ctx, UINT_MAX, 2);
    EXPECT(p == nullptr, "Total wraparound should trigger NULL (Total < num) branch");
}

// Test 4: Exceed maximum allowed memory (Total > MAX_MEMORY_FOR_ALLOC)
void test_total_exceeds_max_memory_returns_null() {
    // Candidate Keywords: MAX_MEMORY_FOR_ALLOC, 512MB guard
    cmsContext ctx = nullptr;
    // Choose values that pass prior checks but exceed allocation cap
    // Ensure that the overflow check does not trigger: choose size such that num < UINT_MAX/size
    void* p = _cmsCallocDefaultFn(ctx, 70000, 10000); // Total = 700,000,000 > 512MB cap
    EXPECT(p == nullptr, "Total > MAX_MEMORY_FOR_ALLOC should return NULL (exceed cap)");
}

// Test 5: Successful allocation path with zero-initialization and proper free
void test_successful_allocation_and_zero_initialization() {
    // Candidate Keywords: successful_alloc, zero_init, free
    cmsContext ctx = nullptr;
    void* p = _cmsCallocDefaultFn(ctx, 2, 3); // Total = 6, valid
    EXPECT(p != nullptr, "Successful small allocation should return non-NULL pointer");

    unsigned char* bytes = static_cast<unsigned char*>(p);
    // Verify memory is zero-initialized (all bytes zero)
    bool all_zero = true;
    for (int i = 0; i < 6; ++i) {
        if (bytes[i] != 0) {
            all_zero = false;
            break;
        }
    }
    EXPECT(all_zero, "Allocated memory should be zero-initialized by _cmsCallocDefaultFn");

    // Free the allocated memory
    _cmsFreeDefaultFn(ctx, p);
}

// Helper to run all tests and print summary
void run_all_tests() {
    test_zero_total_returns_null();
    test_overflow_num_ge_threshold_returns_null();
    test_overflow_total_less_than_num_or_size_returns_null();
    test_total_exceeds_max_memory_returns_null();
    test_successful_allocation_and_zero_initialization();
}

int main() {
    run_all_tests();

    std::cout << "Test results: " << g_total_tests << " tests run, "
              << g_failed_tests << " failures." << std::endl;

    // Return non-zero if any test failed to indicate failure to build system
    return (g_failed_tests != 0) ? 1 : 0;
}