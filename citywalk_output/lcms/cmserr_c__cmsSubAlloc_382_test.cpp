// Lightweight C++11 test suite for the focal method _cmsSubAlloc in cmserr.c
// This test uses the project's internal CMS structures via the included lcms2_internal.h
// and links against the production implementation cmserr.c (or the library providing these symbols).
// It avoids using Google Test and instead provides a small in-file test harness with non-terminating checks.
// Notes:
// - We rely on the internal structures used by Little CMS (as exposed via lcms2_internal.h):
//   _cmsSubAllocator, _cmsSubAllocator_chunk and the helper functions _cmsCreateSubAllocChunk,
//   _cmsCreateSubAlloc, _cmsSubAlloc, and _cmsSubAllocDestroy.
// - The tests focus on two main paths:
//     a) Allocation within the existing chunk (no new chunk allocated).
//     b) Allocation when there isn't enough space in the current chunk, triggering a new chunk.
// - This harness prints PASS/FAIL results and continues execution on failure to maximize coverage.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>


// Import project dependencies.
// The focal method is defined in cmserr.c and uses internal CMS types.
// Include lcms2_internal.h to obtain the necessary type definitions and function prototypes.

// Ensure C linkage for the production API when called from C++
extern "C" {
    // Production API used by the tests
    void* _cmsSubAlloc(_cmsSubAllocator* sub, cmsUInt32Number size);
    _cmsSubAllocator_chunk* _cmsCreateSubAllocChunk(cmsContext ContextID, cmsUInt32Number Initial);
    _cmsSubAllocator* _cmsCreateSubAlloc(cmsContext ContextID, cmsUInt32Number Initial);
    void _cmsSubAllocDestroy(_cmsSubAllocator* sub);
}

// Simple non-terminating assertion helper
static bool chk_true(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "CHECK FAIL: " << msg << std::endl;
        // Do not abort; continue to allow broader coverage
        return false;
    }
    return true;
}

// Helper to print a test header
static void test_header(const char* name) {
    std::cout << "=== TEST: " << name << " ===" << std::endl;
}

/*
Test 1: Sub-allocator allocates within the existing chunk (no new chunk).
- Setup: Create a sub-allocator with an initial block (Initial = 64 bytes).
- Action: Request a size that fits (e.g., 16 bytes).
- Expectations:
  - Returned pointer should be exactly at Block start of the current chunk.
  - sub->h->Used should increase by the requested size (16).
  - The top (current) chunk remains the same (no new chunk created).
*/
static bool test_subAlloc_inside_existing_chunk() {
    test_header("subAlloc_inside_existing_chunk (no new chunk)");

    _cmsSubAllocator* sub = _cmsCreateSubAlloc(nullptr, 64);
    if (!sub) {
        std::cerr << "Failed to create _cmsSubAlloc with Initial=64" << std::endl;
        return false;
    }

    // Perform allocation that fits into the initial block
    void* ptr = _cmsSubAlloc(sub, 16);
    if (!ptr) {
        std::cerr << "SubAlloc returned NULL for a fitting size" << std::endl;
        _cmsSubAllocDestroy(sub);
        return false;
    }

    bool ok = true;
    // Expected: ptr == sub->h->Block (start of the current chunk)
    ok = chk_true(ptr == (void*)sub->h->Block, "Returned pointer should point to Block start of the current chunk");

    // Expected: Used increased by 16
    ok = chk_true(sub->h->Used == 16, "sub->h->Used should be 16 after allocation");

    // No new chunk should have been created; the top chunk should be the original one
    ok = chk_true(sub->h != NULL, "Current chunk should not be NULL");
    _cmsSubAllocDestroy(sub);

    return ok;
}

/*
Test 2: Sub-allocator allocates via a new chunk when there isn't enough space in the current chunk.
- Setup: Create a sub-allocator with an initial block (Initial = 64 bytes).
- Action: Request a size larger than Free (e.g., 80 bytes).
- Expectations:
  - A new chunk is created and linked as the new top (sub->h).
  - The returned pointer equals the Block of the new top chunk (newChunk->Block).
  - The new top chunk's BlockSize equals the computed newSize (128 in this scenario).
  - sub->h->Used equals the requested size (80).
  - The old chunk remains linked in the list (sub->h->next points to the old chunk).
*/
static bool test_subAlloc_allocates_new_chunk() {
    test_header("subAlloc_allocates_new_chunk");

    _cmsSubAllocator* sub = _cmsCreateSubAlloc(nullptr, 64);
    if (!sub) {
        std::cerr << "Failed to create _cmsSubAlloc with Initial=64" << std::endl;
        return false;
    }

    _cmsSubAllocator_chunk* oldChunk = sub->h;
    // Request a size that cannot fit into the initial 64-byte block
    void* ptr = _cmsSubAlloc(sub, 80);
    if (!ptr) {
        std::cerr << "SubAlloc failed to allocate larger size that requires a new chunk" << std::endl;
        _cmsSubAllocDestroy(sub);
        return false;
    }

    bool ok = true;
    // After allocation, a new chunk should be the top
    ok = chk_true(sub->h != NULL, "New top chunk should exist");
    ok = chk_true(sub->h != oldChunk, "Top chunk should be the new chunk after expansion");

    // Returned pointer should point to the new chunk's Block
    ok = chk_true(ptr == (void*)sub->h->Block, "Returned pointer should point to new top chunk's Block");

    // New top chunk should have BlockSize of at least 128 (BlockSize * 2 from 64)
    ok = chk_true(sub->h->BlockSize >= 128, "New top chunk BlockSize should be at least 128");

    // Used should equal the requested size (80)
    ok = chk_true(sub->h->Used == 80, "New top chunk Used should be 80 after allocation");

    // Old chunk should still be in the chain (as next)
    ok = chk_true(sub->h->next == oldChunk, "Old chunk should be linked as next of the new top chunk");

    _cmsSubAllocDestroy(sub);
    return ok;
}

/*
Test 3: Boundary case - exact fit of the initial block.
- Setup: Create a sub-allocator with an initial block of 64 bytes.
- Action: Request exactly 64 bytes.
- Expectations:
  - Returned pointer is at the start of the initial Block.
  - Used becomes exactly 64.
  - No new chunk is created.
*/
static bool test_subAlloc_exact_fit_initial_block() {
    test_header("subAlloc_exact_fit_initial_block");

    _cmsSubAllocator* sub = _cmsCreateSubAlloc(nullptr, 64);
    if (!sub) {
        std::cerr << "Failed to create _cmsSubAlloc with Initial=64" << std::endl;
        return false;
    }

    void* ptr = _cmsSubAlloc(sub, 64);
    if (!ptr) {
        std::cerr << "SubAlloc returned NULL on exact fit" << std::endl;
        _cmsSubAllocDestroy(sub);
        return false;
    }

    bool ok = true;
    ok = chk_true(ptr == (void*)sub->h->Block, "Pointer should be at start of initial Block on exact fit");
    ok = chk_true(sub->h->Used == 64, "Used should be 64 after exact fit");

    _cmsSubAllocDestroy(sub);
    return ok;
}

// Main test runner
int main() {
    int total = 0;
    int passed = 0;

    std::cout << "Starting _cmsSubAlloc focused test suite (no GTest)..." << std::endl;

    // Run tests
    if (test_subAlloc_inside_existing_chunk()) ++passed;
    ++total;

    if (test_subAlloc_allocates_new_chunk()) ++passed;
    ++total;

    if (test_subAlloc_exact_fit_initial_block()) ++passed;
    ++total;

    // Summary
    std::cout << "Test results: " << passed << " / " << total << " tests passed." << std::endl;

    // Return non-zero if any test failed (best effort)
    return (passed == total) ? 0 : 1;
}