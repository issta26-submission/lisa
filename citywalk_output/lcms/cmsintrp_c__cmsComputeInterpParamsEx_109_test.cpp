// Lightweight C++11 unit test suite for a focused reproduction of cmsintrp.c's
// _cmsComputeInterpParamsEx function (Step 2: test generation based on focal method).
// This test suite is self-contained: it redefines the minimal dependencies and
// provides controlled hooks to simulate success/failure paths without GTest.
// It validates key branches: input channel limits, allocation failure, successful
// parameter computation, and interpolation routine registration failure.

#include <cstring>
#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <cstdarg>
#include <cstdlib>
#include <lcms2_internal.h>


// Domain-specific simplified types and constants (reproduced for testing)
typedef unsigned int cmsUInt32Number;
typedef int cmsContext;
typedef bool cmsBool;

#define CMS_MAX_INPUT_DIMENSIONS 4
#ifndef MAX_INPUT_DIMENSIONS
#define MAX_INPUT_DIMENSIONS CMS_MAX_INPUT_DIMENSIONS
#endif

// Forward declaration of the focal struct (simplified version)
struct cmsInterpParams;

// Global test harness data to observe errors and control allocation behavior
static bool g_failMalloc = false;
static std::vector<std::string> g_errorLog;

// Function pointer mechanism to mock _cmsSetInterpolationRoutine behavior
typedef cmsBool (*SetInterpRoutineFunc)(cmsContext, cmsInterpParams*);
static SetInterpRoutineFunc g_SetInterpRoutinePtr = nullptr;

// Default mock behavior: always succeed
static cmsBool default_SetInterpRoutine(cmsContext, cmsInterpParams* /*p*/) {
    return true;
}
static cmsBool failSetInterpRoutine(cmsContext, cmsInterpParams* /*p*/) {
    return false;
}

// Minimal memory helpers to mimic library utilities
static void* _cmsMallocZero(cmsContext /*ContextID*/, size_t size) {
    if (g_failMalloc) return NULL;
    return calloc(1, size);
}
static void _cmsFree(cmsContext /*ContextID*/, void* p) {
    free(p);
}
static void cmsSignalError(cmsContext /*ContextID*/, int /*errorCode*/,
                           const char* fmt, ...) {
    // Simple variadic logger to string form
    char buffer[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    g_errorLog.push_back(std::string(buffer));
}

// Minimal implementation of _cmsSetInterpolationRoutine to be overridable
static cmsBool _cmsSetInterpolationRoutine(cmsContext ContextID, cmsInterpParams* p) {
    (void)ContextID; (void)p;
    if (g_SetInterpRoutinePtr) {
        return g_SetInterpRoutinePtr(ContextID, p);
    }
    // If not set, default to success
    return default_SetInterpRoutine(ContextID, p);
}

// The focal data structure (simplified)
struct cmsInterpParams {
    cmsUInt32Number dwFlags;
    cmsUInt32Number nInputs;
    cmsUInt32Number nOutputs;
    const void* Table;
    cmsContext ContextID;
    cmsUInt32Number nSamples[CMS_MAX_INPUT_DIMENSIONS];
    cmsUInt32Number Domain[CMS_MAX_INPUT_DIMENSIONS];
    cmsUInt32Number opta[CMS_MAX_INPUT_DIMENSIONS];
};

// The focal function under test, reproduced in this test file
cmsInterpParams* _cmsComputeInterpParamsEx(
    cmsContext ContextID,
    const cmsUInt32Number nSamples[],
    cmsUInt32Number InputChan, cmsUInt32Number OutputChan,
    const void *Table,
    cmsUInt32Number dwFlags)
{
    {
        cmsInterpParams* p;
        cmsUInt32Number i;
        // Check for maximum inputs
        if (InputChan > MAX_INPUT_DIMENSIONS) {
            cmsSignalError(ContextID, /*cmsERROR_RANGE*/ 1,
                           "Too many input channels (%d channels, max=%d)",
                           InputChan, MAX_INPUT_DIMENSIONS);
            return NULL;
        }
        // Creates an empty object
        p = (cmsInterpParams*) _cmsMallocZero(ContextID, sizeof(cmsInterpParams));
        if (p == NULL) return NULL;
        // Keep original parameters
        p -> dwFlags  = dwFlags;
        p -> nInputs  = InputChan;
        p -> nOutputs = OutputChan;
        p ->Table     = Table;
        p ->ContextID  = ContextID;
        // Fill samples per input direction and domain (which is number of nodes minus one)
        for (i=0; i < InputChan; i++) {
            p -> nSamples[i] = nSamples[i];
            p -> Domain[i]   = nSamples[i] - 1;
        }
        // Compute factors to apply to each component to index the grid array
        p -> opta[0] = p -> nOutputs;
        for (i=1; i < InputChan; i++)
            p ->opta[i] = p ->opta[i-1] * nSamples[InputChan-i];

        if (!_cmsSetInterpolationRoutine(ContextID, p)) {
            cmsSignalError(ContextID, /*cmsERROR_UNKNOWN_EXTENSION*/ 2,
                           "Unsupported interpolation (%d->%d channels)",
                           InputChan, OutputChan);
            _cmsFree(ContextID, p);
            return NULL;
        }
        // All seems ok
        return p;
    }
}

// Helper: reset global test state
static void resetGlobals() {
    g_errorLog.clear();
    g_failMalloc = false;
    g_SetInterpRoutinePtr = nullptr; // leave as null to use default
}

// Simple test harness: run a test and print PASS/FAIL
static bool runTest(const std::string& name, const std::function<bool()> &testFn) {
    bool ok = testFn();
    std::cout << (ok ? "[PASS] " : "[FAIL] ") << name << std::endl;
    return ok;
}

/* Test 1: Too many input channels should return NULL and log an error.
   This covers the maximum-input-dimensions boundary condition. */
static bool testTooManyInputChannels() {
    resetGlobals();
    // Prepare inputs exceeding MAX_INPUT_DIMENSIONS
    cmsContext ContextID = 1;
    cmsUInt32Number nSamples[CMS_MAX_INPUT_DIMENSIONS] = {2, 2, 2, 2};
    cmsUInt32Number InputChan = MAX_INPUT_DIMENSIONS + 1; // trigger boundary
    cmsUInt32Number OutputChan = 1;
    const void* Table = NULL;
    cmsUInt32Number dwFlags = 0;

    cmsInterpParams* p = _cmsComputeInterpParamsEx(ContextID, nSamples, InputChan, OutputChan, Table, dwFlags);

    // Expect NULL due to too many input channels
    if (p != NULL) return false;

    // Expect an error message mentioning "Too many input channels"
    bool found = false;
    for (const auto& s : g_errorLog) {
        if (s.find("Too many input channels") != std::string::npos) {
            found = true;
            break;
        }
    }
    return found;
}

/* Test 2: Allocation failure should yield NULL and no crash. */
static bool testAllocationFailure() {
    resetGlobals();
    g_failMalloc = true; // force allocation to fail

    cmsContext ContextID = 1;
    cmsUInt32Number nSamples[CMS_MAX_INPUT_DIMENSIONS] = {2, 1, 1, 1};
    cmsUInt32Number InputChan = 1;
    cmsUInt32Number OutputChan = 1;
    const void* Table = NULL;
    cmsUInt32Number dwFlags = 0;

    cmsInterpParams* p = _cmsComputeInterpParamsEx(ContextID, nSamples, InputChan, OutputChan, Table, dwFlags);

    // Expect NULL due to allocation failure
    if (p != NULL) return false;
    return true;
}

/* Test 3: Normal path where _cmsSetInterpolationRoutine succeeds.
   Verifies all fields are correctly populated including computed Domain and opta. */
static bool testNormalPath() {
    resetGlobals();
    g_failMalloc = false;
    // Ensure default success behavior
    g_SetInterpRoutinePtr = default_SetInterpRoutine;

    cmsContext ContextID = 3;
    cmsUInt32Number nSamples[CMS_MAX_INPUT_DIMENSIONS] = {4, 3, 1, 1};
    cmsUInt32Number InputChan = 2;
    cmsUInt32Number OutputChan = 2;
    const void* Table = (void*)0x1234;
    cmsUInt32Number dwFlags = 0xAA;

    cmsInterpParams* p = _cmsComputeInterpParamsEx(ContextID, nSamples, InputChan, OutputChan, Table, dwFlags);
    if (p == NULL) return false;

    bool ok = true;
    // Verify basic fields
    ok &= (p->dwFlags == dwFlags);
    ok &= (p->nInputs == InputChan);
    ok &= (p->nOutputs == OutputChan);
    ok &= (p->Table == Table);
    ok &= (p->ContextID == ContextID);

    // Verify per-input sample and domain
    ok &= (p->nSamples[0] == nSamples[0]);
    ok &= (p->nSamples[1] == nSamples[1]);
    ok &= (p->Domain[0] == (nSamples[0] - 1));
    ok &= (p->Domain[1] == (nSamples[1] - 1));

    // Verify opta computation: opta[0] = nOutputs; opta[i] = opta[i-1] * nSamples[InputChan-i]
    ok &= (p->opta[0] == OutputChan);
    int expectedOpta1 = p->opta[0] * nSamples[1]; // for i=1, InputChan=2, use nSamples[InputChan-1] == nSamples[1]
    ok &= (p->opta[1] == expectedOpta1);

    _cmsFree(ContextID, p);
    return ok;
}

/* Test 4: _cmsSetInterpolationRoutine reports unsupported interpolation; expect NULL. */
static cmsBool failSetInterpRuleFunc(cmsContext, cmsInterpParams*);

static bool testInterpolationRoutineFailure() {
    resetGlobals();
    g_failMalloc = false;
    g_SetInterpRoutinePtr = failSetInterpRuleFunc;

    cmsContext ContextID = 1;
    cmsUInt32Number nSamples[CMS_MAX_INPUT_DIMENSIONS] = {2, 2, 1, 1};
    cmsUInt32Number InputChan = 2;
    cmsUInt32Number OutputChan = 2;
    const void* Table = NULL;
    cmsUInt32Number dwFlags = 0;

    cmsInterpParams* p = _cmsComputeInterpParamsEx(ContextID, nSamples, InputChan, OutputChan, Table, dwFlags);

    // Expect NULL due to unsupported interpolation
    bool result = (p == NULL);

    // Restore default behavior for subsequent tests
    g_SetInterpRoutinePtr = nullptr;

    // Also ensure an error message was logged
    bool logged = false;
    for (const auto& s : g_errorLog) {
        if (s.find("Unsupported interpolation") != std::string::npos) {
            logged = true;
            break;
        }
    }
    return result && logged;
}

// Helper function used to simulate failure of SetInterpolationRoutine
static cmsBool failSetInterpRuleFunc(cmsContext /*ContextID*/, cmsInterpParams* /*p*/) {
    return false;
}

/* Test 5: Zero input channels should still succeed when SetInterpolationRoutine succeeds. */
static bool testZeroInputChannels() {
    resetGlobals();
    g_failMalloc = false;
    g_SetInterpRoutinePtr = default_SetInterpRoutine;

    cmsContext ContextID = 7;
    cmsUInt32Number nSamples[CMS_MAX_INPUT_DIMENSIONS] = {1, 1, 1, 1};
    cmsUInt32Number InputChan = 0; // boundary case
    cmsUInt32Number OutputChan = 1;
    const void* Table = NULL;
    cmsUInt32Number dwFlags = 0;

    cmsInterpParams* p = _cmsComputeInterpParamsEx(ContextID, nSamples, InputChan, OutputChan, Table, dwFlags);
    if (p == NULL) return false;

    bool ok = (p->nInputs == 0);
    _cmsFree(ContextID, p);
    return ok;
}

// Main: run all tests
int main() {
    // Ensure deterministic baseline
    resetGlobals();

    bool overall = true;
    overall &= runTest("Too many input channels", testTooManyInputChannels);
    overall &= runTest("Allocation failure", testAllocationFailure);
    overall &= runTest("Normal path", testNormalPath);
    overall &= runTest("Interpolation routine failure", testInterpolationRoutineFailure);
    overall &= runTest("Zero input channels", testZeroInputChannels);

    std::cout << "Test suite " << (overall ? "PASSED" : "FAILED") << std::endl;
    return overall ? 0 : 1;
}