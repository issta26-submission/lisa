// C++11 unit test suite for the focal method StageAllocNegate in testplugin.c
// This test suite uses a lightweight, self-contained test framework (no Google Test).
// It relies on the public LittleCMS-like API available via lcms2.h (or equivalent) 
// and declares the StageAllocNegate symbol with C linkage for linking.
//
// Key focus:
// - Validate that StageAllocNegate returns a non-null stage for various cmsContext inputs.
// - Validate basic behavior: multiple calls produce distinct stage instances.
// - Ensure graceful handling when a null/zero context is provided.
// Note: The internal structure of cmsStage is not accessed directly; tests rely on
// public API semantics and object non-null guarantees to achieve coverage.

#include <iostream>
#include <testcms2.h>
#include <cstdlib>
#include <lcms2.h>
#include <cstdint>


// Attempt to include the public CMS headers used by the project.
// If your environment uses a different header (e.g., lcms2.h), adjust accordingly.
// The types cmsContext, cmsStage, and the constant prototypes are expected to be defined here.

// Forward declaration of the focal method using C linkage.
// The actual implementation resides in testplugin.c and should be linked in the test executable.
extern "C" cmsStage* StageAllocNegate(cmsContext ContextID);

// Lightweight assertion helpers to avoid terminating tests on first failure.
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do {                              \
    ++g_tests_run;                                                 \
    if(!(cond)) {                                                  \
        std::cerr << "ASSERTION FAILED: " << (msg)                 \
                  << " (Line: " << __LINE__ << ")" << std::endl;  \
        ++g_tests_failed;                                          \
    }                                                              \
} while(0)

#define EXPECT_NOT_NULL(ptr, msg) EXPECT_TRUE((ptr) != nullptr, msg)
#define EXPECT_NOT_EQ(a, b, msg) EXPECT_TRUE((a) != (b), msg)


// Test 1: StageAllocNegate should return a non-null stage when provided with a valid non-null context.
// Rationale: The focal implementation directly delegates to _cmsStageAllocPlaceholder with fixed
// parameters; this test ensures the wrapper does not fail catastrophically for a typical context.
bool test_StageAllocNegate_NotNull_WithValidContext() {
    cmsContext ctx = reinterpret_cast<cmsContext>(0x1234); // arbitrary non-null context
    cmsStage* stage = StageAllocNegate(ctx);

    EXPECT_NOT_NULL(stage, "StageAllocNegate should not return null for a valid non-null context.");
    return stage != nullptr;
}

// Test 2: StageAllocNegate should return a non-null stage even when the context is NULL.
// Rationale: Public API robustness; the underlying implementation does not guard on null context
// in a way that would cause a crash or NULL return in typical usage.
bool test_StageAllocNegate_NotNull_WithNullContext() {
    cmsContext ctx = nullptr;
    cmsStage* stage = StageAllocNegate(ctx);

    EXPECT_NOT_NULL(stage, "StageAllocNegate should not return null when context is NULL.");
    return stage != nullptr;
}

// Test 3: Consecutive calls to StageAllocNegate with the same context should yield distinct stage instances.
// Rationale: Ensures no shared interior state (or pointer reuse) across allocations, which improves test coverage
// of the allocation path.
bool test_StageAllocNegate_MultipleAllocationsAreDistinct() {
    cmsContext ctx = reinterpret_cast<cmsContext>(0xDEADBEEF); // arbitrary context
    cmsStage* s1 = StageAllocNegate(ctx);
    cmsStage* s2 = StageAllocNegate(ctx);

    EXPECT_NOT_NULL(s1, "First StageAllocNegate call returned null.");
    EXPECT_NOT_NULL(s2, "Second StageAllocNegate call returned null.");
    EXPECT_NOT_EQ(s1, s2, "Consecutive StageAllocNegate allocations should yield distinct stage pointers.");

    return (s1 != nullptr) && (s2 != nullptr) && (s1 != s2);
}

// Test 4: Repeated allocations with a synthetic variety of contexts should remain non-null.
// Rationale: Combines various input scenarios to exercise typical usage patterns and widen coverage.
bool test_StageAllocNegate_MultipleContextsNonNull() {
    cmsContext contexts[] = {
        reinterpret_cast<cmsContext>(0x1),
        reinterpret_cast<cmsContext>(0x1001),
        reinterpret_cast<cmsContext>(0x7FFF),
        nullptr
    };

    bool all_ok = true;
    for (size_t i = 0; i < sizeof(contexts)/sizeof(contexts[0]); ++i) {
        cmsStage* stage = StageAllocNegate(contexts[i]);
        if (stage == nullptr) {
            std::cerr << "StageAllocNegate returned null for context index " << i << std::endl;
            all_ok = false;
        }
        // Do not free here; the test harness does not assume ownership semantics of StageAllocNegate.
    }
    EXPECT_TRUE(all_ok, "All StageAllocNegate calls with varied contexts should return non-null or handle gracefully.");
    return all_ok;
}


// Entry point: Run all tests and report a summary.
// Step 3 (Domain knowledge): The tests focus on true/false branches where reasonable (non-null vs null context,
// distinct allocations). They avoid private/internal methods and rely on public stage creation behavior.
int main() {
    std::cout << "Starting StageAllocNegate unit tests (C++11, no GTest)..." << std::endl;

    bool t1 = test_StageAllocNegate_NotNull_WithValidContext();
    bool t2 = test_StageAllocNegate_NotNull_WithNullContext();
    bool t3 = test_StageAllocNegate_MultipleAllocationsAreDistinct();
    bool t4 = test_StageAllocNegate_MultipleContextsNonNull();

    int total = 0 + 0; // placeholder to keep style explicit
    total = 4;
    int passed = (t1 ? 1 : 0) + (t2 ? 1 : 0) + (t3 ? 1 : 0) + (t4 ? 1 : 0);
    int failed = g_tests_failed;

    std::cout << "StageAllocNegate test results: " 
              << passed << " passed, " << failed << " failed, out of " 
              << g_tests_run << " tests." << std::endl;

    // Return non-zero if any test failed to aid CI environments.
    return (failed == 0) ? 0 : 1;
}