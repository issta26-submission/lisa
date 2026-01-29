// Auto-generated C++11 unit tests for the focal method mem_setup in infcover.c
// This test suite uses plain C++ (no Google Test) and relies on the public C interface
// exposed by the project (extern "C" mem_setup) along with zlib's z_stream.
// Notes:
// - We avoid private/private-implementation access and focus on observable behavior.
// - We validate initialization of the z_stream and a best-effort inspection of the
//   internal mem_zone that mem_setup allocates and attaches to strm->opaque.
// - The approach aims for good coverage while remaining robust to small internal changes.
// - The tests are designed to be executable under a C++11 build environment.

#include <cstring>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <inftrees.h>
#include <zlib.h>
#include <stdlib.h>
#include <inflate.h>


// Include zlib types used by the focal function

// Declaration of the focal function under test (from infcover.c).
// The function is declared with C linkage to be callable from C++.
extern "C" void mem_setup(z_stream *strm);

// A forward declaration of the internal mem_zone layout as a test aid.
// The real type is private to infcover.c, but for testing purposes we mirror
// the expected fields to inspect initialization. This assumes the layout is
// compatible and is a best-effort to exercise internal state without relying
// on private access controls.
// If the actual layout differs significantly, this test may need adjustment.
struct mem_zone {
    void *first;
    unsigned long total;
    unsigned long highwater;
    unsigned long limit;
    int notlifo;
    int rogue;
};

// Simple test harness: non-terminating assertions with summary at end.
static int g_failures = 0;
#define ASSERT(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "TEST FAILURE: " << (msg) \
                      << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
            ++g_failures; \
        } \
    } while (0)

// Test 1: Basic mem_setup initialization
// - Verifies that mem_setup allocates a mem_zone and attaches it to strm->opaque.
// - Verifies zalloc and zfree function pointers are set (non-null).
// - Verifies the zone's core fields are initialized to zero/NULL as per the focal code.
void test_mem_setup_basic() {
    // The z_stream object used by the focal function
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    // Act: initialize the stream with the custom allocator setup
    mem_setup(&strm);

    // Assert: basic observable post-conditions
    ASSERT(strm.opaque != NULL, "mem_setup should set strm.opaque to a non-null zone");
    ASSERT(strm.zalloc != NULL, "mem_setup should set strm.zalloc to a valid function");
    ASSERT(strm.zfree != NULL, "mem_setup should set strm.zfree to a valid function");

    // Best-effort internal state check by inspecting the allocated zone layout
    mem_zone *zone = static_cast<mem_zone*>(strm.opaque);
    if (zone != NULL) {
        // The focal code initializes first to NULL and numeric fields to 0
        ASSERT(zone->first == NULL, "zone.first should be initialized to NULL");
        ASSERT(zone->total == 0, "zone.total should be initialized to 0");
        ASSERT(zone->highwater == 0, "zone.highwater should be initialized to 0");
        ASSERT(zone->limit == 0, "zone.limit should be initialized to 0");
        ASSERT(zone->notlifo == 0, "zone.notlifo should be initialized to 0");
        ASSERT(zone->rogue == 0, "zone.rogue should be initialized to 0");
    } else {
        ASSERT(false, "zone pointer (strm.opaque) should be castable to mem_zone");
    }
}

// Test 2: mem_setup reinitialization creates a distinct zone
// - Ensures calling mem_setup twice results in two distinct zones being allocated.
// - This checks that the function does allocate a new memory region on subsequent calls.
void test_mem_setup_reinit() {
    z_stream s1;
    z_stream s2;
    std::memset(&s1, 0, sizeof(s1));
    std::memset(&s2, 0, sizeof(s2));

    mem_setup(&s1);
    void *zone1 = s1.opaque;

    mem_setup(&s2);
    void *zone2 = s2.opaque;

    ASSERT(zone1 != NULL, "First mem_setup should produce a non-null zone");
    ASSERT(zone2 != NULL, "Second mem_setup should produce a non-null zone");
    ASSERT(zone1 != zone2, "Two consecutive mem_setup calls should allocate distinct zones");
}

// Test 3: mem_setup provides functional zalloc/zfree callbacks
// - Uses the provided function pointers to allocate and free a small block.
// - Verifies that the callbacks are callable and return non-null for a small allocation.
// - Ensures no crash occurs when freeing the allocated block via the provided callback.
void test_mem_setup_zalloc_zfree_functionality() {
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    mem_setup(&strm);

    // Ensure function pointers exist
    ASSERT(strm.zalloc != NULL, "zalloc callback should be non-null after mem_setup");
    ASSERT(strm.zfree != NULL, "zfree callback should be non-null after mem_setup");

    // Attempt a small allocation via the zalloc callback
    void *block = strm.zalloc(strm.opaque, 1, 16);
    ASSERT(block != NULL, "zalloc should return a non-null pointer for a small allocation");

    // Free the allocated block using zfree
    strm.zfree(strm.opaque, block);

    // If we reach here, the callbacks are at least invokable without immediate crash
}

// Simple test runner to execute all tests and report summary
int main() {
    std::cout << "Running mem_setup unit tests (infcover.c) with C++11 test harness..." << std::endl;

    test_mem_setup_basic();
    test_mem_setup_reinit();
    test_mem_setup_zalloc_zfree_functionality();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) FAILED" << std::endl;
        return 1;
    }
}