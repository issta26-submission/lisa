// This test suite targets the focal method GetPointOfLine from cmssm.c.
// It uses a lightweight, non-terminating assertion mechanism to maximize
// coverage without relying on a particular testing framework (no GTest).

// Candidate Keywords (from Step 1 understanding):
// - cmsVEC3, cmsLine, cmsFloat64Number, VX, VY, VZ, a, u, t, p
// - Access patterns: p->n[VX], line->a.n[VX], line->u.n[VX], etc.

#include <lcms2_internal.h>
#include <iostream>
#include <string>
#include <cmath>


// Ensure proper linkage for GetPointOfLine if compiled in a C++ project.
#ifdef __cplusplus
extern "C" {
#endif
void GetPointOfLine(cmsVEC3* p, const cmsLine* line, cmsFloat64Number t);
#ifdef __cplusplus
}
#endif

// Simple non-terminating assertion framework
static int g_failures = 0;

static void fail_msg(const std::string& msg) {
    std::cerr << "Test failure: " << msg << std::endl;
    ++g_failures;
}

// Assert approximate equality for floating point numbers
static void expect_approx_equal(cmsFloat64Number a, cmsFloat64Number b, cmsFloat64Number tol, const std::string& desc) {
    if (std::fabs(a - b) > tol) {
        fail_msg(desc + " | expected: " + std::to_string(b) + ", actual: " + std::to_string(a) + ", tol: " + std::to_string(tol));
    }
}

// Assert equality for cmsVEC3 (component-wise)
static void expect_vec3_equal(const cmsVEC3& v, const cmsVEC3& w, cmsFloat64Number tol, const std::string& desc) {
    expect_approx_equal(v.n[0], w.n[0], tol, desc + " [X component]");
    expect_approx_equal(v.n[1], w.n[1], tol, desc + " [Y component]");
    expect_approx_equal(v.n[2], w.n[2], tol, desc + " [Z component]");
}

// Test 1: Basic along X axis
// a = (0,0,0), u = (1,0,0), t -> p = a + t*u -> p = (t,0,0)
static void test_GetPointOfLine_basic_x_axis() {
    cmsLine line;
    // a = (0,0,0)
    line.a.n[0] = 0.0; line.a.n[1] = 0.0; line.a.n[2] = 0.0;
    // u = (1,0,0)
    line.u.n[0] = 1.0; line.u.n[1] = 0.0; line.u.n[2] = 0.0;

    cmsVEC3 p;

    // t = 0
    GetPointOfLine(&p, &line, 0.0);
    cmsVEC3 expected;
    expected.n[0] = 0.0; expected.n[1] = 0.0; expected.n[2] = 0.0;
    expect_vec3_equal(p, expected, 1e-12, "GetPointOfLine: t=0 on X axis");

    // t = 2.5
    GetPointOfLine(&p, &line, 2.5);
    expected.n[0] = 2.5; expected.n[1] = 0.0; expected.n[2] = 0.0;
    expect_vec3_equal(p, expected, 1e-12, "GetPointOfLine: t=2.5 on X axis");

    // t = -1.0
    GetPointOfLine(&p, &line, -1.0);
    expected.n[0] = -1.0; expected.n[1] = 0.0; expected.n[2] = 0.0;
    expect_vec3_equal(p, expected, 1e-12, "GetPointOfLine: t=-1.0 on X axis");
}

// Test 2: Arbitrary line in 3D
// a = (1,2,3), u = (4,5,6), p = a + t*u
static void test_GetPointOfLine_arbitrary_3d() {
    cmsLine line;
    line.a.n[0] = 1.0; line.a.n[1] = 2.0; line.a.n[2] = 3.0;
    line.u.n[0] = 4.0; line.u.n[1] = 5.0; line.u.n[2] = 6.0;

    cmsVEC3 p;
    cmsFloat64Number t = 0.5;

    GetPointOfLine(&p, &line, t);
    cmsVEC3 expected;
    expected.n[0] = line.a.n[0] + t * line.u.n[0]; // 1 + 0.5*4 = 3
    expected.n[1] = line.a.n[1] + t * line.u.n[1]; // 2 + 0.5*5 = 4.5
    expected.n[2] = line.a.n[2] + t * line.u.n[2]; // 3 + 0.5*6 = 6
    expect_vec3_equal(p, expected, 1e-12, "GetPointOfLine: arbitrary 3D test with t=0.5");

    // Additional check: t = -2.0
    t = -2.0;
    GetPointOfLine(&p, &line, t);
    expected.n[0] = line.a.n[0] + t * line.u.n[0]; // 1 - 8 = -7
    expected.n[1] = line.a.n[1] + t * line.u.n[1]; // 2 - 10 = -8
    expected.n[2] = line.a.n[2] + t * line.u.n[2]; // 3 - 12 = -9
    expect_vec3_equal(p, expected, 1e-12, "GetPointOfLine: arbitrary 3D test with t=-2.0");
}

// Test 3: Ensure GetPointOfLine does not modify the input line structure
static void test_GetPointOfLine_no_side_effects() {
    cmsLine line;
    line.a.n[0] = -3.0; line.a.n[1] = 7.0; line.a.n[2] = 0.0;
    line.u.n[0] = 2.0;  line.u.n[1] = -1.0; line.u.n[2] = 4.0;

    // Snapshot of line before call
    cmsVEC3 a_before = line.a;
    cmsVEC3 u_before = line.u;

    cmsVEC3 p;
    GetPointOfLine(&p, &line, 1.25);

    // After call, line.a and line.u should remain unchanged
    expect_vec3_equal(line.a, a_before, 1e-12, "GetPointOfLine: line.a unchanged check");
    expect_vec3_equal(line.u, u_before, 1e-12, "GetPointOfLine: line.u unchanged check");
}

// Runner
int main() {
    // Print basic header
    std::cout << "Running GetPointOfLine unit tests (C++11, no GTest)..." << std::endl;

    test_GetPointOfLine_basic_x_axis();
    test_GetPointOfLine_arbitrary_3d();
    test_GetPointOfLine_no_side_effects();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}