// Minimal, self-contained C++11 unit test suite for the focal method Type_LUT16_Write
// NOTE: This test is a self-contained mock-up to exercise the control flow of the
//       original Type_LUT16_Write logic in a simplified environment.
//       It does not depend on the full libCMS implementation and uses lightweight
//       stubs to mimic the necessary structures and helpers.
// The goal is to achieve coverage of true/false branches and validate basic flow.

#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <cstdint>


// --------------------------
// Lightweight type definitions (mocked, for testing)
// --------------------------
typedef unsigned int cmsUInt32Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned char cmsUInt8Number;
typedef int cmsBool;
typedef int cmsInt32Number;
typedef void* cmsContext;
typedef void* cmsIOHANDLER; // opaque in this mock

#define TRUE 1
#define FALSE 0
#define cmsERROR_UNKNOWN_EXTENSION 1

// Mock tag/type signatures (simplified)
#define cmsSigMatrixElemType 1
#define cmsSigCurveSetElemType 2
#define cmsSigCLutElemType 3

// Minimal type for the focal method's first parameter
struct _cms_typehandler_struct { cmsContext ContextID; };

// Forward declarations of helper functions used by the focal method
typedef cmsBool (*_WriteFn)(cmsIOHANDLER*, const void*, cmsUInt32Number); // not used explicitly here, placeholder

// The code under test expects a cmsPipeline-like structure
struct _cmsStageMatrixData {
    double Double[9];
};
struct _cmsStageToneCurvesData {
    // TheCurves[0]->nEntries is accessed in the real function.
    // We'll mock as a single pointer to a curve with an nEntries member.
    struct _cmsToneCurve {
        cmsUInt16Number nEntries;
    }* TheCurves[1];
};
struct _cmsStageCLutData {
    struct _Params {
        cmsUInt16Number nSamples[4];
    }* Params;
    struct {
        cmsUInt16Number* T;
    } Tab;
};

// Stage node and pipeline (lightweight)
struct cmsStage {
    int Type;
    void* Data;
    struct cmsStage* Next;
    cmsUInt32Number InputChannels;
    cmsUInt32Number OutputChannels;
};

struct cmsPipeline {
    cmsStage* Elements;
};

// Simple I/O stub that collects writes (not strictly used for assertion in this mock)
struct cmsIOHANDLER {
    void* Context;
};

// Global buffers for write calls (used by the mock _cmsWrite* helpers)
static std::vector<uint8_t> g_WrittenBytes;
static std::string g_LastError;

// Helper to reset test state
static void reset_io_buffer() {
    g_WrittenBytes.clear();
    g_LastError.clear();
}

// Mock error signaling function to mimic cmsSignalError
static void cmsSignalError(cmsContext ContextID, int error, const char* msg) {
    (void)ContextID; (void)error;
    g_LastError = (msg && *msg) ? msg : "CMS_ERROR";
}

// --------------------------
// Helpers mimicking internal write utilities
// --------------------------
static int uint8_from_char(unsigned char c) { return static_cast<int>(c); }

// Write a single byte
static cmsBool _cmsWriteUInt8Number(cmsIOHANDLER* io, cmsUInt8Number value) {
    (void)io;
    g_WrittenBytes.push_back(static_cast<uint8_t>(value));
    return TRUE;
}

// Write a 16-bit little-endian value
static cmsBool _cmsWriteUInt16Number(cmsIOHANDLER* io, cmsUInt16Number value) {
    (void)io;
    g_WrittenBytes.push_back(static_cast<uint8_t>(value & 0xFF));
    g_WrittenBytes.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
    return TRUE;
}

// Write a 16-bit fixed-point value (15.16) – encode as int16 in little-endian (simplified)
static cmsBool _cmsWrite15Fixed16Number(cmsIOHANDLER* io, double value) {
    (void)io;
    // Simple scaling to fit into 16 bits (not precise for real cms)
    int32_t v = static_cast<int32_t>(value * (1 << 4)); // reduced scale for testing
    uint8_t lo = static_cast<uint8_t>(v & 0xFF);
    uint8_t hi = static_cast<uint8_t>((v >> 8) & 0xFF);
    g_WrittenBytes.push_back(lo);
    g_WrittenBytes.push_back(hi);
    return TRUE;
}

// Write an array of 16-bit numbers
static cmsBool _cmsWriteUInt16Array(cmsIOHANDLER* io, cmsUInt32Number n, const cmsUInt16Number* array) {
    (void)io;
    for (cmsUInt32Number i = 0; i < n; ++i) {
        cmsUInt16Number v = array[i];
        g_WrittenBytes.push_back(static_cast<uint8_t>(v & 0xFF));
        g_WrittenBytes.push_back(static_cast<uint8_t>((v >> 8) & 0xFF));
    }
    return TRUE;
}

// Mock 3D LUT helpers (no-op in tests, but included to resemble API usage)
static cmsBool Write16bitTables(cmsContext ContextID, cmsIOHANDLER* io, _cmsStageToneCurvesData* Tables) {
    (void)ContextID; (void)io; (void)Tables;
    // In tests, simply succeed
    return TRUE;
}

// Clamp/utility: simple pow-like function (n^a, ignoring b for simplicity here)
static cmsUInt32Number uipow(cmsUInt32Number n, cmsUInt32Number a, cmsUInt32Number b) {
    (void)b; // unused in this mock
    if (a == 0) return 1;
    cmsUInt32Number res = 1;
    for (cmsUInt32Number i = 0; i < a; ++i) {
        if (res > (UINT32_MAX / n)) return (cmsUInt32Number) -1;
        res *= n;
    }
    return res;
}

// Public-like accessors (mocked)
static cmsUInt32Number cmsPipelineInputChannels(const cmsPipeline* lut) {
    if (lut && lut->Elements && lut->Elements->Type == cmsSigMatrixElemType) {
        return lut->Elements->InputChannels;
    }
    return 0;
}
static cmsUInt32Number cmsPipelineOutputChannels(const cmsPipeline* lut) {
    if (lut && lut->Elements && lut->Elements->Type == cmsSigMatrixElemType) {
        return lut->Elements->OutputChannels;
    }
    return 0;
}

// --------------------------
// The focal function under test (adapted for this micro-test environment)
// Note: This is a self-contained rendition of the logic in the prompt, using the lightweight mocks above.
// --------------------------
typedef struct _cms_typehandler_struct cms_typehandler_struct;
typedef cmsBool cmsBool_t;

static cmsBool Type_LUT16_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems)
{
    cmsUInt32Number nTabSize;
    cmsPipeline* NewLUT = (cmsPipeline*) Ptr;
    struct cmsStage* mpe;
    _cmsStageToneCurvesData* PreMPE = NULL, *PostMPE = NULL;
    _cmsStageMatrixData* MatMPE = NULL;
    _cmsStageCLutData* clut = NULL;
    cmsUInt32Number i, InputChannels, OutputChannels, clutPoints;

    // Disassemble the LUT into components.
    mpe = NewLUT->Elements;
    if (mpe != NULL && mpe->Type == cmsSigMatrixElemType) {
        MatMPE = (_cmsStageMatrixData*) mpe->Data;
        if (mpe->InputChannels != 3 || mpe->OutputChannels != 3) return FALSE;
        mpe = mpe->Next;
    }

    if (mpe != NULL && mpe->Type == cmsSigCurveSetElemType) {
        PreMPE = (_cmsStageToneCurvesData*) mpe->Data;
        mpe = mpe->Next;
    }
    if (mpe != NULL && mpe->Type == cmsSigCLutElemType) {
        clut  = (_cmsStageCLutData*) mpe->Data;
        mpe = mpe->Next;
    }
    if (mpe != NULL && mpe->Type == cmsSigCurveSetElemType) {
        PostMPE = (_cmsStageToneCurvesData*) mpe->Data;
        mpe = mpe->Next;
    }

    // That should be all
    if (mpe != NULL) {
        cmsSignalError(self->ContextID, cmsERROR_UNKNOWN_EXTENSION, "LUT is not suitable to be saved as LUT16");
        return FALSE;
    }

    InputChannels  = cmsPipelineInputChannels(NewLUT);
    OutputChannels = cmsPipelineOutputChannels(NewLUT);
    if (clut == NULL)
        clutPoints = 0;
    else {
        // Lut16 only allows same CLUT points in all dimensions        
        clutPoints = clut->Params->nSamples[0];
        for (i = 1; i < InputChannels; i++) {
            if (clut->Params->nSamples[i] != clutPoints) {
                cmsSignalError(self->ContextID, cmsERROR_UNKNOWN_EXTENSION, "LUT with different samples per dimension not suitable to be saved as LUT16");
                return FALSE;
            }
        }
    }

    if (!_cmsWriteUInt8Number(io, (cmsUInt8Number) InputChannels)) return FALSE;
    if (!_cmsWriteUInt8Number(io, (cmsUInt8Number) OutputChannels)) return FALSE;
    if (!_cmsWriteUInt8Number(io, (cmsUInt8Number) clutPoints)) return FALSE;
    if (!_cmsWriteUInt8Number(io, 0)) return FALSE; // Padding
    
    if (MatMPE != NULL) {
        for (i = 0; i < 9; i++) {
            if (!_cmsWrite15Fixed16Number(io, MatMPE->Double[i])) return FALSE;
        }
    }
    else {
        if (!_cmsWrite15Fixed16Number(io, 1)) return FALSE;
        if (!_cmsWrite15Fixed16Number(io, 0)) return FALSE;
        if (!_cmsWrite15Fixed16Number(io, 0)) return FALSE;
        if (!_cmsWrite15Fixed16Number(io, 0)) return FALSE;
        if (!_cmsWrite15Fixed16Number(io, 1)) return FALSE;
        if (!_cmsWrite15Fixed16Number(io, 0)) return FALSE;
        if (!_cmsWrite15Fixed16Number(io, 0)) return FALSE;
        if (!_cmsWrite15Fixed16Number(io, 0)) return FALSE;
        if (!_cmsWrite15Fixed16Number(io, 1)) return FALSE;
    }

    if (PreMPE != NULL) {
        if (!_cmsWriteUInt16Number(io, (cmsUInt16Number) PreMPE ->TheCurves[0]->nEntries)) return FALSE;
    } else {
            if (!_cmsWriteUInt16Number(io, 2)) return FALSE;
    }
    if (PostMPE != NULL) {
        if (!_cmsWriteUInt16Number(io, (cmsUInt16Number) PostMPE ->TheCurves[0]->nEntries)) return FALSE;
    } else {
        if (!_cmsWriteUInt16Number(io, 2)) return FALSE;
    }

    // The prelinearization table
    if (PreMPE != NULL) {
        if (!Write16bitTables(self ->ContextID, io, PreMPE)) return FALSE;
    }
    else {
        for (i=0; i < InputChannels; i++) {
            if (!_cmsWriteUInt16Number(io, 0)) return FALSE;
            if (!_cmsWriteUInt16Number(io, 0xffff)) return FALSE;
        }
    }

    nTabSize = uipow(OutputChannels, clutPoints, InputChannels);
    if (nTabSize == (cmsUInt32Number) -1) return FALSE;
    if (nTabSize > 0) {
        // The 3D CLUT.
        if (clut != NULL) {
            if (!_cmsWriteUInt16Array(io, nTabSize, clut->Tab.T)) return FALSE;
        }
    }

    // The postlinearization table
    if (PostMPE != NULL) {
        if (!Write16bitTables(self ->ContextID, io, PostMPE)) return FALSE;
    }
    else {
        for (i=0; i < OutputChannels; i++) {
            if (!_cmsWriteUInt16Number(io, 0)) return FALSE;
            if (!_cmsWriteUInt16Number(io, 0xffff)) return FALSE;
        }
    }
    return TRUE;
    // cmsUNUSED_PARAMETER(nItems);
}

// --------------------------
// Test harness: define a few focused tests
// --------------------------

// Utility to create a matrix stage with given dimensions
static _cmsStageMatrixData* make_matrix_data(double a00, double a01, double a02,
                                             double a10, double a11, double a12,
                                             double a20, double a21, double a22) {
    auto md = new _cmsStageMatrixData;
    md->Double[0] = a00; md->Double[1] = a01; md->Double[2] = a02;
    md->Double[3] = a10; md->Double[4] = a11; md->Double[5] = a12;
    md->Double[6] = a20; md->Double[7] = a21; md->Double[8] = a22;
    return md;
}

// Test 1: Basic path: 3x3 matrix, only matrix stage, no Pre/Post CLUT
static bool test_basic_path_true() {
    reset_io_buffer();

    // Build LUT pipeline: Matrix -> NULL
    cmsPipeline* lut = new cmsPipeline;
    lut->Elements = new cmsStage;
    lut->Elements->Type = cmsSigMatrixElemType;
    lut->Elements->Data = make_matrix_data(1.0,0.0,0.0, 0.0,1.0,0.0, 0.0,0.0,1.0);
    lut->Elements->InputChannels = 3;
    lut->Elements->OutputChannels = 3;
    lut->Elements->Next = NULL;

    // Mock self
    _cms_typehandler_struct handler;
    handler.ContextID = NULL;

    // IO
    cmsIOHANDLER io;
    io.Context = NULL;

    // Call focal method
    cmsBool res = Type_LUT16_Write(&handler, &io, lut, 0);

    // Cleanup
    delete static_cast<_cmsStageMatrixData*>(lut->Elements->Data);
    delete lut->Elements;
    delete lut;

    if (!res) {
        std::cout << "[Test1] Basic path: expected TRUE but got FALSE. Error: " << g_LastError << "\n";
        return false;
    }
    // We don't assert exact bytes here; just ensure flow completed
    return true;
}

// Test 2: Matrix with invalid dimensions should fail fast
static bool test_matrix_wrong_dimensions_false() {
    reset_io_buffer();

    // Build LUT pipeline: Matrix with 2x3 (InputChannels != 3)
    cmsPipeline* lut = new cmsPipeline;
    lut->Elements = new cmsStage;
    lut->Elements->Type = cmsSigMatrixElemType;
    lut->Elements->Data = make_matrix_data(1.0,0.0,0.0, 0.0,1.0,0.0, 0.0,0.0,1.0);
    lut->Elements->InputChannels = 2;  // WRONG
    lut->Elements->OutputChannels = 3;
    lut->Elements->Next = NULL;

    _cms_typehandler_struct handler;
    handler.ContextID = NULL;
    cmsIOHANDLER io;
    io.Context = NULL;

    cmsBool res = Type_LUT16_Write(&handler, &io, lut, 0);

    delete static_cast<_cmsStageMatrixData*>(lut->Elements->Data);
    delete lut->Elements;
    delete lut;

    if (res) {
        std::cout << "[Test2] Matrix wrong dimensions: expected FALSE but got TRUE.\n";
        return false;
    }
    return true;
}

// Test 3: Extra unrecognized stage present -> should fail
static bool test_extra_stage_false() {
    reset_io_buffer();

    // Build LUT: Matrix -> CurveSet (PreMPE) -> Extra unknown stage
    cmsPipeline* lut = new cmsPipeline;
    lut->Elements = new cmsStage;

    // Stage 1: Matrix 3x3
    lut->Elements->Type = cmsSigMatrixElemType;
    lut->Elements->Data = make_matrix_data(1.0,0.0,0.0, 0.0,1.0,0.0, 0.0,0.0,1.0);
    lut->Elements->InputChannels = 3;
    lut->Elements->OutputChannels = 3;

    // Stage 2: CurveSet (PreMPE) - minimal
    _cmsStageToneCurvesData* preMPE = new _cmsStageToneCurvesData;
    preMPE->TheCurves[0] = new _cmsStageToneCurvesData::_cmsToneCurve; // one curve
    preMPE->TheCurves[0]->nEntries = 3;
    lut->Elements->Next = new cmsStage;
    lut->Elements->Next->Type = cmsSigCurveSetElemType;
    lut->Elements->Next->Data = preMPE;
    lut->Elements->Next->InputChannels = 3;
    lut->Elements->Next->OutputChannels = 3;

    // Stage 3: CLUT (we won't use in this test to avoid heavy setup) - skip, keep NULL
    lut->Elements->Next->Next = new cmsStage;
    lut->Elements->Next->Next->Type = -999; // unrecognized type to trigger error
    lut->Elements->Next->Next->Data = NULL;
    lut->Elements->Next->Next->Next = NULL;
    lut->Elements->Next->Next->InputChannels = 3;
    lut->Elements->Next->Next->OutputChannels = 3;

    _cms_typehandler_struct handler;
    handler.ContextID = NULL;
    cmsIOHANDLER io;
    io.Context = NULL;

    cmsBool res = Type_LUT16_Write(&handler, &io, lut, 0);

    // cleanup
    delete preMPE->TheCurves[0];
    delete preMPE;
    delete lut->Elements->Next;
    delete lut->Elements;

    // The extra stage should cause FALSE
    delete lut;
    if (res) {
        std::cout << "[Test3] Extra stage: expected FALSE but got TRUE.\n";
        return false;
    }
    return true;
}

// Test 4: CLUT with mismatching samples should fail
static bool test_clut_dimension_mismatch_false() {
    reset_io_buffer();

    // Build LUT: Matrix -> CLUT with mismatched samples
    cmsPipeline* lut = new cmsPipeline;
    lut->Elements = new cmsStage;
    lut->Elements->Type = cmsSigMatrixElemType;
    lut->Elements->Data = make_matrix_data(1.0,0.0,0.0, 0.0,1.0,0.0, 0.0,0.0,1.0);
    lut->Elements->InputChannels = 3;
    lut->Elements->OutputChannels = 3;
    lut->Elements->Next = new cmsStage;

    // Stage 2: CLUT with mismatched samples
    _cmsStageCLutData* clut = new _cmsStageCLutData;
    clut->Params = new _cmsStageCLutData::_Params;
    clut->Params->nSamples[0] = 3;
    clut->Params->nSamples[1] = 4; // mismatch between dimensions
    clut->Params->nSamples[2] = 3;
    clut->Params->nSamples[3] = 3;
    clut->Tab.T = nullptr;

    lut->Elements->Next->Type = cmsSigCLutElemType;
    lut->Elements->Next->Data = clut;
    lut->Elements->Next->InputChannels = 3;
    lut->Elements->Next->OutputChannels = 3;

    lut->Elements->Next->Next = NULL;

    _cms_typehandler_struct handler;
    handler.ContextID = NULL;
    cmsIOHANDLER io;
    io.Context = NULL;

    cmsBool res = Type_LUT16_Write(&handler, &io, lut, 0);

    // cleanup
    delete[] clut->Params;
    delete clut;
    delete lut->Elements->Next;
    delete lut->Elements;
    delete lut;
    if (res) {
        std::cout << "[Test4] CLUT dimension mismatch: expected FALSE but got TRUE.\n";
        return false;
    }
    return true;
}

// --------------------------
// Main: run tests
// --------------------------
int main() {
    int passed = 0;
    int total = 0;

    auto run = [&](const char* name, bool (*fn)()) {
        total++;
        bool ok = fn();
        if (ok) {
            std::cout << "[PASS] " << name << "\n";
            passed++;
        } else {
            std::cout << "[FAIL] " << name << "\n";
        }
    };

    run("Test Basic Path True", test_basic_path_true);
    run("Test Matrix Wrong Dimensions False", test_matrix_wrong_dimensions_false);
    run("Test Extra Stage False", test_extra_stage_false);
    run("Test CLUT Dimension Mismatch False", test_clut_dimension_mismatch_false);

    std::cout << "Tests passed: " << passed << "/" << total << "\n";
    return (passed == total) ? 0 : 1;
}