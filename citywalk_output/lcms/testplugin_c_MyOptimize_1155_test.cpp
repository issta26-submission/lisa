/* File: testcms2.h */
#ifndef TESTCMS2_H
#define TESTCMS2_H

#ifdef __cplusplus
extern "C" {
#endif

// Basic type aliases (enough for the focal test)
typedef unsigned int cmsUInt32Number;
typedef unsigned short cmsUInt16Number;
typedef int cmsBool;

// Forward declarations for the types used by MyOptimize
typedef struct cmsStage cmsStage;
typedef struct cmsPipeline cmsPipeline;

// Public constants/macros used by the focal method
#define TRUE 1
#define FALSE 0
#define cmsSigCurveSetElemType 0x0001      // Arbitrary non-zero code for a curve-set stage
#define cmsFLAGS_NOCACHE 0x0001

// Prototype: accessors used by MyOptimize
extern cmsStage* cmsPipelineGetPtrToFirstStage(const cmsPipeline* Lut);
extern cmsStage* cmsStageNext(const cmsStage* stage);
extern int cmsStageType(const cmsStage* stage);
extern void* cmsStageData(const cmsStage* stage);

// The focal function under test (extern C linkage to allow linking with C)
extern cmsBool MyOptimize(cmsPipeline** Lut,
                          cmsUInt32Number  Intent,
                          cmsUInt32Number* InputFormat,
                          cmsUInt32Number* OutputFormat,
                          cmsUInt32Number* dwFlags);

// Internal helpers that the focal code calls
extern void _cmsPipelineSetOptimizationParameters(cmsPipeline* Lut,
                                                void(*Evaluate)(const cmsUInt16Number In[],
                                                                cmsUInt16Number Out[],
                                                                const void* Data),
                                                void* a,
                                                void* b,
                                                void* c);
extern void FastEvaluateCurves(const cmsUInt16Number In[],
                               cmsUInt16Number Out[],
                               const void* Data);

// Gamma estimation helper (used to mimic the real library signature)
extern double cmsEstimateGamma(const void* curve, double param);

#ifdef __cplusplus
}
#endif

#endif // TESTCMS2_H



/* File: mock_cms.cpp */
#include <iostream>
#include <testcms2.h>
#include <iomanip>
#include <cstdlib>
#include <stdlib.h>


extern "C" {

// Simple tone-curve structure used by the test to control gamma
typedef struct {
    double gammaValue;
} _cmsToneCurve;

// Stage data containing an array of tone-curve pointers
typedef struct {
    int nCurves;
    _cmsToneCurve* TheCurves[1]; // We only use TheCurves[0] in tests
} _cmsStageToneCurvesData;

// Simple stage structure consistent with the header (used by tests)
typedef struct cmsStage {
    int Type;
    void* Data;
    struct cmsStage* Next;
} cmsStage;

// Pipeline container with a pointer to the first stage
typedef struct {
    cmsStage* First;
} cmsPipeline;

// Public constants mimic (provided by actual library)
int cmsSigCurveSetElemType = cmsSigCurveSetElemType; // alias (keep as same value)

// Global flag to verify that optimization parameters were set
int g_OptimizationParamsSet = 0;

// Accessors used by MyOptimize
cmsStage* cmsPipelineGetPtrToFirstStage(const cmsPipeline* Lut) {
    return (LUT_NONNULL(Lut)) ? Lut->First : NULL;
}
cmsStage* cmsStageNext(const cmsStage* stage) {
    return (stage) ? stage->Next : NULL;
}
int cmsStageType(const cmsStage* stage) {
    return (stage) ? stage->Type : 0;
}
void* cmsStageData(const cmsStage* stage) {
    return (stage) ? stage->Data : NULL;
}

// Helper macro to guard NULL (simple)
#define LUT_NONNULL(x) ((x) != NULL)

// Mock implementation to capture calls and provide deterministic behavior
void _cmsPipelineSetOptimizationParameters(cmsPipeline* Lut,
                                          void(*Evaluate)(const cmsUInt16Number In[],
                                                          cmsUInt16Number Out[],
                                                          const void* Data),
                                          void* a,
                                          void* b,
                                          void* c) {
    // Indicate that the pipeline optimization parameters were set (used by tests)
    (void)Lut; (void)Evaluate; (void)a; (void)b; (void)c;
    g_OptimizationParamsSet = 1;
}
void FastEvaluateCurves(const cmsUInt16Number In[],
                        cmsUInt16Number Out[],
                        const void* Data) {
    (void)In; (void)Out; (void*)Data;
    // No-op for tests
}
double cmsEstimateGamma(const void* curve, double param) {
    (void)param;
    const _cmsToneCurve* c = (const _cmsToneCurve*)curve;
    return (c != NULL) ? c->gammaValue : 0.0;
}

} // extern "C"



/* File: test_main.cpp */
extern "C" {
}

// Very small test harness with non-terminating assertions
static int g_tests_run = 0;
static int g_tests_failed = 0;
#define TEST_PASSED() do { std::cout << "  PASS\n"; } while(0)
#define TEST_FAILED(msg) do { std::cerr << "  FAIL: " << msg << "\n"; ++g_tests_failed; } while(0)
#define CHECK(cond, msg) do { ++g_tests_run; if (cond) { TEST_PASSED(); } else { TEST_FAILED(msg); } } while(0)

// Helpers to build a minimal pipeline with one curve stage
static cmsStage* createCurveStage(double gammaValue) {
    // Allocate stage
    cmsStage* stage = (cmsStage*)std::malloc(sizeof(cmsStage));
    stage->Type = cmsSigCurveSetElemType;
    // Build stage data
    _cmsStageToneCurvesData* data = (_cmsStageToneCurvesData*)std::malloc(sizeof(_cmsStageToneCurvesData));
    data->nCurves = 1;
    data->TheCurves[0] = (_cmsToneCurve*)std::malloc(sizeof(_cmsToneCurve));
    data->TheCurves[0]->gammaValue = gammaValue;
    stage->Data = data;
    stage->Next = NULL;
    return stage;
}

static void freeCurveStage(cmsStage* stage) {
    if (!stage) return;
    _cmsStageToneCurvesData* data = (_cmsStageToneCurvesData*)stage->Data;
    if (data) {
        if (data->TheCurves[0]) {
            std::free(data->TheCurves[0]);
        }
        std::free(data);
    }
    std::free(stage);
}

// Test 1: True path - all stages are CurveSetElemType, exactly one curve, gamma <= 1.0
static void test_true_path_all_curve_identity() {
    std::cout << "Test 1: True path with a valid single curve stage... ";
    // Build pipeline with a single valid curve stage
    cmsStage* stage = createCurveStage(0.5); // gamma <= 1.0
    cmsPipeline* pipe = (cmsPipeline*)std::malloc(sizeof(cmsPipeline));
    pipe->First = stage;

    cmsPipeline** Lut = &pipe;
    cmsUInt32Number inputFmt = 0;
    cmsUInt32Number outputFmt = 0;
    cmsUInt32Number dwFlags = 0;

    int result = MyOptimize(Lut, 0, &inputFmt, &outputFmt, &dwFlags);

    bool ok = (result != 0) && ((dwFlags & cmsFLAGS_NOCACHE) != 0) && (g_OptimizationParamsSet == 1);
    if (ok) {
        std::cout << "PASS\n";
    } else {
        std::cerr << "FAIL (expected TRUE with NOCACHE set and optimization called)\n";
        ++g_tests_failed;
    }

    // Cleanup
    freeCurveStage(stage);
    std::free(pipe);
    g_OptimizationParamsSet = 0;
}

// Test 2: False path - first stage type is not CurveSetElemType
static void test_false_first_stage_not_curve() {
    std::cout << "Test 2: False when first stage is not CurveSetElemType... ";
    // Build non-curve stage
    cmsStage* stage = (cmsStage*)std::malloc(sizeof(cmsStage));
    stage->Type = 0xDEAD; // not CurveSetElemType
    stage->Data = NULL;
    stage->Next = NULL;

    cmsPipeline* pipe = (cmsPipeline*)std::malloc(sizeof(cmsPipeline));
    pipe->First = stage;

    cmsPipeline** Lut = &pipe;
    cmsUInt32Number inputFmt = 0;
    cmsUInt32Number outputFmt = 0;
    cmsUInt32Number dwFlags = 0;

    int result = MyOptimize(Lut, 0, &inputFmt, &outputFmt, &dwFlags);

    bool ok = (result == FALSE);
    if (ok) {
        std::cout << "PASS\n";
    } else {
        std::cerr << "FAIL (expected FALSE)\n";
        ++g_tests_failed;
    }

    // Cleanup
    std::free(stage);
    std::free(pipe);
    g_OptimizationParamsSet = 0;
}

// Test 3: False path - nCurves != 1
static void test_false_ncurves_not_one() {
    std::cout << "Test 3: False when nCurves != 1... ";
    cmsStage* stage = createCurveStage(0.5);
    // Overwrite to simulate nCurves != 1
    _cmsStageToneCurvesData* data = (_cmsStageToneCurvesData*)stage->Data;
    data->nCurves = 2; // not equal to 1

    cmsPipeline* pipe = (cmsPipeline*)std::malloc(sizeof(cmsPipeline));
    pipe->First = stage;

    cmsPipeline** Lut = &pipe;
    cmsUInt32Number inputFmt = 0;
    cmsUInt32Number outputFmt = 0;
    cmsUInt32Number dwFlags = 0;

    int result = MyOptimize(Lut, 0, &inputFmt, &outputFmt, &dwFlags);

    bool ok = (result == FALSE);
    if (ok) {
        std::cout << "PASS\n";
    } else {
        std::cerr << "FAIL (expected FALSE)\n";
        ++g_tests_failed;
    }

    // Cleanup
    freeCurveStage(stage);
    std::free(pipe);
    g_OptimizationParamsSet = 0;
}

// Test 4: False path - gamma > 1.0
static void test_false_gamma_too_high() {
    std::cout << "Test 4: False when gamma > 1.0... ";
    cmsStage* stage = createCurveStage(1.1); // gamma greater than 1.0
    cmsPipeline* pipe = (cmsPipeline*)std::malloc(sizeof(cmsPipeline));
    pipe->First = stage;

    cmsPipeline** Lut = &pipe;
    cmsUInt32Number inputFmt = 0;
    cmsUInt32Number outputFmt = 0;
    cmsUInt32Number dwFlags = 0;

    int result = MyOptimize(Lut, 0, &inputFmt, &outputFmt, &dwFlags);

    bool ok = (result == FALSE);
    if (ok) {
        std::cout << "PASS\n";
    } else {
        std::cerr << "FAIL (expected FALSE)\n";
        ++g_tests_failed;
    }

    // Cleanup
    freeCurveStage(stage);
    std::free(pipe);
    g_OptimizationParamsSet = 0;
}

int main() {
    std::cout << "Starting unit tests for MyOptimize (focal method) using mock CMS environment.\n";

    // Reset global state
    g_tests_run = 0;
    g_tests_failed = 0;
    g_OptimizationParamsSet = 0;

    test_true_path_all_curve_identity();
    test_false_first_stage_not_curve();
    test_false_ncurves_not_one();
    test_false_gamma_too_high();

    std::cout << "Tests completed: " << g_tests_run << " run, " 
              << g_tests_failed << " failed.\n";

    return (g_tests_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}