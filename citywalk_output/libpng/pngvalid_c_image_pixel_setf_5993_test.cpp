// Unit tests for image_pixel_setf in C (ported to C++11 test harness)
// This test suite targets the focal method image_pixel_setf(image_pixel *this, unsigned int rMax, unsigned int gMax, unsigned int bMax, unsigned int aMax)
// and validates that the dependent fields redf, greenf, bluef, alphaf and their corresponding epsilon components
// (rede, greene, bluee, alphae) are computed according to the implementation.
//
// Notes:
// - We declare the C-compatible image_pixel struct and the function prototype inside extern "C" to allow linkage from C++.
// - The struct layout mirrors the expected members used by image_pixel_setf: red, green, blue, alpha (unsigned int),
//   redf, greenf, bluef, alphaf (double), rede, greene, bluee, alphae (double).
// - We test true/false branches of each predicate (red < rMax, green < gMax, blue < bMax, alpha < aMax) across several cases.
// - Assertions are non-terminating in nature in this harness (we implement simple boolean checks with informative messages).
// - This test harness uses only the C++ standard library (no GTest), and provides a minimal main() to run tests.

#include <signal.h>
#include <string.h>
#include <float.h>
#include <cfloat>
#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <math.h>


// Declare C interface (function and struct) to link with the actual C implementation.
// We avoid using the C++ keyword "this" as a parameter name by using this_ptr, matching the original semantics.
extern "C" {

typedef struct image_pixel {
    unsigned int red;
    unsigned int green;
    unsigned int blue;
    unsigned int alpha;

    double redf;
    double greenf;
    double bluef;
    double alphaf;

    double rede;
    double greene;
    double bluee;
    double alphae;
} image_pixel;

// Function under test (original signature uses parameter named "this").
// We declare with a safe parameter name for C++ compilation.
void image_pixel_setf(image_pixel *this_ptr, unsigned int rMax, unsigned int gMax,
                    unsigned int bMax, unsigned int aMax);
} // extern "C"

static bool almost_equal(double a, double b, double tol = 1e-12) {
    return std::fabs(a - b) <= tol;
}

// Test Case 1: All channels below their max (true branches for red, green, blue, alpha in this case)
bool test_case1() {
    image_pixel ip{};
    ip.red = 5;
    ip.green = 3;
    ip.blue = 7;
    ip.alpha = 2;

    // rMax, gMax, bMax, aMax chosen so all channels are below their max
    const unsigned int rMax = 10;
    const unsigned int gMax = 10;
    const unsigned int bMax = 10;
    const unsigned int aMax = 5;

    image_pixel_setf(&ip, rMax, gMax, bMax, aMax);

    // Verify color component fractions
    if (!almost_equal(ip.redf, 5.0 / (double)rMax)) {
        std::cerr << "[test_case1] redf mismatch: expected " << (5.0 / (double)rMax)
                  << ", got " << ip.redf << "\n";
        return false;
    }
    if (!almost_equal(ip.greenf, 3.0 / (double)gMax)) {
        std::cerr << "[test_case1] greenf mismatch: expected " << (3.0 / (double)gMax)
                  << ", got " << ip.greenf << "\n";
        return false;
    }
    if (!almost_equal(ip.bluef, 7.0 / (double)bMax)) {
        std::cerr << "[test_case1] bluef mismatch: expected " << (7.0 / (double)bMax)
                  << ", got " << ip.bluef << "\n";
        return false;
    }
    if (!almost_equal(ip.alphaf, 2.0 / (double)aMax)) {
        std::cerr << "[test_case1] alphaf mismatch: expected " << (2.0 / (double)aMax)
                  << ", got " << ip.alphaf << "\n";
        return false;
    }

    // Verify epsilon-based components (should be redf*DBL_EPSILON, etc.)
    double expected_rede = (ip.red < rMax) ? ip.redf * DBL_EPSILON : 0.0;
    double expected_greene = (ip.green < gMax) ? ip.greenf * DBL_EPSILON : 0.0;
    double expected_bluee = (ip.blue < bMax) ? ip.bluef * DBL_EPSILON : 0.0;
    double expected_alphae = (ip.alpha < aMax) ? ip.alphaf * DBL_EPSILON : 0.0;

    if (!almost_equal(ip.rede, expected_rede)) {
        std::cerr << "[test_case1] rede mismatch: expected " << expected_rede << ", got " << ip.rede << "\n";
        return false;
    }
    if (!almost_equal(ip.greene, expected_greene)) {
        std::cerr << "[test_case1] greene mismatch: expected " << expected_greene << ", got " << ip.greene << "\n";
        return false;
    }
    if (!almost_equal(ip.bluee, expected_bluee)) {
        std::cerr << "[test_case1] bluee mismatch: expected " << expected_bluee << ", got " << ip.bluee << "\n";
        return false;
    }
    if (!almost_equal(ip.alphae, expected_alphae)) {
        std::cerr << "[test_case1] alphae mismatch: expected " << expected_alphae << ", got " << ip.alphae << "\n";
        return false;
    }

    return true;
}

// Test Case 2: Mixed true/false branches (red >= rMax, blue < bMax, others below)
bool test_case2() {
    image_pixel ip{};
    ip.red = 12;   // red >= rMax => rede should be 0
    ip.green = 2;   // green < gMax
    ip.blue = 11;   // blue >= bMax
    ip.alpha = 1;   // alpha < aMax

    const unsigned int rMax = 10;
    const unsigned int gMax = 6;
    const unsigned int bMax = 10;
    const unsigned int aMax = 5;

    image_pixel_setf(&ip, rMax, gMax, bMax, aMax);

    // redf should reflect 12/10 = 1.2
    if (!almost_equal(ip.redf, 12.0 / (double)rMax)) {
        std::cerr << "[test_case2] redf mismatch: expected " << (12.0 / (double)rMax
                  << ", got " << ip.redf << "\n";
        return false;
    }
    // green below max
    if (!almost_equal(ip.greenf, 2.0 / (double)gMax)) {
        std::cerr << "[test_case2] greenf mismatch: expected " << (2.0 / (double)gMax)
                  << ", got " << ip.greenf << "\n";
        return false;
    }
    // blue not below max -> bluef should be 11/10 = 1.1
    if (!almost_equal(ip.bluef, 11.0 / (double)bMax)) {
        std::cerr << "[test_case2] bluef mismatch: expected " << (11.0 / (double)bMax)
                  << ", got " << ip.bluef << "\n";
        return false;
    }
    // alpha below max
    if (!almost_equal(ip.alphaf, 1.0 / (double)aMax)) {
        std::cerr << "[test_case2] alphaf mismatch: expected " << (1.0 / (double)aMax)
                  << ", got " << ip.alphaf << "\n";
        return false;
    }

    // Expected epsilon-based components
    double expected_rede = (ip.red < rMax) ? ip.redf * DBL_EPSILON : 0.0;
    double expected_greene = (ip.green < gMax) ? ip.greenf * DBL_EPSILON : 0.0;
    double expected_bluee = (ip.blue < bMax) ? ip.bluef * DBL_EPSILON : 0.0;
    double expected_alphae = (ip.alpha < aMax) ? ip.alphaf * DBL_EPSILON : 0.0;

    if (!almost_equal(ip.rede, expected_rede)) {
        std::cerr << "[test_case2] rede mismatch: expected " << expected_rede << ", got " << ip.rede << "\n";
        return false;
    }
    if (!almost_equal(ip.greene, expected_greene)) {
        std::cerr << "[test_case2] greene mismatch: expected " << expected_greene << ", got " << ip.greene << "\n";
        return false;
    }
    if (!almost_equal(ip.bluee, expected_bluee)) {
        std::cerr << "[test_case2] bluee mismatch: expected " << expected_bluee << ", got " << ip.bluee << "\n";
        return false;
    }
    if (!almost_equal(ip.alphae, expected_alphae)) {
        std::cerr << "[test_case2] alphae mismatch: expected " << expected_alphae << ", got " << ip.alphae << "\n";
        return false;
    }

    return true;
}

// Test Case 3: All channels at their max (all false branches)
bool test_case3() {
    image_pixel ip{};
    ip.red = 10;
    ip.green = 10;
    ip.blue = 10;
    ip.alpha = 5;

    const unsigned int rMax = 10;
    const unsigned int gMax = 10;
    const unsigned int bMax = 10;
    const unsigned int aMax = 5;

    image_pixel_setf(&ip, rMax, gMax, bMax, aMax);

    // redf, greenf, bluef, alphaf should be set, but their corresponding *e fields should be 0
    if (!almost_equal(ip.redf, 10.0 / (double)rMax)) {
        std::cerr << "[test_case3] redf mismatch: expected " << (10.0 / (double)rMax)
                  << ", got " << ip.redf << "\n";
        return false;
    }
    if (!almost_equal(ip.greenf, 10.0 / (double)gMax)) {
        std::cerr << "[test_case3] greenf mismatch: expected " << (10.0 / (double)gMax)
                  << ", got " << ip.greenf << "\n";
        return false;
    }
    if (!almost_equal(ip.bluef, 10.0 / (double)bMax)) {
        std::cerr << "[test_case3] bluef mismatch: expected " << (10.0 / (double)bMax)
                  << ", got " << ip.bluef << "\n";
        return false;
    }
    if (!almost_equal(ip.alphaf, 5.0 / (double)aMax)) {
        std::cerr << "[test_case3] alphaf mismatch: expected " << (5.0 / (double)aMax)
                  << ", got " << ip.alphaf << "\n";
        return false;
    }

    // All epsilon-based fields should be zero since each channel is not below its max
    if (ip.rede != 0.0) {
        std::cerr << "[test_case3] rede should be 0, got " << ip.rede << "\n";
        return false;
    }
    if (ip.greene != 0.0) {
        std::cerr << "[test_case3] greene should be 0, got " << ip.greene << "\n";
        return false;
    }
    if (ip.bluee != 0.0) {
        std::cerr << "[test_case3] bluee should be 0, got " << ip.bluee << "\n";
        return false;
    }
    if (ip.alphae != 0.0) {
        std::cerr << "[test_case3] alphae should be 0, got " << ip.alphae << "\n";
        return false;
    }

    return true;
}

int main() {
    int failures = 0;

    std::cout << "Running test_case1: all channels below max (true branches for red/green/blue/alpha)\n";
    if (!test_case1()) {
        std::cout << "test_case1: FAILED\n";
        ++failures;
    } else {
        std::cout << "test_case1: PASSED\n";
    }

    std::cout << "Running test_case2: mixed branches (red>=max, blue>=max, others below)\n";
    if (!test_case2()) {
        std::cout << "test_case2: FAILED\n";
        ++failures;
    } else {
        std::cout << "test_case2: PASSED\n";
    }

    std::cout << "Running test_case3: all channels at max (all predicate false)\n";
    if (!test_case3()) {
        std::cout << "test_case3: FAILED\n";
        ++failures;
    } else {
        std::cout << "test_case3: PASSED\n";
    }

    if (failures > 0) {
        std::cout << "TOTAL: " << failures << " test(s) FAILED\n";
        return 1;
    } else {
        std::cout << "TOTAL: all tests PASSED\n";
        return 0;
    }
}