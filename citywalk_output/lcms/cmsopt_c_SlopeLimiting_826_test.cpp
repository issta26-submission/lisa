/*
   Unit tests for the focal method: SlopeLimiting in cmsopt.c

   Approach:
   - Create cmsToneCurve instances, populate Table16 and nEntries to exercise both
     potential execution paths by influencing the behavior of cmsIsToneCurveDescending.
   - Test key properties after SlopeLimiting:
     - All output values remain within 0..0xFFFF (saturation).
     - Descending branch yields a nonincreasing tail from AtEnd to the end.
     - Ascending branch yields a nondecreasing tail from AtEnd to the end, ending at 0xFFFF.
   - Use a lightweight, non-terminating test harness (console output) suitable for C++11.
   - No GoogleTest; tests are run via main().

   Notes:
   - This test relies on library behavior of cmsIsToneCurveDescending to interpret a curve as
     descending when appropriate. We shape Table16[0], Table16[nEntries-1], and critical
     indices (AtBegin, AtEnd) to trigger the desired branch in typical lcms2 implementations.
   - The tests focus on behavior around the critical 2%/98% cutoffs and the tail ramp logic.
*/

#include <limits>
#include <iostream>
#include <cmath>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// If the test environment requires a different include path, adjust accordingly.

// Forward declaration of the focal function (should be linked from cmsopt.c)
extern "C" void SlopeLimiting(cmsToneCurve* g);

// Lightweight test framework (non-terminating assertions)
static bool g_AllTestsPassed = true;
static void TestFail(const char* testName, const char* reason, int line) {
    std::cerr << "[FAIL] " << testName << " (line " << line << "): " << reason << std::endl;
    g_AllTestsPassed = false;
}
#define TEST_ASSERT(cond, testName, reason) do { \
    if(!(cond)) { TestFail(testName, reason, __LINE__); } \
} while(0)

//
// Test 1: Descending branch tail monotonicity
// We configure a tone curve that should trigger the "descending" path
// and verify the tail portion (From AtEnd to the last entry) is nonincreasing
// and ends at EndVal (0x0000).
//
static bool TestSlopeLimiting_Descending_TailMonotonic() {
    const int N = 256;
    cmsToneCurve g;
    g.nEntries = N;
    g.Table16 = new cmsUInt16Number[N];

    // Prepare a curve that should be identified as descending by cmsIsToneCurveDescending
    // by setting endpoints accordingly.
    g.Table16[0] = 0xFFFF;          // high at start
    g.Table16[N - 1] = 0x0000;      // low at end

    // Choose AtBegin and AtEnd deterministically from N
    int AtBegin = (int) floor((cmsFloat64Number) g.nEntries * 0.02 + 0.5); // 2% cutoff
    int AtEnd   = g.nEntries - AtBegin - 1;

    // Provide non-default values to influence slope calculation
    // Ensure AtBegin value is not trivial so the first slope is well-defined
    g.Table16[AtBegin] = 0xC000; // mid-high
    // Ensure AtEnd value is not the default; it should be preserved for tail computation
    g.Table16[AtEnd] = 0x8000;   // mid

    // Call focal method
    SlopeLimiting(&g);

    // Validate: all outputs stay within 0..0xFFFF
    for (int i = 0; i < N; ++i) {
        if (g.Table16[i] > 0xFFFF) {
            TestFail("TestSlopeLimiting_Descending_TailMonotonic", "Value exceeded 0xFFFF saturation", __LINE__);
            break;
        }
        if (g.Table16[i] < 0) {
            TestFail("TestSlopeLimiting_Descending_TailMonotonic", "Negative value after saturation", __LINE__);
            break;
        }
    }

    // Validate tail monotonicity: from AtEnd to N-2, the sequence should be nonincreasing
    for (int i = AtEnd; i < N - 1; ++i) {
        if (g.Table16[i] < g.Table16[i + 1]) {
            TestFail("TestSlopeLimiting_Descending_TailMonotonic",
                     "Tail portion is not nonincreasing as expected for descending curve", __LINE__);
            break;
        }
    }

    // Validate the tail ends at EndVal = 0x0000
    if (g.Table16[N - 1] != 0) {
        TestFail("TestSlopeLimiting_Descending_TailMonotonic",
                 "Tail end does not reach EndVal (0x0000)", __LINE__);
    }

    delete[] g.Table16;
    return g_AllTestsPassed;
}


//
// Test 2: Ascending branch tail monotonicity
// We configure a tone curve that should trigger the "ascending" path
// and verify the tail portion (From AtEnd to the last entry) is nondecreasing
// and ends at EndVal (0xFFFF).
//
static bool TestSlopeLimiting_Ascending_TailMonotonic() {
    const int N = 256;
    cmsToneCurve g;
    g.nEntries = N;
    g.Table16 = new cmsUInt16Number[N];

    // Prepare endpoints for ascending interpretation
    g.Table16[0] = 0x0000;       // low at start
    g.Table16[N - 1] = 0xFFFF;   // high at end

    int AtBegin = (int) floor((cmsFloat64Number) g.nEntries * 0.02 + 0.5); // 2% cutoff
    int AtEnd   = g.nEntries - AtBegin - 1;

    // Force values so ascending branch is used
    g.Table16[AtBegin] = 0x0000;  // Val at AtBegin
    g.Table16[AtEnd]   = 0x0000;  // Val at AtEnd

    // Execute slope limiting
    SlopeLimiting(&g);

    // Validate range
    for (int i = 0; i < N; ++i) {
        if (g.Table16[i] > 0xFFFF) {
            TestFail("TestSlopeLimiting_Ascending_TailMonotonic",
                     "Value exceeded 0xFFFF saturation in ascending path", __LINE__);
            break;
        }
        if (g.Table16[i] < 0) {
            TestFail("TestSlopeLimiting_Ascending_TailMonotonic",
                     "Negative value after saturation in ascending path", __LINE__);
            break;
        }
    }

    // Tail monotonicity: nondecreasing from AtEnd to end
    for (int i = AtEnd; i < N - 1; ++i) {
        if (g.Table16[i] > g.Table16[i + 1]) {
            TestFail("TestSlopeLimiting_Ascending_TailMonotonic",
                     "Tail portion is not nondecreasing as expected for ascending curve", __LINE__);
            break;
        }
    }

    // End value verification
    if (g.Table16[N - 1] != 0xFFFF) {
        TestFail("TestSlopeLimiting_Ascending_TailMonotonic",
                 "Tail end does not reach EndVal (0xFFFF) in ascending path", __LINE__);
    }

    delete[] g.Table16;
    return g_AllTestsPassed;
}


//
// Main: run all tests and report summary
//
int main() {
    bool pass1 = TestSlopeLimiting_Descending_TailMonotonic();
    bool pass2 = TestSlopeLimiting_Ascending_TailMonotonic();

    int totalPassed = (pass1 ? 1 : 0) + (pass2 ? 1 : 0);
    int totalTests = 2;

    std::cout << "SlopeLimiting test results: " 
              << totalPassed << " / " << totalTests << " tests passed." << std::endl;

    if (!g_AllTestsPassed) {
        std::cerr << "Some tests FAILED. See details above." << std::endl;
        return 2;
    }
    return 0;
}