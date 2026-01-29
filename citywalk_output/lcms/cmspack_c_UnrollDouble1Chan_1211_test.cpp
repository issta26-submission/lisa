// Test suite for UnrollDouble1Chan in cmspack.c
// This test suite is crafted for C++11, without GoogleTest.
// It uses a lightweight, non-terminating assertion approach to maximize coverage.

#include <cstring>
#include <cstdio>
#include <cstdint>
#include <lcms2_internal.h>
#include <cmath>


// Minimal type aliases to match the signature in cms code.
// We avoid pulling in the full lcms headers to keep the test self-contained.
typedef unsigned char      cmsUInt8Number;
typedef unsigned short     cmsUInt16Number;
typedef unsigned int       cmsUInt32Number;
typedef double             cmsFloat64Number;
typedef float              cmsFloat32Number;

// Forward declaration of the opaque _cmsTRANSFORM type used in the focal method.
// We only need an incomplete type to pass to the function; the function does not
// access its internals for this particular test.
typedef struct _cmsTRANSFORM _cmsTRANSFORM;

// Ensure C linkage for the focal function (it's implemented in C).
extern "C" {
    // Prototype of the focal function under test.
    // The exact macro CMSREGISTER is typically defined to nothing in test builds.
    cmsUInt8Number* UnrollDouble1Chan(_cmsTRANSFORM* info,
                                    cmsUInt16Number wIn[],
                                    cmsUInt8Number* accum,
                                    cmsUInt32Number Stride);
}

// Lightweight non-terminating assertion helper
static void logTestFailure(const char* testName, const char* message) {
    std::fprintf(stderr, "Test '%s' FAILED: %s\n", testName, message);
}

// Helper macro to perform a test assertion that doesn't abort the test on failure.
// Returns true if the condition holds, false otherwise; logs a message on failure.
#define ASSERT_TRUE(cond, testName, message) \
    do { \
        if(!(cond)) { logTestFailure(testName, message); return false; } \
    } while(0)


// Test 1: Inks = 0.0  -> wIn[0..2] should be 0, and pointer should advance by 8 bytes.
static bool test_UnrollDouble1Chan_BasicZero() {
    const char* testName = "UnrollDouble1Chan_BasicZero";
    _cmsTRANSFORM dummy;
    // Allocate enough accum space: 16 bytes (enough for 8-byte double + extra)
    cmsUInt8Number* accum = new cmsUInt8Number[16];
    // Place Inks[0] = 0.0 in the accum space
    cmsFloat64Number* Inks = reinterpret_cast<cmsFloat64Number*>(accum);
    Inks[0] = 0.0;
    // Prepare input
    cmsUInt16Number wIn[3] = {12345, 12345, 12345}; // arbitrary initial values
    cmsUInt32Number Stride = 0;

    cmsUInt8Number* res = UnrollDouble1Chan(&_cmsTRANSFORM{}, wIn, accum, Stride);

    // Expected: wIn all zeros (since 0.0 * 65535 = 0)
    bool ok = true;
    ok = ok && (wIn[0] == 0 && wIn[1] == 0 && wIn[2] == 0);
    // Expected: return pointer equals accum + sizeof(cmsFloat64Number) == accum + 8
    ok = ok && (res == accum + sizeof(cmsFloat64Number));

    delete[] accum;
    if(!ok) logTestFailure(testName, "Incorrect saturation or pointer arithmetic for Inks=0.0.");
    return ok;
}

// Test 2: Inks = 1.0  -> wIn[0..2] should be 65535, and pointer should advance by 8 bytes.
static bool test_UnrollDouble1Chan_BasicOne() {
    const char* testName = "UnrollDouble1Chan_BasicOne";
    _cmsTRANSFORM dummy;
    cmsUInt8Number* accum = new cmsUInt8Number[16];
    cmsFloat64Number* Inks = reinterpret_cast<cmsFloat64Number*>(accum);
    Inks[0] = 1.0;
    cmsUInt16Number wIn[3] = {0, 0, 0};
    cmsUInt32Number Stride = 0;

    cmsUInt8Number* res = UnrollDouble1Chan(&_cmsTRANSFORM{}, wIn, accum, Stride);

    // Expected: wIn all 65535
    bool ok = true;
    ok = ok && (wIn[0] == 65535 && wIn[1] == 65535 && wIn[2] == 65535);
    ok = ok && (res == accum + sizeof(cmsFloat64Number));

    delete[] accum;
    if(!ok) logTestFailure(testName, "Incorrect saturation for Inks=1.0 or incorrect return pointer.");
    return ok;
}

// Test 3: Inks = -1.0  -> wIn[0..2] should be 0 (saturation), pointer advance.
static bool test_UnrollDouble1Chan_BasicNegative() {
    const char* testName = "UnrollDouble1Chan_BasicNegative";
    _cmsTRANSFORM dummy;
    cmsUInt8Number* accum = new cmsUInt8Number[16];
    cmsFloat64Number* Inks = reinterpret_cast<cmsFloat64Number*>(accum);
    Inks[0] = -1.0;
    cmsUInt16Number wIn[3] = {1, 1, 1}; // start with non-zero values to ensure overwrite
    cmsUInt32Number Stride = 0;

    cmsUInt8Number* res = UnrollDouble1Chan(&_cmsTRANSFORM{}, wIn, accum, Stride);

    bool ok = true;
    ok = ok && (wIn[0] == 0 && wIn[1] == 0 && wIn[2] == 0);
    ok = ok && (res == accum + sizeof(cmsFloat64Number));

    delete[] accum;
    if(!ok) logTestFailure(testName, "Incorrect saturation for Inks=-1.0 or incorrect return pointer.");
    return ok;
}

// Test 4: Inks = 1.1  -> wIn[0..2] should saturate to 65535 (above 1.0), pointer advance.
static bool test_UnrollDouble1Chan_BasicOverOne() {
    const char* testName = "UnrollDouble1Chan_BasicOverOne";
    _cmsTRANSFORM dummy;
    cmsUInt8Number* accum = new cmsUInt8Number[16];
    cmsFloat64Number* Inks = reinterpret_cast<cmsFloat64Number*>(accum);
    Inks[0] = 1.1; // > 1.0, should saturate to max
    cmsUInt16Number wIn[3] = {0, 0, 0};
    cmsUInt32Number Stride = 0;

    cmsUInt8Number* res = UnrollDouble1Chan(&_cmsTRANSFORM{}, wIn, accum, Stride);

    bool ok = true;
    ok = ok && (wIn[0] == 65535 && wIn[1] == 65535 && wIn[2] == 65535);
    ok = ok && (res == accum + sizeof(cmsFloat64Number));

    delete[] accum;
    if(!ok) logTestFailure(testName, "Incorrect saturation for Inks>1.0 or incorrect return pointer.");
    return ok;
}

// Test 5: Verify pointer arithmetic is exactly 8 bytes ahead (size of cmsFloat64Number)
static bool test_UnrollDouble1Chan_PointerArithmeticConsistency() {
    const char* testName = "UnrollDouble1Chan_PointerArithmeticConsistency";
    _cmsTRANSFORM dummy;
    cmsUInt8Number* accum = new cmsUInt8Number[24];
    cmsFloat64Number* Inks = reinterpret_cast<cmsFloat64Number*>(accum);
    Inks[0] = 0.25; // not used for exact expected value, we only check pointer offset here

    cmsUInt16Number wIn[3] = {0, 0, 0};
    cmsUInt32Number Stride = 0;

    cmsUInt8Number* res = UnrollDouble1Chan(&_cmsTRANSFORM{}, wIn, accum, Stride);

    bool ok = true;
    // Check pointer advancement
    ok = ok && (res == accum + sizeof(cmsFloat64Number));

    delete[] accum;
    if(!ok) logTestFailure(testName, "Pointer did not advance by size of cmsFloat64Number as expected.");
    return ok;
}


// Main function to run all tests
int main() {
    int totalTests = 0;
    int failedTests = 0;

    auto run = [&](bool result, const char* name){
        totalTests++;
        if(!result) {
            failedTests++;
        } else {
            // Optional: print passing messages for verbose feedback
            // std::printf("Test '%s' PASSED\n", name);
        }
    };

    // Run all tests
    run(test_UnrollDouble1Chan_BasicZero(), "UnrollDouble1Chan_BasicZero");
    run(test_UnrollDouble1Chan_BasicOne(),  "UnrollDouble1Chan_BasicOne");
    run(test_UnrollDouble1Chan_BasicNegative(),"UnrollDouble1Chan_BasicNegative");
    run(test_UnrollDouble1Chan_BasicOverOne(),"UnrollDouble1Chan_BasicOverOne");
    run(test_UnrollDouble1Chan_PointerArithmeticConsistency(),"UnrollDouble1Chan_PointerArithmeticConsistency");

    std::printf("Total tests: %d, Failed: %d\n", totalTests, failedTests);
    return failedTests;
}