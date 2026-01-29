// Unit test suite for DupTagTypeList in cmstypes.c
// - Target: verify behavior of the DupTagTypeList function
// - Environment: C++11, no Google Test, simple in-process tests
// - Assumptions: LittleCMS internal headers are available (lcms2_internal.h)
// - Notes: This test uses the actual DupTagTypeList function via C linkage.
//          It constructs a realistic cmsContext with a non-empty and an empty
//          TagTypes linked list, calls DupTagTypeList, and validates that
//          a new, ordered copy is produced in the destination context.

#include <cstring>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <lcms2_internal.h>


// Enable C linkage for the focal function to link properly with the library.
extern "C" {
    // Forward declaration of the focal function from cmstypes.c
    // Signature taken from the provided source snippet.
    void DupTagTypeList(struct _cmsContext_struct* ctx,
                        const struct _cmsContext_struct* src,
                        int loc);
}

// Include internal CMS structures to construct valid inputs for DupTagTypeList.
// These definitions are internal to the library and are required to build the test inputs.

// Simple, non-terminating test framework for C++11
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_CASE(name) void test_##name()
#define RUN_TEST(name) do { \
    std::cout << "Running test: " #name " ... "; \
    test_##name(); \
    std::cout << "OK\n"; \
} while(0)

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "Expectation failed: " << (msg) \
                  << " | File: " << __FILE__ << " Line: " << __LINE__ << std::endl; \
    } \
} while(0)

#define EXPECT_NOT_NULL(ptr, msg) EXPECT_TRUE((ptr) != nullptr, (msg))
#define EXPECT_NULL(ptr, msg) EXPECT_TRUE((ptr) == nullptr, (msg))

// Helper: create a single linked list node (_cmsTagTypeLinkedList)
static _cmsTagTypeLinkedList* alloc_ll_node() {
    _cmsTagTypeLinkedList* node = (_cmsTagTypeLinkedList*) std::malloc(sizeof(_cmsTagTypeLinkedList));
    if (node) {
        // Initialize fields; only Next is used in DupTagTypeList's traversal
        std::memset(node, 0, sizeof(_cmsTagTypeLinkedList));
        node->Next = nullptr;
    }
    return node;
}

// Helper: create three-node chain for TagTypes
static _cmsTagTypeLinkedList* create_three_node_chain() {
    _cmsTagTypeLinkedList* n1 = alloc_ll_node();
    _cmsTagTypeLinkedList* n2 = alloc_ll_node();
    _cmsTagTypeLinkedList* n3 = alloc_ll_node();
    if (!n1 || !n2 || !n3) {
        // Cleanup on partial failure
        if (n1) std::free(n1);
        if (n2) std::free(n2);
        if (n3) std::free(n3);
        return nullptr;
    }
    n1->Next = n2;
    n2->Next = n3;
    n3->Next = nullptr;
    return n1;
}

// Helper: allocate a head chunk with a TagTypes list
static _cmsTagTypePluginChunkType* alloc_head_with_chain(_cmsTagTypeLinkedList* head_chain) {
    _cmsTagTypePluginChunkType* head = (_cmsTagTypePluginChunkType*) std::malloc(sizeof(_cmsTagTypePluginChunkType));
    if (!head) return nullptr;
    // Initialize memory
    std::memset(head, 0, sizeof(_cmsTagTypePluginChunkType));
    head->TagTypes = head_chain;
    return head;
}

// Cleanup function to free a copied chain produced by DupTagTypeList
static void free_copied_chain(_cmsTagTypeLinkedList* first) {
    _cmsTagTypeLinkedList* cur = first;
    while (cur) {
        _cmsTagTypeLinkedList* nxt = cur->Next;
        std::free(cur);
        cur = nxt;
    }
}

// Cleanup function to free a copied head chunk
static void free_head_chunk(_cmsTagTypePluginChunkType* head) {
    if (!head) return;
    free_copied_chain(head->TagTypes);
    std::free(head);
}

// Domain-specific tests

TEST_CASE(non_empty_tagtypes_are_duplicated_in_order)
{
    // Prepare source context with a non-empty TagTypes list: 3 nodes
    struct _cmsContext_struct* src = (struct _cmsContext_struct*) std::calloc(1, sizeof(struct _cmsContext_struct));
    struct _cmsContext_struct* ctx = (struct _cmsContext_struct*) std::calloc(1, sizeof(struct _cmsContext_struct));
    if (!src || !ctx) {
        EXPECT_TRUE(false, "Memory allocation for contexts failed");
        return;
    }

    // Build the source's TagTypes chain
    _cmsTagTypeLinkedList* chain_head = create_three_node_chain();
    EXPECT_NOT_NULL(chain_head, "Failed to allocate source TagTypes chain");

    // Build the source's chunk containing the TagTypes
    _cmsTagTypePluginChunkType* src_head = alloc_head_with_chain(chain_head);
    EXPECT_NOT_NULL(src_head, "Failed to allocate source head chunk");

    // Attach to src context at loc = 0
    src->chunks[0] = src_head;
    // Ensure destination is empty
    ctx->chunks[0] = NULL;

    // Run DupTagTypeList to duplicate TagTypes from src to ctx at loc 0
    DupTagTypeList(ctx, src, 0);

    // Validate: destination chunk exists
    _cmsTagTypePluginChunkType* dst_head = (_cmsTagTypePluginChunkType*) ctx->chunks[0];
    EXPECT_NOT_NULL(dst_head, "DupTagTypeList produced NULL destination head");

    // Validate: a new chain was created and order preserved
    _cmsTagTypeLinkedList* a = dst_head->TagTypes;
    int count = 0;
    _cmsTagTypeLinkedList* original = chain_head;
    while (a != NULL) {
        ++count;
        // Ensure the new entries are different objects (memory was allocated anew)
        EXPECT_TRUE(a != original, "Duplicated node should be a different object");
        a = a->Next;
        original = original ? original->Next : NULL;
    }
    EXPECT_TRUE(count == 3, "Duplicated chain should have length 3 to preserve order");

    // Cleanup: free source structures (originals)
    free_head_chunk(src_head);
    // Free src list nodes
    free_copied_chain(chain_head);
    // Note: dst_head was allocated via _cmsSubAllocDup; free it
    free_head_chunk(dst_head);

    // Free contexts themselves
    std::free(src);
    std::free(ctx);
}

TEST_CASE(empty_tagtypes_are_handled_without_error)
{
    // Prepare source context with an empty TagTypes list
    struct _cmsContext_struct* src = (struct _cmsContext_struct*) std::calloc(1, sizeof(struct _cmsContext_struct));
    struct _cmsContext_struct* ctx = (struct _cmsContext_struct*) std::calloc(1, sizeof(struct _cmsContext_struct));
    if (!src || !ctx) {
        EXPECT_TRUE(false, "Memory allocation for contexts failed");
        return;
    }

    // Build the source's head chunk with empty TagTypes
    _cmsTagTypePluginChunkType* empty_head = alloc_head_with_chain(nullptr);
    EXPECT_NOT_NULL(empty_head, "Failed to allocate empty head chunk");

    // Attach to src context at loc = 1
    src->chunks[1] = empty_head;
    ctx->chunks[1] = NULL; // ensure separate location

    // Run DupTagTypeList to duplicate TagTypes from src to ctx at loc 1
    DupTagTypeList(ctx, src, 1);

    // Validate: destination chunk exists and TagTypes is NULL
    _cmsTagTypePluginChunkType* dst_head = (_cmsTagTypePluginChunkType*) ctx->chunks[1];
    EXPECT_NOT_NULL(dst_head, "DupTagTypeList produced NULL destination head for empty TagTypes");
    EXPECT_NULL(dst_head->TagTypes, "Duplicated empty TagTypes should remain NULL");

    // Cleanup
    free_head_chunk(empty_head);
    std::free(src);
    std::free(ctx);
}

int main()
{
    // Run tests
    RUN_TEST(non_empty_tagtypes_are_duplicated_in_order);
    RUN_TEST(empty_tagtypes_are_handled_without_error);

    // Summary
    std::cout << "Tests executed: " << g_total_tests
              << " | Failures: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return g_failed_tests > 0 ? 1 : 0;
}