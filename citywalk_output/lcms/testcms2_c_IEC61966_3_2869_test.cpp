// Unit tests for IEC61966_3
// Follows the problem statement: create a small C++11 test suite without GTest.
// The tests call the focal method IEC61966_3 (a C function) with carefully chosen
// inputs to cover all branches described in the function.
//
// Candidate Keywords (derived from the focal function):
// - Params, Params[0..3], Params[1], Params[2], Params[3]
// - t = -Params[2] / Params[1], e = Params[1]*x + Params[2]
// - pow(e, Params[0]), x >= t, e > 0, else branch, x < t
// - return value casting to cmsFloat32Number
//
// Implementation notes:
// - We declare the IEC61966_3 symbol with C linkage to ensure proper linking
//   against the C implementation in testcms2.c (or similar).
// - cmsFloat32Number is treated as float and cmsFloat64Number as double.
#include <iostream>
#include <cmath>
#include <testcms2.h>


// Minimal typedefs matching the CMS library types used by IEC61966_3
typedef float cmsFloat32Number;
typedef double cmsFloat64Number;

// Forward declaration of the focal function with C linkage.
// We do not include test cms headers to avoid potential header-specific side effects.
extern "C" cmsFloat32Number IEC61966_3(cmsFloat32Number x, const cmsFloat64Number Params[]);

// Simple, non-terminating test assertion helper for floating-point values
static void check_equal(const char* test_name,
                        cmsFloat32Number actual,
                        cmsFloat32Number expected,
                        cmsFloat64Number tol = 1e-5)
{
    double a = static_cast<double>(actual);
    double e = static_cast<double>(expected);
    if (std::fabs(a - e) <= tol) {
        std::cout << "[PASS] " << test_name << "\n";
    } else {
        std::cout << "[FAIL] " << test_name
                  << " | actual=" << a << " expected=" << e
                  << " | diff=" << std::fabs(a - e) << "\n";
    }
}

int main() {
    // Test 1: x < t (where t = -Params[2] / Params[1]); expect Val = Params[3]
    // Parameters chosen to yield t = -6/3 = -2 and x = -3.0 < t
    {
        const cmsFloat64Number Params[4] = { 2.0, 3.0, 6.0, 1.5 };
        cmsFloat32Number x = -3.0f;
        cmsFloat32Number res = IEC61966_3(x, Params);
        check_equal("IEC61966_3 Test1: x < t -> Val == Params[3]",
                    res, static_cast<cmsFloat32Number>(Params[3]));
    }

    // Test 2: x == t (x equals the threshold, so e == 0); expect Val = Params[3]? No.
    // In the code, when x >= t, e = Params[1]*x + Params[2] becomes 0, and e > 0 is false,
    // so Val = 0.
    {
        const cmsFloat64Number Params[4] = { 2.0, 3.0, 6.0, 1.5 };
        cmsFloat32Number x = -2.0f; // x == t
        cmsFloat32Number res = IEC61966_3(x, Params);
        check_equal("IEC61966_3 Test2: x == t -> Val == 0", res, 0.0f);
    }

    // Test 3: x > t and e > 0
    // Using the same Params as Test 1, choose x = -1.5 to ensure e > 0.
    // e = Params[1]*x + Params[2] = 3*(-1.5) + 6 = 1.5 > 0
    // Val = pow(1.5, Params[0]) + Params[3] = 1.5^2 + 1.5 = 3.75
    {
        const cmsFloat64Number Params[4] = { 2.0, 3.0, 6.0, 1.5 };
        cmsFloat32Number x = -1.5f;
        cmsFloat32Number res = IEC61966_3(x, Params);
        double expected = std::pow(3.0 * x + 6.0, 2.0) + 1.5; // 1.5^2 + 1.5 = 3.75
        check_equal("IEC61966_3 Test3: x > t, e>0 -> Val = (e)^Params[0] + Params[3]",
                    res, static_cast<cmsFloat32Number>(expected));
    }

    // Test 4: x > t but e <= 0 (requires a negative slope so that e <= 0 for x > t)
    // Let Params[1] = -1, Params[2] = 1, Params[0] = 2, Params[3] = -0.5
    // Then t = -Params[2] / Params[1] = -1 / -1 = 1
    // For x = 2 > t, e = Params[1]*x + Params[2] = -2 + 1 = -1 <= 0
    // Expect Val = 0
    {
        const cmsFloat64Number Params[4] = { 2.0, -1.0, 1.0, -0.5 };
        cmsFloat32Number x = 2.0f;
        cmsFloat32Number res = IEC61966_3(x, Params);
        check_equal("IEC61966_3 Test4: x > t & e<=0 -> Val == 0", res, 0.0f);
    }

    std::cout << "Unit tests finished.\n";
    return 0;
}