// Unit test suite for Type_ColorantOrderType_Free in cmstypes.c
// This test uses a lightweight manual test harness (no gtest/gmock).

#include <cstring>
#include <utility>
#include <vector>
#include <iostream>
#include <cassert>
#include <lcms2_internal.h>
#include <cstdint>


// Import the internal CMS types to interact with the focal method under test.

// Forward declaration of the focal function under test.
// Signature exactly matches the production code.
extern "C" void Type_ColorantOrderType_Free(struct _cms_typehandler_struct* self, void* Ptr);

// -----------------------------------------------------------------------------
// Mocking _cmsFree to observe calls without needing the real allocator details.
// We provide a C linkage implementation so that the linker resolves the production
// call to our mock.
// The mock records each invocation (ContextID, Ptr) in a global log.
// -----------------------------------------------------------------------------

static std::vector<std::pair<void*, void*>> g_cmsFreeCalls;

// Mock implementation of _cmsFree. It records the parameters for verification.
// The real cms library would free memory via the context's allocator; here we only
// observe that the focal method passes the correct arguments through.
extern "C" void _cmsFree(void* ContextID, void* Ptr)
{
    g_cmsFreeCalls.emplace_back(ContextID, Ptr);
}

// Convenience helper to clear the log between tests.
static void clear_cmsFree_log() {
    g_cmsFreeCalls.clear();
}

// -----------------------------------------------------------------------------
// Test helpers and tests
// NOTE: The tests are designed to be executed from main() as a simple harness.
// Each test is annotated with comments describing intent and expected outcomes.
// -----------------------------------------------------------------------------

static int gFailures = 0;

// Lightweight assertion macro that logs failures but does not abort execution.
// This aligns with the request to use non-terminating assertions to maximize coverage.
#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test Failure: " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++gFailures; \
    } \
} while(0)

// Test 1: Ensure Type_ColorantOrderType_Free passes through the exact ContextID and Ptr
// to _cmsFree when Ptr is non-null.
static void test_ColorantOrderType_Free_PassesContextAndPtr()
{
    // Arrange
    clear_cmsFree_log();
    struct _cms_typehandler_struct self;
    // Use a distinct context identifier to verify correct propagation.
    cmsContext ctx = (cmsContext)reinterpret_cast<void*>(0xA1A1A1A1);
    self.ContextID = ctx;
    void* ptr = reinterpret_cast<void*>(0xDEADBEEF);

    // Act
    Type_ColorantOrderType_Free(&self, ptr);

    // Assert
    CHECK(g_cmsFreeCalls.size() == 1, "Expected exactly 1 _cmsFree call");
    if (!g_cmsFreeCalls.empty()) {
        CHECK(g_cmsFreeCalls[0].first == reinterpret_cast<void*>(0xA1A1A1A1),
              "Expected ContextID to be passed through to _cmsFree");
        CHECK(g_cmsFreeCalls[0].second == reinterpret_cast<void*>(0xDEADBEEF),
              "Expected Ptr to be passed through to _cmsFree");
    }
}

// Test 2: Ensure Type_ColorantOrderType_Free handles a null Ptr gracefully.
// The mock should still be invoked with a null Ptr.
static void test_ColorantOrderType_Free_NullPtr()
{
    // Arrange
    clear_cmsFree_log();
    struct _cms_typehandler_struct self;
    cmsContext ctx = (cmsContext)reinterpret_cast<void*>(0xB2B2B2B2);
    self.ContextID = ctx;
    void* ptr = nullptr;

    // Act
    Type_ColorantOrderType_Free(&self, ptr);

    // Assert
    CHECK(g_cmsFreeCalls.size() == 1, "Expected exactly 1 _cmsFree call for NULL Ptr");
    if (!g_cmsFreeCalls.empty()) {
        CHECK(g_cmsFreeCalls[0].first == reinterpret_cast<void*>(0xB2B2B2B2),
              "ContextID should still be passed through for NULL Ptr");
        CHECK(g_cmsFreeCalls[0].second == nullptr,
              "Ptr should be passed as NULL to _cmsFree");
    }
}

// Test 3: Verify behavior with multiple different contexts.
// The function should call _cmsFree with the corresponding ContextID for each call.
static void test_ColorantOrderType_Free_MultipleContexts()
{
    // Arrange
    clear_cmsFree_log();
    struct _cms_typehandler_struct s1;
    cmsContext ctx1 = (cmsContext)reinterpret_cast<void*>(0x11111111);
    s1.ContextID = ctx1;

    struct _cms_typehandler_struct s2;
    cmsContext ctx2 = (cmsContext)reinterpret_cast<void*>(0x22222222);
    s2.ContextID = ctx2;

    void* p1 = reinterpret_cast<void*>(0x10101010);
    void* p2 = reinterpret_cast<void*>(0x20202020);

    // Act
    Type_ColorantOrderType_Free(&s1, p1);
    Type_ColorantOrderType_Free(&s2, p2);

    // Assert
    CHECK(g_cmsFreeCalls.size() == 2, "Expected two _cmsFree calls for two different contexts");
    if (g_cmsFreeCalls.size() == 2) {
        CHECK(g_cmsFreeCalls[0].first == reinterpret_cast<void*>(0x11111111),
              "First call should use first context");
        CHECK(g_cmsFreeCalls[0].second == reinterpret_cast<void*>(0x10101010),
              "First call should pass first Ptr");
        CHECK(g_cmsFreeCalls[1].first == reinterpret_cast<void*>(0x22222222),
              "Second call should use second context");
        CHECK(g_cmsFreeCalls[1].second == reinterpret_cast<void*>(0x20202020),
              "Second call should pass second Ptr");
    }
}

// Test 4: Ensure self's ContextID is not modified by Type_ColorantOrderType_Free.
// This checks that the function does not write into the self structure.
static void test_ColorantOrderType_Free_DoesNotModify_Self()
{
    // Arrange
    clear_cmsFree_log();
    struct _cms_typehandler_struct self;
    cmsContext ctx = (cmsContext)reinterpret_cast<void*>(0x9ABCDEF0);
    self.ContextID = ctx;
    void* originalCtx = self.ContextID;
    void* ptr = reinterpret_cast<void*>(0x12345678);

    // Act
    Type_ColorantOrderType_Free(&self, ptr);

    // Assert: ContextID remains unchanged and no undefined writes occurred.
    CHECK(self.ContextID == reinterpret_cast<cmsContext>(originalCtx),
          "Self.ContextID should remain unchanged after Type_ColorantOrderType_Free");
    // Also ensure that at least one call happened (sanity check)
    CHECK(!g_cmsFreeCalls.empty(), "Expected _cmsFree to be invoked");
}

// Test 5: Stress test with several interleaved calls to ensure no state bleed between tests.
// This uses alternating Ptr/context values.
static void test_ColorantOrderType_Free_InterleavedCalls()
{
    // Arrange
    clear_cmsFree_log();
    struct _cms_typehandler_struct a;
    a.ContextID = (cmsContext)reinterpret_cast<void*>(0xAAA1);

    struct _cms_typehandler_struct b;
    b.ContextID = (cmsContext)reinterpret_cast<void*>(0xBBB2);

    void* pA = reinterpret_cast<void*>(0xAA0001);
    void* pB = reinterpret_cast<void*>(0xBB0002);

    // Act
    Type_ColorantOrderType_Free(&a, pA);
    Type_ColorantOrderType_Free(&b, pB);
    Type_ColorantOrderType_Free(&a, nullptr);

    // Assert
    CHECK(g_cmsFreeCalls.size() == 3, "Expected 3 _cmsFree calls in interleaved test");
    if (g_cmsFreeCalls.size() == 3) {
        CHECK(g_cmsFreeCalls[0].first == reinterpret_cast<void*>(0xAAA1) &&
              g_cmsFreeCalls[0].second == reinterpret_cast<void*>(0xAA0001),
              "First call context/ptr mismatch");
        CHECK(g_cmsFreeCalls[1].first == reinterpret_cast<void*>(0xBBB2) &&
              g_cmsFreeCalls[1].second == reinterpret_cast<void*>(0xBB0002),
              "Second call context/ptr mismatch");
        CHECK(g_cmsFreeCalls[2].first == reinterpret_cast<void*>(0xAAA1) &&
              g_cmsFreeCalls[2].second == nullptr,
              "Third call should pass through NULL Ptr for same context as first");
    }
}

// Run all tests
static void run_all_tests()
{
    test_ColorantOrderType_Free_PassesContextAndPtr();
    test_ColorantOrderType_Free_NullPtr();
    test_ColorantOrderType_Free_MultipleContexts();
    test_ColorantOrderType_Free_DoesNotModify_Self();
    test_ColorantOrderType_Free_InterleavedCalls();
}

// -----------------------------------------------------------------------------
// Main: entry point for the test harness
// -----------------------------------------------------------------------------

int main()
{
    run_all_tests();

    if (gFailures == 0)
    {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    }
    else
    {
        std::cerr << gFailures << " TEST(S) FAILED" << std::endl;
        return gFailures;
    }
}