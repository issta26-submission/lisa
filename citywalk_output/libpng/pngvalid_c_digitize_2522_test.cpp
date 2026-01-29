// Test suite for the focal method: digitize in pngvalid.c
// This test suite is written in C++11 (no Google Test) and calls the C function
// with C linkage. It covers true/false branches and edge conditions of the
// digitize function as described in the provided source.
//
// Notes:
// - We declare the function as extern "C" to ensure correct linkage from C++.
// - We do not rely on private state or static internal helpers of the source file.
// - The tests are designed to be executable in a standard C++11 environment.
// - The tests focus on correctness of digitize with respect to value clamping,
//   digitization_factor calculation, rounding/truncation behavior, and
//   varying bit depths (depth parameter).

#include <signal.h>
#include <string.h>
#include <iomanip>
#include <string>
#include <float.h>
#include <cmath>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <math.h>


// Declare the focal function with C linkage.
extern "C" double digitize(double value, int depth, int do_round);

// Simple approximate equality for doubles
static bool approx_equal(double a, double b, double tol = 1e-12) {
    return std::fabs(a - b) <= tol;
}

// Lightweight test harness
static void run_test(const std::string& name, bool passed) {
    if (passed) {
        std::cout << "[PASS] " << name << std::endl;
    } else {
        std::cout << "[FAIL] " << name << std::endl;
    }
}

int main() {
    int failures = 0;

    // Test 1: value clamped to 0 when value <= 0 (do_round = 1)
    {
        double val = digitize(-1.0, 8, 1); // expected 0.0
        bool ok = approx_equal(val, 0.0);
        run_test("digitize: clamp to 0 when value <= 0 (depth=8, round=1)", ok);
        if (!ok) ++failures;
    }

    // Test 2: value clamped to 1 when value >= 1 (do_round = 1)
    {
        double val = digitize(1.2, 8, 1); // expected 1.0
        bool ok = approx_equal(val, 1.0);
        run_test("digitize: clamp to 1 when value >= 1 (depth=8, round=1)", ok);
        if (!ok) ++failures;
    }

    // Test 3: value = 0.5, depth=8, do_round=1
    // digitization_factor = (1<<8) - 1 = 255
    // value*digitization_factor = 127.5; +0.5 -> 128.0; floor = 128; 128/255
    {
        double expected = 128.0 / 255.0;
        double got = digitize(0.5, 8, 1);
        bool ok = approx_equal(got, expected, 1e-12);
        run_test("digitize: 0.5 with depth=8, round=1 -> 128/255", ok);
        if (!ok) ++failures;
    }

    // Test 4: value = 0.5, depth=8, do_round=0
    // digitization_factor = 255; value*digitization_factor = 127.5; floor=127; 127/255
    {
        double expected = 127.0 / 255.0;
        double got = digitize(0.5, 8, 0);
        bool ok = approx_equal(got, expected, 1e-12);
        run_test("digitize: 0.5 with depth=8, round=0 -> 127/255", ok);
        if (!ok) ++failures;
    }

    // Test 5: value near 0, depth=16, do_round=1
    // digitization_factor = 65535; value*factor ≈ 6.5535; +0.5 → 7.0535; floor=7
    // result = 7/65535
    {
        double expected = 7.0 / 65535.0;
        double got = digitize(0.0001, 16, 1);
        bool ok = approx_equal(got, expected, 1e-12);
        run_test("digitize: small value with depth=16, round=1 -> 7/65535", ok);
        if (!ok) ++failures;
    }

    // Test 6: value -0.5 with rounding should clamp to 0 (depth=8, round=1)
    {
        double got = digitize(-0.5, 8, 1);
        bool ok = approx_equal(got, 0.0);
        run_test("digitize: negative value clamps to 0 (depth=8, round=1)", ok);
        if (!ok) ++failures;
    }

    // Test 7: value > 1 with depth=8; ensure returns 1 (rounding impact)
    {
        double got_round1 = digitize(1.5, 8, 1);
        double got_round0 = digitize(1.5, 8, 0);
        bool ok1 = approx_equal(got_round1, 1.0);
        bool ok0 = approx_equal(got_round0, 1.0);
        run_test("digitize: value>1 returns 1 (depth=8, round=1)", ok1);
        if (!ok1) ++failures;
        run_test("digitize: value>1 returns 1 (depth=8, round=0)", ok0);
        if (!ok0) ++failures;
    }

    // Test 8: depth = 1 boundary behavior with small/large values and rounding
    // digitization_factor = 1
    {
        // 0.3 * 1 = 0.3; with round=1 => floor(0.3+0.5)=0 -> 0/1 = 0
        double got0 = digitize(0.3, 1, 1);
        bool ok0 = approx_equal(got0, 0.0);
        run_test("digitize: depth=1, value=0.3, round=1 -> 0", ok0);
        if (!ok0) ++failures;

        // 0.8 * 1 = 0.8; with round=1 => floor(0.8+0.5)=1 -> 1/1 = 1
        double got1 = digitize(0.8, 1, 1);
        bool ok1 = approx_equal(got1, 1.0);
        run_test("digitize: depth=1, value=0.8, round=1 -> 1", ok1);
        if (!ok1) ++failures;
    }

    // Test 9: depth=16, value=0.999999, do_round=0
    // digitization_factor = 65535; value*factor ≈ 65534.934465; floor = 65534; 65534/65535
    {
        double expected = 65534.0 / 65535.0;
        double got = digitize(0.999999, 16, 0);
        bool ok = approx_equal(got, expected, 1e-12);
        run_test("digitize: depth=16, value=0.999999, round=0 -> 65534/65535", ok);
        if (!ok) ++failures;
    }

    // Test 10: depth=16, value=0.999999, do_round=1 -> should push to 1.0
    {
        double got = digitize(0.999999, 16, 1);
        bool ok = approx_equal(got, 1.0);
        run_test("digitize: depth=16, value=0.999999, round=1 -> 1", ok);
        if (!ok) ++failures;
    }

    // Summary
    if (failures == 0) {
        std::cout << "[SUMMARY] All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << "[SUMMARY] " << failures << " test(s) failed." << std::endl;
        return 1;
    }
}