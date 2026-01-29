/*
   Lightweight C++11 test suite for the focal method:
   From16ToFloat(const cmsUInt16Number In[], cmsFloat32Number Out[], cmsUInt32Number n)

   Purpose:
   - Verify proper conversion of 16-bit unsigned integers in [0, 65535] to
     single-precision float in [0.0, 1.0] by dividing by 65535.0f.
   - Validate edge cases (min, max), midpoints, mixed sequences, and zero-length input.

   Notes:
   - This test uses a minimal, self-contained test harness (no Google Test).
   - We assume the build environment provides cmslut.c (which contains From16ToFloat)
     and the internal types via lcms2_internal.h. We declare the function with C linkage
     to ensure proper linking when compiled with a C++11 driver.
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cmath>


// Include CMS internal type definitions (cmsUInt16Number, cmsFloat32Number, etc.)
// This header is typically provided with the LittleCMS project.
// Adjust include path if necessary in your build environment.

extern "C" {
    // Declaration of the focal function with C linkage
    void From16ToFloat(const cmsUInt16Number In[], cmsFloat32Number Out[], cmsUInt32Number n);
}

// Simple non-terminating assertion macro.
// It reports failures but continues executing remaining tests.
#define ASSERT_ALMOST_EQUAL(expected, actual, eps, msg) do { \
    if (std::fabs((expected) - (actual)) > (eps)) { \
        std::cerr << "[FAIL] " << (msg) \
                  << " | Expected: " << (expected) \
                  << "  Actual: " << (actual) \
                  << "  Diff: " << std::fabs((expected) - (actual)) \
                  << std::endl; \
        return false; \
    } \
} while (0)

#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << (msg) << std::endl; \
        return false; \
    } \
} while (0)

static bool test_From16ToFloat_AllZeros() {
    // Test that input all zeros maps to output all zeros
    // This covers the lower boundary of the domain.
    const cmsUInt32Number N = 8;
    cmsUInt16Number In[N] = {0, 0, 0, 0, 0, 0, 0, 0};
    cmsFloat32Number Out[N];

    From16ToFloat(In, Out, N);

    const float EPS = 1e-6f;
    for (cmsUInt32Number i = 0; i < N; ++i) {
        ASSERT_ALMOST_EQUAL(0.0f, Out[i], EPS, "From16ToFloat should map 0 -> 0.0f");
    }
    return true;
}

static bool test_From16ToFloat_AllMax() {
    // Test that input all 65535 maps to output all 1.0
    const cmsUInt32Number N = 8;
    cmsUInt16Number In[N];
    for (cmsUInt32Number i = 0; i < N; ++i) In[i] = 65535;
    cmsFloat32Number Out[N];

    From16ToFloat(In, Out, N);

    const float EPS = 1e-6f;
    for (cmsUInt32Number i = 0; i < N; ++i) {
        ASSERT_ALMOST_EQUAL(1.0f, Out[i], EPS, "From16ToFloat should map 65535 -> 1.0f");
    }
    return true;
}

static bool test_From16ToFloat_Midpoint() {
    // Test a midpoint value to ensure standard mapping is correct.
    // 32768 / 65535 ≈ 0.50001526
    const cmsUInt32Number N = 4;
    cmsUInt16Number In[N] = {32768, 32768, 32768, 32768};
    cmsFloat32Number Out[N];

    From16ToFloat(In, Out, N);

    const float expected = 32768.0f / 65535.0f;
    const float EPS = 1e-6f;
    for (cmsUInt32Number i = 0; i < N; ++i) {
        ASSERT_ALMOST_EQUAL(expected, Out[i], EPS, "From16ToFloat midpoint value incorrect");
    }
    return true;
}

static bool test_From16ToFloat_MixedSequence() {
    // Test a small mixed sequence to verify per-element mapping correctness
    const cmsUInt32Number N = 6;
    cmsUInt16Number In[N] = {0, 1, 32767, 32768, 65534, 65535};
    cmsFloat32Number Out[N];

    From16ToFloat(In, Out, N);

    const float EPS = 1e-6f;
    for (cmsUInt32Number i = 0; i < N; ++i) {
        const float expected = (float)In[i] / 65535.0f;
        ASSERT_ALMOST_EQUAL(expected, Out[i], EPS, "From16ToFloat mixed sequence value");
    }
    return true;
}

static bool test_From16ToFloat_ZeroLength() {
    // Test that n = 0 does not crash and produces no writes
    const cmsUInt32Number N = 0;
    cmsUInt16Number In[1] = {12345};
    cmsFloat32Number Out[1] = {0.0f};

    From16ToFloat(In, Out, N);

    // Output should remain untouched (still the initial value)
    const float EPS = 1e-6f;
    ASSERT_ALMOST_EQUAL(0.0f, Out[0], EPS, "From16ToFloat with n=0 should not modify output"); 
    return true;
}

int main() {
    std::cout << "Running From16ToFloat unit tests (C++11 harness)" << std::endl;

    int total = 0;
    int passed = 0;

    auto run = [&](const char* name, bool (*test)()) {
        ++total;
        bool ok = test();
        if (ok) {
            ++passed;
            std::cout << "[PASS] " << name << std::endl;
        } else {
            std::cout << "[FAIL] " << name << std::endl;
        }
    };

    run("AllZeros maps to 0.0f", test_From16ToFloat_AllZeros);
    run("AllMax maps to 1.0f", test_From16ToFloat_AllMax);
    run("Midpoint mapping (32768/65535)", test_From16ToFloat_Midpoint);
    run("Mixed sequence mapping", test_From16ToFloat_MixedSequence);
    run("Zero-length input does not crash", test_From16ToFloat_ZeroLength);

    std::cout << "Tests passed: " << passed << " / " << total << std::endl;

    // Return 0 for success, non-zero if any test failed
    return (passed == total) ? 0 : 1;
}