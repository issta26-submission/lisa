// Lightweight C++11 test harness for the focal function SpeedTest16bitsCMYK
// Note: This test suite is designed to minimize external dependencies and to
// be compilable without a full testing framework (no GTest required).

// Step 0: High-level intent
// - We exercise SpeedTest16bitsCMYK(ct, hlcmsProfileIn, hlcmsProfileOut) with two kinds of inputs:
//   1) Non-null dummy profiles to cover the normal (false) branch of the input-check.
//   2) Null profiles to cover the true-branch path (hlcmsProfileIn == NULL || hlcmsProfileOut == NULL).
// - Since the original code allocates a large buffer (128 MB for 256x256x256 16-bit CMYK pixels),
//   we provide a lightweight mock for the LittleCMS dependencies to keep the test self-contained.
// - We focus on runtime behavior (non-crashing, returning a numeric value) rather than exact transform results.
// - This test uses a minimal non-terminating assertion approach (tests continue after failures).

#include <fast_float_internal.h>
#include <iostream>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>
#include <cmath>
#include <ctime>


// Forward declare the focal function prototype (as provided by the original source)
typedef void* cmsContext;
typedef void* cmsHPROFILE;
typedef void* cmsHTRANSFORM;
typedef unsigned int cmsUInt32Number;
typedef double cmsFloat64Number;

// The actual value macros used by the focal method (kept minimal for test harness)
#define TYPE_CMYK_16 0
#define INTENT_PERCEPTUAL 0
#define cmsFLAGS_NOCACHE 0

extern "C" cmsFloat64Number SpeedTest16bitsCMYK(cmsContext ct,
                                             cmsHPROFILE hlcmsProfileIn,
                                             cmsHPROFILE hlcmsProfileOut);

// Mocked LittleCMS-like API to isolate the focal method from external dependencies
// This mock provides just enough behavior for the focal method to run and be observable.

extern "C" {

// Basic type aliases to match the focal method's usage (opaque in test harness)
typedef void* cmsContext;
typedef void* cmsHPROFILE;
typedef void* cmsHTRANSFORM;

// Lightweight stubs for LittleCMS API that are sufficient for our test
typedef unsigned int cmsUInt32Number;

static int g_transformCounter = 0;

// Create a transform handle (mock)
cmsHTRANSFORM cmsCreateTransformTHR(cmsContext /*ct*/, cmsHPROFILE /*in*/, int /*typeIn*/,
                                    cmsHPROFILE /*out*/, int /*typeOut*/, int /*intent*/, int /*flags*/) {
    // Return a non-null fake handle
    ++g_transformCounter;
    return (cmsHTRANSFORM)(void*)(&g_transformCounter);
}

// Close a profile (mock)
void cmsCloseProfile(cmsHPROFILE /*profile*/) {
    // No-op for test; profiles are dummy pointers
}

// Do a transform (mock: no actual data processing for test harness)
void cmsDoTransform(cmsHTRANSFORM /*transform*/, const void* /*In*/, void* /*Out*/, cmsUInt32Number /*size*/) {
    // Intentionally a no-op in test harness
}

// Delete a transform (mock)
void cmsDeleteTransform(cmsHTRANSFORM /*transform*/) {
    // No resources to release in mock
}

// Convert diff (mock)
cmsFloat64Number MPixSec(cmsFloat64Number diff) {
    // Pass through the diff value; tests only need a numeric result
    return diff;
}

// Global error handler (mock)
void Fail(const char* /*frm*/, ...) {
    // No-op for test harness
}
} // extern "C"

// Small, type-safe assertion macro used by tests (non-terminating)
static int g_test_failures = 0;
#define TEST_ASSERT(cond, desc) do { \
    if(!(cond)) { \
        std::cerr << "TEST FAIL: " << (desc) \
                  << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
        ++g_test_failures; \
    } \
} while(0)

// Test 1: Non-null input/output profiles
// Rationale: exercise the false branch of the input-check in SpeedTest16bitsCMYK
// and ensure the function returns a finite numeric value.
void test_SpeedTest16bitsCMYK_NonNullProfiles() {
    // Allocate a dummy 128 MB input buffer expected by the function (256^3 pixels * 8 bytes)
    // We assume each Scanline_cmyk16bits is 8 bytes (4 x uint16: c, m, y, k)
    const size_t bytesPerPixel = 8;
    const size_t pixelCount = 256u * 256u * 256u;
    const size_t allocSize = pixelCount * bytesPerPixel;

    void* dummyBuffer = std::malloc(allocSize);
    if (!dummyBuffer) {
        std::cerr << "Failed to allocate test buffer of size " << allocSize << " bytes" << std::endl;
        ++g_test_failures;
        return;
    }

    // Call the focal method with non-null (dummy) profile pointers
    cmsFloat64Number result = SpeedTest16bitsCMYK(nullptr, dummyBuffer, dummyBuffer);

    // Ensure we receive a finite numeric result
    TEST_ASSERT(std::isfinite(result), "SpeedTest16bitsCMYK should return a finite non-NaN value for non-null profiles");

    // Cleanup
    std::free(dummyBuffer);
}

// Test 2: Null input/output profiles
// Rationale: exercise the true branch of the input-check in SpeedTest16bitsCMYK.
// The mock TLS allows null profiles to proceed without crashing.
void test_SpeedTest16bitsCMYK_NullProfiles() {
    // Call the focal method with null pointers to simulate the true-branch path
    cmsFloat64Number result = SpeedTest16bitsCMYK(nullptr, nullptr, nullptr);

    // Ensure we receive a finite numeric result even when profiles are NULL
    TEST_ASSERT(std::isfinite(result), "SpeedTest16bitsCMYK should return a finite non-NaN value for NULL profiles");
}

// Main test runner
int main() {
    std::cout << "Starting SpeedTest16bitsCMYK unit tests (C++11 harness, GTest not used)" << std::endl;

    test_SpeedTest16bitsCMYK_NonNullProfiles();
    test_SpeedTest16bitsCMYK_NullProfiles();

    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_test_failures << " TEST(S) FAILED" << std::endl;
        return g_test_failures;
    }
}