/*
  Lightweight C++11 unit tests for the focal method Compare8bitXFORM
  Note:
  - This test is self-contained and does not rely on external GTest.
  - It provides a minimal mock of the cmsDoTransform and Fail interfaces
    to exercise the Compare8bitXFORM logic without depending on the real
    Little CMS library.
  - The tests cover both branches of the predicate:
      - True branch: identical transforms yield a pass (returns 1).
      - False branch: sufficiently different transforms yield a fail (returns 0).
  - Static/global handling is kept simple since there are no static members
    in this isolated test scenario.
*/

#include <cstring>
#include <iostream>
#include <testcms2.h>
#include <cstdlib>
#include <cstdarg>
#include <cmath>


// Domain-specific type aliases (mimicking the CMS typedefs for testing)
using cmsInt32Number = int;
using cmsUInt8Number = unsigned char;
using cmsHTRANSFORM = void*;

// Constants used by the focal function
static const int cmsMAXCHANNELS = 64; // conservative upper bound for testing

// Global test-state tracking
static int g_num_tests_run = 0;
static int g_num_tests_failed = 0;
static int g_fail_called = 0;

// Minimal Fail implementation to mimic the original behavior.
// It should be non-terminating and record failure counts.
void Fail(const char* frm, ...)
{
    g_fail_called++;
    va_list ap;
    va_start(ap, frm);
    vfprintf(stderr, frm, ap);
    fprintf(stderr, "\n");
    va_end(ap);
}

// Lightweight, mock cmsDoTransform to drive deterministic test behavior.
// It interprets the xform pointer value to decide which transformation to apply.
extern "C" {

void cmsDoTransform(cmsHTRANSFORM xform, const cmsUInt8Number* In, cmsUInt8Number* Out, cmsInt32Number nPixels)
{
    // We consider a single "pixel" with cmsMAXCHANNELS channels.
    // For simplicity, treat In as an array of cmsMAXCHANNELS bytes.
    // Output length must be sufficient for at least cmsMAXCHANNELS entries.

    // Identity transform for xform == (void*)1
    if (xform == (cmsHTRANSFORM)1 || xform == reinterpret_cast<void*>(1)) {
        // Out should mirror In for all channels up to cmsMAXCHANNELS
        for (int i = 0; i < cmsMAXCHANNELS; ++i) {
            Out[i] = In[i];
        }
        return;
    }

    // Invert transform for xform == (void*)2
    if (xform == (cmsHTRANSFORM)2 || xform == reinterpret_cast<void*>(2)) {
        for (int i = 0; i < cmsMAXCHANNELS; ++i) {
            Out[i] = static_cast<unsigned char>(255 - In[i]);
        }
        return;
    }

    // Default: Passthrough (no transform)
    for (int i = 0; i < cmsMAXCHANNELS; ++i) {
        Out[i] = In[i];
    }
}
} // extern "C"

// The focal method under test (re-implemented here for isolation in this test file)
cmsInt32Number Compare8bitXFORM(cmsHTRANSFORM xform1, cmsHTRANSFORM xform2, cmsInt32Number nChan)
{
    {
        cmsInt32Number n2, i, j;
        cmsUInt8Number Inw[cmsMAXCHANNELS], Outw1[cmsMAXCHANNELS], Outw2[cmsMAXCHANNELS];
        n2 = 0;
        for (j = 0; j < 0xFF; j++) {
            // Fill input with the same byte across all channels, as in the real code
            memset(Inw, (unsigned char)j, sizeof(Inw));
            cmsDoTransform(xform1, Inw, Outw1, 1);
            cmsDoTransform(xform2, Inw, Outw2, 1);
            for (i = 0; i < nChan; i++) {
                cmsInt32Number dif = std::abs(static_cast<int>(Outw2[i]) - static_cast<int>(Outw1[i]));
                if (dif > n2) n2 = dif;
            }
        }
        // We allow 2 contone of difference on 8 bits
        if (n2 > 2) {
            Fail("Differences too big (%x)", n2);
            return 0;
        }

        return 1;
    }
}

// Unit test 1: Identical transforms should pass (return 1) and produce no Fail calls.
void test_Compare8bitXFORM_IdenticalTransforms_Pass(void)
{
    g_num_tests_run++;

    cmsHTRANSFORM t1 = (cmsHTRANSFORM)1;
    cmsHTRANSFORM t2 = (cmsHTRANSFORM)1;
    int nChan = 3; // test with multiple channels

    int result = Compare8bitXFORM(t1, t2, nChan);

    // Assertions (non-terminating, to maximize code coverage)
    if (result != 1) {
        std::cerr << "[FAIL] test_Compare8bitXFORM_IdenticalTransforms_Pass: Expected result 1, got " << result << "\n";
        g_num_tests_failed++;
    } else if (g_fail_called != 0) {
        std::cerr << "[FAIL] test_Compare8bitXFORM_IdenticalTransforms_Pass: Unexpected Fail() call(s)\n";
        g_num_tests_failed++;
    } else {
        // Passed
        std::cout << "[PASS] test_Compare8bitXFORM_IdenticalTransforms_Pass\n";
    }
}

// Unit test 2: Different transforms should fail (return 0) due to large differences.
void test_Compare8bitXFORM_DifferentTransforms_Fail(void)
{
    g_num_tests_run++;

    cmsHTRANSFORM t1 = (cmsHTRANSFORM)1;
    cmsHTRANSFORM t2 = (cmsHTRANSFORM)2;
    int nChan = 3; // test with multiple channels

    int result = Compare8bitXFORM(t1, t2, nChan);

    // Assertions
    if (result != 0) {
        std::cerr << "[FAIL] test_Compare8bitXFORM_DifferentTransforms_Fail: Expected result 0, got " << result << "\n";
        g_num_tests_failed++;
    } else if (g_fail_called == 0) {
        std::cerr << "[FAIL] test_Compare8bitXFORM_DifferentTransforms_Fail: Expected Fail() to be called\n";
        g_num_tests_failed++;
    } else {
        // Passed
        std::cout << "[PASS] test_Compare8bitXFORM_DifferentTransforms_Fail\n";
    }
}

// Simple test runner
int main(void)
{
    std::cout << "Running tests for Compare8bitXFORM (self-contained test harness)\n";

    // Reset global state
    g_num_tests_run = 0;
    g_num_tests_failed = 0;
    g_fail_called = 0;

    // Run tests
    test_Compare8bitXFORM_IdenticalTransforms_Pass();
    test_Compare8bitXFORM_DifferentTransforms_Fail();

    // Summary
    std::cout << "Test summary: " << g_num_tests_run << " run, "
              << g_num_tests_failed << " failed, "
              << g_fail_called << " Fail() calls.\n";

    return (g_num_tests_failed == 0) ? 0 : 1;
}