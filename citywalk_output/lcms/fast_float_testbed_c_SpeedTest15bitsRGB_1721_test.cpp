/*
Unit Test Suite for SpeedTest15bitsRGB (focal method)

Step 1 - Program Understanding and Candidate Keywords
- Core function under test: cmsFloat64Number SpeedTest15bitsRGB(cmsContext ct, cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut)
- Key local/dependent elements inside the focal method:
  - r, g, b, j: loop counters for 256x256x256 RGB space
  - atime, diff: timing metrics
  - hlcmsxform: transformation handle created by cmsCreateTransformTHR
  - In: input buffer of Scanline_rgb15bits containing r,g,b
  - Mb: size of the input buffer
  - Check for NULL profiles and early exit via Fail
  - cmsCloseProfile to release input/output profiles
  - cmsDoTransform to perform the color transform
  - cmsDeleteTransform to release the transform
  - MPixSec(diff): converts clock diff to megapixels per second
- Important related calls and types (derived from the code):
  - cmsCreateTransformTHR, cmsCloseProfile, cmsDoTransform, cmsDeleteTransform
  - TYPE_RGB_15, INTENT_PERCEPTUAL, cmsFLAGS_NOCACHE
  - 256^3 (16,777,216) pixel processing
  - Scanline_rgb15bits memory layout
  - The function uses clock() timing (diff) and allocates/free a large buffer
- Related dependencies observed in <FOCAL_CLASS_DEP>:
  - FatalErrorQuit, Fail, loadProfile, MPixSec, trace (stubs present)
  - The code uses internal helper/mocking infrastructure (empty stubs in the test header)

Candidate Keywords to drive test design
- cmsFloat64Number, SpeedTest15bitsRGB, cmsContext, cmsHPROFILE
- hlcmsProfileIn, hlcmsProfileOut, hlcmsxform
- Scanline_rgb15bits, In, Mb, r, g, b, j, 256, 256, 256
- cmsCreateTransformTHR, cmsCloseProfile, cmsDoTransform, cmsDeleteTransform
- TYPE_RGB_15, INTENT_PERCEPTUAL, cmsFLAGS_NOCACHE
- MPixSec, clock, atime, diff

Step 2 - Unit Test Generation (without GTest)
- We provide a lightweight, self-contained C++11 test harness (no GTest) that:
  - Declares the focal function SpeedTest15bitsRGB with C linkage
  - Exercises a few representative scenarios using non-terminating assertions
  - Avoids any private methods/fields (as required by the domain knowledge)
- Test design notes:
  - Test 1: Normal path with non-null in/out profiles (best-effort sanity check that function returns a finite value)
  - Test 2: Null input profile (to exercise early predicate path)
  - Test 3: Null output profile (to exercise early predicate path)
- Constraints acknowledged:
  - The real function internally creates a transform and processes 16M pixels; running this in a unit test can be heavy. The tests rely on the actual implementation to behave gracefully for null and non-null inputs, but if the underlying library crashes on invalid inputs, these tests may require an environment where the dependent library is available and sane. The tests are written to be non-terminating (they don’t exit on first failure) and report failures via a counter.

Step 3 - Test Case Refinement
- The tests use a small, self-contained assertion mechanism and a straightforward main() harness to maximize code coverage without relying on external test frameworks.
- They purposefully avoid private/static internals and focus on the public focal method behavior and boundary cases.
- They are designed to be portable to standard C++11 environments and rely only on standard library facilities for assertions and I/O.

Code (C++11 test harness)

*/

// Lightweight test harness for SpeedTest15bitsRGB without GTest
#include <cstdio>
#include <fast_float_internal.h>
#include <cstdint>
#include <cstddef>
#include <memory.h>
#include <stdlib.h>
#include <cmath>


// Forward declaration of the focal function (C linkage)
extern "C" {
    // The actual signature from the focal code:
    // cmsFloat64Number SpeedTest15bitsRGB(cmsContext ct, cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut);
    // We declare it here so the test can link against the real implementation if provided in the build.
    typedef void* cmsContext;
    typedef void* cmsHPROFILE;
    typedef double cmsFloat64Number;
    cmsFloat64Number SpeedTest15bitsRGB(cmsContext ct, cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut);
}

// Simple non-terminating assertion macro
static int g_failures = 0;
#define ASSERT(cond, msg) do { if(!(cond)) { \
    fprintf(stderr, "Assertion failed: %s\n", msg); \
    ++g_failures; \
} } while(0)

// Helper to indicate test name
#define TEST_NAME(name) printf("Running test: %s\n", name)

// Test 1: Normal path with non-null profiles. Expect a finite non-negative result.
// This test relies on the underlying implementation being available and sane.
// It is designed to be non-destructive and non-terminating on failure.
void test_SpeedTest15bitsRGB_NormalPath() {
    TEST_NAME("NormalPath: non-null in/out profiles");
    // Create placeholder non-null profile handles.
    // In a real environment these would be valid cmsHPROFILE handles created via the library.
    cmsHPROFILE inProfile  = reinterpret_cast<cmsHPROFILE>(0x1);
    cmsHPROFILE outProfile = reinterpret_cast<cmsHPROFILE>(0x2);
    cmsContext ct = nullptr;

    cmsFloat64Number res = SpeedTest15bitsRGB(ct, inProfile, outProfile);

    // We expect a finite numeric result; it could be large due to the heavy workload,
    // but it should be a finite double (not NaN or Inf) if the implementation is sane.
    ASSERT(!std::isnan(res) && std::isfinite(res) && res >= 0.0,
           "NormalPath: SpeedTest15bitsRGB returned non-finite value");
}

// Test 2: Null input profile triggers early predicate path in focal method.
// This test checks that the function handles a NULL hlcmsProfileIn gracefully
// (the actual behavior depends on the implementation; we only verify that it does not crash here).
void test_SpeedTest15bitsRGB_NullInputProfile() {
    TEST_NAME("NullInputProfile: hlcmsProfileIn is NULL");
    cmsHPROFILE inProfile = nullptr;
    cmsHPROFILE outProfile = reinterpret_cast<cmsHPROFILE>(0x3);
    cmsContext ct = nullptr;

    // Expect the function to return some value; in real code it might Fail and proceed differently.
    // We conservatively just call and ensure the result is a finite number when possible.
    cmsFloat64Number res = SpeedTest15bitsRGB(ct, inProfile, outProfile);

    // If the implementation handles NULL gracefully, res should still be finite.
    // If it crashes, the test run will abort; we simply record a failure if result is NaN/Inf.
    ASSERT(!std::isnan(res) && std::isfinite(res) && res >= 0.0,
           "NullInputProfile: SpeedTest15bitsRGB returned non-finite value or crashed");
}

// Test 3: Null output profile triggers early predicate path in focal method.
// Similar reasoning to Test 2, but hlcmsProfileOut is NULL.
void test_SpeedTest15bitsRGB_NullOutputProfile() {
    TEST_NAME("NullOutputProfile: hlcmsProfileOut is NULL");
    cmsHPROFILE inProfile = reinterpret_cast<cmsHPROFILE>(0x4);
    cmsHPROFILE outProfile = nullptr;
    cmsContext ct = nullptr;

    cmsFloat64Number res = SpeedTest15bitsRGB(ct, inProfile, outProfile);

    ASSERT(!std::isnan(res) && std::isfinite(res) && res >= 0.0,
           "NullOutputProfile: SpeedTest15bitsRGB returned non-finite value or crashed");
}

int main() {
    // Run tests
    test_SpeedTest15bitsRGB_NormalPath();
    test_SpeedTest15bitsRGB_NullInputProfile();
    test_SpeedTest15bitsRGB_NullOutputProfile();

    if (g_failures == 0) {
        printf("All tests passed (no assertion failures).\n");
        return 0;
    } else {
        fprintf(stderr, "Total failures: %d\n", g_failures);
        return 1;
    }
}

/*
Notes for future refinement (Step 3):
- If an environment provides proper ICC profile handling, it would be ideal to instantiate real cmsHPROFILE handles via the library (e.g., loadProfile function shown in dependencies) and verify behavior with valid inputs and various intents.
- To improve coverage further, add tests that exercise:
  - Correct resource management: ensure cmsCloseProfile and cmsDeleteTransform are invoked without leaks (requires instrumentation of the mocks).
  - Performance path (though expensive): ensure the function returns within a reasonable time bound for small synthetic workloads (requires modification to the focal function or a test-only build flag to reduce Mb).
  - Boundary checks for r/g/b loops (ideally by substituting a smaller dataset or using a test hook to limit iterations).
- Static function coverage (in the real file) would require integration tests within the same translation unit or a build configuration that exposes these statics via test scaffolding; the current test avoids touching private scope per domain knowledge.
*/