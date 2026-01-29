/*
  C++11 unit test suite for the focal function:
  DupFormatterFactoryList(struct _cmsContext_struct* ctx, const struct _cmsContext_struct* src)

  Notes:
  - This test suite is designed to run without a GTest framework.
  - It relies on the external C function DupFormatterFactoryList compiled together with the project
    (e.g., cmspack.c). The test provides minimal, compatible type definitions to exercise the function.
  - We expose necessary C-style interfaces for cross-translation-unit compatibility.
  - The tests exercise non-terminating assertions and report failures to stdout.
  - We focus on core behavior: copying a non-empty linked list, handling an empty list, and a single-element list.
  - No private methods are accessed directly; tests interact via the public function and its data structures.

  Important: The test definitions make a best-effort assumption about the layout of the involved
  LCMS-like structures to align with the focal function as provided. If the real headers differ,
  adjust the typedefs to match the exact memory layout used by the compilation unit that defines
  DupFormatterFactoryList.
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


// Domain-aligned minimal type definitions to match the focal function's expectations.
// These definitions mirror the internal layout assumed by the DupFormatterFactoryList implementation.

// Forward enum/index for chunk array position (FormattersPlugin).
// In the real code, this is part of a larger enum; here we only need the index.
enum { FormattersPlugin = 0 };

// Typedef for a factory list node used by the DupFormatterFactoryList function.
typedef struct cmsFormattersFactoryList {
    struct cmsFormattersFactoryList* Next;
    int Factory; // Used to differentiate nodes for test assertions
} cmsFormattersFactoryList;

// Typedef for the chunk that contains a FactoryList head.
// The function accesses head->FactoryList, so this struct must be layout-compatible.
typedef struct _cmsFormattersPluginChunkType {
    cmsFormattersFactoryList* FactoryList;
} _cmsFormattersPluginChunkType;

// Context structure passed to DupFormatterFactoryList.
// The function reads ctx->MemPool and writes to ctx->chunks[FormattersPlugin].
typedef struct _cmsContext_struct {
    void* MemPool;
    void* chunks[8]; // Sufficient for tests (index 0 used for FormattersPlugin)
} _cmsContext_struct;

// Function declarations from the focal file (link against the actual implementation).
extern "C" void DupFormatterFactoryList(struct _cmsContext_struct* ctx, 
                                        const struct _cmsContext_struct* src);

extern "C" void* _cmsSubAllocDup(void* MemPool, const void* src, size_t size);
extern "C" void _cmsAssert(int);

// Utility to track global test state (non-terminating assertions)
static int g_total_assertions = 0;
static int g_failed_assertions = 0;

static void expect_true(bool cond, const char* msg) {
    ++g_total_assertions;
    if (!cond) {
        ++g_failed_assertions;
        std::cout << "[ASSERT FAIL] " << msg << std::endl;
    }
}

// Helper to build a chain of cmsFormattersFactoryList nodes for tests
static cmsFormattersFactoryList* build_factory_node(int value, cmsFormattersFactoryList* next) {
    cmsFormattersFactoryList* node = (cmsFormattersFactoryList*)std::malloc(sizeof(cmsFormattersFactoryList));
    if (!node) return nullptr;
    node->Factory = value;
    node->Next = next;
    return node;
}

// Test 1: Non-empty list copying preserves order and creates independent copies.
// The destination ctx should end up with a new chunk whose FactoryList contains copies of originals
// with the same Factory values in the same order.
static void test_CopyNonEmptyList_PreservesOrder() {
    std::cout << "Running test_CopyNonEmptyList_PreservesOrder..." << std::endl;

    // Build original list: 1 -> 2 -> 3
    cmsFormattersFactoryList* n3 = build_factory_node(3, nullptr);
    cmsFormattersFactoryList* n2 = build_factory_node(2, n3);
    cmsFormattersFactoryList* n1 = build_factory_node(1, n2);

    // head chunk containing the original list
    _cmsFormattersPluginChunkType head;
    head.FactoryList = n1;

    // src context with the head in FormattersPlugin slot
    _cmsContext_struct src;
    src.MemPool = (void*)0xABCDEF;
    src.chunks[FormattersPlugin] = &head;

    // destination context to receive the duplicated list
    _cmsContext_struct dst;
    dst.MemPool = (void*)0x123450;
    dst.chunks[FormattersPlugin] = nullptr;

    // Call the focal function
    DupFormatterFactoryList(&dst, &src);

    // Validate: dst.chunks[FormattersPlugin] points to a non-null chunk with 3 elements: 1, 2, 3
    _cmsFormattersPluginChunkType* newChunk = ( _cmsFormattersPluginChunkType*) dst.chunks[FormattersPlugin];
    expect_true(newChunk != nullptr, "Destination chunk should be allocated and non-null.");

    cmsFormattersFactoryList* curr = newChunk->FactoryList;
    int expected[] = {1, 2, 3};
    int idx = 0;
    while (curr != nullptr && idx < 3) {
        std::string msg = "Element " + std::to_string(idx) + " Factory mismatch: expected " + std::to_string(expected[idx]);
        expect_true(curr->Factory == expected[idx], msg.c_str());
        curr = curr->Next;
        ++idx;
    }
    // Should have exactly 3 elements
    expect_true(curr == nullptr && idx == 3, "Copied list should contain exactly 3 elements in order.");

    // Cleanup: normally copying would allocate independent blocks; free original and copied chains if needed.
    // Free original
    std::free(n1);
    std::free(n2);
    std::free(n3);
    // Free copied chain
    cmsFormattersFactoryList* c = newChunk->FactoryList;
    while (c) {
        cmsFormattersFactoryList* toFree = c;
        c = c->Next;
        std::free(toFree);
    }
    // Free copied chunk
    std::free(newChunk);
    // Clear dst chunk reference
    dst.chunks[FormattersPlugin] = nullptr;

    std::cout << "test_CopyNonEmptyList_PreservesOrder completed." << std::endl;
}

// Test 2: Empty listCopy results in a dst chunk with a NULL FactoryList.
// This validates the no-op behavior when there is nothing to copy.
static void test_CopyEmptyListResultsInNullFactoryList() {
    std::cout << "Running test_CopyEmptyListResultsInNullFactoryList..." << std::endl;

    // Build empty head
    _cmsFormattersPluginChunkType head;
    head.FactoryList = nullptr;

    // src context with empty FactoryList
    _cmsContext_struct src;
    src.MemPool = (void*)0xDEADBEEF;
    src.chunks[FormattersPlugin] = &head;

    // dst context
    _cmsContext_struct dst;
    dst.MemPool = (void*)0xFEEDBEEF;
    dst.chunks[FormattersPlugin] = nullptr;

    // Call focal function
    DupFormatterFactoryList(&dst, &src);

    // Validate: dst chunk exists and its FactoryList is NULL
    _cmsFormattersPluginChunkType* newChunk = ( _cmsFormattersPluginChunkType*) dst.chunks[FormattersPlugin];
    expect_true(newChunk != nullptr, "Destination chunk should be allocated even for empty list.");
    expect_true(newChunk->FactoryList == nullptr, "Copied FactoryList should be NULL for empty input.");

    // Cleanup
    std::free(newChunk);
    dst.chunks[FormattersPlugin] = nullptr;

    std::cout << "test_CopyEmptyListResultsInNullFactoryList completed." << std::endl;
}

// Test 3: Single element list is copied correctly preserving the single node value.
// This ensures minimal non-empty case works as expected.
static void test_CopySingleElementList() {
    std::cout << "Running test_CopySingleElementList..." << std::endl;

    // Build single node list: 42
    cmsFormattersFactoryList* n1 = build_factory_node(42, nullptr);

    _cmsFormattersPluginChunkType head;
    head.FactoryList = n1;

    // src
    _cmsContext_struct src;
    src.MemPool = (void*)0x5555;
    src.chunks[FormattersPlugin] = &head;

    // dst
    _cmsContext_struct dst;
    dst.MemPool = (void*)0x6666;
    dst.chunks[FormattersPlugin] = nullptr;

    // Call
    DupFormatterFactoryList(&dst, &src);

    // Validate: exactly one element with Factory == 42
    _cmsFormattersPluginChunkType* newChunk = ( _cmsFormattersPluginChunkType*) dst.chunks[FormattersPlugin];
    expect_true(newChunk != nullptr, "Destination chunk should be allocated.");
    expect_true(newChunk->FactoryList != nullptr, "Copied FactoryList should not be NULL for single element.");

    cmsFormattersFactoryList* curr = newChunk->FactoryList;
    expect_true(curr->Factory == 42, "Single element Factory value should be 42.");
    curr = curr->Next;
    expect_true(curr == nullptr, "There should be exactly one element in the copied list.");

    // Cleanup
    std::free(n1);
    std::free(newChunk);
    dst.chunks[FormattersPlugin] = nullptr;

    std::cout << "test_CopySingleElementList completed." << std::endl;
}

int main() {
    // Run the tests
    test_CopyNonEmptyList_PreservesOrder();
    test_CopyEmptyListResultsInNullFactoryList();
    test_CopySingleElementList();

    // Summary
    std::cout << "==========================" << std::endl;
    if (g_failed_assertions == 0) {
        std::cout << "ALL TESTS PASSED (" << g_total_assertions << " assertions)." << std::endl;
        return 0;
    } else {
        std::cout << g_failed_assertions << " assertion(s) FAILED out of " << g_total_assertions << "." << std::endl;
        return 1;
    }
}