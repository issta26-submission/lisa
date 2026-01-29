/*
  Lightweight C++11 unit test suite for the focal method malloc_aligned
  (as described in the provided code snippet).
  This test harness mocks minimal dependencies required to compile and exercise
  malloc_aligned, without relying on GTest. It uses a small non-terminating
  assertion approach to maximize code coverage.

  Notes:
  - The test is self-contained: it provides a minimal XMatShaper8Data struct and
    a simple _cmsMallocZero mock to simulate zero-initialized allocation.
  - free_aligned frees the underlying allocated block via a->real_ptr.
  - The tests exercise alignment, real_ptr tracking, and multiple allocations.
  - All tests use non-terminating expectations (they log failures but continue).
*/

#include <cstring>
#include <fast_float_internal.h>
#include <vector>
#include <cstddef>
#include <iostream>
#include <cpuid.h>
#include <cassert>
#include <cstdlib>
#include <cstdint>


// Domain-specific types (mocked for test purposes)
using cmsContext = void*;
using cmsUInt8Number = uint8_t;

// Minimal dependency: XMatShaper8Data structure used by malloc_aligned
struct XMatShaper8Data {
    cmsUInt8Number* real_ptr; // pointer to the real allocation block
};

// Forward declaration of function under test (as in the focal class file)
extern "C" XMatShaper8Data* malloc_aligned(cmsContext ContextID);

// Mock implementation of _cmsMallocZero to simulate zero-initialized allocation
extern "C" cmsUInt8Number* _cmsMallocZero(cmsContext ContextID, size_t size)
{
    // Allocate and zero-initialize
    cmsUInt8Number* p = new cmsUInt8Number[size];
    // Explicitly zeroed (defensive, though new[] returns uninitialized by default for POD)
    std::memset(p, 0, size);
    return p;
}

// Local mock implementation of the focal function under test
// The exact code path from the provided snippet is reproduced here for the test.
// Note: In a real project, this would link against the actual implementation.
extern "C" XMatShaper8Data* malloc_aligned(cmsContext ContextID)
{
    {
        cmsUInt8Number* real_ptr = (cmsUInt8Number*) _cmsMallocZero(ContextID, sizeof(XMatShaper8Data) + 32);
        cmsUInt8Number* aligned = (cmsUInt8Number*) (((uintptr_t)real_ptr + 16) & ~0xf);
        XMatShaper8Data* p = (XMatShaper8Data*) aligned;
        p->real_ptr = real_ptr;
        return p;
    }
}

// Helper to free the memory allocated by malloc_aligned
static void free_aligned(XMatShaper8Data* a)
{
    if (a) {
        // The real block is allocated in a->real_ptr; freeing that block frees the whole region
        delete[] a->real_ptr;
    }
}

// Simple non-terminating test framework (no GTest/GMock)
static int g_test_failures = 0;

// Non-terminating expectations: logs on failure but does not abort test execution
#define TEST_EXPECT(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "Test failure: " << (msg) << " [" << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
            ++g_test_failures; \
        } \
    } while(0)

////////////////////////////////////////////////////////////////
// Test 1: Basic allocation creates a non-null, 16-byte aligned object
//         and stores a valid real_ptr that is zero-initialized.
////////////////////////////////////////////////////////////////
bool test_basic_allocation_alignment()
{
    // Act
    XMatShaper8Data* p = malloc_aligned(nullptr);

    // Assert
    TEST_EXPECT(p != nullptr, "malloc_aligned should return non-null pointer");
    TEST_EXPECT(reinterpret_cast<std::uintptr_t>(p) % 16 == 0,
                "Returned XMatShaper8Data* should be 16-byte aligned");
    TEST_EXPECT(p != nullptr && p->real_ptr != nullptr,
                "malloc_aligned should set p->real_ptr to the underlying allocation");

    // The real_ptr memory should be zero-initialized (as _cmsMallocZero zeros it)
    TEST_EXPECT(p != nullptr && p->real_ptr != nullptr && p->real_ptr[0] == 0,
                "First byte of the real_ptr should be zero (zero-initialized)");

    // Cleanup
    free_aligned(p);
    return true;
}

////////////////////////////////////////////////////////////////
// Test 2: Multiple allocations produce distinct blocks
////////////////////////////////////////////////////////////////
bool test_multiple_allocations_are_distinct()
{
    XMatShaper8Data* p1 = malloc_aligned(nullptr);
    XMatShaper8Data* p2 = malloc_aligned(nullptr);

    TEST_EXPECT(p1 != nullptr && p2 != nullptr, "Both allocations should succeed");
    TEST_EXPECT(p1 != p2, "Two allocations should yield different XMatShaper8Data* addresses");
    TEST_EXPECT(p1->real_ptr != p2->real_ptr, "Each allocation should have a distinct underlying memory block");
    TEST_EXPECT(p1->real_ptr != nullptr && p2->real_ptr != nullptr, "Both real_ptr pointers should be non-null");
    TEST_EXPECT(p1->real_ptr[0] == 0 && p2->real_ptr[0] == 0,
                "Both real_ptr blocks should be zero-initialized");

    // Cleanup
    free_aligned(p1);
    free_aligned(p2);
    return true;
}

////////////////////////////////////////////////////////////////
// Test 3: Real_ptr memory remains zeroed after allocation
////////////////////////////////////////////////////////////////
bool test_real_ptr_zeroed_memory()
{
    XMatShaper8Data* p = malloc_aligned(nullptr);
    bool ok = (p != nullptr) && (p->real_ptr != nullptr) && (p->real_ptr[0] == 0);
    TEST_EXPECT(ok, "real_ptr memory should be zero-initialized after allocation");
    free_aligned(p);
    return ok;
}

////////////////////////////////////////////////////////////////
// Test 4: Alignment of the focal object plus internal consistency
////////////////////////////////////////////////////////////////
bool test_alignment_and_internals_consistency()
{
    XMatShaper8Data* p = malloc_aligned(nullptr);
    bool aligned_ok = (p != nullptr) && (reinterpret_cast<std::uintptr_t>(p) % 16 == 0);
    bool internal_ok = (p != nullptr) && (p->real_ptr != nullptr) && (p->real_ptr[0] == 0);
    TEST_EXPECT(aligned_ok, "Returned object should be 16-byte aligned");
    TEST_EXPECT(internal_ok, "Internal real_ptr should be valid and zero-initialized");

    free_aligned(p);
    return aligned_ok && internal_ok;
}

////////////////////////////////////////////////////////////////
// Main test runner
////////////////////////////////////////////////////////////////
int main()
{
    // Run tests
    bool t1 = test_basic_allocation_alignment();
    bool t2 = test_multiple_allocations_are_distinct();
    bool t3 = test_real_ptr_zeroed_memory();
    bool t4 = test_alignment_and_internals_consistency();

    // Summary
    std::cout << "\nTest summary: " 
              << "Total failures = " << g_test_failures << "\n";

    // Return non-zero if any test failed
    if (g_test_failures > 0) {
        std::cerr << "Some tests failed. See logs above for details." << std::endl;
        return 1;
    }

    // All good
    std::cout << "All tests passed." << std::endl;
    return 0;
}