// This test suite targets the focal method malloc_aligned in fast_float_matsh.c
// It uses a lightweight, non-GTest custom test harness to verify key properties
// without terminating on the first failure. It relies on the project headers
// (fast_float_internal.h) to provide the necessary type definitions like VXMatShaperFloatData,
// cmsContext, and cmsUInt8Number. The tests are designed to exercise true/false
// branches where applicable and to validate pointer/structure relationships.

// Compile note:
// - Ensure the object file for fast_float_matsh.c (or the library) is linked.
// - The header fast_float_internal.h should be accessible to provide type definitions.
// - If needed, provide extern "C" declarations for malloc_aligned and FreeMatShaper.

#include <cstring>
#include <iostream>
#include <cstdint>
#include <fast_float_internal.h>


// Include project-specific headers to gain access to VXMatShaperFloatData and related types

// Provide forward declarations with C linkage to ensure correct linkage when calling from C++
extern "C" VXMatShaperFloatData* malloc_aligned(cmsContext ContextID);
extern "C" void FreeMatShaper(cmsContext ContextID, void* Data);

// Lightweight test framework
static int g_fail_count = 0;

#define TEST_MSG_PASS(msg)  std::cout << "[PASS] " << msg << std::endl
#define TEST_MSG_FAIL(msg)  std::cerr << "[FAIL] " << msg << std::endl
#define TEST_ASSERT(cond, msg) do { \
    if (cond) { \
        TEST_MSG_PASS(msg); \
    } else { \
        TEST_MSG_FAIL(msg); \
        ++g_fail_count; \
    } \
} while (0)

// Test 1: Basic properties of malloc_aligned
// - Returns a non-null pointer
// - p->real_ptr is non-null
// - The address of the allocated VXMatShaperFloatData struct (p) matches the
//   computed aligned address from real_ptr using the original formula.
void test_malloc_aligned_basic() {
    // Use a neutral context (nullptr is commonly acceptable for allocation mocks in tests)
    cmsContext ctx = nullptr;

    VXMatShaperFloatData* p = malloc_aligned(ctx);
    TEST_ASSERT(p != nullptr, "malloc_aligned should return non-null pointer");

    if (p != nullptr) {
        // Access the member that stores the original allocated block pointer
        cmsUInt8Number* real_ptr = p->real_ptr;
        TEST_ASSERT(real_ptr != nullptr, "VXMatShaperFloatData.real_ptr should be non-null after allocation");

        if (real_ptr != nullptr) {
            // Compute expected aligned address using the same formula as in the source
            uintptr_t real_addr = reinterpret_cast<uintptr_t>(real_ptr);
            uintptr_t expected_aligned = (((real_addr) + 16) & ~((uintptr_t)0xF));

            uintptr_t p_addr = reinterpret_cast<uintptr_t>(p);
            TEST_ASSERT(p_addr == expected_aligned,
                        "Pointer 'p' should be located at the aligned address derived from real_ptr");
        }

        // Free the allocated memory to avoid leaks
        FreeMatShaper(ctx, p);
    }
}

// Test 2: Multiple allocations should yield distinct structures and valid real_ptrs
void test_malloc_aligned_multiple_allocations() {
    cmsContext ctx1 = nullptr;
    cmsContext ctx2 = nullptr;

    VXMatShaperFloatData* p1 = malloc_aligned(ctx1);
    VXMatShaperFloatData* p2 = malloc_aligned(ctx2);

    TEST_ASSERT(p1 != nullptr, "First malloc_aligned call should return non-null");
    TEST_ASSERT(p2 != nullptr, "Second malloc_aligned call should return non-null");
    if (p1 != nullptr && p2 != nullptr) {
        // Ensure distinct allocations (addresses should differ)
        uintptr_t a1 = reinterpret_cast<uintptr_t>(p1);
        uintptr_t a2 = reinterpret_cast<uintptr_t>(p2);
        TEST_ASSERT(a1 != a2, "Two consecutive allocations should yield different addresses");

        // Validate internal pointers
        TEST_ASSERT(p1->real_ptr != nullptr, "First allocation real_ptr should be non-null");
        TEST_ASSERT(p2->real_ptr != nullptr, "Second allocation real_ptr should be non-null");

        // Optional: verify alignment property for both
        uintptr_t r1 = reinterpret_cast<uintptr_t>(p1->real_ptr);
        uintptr_t expected1 = (((r1) + 16) & ~((uintptr_t)0xF));
        TEST_ASSERT(reinterpret_cast<uintptr_t>(p1) == expected1,
                    "First allocated structure should be aligned relative to its real_ptr");

        uintptr_t r2 = reinterpret_cast<uintptr_t>(p2->real_ptr);
        uintptr_t expected2 = (((r2) + 16) & ~((uintptr_t)0xF));
        TEST_ASSERT(reinterpret_cast<uintptr_t>(p2) == expected2,
                    "Second allocated structure should be aligned relative to its real_ptr");
    }

    // Free resources
    if (p1) FreeMatShaper(ctx1, p1);
    if (p2) FreeMatShaper(ctx2, p2);
}

int main() {
    // Run tests
    std::cout << "Starting malloc_aligned test suite for VXMatShaperFloatData\n";

    test_malloc_aligned_basic();
    test_malloc_aligned_multiple_allocations();

    std::cout << "Test suite completed. Failures: " << g_fail_count << "\n";

    // Return non-zero if any test failed to aid integration in CI scripts
    return g_fail_count ? 1 : 0;
}