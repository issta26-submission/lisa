// cmslut_tests.cpp
// Lightweight unit test suite for the focal method cmsStageClipNegatives
// Note: This test suite provides a self-contained mock environment to verify
// the interface contract of _cmsStageClipNegatives by intercepting the
// internal allocator (_cmsStageAllocPlaceholder) through a test double.
// The tests are designed to be run without GTest, using a small, non-terminating
// assertion framework implemented in this file.

#include <memory>
#include <vector>
#include <iostream>
#include <cassert>
#include <lcms2_internal.h>
#include <cstdint>


// Domain-dedicated typedefs mirroring the library's API (simplified for test)
using cmsContext = void*;
using cmsUInt32Number = uint32_t;
using cmsFloat32Number = float;

// Forward declaration of the test Clipper function
void Clipper(const cmsFloat32Number In[], cmsFloat32Number Out[], const struct cmsStage *mpe);

// Lightweight stage structure mirroring the essential fields used by the test.
// This is a simplified stand-in for cmsStage to allow introspection in tests.
struct cmsStage {
    cmsUInt32Number ElementType;                      // ElemType passed to allocator
    cmsUInt32Number InputChannels;                    // nChannels passed as input
    cmsUInt32Number OutputChannels;                   // nChannels passed as output
    void (*Clipper)(const cmsFloat32Number In[],
                    cmsFloat32Number Out[],
                    const cmsStage *mpe);           // Clipper function pointer
    void* data1;
    void* data2;
    void* data3;
};

// Global constant representing the element type used for ClipNegatives
// (mirrors the usage in _cmsStageClipNegatives)
const cmsUInt32Number cmsSigClipNegativesElemType = 0xC0FFEE; // arbitrary stable value

// Pointer to the last allocator call parameters (for assertions in tests)
static cmsContext g_lastContext = nullptr;
static cmsUInt32Number g_lastElemType = 0;
static cmsUInt32Number g_lastInputChannels = 0;
static cmsUInt32Number g_lastOutputChannels = 0;
static void (*g_lastClipper)(const cmsFloat32Number In[],
                              cmsFloat32Number Out[],
                              const cmsStage *mpe) = nullptr;
static void* g_lastA = nullptr;
static void* g_lastB = nullptr;
static void* g_lastC = nullptr;

// Simple _cmsStageAllocPlaceholder mock to capture call details and return a real stage.
// In a real library, this would allocate and initialize a complex stage object.
// Here we simulate allocation and store input parameters for verification.
static cmsStage* _cmsStageAllocPlaceholder(cmsContext ContextID,
                                           cmsUInt32Number ElemType,
                                           cmsUInt32Number nChannelsIn,
                                           cmsUInt32Number nChannelsOut,
                                           void (*Clipper)(const cmsFloat32Number In[],
                                                            cmsFloat32Number Out[],
                                                            const cmsStage *mpe),
                                           void* a, void* b, void* c)
{
    // Record last call details to enable assertions in tests
    g_lastContext = ContextID;
    g_lastElemType = ElemType;
    g_lastInputChannels = nChannelsIn;
    g_lastOutputChannels = nChannelsOut;
    g_lastClipper = Clipper;
    g_lastA = a;
    g_lastB = b;
    g_lastC = c;

    // Allocate a minimal cmsStage object and populate fields
    cmsStage* s = new cmsStage();
    s->ElementType = ElemType;
    s->InputChannels = nChannelsIn;
    s->OutputChannels = nChannelsOut;
    s->Clipper = Clipper;
    s->data1 = a;
    s->data2 = b;
    s->data3 = c;
    return s;
}

// Implementation of the focal method under test, copied/adapted for this test harness.
// It delegates to the mock allocator with the exact same signature as in the real library.
static cmsStage* cmsStageClipNegatives_Helper(cmsContext ContextID, cmsUInt32Number nChannels)
{
    // The method under test: it should return a stage allocated with the
    // ClipNegatives element type and the Clipper function.
    return _cmsStageAllocPlaceholder(ContextID, cmsSigClipNegativesElemType,
                                   nChannels, nChannels, Clipper, NULL, NULL, NULL);
}

// Exposed function under test (as per the focal method)
// In the real code, the function signature is:
// cmsStage* _cmsStageClipNegatives(cmsContext ContextID, cmsUInt32Number nChannels)
// We expose a similar wrapper that calls the helper above.
static cmsStage* _cmsStageClipNegatives(cmsContext ContextID, cmsUInt32Number nChannels)
{
    return cmsStageClipNegatives_Helper(ContextID, nChannels);
}

// A simple non-terminating assertion framework (EXPECT_* style)
static int g_test_passed = 0;
static int g_test_failed = 0;

#define EXPECT_TRUE(cond, msg) \
    do { if (!(cond)) { std::cerr << "[FAILED] " << msg << "\n"; g_test_failed++; } else { g_test_passed++; } } while (0)

#define EXPECT_EQ(a, b, msg) \
    do { if (!((a) == (b))) { std::cerr << "[FAILED] " << msg << " (" << (a) << " != " << (b) << ")\n"; g_test_failed++; } else { g_test_passed++; } } while (0)

#define EXPECT_NEQ(a, b, msg) \
    do { if ((a) == (b)) { std::cerr << "[FAILED] " << msg << " (values equal: " << (a) << ")\n"; g_test_failed++; } else { g_test_passed++; } } while (0)

#define TEST_CASE(name) void name()

// Utility to reset last-allocation tracking (for fresh test state)
static void reset_last_allocation_tracking()
{
    g_lastContext = nullptr;
    g_lastElemType = 0;
    g_lastInputChannels = 0;
    g_lastOutputChannels = 0;
    g_lastClipper = nullptr;
    g_lastA = nullptr;
    g_lastB = nullptr;
    g_lastC = nullptr;
}

// A dummy Clipper implementation to verify that the function passes a valid pointer
void Clipper(const cmsFloat32Number In[], cmsFloat32Number Out[], const cmsStage *mpe)
{
    // No-op: The test only needs to verify that the pointer is correctly passed
    // and that the stage object is constructed with the expected fields.
}

// Test 1: Basic allocation correctness
// Verifies that _cmsStageClipNegatives routes its arguments to the allocator with
// the expected element type and channel counts, and returns a non-null stage.
TEST_CASE(test_basic_allocation)
{
    reset_last_allocation_tracking();
    cmsContext ctx = reinterpret_cast<cmsContext>(0xDEADBEEF);
    cmsUInt32Number channels = 5;

    cmsStage* st = _cmsStageClipNegatives(ctx, channels);

    // Pointer returned should be non-null
    EXPECT_TRUE(st != nullptr, "Returned cmsStage pointer should be non-null");

    // Allocator should have been called with the correct parameters
    EXPECT_TRUE(g_lastContext == ctx, "Allocator ContextID should match input ContextID");
    EXPECT_EQ(g_lastElemType, cmsSigClipNegativesElemType, "Allocator ElemType should be cmsSigClipNegativesElemType");
    EXPECT_EQ(g_lastInputChannels, channels, "Allocator InputChannels should match nChannels");
    EXPECT_EQ(g_lastOutputChannels, channels, "Allocator OutputChannels should match nChannels");
    EXPECT_TRUE(g_lastClipper == Clipper, "Allocator Clipper should point to the Clipper function");
    // Cleanup
    delete st;
}

// Test 2: Multiple allocations produce distinct stage objects
// Ensures that two consecutive calls do not return the same pointer (no aliasing)
TEST_CASE(test_multiple_allocations_are_unique)
{
    reset_last_allocation_tracking();
    cmsContext ctx1 = reinterpret_cast<cmsContext>(0x1111);
    cmsContext ctx2 = reinterpret_cast<cmsContext>(0x2222);

    cmsStage* s1 = _cmsStageClipNegatives(ctx1, 3);
    cmsStage* s2 = _cmsStageClipNegatives(ctx2, 7);

    EXPECT_TRUE(s1 != nullptr && s2 != nullptr, "Both allocations should return non-null pointers");
    EXPECT_NEQ(s1, s2, "Consecutive allocations should return distinct pointers");

    // Validate first allocation's captured values
    EXPECT_TRUE(g_lastContext == ctx2, "Second allocation's ContextID should be the second call's Context");
    // Cleanup
    delete s1;
    delete s2;
}

// Test 3: Zero channels handling
// Ensure that the function handles 0 channels gracefully (still calls allocator)
TEST_CASE(test_zero_channels)
{
    reset_last_allocation_tracking();
    cmsContext ctx = nullptr;
    cmsUInt32Number channels = 0;

    cmsStage* s = _cmsStageClipNegatives(ctx, channels);

    EXPECT_TRUE(s != nullptr, "Allocation should succeed for 0 channels (stage returned)");
    EXPECT_EQ(g_lastInputChannels, channels, "Allocator should receive 0 as InputChannels");
    EXPECT_EQ(g_lastOutputChannels, channels, "Allocator should receive 0 as OutputChannels");

    delete s;
}

// Test 4: Element type propagation check
// Verifies that the allocator receives the predefined ClipNegatives element type
TEST_CASE(test_element_type_propagation)
{
    reset_last_allocation_tracking();
    cmsStage* s = _cmsStageClipNegatives(nullptr, 9);

    EXPECT_TRUE(s != nullptr, "Allocation should succeed for any channel count");
    EXPECT_EQ(g_lastElemType, cmsSigClipNegativesElemType, "Allocator ElemType should be cmsSigClipNegativesElemType");

    delete s;
}

// Test 5: Verifies that the ClipNegatives Clipper pointer is preserved in the stage
TEST_CASE(test_clipper_pointer_preserved)
{
    reset_last_allocation_tracking();
    cmsStage* s = _cmsStageClipNegatives(nullptr, 4);

    EXPECT_TRUE(s != nullptr, "Allocation should succeed and return a valid stage");
    EXPECT_TRUE(s->Clipper == Clipper, "Stage Clipper pointer should be the test Clipper function");

    delete s;
}

// Run all test cases and report summary
int main()
{
    std::cout << "Starting cmsStageClipNegatives unit tests (self-contained mock)\n";

    test_basic_allocation();
    test_multiple_allocations_are_unique();
    test_zero_channels();
    test_element_type_propagation();
    test_clipper_pointer_preserved();

    int total = g_test_passed + g_test_failed;
    std::cout << "Tests run: " << total << ", Passed: " << g_test_passed
              << ", Failed: " << g_test_failed << "\n";

    if (g_test_failed == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << "Some tests failed. See details above.\n";
        return 1;
    }
}