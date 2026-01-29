// Test suite for Type_UcrBg_Dup in cmstypes.c
// This test uses a lightweight, self-contained harness (no GTest) to validate
// the behavior of the Type_UcrBg_Dup function, using stubs for dependent
// functions (_cmsMallocZero, cmsDupToneCurve, cmsMLUdup) to avoid requiring
// the full LCMS2 runtime. It compiles under C++11.

#include <lcms2_internal.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>


// Include the LCMS internal headers to obtain type definitions.
// This assumes the testing environment provides these headers.
// The test focuses on Type_UcrBg_Dup behavior and uses real type names.
extern "C" {
}

// Forward declaration of focal function (C linkage)
extern "C" void* Type_UcrBg_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n);

// Test harness globals to observe allocation behavior
static int g_alloc_calls = 0;
static void* g_last_alloc_context = nullptr;
static cmsUInt32Number g_last_alloc_size = 0;
static bool g_fail_alloc = false;

// Stubbed implementations to intercept dependent calls in the focal method.
// These are provided in place of the real library implementations for isolated testing.

extern "C" {
    // Simple allocator that respects a controlled failure flag and records calls.
    void* _cmsMallocZero(cmsContext ContextID, cmsUInt32Number size) {
        g_alloc_calls++;
        g_last_alloc_context = (void*)ContextID;
        g_last_alloc_size = size;
        if (g_fail_alloc) {
            return NULL;
        }
        // Allocate and zero the memory to mimic allocator behavior.
        void* p = calloc(1, (size_t)size);
        return p;
    }

    // Fake duplication for a tone curve: shallow copy of the memory block.
    cmsToneCurve* cmsDupToneCurve(const cmsToneCurve* src) {
        if (src == NULL) return NULL;
        cmsToneCurve* dst = (cmsToneCurve*) malloc(sizeof(cmsToneCurve));
        if (dst == NULL) return NULL;
        // Copy the entire structure; in real world, this would duplicate internals.
        memcpy(dst, src, sizeof(cmsToneCurve));
        return dst;
    }

    // Fake duplication for a MLU (multilingual string) structure.
    cmsMLU* cmsMLUdup(const cmsMLU* src) {
        if (src == NULL) return NULL;
        cmsMLU* dst = (cmsMLU*) malloc(sizeof(cmsMLU));
        if (dst == NULL) return NULL;
        memcpy(dst, src, sizeof(cmsMLU));
        return dst;
    }
}

// Lightweight test harness helpers
static void test_begin(const char* name) {
    printf("BEGIN TEST: %s\n", name);
}

static void test_end(bool passed) {
    if (passed) {
        printf("END TEST: PASSED\n\n");
    } else {
        printf("END TEST: FAILED\n\n");
    }
}

// Convenience assertions (non-terminating)
#define EXPECT_TRUE(x) do { if(!(x)) { \
    fprintf(stderr, "EXPECT_TRUE failed: %s:%d\n", __FILE__, __LINE__); g_failures++; } } while(0)
#define EXPECT_EQ(a,b) do { if((a) != (b)) { \
    fprintf(stderr, "EXPECT_EQ failed: (%p) != (%p) at %s:%d\n", (void*)(a), (void*)(b), __FILE__, __LINE__); g_failures++; } } while(0)
#define EXPECT_NE(a,b) do { if((a) == (b)) { \
    fprintf(stderr, "EXPECT_NE failed: (%p) == (%p) at %s:%d\n", (void*)(a), (void*)(b), __FILE__, __LINE__); g_failures++; } } while(0)
static int g_failures = 0;

// Test 1: Success path - all components are duplicated and pointers are distinct
void test_Type_UcrBg_Dup_Success() {
    test_begin("Type_UcrBg_Dup_Success");

    // Prepare a fake self with a ContextID
    struct _cms_typehandler_struct self;
    void* test_ctx = (void*)0xDEADBEEF;
    self.ContextID = (cmsContext)test_ctx;

    // Create source cmsUcrBg with non-NULL components
    cmsToneCurve* BgCurve = (cmsToneCurve*) malloc(sizeof(cmsToneCurve));
    cmsToneCurve* UcrCurve = (cmsToneCurve*) malloc(sizeof(cmsToneCurve));
    cmsMLU* Desc = (cmsMLU*) malloc(sizeof(cmsMLU));

    cmsUcrBg Src;
    Src.Bg = BgCurve;
    Src.Ucr = UcrCurve;
    Src.Desc = Desc;

    // Call focal function
    void* dupPtr = Type_UcrBg_Dup(&self, &Src, 0);

    // Assertions
    EXPECT_TRUE(dupPtr != NULL);
    cmsUcrBg* New = (cmsUcrBg*) dupPtr;
    EXPECT_NE(New, &Src);                 // Different objects
    EXPECT_TRUE(New->Bg != Src.Bg);       // Tone curves duplicated (pointers differ)
    EXPECT_TRUE(New->Ucr != Src.Ucr);     // Tone curves duplicated
    EXPECT_TRUE(New->Desc != Src.Desc);   // MLU duplicated

    // Verify allocator was used and context id passed through
    EXPECT_EQ(g_last_alloc_context, test_ctx);
    EXPECT_TRUE(g_alloc_calls == 1);
    // Note: Memory leaks acceptable for this focused unit test; in a real test env we would free.

    test_end(true);
}

// Test 2: Allocation failure propagation - _cmsMallocZero returns NULL
void test_Type_UcrBg_Dup_AllocFailure() {
    test_begin("Type_UcrBg_Dup_AllocFailure");

    // Prepare a fake self
    struct _cms_typehandler_struct self;
    void* test_ctx = (void*)0xCAFEBABE;
    self.ContextID = (cmsContext)test_ctx;

    // Create valid Src
    cmsToneCurve* BgCurve = (cmsToneCurve*) malloc(sizeof(cmsToneCurve));
    cmsToneCurve* UcrCurve = (cmsToneCurve*) malloc(sizeof(cmsToneCurve));
    cmsMLU* Desc = (cmsMLU*) malloc(sizeof(cmsMLU));

    cmsUcrBg Src;
    Src.Bg = BgCurve;
    Src.Ucr = UcrCurve;
    Src.Desc = Desc;

    // Enable allocation failure
    g_fail_alloc = true;
    g_alloc_calls = 0;
    g_last_alloc_context = nullptr;

    void* dupPtr = Type_UcrBg_Dup(&self, &Src, 0);

    // Should return NULL on allocation failure
    EXPECT_TRUE(dupPtr == NULL);
    // Ensure that allocation function was invoked (even if it failed)
    EXPECT_TRUE(g_alloc_calls >= 1);

    // Reset flag for other tests
    g_fail_alloc = false;

    test_end(true);
}

// Test 3: NULL Desc in source - ensure function handles NULL Desc gracefully
void test_Type_UcrBg_Dup_DescNull() {
    test_begin("Type_UcrBg_Dup_DescNull");

    struct _cms_typehandler_struct self;
    void* test_ctx = (void*)0x12345678;
    self.ContextID = (cmsContext)test_ctx;

    cmsToneCurve* BgCurve = (cmsToneCurve*) malloc(sizeof(cmsToneCurve));
    cmsToneCurve* UcrCurve = (cmsToneCurve*) malloc(sizeof(cmsToneCurve));

    cmsUcrBg Src;
    Src.Bg = BgCurve;
    Src.Ucr = UcrCurve;
    Src.Desc = NULL; // NULL Desc

    void* dupPtr = Type_UcrBg_Dup(&self, &Src, 0);

    EXPECT_TRUE(dupPtr != NULL);
    cmsUcrBg* New = (cmsUcrBg*) dupPtr;
    EXPECT_NE(New, &Src);
    EXPECT_TRUE(New->Bg != Src.Bg);
    EXPECT_TRUE(New->Ucr != Src.Ucr);
    EXPECT_TRUE(New->Desc == NULL); // Should remain NULL if source is NULL

    // Also verify allocator received the proper context
    EXPECT_EQ(g_last_alloc_context, test_ctx);
    test_end(true);
}

// Main entry: run all tests
int main() {
    test_Type_UcrBg_Dup_Success();
    test_Type_UcrBg_Dup_AllocFailure();
    test_Type_UcrBg_Dup_DescNull();

    if (g_failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("TOTAL FAILED TESTS: %d\n", g_failures);
        return 1;
    }
}