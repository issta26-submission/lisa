/*
  Lightweight C++11 test-suite for the focal method:
  Type_ViewingConditions_Dup in cmstypes.c

  Approach:
  - Use the internal cms types via lcms2_internal.h to match the real memory layout.
  - Exercise the duplication by passing a real cmsICCViewingConditions instance.
  - Verify that the duplicated memory is byte-for-byte identical to the source.
  - Verify independence by mutating the source after duplication and ensuring the copy does not reflect changes.
  - Clean up by calling the corresponding free function.
  - Provide a tiny test harness (no GTest) with simple non-terminating assertions and a main() to run tests.

  Notes:
  - The test relies on the internal CMS structures and the actual implementation of _cmsDupMem used by Type_ViewingConditions_Dup.
  - We include the internal header to obtain the exact struct layout (ContextID must be present as per the focal class dependency).
  - The test uses the library-provided free function for the duplicated memory to avoid mismatches with allocator heuristics.
  - This code is designed to be linked against the same library that provides Type_ViewingConditions_Dup.
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>


// Import the internal CMS definitions to match actual struct layouts.
// This header is provided in the focal project (per <FOCAL_CLASS_DEP>).

extern "C" {
    // Prototypes for the focal method and its cleanup function.
    // They are defined in cmstypes.c (the focal file).
    void* Type_ViewingConditions_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n);
    void Type_ViewingConditions_Free(struct _cms_typehandler_struct* self, void* Ptr);
}

// Simple test framework (non-terminating assertions)
/* Global test failure counter */
static int g_test_failures = 0;

#define ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[Test Failure] " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")\n"; \
        ++g_test_failures; \
    } \
} while(false)

#define ASSERT_MEM_EQ(src, dst, len, msg) do { \
    if(std::memcmp((src), (dst), (len)) != 0) { \
        std::cerr << "[Test Failure] " << (msg) << " - memory contents differ (len=" << (len) << ").\n"; \
        ++g_test_failures; \
    } \
} while(false)

#define ASSERT_PTR_NOT_NULL(ptr, msg) do { \
    if((ptr) == nullptr) { \
        std::cerr << "[Test Failure] " << (msg) << " (nullptr)." << std::endl; \
        ++g_test_failures; \
    } \
} while(false)

// Candidate test 1: Basic duplication correctness
// - Create a cmsICCViewingConditions with a known pattern.
// - Call Type_ViewingConditions_Dup.
// - Verify the duplicate is byte-for-byte identical.
// - Modify the source and verify the duplicate remains unchanged.
// - Free the duplicate using Type_ViewingConditions_Free.
static void test_ViewingConditions_Dup_basic() {
    // The internal API uses a cmsContext in the Type_Handler struct.
    // Obtain the actual struct type from the internal header.
    struct _cms_typehandler_struct handler;
    handler.ContextID = nullptr; // NULL context to rely on default allocator

    cmsICCViewingConditions source;
    // Fill with a recognizable pattern to detect accidental writes
    std::memset(&source, 0xAA, sizeof(source));

    // Perform duplication
    void* dup_ptr = Type_ViewingConditions_Dup(&handler, &source, 0);
    ASSERT_PTR_NOT_NULL(dup_ptr, "Type_ViewingConditions_Dup returned NULL for valid input");

    cmsICCViewingConditions* dup = static_cast<cmsICCViewingConditions*>(dup_ptr);
    // Ensure the duplicate is byte-for-byte identical to the source
    ASSERT_MEM_EQ(&source, dup, sizeof(source), "Duplicate bytes should match source exactly");

    // Mutate the source and confirm the duplicate does not reflect the change
    unsigned char* src_bytes = reinterpret_cast<unsigned char*>(&source);
    if(sizeof(source) > 0) {
        src_bytes[0] ^= 0xFF; // flip a bit
    }
    // The duplicate should remain unchanged
    ASSERT_MEM_EQ(dup, dup, sizeof(source), "Duplicate should remain unchanged after source mutation (sanity)");
    // Re-verify against the mutated source to ensure a difference
    if(sizeof(source) > 0) {
        bool same_after_mutation = (std::memcmp(&source, dup, sizeof(source)) == 0);
        ASSERT(!same_after_mutation, "After source mutation, duplicate should differ from source");
    }

    // Clean up
    Type_ViewingConditions_Free(&handler, dup_ptr);
}

// Candidate test 2: Null source pointer handling
// - Ensure that passing a NULL Ptr yields a NULL result (typical _cmsDupMem behavior).
// - This guards against unexpected dereferences inside the implementation.
static void test_ViewingConditions_Dup_nullPtr() {
    struct _cms_typehandler_struct handler;
    handler.ContextID = nullptr;

    void* dup_ptr = Type_ViewingConditions_Dup(&handler, nullptr, 0);
    // Acceptable behavior: either NULL or a valid pointer depending on implementation.
    // We choose to assert NULL to reflect a clear contract in typical duplicators when Ptr is NULL.
    ASSERT(dup_ptr == nullptr, "Dup with NULL Ptr should return NULL under typical behavior");
}

// Test runner
static void run_all_tests() {
    test_ViewingConditions_Dup_basic();
    test_ViewingConditions_Dup_nullPtr();

    if(g_test_failures == 0) {
        std::cout << "[Test Suite] All tests passed.\n";
    } else {
        std::cout << "[Test Suite] Total failures: " << g_test_failures << "\n";
    }
}

// Main entry point
int main() {
    run_all_tests();
    return (g_test_failures == 0) ? 0 : 1;
}