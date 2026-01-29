// This test suite targets the DupTagList function implemented in cmstypes.c.
// The suite uses a lightweight, self-contained approach (no GTest) and relies on
// the public/internal CMS types provided by the project headers (lcms2_internal.h).
// It also provides a minimal mock for the _cmsSubAllocDup function to enable
// controlled memory duplication during tests without requiring the full allocator logic.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


// Forward declarations pulled from the CMS headers (vialcms2_internal.h in the real project).
// We rely on the project-provided definitions for correctness during linking.
extern "C" {
}

// The DupTagList function is defined in cmstypes.c with C linkage.
// We declare it here so the test can call it directly.
extern "C" void DupTagList(struct _cmsContext_struct* ctx, const struct _cmsContext_struct* src);

// Provide a minimal C linkage mock for _cmsSubAllocDup used by DupTagList during tests.
// We implement a simple malloc+memcpy allocator to duplicate memory blocks.
// Note: This is a test-only mock. The real project uses a more complex allocator,
// but this mock preserves behavior needed to validate DupTagList's logic.
extern "C" void* _cmsSubAllocDup(void* MemPool, const void* Ptr, size_t Size) {
    (void)MemPool; // MemPool is ignored in this mock.
    if (Ptr == nullptr || Size == 0) return nullptr;
    void* p = std::malloc(Size);
    if (!p) return nullptr;
    std::memcpy(p, Ptr, Size);
    return p;
}

// Simple test framework primitives (no external libraries).
#define TEST_ASSERT(condition)                                           \
    do {                                                                 \
        if (!(condition)) {                                              \
            std::cerr << "Assertion failed: " #condition                 \
                      << " at " << __FILE__ << ":" << __LINE__ << "\n";  \
            return false;                                              \
        }                                                                \
    } while (0)


// Test 1: DupTagList duplicates a non-empty TagPlugin linked list and preserves order.
// Precondition: src has a TagPlugin head with N nodes linked via Next.
// Postcondition: dest (ctx) has a new TagPlugin head with a list of N nodes,
// and no node in the destination equals its corresponding source node
// (i.e., a deep copy with preserved order).
bool test_dup_taglist_non_empty_three_nodes() {
    // Prepare source context (src)
    struct _cmsContext_struct* src = (struct _cmsContext_struct*) std::calloc(1, sizeof(struct _cmsContext_struct));
    if (!src) return false;

    // Build a linked list of three nodes: n1 -> n2 -> n3
    struct _cmsTagLinkedList* n3 = (struct _cmsTagLinkedList*) std::calloc(1, sizeof(struct _cmsTagLinkedList));
    struct _cmsTagLinkedList* n2 = (struct _cmsTagLinkedList*) std::calloc(1, sizeof(struct _cmsTagLinkedList));
    struct _cmsTagLinkedList* n1 = (struct _cmsTagLinkedList*) std::calloc(1, sizeof(struct _cmsTagLinkedList));
    if (!n1 || !n2 || !n3) {
        std::free(n1); std::free(n2); std::free(n3);
        std::free(src);
        return false;
    }
    n1->Next = n2;
    n2->Next = n3;
    n3->Next = NULL;

    // Create head chunk with Tag pointing to the first node
    struct _cmsTagPluginChunkType* head = (struct _cmsTagPluginChunkType*) std::calloc(1, sizeof(struct _cmsTagPluginChunkType));
    if (!head) {
        std::free(n1); std::free(n2); std::free(n3); std::free(src);
        return false;
    }
    head->Tag = n1;

    // Attach to source context
    // src->chunks[TagPlugin] is used by DupTagList. The TagPlugin index is provided by the library.
    src->chunks[TagPlugin] = head;

    // Prepare destination context (dst)
    struct _cmsContext_struct* dst = (struct _cmsContext_struct*) std::calloc(1, sizeof(struct _cmsContext_struct));
    if (!dst) {
        // cleanup
        std::free(head);
        std::free(n1); std::free(n2); std::free(n3);
        std::free(src);
        return false;
    }
    // dst MemPool can be NULL for the mock; the DupTagList will call _cmsSubAllocDup
    dst->MemPool = NULL;
    dst->chunks[TagPlugin] = NULL;

    // Call the focal method
    DupTagList(dst, src);

    // Validate results
    struct _cmsTagPluginChunkType* outHead = (struct _cmsTagPluginChunkType*) dst->chunks[TagPlugin];
    TEST_ASSERT(outHead != nullptr);
    TEST_ASSERT(outHead->Tag != nullptr);

    // Traverse destination list to count nodes
    int count = 0;
    struct _cmsTagLinkedList* cur = outHead->Tag;
    struct _cmsTagLinkedList* destNodes[4] = {nullptr, nullptr, nullptr, nullptr}; // max 3 for this test
    while (cur != nullptr && count < 4) {
        destNodes[count++] = cur;
        cur = cur->Next;
    }
    TEST_ASSERT(count == 3);

    // Ensure that destination nodes are new copies (addresses differ from sources)
    struct _cmsTagLinkedList* srcNodes[3] = { n1, n2, n3 };
    for (int i = 0; i < 3; ++i) {
        TEST_ASSERT(destNodes[i] != nullptr);
        TEST_ASSERT(destNodes[i] != srcNodes[i]); // ensure deep copy
    }

    // Clean up allocated memory (best-effort; duplicates created by DupTagList are also freed by us)
    // Free dest chain
    cur = outHead->Tag;
    while (cur) {
        struct _cmsTagLinkedList* tmp = cur->Next;
        std::free(cur);
        cur = tmp;
    }
    std::free(outHead);
    std::free(dst);

    // Free source chain
    cur = head->Tag;
    while (cur) {
        struct _cmsTagLinkedList* tmp = cur->Next;
        std::free(cur);
        cur = tmp;
    }
    std::free(head);
    std::free(src);

    return true;
}

// Test 2: DupTagList handles an empty TagPlugin list gracefully.
// Precondition: src has a TagPlugin head whose Tag is NULL.
// Postcondition: dst has a TagPlugin head with a NULL Tag, and no crash occurs.
bool test_dup_taglist_empty_list() {
    // Prepare source context (src)
    struct _cmsContext_struct* src = (struct _cmsContext_struct*) std::calloc(1, sizeof(struct _cmsContext_struct));
    if (!src) return false;

    // Create head chunk with NULL Tag
    struct _cmsTagPluginChunkType* head = (struct _cmsTagPluginChunkType*) std::calloc(1, sizeof(struct _cmsTagPluginChunkType));
    if (!head) {
        std::free(src);
        return false;
    }
    head->Tag = NULL;

    // Attach to source context
    src->chunks[TagPlugin] = head;

    // Prepare destination context
    struct _cmsContext_struct* dst = (struct _cmsContext_struct*) std::calloc(1, sizeof(struct _cmsContext_struct));
    if (!dst) {
        std::free(head);
        std::free(src);
        return false;
    }
    dst->MemPool = NULL;
    dst->chunks[TagPlugin] = NULL;

    // Call the focal method
    DupTagList(dst, src);

    // Validate results
    struct _cmsTagPluginChunkType* outHead = (struct _cmsTagPluginChunkType*) dst->chunks[TagPlugin];
    TEST_ASSERT(outHead != nullptr);
    // Since the source list was empty, the destination should also have a NULL Tag
    TEST_ASSERT(outHead->Tag == NULL);

    // Clean up
    std::free(outHead);
    std::free(dst);
    std::free(head);
    std::free(src);

    return true;
}


// Main test runner
int main() {
    int passed = 0, failed = 0;

    if (test_dup_taglist_non_empty_three_nodes()) {
        std::cout << "[PASS] test_dup_taglist_non_empty_three_nodes\n";
        ++passed;
    } else {
        std::cerr << "[FAIL] test_dup_taglist_non_empty_three_nodes\n";
        ++failed;
    }

    if (test_dup_taglist_empty_list()) {
        std::cout << "[PASS] test_dup_taglist_empty_list\n";
        ++passed;
    } else {
        std::cerr << "[FAIL] test_dup_taglist_empty_list\n";
        ++failed;
    }

    std::cout << "Summary: " << passed << " passed, " << failed << " failed.\n";
    return (failed == 0) ? 0 : 1;
}