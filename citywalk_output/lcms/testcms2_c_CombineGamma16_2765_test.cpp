// test_CombineGamma16.cpp
// Purpose: Validate the behavior of CombineGamma16(g1, g2) by exercising
//          expected tabulated results for various dependent curves.
// Notes:
// - This test uses only the C++ standard library and the public CMS/LittleCMS API.
// - We do not rely on a testing framework; a small inline harness is provided.
// - All tests assume identity mapping for g1 where advantageous to simplify expectations.

#include <iostream>
#include <testcms2.h>
#include <cassert>
#include <lcms2.h>
#include <cstdint>


// Include LittleCMS core headers and test harness helpers.
// The provided focal method depends on cmsToneCurve*, cmsEvalToneCurve16, cmsBuildTabulatedToneCurve16,
// and _cmsQuantizeVal (via the internal quantization that CombineGamma16 uses).
// We will replicate the quantization behavior locally for test expectations.


// Helper: compute the 16-bit quantized value for i in [0, N-1] with N=256.
// This emulates _cmsQuantizeVal(i, 256) from LittleCMS (scaling 0..255 to 0..65535).
static cmsUInt16Number QuantizeVal256(int i) {
    // Using the standard formula: floor(i * 65535 / (N-1)) where N-1 = 255
    return static_cast<cmsUInt16Number>((static_cast<uint64_t>(i) * 65535ULL) / 255ULL);
}


// Helper: construct an identity tone curve (256-entry) where Tab[i] maps i -> i*65535/255.
static cmsToneCurve* BuildIdentityToneCurve16() {
    cmsUInt16Number Tab[256];
    for (int i = 0; i < 256; ++i) {
        Tab[i] = QuantizeVal256(i);
    }
    return cmsBuildTabulatedToneCurve16(DbgThread(), 256, Tab);
}


// Helper: free a tone curve if non-null.
static void SafeFreeToneCurve(cmsToneCurve* tc) {
    if (tc) cmsFreeToneCurve(tc);
}


// Test 1: Identity g1 and g2 as inverse mapping (g2(x) = 65535 - x after identity g1).
// Expected: For each wValIn = QuantizeVal256(i), the resulting combined value equals
//           cmsEvalToneCurve16(g2, wValIn) due to g1 being identity.
static bool Test_CombineGamma16_IdentityInverse(void) {
    // Build g1: identity
    cmsToneCurve* g1 = BuildIdentityToneCurve16();
    // Build g2: inverse mapping of identity
    cmsUInt16Number Tab2[256];
    for (int i = 0; i < 256; ++i) {
        cmsUInt16Number wValIn = QuantizeVal256(i);
        Tab2[i] = static_cast<cmsUInt16Number>(65535U - wValIn);
    }
    cmsToneCurve* g2 = cmsBuildTabulatedToneCurve16(DbgThread(), 256, Tab2);

    // Combine
    cmsToneCurve* res = CombineGamma16(g1, g2);

    bool ok = true;
    for (int i = 0; i < 256; ++i) {
        cmsUInt16Number wValIn = QuantizeVal256(i);
        cmsUInt16Number expectedOut = cmsEvalToneCurve16(g2, wValIn);   // since g1 is identity
        cmsUInt16Number actualOut   = cmsEvalToneCurve16(res, wValIn);

        if (actualOut != expectedOut) {
            std::cerr << "[Test IdentityInverse] Mismatch at i=" << i
                      << ": expected " << expectedOut << ", got " << actualOut << "\n";
            ok = false;
            break;
        }
    }

    SafeFreeToneCurve(g1);
    SafeFreeToneCurve(g2);
    SafeFreeToneCurve(res);
    return ok;
}


// Test 2: Identity g1 and g2 constant (g2 maps all inputs to a fixed 32768).
// Expected: For any wValIn, combined output should be 32768.
static bool Test_CombineGamma16_IdentityConstant(void) {
    // g1: identity
    cmsToneCurve* g1 = BuildIdentityToneCurve16();

    // g2: constant 32768
    cmsUInt16Number Tab2[256];
    for (int i = 0; i < 256; ++i) {
        Tab2[i] = 32768;
    }
    cmsToneCurve* g2 = cmsBuildTabulatedToneCurve16(DbgThread(), 256, Tab2);

    // Combine
    cmsToneCurve* res = CombineGamma16(g1, g2);

    bool ok = true;
    for (int i = 0; i < 256; ++i) {
        cmsUInt16Number wValIn = QuantizeVal256(i);
        cmsUInt16Number expectedOut = 32768;
        cmsUInt16Number actualOut   = cmsEvalToneCurve16(res, wValIn);

        if (actualOut != expectedOut) {
            std::cerr << "[Test IdentityConstant] Mismatch at i=" << i
                      << ": expected " << expectedOut << ", got " << actualOut << "\n";
            ok = false;
            break;
        }
    }

    SafeFreeToneCurve(g1);
    SafeFreeToneCurve(g2);
    SafeFreeToneCurve(res);
    return ok;
}


// Test 3: Identity g1 and g2 monotonic non-linear mapping (g2[i] = (i*i*65535)/(255*255)).
// This exercises non-linear behavior of cmsEvalToneCurve16 and validates propagation
// through CombineGamma16 via g1(g2(...)) composition when g1 is identity.
static bool Test_CombineGamma16_IdentityNonLinear(void) {
    // g1: identity
    cmsToneCurve* g1 = BuildIdentityToneCurve16();

    // g2: non-linear monotonic mapping: val = (i*i * 65535) / (255*255)
    cmsUInt16Number Tab2[256];
    const uint64_t denom = 255ULL * 255ULL;
    for (int i = 0; i < 256; ++i) {
        uint64_t t = static_cast<uint64_t>(i);
        uint64_t val = (t * t * 65535ULL) / denom;
        if (val > 65535ULL) val = 65535ULL;
        Tab2[i] = static_cast<cmsUInt16Number>(val);
    }
    cmsToneCurve* g2 = cmsBuildTabulatedToneCurve16(DbgThread(), 256, Tab2);

    // Combine
    cmsToneCurve* res = CombineGamma16(g1, g2);

    bool ok = true;
    for (int i = 0; i < 256; ++i) {
        cmsUInt16Number wValIn = QuantizeVal256(i);
        cmsUInt16Number expectedOut = cmsEvalToneCurve16(g2, wValIn);
        cmsUInt16Number actualOut   = cmsEvalToneCurve16(res, wValIn);

        if (actualOut != expectedOut) {
            std::cerr << "[Test IdentityNonLinear] Mismatch at i=" << i
                      << ": expected " << expectedOut << ", got " << actualOut << "\n";
            ok = false;
            break;
        }
    }

    SafeFreeToneCurve(g1);
    SafeFreeToneCurve(g2);
    SafeFreeToneCurve(res);
    return ok;
}


// Simple test harness
int main() {
    int failures = 0;

    std::cout << "Running CombineGamma16 test suite (C++11, no GTest)...\n";

    if (!Test_CombineGamma16_IdentityInverse()) {
        std::cerr << "Test 1 (Identity + Inverse) FAILED\n";
        ++failures;
    } else {
        std::cout << "Test 1 (Identity + Inverse) PASSED\n";
    }

    if (!Test_CombineGamma16_IdentityConstant()) {
        std::cerr << "Test 2 (Identity + Constant) FAILED\n";
        ++failures;
    } else {
        std::cout << "Test 2 (Identity + Constant) PASSED\n";
    }

    if (!Test_CombineGamma16_IdentityNonLinear()) {
        std::cerr << "Test 3 (Identity + NonLinear) FAILED\n";
        ++failures;
    } else {
        std::cout << "Test 3 (Identity + NonLinear) PASSED\n";
    }

    if (failures == 0) {
        std::cout << "All CombineGamma16 tests PASSED.\n";
        return 0;
    } else {
        std::cerr << failures << " test(s) FAILED.\n";
        return 1;
    }
}