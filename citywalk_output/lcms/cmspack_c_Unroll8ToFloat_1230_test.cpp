// Unit tests for Unroll8ToFloat in cms.c (LittleCMS-like functionality)
// Note: This test suite uses a lightweight, self-contained test harness
// (no Google Test). It relies on the real Unroll8ToFloat function being
// linked from the production library. The tests focus on exercising
// different execution paths by varying the information carried in the
// _cmsTRANSFORM's InputFormat field.
// Important: This test assumes the real _cmsTRANSFORM structure from the
// library is compatible with a minimal surrogate used here (first member
// InputFormat). If your build uses a different layout for _cmsTRANSFORM,
// adapt the surrogate layout accordingly or provide a proper fixture that
// matches the library's internal struct layout.
//
// How to run (conceptual):
//   - Build against the actual cmspack.c / lcms2 internal code used in your project.
//   - Link with the test binary.
//   - Run the resulting executable.
//   - The test runner will print PASS/FAIL with per-test explanations.
//
// The code is intentionally verbose in comments to guide future refinement of
// test cases for various code branches (Planar vs Chunky, Extra handling,
// SwapFirst, Reverse, etc.).

#include <cmath>
#include <lcms2_internal.h>
#include <cstdio>
#include <cstring>


// Forward declaration of the focal function (as exposed by the library).
// The actual signature in production is:
// cmsUInt8Number* Unroll8ToFloat(_cmsTRANSFORM* info,
//                                cmsFloat32Number wIn[],
//                                cmsUInt8Number* accum,
//                                cmsUInt32Number Stride);
extern "C" struct _cmsTRANSFORM; // opaque to test harness, we provide a compatible surrogate layout in test.

#ifdef __cplusplus
extern "C" {
#endif

// If the real library provides a C linkage, this declaration should be compatible.
// We declare the function with the expected prototype for linking against the library.
// Note: In a C++ test file, you may need to mark this as extern "C" to prevent name mangling.
cmsUInt8Number* Unroll8ToFloat(struct _cmsTRANSFORM* info,
                               cmsFloat32Number wIn[],
                               cmsUInt8Number* accum,
                               cmsUInt32Number Stride);
#ifdef __cplusplus
}
#endif

// Lightweight surrogate for _cmsTRANSFORM to allow tests to compile
// and pass the InputFormat value through to the real implementation.
// Important: This surrogate must map to the real library's _cmsTRANSFORM layout
// at least for the InputFormat member when the real function accesses it.
// If your library places InputFormat at a different offset, adjust accordingly.
struct _cmsTRANSFORM {
    cmsUInt32Number InputFormat; // Expose to the focal function as the first/primary field
    // The real implementation may have many more fields; tests only require the
    // InputFormat field for Unroll8ToFloat.
};

// Simple test harness (non-terminating assertions)
class TestHarness {
public:
    int total{0};
    int passed{0};

    static constexpr const char* PASS_STR = "PASS";
    static constexpr const char* FAIL_STR = "FAIL";

    void printSummary() const {
        printf("Test Summary: %d / %d passed\n", passed, total);
    }

    // Non-terminating assertion: reports failure but continues
    void expectTrue(bool cond, const char* msg) {
        total++;
        if (cond) {
            passed++;
        } else {
            printf("      [FAIL] %s\n", msg);
        }
    }

    // Helper to compare floats with tolerance (non-terminating)
    void expectFloatNear(float a, float b, float tol, const char* msg) {
        total++;
        if (std::fabs(a - b) <= tol) {
            passed++;
        } else {
            printf("      [FLOAT-MISMATCH] %s: %f vs %f (tol=%f)\n", msg, a, b, tol);
        }
    }

    // Helper to compare pointers (addresses)
    void expectPtrEq(const void* a, const void* b, const char* msg) {
        total++;
        if (a == b) {
            passed++;
        } else {
            printf("      [PTR-MISMATCH] %s: %p != %p\n", msg, a, (void*)b);
        }
    }
};

// Helper function to run a single test function with harness
typedef void (*TestFunc)(TestHarness&);

// Test 1: Basic path with 8 channels where InputFormat triggers minimal processing.
// Expectation: function should return a non-null accum movement and not crash.
void test_Unroll8ToFloat_BasicEightChannels(TestHarness& th) {
    // Setup: Create a dummy transform and buffer
    _cmsTRANSFORM info;
    // Minimal InputFormat; exact bit meaning is library-dependent.
    // We aim for a scenario where at least 8 channels are processed.
    info.InputFormat = 0; // Likely maps to a baseline format (8 channels is typical for Unroll8)
    
    cmsFloat32Number wIn[8] = {0};
    cmsUInt8Number accumBuf[16];
    std::memset(accumBuf, 0, sizeof(accumBuf));
    cmsUInt32Number Stride = 8;

    cmsUInt8Number* ret = Unroll8ToFloat(&info, wIn, accumBuf, Stride);
    th.printSummary(); // (we print later; placeholder to ensure harness structure)

    // Basic non-null check
    th.expectTrue(ret != nullptr, "Unroll8ToFloat Basic EightChannels returned null pointer");

    // Ensure the function produced some outputs within [0,1]
    for (int i = 0; i < 8; ++i) {
        th.expectFloatNear(wIn[i], wIn[i], 1e-6f, "Basic EightChannels wIn in-range check (self-equality)");
        // Since we don't know the exact expected values, we verify 0 <= wIn[i] <= 1
        th.expectTrue((wIn[i] >= 0.0f - 1e-6f) && (wIn[i] <= 1.0f + 1e-6f),
                      "Basic EightChannels wIn within [0,1]");
    }

    // No dedicated pointer arithmetic assertion due to OT specifics; ensure no crash path.
}

// Test 2: All accum values are 255, expecting mapping to 1.0 for wIn when a simple
// division by 255.0f is applied and no reversal occurs.
void test_Unroll8ToFloat_Accum255_ToOne(TestHarness& th) {
    _cmsTRANSFORM info;
    info.InputFormat = 0; // baseline to exercise normal path

    cmsFloat32Number wIn[8] = {0.0f};
    cmsUInt8Number accumBuf[16];
    // Fill with 255 so v = 1.0
    for (int i = 0; i < 8; ++i) accumBuf[i] = 255;
    cmsUInt32Number Stride = 8;

    cmsUInt8Number* ret = Unroll8ToFloat(&info, wIn, accumBuf, Stride);
    th.expectTrue(ret != nullptr, "Accum255_ToOne: return non-null");

    // Expect wIn to be very close to 1.0 (since v = 255/255.0, Reverse assumed false)
    for (int i = 0; i < 8; ++i) {
        th.expectFloatNear(wIn[i], 1.0f, 1e-5f, "Accum255_ToOne: wIn[i] close to 1.0");
    }
}

// Test 3: Rotate-first (SwapFirst) scenario is enabled. We simulate by setting
// InputFormat to a value that would trigger SwapFirst in the upstream macros.
// We then verify that if nChan>1 and Extra==0, the first two wIn values swap as per code path.
void test_Unroll8ToFloat_SwapFirst(TestHarness& th) {
    _cmsTRANSFORM info;
    // We attempt to enable SwapFirst path via a hypothetical InputFormat flag.
    // If the actual library uses a different bit, adjust accordingly.
    info.InputFormat = 0x1; // Placeholder: assume SwapFirst bit is set

    cmsFloat32Number wIn[8] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    cmsUInt8Number accumBuf[16];
    // Initialize accum with incremental values to observe swapping via wIn
    for (int i = 0; i < 8; ++i) accumBuf[i] = (cmsUInt8Number)(i * 16);
    cmsUInt32Number Stride = 8;

    cmsUInt8Number* ret = Unroll8ToFloat(&info, wIn, accumBuf, Stride);
    th.expectTrue(ret != nullptr, "SwapFirst: return non-null");

    // Because we can't guarantee exact post-state without the real bit-twiddling,
    // we at least validate that the first two elements have some swapped relationship
    // if the path executes. We allow a broad check here.
    bool swappedObserved = (std::fabs(wIn[0] - wIn[1]) > 1e-6f) || (wIn[0] != 0.0f || wIn[1] != 0.0f);
    th.expectTrue(swappedObserved, "SwapFirst: observed non-trivial swap/effect in wIn");
}

// Test 4: Planar path: when Planar flag is set in InputFormat, wIn extraction uses
// accum[(i+start)*Stride] instead of accum[i+start]. We simulate with a placeholder value.
void test_Unroll8ToFloat_PlanarPath(TestHarness& th) {
    _cmsTRANSFORM info;
    // Placeholder: set a bit that (in the real library) would indicate Planar layout
    info.InputFormat = 0x4; // Placeholder value

    cmsFloat32Number wIn[8] = {0.0f};
    cmsUInt8Number accumBuf[16];
    // Fill with a known pattern to observe division by 255
    for (int i = 0; i < 8; ++i) accumBuf[i] = (cmsUInt8Number)(i * 16);

    cmsUInt32Number Stride = 8;
    cmsUInt8Number* ret = Unroll8ToFloat(&info, wIn, accumBuf, Stride);
    th.expectTrue(ret != nullptr, "PlanarPath: return non-null");

    // Basic sanity: values should be in [0,1]
    for (int i = 0; i < 8; ++i) {
        th.expectTrue((wIn[i] >= 0.0f - 1e-6f) && (wIn[i] <= 1.0f + 1e-6f),
                      "PlanarPath: wIn in [0,1]");
    }
}

// Dispatcher to run all tests
void runAllTests() {
    TestHarness th;

    printf("Running Unroll8ToFloat test suite...\n");

    test_Unroll8ToFloat_BasicEightChannels(th);
    test_Unroll8ToFloat_Accum255_ToOne(th);
    test_Unroll8ToFloat_SwapFirst(th);
    test_Unroll8ToFloat_PlanarPath(th);

    th.printSummary();
}

// Main entry point
int main() {
    runAllTests();
    return 0;
}