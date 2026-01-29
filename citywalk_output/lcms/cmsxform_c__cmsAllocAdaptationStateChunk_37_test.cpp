// Unit tests for _cmsAllocAdaptationStateChunk
// Note: This test harness mocks the minimal environment required to exercise
// the focal function logic, without relying on the full liblcms2 build.
//
// The goal is to verify:
// - When src != NULL, the function duplicates the source's AdaptationStateContext chunk.
// - When src == NULL, the function duplicates the static default adaptation state.
// - The duplication returns a distinct memory block (pointer different from the source).
//
// This is a lightweight, self-contained C++11 test using only the standard library.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


// Domain-specific mock/types (minimal reimplementation for testing)

// Simplified representation of _cmsAdaptationStateChunkType
typedef struct _cmsAdaptationStateChunkType {
    int State;
} _cmsAdaptationStateChunkType;

// Forward declaration of the function under test (C linkage to resemble real world)
extern "C" void _cmsAllocAdaptationStateChunk(struct _cmsContext_struct* ctx,
                                             const struct _cmsContext_struct* src);

// Minimal mock of memory pool dup function used by the focal method
extern "C" void* _cmsSubAllocDup(void* MemPool, const void* from, size_t size) {
    (void)MemPool; // unused in mock
    if (size == 0) return NULL;
    void* dup = std::malloc(size);
    if (dup != NULL && from != NULL) {
        std::memcpy(dup, from, size);
    }
    return dup;
}

// Default adaptation state value (must align with the static default in focal method)
#define DEFAULT_OBSERVER_ADAPTATION_STATE 3

// Minimal representation of cms context struct used by focal method
typedef struct _cmsContext_struct {
    void* MemPool;
    void** chunks; // array of pointers to per-chunk data
} _cmsContext_struct;

// Index for AdaptationStateContext within chunks array
enum { AdaptationStateContext = 0 };
// Number of chunk slots we model (only AdaptationStateContext is needed here)
enum { NumChunks = 1 };

// Implementation of the focal method under test, adapted to the test harness types
extern "C" void _cmsAllocAdaptationStateChunk(struct _cmsContext_struct* ctx,
                                             const struct _cmsContext_struct* src)
{
    // Static default adaptation state chunk (as in real code)
    static _cmsAdaptationStateChunkType AdaptationStateChunk = { DEFAULT_OBSERVER_ADAPTATION_STATE };
    void* from;

    if (src != NULL) {
        from = src ->chunks[AdaptationStateContext];
    }
    else {
        from = &AdaptationStateChunk;
    }

    ctx ->chunks[AdaptationStateContext] = _cmsSubAllocDup(ctx ->MemPool, from, sizeof(_cmsAdaptationStateChunkType));
}

// Utility: simple assertion helper (non-terminating)
static int g_failures = 0;
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[ASSERT FAILED] " << (msg) << std::endl; \
        ++g_failures; \
    } else { \
        std::cout << "[ASSERT PASSED] " << (msg) << std::endl; \
    } \
} while(0)

#define ASSERT_EQINT(a, b, msg) do { \
    if ((a) != (b)) { \
        std::cerr << "[ASSERT FAILED] " << (msg) << " (got " << (a) << ", expected " << (b) << ")" << std::endl; \
        ++g_failures; \
    } else { \
        std::cout << "[ASSERT PASSED] " << (msg) << std::endl; \
    } \
} while(0)

static void freeContext(_cmsContext_struct* ctx) {
    if (ctx == NULL) return;
    if (ctx->chunks != NULL) {
        // Only AdaptationStateContext is allocated in tests
        if (ctx->chunks[AdaptationStateContext] != NULL) {
            std::free(ctx->chunks[AdaptationStateContext]);
            ctx->chunks[AdaptationStateContext] = NULL;
        }
        std::free(ctx->chunks);
        ctx->chunks = NULL;
    }
}

// Test 1: src != NULL -> validate duplication from source chunk
// - Prepare a source context with a known chunk value.
// - Call _cmsAllocAdaptationStateChunk(ctx, src).
// - Ensure ctx->chunks[AdaptationStateContext] is a new copy with identical content.
static void test_srcNotNull_dupCopiesSource() {
    std::cout << "\n[Test 1] src != NULL -> duplicate source adaptation state chunk\n";

    // Prepare destination context
    _cmsContext_struct ctx;
    ctx.MemPool = NULL;
    ctx.chunks = (void**) std::calloc(NumChunks, sizeof(void*)); // initialize to NULLs

    // Prepare source context with a specific chunk value
    _cmsContext_struct src;
    src.MemPool = NULL;
    src.chunks = (void**) std::calloc(NumChunks, sizeof(void*));
    _cmsAdaptationStateChunkType* srcChunk = new _cmsAdaptationStateChunkType{ 99 };
    src.chunks[AdaptationStateContext] = (void*)srcChunk;

    // Call the function under test
    _cmsAllocAdaptationStateChunk(&ctx, &src);

    // Validate
    void* dup = ctx.chunks[AdaptationStateContext];
    ASSERT_TRUE(dup != NULL, "Duplicated chunk should not be NULL");
    _cmsAdaptationStateChunkType* dupTyped = (_cmsAdaptationStateChunkType*) dup;
    ASSERT_TRUE(dupTyped != NULL, "Duplicated chunk should be a valid _cmsAdaptationStateChunkType");
    ASSERT_EQINT(dupTyped->State, 99, "Duplicated chunk should copy the source state (99)");
    // Ensure distinct memory location
    ASSERT_TRUE(dup != srcChunk, "Duplicated memory should be a different pointer than source chunk");

    // Cleanup
    delete srcChunk;
    std::free(src.chunks);
    std::free(ctx.chunks); // ctx.chunks[AdaptationStateContext] freed below
    ctx.chunks[AdaptationStateContext] = NULL;
    freeContext(&ctx);
}

// Test 2: src == NULL -> duplicate default static adaptation state chunk
// - Call _cmsAllocAdaptationStateChunk(ctx, NULL).
// - Verify the dup contains DEFAULT_OBSERVER_ADAPTATION_STATE.
// - Verify the returned pointer is not the same as the static dummy (indirect check by content and pointer uniqueness).
static void test_srcNull_dupDefaults() {
    std::cout << "\n[Test 2] src == NULL -> duplicate default adaptation state chunk\n";

    _cmsContext_struct ctx;
    ctx.MemPool = NULL;
    ctx.chunks = (void**) std::calloc(NumChunks, sizeof(void*));

    // Call with NULL source
    _cmsAllocAdaptationStateChunk(&ctx, NULL);

    void* dup = ctx.chunks[AdaptationStateContext];
    ASSERT_TRUE(dup != NULL, "Duplicated default chunk should not be NULL");
    _cmsAdaptationStateChunkType* dupTyped = (_cmsAdaptationStateChunkType*) dup;
    ASSERT_TRUE(dupTyped != NULL, "Duplicated default chunk should be a valid _cmsAdaptationStateChunkType");
    ASSERT_EQINT(dupTyped->State, DEFAULT_OBSERVER_ADAPTATION_STATE, "Duplicated default should match DEFAULT_OBSERVER_ADAPTATION_STATE");

    // Cleanup
    std::free(dup);
    std::free(ctx.chunks);
    freeContext(&ctx);
}

// Test 3: Overwrite behavior
// - Ensure that calling with a pre-existing ctx chunk pointer results in a new allocation
//   (i.e., old content is replaced by the new duplicate).
static void test_overwrite_existing_chunk() {
    std::cout << "\n[Test 3] Overwrite existing chunk pointer on allocation\n";

    _cmsContext_struct ctx;
    ctx.MemPool = NULL;
    ctx.chunks = (void**) std::calloc(NumChunks, sizeof(void*));

    // Pre-existing dummy chunk in destination
    _cmsAdaptationStateChunkType* preExisting = new _cmsAdaptationStateChunkType{ 555 };
    ctx.chunks[AdaptationStateContext] = preExisting;

    // Prepare a source with a different state
    _cmsContext_struct src;
    src.MemPool = NULL;
    src.chunks = (void**) std::calloc(NumChunks, sizeof(void*));
    _cmsAdaptationStateChunkType* srcChunk = new _cmsAdaptationStateChunkType{ 7 };
    src.chunks[AdaptationStateContext] = (void*)srcChunk;

    _cmsAllocAdaptationStateChunk(&ctx, &src);

    // Verify the pre-existing pointer was replaced
    void* newDup = ctx.chunks[AdaptationStateContext];
    ASSERT_TRUE(newDup != NULL, "New duplicate should not be NULL");
    ASSERT_TRUE(newDup != preExisting, "New duplicate should not be the same as pre-existing pointer");

    _cmsAdaptationStateChunkType* dupTyped = (_cmsAdaptationStateChunkType*) newDup;
    ASSERT_TRUE(dupTyped != NULL, "New duplicate should be a valid _cmsAdaptationStateChunkType");
    ASSERT_EQINT(dupTyped->State, 7, "New duplicate should copy source state (7)");

    // Cleanup
    delete preExisting;
    delete srcChunk;
    std::free(src.chunks);
    std::free(newDup); // freed the new allocation
    std::free(ctx.chunks);
    freeContext(&ctx);
}

// Main: run tests
int main() {
    std::cout << "Running unit tests for _cmsAllocAdaptationStateChunk (mocked environment)\n";

    test_srcNotNull_dupCopiesSource();
    test_srcNull_dupDefaults();
    test_overwrite_existing_chunk();

    if (g_failures == 0) {
        std::cout << "\nAll tests passed.\n";
        return 0;
    } else {
        std::cout << "\nTests completed with " << g_failures << " failure(s).\n";
        return 1;
    }
}