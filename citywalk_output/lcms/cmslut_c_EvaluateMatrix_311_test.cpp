// Unit tests for EvaluateMatrix in cmslut.c
// This test targets the focal method by constructing minimal mock structures
// that mimic the necessary fields (Data, InputChannels, OutputChannels) and
// invoking EvaluateMatrix via extern "C" linkage. No GoogleTest; a small
// homemade test harness with lightweight assertions is used.

#include <vector>
#include <iostream>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


// Forward declaration of the function under test (C linkage)
extern "C" void EvaluateMatrix(const cmsFloat32Number In[],
                               cmsFloat32Number Out[],
                               const cmsStage *mpe);

// Type aliases matching cms types (as used by the focal code)
typedef float  cmsFloat32Number;
typedef double cmsFloat64Number;
typedef uint32_t cmsUInt32Number;

// Lightweight mock of the internal data structure used by EvaluateMatrix.
// We assume the real code uses a struct with fields named Double and Offset.
typedef struct _cmsStageMatrixData {
    cmsFloat64Number* Double;
    cmsFloat64Number* Offset;
} _cmsStageMatrixData;

// Lightweight mock of cmsStage, exposing only the fields used by the focal method.
typedef struct {
    void* Data;
    cmsUInt32Number InputChannels;
    cmsUInt32Number OutputChannels;
} cmsStage;

// Simple, non-terminating assertion helper: logs failure but continues.
static void failIfFalse(bool condition, const char* message)
{
    if (!condition) {
        std::cerr << "Test failure: " << message << std::endl;
    }
}

// Approximate float comparison with tolerance
static bool almostEqual(float a, float b, float tol = 1e-5f)
{
    return std::abs(static_cast<double>(a) - static_cast<double>(b)) <= static_cast<double>(tol);
}

/*
Test 1: Basic 2x2 matrix with no offset.
- In: 2 inputs
- Data->Double: 4 values (row-major: i=0..1, j=0..1)
- Offset is NULL
- Expected Out: Out[0] = In0*D[0] + In1*D[1], Out[1] = In0*D[2] + In1*D[3]
*/
static bool test_EvaluateMatrix_basic_no_offset()
{
    // Prepare inputs
    const cmsUInt32Number InputChannels = 2;
    const cmsUInt32Number OutputChannels = 2;
    cmsFloat32Number In[2] = { 0.5f, 0.5f }; // 0..1 domain
    cmsFloat32Number Out[2] = { 0.0f, 0.0f };

    // Matrix values (Double): 2x2
    // Row 0: 0.5, 0.3
    // Row 1: 0.2, 0.4
    cmsFloat64Number* mat = new cmsFloat64Number[4];
    mat[0] = 0.5; mat[1] = 0.3;
    mat[2] = 0.2; mat[3] = 0.4;

    // Data with NULL Offset
    cmsFloat64Number* offsetArr = nullptr;
    _cmsStageMatrixData* data = new _cmsStageMatrixData;
    data->Double = mat;
    data->Offset = offsetArr;

    // Stage setup
    cmsStage mpe;
    mpe.Data = static_cast<void*>(data);
    mpe.InputChannels = InputChannels;
    mpe.OutputChannels = OutputChannels;

    // Call the function under test
    EvaluateMatrix(In, Out, &mpe);

    // Expected results
    float exp0 = static_cast<float>(0.5 * 0.5 + 0.5 * 0.3); // 0.25 + 0.15 = 0.40
    float exp1 = static_cast<float>(0.5 * 0.2 + 0.5 * 0.4); // 0.10 + 0.20 = 0.30

    bool ok = almostEqual(Out[0], exp0) && almostEqual(Out[1], exp1);

    // Cleanup
    delete[] mat;
    delete data;

    if (!ok) {
        failIfFalse(ok, "EvaluateMatrix basic no offset failed: incorrect outputs");
    }
    return ok;
}

/*
Test 2: Basic 2x2 matrix with non-null offset.
- In: 2 inputs
- Data->Double as in Test 1
- Offset: [0.1, 0.02]
- Expected Out: Out[0] = 0.40 + 0.1 = 0.50, Out[1] = 0.30 + 0.02 = 0.32
*/
static bool test_EvaluateMatrix_with_offset()
{
    // Prepare inputs
    const cmsUInt32Number InputChannels = 2;
    const cmsUInt32Number OutputChannels = 2;
    cmsFloat32Number In[2] = { 0.5f, 0.5f };
    cmsFloat32Number Out[2] = { 0.0f, 0.0f };

    cmsFloat64Number* mat = new cmsFloat64Number[4];
    mat[0] = 0.5; mat[1] = 0.3;
    mat[2] = 0.2; mat[3] = 0.4;

    cmsFloat64Number* offsetArr = new cmsFloat64Number[2];
    offsetArr[0] = 0.1;
    offsetArr[1] = 0.02;

    _cmsStageMatrixData* data = new _cmsStageMatrixData;
    data->Double = mat;
    data->Offset = offsetArr;

    cmsStage mpe;
    mpe.Data = static_cast<void*>(data);
    mpe.InputChannels = InputChannels;
    mpe.OutputChannels = OutputChannels;

    EvaluateMatrix(In, Out, &mpe);

    float exp0 = static_cast<float>(0.4 + 0.1);  // 0.5*0.5 + 0.5*0.3 + 0.1
    float exp1 = static_cast<float>(0.3 + 0.02); // 0.5*0.2 + 0.5*0.4 + 0.02

    bool ok = almostEqual(Out[0], exp0) && almostEqual(Out[1], exp1);

    delete[] mat;
    delete[] offsetArr;
    delete data;

    if (!ok) {
        failIfFalse(ok, "EvaluateMatrix with offset failed: incorrect outputs");
    }
    return ok;
}

/*
Test 3: 3-input x 2-output matrix to verify indexing with more inputs.
- In: 3 inputs
- Data->Double: 2x3 matrix
- Offset: NULL
*/
static bool test_EvaluateMatrix_three_inputs()
{
    const cmsUInt32Number InputChannels = 3;
    const cmsUInt32Number OutputChannels = 2;
    cmsFloat32Number In[3] = { 0.2f, 0.3f, 0.4f };
    cmsFloat32Number Out[2] = { 0.0f, 0.0f };

    // Row 0: [0.5, 0.25, 0.75]
    // Row 1: [0.1, 0.60, 0.20]
    cmsFloat64Number* mat = new cmsFloat64Number[6];
    mat[0] = 0.5;  mat[1] = 0.25; mat[2] = 0.75;
    mat[3] = 0.1;  mat[4] = 0.60; mat[5] = 0.20;

    _cmsStageMatrixData* data = new _cmsStageMatrixData;
    data->Double = mat;
    data->Offset = nullptr;

    cmsStage mpe;
    mpe.Data = static_cast<void*>(data);
    mpe.InputChannels = InputChannels;
    mpe.OutputChannels = OutputChannels;

    EvaluateMatrix(In, Out, &mpe);

    // Compute expected:
    // Out0 = 0.2*0.5 + 0.3*0.25 + 0.4*0.75 = 0.1 + 0.075 + 0.3 = 0.475
    // Out1 = 0.2*0.1 + 0.3*0.60 + 0.4*0.20  = 0.02 + 0.18 + 0.08 = 0.28
    float exp0 = 0.475f;
    float exp1 = 0.28f;
    bool ok = almostEqual(Out[0], exp0) && almostEqual(Out[1], exp1);

    delete[] mat;
    delete data;

    if (!ok) {
        failIfFalse(ok, "EvaluateMatrix three inputs failed: incorrect outputs");
    }
    return ok;
}

int main()
{
    int failures = 0;

    if (!test_EvaluateMatrix_basic_no_offset()) {
        ++failures;
    }

    if (!test_EvaluateMatrix_with_offset()) {
        ++failures;
    }

    if (!test_EvaluateMatrix_three_inputs()) {
        ++failures;
    }

    if (failures == 0) {
        std::cout << "All EvaluateMatrix tests PASSED." << std::endl;
    } else {
        std::cout << "EvaluateMatrix tests FAILED: " << failures << " failure(s)." << std::endl;
    }

    return failures;
}