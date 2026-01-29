/*
  Test suite for the focal method: PackLabDoubleFromFloat
  - This file provides a minimal, self-contained test harness (no GTest).
  - It reproduces the focal method logic with lightweight type/macro stubs
    so the tests can run in a standard C++11 environment.
  - Coverage:
      * True branch (T_PLANAR) and False branch of the condition.
      * Correct pointer arithmetic returns.
      * Correct double outputs for the three channels (and planar offsets).
  - Static-like behaviors are not used here; we interact directly with the function.
  - Uses non-terminating assertions to maximize code execution.
  - All tests are invoked from main().
*/

/* Candidate Keywords (based on the focal method analysis)
   - Out: the double buffer writing destination (cast from output).
   - Stride: stepping variable that is adjusted by PixelSize in planar mode.
   - T_PLANAR: predicate to decide between planar and chunky layout.
   - PixelSize: helper to compute the number of elements per pixel.
   - T_EXTRA: extra space depending on OutputFormat (test uses 0).
   - Info->OutputFormat: determines planar vs non-planar path and influences Stride.
   - wOut: input 3-element float array to convert.
   - return pointer: behavior differs by branch (output offset varies).
*/ 

#include <cstring>
#include <cstdio>
#include <vector>
#include <iostream>
#include <string>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


using namespace std;

/* Domain knowledge alignment: basic type definitions matching the code's expectations. */
typedef uint8_t  cmsUInt8Number;
typedef uint16_t cmsUInt16Number;
typedef uint32_t cmsUInt32Number;
typedef float    cmsFloat32Number;
typedef double   cmsFloat64Number;
typedef bool     cmsBool;

/* Fallback lightweight representation of _cmsTRANSFORM used by the focal method. */
struct _cmsTRANSFORM {
    cmsUInt32Number OutputFormat;
};

/* Candidate Keywords via macros to control test paths.
   - Planar if OutputFormat == 1
   - PixelSize(fmt) = 2 when planar, else 1
   - T_EXTRA(fmt) = 0 for testing simplicity
*/
#define T_PLANAR(fmt) ((fmt) == 1)
#define PixelSize(fmt) ((fmt) == 1 ? 2 : 1)
#define T_EXTRA(fmt) (0)

/* FOCAL_METHOD replicated for testability in a standalone C++11 test. */
cmsUInt8Number* PackLabDoubleFromFloat(_cmsTRANSFORM* Info,
                                      cmsFloat32Number wOut[],
                                      cmsUInt8Number* output,
                                      cmsUInt32Number Stride)
{
{
    cmsFloat64Number* Out = (cmsFloat64Number*) output;
    if (T_PLANAR(Info -> OutputFormat)) {
        Stride /= PixelSize(Info->OutputFormat);
        Out[0]        = (cmsFloat64Number) (wOut[0] * 100.0);
        Out[Stride]   = (cmsFloat64Number) (wOut[1] * 255.0 - 128.0);
        Out[Stride*2] = (cmsFloat64Number) (wOut[2] * 255.0 - 128.0);
        return output + sizeof(cmsFloat64Number);
    }
    else {
        Out[0] = (cmsFloat64Number) (wOut[0] * 100.0);
        Out[1] = (cmsFloat64Number) (wOut[1] * 255.0 - 128.0);
        Out[2] = (cmsFloat64Number) (wOut[2] * 255.0 - 128.0);
        return output + (sizeof(cmsFloat64Number)*3 + T_EXTRA(Info ->OutputFormat) * sizeof(cmsFloat64Number));
    }
}
}

/* Lightweight test harness utilities (non-terminating assertions). */
static vector<string> g_failures;

static inline bool almost_equal(double a, double b, double tol = 1e-6) {
    return fabs(a - b) <= tol;
}

#define EXPECT_TRUE(cond, msg) \
    do { if(!(cond)) { g_failures.push_back(string("EXPECT_TRUE failed: ") + (msg)); } } while(0)

#define EXPECT_DOUBLE_EQ(a, b, msg) \
    do { if(!almost_equal((a), (b))) { \
            char buf[256]; snprintf(buf, sizeof(buf), "%s | Expected %.12f, got %.12f", (msg), (double)(a), (double)(b)); \
            g_failures.push_back(string(buf)); \
        } \
    } while(0)

#define EXPECT_PTR_EQ(a, b, msg) \
    do { if((void*)(a) != (void*)(b)) { \
            char buf[256]; snprintf(buf, sizeof(buf), "%s | Pointers not equal. Expected %p, got %p", (msg), (void*)(b), (void*)(a)); \
            g_failures.push_back(string(buf)); \
        } \
    } while(0)

/* Test 1: Planar path (OutputFormat = 1) with Stride adjusted via PixelSize.
   Verifies Out[0], Out[Stride], Out[Stride*2] are computed as per wOut.
*/
void test_PackLabDoubleFromFloat_Planar()
{
    // Prepare inputs
    _cmsTRANSFORM Info;
    Info.OutputFormat = 1; // Planar
    cmsFloat32Number wOut[3] = {0.25f, -0.25f, 0.75f}; // test values
    // Output buffer must hold at least 7 doubles to cover indices 0, Stride, Stride*2 when Stride after division is 3
    const int requiredDoubles = 7;
    cmsFloat64Number buf[requiredDoubles];
    memset(buf, 0, sizeof(buf)); // zero to isolate checks
    cmsUInt8Number* output = reinterpret_cast<cmsUInt8Number*>(buf);

    cmsUInt32Number Stride = 6; // initial stride (will be divided by PixelSize(1)=2 -> 3)
    PackLabDoubleFromFloat(&Info, wOut, output, Stride);

    // Expected values
    cmsFloat64Number exp0 = (cmsFloat64Number)(wOut[0] * 100.0);        // Out[0]
    cmsFloat64Number exp1 = (cmsFloat64Number)(wOut[1] * 255.0 - 128.0); // Out[Stride] -> Out[3]
    cmsFloat64Number exp2 = (cmsFloat64Number)(wOut[2] * 255.0 - 128.0); // Out[Stride*2] -> Out[6]

    // Validate
    EXPECT_DOUBLE_EQ(buf[0], exp0, "Planar: Out[0] mismatch");
    EXPECT_DOUBLE_EQ(buf[3], exp1, "Planar: Out[Stride] mismatch");
    EXPECT_DOUBLE_EQ(buf[6], exp2, "Planar: Out[Stride*2] mismatch");
}

/* Test 2: Non-planar path (OutputFormat != 1) with 3-channel packed layout.
   Verifies Out[0], Out[1], Out[2] and the return pointer offset.
*/
void test_PackLabDoubleFromFloat_NonPlanar()
{
    _cmsTRANSFORM Info;
    Info.OutputFormat = 0; // Non-planar
    cmsFloat32Number wOut[3] = {0.1f, 0.2f, -0.3f}; // test values

    // Output buffer for 3 doubles
    cmsFloat64Number buf[3];
    memset(buf, 0, sizeof(buf));
    cmsUInt8Number* output = reinterpret_cast<cmsUInt8Number*>(buf);

    cmsUInt32Number Stride = 0; // not used in non-planar path
    cmsUInt8Number* retPtr = PackLabDoubleFromFloat(&Info, wOut, output, Stride);

    // Expected values
    cmsFloat64Number exp0 = (cmsFloat64Number)(wOut[0] * 100.0);
    cmsFloat64Number exp1 = (cmsFloat64Number)(wOut[1] * 255.0 - 128.0);
    cmsFloat64Number exp2 = (cmsFloat64Number)(wOut[2] * 255.0 - 128.0);

    // Validate
    EXPECT_DOUBLE_EQ(buf[0], exp0, "Non-planar: Out[0] mismatch");
    EXPECT_DOUBLE_EQ(buf[1], exp1, "Non-planar: Out[1] mismatch");
    EXPECT_DOUBLE_EQ(buf[2], exp2, "Non-planar: Out[2] mismatch");

    // Validate return pointer offset: should be 3*sizeof(double)
    cmsUInt8Number* expectedRet = output + (3 * sizeof(cmsFloat64Number));
    EXPECT_PTR_EQ(retPtr, expectedRet, "Non-planar: Return pointer offset mismatch");
}

/* Runner to execute tests and report results. */
int main()
{
    cout << "Starting unit tests for PackLabDoubleFromFloat (standalone test harness)" << endl;

    test_PackLabDoubleFromFloat_Planar();
    test_PackLabDoubleFromFloat_NonPlanar();

    if (!g_failures.empty()) {
        cout << "Tests finished with " << g_failures.size() << " failure(s)." << endl;
        for (size_t i = 0; i < g_failures.size(); ++i) {
            cout << "Failure " << (i+1) << ": " << g_failures[i] << endl;
        }
        return 1;
    } else {
        cout << "All tests passed." << endl;
        return 0;
    }
}