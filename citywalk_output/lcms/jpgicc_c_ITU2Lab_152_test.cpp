// Lightweight C++11 unit test suite for ITU2Lab (no Google Test, pure C++ test harness)
#include <iccjpeg.h>
#include <cstdio>
#include <cstdint>
#include <jpeglib.h>
#include <string>
#include <utils.h>
#include <cmath>


// Forward declarations to match the focal function's signature.
// We assume the library uses the following definitions (typical for liblcms):
typedef uint16_t cmsUInt16Number;
typedef struct { double L; double a; double b; } cmsCIELab;

// Function under test - provided by the target project (C linkage)
extern "C" void ITU2Lab(const cmsUInt16Number In[3], cmsCIELab* Lab);

// Minimal assertion helpers (non-terminating): accumulate failures but continue
static bool approx(double a, double b, double eps = 1e-6) {
    return std::fabs(a - b) <= eps;
}

// Compute expected Lab values using the exact formula from the focal method
static void compute_expected(const cmsUInt16Number In[3], double* expL, double* expA, double* expB) {
    *expL = (double)In[0] / 655.35;
    *expA = 170.0 * ((double)In[1] - 32768.0) / 65535.0;
    *expB = 200.0 * ((double)In[2] - 24576.0) / 65535.0;
}

// Global helper to print test result
static void print_result(const std::string& testName, bool passed) {
    if (passed) {
        printf("[PASS] %s\n", testName.c_str());
    } else {
        printf("[FAIL] %s\n", testName.c_str());
    }
}

// Test 1: In = {0, 32768, 24576} should yield L=0, a=0, b=0
// This verifies the exact zeroed Lab components for a neutral input triplet.
static bool test_ITU2Lab_zero_lab() {
    const cmsUInt16Number In[3] = {0, 32768, 24576};
    cmsCIELab Lab;
    ITU2Lab(In, &Lab);

    double expL, expA, expB;
    compute_expected(In, &expL, &expA, &expB);

    bool ok = approx(Lab.L, expL) && approx(Lab.a, expA) && approx(Lab.b, expB);
    if (!ok) {
        printf("  Expected L=%.9f a=%.9f b=%.9f, Got L=%.9f a=%.9f b=%.9f\n",
               expL, expA, expB, Lab.L, Lab.a, Lab.b);
    }
    print_result("test_ITU2Lab_zero_lab", ok);
    return ok;
}

// Test 2: In = {65535, 32768, 24576} should yield L=100, a=0, b=0
// Verifies the maximum L case with neutral chromatic axes.
static bool test_ITU2Lab_max_L_neutral_ab() {
    const cmsUInt16Number In[3] = {65535, 32768, 24576};
    cmsCIELab Lab;
    ITU2Lab(In, &Lab);

    double expL, expA, expB;
    compute_expected(In, &expL, &expA, &expB);

    bool ok = approx(Lab.L, expL) && approx(Lab.a, expA) && approx(Lab.b, expB);
    if (!ok) {
        printf("  Expected L=%.9f a=%.9f b=%.9f, Got L=%.9f a=%.9f b=%.9f\n",
               expL, expA, expB, Lab.L, Lab.a, Lab.b);
    }
    print_result("test_ITU2Lab_max_L_neutral_ab", ok);
    return ok;
}

// Test 3: In = {32768, 0, 65535} tests mid L, negative a, high b
// This covers a non-trivial mixed scenario for a and b.
static bool test_ITU2Lab_mid_values_mixed() {
    const cmsUInt16Number In[3] = {32768, 0, 65535};
    cmsCIELab Lab;
    ITU2Lab(In, &Lab);

    double expL, expA, expB;
    compute_expected(In, &expL, &expA, &expB);

    bool ok = approx(Lab.L, expL) && approx(Lab.a, expA) && approx(Lab.b, expB);
    if (!ok) {
        printf("  Expected L=%.9f a=%.9f b=%.9f, Got L=%.9f a=%.9f b=%.9f\n",
               expL, expA, expB, Lab.L, Lab.a, Lab.b);
    }
    print_result("test_ITU2Lab_mid_values_mixed", ok);
    return ok;
}

// Test 4: In = {65535, 0, 0} tests max L, strong negative a, moderate negative b
static bool test_ITU2Lab_edge_extremes_out() {
    const cmsUInt16Number In[3] = {65535, 0, 0};
    cmsCIELab Lab;
    ITU2Lab(In, &Lab);

    double expL, expA, expB;
    compute_expected(In, &expL, &expA, &expB);

    bool ok = approx(Lab.L, expL) && approx(Lab.a, expA) && approx(Lab.b, expB);
    if (!ok) {
        printf("  Expected L=%.9f a=%.9f b=%.9f, Got L=%.9f a=%.9f b=%.9f\n",
               expL, expA, expB, Lab.L, Lab.a, Lab.b);
    }
    print_result("test_ITU2Lab_edge_extremes_out", ok);
    return ok;
}

// Test 5: In = {32768, 32768, 0} checks mid L, neutral a, negative b
static bool test_ITU2Lab_mid_L_neutral_ab() {
    const cmsUInt16Number In[3] = {32768, 32768, 0};
    cmsCIELab Lab;
    ITU2Lab(In, &Lab);

    double expL, expA, expB;
    compute_expected(In, &expL, &expA, &expB);

    bool ok = approx(Lab.L, expL) && approx(Lab.a, expA) && approx(Lab.b, expB);
    if (!ok) {
        printf("  Expected L=%.9f a=%.9f b=%.9f, Got L=%.9f a=%.9f b=%.9f\n",
               expL, expA, expB, Lab.L, Lab.a, Lab.b);
    }
    print_result("test_ITU2Lab_mid_L_neutral_ab", ok);
    return ok;
}

// Test 6: In = {1000, 1000, 1000} arbitrary small input values
static bool test_ITU2Lab_arbitrary_small_input() {
    const cmsUInt16Number In[3] = {1000, 1000, 1000};
    cmsCIELab Lab;
    ITU2Lab(In, &Lab);

    double expL, expA, expB;
    compute_expected(In, &expL, &expA, &expB);

    bool ok = approx(Lab.L, expL) && approx(Lab.a, expA) && approx(Lab.b, expB);
    if (!ok) {
        printf("  Expected L=%.9f a=%.9f b=%.9f, Got L=%.9f a=%.9f b=%.9f\n",
               expL, expA, expB, Lab.L, Lab.a, Lab.b);
    }
    print_result("test_ITU2Lab_arbitrary_small_input", ok);
    return ok;
}

// Main runner
int main() {
    // Run all tests and aggregate results
    int total = 0;
    int passed = 0;

    auto run = [&](bool ok) {
        ++total;
        if (ok) ++passed;
        return ok;
    };

    run(test_ITU2Lab_zero_lab());
    run(test_ITU2Lab_max_L_neutral_ab());
    run(test_ITU2Lab_mid_values_mixed());
    run(test_ITU2Lab_edge_extremes_out());
    run(test_ITU2Lab_mid_L_neutral_ab());
    run(test_ITU2Lab_arbitrary_small_input());

    printf("Summary: %d/%d tests passed.\n", passed, total);
    return (passed == total) ? 0 : 1;
}