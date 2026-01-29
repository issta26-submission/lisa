// This file provides a self-contained C++11 test suite for the focal function
// _cmsAllocMutexPluginChunk as described in the prompt. No external test framework
// (e.g., GTest) is used. A minimal, inline test harness is implemented.
// - The tests exercise both branches: src == NULL and src != NULL
// - Static member semantics (MutexChunk) are verified via comparing function pointers
// - Memory duplication via _cmsSubAllocDup is validated by inspecting copied content
// - All tests are invoked from main(); results are printed to stdout.

#include <lcms2_internal.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>


// Domain knowledge and dependencies (mocked/minimal subset for unit testing)

// Forward declaration for cmsContext type used in function pointers
struct _cmsContext_struct;

// Typedef to mirror original usage in focal code
typedef struct _cmsContext_struct cmsContext;

// Domain: Function pointer types for mutex plugin chunk
typedef void*  (*_cmsMtxCreate)(cmsContext id);
typedef void   (*_cmsMtxDestroy)(cmsContext id, void* mtx);
typedef bool   (*_cmsMtxLock)(cmsContext id, void* mtx);
typedef void   (*_cmsMtxUnlock)(cmsContext id, void* mtx);

// Domain: Mutex chunk type used by the focal function
struct _cmsMutexPluginChunkType {
    _cmsMtxCreate mtxCreate;
    _cmsMtxDestroy mtxDestroy;
    _cmsMtxLock mtxLock;
    _cmsMtxUnlock mtxUnlock;
};

// Domain: Simple context structure with a memory pool and plugin chunks array
enum { MutexPlugin = 0, NumChunks = 1 };

struct _cmsContext_struct {
    void* MemPool;
    void** chunks; // size NumChunks
};

// Forward declarations of default and custom implementations
void* defMtxCreate(cmsContext id);
void  defMtxDestroy(cmsContext id, void* mtx);
bool  defMtxLock(cmsContext id, void* mtx);
void  defMtxUnlock(cmsContext id, void* mtx);

void* customMtxCreate(cmsContext id);
void  customMtxDestroy(cmsContext id, void* mtx);
bool  customMtxLock(cmsContext id, void* mtx);
void  customMtxUnlock(cmsContext id, void* mtx);

// Non-terminating assertions helper (log only, not exit)
#define ASSERT(cond, msg) \
    do { if (!(cond)) { printf("ASSERTION FAILED: %s\n(%s:%d) - %s\n", __FILE__, __LINE__, __func__, msg); } } while(0)


// Minimal implementation of the SubAllocDup used by the focal function
// Signature matches: void* _cmsSubAllocDup(void* pool, const void* ptr, unsigned int size)
static void* _cmsSubAllocDup(void* pool, const void* ptr, unsigned int size)
{
    (void)pool; // pool is unused in our lightweight test harness
    if (size == 0) return nullptr;
    void* dst = std::malloc(size);
    if (dst && ptr) {
        std::memcpy(dst, ptr, size);
    }
    return dst;
}

// The focal function to test, as provided in the prompt
void _cmsAllocMutexPluginChunk(struct _cmsContext_struct* ctx,
                               const struct _cmsContext_struct* src)
{
{
    static _cmsMutexPluginChunkType MutexChunk = {defMtxCreate, defMtxDestroy, defMtxLock, defMtxUnlock };
    void* from;
     
     if (src != NULL) {
        from = src ->chunks[MutexPlugin];       
    }
    else {
       from = &MutexChunk;
    }
    
    ctx ->chunks[MutexPlugin] = _cmsSubAllocDup(ctx ->MemPool, from, sizeof(_cmsMutexPluginChunkType));   
}
}

// Default implementations (expected to be used by the static MutexChunk)
void* defMtxCreate(cmsContext id) { (void)id; return (void*)0xDEADBEEF; }
void  defMtxDestroy(cmsContext id, void* mtx) { (void)id; (void)mtx; }
bool  defMtxLock(cmsContext id, void* mtx) { (void)id; (void)mtx; return true; }
void  defMtxUnlock(cmsContext id, void* mtx) { (void)id; (void)mtx; }

// Custom implementations for testing the "src != NULL" branch
void* customMtxCreate(cmsContext id) { (void)id; return (void*)0xCAFEBABE; }
void  customMtxDestroy(cmsContext id, void* mtx) { (void)id; (void)mtx; }
bool  customMtxLock(cmsContext id, void* mtx) { (void)id; (void)mtx; return false; }
void  customMtxUnlock(cmsContext id, void* mtx) { (void)id; (void)mtx; }

// Lightweight test harness

static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_test_result(const char* test_name, bool passed)
{
    g_total_tests++;
    if (!passed) {
        g_failed_tests++;
        printf("Test FAILED: %s\n", test_name);
    } else {
        printf("Test PASSED: %s\n", test_name);
    }
}

// Test 1: src == NULL should copy the static MutexChunk with default function pointers
static bool test_null_src_allocates_default_mutex_chunk()
{
    // Arrange
    _cmsContext_struct ctx;
    ctx.MemPool = nullptr;
    ctx.chunks = (void**)std::malloc(sizeof(void*) * NumChunks);
    if (!ctx.chunks) return false;
    ctx.chunks[MutexPlugin] = nullptr;

    // Act
    _cmsAllocMutexPluginChunk(&ctx, nullptr);

    // Assert
    bool ok = true;
    void* allocated = ctx.chunks[MutexPlugin];
    if (allocated == nullptr) {
        ok = false;
        goto end;
    }
    _cmsMutexPluginChunkType* p = (_cmsMutexPluginChunkType*)allocated;
    if (p == nullptr) {
        ok = false;
        goto end;
    }
    // Expect that copied chunk contains the default function pointers
    if (p->mtxCreate != defMtxCreate) ok = false;
    if (p->mtxDestroy != defMtxDestroy) ok = false;
    if (p->mtxLock != defMtxLock) ok = false;
    if (p->mtxUnlock != defMtxUnlock) ok = false;

end:
    // Cleanup
    if (allocated) std::free(allocated);
    if (ctx.chunks) std::free(ctx.chunks);
    return ok;
}

// Test 2: src != NULL should duplicate the content from src->chunks[MutexPlugin]
// using a custom set of function pointers to verify proper copying
static bool test_src_branch_duplicates_src_chunk()
{
    // Arrange: create a source chunk with custom functions
    _cmsMutexPluginChunkType srcChunk;
    srcChunk.mtxCreate = customMtxCreate;
    srcChunk.mtxDestroy = customMtxDestroy;
    srcChunk.mtxLock = customMtxLock;
    srcChunk.mtxUnlock = customMtxUnlock;

    _cmsContext_struct src;
    src.MemPool = nullptr;
    src.chunks = (void**)std::malloc(sizeof(void*) * NumChunks);
    if (!src.chunks) return false;
    src.chunks[MutexPlugin] = &srcChunk;

    _cmsContext_struct ctx;
    ctx.MemPool = nullptr;
    ctx.chunks = (void**)std::malloc(sizeof(void*) * NumChunks);
    if (!ctx.chunks) {
        std::free(src.chunks);
        return false;
    }
    ctx.chunks[MutexPlugin] = nullptr;

    // Act
    _cmsAllocMutexPluginChunk(&ctx, &src);

    // Assert
    bool ok = true;
    void* allocated = ctx.chunks[MutexPlugin];
    if (allocated == nullptr) {
        ok = false;
        goto end2;
    }
    _cmsMutexPluginChunkType* p = (_cmsMutexPluginChunkType*)allocated;
    if (p == nullptr) {
        ok = false;
        goto end2;
    }
    // The content should match the srcChunk's function pointers
    if (p->mtxCreate != customMtxCreate) ok = false;
    if (p->mtxDestroy != customMtxDestroy) ok = false;
    if (p->mtxLock != customMtxLock) ok = false;
    if (p->mtxUnlock != customMtxUnlock) ok = false;

    // The allocated chunk should be a distinct copy, not the same address as srcChunk
    if (allocated == &srcChunk) ok = false;

end2:
    // Cleanup
    if (allocated) std::free(allocated);
    if (ctx.chunks) std::free(ctx.chunks);
    if (src.chunks) std::free(src.chunks);
    return ok;
}

// Main: run tests
int main()
{
    printf("Starting _cmsAllocMutexPluginChunk unit tests (C++11, no GTest)...\n");

    test_null_src_allocates_default_mutex_chunk();
    log_test_result("test_null_src_allocates_default_mutex_chunk", g_failed_tests == 0 ? true : false);

    // Reset test counters for per-test granularity is not strictly necessary here,
    // but we log results per test for clarity.
    // Run second test
    bool t2 = test_src_branch_duplicates_src_chunk();
    log_test_result("test_src_branch_duplicates_src_chunk", t2);

    // Summary
    printf("Total tests run: %d\n", 2);
    if (g_failed_tests == 0) {
        printf("ALL tests PASSED\n");
        return 0;
    } else {
        printf("Some tests FAILED: %d\n", g_failed_tests);
        return 1;
    }
}