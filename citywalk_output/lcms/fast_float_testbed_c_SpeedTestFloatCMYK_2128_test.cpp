// fast_float_testbed_tests.cpp
// Lightweight, self-contained unit tests for SpeedTestFloatCMYK
// using a tiny in-house test harness (no GTest).
// This test suite mocks the external color-management library functions
// to enable fast, deterministic tests while exercising the focal method.
//
// Notes:
// - The tests target the true/false branches of the NULL check for profiles.
// - We provide minimal C-linkage mocks for the required CMS API used by
//   SpeedTestFloatCMYK (cmsCreateTransformTHR, cmsCloseProfile, cmsDoTransform,
//   cmsDeleteTransform). The mocks are intentionally lightweight to keep
//   memory and runtime reasonable during tests.
// - We assume the surrounding environment provides the remaining pieces of
//   the original code (SpeedTestFloatCMYK itself and MPixSec, etc. from the
//   focal project). The test focuses on validating control flow and basic
//   interaction with the mocked CMS API.

#include <cstdint>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include <cmath>


// Forward declare the focal function so the test can call it.
// The actual implementation lives in fast_float_testbed.c and links with this test.
// We use C linkage to match the function's binary interface.
extern "C" {
    // Focal function signature used by the test
    // cmsFloat64Number SpeedTestFloatCMYK(cmsContext ct, cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut);
    // We declare it here to enable a direct call from C++ test code.
    typedef void* cmsContext;
    typedef void* cmsHPROFILE;
    typedef void* cmsHTRANSFORM;
    typedef double cmsFloat64Number;
    typedef int cmsInt32Number;
    typedef unsigned int cmsUInt32Number;

    // Opaque type for the transform in the focal code (we hold it via mock).
    extern cmsFloat64Number SpeedTestFloatCMYK(cmsContext ct, cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut);
}

// ------------------------------------------------------------------------------------
// Mocked CMS API (minimal, deterministic behavior)
// We supply C linkage so the focal code (compiled separately) can link to these.
// The mocks are deliberately small and side-effect free beyond keeping a live
// set of transforms to be cleaned up by the test.
extern "C" {

typedef struct { int dummy; } CmsDummyHandle;

// Track allocated transforms so we can clean them up (simulate cmsDeleteTransform)
static std::vector<void*> g_allocatedTransforms;

// Mock for cmsCreateTransformTHR: returns a non-null opaque handle
void* cmsCreateTransformTHR(void* ctx,
                          void* inProfile, int inType,
                          void* outProfile, int outType,
                          int intent, unsigned long flags)
{
    // Simple, deterministic fake handle
    void* h = new CmsDummyHandle{0};
    g_allocatedTransforms.push_back(h);
    return h;
}

// Mock for cmsCloseProfile: no-op in test
void cmsCloseProfile(void* profile)
{
    // Profiles are fake; nothing to close in test
    (void)profile;
}

// Mock for cmsDoTransform: pretend to transform; no-op for test efficiency
void cmsDoTransform(void* transform, const void* input, void* output, cmsUInt32Number size)
{
    // No-op; in a real test we could copy or simulate some processing.
    (void)transform;
    (void)input;
    (void)output;
    (void)size;
}

// Mock for cmsDeleteTransform: free the fake handle
void cmsDeleteTransform(void* transform)
{
    // Remove from our tracking vector if present
    if (transform) {
        // find and erase
        for (auto it = g_allocatedTransforms.begin(); it != g_allocatedTransforms.end(); ++it) {
            if (*it == transform) {
                delete reinterpret_cast<CmsDummyHandle*>(transform);
                g_allocatedTransforms.erase(it);
                break;
            }
        }
    }
}
} // extern "C"

// ------------------------------------------------------------------------------------
// Lightweight test harness

// Helper: simple assertion that does not abort on failure
#define ASSERT_TRUE(cond, msg)                                            \
    do { if(!(cond)) { std::cerr << "[ASSERT FAILED] " << (msg) << "\n"; } } \
    while(false)

// Test 1: Non-null input and output profiles should execute the full path (to the extent mocked)
bool test_SpeedTestFloatCMYK_NonNullProfiles() {
    // Prepare fake non-null profiles (we pass non-null pointers as required)
    void* fakeCtx = nullptr;
    void* fakeInProfile  = (void*)0xDEADBEEF;
    void* fakeOutProfile = (void*)0xFEEDBEEF;

    // Call the focal function
    cmsFloat64Number result = SpeedTestFloatCMYK(reinterpret_cast<cmsContext>(fakeCtx),
                                                reinterpret_cast<cmsHPROFILE>(fakeInProfile),
                                                reinterpret_cast<cmsHPROFILE>(fakeOutProfile));

    // We expect a finite numeric result; the exact value is implementation-defined (depends on MPixSec and clock)
    bool ok = std::isfinite(static_cast<double>(result));
    if (!ok) {
        std::cerr << "SpeedTestFloatCMYK returned non-finite value for non-null profiles.\n";
    }
    return ok;
}

// Test 2: NULL input profile should trigger the true branch (Fail) and still complete without crash
bool test_SpeedTestFloatCMYK_NullProfiles() {
    // Pass NULL for input profile and a valid non-null output profile
    void* fakeCtx = nullptr;
    void* fakeInProfile  = nullptr;
    void* fakeOutProfile = (void*)0xBADC0DE;

    cmsFloat64Number result = SpeedTestFloatCMYK(reinterpret_cast<cmsContext>(fakeCtx),
                                                reinterpret_cast<cmsHPROFILE>(fakeInProfile),
                                                reinterpret_cast<cmsHPROFILE>(fakeOutProfile));

    // Expect a finite numeric result as the function handles the NULL case gracefully in mocks
    bool ok = std::isfinite(static_cast<double>(result));
    if (!ok) {
        std::cerr << "SpeedTestFloatCMYK returned non-finite value for NULL input profile (should handle gracefully).\n";
    }
    return ok;
}

// Main test runner
int main() {
    std::cout << "Running SpeedTestFloatCMYK unit tests (mocked CMS environment)..." << std::endl;

    bool t1 = test_SpeedTestFloatCMYK_NonNullProfiles();
    bool t2 = test_SpeedTestFloatCMYK_NullProfiles();

    if (t1) std::cout << "[PASS] Non-null profile test" << std::endl;
    else     std::cout << "[FAIL] Non-null profile test" << std::endl;

    if (t2) std::cout << "[PASS] Null profile test" << std::endl;
    else     std::cout << "[FAIL] Null profile test" << std::endl;

    int overall = (t1 && t2) ? 0 : 1;
    // Cleanup any remaining transforms
    for (void* t : g_allocatedTransforms) {
        delete reinterpret_cast<CmsDummyHandle*>(t);
    }
    g_allocatedTransforms.clear();

    return overall;
}