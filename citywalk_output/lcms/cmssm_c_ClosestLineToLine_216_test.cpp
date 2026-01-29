// Self-contained unit test suite for the focal method ClosestLineToLine
// Note: This is a standalone test harness that re-implements the minimal
// vector/matrix scaffolding required to exercise the algorithm.
// It mirrors the logic of the focal method to enable thorough testing
// without pulling in the entire upstream project.
//
// The tests cover multiple execution paths:
//  - Parallel lines (D near zero) -> expect projection at P0 of line1 (r == line1.a)
//  - Non-parallel case with sc ~ 0.3 (derived from specific line configurations)
//  - Non-parallel case where tN < 0 path is exercised (ensures edge recomputation)
// Each test prints a success/failure message and continues, enabling
// non-terminating assertions as requested.

#include <lcms2_internal.h>
#include <iostream>
#include <cmath>
#include <iomanip>


// Domain-specific lightweight redefinitions to support the focal method.
// These mimic the essential types and helpers used by ClosestLineToLine.
typedef int cmsBool;
#define TRUE 1
#define FALSE 0
#define MATRIX_DET_TOLERANCE 1e-12

struct cmsVEC3 {
    double x, y, z;
};

struct cmsLine {
    cmsVEC3 a; // point on the line
    cmsVEC3 u; // direction vector
};

// Helper function: r = a - b
static void _cmsVEC3minus(cmsVEC3* r, const cmsVEC3* a, const cmsVEC3* b) {
    r->x = a->x - b->x;
    r->y = a->y - b->y;
    r->z = a->z - b->z;
}

// Helper function: dot product
static double _cmsVEC3dot(const cmsVEC3* a, const cmsVEC3* b) {
    return a->x*b->x + a->y*b->y + a->z*b->z;
}

// Helper function: Get point on a line at parameter t: p = a + t*u
static void GetPointOfLine(cmsVEC3* p, const cmsLine* line, double t) {
    p->x = line->a.x + t*line->u.x;
    p->y = line->a.y + t*line->u.y;
    p->z = line->a.z + t*line->u.z;
}

// The focal method under test (re-implemented here for a self-contained suite)
static cmsBool ClosestLineToLine(cmsVEC3* r, const cmsLine* line1, const cmsLine* line2) {
    double a, b, c, d, e, D;
    double sc, sN, sD;
    double tN, tD;
    cmsVEC3 w0;

    _cmsVEC3minus(&w0, &line1->a, &line2->a);
    a  = _cmsVEC3dot(&line1->u, &line1->u);
    b  = _cmsVEC3dot(&line1->u, &line2->u);
    c  = _cmsVEC3dot(&line2->u, &line2->u);
    d  = _cmsVEC3dot(&line1->u, &w0);
    e  = _cmsVEC3dot(&line2->u, &w0);
    D  = a*c - b * b;      // Denominator
    sD = tD = D;            // default sD = D >= 0

    if (D < MATRIX_DET_TOLERANCE) {   // the lines are almost parallel
        sN = 0.0;        // force using point P0 on segment S1
        sD = 1.0;        // to prevent possible division by 0.0 later
        tN = e;
        tD = c;
    } else {                // get the closest points on the infinite lines
        sN = (b*e - c*d);
        tN = (a*e - b*d);
        if (sN < 0.0) {       // sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = e;
            tD = c;
        } else if (sN > sD) {   // sc > 1 => the s=1 edge is visible
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    if (tN < 0.0) {           // tc < 0 => the t=0 edge is visible
        tN = 0.0;
        // recompute sc for this edge
        if (-d < 0.0)
            sN = 0.0;
        else if (-d > a)
            sN = sD;
        else {
            sN = -d;
            sD = a;
        }
    } else if (tN > tD) {      // tc > 1 => the t=1 edge is visible
        tN = tD;
        // recompute sc for this edge
        if ((-d + b) < 0.0)
            sN = 0;
        else if ((-d + b) > a)
            sN = sD;
        else {
            sN = (-d + b);
            sD = a;
        }
    }

    // finally do the division to get sc and tc
    sc = (std::fabs(sN) < MATRIX_DET_TOLERANCE ? 0.0 : sN / sD);
    // tc is left for future use in the original code
    GetPointOfLine(r, line1, sc);
    return TRUE;
}

// Utility: compare two vectors with tolerance
static bool vecNear(const cmsVEC3& A, const cmsVEC3& B, double tol = 1e-6) {
    return std::fabs(A.x - B.x) < tol &&
           std::fabs(A.y - B.y) < tol &&
           std::fabs(A.z - B.z) < tol;
}

// Utility: pretty print a vector
static void printVec(const cmsVEC3& v) {
    std::cout << std::fixed << std::setprecision(6) << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

// Entry point / test runner
int main() {
    int tests_passed = 0;
    int tests_total = 0;

    std::cout << "Running ClosestLineToLine test suite (self-contained):\n";

    // Test 1: Parallel lines (D ~ 0) -> result should be line1.a
    {
        tests_total++;
        cmsLine line1, line2;
        line1.a = {0.0, 0.0, 0.0};
        line1.u = {1.0, 0.0, 0.0};
        line2.a = {0.0, 1.0, 0.0};
        line2.u = {2.0, 0.0, 0.0}; // parallel to line1.u

        cmsVEC3 r;
        cmsBool ok = ClosestLineToLine(&r, &line1, &line2);
        bool expected_ok = (ok == TRUE);
        cmsVEC3 expected = line1.a; // expect P0 of S1

        bool pass = expected_ok && vecNear(r, expected);
        std::cout << "Test 1 - Parallel lines: " << (pass ? "PASS" : "FAIL") << "  ";
        if (!pass) {
            std::cout << "Expected r = ";
            printVec(expected);
            std::cout << ", Got r = ";
            printVec(r);
        }
        std::cout << "\n";
        if (pass) tests_passed++;
    }

    // Test 2: Non-parallel simple case -> expect sc ~ 0.3, r = line1.a + 0.3 * line1.u
    {
        tests_total++;
        cmsLine line1, line2;
        line1.a = {0.0, 0.0, 0.0};
        line1.u = {1.0, 0.0, 0.0}; // along x-axis
        line2.a = {0.3, 0.0, 0.0}; // shifted along x
        line2.u = {0.70710678, 0.70710678, 0.0}; // 45 degrees in xy-plane

        cmsVEC3 r;
        cmsBool ok = ClosestLineToLine(&r, &line1, &line2);
        bool expected_ok = (ok == TRUE);
        cmsVEC3 expected = {0.3, 0.0, 0.0}; // as derived from sc = 0.3

        bool pass = expected_ok && vecNear(r, expected);
        std::cout << "Test 2 - Non-parallel simple: " << (pass ? "PASS" : "FAIL") << "  ";
        if (!pass) {
            std::cout << "Expected r = ";
            printVec(expected);
            std::cout << ", Got r = ";
            printVec(r);
        }
        std::cout << "\n";
        if (pass) tests_passed++;
    }

    // Test 3: Non-parallel case that exercises tN < 0 path -> expect sc ~ 0.3 again
    // with a2 = (0.3, 0.5, 0); this causes recomputation that lands on r.x ~ 0.3
    {
        tests_total++;
        cmsLine line1, line2;
        line1.a = {0.0, 0.0, 0.0};
        line1.u = {1.0, 0.0, 0.0}; // along x-axis
        line2.a = {0.3, 0.5, 0.0};
        line2.u = {0.70710678, 0.70710678, 0.0}; // 45 degrees in xy-plane

        cmsVEC3 r;
        cmsBool ok = ClosestLineToLine(&r, &line1, &line2);
        bool expected_ok = (ok == TRUE);
        cmsVEC3 expected = {0.3, 0.0, 0.0}; // as derived from sc = 0.3

        bool pass = expected_ok && vecNear(r, expected);
        std::cout << "Test 3 - tN<0 path: " << (pass ? "PASS" : "FAIL") << "  ";
        if (!pass) {
            std::cout << "Expected r = ";
            printVec(expected);
            std::cout << ", Got r = ";
            printVec(r);
        }
        std::cout << "\n";
        if (pass) tests_passed++;
    }

    // Summary
    std::cout << "\nTest suite completed: " << tests_passed << " / " << tests_total << " tests passed.\n";
    return (tests_total == tests_passed) ? 0 : 1;
}