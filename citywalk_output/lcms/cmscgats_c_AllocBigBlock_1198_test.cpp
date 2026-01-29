// Self-contained unit test suite for AllocBigBlock (C++11, no GTest)
// This test suite is designed to be compiled and run independently.
// It mocks the internal memory allocation functions used by AllocBigBlock
// and verifies the true/false branches of the conditional logic.

#include <cstring>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <lcms2_internal.h>


// Domain-specific minimal declarations to mirror the focal function's usage

typedef void cmsContext;
typedef unsigned int cmsUInt32Number;

// Minimal OWNEDMEM structure used by AllocBigBlock
typedef struct OWNEDMEM {
    void* Ptr;
    struct OWNEDMEM* Next;
} OWNEDMEM;

// Minimal cmsIT8 structure used by AllocBigBlock
typedef struct cmsIT8 {
    cmsContext* ContextID;
    OWNEDMEM* MemorySink;
} cmsIT8;

// Forward declaration of the focal function (mocked within this TU)
void* AllocBigBlock(cmsIT8* it8, cmsUInt32Number size);

// Mockable internal memory allocation functions used by AllocBigBlock
// These definitions override any real library implementations when linked in this TU.
void* _cmsMallocZero(void* ContextID, size_t size);
void  _cmsFree(void* ContextID, void* ptr);

// ------------------------------------------------------------------
// Test Harness: Mocks for memory management to drive different paths
// ------------------------------------------------------------------

// Global state to control allocation success/failure in tests
static int gAllocCallCounter = 0;
static int gFailOnCall = -1; // -1 means never fail, 1-based call counting otherwise

// Tracking allocations for verification and cleanup
static std::vector<void*> gActiveAllocs;

// Simple assertion framework: non-terminating assertions
static int gTestFailures = 0;
#define ASSERT_TRUE(cond, msg)                                                         \
    do { if(!(cond)) { fprintf(stderr, "FAILED: %s\n", msg); ++gTestFailures; } } while(0)
#define ASSERT_EQ(a, b, msg)                                                           \
    do { if((a) != (b)) { fprintf(stderr, "FAILED: %s (expected %u, got %u)\n", msg, (unsigned)(b), (unsigned)(a)); ++gTestFailures; } } while(0)
#define ASSERT_PTR_EQ(a, b, msg)                                                       \
    do { if((void*)(a) != (void*)(b)) { fprintf(stderr, "FAILED: %s (pointers differ)\n", msg); ++gTestFailures; } } while(0)

// Reset global state before each test
static void reset_memory_simulation() {
    gAllocCallCounter = 0;
    gFailOnCall = -1;
    gActiveAllocs.clear();
}

// Cleanup all allocations tracked during the test
static void cleanup_allocations(cmsIT8* it8) {
    for (void* p : gActiveAllocs) {
        if (p) {
            // emulate library behavior calling _cmsFree for allocations
            _cmsFree(nullptr, p);
        }
    }
    gActiveAllocs.clear();

    // Additionally, free any OWNEDMEM nodes (the linked list in it8)
    OWNEDMEM* node = it8->MemorySink;
    while (node != NULL) {
        OWNEDMEM* next = node->Next;
        // Free the node itself (allocated as OWNEDMEM block)
        _cmsFree(nullptr, node);
        node = next;
    }
    it8->MemorySink = NULL;
}

// Implementation of mocked _cmsMallocZero
void* _cmsMallocZero(void* ContextID, size_t size) {
    (void)ContextID; // not used in mock
    ++gAllocCallCounter;
    if (gFailOnCall == gAllocCallCounter) {
        return NULL; // simulate allocation failure on exact call
    }
    void* p = std::calloc(1, size);
    if (p) {
        gActiveAllocs.push_back(p);
    }
    return p;
}

// Implementation of mocked _cmsFree
void _cmsFree(void* ContextID, void* ptr) {
    (void)ContextID;
    if (ptr) {
        // remove from active allocations list if present
        auto it = std::find(gActiveAllocs.begin(), gActiveAllocs.end(), ptr);
        if (it != gActiveAllocs.end()) {
            gActiveAllocs.erase(it);
        }
        std::free(ptr);
    }
}

// ------------------------------------------------------------------
// Implementation of AllocBigBlock (replica for unit testing)
// This mirrors the focal method's logic as provided.
// It uses the mocked _cmsMallocZero/_cmsFree so tests can drive scenarios.
// ------------------------------------------------------------------
void* AllocBigBlock(cmsIT8* it8, cmsUInt32Number size)
{
{
    OWNEDMEM* ptr1;
    void* ptr = _cmsMallocZero(it8->ContextID, size);
    if (ptr != NULL) {
        ptr1 = (OWNEDMEM*) _cmsMallocZero(it8 ->ContextID, sizeof(OWNEDMEM));
        if (ptr1 == NULL) {
            _cmsFree(it8 ->ContextID, ptr);
            return NULL;
        }
        ptr1-> Ptr        = ptr;
        ptr1-> Next       = it8 -> MemorySink;
        it8 -> MemorySink = ptr1;
    }
    return ptr;
}
}

// ------------------------------------------------------------------
// Test Cases
// ------------------------------------------------------------------

static void test_allocbigblock_first_allocation_failure() {
    // Expect AllocBigBlock to return NULL when the first allocation fails
    reset_memory_simulation();
    cmsIT8 it8;
    it8.ContextID = (cmsContext*)0x1;
    it8.MemorySink = NULL;

    // Configure mock to fail on the first allocation
    gFailOnCall = 1;

    void* res = AllocBigBlock(&it8, 64);

    ASSERT_TRUE(res == NULL, "AllocBigBlock should return NULL if first allocation fails");
    ASSERT_TRUE(it8.MemorySink == NULL, "MemorySink should remain NULL when first alloc fails");
    // No allocations should remain active
    ASSERT_EQ((unsigned)gActiveAllocs.size(), 0u, "No allocations should remain after failure");
    // Ensure exactly one allocation attempted
    ASSERT_EQ((unsigned)gAllocCallCounter, 1u, "Exactly one allocation attempt should occur");
    cleanup_allocations(&it8);
}

static void test_allocbigblock_second_allocation_failure() {
    // First alloc succeeds, second (OWNEDMEM) alloc fails
    reset_memory_simulation();
    cmsIT8 it8;
    it8.ContextID = (cmsContext*)0x1;
    it8.MemorySink = NULL;

    // Configure mock: first call succeed, second fail (call #2)
    gFailOnCall = 2;

    void* res = AllocBigBlock(&it8, 32);

    ASSERT_TRUE(res == NULL, "AllocBigBlock should return NULL if second allocation fails");
    ASSERT_TRUE(it8.MemorySink == NULL, "MemorySink should remain NULL when second alloc fails");
    // The first allocation should have been freed due to failure of second
    ASSERT_EQ((unsigned)gActiveAllocs.size(), 0u, "All allocations should be freed after second fail");
    // Two allocation attempts
    ASSERT_EQ((unsigned)gAllocCallCounter, 2u, "Two allocation attempts should occur");
    cleanup_allocations(&it8);
}

static void test_allocbigblock_both_allocations_succeed() {
    // Both allocations succeed: verify ptr and MemorySink linkage
    reset_memory_simulation();
    cmsIT8 it8;
    it8.ContextID = (cmsContext*)0x1;
    it8.MemorySink = NULL;

    gFailOnCall = -1; // never fail

    void* first = AllocBigBlock(&it8, 16);

    ASSERT_TRUE(first != NULL, "First allocation should succeed");
    ASSERT_TRUE(it8.MemorySink != NULL, "MemorySink should be updated after first allocation");
    OWNEDMEM* top = it8.MemorySink;
    ASSERT_PTR_EQ(top->Ptr, first, "Top node should point to the first allocated block");
    ASSERT_TRUE(top->Next == NULL, "First node Next should be NULL");

    // Free all after test
    cleanup_allocations(&it8);
}

static void test_allocbigblock_chain_of_two_nodes() {
    // Allocate two blocks on successive calls; ensure a linked chain of two OWNEDMEM nodes exists
    reset_memory_simulation();
    cmsIT8 it8;
    it8.ContextID = (cmsContext*)0x1;
    it8.MemorySink = NULL;

    gFailOnCall = -1;

    void* first = AllocBigBlock(&it8, 8);
    ASSERT_TRUE(first != NULL, "First allocation should succeed");
    ASSERT_TRUE(it8.MemorySink != NULL, "MemorySink should be updated after first allocation");
    OWNEDMEM* firstNode = it8.MemorySink;
    ASSERT_PTR_EQ(firstNode->Ptr, first, "First node should point to first allocated block");
    ASSERT_TRUE(firstNode->Next == NULL, "First node Next should be NULL");

    void* second = AllocBigBlock(&it8, 4);
    ASSERT_TRUE(second != NULL, "Second allocation should succeed");
    // MemorySink should now be a new node with Next pointing to first node
    OWNEDMEM* secondNode = it8.MemorySink;
    ASSERT_PTR_EQ(secondNode->Ptr, second, "Second node should point to second allocated block");
    ASSERT_TRUE(secondNode->Next == firstNode, "Second node Next should point to first node");

    // Cleanup
    cleanup_allocations(&it8);
}

// Entry point to run tests
int main(void) {
    printf("Starting AllocBigBlock unit tests (self-contained, no external framework)\n");

    test_allocbigblock_first_allocation_failure();
    test_allocbigblock_second_allocation_failure();
    test_allocbigblock_both_allocations_succeed();
    test_allocbigblock_chain_of_two_nodes();

    if (gTestFailures == 0) {
        printf("ALL TESTS PASSED\n");
    } else {
        printf("TOTAL FAILED: %d\n", gTestFailures);
    }

    return (gTestFailures == 0) ? 0 : 1;
}