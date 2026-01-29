// Comprehensive unit test suite for OptimizeMatrixShaper15
// This test suite provides a self-contained mock environment to exercise
// the focal method along with its core dependencies without GTest.
// It uses a lightweight, non-terminating EXPECT framework and runs in
// a single main() function.

#include <cstring>
#include <cstdio>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <cstdlib>


// Domain-specific mock definitions and lightweight test framework

// Lightweight types (mocked for test purposes)
typedef int cmsBool;
typedef unsigned int cmsUInt32Number;
typedef int cmsContext;
typedef double cmsFloat64Number;
typedef double cmsVEC3[3];

// Public-ish constants (mock)
static const int cmsAT_BEGIN = 0;
static const int cmsAT_END   = 1;

static const int cmsSigCurveSetElemType = 1;
static const int cmsSigMatrixElemType     = 2;

static const unsigned int cmsFLAGS_NOCACHE = 0x01;
static const unsigned int cmsFLAGS_CAN_CHANGE_FORMATTER = 0x02;

// Basic numeric matrix type (mock)
struct cmsMAT3 {
    double m[3][3];
};

// Forward declare to satisfy references in the focal method
struct _cmstransform_struct {};

// Stage and Pipeline mocks
struct cmsStage {
    int Type;
    void* Data;
    // Simple constructor helper (not used in production)
    cmsStage(int t=0, void* d=nullptr) : Type(t), Data(d) {}
};

struct cmsPipeline {
    std::vector<cmsStage*> Stage;
};

// Data payloads for stages
struct cmsToneCurve; // opaque type for mock
struct _cmsStageToneCurvesData {
    cmsToneCurve* TheCurves[3];
};

struct _cmsStageMatrixData {
    void* Offset;
    void* Double; // intended to point to cmsMAT3
};

// Dummy ToneCurve type
struct cmsToneCurve {
    int dummy;
};

// Function pointer typedefs (mock signatures)
typedef void (*_cmsTransform2Fn)(struct _cmstransform_struct*, const void*, void*, cmsUInt32Number, cmsUInt32Number, const void*);
typedef void (*_cmsFreeUserDataFn)(void*);

// SetMatShaper/FreeMatShaper function signatures
void* SetMatShaper(cmsContext ContextID,
                   cmsToneCurve* Curve1[3],
                   cmsMAT3* Mat,
                   cmsVEC3* Off,
                   cmsToneCurve* Curve2[3],
                   cmsBool IdentityMat);
void FreeMatShaper(cmsContext ContextID, void* Data);

// MatShaper transform (mock)
void MatShaperXform(struct _cmstransform_struct *CMMcargo,
                    const void* Input,
                    void* Output,
                    cmsUInt32Number PixelsPerLine,
                    cmsUInt32Number LineCount,
                    const void* Stride);

// Helpers
static cmsContext g_ContextID = 1;

// Debug/test helpers
static int g_TestFailures = 0;
#define EXPECT_TRUE(cond, msg) do { if(!(cond)) { ++g_TestFailures; fprintf(stderr, "EXPECT_TRUE failed: %s\n", msg); } } while(0)
#define EXPECT_FALSE(cond, msg) do { if((cond)) { ++g_TestFailures; fprintf(stderr, "EXPECT_FALSE failed: %s\n", msg); } } while(0)
#define EXPECT_EQ(a,b, msg) do { if(((a) != (b))) { ++g_TestFailures; fprintf(stderr, "EXPECT_EQ failed: %s (exp %d, got %d)\n", msg, (int)(b), (int)(a)); } } while(0)

// Macros to mimic CMS format macros (mock behavior)
#define T_CHANNELS(fmt) ((cmsUInt32Number)((fmt) & 0xFF))
#define T_BYTES(fmt)    ((cmsUInt32Number)(((fmt) >> 8) & 0xFF))
#define T_BIT15(fmt)    ((cmsUInt32Number)(((fmt) >> 16) & 0x01))

// Helpers for test setup
static cmsMAT3 IdentityMatrix() {
    cmsMAT3 I;
    for (int i=0;i<3;++i) for (int j=0;j<3;++j) I.m[i][j] = (i==j) ? 1.0 : 0.0;
    return I;
}
static cmsMAT3 NonIdentityMatrix() {
    cmsMAT3 M = IdentityMatrix();
    M.m[0][0] = 2.0;
    M.m[1][1] = 3.0;
    M.m[2][2] = 4.0;
    return M;
}

// Matrix multiply (dst = B * A)
static void _cmsMAT3per(cmsMAT3* dst, const cmsMAT3* B, const cmsMAT3* A) {
    for (int i=0;i<3;++i) {
        for (int j=0;j<3;++j) {
            double s = 0.0;
            for (int k=0;k<3;++k) s += B->m[i][k] * A->m[k][j];
            dst->m[i][j] = s;
        }
    }
}

// Identity check
static int _cmsMAT3isIdentity(const cmsMAT3* M) {
    for (int i=0;i<3;++i)
        for (int j=0;j<3;++j)
            if ((i==j && M->m[i][j] != 1.0) || (i!=j && M->m[i][j] != 0.0))
                return 0;
    return 1;
}

// Pipeline helpers (mocked)
static cmsUInt32Number cmsGetPipelineContextID(cmsPipeline* /*Src*/) {
    return g_ContextID;
}
static cmsStage* cmsStageDup(const cmsStage* s) {
    if (!s) return nullptr;
    cmsStage* n = new cmsStage;
    n->Type = s->Type;
    // Shallow copy Data
    n->Data = s->Data;
    return n;
}
static cmsStage* cmsStageAllocMatrix(cmsContext ContextID, int /*Rows*/, int /*Cols*/, const cmsFloat64Number* Matrix, void* Offset) {
    cmsStage* s = new cmsStage;
    s->Type = cmsSigMatrixElemType;
    _cmsStageMatrixData* data = new _cmsStageMatrixData;
    data->Offset = Offset;
    // Copy the 3x3 matrix into a freshly allocated cmsMAT3
    cmsMAT3* mat = new cmsMAT3;
    for (int r=0;r<3;++r)
        for (int c=0;c<3;++c)
            mat->m[r][c] = Matrix ? Matrix[r*3 + c] : 0.0;
    data->Double = (void*)mat;
    s->Data = (void*)data;
    return s;
}
static cmsStage* cmsStageAllocCurve(cmsContext ContextID) {
    cmsStage* s = new cmsStage;
    s->Type = cmsSigCurveSetElemType;
    _cmsStageToneCurvesData* data = new _cmsStageToneCurvesData;
    // Create 3 dummy curves
    data->TheCurves[0] = new cmsToneCurve{0};
    data->TheCurves[1] = new cmsToneCurve{0};
    data->TheCurves[2] = new cmsToneCurve{0};
    s->Data = (void*)data;
    return s;
}
static void cmsPipelineInsertStage(cmsPipeline* Dest, int /*StageOrder*/, cmsStage* Stage) {
    if (!Dest || !Stage) return;
    if (Dest->Stage.empty()) {
        Dest->Stage.push_back(Stage);
    } else {
        // For cmsAT_BEGIN treat as prepend
        // if StageOrder==cmsAT_BEGIN -> at start else at end
        Dest->Stage.push_back(Stage); // keep simple: append
    }
}
static cmsPipeline* cmsPipelineAlloc(cmsContext ContextID, cmsUInt32Number /*In*/, cmsUInt32Number /*Out*/) {
    cmsPipeline* p = new cmsPipeline;
    return p;
}
static void cmsPipelineFree(cmsPipeline* p) {
    if (!p) return;
    for (auto st : p->Stage) {
        // Free Data payloads selectively
        if (st) {
            if (st->Data) {
                // Free known payloads
                if (st->Type == cmsSigMatrixElemType) {
                    _cmsStageMatrixData* d = (_cmsStageMatrixData*)st->Data;
                    if (d->Double) {
                        cmsMAT3* m = (cmsMAT3*)d->Double;
                        delete m;
                    }
                    delete d;
                } else if (st->Type == cmsSigCurveSetElemType) {
                    _cmsStageToneCurvesData* d = (_cmsStageToneCurvesData*)st->Data;
                    if (d) {
                        delete d->TheCurves[0];
                        delete d->TheCurves[1];
                        delete d->TheCurves[2];
                        delete d;
                    }
                } else {
                    // Other types: leave as is
                    // free if needed
                }
            }
            delete st;
        }
    }
    delete p;
}
static void cmsPipelineFreeV(cmsPipeline* p) { cmsPipelineFree(p); }

// Pipeline stage data helper
static void* cmsStageData(cmsStage* s) { return s ? s->Data : nullptr; }

// Pipeline stage check helper (mock)
static int cmsPipelineCheckAndRetreiveStages(cmsPipeline* Src, int n,
                                           int sig1, int sig2, int sig3, int sig4,
                                           cmsStage** Curve1, cmsStage** Matrix1,
                                           cmsStage** Matrix2, cmsStage** Curve2) {
    if (!Src || (int)Src->Stage.size() != 4) return 0;
    *Curve1  = Src->Stage[0];
    *Matrix1 = Src->Stage[1];
    *Matrix2 = Src->Stage[2];
    *Curve2  = Src->Stage[3];
    // Very light type check
    if (!(*Curve1) || !(*Matrix1) || !(*Matrix2) || !(*Curve2)) return 0;
    return 1;
}

// Get a simple format for test inputs (bitfields)
static CmsFake = 0; // placeholder to avoid warnings

// Identity check for test
static int resolveIdentityFlag(const cmsMAT3* res, void* Offset) {
    (void)res; (void)Offset;
    return 0;
}

// External function prototypes (mock)
void* SetMatShaper(cmsContext ContextID,
                   cmsToneCurve* Curve1[3],
                   cmsMAT3* Mat,
                   cmsVEC3* Off,
                   cmsToneCurve* Curve2[3],
                   cmsBool IdentityMat) {
    // Return a non-null dummy pointer
    static int dummy = 0;
    (void)ContextID; (void)Curve1; (void)Mat; (void)Off; (void)Curve2; (void)IdentityMat;
    ++dummy; 
    return &dummy;
}
void FreeMatShaper(cmsContext ContextID, void* Data) {
    (void)ContextID; (void)Data;
}
void MatShaperXform(struct _cmstransform_struct *CMMcargo,
                    const void* Input,
                    void* Output,
                    cmsUInt32Number PixelsPerLine,
                    cmsUInt32Number LineCount,
                    const void* Stride) {
    (void)CMMcargo; (void)Input; (void)Output; (void)PixelsPerLine; (void)LineCount; (void)Stride;
}

// The focal method under test (copied in simplified form + adapted to mock environment)
cmsBool OptimizeMatrixShaper15(_cmsTransform2Fn* TransformFn,
                               void** UserData,
                               _cmsFreeUserDataFn* FreeUserData,
                               cmsPipeline** Lut,
                               cmsUInt32Number* InputFormat,
                               cmsUInt32Number* OutputFormat,
                               cmsUInt32Number* dwFlags)
{
{
       cmsStage* Curve1, *Curve2;
       cmsStage* Matrix1, *Matrix2;
       _cmsStageMatrixData* Data1;
       _cmsStageMatrixData* Data2;
       cmsMAT3 res;
       cmsBool IdentityMat = FALSE;
       cmsPipeline* Dest, *Src;
       cmsContext ContextID;
       cmsUInt32Number nChans;
       
       // Only works on RGB to RGB and gray
       if (!(T_CHANNELS(*InputFormat) == 3 && T_CHANNELS(*OutputFormat) == 3)) return FALSE;
       // Only works on 15 bit to 15 bit
       if (T_BYTES(*InputFormat) != 2 || T_BYTES(*OutputFormat) != 2 || 
              T_BIT15(*InputFormat) == 0 || T_BIT15(*OutputFormat) == 0) return FALSE;
       // Seems suitable, proceed
       Src = *Lut;
       // Check for shaper-matrix-matrix-shaper structure, that is what this optimizer stands for
       if (!cmsPipelineCheckAndRetreiveStages(Src, 4,
              cmsSigCurveSetElemType, cmsSigMatrixElemType, cmsSigMatrixElemType, cmsSigCurveSetElemType,
              &Curve1, &Matrix1, &Matrix2, &Curve2)) return FALSE;
       ContextID = cmsGetPipelineContextID(Src);
       nChans = T_CHANNELS(*InputFormat);
       // Get both matrices, which are 3x3
       Data1 = (_cmsStageMatrixData*)cmsStageData(Matrix1);
       Data2 = (_cmsStageMatrixData*)cmsStageData(Matrix2);
       // Input offset should be zero
       if (Data1->Offset != NULL) return FALSE;
       // Multiply both matrices to get the result
       _cmsMAT3per(&res, (cmsMAT3*)Data2->Double, (cmsMAT3*)Data1->Double);
       // Now the result is in res + Data2 -> Offset. Maybe is a plain identity?
       IdentityMat = FALSE;
       if (_cmsMAT3isIdentity(&res) && Data2->Offset == NULL) {
              // We can get rid of full matrix
              IdentityMat = TRUE;
       }

       // Allocate an empty LUT 
       Dest = cmsPipelineAlloc(ContextID, nChans, nChans);
       if (!Dest) return FALSE;
       // Assamble the new LUT
       cmsPipelineInsertStage(Dest, cmsAT_BEGIN, cmsStageDup(Curve1));
       if (!IdentityMat) {
              cmsPipelineInsertStage(Dest, cmsAT_END,
                     cmsStageAllocMatrix(ContextID, 3, 3, (const cmsFloat64Number*)&res, Data2->Offset));
       }
       cmsPipelineInsertStage(Dest, cmsAT_END, cmsStageDup(Curve2));
       
       {
              _cmsStageToneCurvesData* mpeC1 = (_cmsStageToneCurvesData*)cmsStageData(Curve1);
              _cmsStageToneCurvesData* mpeC2 = (_cmsStageToneCurvesData*)cmsStageData(Curve2);
              // In this particular optimization, cache does not help as it takes more time to deal with 
              // the cache than with the pixel handling
              *dwFlags |= cmsFLAGS_NOCACHE;
              // Setup the optimizarion routines
              *UserData = SetMatShaper(ContextID, mpeC1->TheCurves, &res, (cmsVEC3*)Data2->Offset, mpeC2->TheCurves, IdentityMat);
              *FreeUserData = FreeMatShaper;
              *TransformFn = MatShaperXform;
       }
       
       cmsPipelineFree(Src);
       *dwFlags &= ~cmsFLAGS_CAN_CHANGE_FORMATTER;
       *Lut = Dest;
       return TRUE;
}
}

// Test harness

static cmsUInt32Number make_fmt(int channels, int bytes, int bit15) {
    cmsUInt32Number v = 0;
    v |= (cmsUInt32Number)channels;
    v |= ((cmsUInt32Number)bytes) << 8;
    v |= ((cmsUInt32Number)bit15) << 16;
    return v;
}

static cmsStage* createCurveStage() {
    cmsStage* s = cmsStageAllocCurve(g_ContextID);
    return s;
}

static cmsStage* createMatrixStageIdentity() {
    cmsStage* s = new cmsStage;
    s->Type = cmsSigMatrixElemType;
    _cmsStageMatrixData* d = new _cmsStageMatrixData;
    d->Offset = NULL;
    cmsMAT3* m = new cmsMAT3;
    *m = IdentityMatrix();
    d->Double = (void*)m;
    s->Data = (void*)d;
    return s;
}

static cmsStage* createMatrixStageNonIdentity() {
    cmsStage* s = new cmsStage;
    s->Type = cmsSigMatrixElemType;
    _cmsStageMatrixData* d = new _cmsStageMatrixData;
    d->Offset = NULL;
    cmsMAT3* m = new cmsMAT3;
    *m = NonIdentityMatrix();
    d->Double = (void*)m;
    s->Data = (void*)d;
    return s;
}

// Helpers to build a Src pipeline with 4 stages: Curve, Matrix, Matrix, Curve
static cmsPipeline* buildSrcPipeline(int identityPath /*0 or 1*/) {
    // Curve1
    cmsStage* Curve1 = createCurveStage();
    _cmsStageToneCurvesData* c1 = new _cmsStageToneCurvesData;
    c1->TheCurves[0] = new cmsToneCurve{0};
    c1->TheCurves[1] = new cmsToneCurve{0};
    c1->TheCurves[2] = new cmsToneCurve{0};
    Curve1->Data = (void*)c1;

    // Matrix1 (identity)
    cmsStage* Matrix1 = createMatrixStageIdentity();

    // Matrix2
    cmsStage* Matrix2 = identityPath ? createMatrixStageIdentity() : createMatrixStageNonIdentity();

    // Curve2
    cmsStage* Curve2 = createCurveStage();
    _cmsStageToneCurvesData* c2 = new _cmsStageToneCurvesData;
    c2->TheCurves[0] = new cmsToneCurve{0};
    c2->TheCurves[1] = new cmsToneCurve{0};
    c2->TheCurves[2] = new cmsToneCurve{0};
    Curve2->Data = (void*)c2;

    // Src pipeline
    cmsPipeline* Src = cmsPipelineAlloc(g_ContextID, 3, 3);
    Src->Stage.push_back(Curve1);
    Src->Stage.push_back(Matrix1);
    Src->Stage.push_back(Matrix2);
    Src->Stage.push_back(Curve2);
    return Src;
}

// Test 1: Identity path (IdentityMat = TRUE) - Expect 2 curve stages only
static void test_identity_path() {
    // Build input/output formats with 3 channels, 15-bit, 2-byte integers
    cmsUInt32Number InFmt = make_fmt(3, 2, 1);
    cmsUInt32Number OutFmt = make_fmt(3, 2, 1);

    // Build source LUT
    cmsPipeline* Src = buildSrcPipeline(/*identityPath*/ true);

    cmsPipeline* Lut = Src;
    cmsUInt32Number dwFlags = 0;
    cmsBool ok;
    _cmsTransform2Fn tmpFn = nullptr;
    void* UserData = nullptr;
    _cmsFreeUserDataFn FreeFn = nullptr;

    // Input/output type wrappers
    cmsPipeline** pLut = &Lut;

    ok = OptimizeMatrixShaper15(&tmpFn, &UserData, &FreeFn, pLut, &InFmt, &OutFmt, &dwFlags);

    // Expectations
    EXPECT_TRUE(ok, "Identity path should return TRUE");
    EXPECT_TRUE(Lut != nullptr, "LUT should be non-null after transformation");
    // Dest should have 2 curve stages (BEGIN Curve1 and Curve2)
    if (Lut && Lut->Stage.size() == 2) {
        if (Lut->Stage[0]) EXPECT_TRUE(Lut->Stage[0]->Type == cmsSigCurveSetElemType, "First stage should be CurveSetElemType");
        if (Lut->Stage[1]) EXPECT_TRUE(Lut->Stage[1]->Type == cmsSigCurveSetElemType, "Second stage should be CurveSetElemType");
    } else {
        EXPECT_TRUE(false, "LUT stage count should be 2 for identity path");
    }
    // Flags
    EXPECT_TRUE((dwFlags & cmsFLAGS_NOCACHE) != 0, "NOCACHE flag should be set");
    EXPECT_TRUE((dwFlags & cmsFLAGS_CAN_CHANGE_FORMATTER) == 0, "CAN_CHANGE_FORMATTER should be cleared");

    // Cleanup
    cmsPipelineFree(Src);
}

// Test 2: Non-Identity path (IdentityMat = FALSE) - Expect 3 stages: Curve, Matrix, Curve
static void test_non_identity_path() {
    // Build input/output formats with 3 channels, 15-bit, 2-byte integers
    cmsUInt32Number InFmt = make_fmt(3, 2, 1);
    cmsUInt32Number OutFmt = make_fmt(3, 2, 1);

    // Build source LUT with non-identity second matrix
    // Curve1
    cmsStage* Curve1 = createCurveStage();
    _cmsStageToneCurvesData* c1 = new _cmsStageToneCurvesData;
    c1->TheCurves[0] = new cmsToneCurve{0};
    c1->TheCurves[1] = new cmsToneCurve{0};
    c1->TheCurves[2] = new cmsToneCurve{0};
    Curve1->Data = (void*)c1;

    // Matrix1 (Identity)
    cmsStage* Matrix1 = createMatrixStageIdentity();

    // Matrix2 (Non-Identity)
    cmsStage* Matrix2 = createMatrixStageNonIdentity();

    // Curve2
    cmsStage* Curve2 = createCurveStage();
    _cmsStageToneCurvesData* c2 = new _cmsStageToneCurvesData;
    c2->TheCurves[0] = new cmsToneCurve{0};
    c2->TheCurves[1] = new cmsToneCurve{0};
    c2->TheCurves[2] = new cmsToneCurve{0};
    Curve2->Data = (void*)c2;

    cmsPipeline* Src = cmsPipelineAlloc(g_ContextID, 3, 3);
    Src->Stage.push_back(Curve1);
    Src->Stage.push_back(Matrix1);
    Src->Stage.push_back(Matrix2);
    Src->Stage.push_back(Curve2);

    cmsPipeline* Lut = Src;
    cmsUInt32Number dwFlags = 0;
    cmsTransform2Fn tmpFn = nullptr;
    void* UserData = nullptr;
    _cmsFreeUserDataFn FreeFn = nullptr;

    cmsPipeline** pLut = &Lut;

    // Call focal method
    cmsBool ok = OptimizeMatrixShaper15(((_cmsTransform2Fn*)(&tmpFn)),
                                        &UserData, &FreeFn, pLut, &InFmt, &OutFmt, &dwFlags);

    // Expectations
    EXPECT_TRUE(ok, "Non-identity path should return TRUE");
    EXPECT_TRUE(Lut != nullptr, "LUT should be non-null after transformation");
    // Dest should have 3 stages
    if (Lut && Lut->Stage.size() == 3) {
        EXPECT_TRUE(Lut->Stage[0]->Type == cmsSigCurveSetElemType, "First stage should be CurveSetElemType");
        EXPECT_TRUE(Lut->Stage[1]->Type == cmsSigMatrixElemType, "Second stage should be MatrixElemType");
        EXPECT_TRUE(Lut->Stage[2]->Type == cmsSigCurveSetElemType, "Third stage should be CurveSetElemType");
    } else {
        EXPECT_TRUE(false, "LUT stage count should be 3 for non-identity path");
    }

    // Cleanup
    cmsPipelineFree(Src);
}

// Test 3: Wrong input channels -> should fail
static void test_wrong_input_channels() {
    // 2 channels input/output should fail
    cmsUInt32Number InFmt = make_fmt(2, 2, 1);
    cmsUInt32Number OutFmt = make_fmt(3, 2, 1);

    cmsPipeline* Src = nullptr; // Not needed for this test
    cmsPipeline* Lut = Src;
    cmsUInt32Number dwFlags = 0;
    cmsBool ok;
    _cmsTransform2Fn tmpFn = nullptr;
    void* UserData = nullptr;
    _cmsFreeUserDataFn FreeFn = nullptr;

    cmsPipeline** pLut = &Lut;

    ok = OptimizeMatrixShaper15(&tmpFn, &UserData, &FreeFn, pLut, &InFmt, &OutFmt, &dwFlags);

    EXPECT_FALSE(ok, "Wrong input channel count should return FALSE");
    EXPECT_TRUE(Lut == nullptr, "LUT should remain null on failure");
}

// Test 4: Wrong bit15 flags -> should fail
static void test_wrong_bit15_flags() {
    // 3 channels but bit15 not set for input or output
    cmsUInt32Number InFmt = make_fmt(3, 2, 0); // bit15 0
    cmsUInt32Number OutFmt = make_fmt(3, 2, 0);

    cmsPipeline* Src = nullptr;
    cmsPipeline* Lut = Src;
    cmsUInt32Number dwFlags = 0;
    cmsBool ok;
    _cmsTransform2Fn tmpFn = nullptr;
    void* UserData = nullptr;
    _cmsFreeUserDataFn FreeFn = nullptr;

    cmsPipeline** pLut = &Lut;

    ok = OptimizeMatrixShaper15(&tmpFn, &UserData, &FreeFn, pLut, &InFmt, &OutFmt, &dwFlags);

    EXPECT_FALSE(ok, "Wrong 15-bit flag should return FALSE");
}

// Test 5: Data1.Offset non-NULL triggers early failure
static void test_offset_non_null_failure() {
    // Build input/output formats with 3 channels, 15-bit
    cmsUInt32Number InFmt = make_fmt(3, 2, 1);
    cmsUInt32Number OutFmt = make_fmt(3, 2, 1);

    // Build Src with a non-null Data1->Offset
    cmsStage* Curve1 = createCurveStage();
    _cmsStageToneCurvesData* c1 = new _cmsStageToneCurvesData;
    c1->TheCurves[0] = new cmsToneCurve{0};
    c1->TheCurves[1] = new cmsToneCurve{0};
    c1->TheCurves[2] = new cmsToneCurve{0};
    Curve1->Data = (void*)c1;

    cmsStage* Matrix1 = new cmsStage;
    Matrix1->Type = cmsSigMatrixElemType;
    _cmsStageMatrixData* d1 = new _cmsStageMatrixData;
    d1->Offset = (void*)0xDEADBEEF; // non-null
    cmsMAT3* m = new cmsMAT3;
    *m = IdentityMatrix();
    d1->Double = (void*)m;
    Matrix1->Data = (void*)d1;

    cmsStage* Matrix2 = new cmsStage;
    Matrix2->Type = cmsSigMatrixElemType;
    _cmsStageMatrixData* d2 = new _cmsStageMatrixData;
    d2->Offset = NULL;
    cmsMAT3* m2 = new cmsMAT3;
    *m2 = IdentityMatrix();
    d2->Double = (void*)m2;
    Matrix2->Data = (void*)d2;

    cmsStage* Curve2 = createCurveStage();
    _cmsStageToneCurvesData* c2 = new _cmsStageToneCurvesData;
    c2->TheCurves[0] = new cmsToneCurve{0};
    c2->TheCurves[1] = new cmsToneCurve{0};
    c2->TheCurves[2] = new cmsToneCurve{0};
    Curve2->Data = (void*)c2;

    cmsPipeline* Src = cmsPipelineAlloc(g_ContextID, 3, 3);
    Src->Stage.push_back(Curve1);
    Src->Stage.push_back(Matrix1);
    Src->Stage.push_back(Matrix2);
    Src->Stage.push_back(Curve2);

    cmsPipeline* Lut = Src;
    cmsUInt32Number dwFlags = 0;
    cmsTransform2Fn tmpFn = nullptr;
    void* UserData = nullptr;
    _cmsFreeUserDataFn FreeFn = nullptr;

    cmsPipeline** pLut = &Lut;

    cmsBool ok = OptimizeMatrixShaper15(((_cmsTransform2Fn*)(&tmpFn)),
                                         &UserData, &FreeFn, pLut, &InFmt, &OutFmt, &dwFlags);

    EXPECT_FALSE(ok, "Data1.Offset non-null should fail early");
    cmsPipelineFree(Src);
}

// Main test runner
int main() {
    printf("Running OptimizeMatrixShaper15 unit tests (mocked environment)\n");

    test_identity_path();
    test_non_identity_path();
    test_wrong_input_channels();
    test_wrong_bit15_flags();
    test_offset_non_null_failure();

    if (g_TestFailures == 0) {
        printf("All tests passed.\n");
        return 0;
    } else {
        printf("Tests completed with %d failure(s).\n", g_TestFailures);
        return 1;
    }
}