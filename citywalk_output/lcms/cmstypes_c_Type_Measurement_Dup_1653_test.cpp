/*
Test Suite for focal method: Type_Measurement_Dup (in cmstypes.c)

Step 1 (Program Understanding):
- Focal method copies a cmsICCMeasurementConditions block by delegating to _cmsDupMem using the context ID from self->ContextID.
- Signature: void* Type_Measurement_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n)
- Core dependencies evidenced in the provided class dependency block: cmsICCMeasurementConditions, _cmsDupMem, cmsContext, cmsUInt32Number, and the type self (struct _cms_typehandler_struct).
- Important behavior: n is unused (cmsUNUSED_PARAMETER(n)) and the duplication size is sizeof(cmsICCMeasurementConditions).

Candidate keywords (core components to reflect in tests):
- Type_Measurement_Dup, _cmsDupMem, cmsICCMeasurementConditions, ContextID, cmsContext, Ptr, n, sizeof(...)

Step 2 (Unit Test Generation):
- Strategy: Override _cmsDupMem via a test-specific, link-time definition to capture ContextID passed and to simulate a memory-duplication operation without requiring the full external library behavior.
- Tests aim to cover:
  - Basic duplication: content of duplicate equals the source, and the correct ContextID is passed to _cmsDupMem.
  - n parameter is ignored by implementation (test with different n values yields identical results).
  - Behavior is consistent for multiple calls with different ContextIDs.

Step 3 (Test Case Refinement):
- Use a small, self-contained test harness (no GTest); implement a micro test runner in main().
- Use non-terminating EXPECT-like assertions that accumulate failures but continue execution to maximize coverage.
- Access to static/private internals is avoided; tests rely on public interface plus the test-provided stub for _cmsDupMem.

Code (C++11, no GTest, main() entry point, with explanatory comments per test):

*/

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


// Include the library's internal definitions to match types used by the focal method.
// Ensure this header is accessible in the test environment.
extern "C" {
}

// Forward declarations of the function under test.
// The actual implementation is in cmstypes.c, linked during build.
extern "C" void* Type_Measurement_Dup(struct _cms_typehandler_struct* self, const void *Ptr, cmsUInt32Number n);

// Test-time stub to override _cmsDupMem behavior.
// This allows us to validate that Type_Measurement_Dup calls _cmsDupMem with the correct parameters
// and duplicates memory correctly, without relying on the full library memory management.
static cmsContext g_lastContextID = 0;
static const void* g_lastPtr = nullptr;
static cmsUInt32Number g_lastSize = 0;

extern "C" void* _cmsDupMem(cmsContext ContextID, const void* Ptr, cmsUInt32Number Size)
{
    // Capture parameters for verification
    g_lastContextID = ContextID;
    g_lastPtr = Ptr;
    g_lastSize = Size;

    // Simple and safe memory copy implementation for testing.
    if (Ptr == nullptr || Size == 0) {
        // Return a non-null pointer for Size==0 if needed, but keep behavior deterministic.
        void* r = malloc(0);
        return r;
    }
    void* dup = malloc((size_t)Size);
    if (!dup) {
        return nullptr;
    }
    memcpy(dup, Ptr, (size_t)Size);
    return dup;
}

// Lightweight test harness helpers (non-terminating assertions)
static int gFailures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[FAILED] " << (msg) << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++gFailures; \
    } \
} while(0)

#define EXPECT_NOT_NULL(ptr, msg) do { \
    if ((ptr) == nullptr) { \
        std::cerr << "[FAILED] " << (msg) << " (nullptr) at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++gFailures; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if (!((a) == (b))) { \
        std::cerr << "[FAILED] " << (msg) << " (" << (a) << " != " << (b) << ") at " \
                  << __FILE__ << ":" << __LINE__ << "\n"; \
        ++gFailures; \
    } \
} while(0)


// Test 1: Basic duplication preserves content and uses correct ContextID
void test_basic_dup() {
    // Prepare a dummy self with a known ContextID
    struct _cms_typehandler_struct testSelf;
    testSelf.ContextID = (cmsContext)0xABCDEF01;

    // Prepare source memory block of known size
    cmsUInt32Number size = sizeof(cmsICCMeasurementConditions);
    void* original = malloc((size_t)size);
    // Fill with a recognizable pattern
    memset(original, 0xAA, (size_t)size);

    // Call the function under test
    void* dup = Type_Measurement_Dup(&testSelf, original, 5);

    // Assertions
    EXPECT_NOT_NULL(dup, "Type_Measurement_Dup should return a non-null pointer");
    // The test-time stub should have captured the correct ContextID
    EXPECT_TRUE(g_lastContextID == testSelf.ContextID, "ContextID passed to _cmsDupMem should equal self.ContextID");
    // The duplicate should have identical contents
    EXPECT_TRUE(memcmp(dup, original, (size_t)size) == 0, "Duplicated memory should match original bytes");

    // Cleanup
    free(dup);
    free(original);
    // Reset last-context info for isolation between tests
    g_lastContextID = 0;
    g_lastPtr = nullptr;
    g_lastSize = 0;
}

// Test 2: The n parameter is unused; verify identical results with different n values
void test_n_param_unused() {
    struct _cms_typehandler_struct testSelf;
    testSelf.ContextID = (cmsContext)0x12345678;

    cmsUInt32Number size = sizeof(cmsICCMeasurementConditions);
    void* original = malloc((size_t)size);
    memset(original, 0x55, (size_t)size);

    void* dup1 = Type_Measurement_Dup(&testSelf, original, 0);
    void* dup2 = Type_Measurement_Dup(&testSelf, original, 999);

    EXPECT_NOT_NULL(dup1, "First duplication should not be null");
    EXPECT_NOT_NULL(dup2, "Second duplication should not be null");
    EXPECT_TRUE(g_lastContextID == testSelf.ContextID, "ContextID should be passed consistently to _cmsDupMem");
    EXPECT_TRUE(memcmp(dup1, original, (size_t)size) == 0, "First duplicate content should match original");
    EXPECT_TRUE(memcmp(dup2, original, (size_t)size) == 0, "Second duplicate content should match original (n unused)");

    free(dup1);
    free(dup2);
    free(original);
    g_lastContextID = 0;
    g_lastPtr = nullptr;
    g_lastSize = 0;
}

// Test 3: Dup with multiple different ContextIDs ensures per-call ContextID usage
void test_context_id_variants() {
    struct _cms_typehandler_struct testSelf1;
    testSelf1.ContextID = (cmsContext)0x0A0B0C0D;

    struct _cms_typehandler_struct testSelf2;
    testSelf2.ContextID = (cmsContext)0x0D0C0B0A;

    cmsUInt32Number size = sizeof(cmsICCMeasurementConditions);
    void* original = malloc((size_t)size);
    memset(original, 0x77, (size_t)size);

    void* dup1 = Type_Measurement_Dup(&testSelf1, original, 0);
    EXPECT_NOT_NULL(dup1, "Dup with first ContextID should not be null");
    EXPECT_TRUE(g_lastContextID == testSelf1.ContextID, "First ContextID passed to _cmsDupMem");

    free(dup1);
    g_lastContextID = 0; g_lastPtr = nullptr; g_lastSize = 0;

    void* dup2 = Type_Measurement_Dup(&testSelf2, original, 0);
    EXPECT_NOT_NULL(dup2, "Dup with second ContextID should not be null");
    EXPECT_TRUE(g_lastContextID == testSelf2.ContextID, "Second ContextID passed to _cmsDupMem");

    free(dup2);
    free(original);
    g_lastContextID = 0; g_lastPtr = nullptr; g_lastSize = 0;
}


// Main test runner
int main() {
    std::cout << "Running tests for Type_Measurement_Dup (cmstypes.c) under C++11 test harness\n";

    test_basic_dup();
    test_n_param_unused();
    test_context_id_variants();

    if (gFailures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << gFailures << " test(s) failed.\n";
        return 1;
    }
}