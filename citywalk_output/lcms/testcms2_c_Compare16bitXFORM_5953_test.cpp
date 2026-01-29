// A self-contained C++11 unit test harness for the focal function
// Compare16bitXFORM, with minimal mockups of the surrounding CMS-like API.
// This is designed to be compilable without GTest and uses a small in-file test runner.

#include <cstdio>
#include <cstdint>
#include <limits>
#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <cstdarg>
#include <cmath>


// -----------------------------------------------------------------------------
// Minimal CMS-like type and API definitions (mocked for unit tests)
// -----------------------------------------------------------------------------

using cmsInt32Number   = int;
using cmsUInt32Number  = uint32_t;
using cmsUInt16Number  = uint16_t;
using cmsBool          = int;      // 0 = False, non-zero = True
using cmsContext       = void*;    // opaque, unused in tests
using cmsHTRANSFORM    = void*;    // opaque transform handle

// Public constants (as used by the focal method)
const int cmsMAXCHANNELS = 16;

// Clamp helper for 16-bit range
static inline cmsUInt16Number clamp16(int v) {
    if (v <= 0) return 0;
    if (v >= 65535) return 65535;
    return static_cast<cmsUInt16Number>(v);
}

// -----------------------------------------------------------------------------
// Transform object and CMS DoTransform mock
// -----------------------------------------------------------------------------

// Simple transform representation: scale and bias (per-channel mapping)
struct MockTransform {
    double scale;
    double bias;
};

// Create a transform handle from a MockTransform
static cmsHTRANSFORM CreateMockTransform(double scale, double bias = 0.0) {
    MockTransform* t = new MockTransform{scale, bias};
    return static_cast<cmsHTRANSFORM>(t);
}

// Free a transform handle
static void FreeMockTransform(cmsHTRANSFORM h) {
    MockTransform* t = static_cast<MockTransform*>(h);
    delete t;
}

// The cmsDoTransform function used by Compare16bitXFORM
// Applies per-sample: Out[i] = clamp(In[i] * scale + bias, 0, 65535)
static void cmsDoTransform(cmsHTRANSFORM h,
                           const cmsUInt16Number In[],
                           cmsUInt16Number Out[],
                           cmsUInt32Number size) {
    // In this mock, we apply the transform to all channels (up to cmsMAXCHANNELS)
    const MockTransform* t = static_cast<MockTransform*>(h);
    (void)size; // unused in this simplified mock
    for (int i = 0; i < cmsMAXCHANNELS; ++i) {
        double val = static_cast<double>(In[i]) * t->scale + t->bias;
        // Clamp to 16-bit range
        int iv = static_cast<int>(std::round(val));
        Out[i] = clamp16(iv);
    }
}

// -----------------------------------------------------------------------------
// Logging and test helpers (non-terminating assertions)
// -----------------------------------------------------------------------------

static int gFailCount = 0;
static std::string gLastFailMsg;

// Variadic logging used by the focal function's Fail
static void Fail(const char* frm, ...) {
    char buffer[512];
    va_list args;
    va_start(args, frm);
    vsnprintf(buffer, sizeof(buffer), frm, args);
    va_end(args);
    gFailCount++;
    gLastFailMsg = buffer;
    // Do not terminate, allow tests to continue for coverage
}

// Die stub (not used in tests, but kept for compatibility if linked)
static void Die(const char* Reason, ...) {
    // Minimal no-op for test harness
    (void)Reason;
}

// Helper to reset test state
static void ResetTestState() {
    gFailCount = 0;
    gLastFailMsg.clear();
}

// -----------------------------------------------------------------------------
// The focal method under test (re-implemented in this harness for isolation)
// -----------------------------------------------------------------------------

// Compare16bitXFORM: compare two transforms across all 16-bit input combinations
static cmsInt32Number Compare16bitXFORM(cmsHTRANSFORM xform1, cmsHTRANSFORM xform2, cmsInt32Number nChan) {
    cmsInt32Number n2 = 0;
    cmsUInt16Number Inw[cmsMAXCHANNELS], Outw1[cmsMAXCHANNELS], Outw2[cmsMAXCHANNELS];
    // Initialize channel inputs beyond nChan to a safe value (0) to avoid UB
    for (int i = 0; i < cmsMAXCHANNELS; ++i) Inw[i] = 0;

    for (int j = 0; j < 0xFFFF; ++j) {
        // Fill first nChan inputs with the test sample
        for (int i = 0; i < nChan; ++i) Inw[i] = static_cast<cmsUInt16Number>(j);
        // Inw[i] for i >= nChan remains 0 (initialized above)

        // Transform both inputs
        cmsDoTransform(xform1, Inw, Outw1, 1);
        cmsDoTransform(xform2, Inw, Outw2, 1);

        // Compare outputs for all channels used
        for (int i = 0; i < nChan; ++i) {
            cmsInt32Number dif = static_cast<cmsInt32Number>(static_cast<int>(Outw2[i]) - static_cast<int>(Outw1[i]));
            if (dif < 0) dif = -dif;
            if (dif > n2) n2 = dif;
        }

        // Early exit optimization is not used to preserve coverage
    }

    // We allow up to 0x200 (512) difference
    if (n2 > 0x200) {
        Fail("Differences too big (%x)", static_cast<unsigned int>(n2));
        return 0;
    }

    return 1;
}

// -----------------------------------------------------------------------------
// Lightweight test harness (no GTest) - three targeted tests for coverage
// -----------------------------------------------------------------------------

// Test 1: Identical transforms should yield success (no difference)
static bool Test_IdenticalTransforms() {
    ResetTestState();
    cmsHTRANSFORM t1 = CreateMockTransform(1.0);
    cmsHTRANSFORM t2 = CreateMockTransform(1.0);

    cmsInt32Number result = Compare16bitXFORM(t1, t2, 3); // test with 3 channels
    FreeMockTransform(t1);
    FreeMockTransform(t2);

    bool ok = (result == 1) && (gFailCount == 0);
    if (!ok) {
        std::cout << "Test_IdenticalTransforms: FAILED";
        if (gFailCount > 0) std::cout << " – Fail reason: " << gLastFailMsg;
        std::cout << std::endl;
    } else {
        std::cout << "Test_IdenticalTransforms: PASSED" << std::endl;
    }
    return ok;
}

// Test 2: Near-identical transforms (small difference) should still pass
static bool Test_NearIdenticalTransforms() {
    ResetTestState();
    cmsHTRANSFORM t1 = CreateMockTransform(1.0);
    // Slightly different scale to keep max per-channel difference under threshold
    cmsHTRANSFORM t2 = CreateMockTransform(0.999); // 0.1% difference

    cmsInt32Number result = Compare16bitXFORM(t1, t2, 2); // test with 2 channels
    FreeMockTransform(t1);
    FreeMockTransform(t2);

    bool ok = (result == 1) && (gFailCount == 0);
    if (!ok) {
        std::cout << "Test_NearIdenticalTransforms: FAILED";
        if (gFailCount > 0) std::cout << " – Fail reason: " << gLastFailMsg;
        std::cout << std::endl;
    } else {
        std::cout << "Test_NearIdenticalTransforms: PASSED" << std::endl;
    }
    return ok;
}

// Test 3: Large difference should trigger the failure path (n2 > 0x200)
static bool Test_LargeDifferenceTriggersFail() {
    ResetTestState();
    cmsHTRANSFORM t1 = CreateMockTransform(1.0);
    cmsHTRANSFORM t2 = CreateMockTransform(0.5); // large difference

    cmsInt32Number result = Compare16bitXFORM(t1, t2, 2);
    FreeMockTransform(t1);
    FreeMockTransform(t2);

    bool ok = (result == 0) && (gFailCount > 0);
    if (!ok) {
        std::cout << "Test_LargeDifferenceTriggersFail: FAILED";
        std::cout << " (expected fail, got result=" << result << ", FailCount=" << gFailCount << ")";
        std::cout << std::endl;
    } else {
        std::cout << "Test_LargeDifferenceTriggersFail: PASSED" << std::endl;
    }
    return ok;
}

// Helper to run all tests
static void RunAllTests() {
    std::cout << "Running Compare16bitXFORM unit tests (no GTest)..." << std::endl;

    int passed = 0;
    int total = 0;

    if (Test_IdenticalTransforms()) ++passed;
    ++total;

    if (Test_NearIdenticalTransforms()) ++passed;
    ++total;

    if (Test_LargeDifferenceTriggersFail()) ++passed;
    ++total;

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;
}

// -----------------------------------------------------------------------------
// Main entry for non-GTest test runner
// -----------------------------------------------------------------------------

int main() {
    RunAllTests();
    return 0;
}