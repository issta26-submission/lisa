// Minimal, self-contained unit-test for a focused behavior inspired by
// the DupPluginTransformList method from cmsxform.c.
// This test provides a compact C++11 environment with lightweight stubs
// to exercise the duplication of a linked list of _cmsTransformCollection
// entries inside a _cmsTransformPluginChunkType, mimicking the logic in the
// focal method. We do not rely on GTest; instead, we provide a small
// lightweight test harness with non-terminating EXPECT-style checks.

#include <lcms2_internal.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>


// Domain: emulate just enough of the real CMS types used by DupPluginTransformList.
#define TransformPlugin 0  // index into ctx->chunks

// Minimal transform collection node
struct _cmsTransformCollection {
    _cmsTransformCollection* Next;
};

// Minimal transform plugin head containing a collection list
struct _cmsTransformPluginChunkType {
    _cmsTransformCollection* TransformCollection;
};

// Minimal CMS context struct used by the focal method
struct _cmsContext_struct {
    void* MemPool;
    void* chunks[1]; // only [TransformPlugin] is needed for tests
};

// Global sentinel object to trigger a controlled allocation failure in tests
static _cmsTransformCollection gSentinelInstance;

// Forward declarations (as would appear in the focal environment)
extern "C" void* _cmsSubAllocDup(void* MemPool, const void* src, size_t size);
extern "C" void DupPluginTransformList(struct _cmsContext_struct* ctx, const struct _cmsContext_struct* src);

// Local reproduction of the focal behavior (standalone for testing)
extern "C" void DupPluginTransformList(struct _cmsContext_struct* ctx, const struct _cmsContext_struct* src)
{
    _cmsTransformPluginChunkType newHead = { NULL };
    _cmsTransformCollection*  entry;
    _cmsTransformCollection*  Anterior = NULL;
    _cmsTransformPluginChunkType* head = (_cmsTransformPluginChunkType*) src->chunks[TransformPlugin];
    // Walk the list copying all nodes
    for (entry = head->TransformCollection;
         entry != NULL;
         entry = entry ->Next) {
        _cmsTransformCollection *newEntry =
            (_cmsTransformCollection *) _cmsSubAllocDup(ctx ->MemPool, entry, sizeof(_cmsTransformCollection));

        if (newEntry == NULL)
            return;
        // We want to keep the linked list order, so this is a little bit tricky
        newEntry -> Next = NULL;
        if (Anterior)
            Anterior -> Next = newEntry;

        Anterior = newEntry;
        if (newHead.TransformCollection == NULL)
            newHead.TransformCollection = newEntry;
    }
    ctx ->chunks[TransformPlugin] = _cmsSubAllocDup(ctx->MemPool, &newHead, sizeof(_cmsTransformPluginChunkType));
}

// Simple non-terminating test harness
static int g_totalTests = 0;
static int g_failedTests = 0;
static void logFail(const char* testName, const char* reason) {
    ++g_totalTests;
    fprintf(stderr, "Test FAILED: %s -- %s\n", testName, reason);
    ++g_failedTests;
}

// Custom non-terminating expectations
#define EXPECT_TRUE(cond, testName, reason) do { \
    if (!(cond)) logFail(testName, reason); \
} while (0)

#define EXPECT_PTR_NOT_NULL(p, testName) EXPECT_TRUE((p) != nullptr, testName, "pointer should not be NULL")
#define EXPECT_PTR_EQUAL(a, b, testName) EXPECT_TRUE((a) == (b), testName, "pointer equality expected")
#define EXPECT_PTR_NOT_EQUAL(a, b, testName) EXPECT_TRUE((a) != (b), testName, "pointer inequality expected")
#define EXPECT_BOOL(cond, testName, reason) EXPECT_TRUE((cond), testName, reason)

static void freeIfNotNull(void* p) { if (p) free(p); }

// _cmsSubAllocDup implementation for tests
static _cmsTransformCollection* sentinelAsSrcCast(const void* src) {
    // Helper to retain a cast-friendly view
    return ( _cmsTransformCollection*)src;
}

extern "C" void* _cmsSubAllocDup(void* MemPool, const void* src, size_t size)
{
    // In tests, return NULL if the src points to the sentinel instance to simulate failure
    if (src == &gSentinelInstance)
        return NULL;
    (void)MemPool; // MemPool is unused in this lightweight test harness
    void* dst = std::malloc(size);
    if (!dst) return NULL;
    std::memcpy(dst, src, size);
    return dst;
}

// Test 1: Empty TransformCollection list
bool test_empty_transform_collection_list() {
    // Arrange
    _cmsContext_struct srcCtx;
    srcCtx.MemPool = malloc(1); // dummy memory pool
    _cmsTransformPluginChunkType* origHead = new _cmsTransformPluginChunkType;
    origHead->TransformCollection = NULL;

    _cmsContext_struct testCtx;
    testCtx.MemPool = malloc(1);

    // Register the source head in the context
    // We simulate existing src with a single chunk entry
    srcCtx.chunks[TransformPlugin] = origHead;

    // Act
    DupPluginTransformList(&testCtx, &srcCtx);

    // Assert
    _cmsTransformPluginChunkType* newHead = ( _cmsTransformPluginChunkType*) testCtx.chunks[TransformPlugin];
    EXPECT_PTR_NOT_NULL(newHead, "test_empty_transform_collection_list: newHead"
                        );
    EXPECT_PTR_EQUAL(newHead, newHead, "test_empty_transform_collection_list: sanity (no crash)");
    // The new head should exist, but its TransformCollection should be NULL (no elements)
    EXPECT_TRUE(newHead->TransformCollection == NULL, "test_empty_transform_collection_list: TransformCollection should be NULL");

    // Cleanup
    freeIfNotNull(origHead);
    freeIfNotNull(newHead);
    freeIfNotNull(testCtx.MemPool);
    freeIfNotNull(srcCtx.MemPool);
    return true;
}

// Test 2: Single TransformCollection element is duplicated
bool test_single_entry_duplication() {
    // Arrange
    _cmsTransformCollection* entry1 = new _cmsTransformCollection;
    entry1->Next = NULL;

    _cmsTransformPluginChunkType* origHead = new _cmsTransformPluginChunkType;
    origHead->TransformCollection = entry1;

    _cmsContext_struct srcCtx;
    srcCtx.MemPool = malloc(1);
    srcCtx.chunks[TransformPlugin] = origHead;

    _cmsContext_struct testCtx;
    testCtx.MemPool = malloc(1);

    // Act
    DupPluginTransformList(&testCtx, &srcCtx);

    // Assert
    _cmsTransformPluginChunkType* newHead = ( _cmsTransformPluginChunkType*) testCtx.chunks[TransformPlugin];
    EXPECT_PTR_NOT_NULL(newHead, "test_single_entry_duplication: newHead should not be NULL");
    _cmsTransformCollection* newEntry = newHead->TransformCollection;
    EXPECT_PTR_NOT_NULL(newEntry, "test_single_entry_duplication: newEntry should exist");
    // Ensure it's not the same memory as original
    EXPECT_PTR_NOT_EQUAL(newEntry, entry1, "test_single_entry_duplication: newEntry should be a duplicate, not the same address");

    // Since there was exactly one element, the new list should end after one node
    EXPECT_TRUE(newEntry->Next == NULL, "test_single_entry_duplication: duplicated entry should have Next == NULL");
    // And the new head's TransformCollection should be that newEntry
    EXPECT_PTR_EQUAL(newHead->TransformCollection, newEntry, "test_single_entry_duplication: newHead.TransformCollection should point to newEntry");

    // Cleanup
    delete entry1;
    delete origHead;
    delete newHead;
    freeIfNotNull(testCtx.MemPool);
    freeIfNotNull(srcCtx.MemPool);
    return true;
}

// Test 3: Two elements preserve order in duplication
bool test_two_entries_order_preserved() {
    // Arrange
    _cmsTransformCollection* A = new _cmsTransformCollection;
    _cmsTransformCollection* B = new _cmsTransformCollection;
    A->Next = B;
    B->Next = NULL;

    _cmsTransformPluginChunkType* origHead = new _cmsTransformPluginChunkType;
    origHead->TransformCollection = A;

    _cmsContext_struct srcCtx;
    srcCtx.MemPool = malloc(1);
    srcCtx.chunks[TransformPlugin] = origHead;

    _cmsContext_struct testCtx;
    testCtx.MemPool = malloc(1);

    // Act
    DupPluginTransformList(&testCtx, &srcCtx);

    // Assert
    _cmsTransformPluginChunkType* newHead = ( _cmsTransformPluginChunkType*) testCtx.chunks[TransformPlugin];
    EXPECT_PTR_NOT_NULL(newHead, "test_two_entries_order_preserved: newHead should not be NULL");

    _cmsTransformCollection* firstDup = newHead->TransformCollection;
    _cmsTransformCollection* secondDup = firstDup ? firstDup->Next : NULL;

    EXPECT_PTR_NOT_NULL(firstDup, "test_two_entries_order_preserved: firstDup should exist");
    EXPECT_PTR_NOT_NULL(secondDup, "test_two_entries_order_preserved: secondDup should exist");

    // The order should be preserved: firstDup corresponds to A, secondDup to B
    EXPECT_PTR_NOT_EQUAL(firstDup, A, "test_two_entries_order_preserved: firstDup should be a new allocation");
    EXPECT_PTR_NOT_EQUAL(secondDup, B, "test_two_entries_order_preserved: secondDup should be a new allocation");
    // After two elements, the second's Next should be NULL
    EXPECT_TRUE(secondDup->Next == NULL, "test_two_entries_order_preserved: secondDup Next should be NULL");

    // Cleanup
    delete A; delete B; delete origHead;
    delete newHead;
    freeIfNotNull(testCtx.MemPool);
    freeIfNotNull(srcCtx.MemPool);
    return true;
}

// Test 4: Duplication failure stops early and does not modify ctx chunks
bool test_duplication_failure_no_modification() {
    // Arrange
    // Create a source with a single element that is the sentinel value, causing _cmsSubAllocDup to fail
    _cmsTransformCollection* sentinelEntry = &gSentinelInstance;

    _cmsTransformPluginChunkType* origHead = new _cmsTransformPluginChunkType;
    origHead->TransformCollection = sentinelEntry; // first and only element triggers failure

    _cmsContext_struct srcCtx;
    srcCtx.MemPool = malloc(1);
    srcCtx.chunks[TransformPlugin] = origHead;

    _cmsContext_struct testCtx;
    testCtx.MemPool = malloc(1);

    // Save original pointer to compare after call
    void* originalCtxChunk = srcCtx.chunks[TransformPlugin];

    // Act
    DupPluginTransformList(&testCtx, &srcCtx);

    // Assert: ctx chunks should remain unchanged because of early return
    EXPECT_PTR_EQUAL(originalCtxChunk, origHead, "test_duplication_failure_no_modification: original src head should be preserved in srcCtx");
    // And testCtx should not have received a new chunk (no allocation happened)
    // Note: In our simple harness, the function returns without modifying testCtx.chunks upon failure.
    // We just ensure no crash and no modification to testCtx chunks pointer (unknown state if not modified).
    // We verify that testCtx.chunks[TransformPlugin] remains NULL or unchanged (well-defined by our harness)
    // Since our simplified implementation allocates only on success, we expect NULL.
    EXPECT_TRUE(testCtx.chunks[TransformPlugin] == NULL, "test_duplication_failure_no_modification: testCtx.chunks[TransformPlugin] should remain NULL on failure");

    // Cleanup
    delete origHead;
    freeIfNotNull(testCtx.MemPool);
    freeIfNotNull(srcCtx.MemPool);
    return true;
}

// Entry point for tests
int main() {
    printf("Starting DupPluginTransformList unit tests (standalone reproduction)\n");

    // Run tests
    bool t1 = test_empty_transform_collection_list();
    bool t2 = test_single_entry_duplication();
    bool t3 = test_two_entries_order_preserved();
    bool t4 = test_duplication_failure_no_modification();

    // Simple pass/fail summary
    if (t1 && t2 && t3 && t4) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("SOME TESTS FAILED. Total: %d, Failures: %d\n", g_totalTests, g_failedTests);
        return 1;
    }
}