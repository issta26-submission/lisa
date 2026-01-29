/*
  Unit test suite for the focal method: mem_alloc (as defined in infcover.c)
  - This test harness is written for C++11 (no GTest).
  - It relies on the external symbol: mem_alloc(void *mem, unsigned count, unsigned size)
  - We recreate minimal dependencies (mem_zone and mem_item) consistent with mem_alloc usage.

  Step 1/2: Understanding candidate components (embedded as comments for clarity)
  - Candidate Keywords: zone (mem_zone), item (mem_item), first (head of list), total (accumulated size),
    highwater (max concurrent total), limit (allocation cap), ptr (allocated block).
  - mem_alloc behavior:
    - Fails if zone is NULL or if limit is non-zero and total + len > limit.
    - Allocates len = count * size; fills with 0xa5.
    - Creates a mem_item record, inserts at beginning of zone's list, updates total/highwater.
  - Tests should cover:
    - Normal allocation path, memory contents, and zone statistics.
    - NULL zone path.
    - Limit-exceeded path.
    - Multiple allocations updating the linked-list order and highwater.
  - Dependencies (as used by mem_alloc) to mirror in tests:
    struct mem_item { void *ptr; size_t size; struct mem_item *next; };
    struct mem_zone { struct mem_item *first; size_t total; size_t highwater; size_t limit; };
  - Static/private details are treated as opaque for testing; we rely on the public mem_alloc symbol.

  Important: The test expects mem_alloc to be externally visible (not strictly static). If the real file marks mem_alloc as static,
  this test will need to be adapted to expose it (e.g., via a wrapper in infcover.c or by compiling in the same TU).
*/

#include <cstdio>
#include <cstring>
#include <string.h>
#include <assert.h>
#include <cstddef>
#include <stdio.h>
#include <inftrees.h>
#include <zlib.h>
#include <stdlib.h>
#include <inflate.h>
#include <cstdlib>


// Declaration of the focal function under test.
// Assumes C linkage; the actual file likely defines 'local' as a macro for static or inline.
// If your build defines mem_alloc with C linkage, this declaration should be fine.
extern "C" void *mem_alloc(void *mem, unsigned count, unsigned size);

// Mirror the required internal structures used by mem_alloc.
// These definitions must match the ones used by infcover.c to ensure compatibility.
struct mem_item {
    void *ptr;
    size_t size;
    struct mem_item *next;
};

struct mem_zone {
    struct mem_item *first;
    size_t total;
    size_t highwater;
    size_t limit;
};

// Helper: clean up a mem_zone by freeing any allocated blocks and reset metadata.
static void mem_cleanup(struct mem_zone &zone) {
    struct mem_item *cur = zone.first;
    while (cur != nullptr) {
        struct mem_item *nxt = cur->next;
        free(cur->ptr);
        free(cur);
        cur = nxt;
    }
    zone.first = nullptr;
    zone.total = 0;
    zone.highwater = 0;
}

// Basic helper to print a test result for non-terminating test harness
static void report_test_result(const char *name, bool passed) {
    if (passed) {
        fprintf(stderr, "PASS: %s\n", name);
    } else {
        fprintf(stderr, "FAIL: %s\n", name);
    }
}

// Test 1: Basic allocation under zero-limit (unlimited) behavior
// - Allocate a small block, verify non-NULL, content, and zone statistics.
// - Validate that the new item is at zone.first and that the memory is filled with 0xa5.
static bool test_mem_alloc_basic_allocation() {
    // Initialize zone (unlimited limit)
    struct mem_zone zone;
    zone.first = nullptr;
    zone.total = 0;
    zone.highwater = 0;
    zone.limit = 0; // unlimited

    // Perform allocation: len = 2 * 4 = 8
    void *ptr = mem_alloc((void *)&zone, 2, 4);
    bool ok = true;

    if (ptr == nullptr) {
        report_test_result("test_mem_alloc_basic_allocation - allocation returned NULL", false);
        ok = false;
    } else {
        // Check memory contents (should be 0xa5)
        unsigned char *bytes = (unsigned char*)ptr;
        const size_t len = 8;
        for (size_t i = 0; i < len; ++i) {
            if (bytes[i] != 0xa5) {
                ok = false;
                break;
            }
        }

        // Check zone statistics
        if (zone.total != 8) ok = false;
        if (zone.highwater != 8) ok = false;
        if (zone.first == nullptr) ok = false;

        // Check that the new item points to the allocated pointer
        struct mem_item *head = zone.first;
        if (head != nullptr && head->ptr != ptr) ok = false;

        // Cleanup allocated resources
        mem_cleanup(zone);
    }

    report_test_result("test_mem_alloc_basic_allocation", ok);
    return ok;
}

// Test 2: NULL zone handling should fail safely
// - mem_alloc(NULL, ...) must return NULL and not crash.
static bool test_mem_alloc_null_zone() {
    void *ptr = mem_alloc(nullptr, 1, 8);
    bool ok = (ptr == nullptr);

    report_test_result("test_mem_alloc_null_zone", ok);
    // Nothing to cleanup since nothing was allocated
    return ok;
}

// Test 3: Allocation should fail when limit would be exceeded
// - zone.limit = 16, zone.total = 12, request len = 6 -> 18 > 16 should fail
static bool test_mem_alloc_limit_exceeded() {
    struct mem_zone zone;
    zone.first = nullptr;
    zone.total = 12;
    zone.highwater = 0;
    zone.limit = 16;

    void *ptr = mem_alloc((void *)&zone, 1, 6);
    bool ok = (ptr == nullptr);

    report_test_result("test_mem_alloc_limit_exceeded", ok);
    // No allocation should have been made; ensure no resources to cleanup
    return ok;
}

// Test 4: Multiple allocations should chain items and update highwater
// - First allocation: len=12 (3*4); Second allocation: len=16 (2*8)
// - Verify: zone.total reflects sum, highwater updated to the max, and list order (new head points to previous).
static bool test_mem_alloc_multiple_allocations_and_highwater() {
    struct mem_zone zone;
    zone.first = nullptr;
    zone.total = 0;
    zone.highwater = 0;
    zone.limit = 0; // unlimited

    // First allocation
    void *p1 = mem_alloc((void *)&zone, 3, 4); // len=12
    bool ok = true;
    if (p1 == nullptr) {
        ok = false;
    } else {
        // Second allocation
        void *p2 = mem_alloc((void *)&zone, 2, 8); // len=16
        if (p2 == nullptr) {
            ok = false;
        } else {
            // Verify stats
            if (zone.total != 28) ok = false;      // 12 + 16
            if (zone.highwater != 28) ok = false;

            // Verify list order: head should be p2, next should be p1
            struct mem_item *head = zone.first;
            if (head == nullptr || head->ptr != p2) ok = false;
            if (head->next == nullptr || head->next->ptr != p1) ok = false;

            // Verify both memory blocks contain 0xa5
            unsigned char *b1 = (unsigned char*)p1;
            for (size_t i = 0; i < 12; ++i) { if (b1[i] != 0xa5) { ok = false; break; } }
            unsigned char *b2 = (unsigned char*)p2;
            for (size_t i = 0; i < 16; ++i) { if (b2[i] != 0xa5) { ok = false; break; } }

            // Cleanup both blocks
            mem_cleanup(zone);
        }
    }

    report_test_result("test_mem_alloc_multiple_allocations_and_highwater", ok);
    return ok;
}

// Main: run all tests
int main(void) {
    bool all_ok = true;
    all_ok &= test_mem_alloc_basic_allocation();
    all_ok &= test_mem_alloc_null_zone();
    all_ok &= test_mem_alloc_limit_exceeded();
    all_ok &= test_mem_alloc_multiple_allocations_and_highwater();

    if (all_ok) {
        fprintf(stderr, "ALL TESTS PASSED\n");
        return 0;
    } else {
        fprintf(stderr, "SOME TESTS FAILED\n");
        return 1;
    }
}