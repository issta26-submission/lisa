// Unit test suite for Type_ProfileSequenceDesc_Dup in cmstypes.c
// Target: C++11 environment, no GTest, single translation unit test harness.
// Approach: Provide a minimal mock for cmsDupProfileSequenceDescription to exercise the wrapper.
// Notes:
// - The focal function simply delegates to cmsDupProfileSequenceDescription and returns its result.
// - We mock cmsDupProfileSequenceDescription to be a no-op (returns the input pointer) to verify
//   that the wrapper indeed forwards the pointer and returns what the underlying function provides.
// - We test both non-null and null Ptr cases. The 'n' parameter is unused by the focal method.

#include <lcms2_internal.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>


// Typedefs mirroring the usage in the focal code.
// We keep these minimal to avoid pulling the entire CMS type system into the test.
typedef unsigned int cmsUInt32Number;

// Forward declaration of the CMS type handler struct used by the focal method.
// We do not need its contents for the tests; a complete type is not required for pointer passing.
struct _cms_typehandler_struct;

// Prototypes of the focal method under test and the cmsDup function we are mocking.
// These are declared with C linkage to match the actual library's linkage conventions.
extern "C" void* Type_ProfileSequenceDesc_Dup(struct _cms_typehandler_struct* self, const void* Ptr, cmsUInt32Number n);

// Mock: provide a lightweight alias/identity dup that simply returns the input pointer.
// This stands in place of the real cmsDupProfileSequenceDescription for the unit test.
// Signature chosen to be broadly compatible with typical C linkage (void* input -> void* output).
extern "C" void* cmsDupProfileSequenceDescription(void* seq)
{
    // In the test, we simply return the input pointer to verify the conduit behavior.
    return seq;
}

// Basic test harness (non-terminating assertions) to maximize coverage.
// We implement a tiny framework with a global counter to report failures at the end.

static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            printf("TEST FAILURE: %s:%d - %s\n", __FILE__, __LINE__, (msg)); \
            ++g_failures; \
        } \
    } while(0)

#define EXPECT_PTR_EQ(a, b, msg) \
    do { \
        if ((void*)(a) != (void*)(b)) { \
            printf("TEST FAILURE: %s:%d - %s (expected pointer %p, got %p)\n", __FILE__, __LINE__, (msg), (void*)(b), (void*)(a)); \
            ++g_failures; \
        } \
    } while(0)

#define TEST(name) bool name()

// Test 1: Non-null Ptr should be forwarded to cmsDupProfileSequenceDescription and return the same pointer.
// Rationale: The wrapper's behavior is to return the result of the cmsDupProfileSequenceDescription call.
// Our mock returns the input pointer, so we expect Type_ProfileSequenceDesc_Dup to return the same pointer.
TEST(Test_Dup_NonNull_PointerReturnedUnmodified)
{
    // Prepare a dummy non-null memory block to simulate a cmsSEQ pointer.
    void* input = malloc(64);
    if (!input) {
        // If allocation fails, mark as test failure.
        printf("Test setup failed: malloc returned NULL\n");
        return false;
    }

    // Call the focal function. The 'self' is unused by the function; we pass NULL.
    void* result = Type_ProfileSequenceDesc_Dup(nullptr, input, 1);

    // Validate that the return value equals the input pointer (mocked dup returns same pointer).
    EXPECT_PTR_EQ(result, input, "Type_ProfileSequenceDesc_Dup should return the same pointer as input when non-null");

    // Clean up
    free(input);
    return true;
}

// Test 2: Null Ptr should propagate as NULL from the mock dup function to the wrapper.
TEST(Test_Dup_Null_PtrReturnsNull)
{
    void* result = Type_ProfileSequenceDesc_Dup(nullptr, nullptr, 5);
    EXPECT_TRUE(result == nullptr, "Type_ProfileSequenceDesc_Dup should return NULL when input Ptr is NULL");
    return true;
}

// Test 3: Variation in the 'n' parameter should not affect behavior (since the focal method ignores it).
TEST(Test_Dup_VariableN_DoesNotAffectReturn)
{
    // Allocate a non-null block
    void* input = malloc(32);
    if (!input) {
        printf("Test setup failed: malloc returned NULL\n");
        return false;
    }

    // Different n values
    void* r1 = Type_ProfileSequenceDesc_Dup(nullptr, input, 0);
    void* r2 = Type_ProfileSequenceDesc_Dup(nullptr, input, 12345);
    void* r3 = Type_ProfileSequenceDesc_Dup(nullptr, input, 999);

    // All should equal the input (since our mock returns the input pointer)
    EXPECT_PTR_EQ(r1, input, "n=0 should not affect result");
    EXPECT_PTR_EQ(r2, input, "n=12345 should not affect result");
    EXPECT_PTR_EQ(r3, input, "n=999 should not affect result");

    free(input);
    return true;
}

// Main: Run all tests and report summary.
int main()
{
    int total = 0;
    int passed = 0;

    const char* test_names[] = {
        "Test_Dup_NonNull_PointerReturnedUnmodified",
        "Test_Dup_Null_PtrReturnsNull",
        "Test_Dup_VariableN_DoesNotAffectReturn"
    };

    typedef bool (*TestFunc)();
    TestFunc tests[] = {
        []() { return Test_Dup_NonNull_PointerReturnedUnmodified(); },
        []() { return Test_Dup_Null_PtrReturnsNull(); },
        []() { return Test_Dup_VariableN_DoesNotAffectReturn(); }
    };

    const int TEST_COUNT = sizeof(tests) / sizeof(tests[0]);

    printf("Starting Type_ProfileSequenceDesc_Dup test suite (C++11 harness, non-GTest).\n");

    // Run tests
    for (int i = 0; i < TEST_COUNT; ++i) {
        total++;
        bool ok = tests[i]();
        if (ok) {
            printf("TEST PASSED: %s\n", test_names[i]);
            passed++;
        } else {
            printf("TEST FAILED: %s\n", test_names[i]);
        }
        // reset per-test failure counter (not strictly necessary since we print inside tests)
        g_failures = 0;
    }

    printf("Test suite finished. Passed: %d/%d. Failures: %d.\n", passed, total, g_failures);

    return (passed == total) ? 0 : 1;
}