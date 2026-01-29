// Unit test suite for SpeedTest16bitsRGB in fast_float_testbed.c
// Target: C++11, no GTest. Tests are implemented with simple asserts and
// a C wrapper layer to stub external LittleCMS-like dependencies.
// The test suite uses non-terminating behavior where possible and captures
// both true/false branches of critical predicates.

#include <cstring>
#include <cstdio>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>
#include <cstdarg>
#include <cassert>
#include <memory.h>
#include <stdlib.h>
#include <ctime>


// Provide C-style external interface expected by the focal function.
// We re-declare essential types and functions used by SpeedTest16bitsRGB.

extern "C" {

// Basic type aliases (simplified stand-ins for the actual library types)
typedef void* cmsContext;
typedef void* cmsHPROFILE;
typedef void* cmsHTRANSFORM;
typedef unsigned int cmsUInt32Number;
typedef unsigned short cmsUInt16Number;
typedef double cmsFloat64Number;

// Pixel scanline structure used by the 16-bit RGB path
typedef struct {
    cmsUInt16Number r;
    cmsUInt16Number g;
    cmsUInt16Number b;
} Scanline_rgb16bits;

// Macros as used by the focal method
#define TYPE_RGB_16 0
#define INTENT_PERCEPTUAL 0
#define cmsFLAGS_NOCACHE 0

// Forward declarations of the external "C" API used by SpeedTest16bitsRGB
typedef cmsHTRANSFORM (*F)();
extern "C" cmsHTRANSFORM cmsCreateTransformTHR(
        cmsContext ContextID,
        cmsHPROFILE InputProfile,
        cmsUInt32Number InputFormat,
        cmsHPROFILE OutputProfile,
        cmsUInt32Number OutputFormat,
        cmsUInt32Number Intent,
        cmsUInt32Number Flags);

extern "C" void cmsCloseProfile(cmsHPROFILE h);
extern "C" void cmsDeleteTransform(cmsHTRANSFORM xform);
extern "C" void cmsDoTransform(cmsHTRANSFORM xform,
                             const void* In,
                             void* Out,
                             cmsUInt32Number SampleCount);

// MPixSec converter (stub)
extern "C" cmsFloat64Number MPixSec(cmsFloat64Number diff);

// Failure reporter used in the focal function
extern "C" void Fail(const char* frm, ...);

// The focal function itself (declaration for linking)
extern "C" cmsFloat64Number SpeedTest16bitsRGB(cmsContext ct,
                                             cmsHPROFILE hlcmsProfileIn,
                                             cmsHPROFILE hlcmsProfileOut);

} // extern "C"

// Globals to inspect internal behavior from tests
extern "C" int g_dotransform_calls = 0;

// Stubbed implementation details to emulate the environment of the focal code

// Memory-friendly, fast stub of the transform operation.
// We'll copy input to output using memcpy to simulate a transform.
// Also increment a global counter to indicate the transform was invoked.
extern "C" void cmsDoTransform(cmsHTRANSFORM /*xform*/,
                               const void* In,
                               void* Out,
                               cmsUInt32Number SampleCount)
{
    if (In == nullptr || Out == nullptr || SampleCount == 0) return;
    // Cast to the known struct for copying
    const Scanline_rgb16bits* src = reinterpret_cast<const Scanline_rgb16bits*>(In);
    Scanline_rgb16bits* dst = reinterpret_cast<Scanline_rgb16bits*>(Out);
    size_t bytes = static_cast<size_t>(SampleCount) * sizeof(Scanline_rgb16bits);
    std::memcpy(dst, src, bytes);
    // Increment invocation counter for verification in tests
    g_dotransform_calls++;
}

// Allocate/return a non-null transform handle (stub)
extern "C" cmsHTRANSFORM cmsCreateTransformTHR(
    cmsContext /*ContextID*/,
    cmsHPROFILE /*InputProfile*/,
    cmsUInt32Number /*InputFormat*/,
    cmsHPROFILE /*OutputProfile*/,
    cmsUInt32Number /*OutputFormat*/,
    cmsUInt32Number /*Intent*/,
    cmsUInt32Number /*Flags*/)
{
    // Return a dummy non-null handle
    return reinterpret_cast<cmsHTRANSFORM>(0x1);
}

// Stubbed profile close and transform delete (no-op)
extern "C" void cmsCloseProfile(cmsHPROFILE /*h*/) { /* no-op in test stub */ }
extern "C" void cmsDeleteTransform(cmsHTRANSFORM /*xform*/) { /* no-op in test stub */ }

// Simple clock-based performance helper (non-precise for testing)
extern "C" cmsFloat64Number MPixSec(cmsFloat64Number diff)
{
    // Convert clock ticks to a small, positive double value for testing.
    // If diff is zero/negative, return zero to avoid NaNs.
    if (diff <= 0.0) return 0.0;
    // Heuristic factor chosen for test stability (not used for assertions)
    return diff * 1e-6;
}

// Fail needs to propagate to tests to verify error branches.
// Implementing a variadic reporter that throws to allow test capture.

static std::string vformat(const char* fmt, va_list args) {
    va_list args_copy;
    va_copy(args_copy, args);
    int needed = vsnprintf(nullptr, 0, fmt, args_copy);
    va_end(args_copy);
    if (needed <= 0) return "";
    std::vector<char> buf(needed + 1);
    vsnprintf(buf.data(), buf.size(), fmt, args);
    return std::string(buf.data(), buf.data() + needed);
}

extern "C" void Fail(const char* frm, ...)
{
    va_list args;
    va_start(args, frm);
    std::string message = vformat(frm, args);
    va_end(args);
    throw std::runtime_error("Fail: " + message);
}

// Test helpers: namespace-less tests using simple asserts

// Test 1: Normal operation path (non-null profiles) should not throw and should invoke transform.
void test_SpeedTest16bitsRGB_NormalPath()
{
    // Arrange
    void* inProfile  = reinterpret_cast<void*>(0x100);
    void* outProfile = reinterpret_cast<void*>(0x200);
    // Reset transform counter
    g_dotransform_calls = 0;

    // Act
    cmsFloat64Number result = 0.0;
    try {
        result = SpeedTest16bitsRGB(nullptr, inProfile, outProfile);
    } catch (...) {
        // If any exception escapes, the test should fail
        assert(false && "SpeedTest16bitsRGB threw on valid inputs");
    }

    // Assert
    // Verify that the transform was performed (our stub increments this counter)
    assert(g_dotransform_calls > 0 && "cmsDoTransform should have been invoked");
    // Result should be a finite non-negative value (MPixSec returns non-negative)
    assert(result >= 0.0 && "SpeedTest16bitsRGB should return non-negative value");
    // Optional: ensure result is a finite number
    if (!std::isfinite(static_cast<double>(result))) {
        assert(false && "SpeedTest16bitsRGB returned non-finite value");
    }

    // Print diagnostic (optional)
    std::cout << "test_SpeedTest16bitsRGB_NormalPath passed. MPixSec=" << static_cast<double>(result)
              << ", transform_calls=" << g_dotransform_calls << "\n";
}

// Test 2: Null input profile should trigger Fail() path (exception thrown).
void test_SpeedTest16bitsRGB_NullInputProfileTriggersFail()
{
    // Arrange
    void* inProfile = nullptr;
    void* outProfile = reinterpret_cast<void*>(0x200);

    // Act & Assert
    bool caught = false;
    try {
        SpeedTest16bitsRGB(nullptr, inProfile, outProfile);
    } catch (const std::exception& e) {
        caught = true;
        // Optional: verify error message content
        std::string msg = e.what();
        (void)msg; // suppress unused if not checked
    }
    assert(caught && "SpeedTest16bitsRGB should throw when input profile is NULL");

    std::cout << "test_SpeedTest16bitsRGB_NullInputProfileTriggersFail passed.\n";
}

// Test 3: Null output profile should trigger Fail() path (exception thrown).
void test_SpeedTest16bitsRGB_NullOutputProfileTriggersFail()
{
    // Arrange
    void* inProfile = reinterpret_cast<void*>(0x100);
    void* outProfile = nullptr;

    // Act & Assert
    bool caught = false;
    try {
        SpeedTest16bitsRGB(nullptr, inProfile, outProfile);
    } catch (const std::exception& e) {
        caught = true;
        (void)e;
    }
    assert(caught && "SpeedTest16bitsRGB should throw when output profile is NULL");

    std::cout << "test_SpeedTest16bitsRGB_NullOutputProfileTriggersFail passed.\n";
}

// Main entry to run tests
int main()
{
    // Run tests in a deterministic order
    try {
        test_SpeedTest16bitsRGB_NormalPath();
        test_SpeedTest16bitsRGB_NullInputProfileTriggersFail();
        test_SpeedTest16bitsRGB_NullOutputProfileTriggersFail();
    } catch (const std::exception& e) {
        std::cerr << "A test failed with exception: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << "A test failed with an unknown exception\n";
        return 1;
    }

    std::cout << "All tests passed.\n";
    return 0;
}