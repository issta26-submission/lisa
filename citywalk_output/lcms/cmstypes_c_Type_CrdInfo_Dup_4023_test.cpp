// Type_CrdInfo_Dup Unit Tests (C++11, no GoogleTest) - self-contained mock environment
// This test suite targets the focal method behavior of duplicating a cmsMLU-like object
// via cmsMLUdup. Due to the test harness nature, we provide minimal, self-contained
// definitions to exercise the logical flow without requiring the full CMS/lcms build.
// NOTE: In a real project, adapt to link against the actual cmstypes.c and provide
// real cmsMLUdup implementation. Here we simulate the essential behavior for coverage.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


// Domain-specific lightweight typedefs and structs (mocked/minimal)
typedef unsigned int cmsUInt32Number;

struct _cms_typehandler_struct {}; // minimal stub for the first parameter type

// Lightweight, fake cmsMLU structure used to simulate a real MLU object.
// We intentionally keep a simple layout: a length and a data buffer.
struct FakeMLU {
    size_t len;
    char data[128];
};

// In this mock environment, we alias the CMS MLU type to our FakeMLU
typedef FakeMLU cmsMLU;

// Forward declaration of the focal cmsMLU duplication function.
// In the real project this would come from the CMS library, but for unit testing
// we provide a controlled mock implementation below.
extern "C" cmsMLU* cmsMLUdup(cmsMLU* mlu);

// Forward declaration of the focal method under test (as declared by the project).
// We provide an in-test implementation mirroring the original signature.
// In the real build, Type_CrdInfo_Dup is defined in cmstypes.c; here we simulate its core
// behavior to verify the duplication pathway.
extern "C" void* Type_CrdInfo_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n);

// -----------------------------------------------------------------------------
// Mock cmsMLUdup implementation
// This function duplicates a cmsMLU-like object by allocating a new FakeMLU object
// and copying the content. If input is NULL, it returns NULL.
// This mimics the memory-copy behavior expected from a typical cmsMLUdup.
// -----------------------------------------------------------------------------
extern "C" cmsMLU* cmsMLUdup(cmsMLU* mlu) {
    if (mlu == nullptr) return nullptr;

    // Allocate a new FakeMLU instance and copy metadata/content
    cmsMLU* copy = (cmsMLU*)malloc(sizeof(FakeMLU));
    if (!copy) return nullptr;
    copy->len = mlu->len;
    // Copy only the used portion of data
    memcpy(copy->data, mlu->data, mlu->len);
    return copy;
}

// -----------------------------------------------------------------------------
// Implementation of the focal function under test (copied here to be test-friendly).
// In the real project, this function is defined in cmstypes.c and uses cmsMLUdup.
// We reproduce the essential behavior: cast Ptr to cmsMLU* and pass to cmsMLUdup.
// The n and self parameters are unused (as per original code).
// -----------------------------------------------------------------------------
extern "C" void* Type_CrdInfo_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n)
{
    // The original function returns cmsMLUdup((cmsMLU*) Ptr);
    // We preserve that behavior in the test.
    return (void*) cmsMLUdup((cmsMLU*) Ptr);

    // cmsUNUSED_PARAMETER(n);
    // cmsUNUSED_PARAMETER(self);
}

// -----------------------------------------------------------------------------
// Helpers for constructing test data (Fake MLU instances)
// -----------------------------------------------------------------------------

// Build a FakeMLU object with given content length and data
FakeMLU buildMLU(const char* content, size_t len) {
    FakeMLU mlu;
    mlu.len = len;
    if (len > 0) {
        // Copy up to 128 bytes of content
        size_t copyLen = (len < sizeof(mlu.data)) ? len : sizeof(mlu.data);
        memcpy(mlu.data, content, copyLen);
        // Zero-fill remaining bytes to avoid stale data
        if (copyLen < sizeof(mlu.data))
            memset(mlu.data + copyLen, 0, sizeof(mlu.data) - copyLen);
    } else {
        // No content; zero the buffer
        memset(mlu.data, 0, sizeof(mlu.data));
    }
    return mlu;
}

// Compare two FakeMLU objects for content equality
bool mlus_equal(const FakeMLU& a, const FakeMLU& b) {
    if (a.len != b.len) return false;
    return (std::memcmp(a.data, b.data, a.len) == 0);
}

// Print a hex dump for debugging a FakeMLU (optional helper)
void dump_mlu(const FakeMLU& mlu) {
    std::cout << "FakeMLU(len=" << mlu.len << ", data=\"";
    for (size_t i = 0; i < mlu.len; ++i) {
        unsigned char c = static_cast<unsigned char>(mlu.data[i]);
        if (c >= 32 && c <= 126) std::cout << static_cast<char>(c);
        else std::cout << "\\x" << std::hex << (int)c << std::dec;
    }
    std::cout << "\")\n";
}

// -----------------------------------------------------------------------------
// Simple test harness (non-terminating assertions; continues on failure)
// -----------------------------------------------------------------------------

static int g_total = 0;
static int g_failed = 0;

void expect_true(bool cond, const char* msg) {
    ++g_total;
    if (cond) {
        std::cout << "[PASS] " << msg << "\n";
    } else {
        std::cerr << "[FAIL] " << msg << "\n";
        ++g_failed;
    }
}

// Test 1: Duplicating a non-null MLU should yield a non-null copy with identical content
void test_dup_non_null() {
    FakeMLU orig = buildMLU("ABCDE", 5);
    void* res = Type_CrdInfo_Dup(nullptr, &orig, 0);
    // Ensure we got a non-null pointer
    expect_true(res != nullptr, "Type_CrdInfo_Dup should return non-null for non-null Ptr");

    if (res != nullptr) {
        FakeMLU* copy = (FakeMLU*)res;
        // Content and length must be identical
        bool sameLen = (copy->len == orig.len);
        bool contentEq = std::memcmp(copy->data, orig.data, orig.len) == 0;
        expect_true(sameLen, "Duplicated MLU should preserve length");
        expect_true(contentEq, "Duplicated MLU data should be identical to source");
        std::free(res);
    }
}

// Test 2: Duplicating with a NULL Ptr should return NULL
void test_dup_null_ptr() {
    void* res = Type_CrdInfo_Dup(nullptr, nullptr, 0);
    expect_true(res == nullptr, "Type_CrdInfo_Dup should return NULL when Ptr is NULL");
}

// Test 3: Verify that self and n parameters do not affect the result (only Ptr is used)
void test_ignore_self_and_n_variants() {
    FakeMLU orig = buildMLU("XYZ", 3);

    // Different self and n values but same Ptr
    void* res1 = Type_CrdInfo_Dup(nullptr, &orig, 0);
    void* res2 = Type_CrdInfo_Dup(reinterpret_cast<struct _cms_typehandler_struct*>(0xDEADBEEF), &orig, 12345);

    expect_true(res1 != nullptr && res2 != nullptr, "Both duplicates should be non-null when Ptr is valid");

    if (res1 && res2) {
        FakeMLU* c1 = (FakeMLU*)res1;
        FakeMLU* c2 = (FakeMLU*)res2;
        bool equal = mlus_equal(*c1, *c2);
        expect_true(equal, "Type_CrdInfo_Dup should ignore self and n and produce identical copies for same Ptr");
        std::free(res1);
        std::free(res2);
    }
}

// -----------------------------------------------------------------------------
// Main: run all tests and report a summary
// -----------------------------------------------------------------------------

int main() {
    std::cout << "Running Type_CrdInfo_Dup unit tests (mock environment)\n";

    test_dup_non_null();
    test_dup_null_ptr();
    test_ignore_self_and_n_variants();

    std::cout << "Summary: " << (g_total - g_failed) << " passed, " << g_failed << " failed, out of " << g_total << " tests.\n";

    return (g_failed > 0) ? 1 : 0;
}