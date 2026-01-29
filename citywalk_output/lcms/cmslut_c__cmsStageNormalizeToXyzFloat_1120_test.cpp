// cmsStageNormalizeToXyzFloat_Test.cpp
// A lightweight C++11 unit test suite for the focal method
// _cmsStageNormalizeToXyzFloat in cmslut.c (LCMS2 project).
//
// Key ideas implemented here:
// - Directly invokes the focal function under test.
// - Uses simple, non-terminating test assertions to maximize code flow.
// - Validates basic post-conditions of the function (non-null stage and correct Implements signature).
// - Demonstrates a minimal test harness without any external test framework (no GTest).
//
// Assumptions:
// - The LCMS2 public headers are available (lcms2.h) and provide cmsContext, cmsStage, cmsSigFloatPCS2XYZ, etc.
// - The focal function is linked into the test binary (i.e., cmslut.c is compiled/linked with this test).
// - The focal function is accessible (non-static) from the test build. If it's static in your build, this test must be compiled together with cmslut.c to access it directly.

#include <iostream>
#include <iomanip>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include LCMS2 public headers in a C linkage block for correct symbol resolution.
extern "C" {
}

// Declaration of the focal function under test.
// It is defined in cmslut.c and, for the purposes of this test, is brought into the linkage of this binary.
// If your build uses a different internal declaration (e.g., static), ensure cmslut.c is compiled
// alongside this test to allow direct access.
extern "C" cmsStage* _cmsStageNormalizeToXyzFloat(cmsContext ContextID);

// Simple, minimal test framework (non-terminating assertions)
static int gTestCount = 0;
static int gFailCount = 0;

#define TEST_PASSED (gFailCount == 0)

static void testHeader(const char* name) {
    std::cout << "Running test: " << name << " ..." << std::endl;
}

// Basic assertion helpers (non-terminating)
static void expect_true(bool cond, const char* msg) {
    ++gTestCount;
    if (!cond) {
        ++gFailCount;
        std::cerr << "  [FAILED] " << msg << std::endl;
    }
}
static void expect_true_with_note(bool cond, const char* msg, const char* note) {
    ++gTestCount;
    if (!cond) {
        ++gFailCount;
        std::cerr << "  [FAILED] " << msg << " | Note: " << note << std::endl;
    }
}

// Test 1: _cmsStageNormalizeToXyzFloat should return a non-null stage when called (true branch)
static bool test_NormalizeToXyzFloat_ReturnsNonNull() {
    testHeader("NormalizeToXyzFloat_ReturnsNonNull");

    // Call with a NULL context (as used in internal test scenarios for stability)
    cmsStage* stage = _cmsStageNormalizeToXyzFloat(nullptr);

    // Expect a non-null pointer (the true-branch of the NULL-check in the focal function)
    if (stage == nullptr) {
        std::cerr << "    ERROR: _cmsStageNormalizeToXyzFloat(nullptr) returned NULL." << std::endl;
        return false;
    }

    // Basic sanity: non-null stage obtained
    expect_true(stage != nullptr, "Stage should be non-null after normalization to XYZ float.");
    return TEST_PASSED;
}

// Test 2: The created stage should implement the expected signature (Implements field)
static bool test_NormalizeToXyzFloat_ImplementsSignature() {
    testHeader("NormalizeToXyzFloat_ImplementsSignature");

    cmsStage* stage = _cmsStageNormalizeToXyzFloat(nullptr);

    if (stage == nullptr) {
        std::cerr << "    ERROR: _cmsStageNormalizeToXyzFloat(nullptr) returned NULL; cannot verify Implements." << std::endl;
        return false;
    }

    // Access the public member 'Implements' of cmsStage and verify the signature
    // The expected signature for a PCS2XYZ transform in this context is cmsSigFloatPCS2XYZ.
    // We rely on the public API macro cmsSigFloatPCS2XYZ.
    if (stage->Implements != cmsSigFloatPCS2XYZ) {
        std::cerr << "    ERROR: Stage Implements field mismatch. Expected cmsSigFloatPCS2XYZ, got " 
                  << std::hex << static_cast<uint32_t>(stage->Implements) << std::dec << "." << std::endl;
        return false;
    }

    return TEST_PASSED;
}

// Test 3: Stability check - calling with a NULL context should not crash (best-effort observation)
static bool test_NormalizeToXyzFloat_NullContextStability() {
    testHeader("NormalizeToXyzFloat_NullContextStability");

    // Some builds may treat NULL context gracefully; others may fail. We only assert that
    // the function does not dereference a NULL context in a way that crashes the process.
    // We implement this as a no-throw, but still report success if no crash occurs.
    // Since C++ cannot catch a hard crash, this test relies on the platform's ability to continue.
    // Here we simply call and rely on the absence of a crash for success.
    cmsStage* stage = _cmsStageNormalizeToXyzFloat(nullptr);

    // If the call returns NULL, that's a distinct failure for this environment
    // but we still report based on whether a non-null stage was produced (consistent with Test 1).
    bool ok = (stage != nullptr);
    expect_true(ok, "Null context should still produce a non-null stage (if library supports NULL context).");

    return TEST_PASSED;
}

// Entry point for tests
int main() {
    // Run tests
    bool t1 = test_NormalizeToXyzFloat_ReturnsNonNull();
    bool t2 = test_NormalizeToXyzFloat_ImplementsSignature();
    bool t3 = test_NormalizeToXyzFloat_NullContextStability();

    // Summary
    std::cout << "\nTest Summary: " << gTestCount << " tests run, "
              << gFailCount << " failures." << std::endl;

    // Simple pass/fail exit code
    return (gFailCount == 0) ? 0 : 1;
}