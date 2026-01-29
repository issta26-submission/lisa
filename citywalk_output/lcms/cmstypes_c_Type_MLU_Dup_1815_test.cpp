// Test suite for the focal method: Type_MLU_Dup in cmstypes.c
// This test suite is designed for C++11 compilation without Google Test.
// It provides a minimal, non-terminating test harness with simple EXPECT-like macros.
// It mocks the cmsMLUdup function to verify correct duplication behavior of Type_MLU_Dup.
// Important: The actual production code cmstypes.c is assumed to be compiled and linked together.

// Step 1 (highlights)
 // Candidate Keywords representing core components in Type_MLU_Dup:
 // - cmsMLUdup: duplicator function for cmsMLU structures (mocked here)
 // - cmsMLU: MLU (multilingual user text) structure
 // - _cms_typehandler_struct: type handler context parameter (unused in logic)
 // - Ptr: pointer to cmsMLU to be duplicated
 // - n: size/count parameter (unused in logic)

#include <lcms2_internal.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>


// Forward declarations to match the focal function's signature (as used in cmstypes.c)
typedef unsigned int cmsUInt32Number;

struct _cms_typehandler_struct; // opaque in the focal method

// The focal function is declared with C linkage in the original C file.
// We declare it with extern "C" to ensure correct linkage when compiling with C++.
extern "C" void* Type_MLU_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n);

// Minimal translation unit declarations for the test harness:

// Mocked cmsMLU structure (as expected by the focal method's usage)
extern "C" struct cmsMLU {
    int a;
    int b;
    char text[16];
};

// Mocked cmsMLUdup to be used by Type_MLU_Dup
extern "C" cmsMLU* cmsMLUdup(const cmsMLU* orig);

// Provide a simple, predictable implementation of cmsMLUdup for testing
extern "C" cmsMLU* cmsMLUdup(const cmsMLU* orig) {
    if (orig == nullptr) return nullptr;
    cmsMLU* dup = new cmsMLU(*orig); // shallow copy suffices for test; contents are simple POD
    return dup;
}

// Provide a small, non-intrusive implementation of the actual Type_MLU_Dup
// This is not the implementation under test; it exists to allow linking in environments
// where cmstypes.c is compiled separately. The real Type_MLU_Dup is provided by cmstypes.c.
 // Note: The real function is in cmstypes.c; we only declare it here with C linkage for linking.
 // The test will link against the existing cmstypes.c implementation.
struct _cms_typehandler_struct;

extern "C" void* Type_MLU_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n);

// Simple test harness (non-terminating assertions)
static int g_failures = 0;

#define EXPECT_EQ_INT(actual, expected) \
    do { if ((int)(actual) != (int)(expected)) { \
        printf("EXPECT_EQ_INT failed: actual=%d, expected=%d at %s:%d\n", (int)(actual), (int)(expected), __FILE__, __LINE__); \
        ++g_failures; \
    } } while (0)

#define EXPECT_NE_PTR(actual, expected) \
    do { if ((actual) == (expected)) { \
        printf("EXPECT_NE_PTR failed: actual=%p, expected != %p at %s:%d\n", (void*)(actual), (void*)(expected), __FILE__, __LINE__); \
        ++g_failures; \
    } } while (0)

#define EXPECT_TRUE(cond) \
    do { if (!(cond)) { \
        printf("EXPECT_TRUE failed: condition is false at %s:%d\n", __FILE__, __LINE__); \
        ++g_failures; \
    } } while (0)

#define EXPECT_NONNULL(ptr) \
    do { if ((ptr) == nullptr) { \
        printf("EXPECT_NONNULL failed: pointer is null at %s:%d\n", __FILE__, __LINE__); \
        ++g_failures; \
    } } while (0)

// Test 1: Basic duplication correctness and independence
// - Create a cmsMLU object with known values
// - Call Type_MLU_Dup with the object as Ptr
// - Verify the returned pointer is non-null, a separate copy, and has identical content
// - Modify the original after duplication and ensure the duplicate remains unchanged
void test_Type_MLU_Dup_basic_duplication_content_independence() {
    printf("Running test_Type_MLU_Dup_basic_duplication_content_independence...\n");

    // Prepare original cmsMLU content
    cmsMLU orig;
    orig.a = 42;
    orig.b = -7;
    std::memset(orig.text, 0, sizeof(orig.text));
    std::strcpy(orig.text, "HELLO_MLU");

    // Self parameter (not used by the function logic, but provided for signature completeness)
    _cms_typehandler_struct dummySelf;

    // Call the focal function Type_MLU_Dup
    void* dupVoid = Type_MLU_Dup(&dummySelf, &orig, 12345);

    // Basic sanity checks
    EXPECT_NONNULL(dupVoid);
    cmsMLU* dup = (cmsMLU*)dupVoid;
    EXPECT_NE_PTR(dup, &orig);

    // Content equality checks
    EXPECT_EQ_INT(dup->a, orig.a);
    EXPECT_EQ_INT(dup->b, orig.b);
    EXPECT_TRUE(std::strcmp(dup->text, orig.text) == 0);

    // Independence check: modify original and ensure duplicate remains unchanged
    orig.a = 999;
    orig.b = 999;
    std::strcpy(orig.text, "CHANGED");
    EXPECT_EQ_INT(dup->a, 42);
    EXPECT_EQ_INT(dup->b, -7);
    EXPECT_TRUE(std::strcmp(dup->text, "HELLO_MLU") == 0);

    // Cleanup
    delete dup;

    printf("test_Type_MLU_Dup_basic_duplication_content_independence completed.\n");
}

// Test 2: Ensure that non-zero n and a non-null self do not affect the duplication behavior
// - Provide non-default n and a dummy self structure
// - Ensure the function still returns a valid distinct copy with identical content
void test_Type_MLU_Dup_with_nonzero_n_and_self() {
    printf("Running test_Type_MLU_Dup_with_nonzero_n_and_self...\n");

    cmsMLU orig;
    orig.a = 7;
    orig.b = 14;
    std::memset(orig.text, 0, sizeof(orig.text));
    std::strcpy(orig.text, "TEST2");

    _cms_typehandler_struct dummySelf;
    dummySelf = *static_cast<_cms_typehandler_struct*>(nullptr); // intentionally unused; just to simulate non-null

    void* dupVoid = Type_MLU_Dup(&dummySelf, &orig, 999);

    EXPECT_NONNULL(dupVoid);
    cmsMLU* dup = (cmsMLU*)dupVoid;
    EXPECT_NE_PTR(dup, &orig);

    EXPECT_EQ_INT(dup->a, orig.a);
    EXPECT_EQ_INT(dup->b, orig.b);
    EXPECT_TRUE(std::strcmp(dup->text, orig.text) == 0);

    // Cleanup
    delete dup;

    printf("test_Type_MLU_Dup_with_nonzero_n_and_self completed.\n");
}

// Test 3: Validate that multiple duplications produce independent copies
// - Duplicate twice and ensure both copies retain original content independently
void test_Type_MLU_Dup_multiple_duplications_are_independent() {
    printf("Running test_Type_MLU_Dup_multiple_duplications_are_independent...\n");

    cmsMLU orig;
    orig.a = -1;
    orig.b = -2;
    std::strcpy(orig.text, "MULTI");

    _cms_typehandler_struct dummySelf;

    // First duplication
    void* dupVoid1 = Type_MLU_Dup(&dummySelf, &orig, 0);
    EXPECT_NONNULL(dupVoid1);
    cmsMLU* dup1 = (cmsMLU*)dupVoid1;

    // Second duplication
    void* dupVoid2 = Type_MLU_Dup(&dummySelf, &orig, 0);
    EXPECT_NONNULL(dupVoid2);
    cmsMLU* dup2 = (cmsMLU*)dupVoid2;

    // Both copies should be independent and equal to original at creation time
    EXPECT_NE_PTR(dup1, dup2);
    EXPECT_TRUE(std::strcmp(dup1->text, orig.text) == 0);
    EXPECT_TRUE(std::strcmp(dup2->text, orig.text) == 0);

    // Modify original and ensure both duplicates reflect the original content (should stay identical to original at time of copy)
    orig.a = 123;
    orig.b = 456;
    std::strcpy(orig.text, "CHG");
    EXPECT_EQ_INT(dup1->a, -1);
    EXPECT_EQ_INT(dup1->b, -2);
    EXPECT_TRUE(std::strcmp(dup1->text, "MULTI") == 0);
    EXPECT_EQ_INT(dup2->a, -1);
    EXPECT_EQ_INT(dup2->b, -2);
    EXPECT_TRUE(std::strcmp(dup2->text, "MULTI") == 0);

    // Cleanup
    delete dup1;
    delete dup2;

    printf("test_Type_MLU_Dup_multiple_duplications_are_independent completed.\n");
}

// Run all tests
void run_all_tests() {
    test_Type_MLU_Dup_basic_duplication_content_independence();
    test_Type_MLU_Dup_with_nonzero_n_and_self();
    test_Type_MLU_Dup_multiple_duplications_are_independent();

    if (g_failures > 0) {
        printf("Tests completed with %d failure(s).\n", g_failures);
    } else {
        printf("All tests passed.\n");
    }
}

// Main entry point
int main() {
    run_all_tests();
    // Return non-zero if any test failed
    return g_failures ? 1 : 0;
}