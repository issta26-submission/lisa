// This test suite targets the focal function:
// cmsFloat32Number param_6(cmsFloat32Number x, const cmsFloat64Number Params[])
// It uses only standard C++11 facilities and the project's provided types from testcms2.h.
// The tests are invoked from main (no Google Test or other frameworks).

// Step 1: Program Understanding (Candidate Keywords)
// - Core inputs: x (cmsFloat32Number), Params[] (cmsFloat64Number) with indices used: Params[0], Params[1], Params[2], Params[3]
// - Core computations: e = Params[1] * x + Params[2]; conditional branch on (e > 0)
// - Branch 1 (true): Val = pow(e, Params[0]) + Params[3]
// - Branch 2 (false): Val = 0
// - Return: (cmsFloat32Number) Val
// - Important behaviors to cover: 
//   a) e > 0 and various exponents (Params[0]); including integer and fractional exponents
//   b) e <= 0 (negative or zero) leading to 0 return
//   c) Boundary case e == 0 (should take false branch)
// - Static/global considerations: none accessed; tests use provided types and function directly.

#include <cstdio>
#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <cmath>


// Include project headers to get types like cmsFloat32Number, cmsFloat64Number

// Ensure we have C linkage for the focal function when linking with C code
extern "C" cmsFloat32Number param_6(cmsFloat32Number x, const cmsFloat64Number Params[]);

static std::vector<std::string> g_failures; // collect failures to report without aborting

static void logFailure(const std::string& msg) {
    g_failures.push_back(msg);
}

// Lightweight assertion helpers that do not terminate on failure
static void EXPECT_NEAR_F32(cmsFloat32Number actual, double expected, double tol, const std::string& note) {
    double a = static_cast<double>(actual);
    if (std::fabs(a - expected) > tol) {
        char buf[512];
        std::snprintf(buf, sizeof(buf),
                      "FAIL: %s - actual=%f, expected=%f (tol=%e)", note.c_str(), a, expected, tol);
        logFailure(buf);
    }
}

static void EXPECT_EQ_INT(int a, int b, const std::string& note) {
    if (a != b) {
        char buf[256];
        std::snprintf(buf, sizeof(buf),
                      "FAIL: %s - %d != %d", note.c_str(), a, b);
        logFailure(buf);
    }
}

// Main test suite entry
int main() {
    // Test 1: e > 0 path with a simple positive exponent
    // Params: [0]=2.0, [1]=3.0, [2]=-1.0, [3]=4.0
    // x = 1.0 -> e = 3.0*1.0 + (-1.0) = 2.0 > 0
    // Val = pow(2.0, 2.0) + 4.0 = 4.0 + 4.0 = 8.0
    {
        cmsFloat64Number Params[4] = {2.0, 3.0, -1.0, 4.0};
        cmsFloat32Number res = param_6(1.0f, Params);
        EXPECT_NEAR_F32(res, 8.0, 1e-6, "Test1_positive_branch_basic");
    }

    // Test 2: e <= 0 path (negative e)
    // Use same Params but x = 0.0 -> e = 3.0*0.0 + (-1.0) = -1.0 (negative)
    // Val should be 0
    {
        cmsFloat64Number Params[4] = {2.0, 3.0, -1.0, 4.0};
        cmsFloat32Number res = param_6(0.0f, Params);
        EXPECT_NEAR_F32(res, 0.0, 1e-6, "Test2_negative_branch");
    }

    // Test 3: boundary case where e == 0; should take the false branch
    // Choose Params such that e = Params[1]*x + Params[2] = 0
    // Example: Params[1] = 2.0, Params[2] = -2.0, x = 1.0 -> e = 0
    {
        cmsFloat64Number Params[4] = {2.0, 2.0, -2.0, 5.0};
        cmsFloat32Number res = param_6(1.0f, Params);
        EXPECT_NEAR_F32(res, 0.0, 1e-6, "Test3_boundary_e_zero");
    }

    // Test 4: exponent is zero (Params[0] == 0) and e > 0
    // Choose: Params[1]=2.0, Params[2]=-1.0, x=1.0 -> e = 1.0 > 0
    // Val = pow(1.0, 0) + Params[3] = 1 + Params[3]
    {
        cmsFloat64Number Params[4] = {0.0, 2.0, -1.0, 5.0};
        cmsFloat32Number res = param_6(1.0f, Params);
        double expected = std::pow(1.0, 0.0) + 5.0; // = 6.0
        EXPECT_NEAR_F32(res, expected, 1e-6, "Test4_exponent_zero");
    }

    // Test 5: fractional exponent with e > 0
    // Params: [0]=1.5, [1]=1.0, [2]=-0.5, [3]=0.0
    // x=2 -> e = 1.0*2 + (-0.5) = 1.5 > 0
    // Val = pow(1.5, 1.5) + 0
    {
        cmsFloat64Number Params[4] = {1.5, 1.0, -0.5, 0.0};
        cmsFloat32Number res = param_6(2.0f, Params);
        double e = 1.0 * 2.0 - 0.5; // 1.5
        double expected = std::pow(e, 1.5);
        EXPECT_NEAR_F32(res, expected, 1e-6, "Test5_fractional_exponent");
    }

    // Report results
    if (!g_failures.empty()) {
        std::cerr << g_failures.size() << " test(s) failed:\n";
        for (const auto& s : g_failures) std::cerr << s << "\n";
        return 1;
    } else {
        std::cout << "All param_6 tests passed.\n";
        return 0;
    }
}