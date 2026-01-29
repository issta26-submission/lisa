/*
Unit Test Suite for _cmsMallocDefaultFn in cmserr.c

Step 1 - Program Understanding (mapped to test goals)
- Core function under test: _cmsMallocDefaultFn(ContextID, size)
  - Returns NULL if size == 0 or size > MAX_MEMORY_FOR_ALLOC
  - Otherwise returns a pointer from malloc(size)
  - ContextID is unused (cmsUNUSED_PARAMETER macro)
- Key behavior to cover (true/false branches):
  - size == 0 -> NULL
  - size > MAX_MEMORY_FOR_ALLOC -> NULL
  - 1 <= size <= MAX_MEMORY_FOR_ALLOC -> non-NULL pointer from malloc
- Important dependencies:
  - cmsContext and cmsUInt32Number types
  - MAX_MEMORY_FOR_ALLOC gate and malloc
  - Context parameter is unused (no observable effect)
- Testing approach:
  - Validate NULL for boundary conditions (0 and very large size)
  - Validate non-NULL for a small valid size and ensure memory usable
  - Optional: test multiple allocations are distinct and can be freed
- Constraints / Domain knowledge considerations:
  - Use C++11 compatible tests without GTest
  - Use a lightweight, non-terminating assertion approach
  - Access C function with C linkage

Step 2 - Unit Test Generation (suite for _cmsMallocDefaultFn)
Notes:
- We implement a minimal C++ test harness (no GTest) that links against the C implementation cmserr.c.
- We exercise both branches and ensure proper memory handling (read/write and free).
- We avoid relying on MAX_MEMORY_FOR_ALLOC's exact numeric value by testing boundary at 0 and a very large size; test an explicit small size (e.g., 16) as valid case.

Step 3 - Test Case Refinement (domain knowledge enhancements)
- We include a second valid allocation test to ensure distinct allocations are returned.
- We verify basic memory write/read to ensure allocated memory is usable.
- We provide explanatory comments for each test case.

Code (C++11 test suite)
*/ 

#include <lcms2_internal.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>


// Extern C prototype for the focal function under test.
// These types are aligned with the library's expectations in cmserr.c.
typedef void* cmsContext;
typedef unsigned int cmsUInt32Number;

extern "C" void* _cmsMallocDefaultFn(cmsContext ContextID, cmsUInt32Number size);

/*
Simple non-terminating test harness:
- Each test function returns true on success, false on failure.
- main() runs all tests, prints per-test results, and returns non-zero if any test fails.
- This avoids terminating tests early and allows full test suite execution.
*/

// Test 1: size == 0 should return NULL
bool test_zero_size_returns_null() {
    // Use a dummy context (NULL). ContextID is unused by the function.
    void* ptr = _cmsMallocDefaultFn(nullptr, 0);
    bool ok = (ptr == nullptr);

    if (!ok) {
        std::fprintf(stderr, "test_zero_size_returns_null: Expected NULL for size=0, got non-NULL pointer.\n");
    } else {
        // No allocation to free; nothing to clean up.
    }

    return ok;
}

// Test 2: size > MAX_MEMORY_FOR_ALLOC should return NULL
bool test_large_size_returns_null() {
    // Use an extremely large size to guarantee it's above the internal limit.
    cmsUInt32Number largeSize = 0x7fffffff; // A very large value
    void* ptr = _cmsMallocDefaultFn(nullptr, largeSize);
    bool ok = (ptr == nullptr);

    if (!ok) {
        std::fprintf(stderr, "test_large_size_returns_null: Expected NULL for oversized allocation, got non-NULL pointer.\n");
        // If allocation unexpectedly succeeded, free to avoid leaks
        std::free(ptr);
    }

    return ok;
}

// Test 3: valid small size should allocate memory (and be usable)
bool test_valid_small_allocation_allocates_memory() {
    cmsUInt32Number size = 16; // Small, likely within MAX_MEMORY_FOR_ALLOC
    void* ptr = _cmsMallocDefaultFn(nullptr, size);
    bool ok = (ptr != nullptr);

    if (ok) {
        // Write and read to ensure memory is usable
        unsigned char* data = (unsigned char*)ptr;
        std::memset(data, 0xAB, size);
        // Verify first byte to ensure memory is writable
        if (data[0] != 0xAB) {
            ok = false;
            std::fprintf(stderr, "test_valid_small_allocation_allocates_memory: Memory not writable as expected.\n");
        }
        std::free(ptr);
    } else {
        std::fprintf(stderr, "test_valid_small_allocation_allocates_memory: Allocation returned NULL for size=%u.\n", (unsigned)size);
    }

    return ok;
}

// Test 4 (optional): multiple allocations produce distinct pointers and are independently usable
bool test_multiple_allocations_are_distinct() {
    cmsUInt32Number size = 32;
    void* p1 = _cmsMallocDefaultFn(nullptr, size);
    void* p2 = _cmsMallocDefaultFn(nullptr, size);

    bool ok = (p1 != nullptr) && (p2 != nullptr) && (p1 != p2);

    if (ok) {
        unsigned char* a = (unsigned char*)p1;
        unsigned char* b = (unsigned char*)p2;
        // Write distinct patterns
        std::memset(a, 0x5A, size);
        std::memset(b, 0xA5, size);
        // Ensure distinct content per block
        if (a[0] != 0x5A || b[0] != 0xA5) {
            ok = false;
            std::fprintf(stderr, "test_multiple_allocations_are_distinct: Memory contents do not match written patterns.\n");
        }
    }

    if (p1) std::free(p1);
    if (p2) std::free(p2);

    return ok;
}

int main() {
    int total = 0;
    int passed = 0;

    // Run Test 1
    ++total;
    if (test_zero_size_returns_null()) {
        ++passed;
        std::printf("PASSED: test_zero_size_returns_null\n");
    } else {
        std::printf("FAILED: test_zero_size_returns_null\n");
    }

    // Run Test 2
    ++total;
    if (test_large_size_returns_null()) {
        ++passed;
        std::printf("PASSED: test_large_size_returns_null\n");
    } else {
        std::printf("FAILED: test_large_size_returns_null\n");
    }

    // Run Test 3
    ++total;
    if (test_valid_small_allocation_allocates_memory()) {
        ++passed;
        std::printf("PASSED: test_valid_small_allocation_allocates_memory\n");
    } else {
        std::printf("FAILED: test_valid_small_allocation_allocates_memory\n");
    }

    // Run Test 4
    ++total;
    if (test_multiple_allocations_are_distinct()) {
        ++passed;
        std::printf("PASSED: test_multiple_allocations_are_distinct\n");
    } else {
        std::printf("FAILED: test_multiple_allocations_are_distinct\n");
    }

    std::printf("Summary: %d/%d tests passed.\n", passed, total);
    return (passed == total) ? 0 : 1;
}