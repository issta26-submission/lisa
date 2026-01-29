// Unit tests for the focal method: cmsBool _cmsRegisterParametricCurvesPlugin
// This test suite is implemented in C++11, without GoogleTest, using a small custom test harness.
// The tests provide minimal mock implementations of the necessary LittleCMS-like internals
// to exercise the focal function in isolation.

#include <cstring>
#include <vector>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <lcms2_internal.h>


// Domain knowledge notes followed in tests:
// - Use only standard library facilities
// - Cover true/false branches for key predicates
// - Access static-like helpers via function scope (we simulate with globals here)
// - Do not rely on private/internal implementation beyond the provided focal function's scope

// ----------------------
// Minimal type/macro definitions to compile the focal function in isolation
// ----------------------

// Basic boolean type and constants
typedef int cmsBool;
#define TRUE 1
#define FALSE 0

// Public type aliases used by the focal function
typedef void* cmsContext;
typedef void* cmsPluginBase;

// Maximum function types supported by the plugin (chosen to be safe for tests)
#define MAX_TYPES_IN_LCMS_PLUGIN 64

// The parametric curves collection node (as used by the focal function)
typedef struct _cmsParametricCurvesCollection {
    cmsBool Evaluator;
    cmsUInt32Number nFunctions;
    unsigned int FunctionTypes[MAX_TYPES_IN_LCMS_PLUGIN];
    unsigned int ParameterCount[MAX_TYPES_IN_LCMS_PLUGIN];
    struct _cmsParametricCurvesCollection* Next;
} _cmsParametricCurvesCollection;

// The curves plugin chunk in the context
typedef struct _cmsCurvesPluginChunkType {
    _cmsParametricCurvesCollection* ParametricCurves;
} _cmsCurvesPluginChunkType;

// Plugin base and parametric curves plugin structure
typedef struct cmsPluginBase {
    int _dummy; // placeholder to emulate real CMS base
} cmsPluginBase;

typedef struct cmsPluginParametricCurves {
    cmsBool Evaluator;
    unsigned int nFunctions;
    const unsigned int* FunctionTypes;
    const unsigned int* ParameterCount;
} cmsPluginParametricCurves;

// Typedefs for the function under test
typedef unsigned int cmsUInt32Number;


// ----------------------
// Test-specific global state and helpers
// ----------------------

// Global simulated context chunk for CurvesPlugin
static _cmsCurvesPluginChunkType g_ctx;

// Simulated malloc failure switch for testing edge case
static int g_failMalloc = 0;

// Forward declaration of the function under test
extern "C" cmsBool _cmsRegisterParametricCurvesPlugin(cmsContext ContextID, cmsPluginBase* Data);

// Simulated internal helpers (stubs) required by the focal function

// Simulate _cmsContextGetClientChunk: always return the address of our global ctx chunk
extern "C" void* _cmsContextGetClientChunk(cmsContext ContextID, int /*ChunkType*/) {
    (void)ContextID; // unused in test stub
    return &g_ctx;
}

// Simulate _cmsPluginMalloc: optionally fail, otherwise use standard malloc
extern "C" void* _cmsPluginMalloc(cmsContext ContextID, size_t size) {
    (void)ContextID;
    if (g_failMalloc) return NULL;
    return std::malloc(size);
}

// A simple helper to reset context between tests
static void reset_ctx() {
    g_ctx.ParametricCurves = NULL;
}

// ----------------------
// Focal function under test (reproduced here for isolation in the test harness)
// ----------------------

extern "C" cmsBool _cmsRegisterParametricCurvesPlugin(cmsContext ContextID, cmsPluginBase* Data)
{
{
    _cmsCurvesPluginChunkType* ctx = ( _cmsCurvesPluginChunkType*) _cmsContextGetClientChunk(ContextID, 0 /*CurvesPlugin*/);
    cmsPluginParametricCurves* Plugin = (cmsPluginParametricCurves*) Data;
    _cmsParametricCurvesCollection* fl;
    if (Data == NULL) {
          ctx -> ParametricCurves =  NULL;
          return TRUE;
    }
    fl = (_cmsParametricCurvesCollection*) _cmsPluginMalloc(ContextID, sizeof(_cmsParametricCurvesCollection));
    if (fl == NULL) return FALSE;
    // Copy the parameters
    fl ->Evaluator  = Plugin ->Evaluator;
    fl ->nFunctions = Plugin ->nFunctions;
    // Make sure no mem overwrites
    if (fl ->nFunctions > MAX_TYPES_IN_LCMS_PLUGIN)
        fl ->nFunctions = MAX_TYPES_IN_LCMS_PLUGIN;
    // Copy the data
    memmove(fl->FunctionTypes,  Plugin ->FunctionTypes,   fl->nFunctions * sizeof(cmsUInt32Number));
    memmove(fl->ParameterCount, Plugin ->ParameterCount,  fl->nFunctions * sizeof(cmsUInt32Number));
    // Keep linked list
    fl ->Next = ctx->ParametricCurves;
    ctx->ParametricCurves = fl;
    // All is ok
    return TRUE;
}
}

// ----------------------
// Lightweight test harness
// ----------------------

// Simple test result collector
static std::vector<std::string> g_failures;

#define CHECK(cond, msg) do { if(!(cond)) { g_failures.push_back(std::string("FAIL: ") + (msg)); } } while(0)

// Run individual tests and report results
static void test_null_data_branch() {
    reset_ctx();
    g_failures.clear();

    cmsContext ctx = (cmsContext)0x1234; // arbitrary non-null context

    // Test the true-branch: Data == NULL should not crash and should set ParametricCurves to NULL
    cmsBool ret = _cmsRegisterParametricCurvesPlugin(ctx, NULL);
    CHECK(ret == TRUE, "Expected TRUE return for NULL Data");
    CHECK(g_ctx.ParametricCurves == NULL, "ParametricCurves should be NULL after NULL Data call");

    if (g_failures.empty()) {
        std::cout << "[PASS] test_null_data_branch\n";
    } else {
        for (auto &s : g_failures) std::cerr << s << "\n";
        std::cout << "[FAIL] test_null_data_branch\n";
    }
}

static void test_normal_input() {
    reset_ctx();
    g_failures.clear();

    cmsContext ctx = (cmsContext)0x1234;

    cmsPluginParametricCurves Plugin;
    Plugin.Evaluator = 5;
    Plugin.nFunctions = 3;
    cmsUInt32Number ftypes[3] = { 10, 20, 30 };
    cmsUInt32Number pcount[3] = { 1, 2, 3 };
    Plugin.FunctionTypes = ftypes;
    Plugin.ParameterCount = pcount;

    cmsPluginBase* Data = (cmsPluginBase*) &Plugin;

    cmsBool ret = _cmsRegisterParametricCurvesPlugin(ctx, Data);
    CHECK(ret == TRUE, "Expected TRUE return for valid Data");
    // Validate the new node in the linked list
    _cmsParametricCurvesCollection* fl = g_ctx.ParametricCurves;
    CHECK(fl != NULL, "ParametricCurves node should be allocated and linked");
    if (fl != NULL) {
        CHECK(fl->Evaluator == 5, "Evaluator should be copied to new node");
        CHECK(fl->nFunctions == 3, "nFunctions should be copied (3)");
        CHECK(fl->FunctionTypes[0] == 10 && fl->FunctionTypes[1] == 20 && fl->FunctionTypes[2] == 30,
              "FunctionTypes should be copied correctly");
        CHECK(fl->ParameterCount[0] == 1 && fl->ParameterCount[1] == 2 && fl->ParameterCount[2] == 3,
              "ParameterCount should be copied correctly");
        CHECK(fl->Next == NULL, "Next should be NULL when there was no prior list");
    }

    if (g_failures.empty()) {
        std::cout << "[PASS] test_normal_input\n";
    } else {
        for (auto &s : g_failures) std::cerr << s << "\n";
        std::cout << "[FAIL] test_normal_input\n";
    }
}

static void test_overflow_nFunctions() {
    reset_ctx();
    g_failures.clear();

    cmsContext ctx = (cmsContext)0x1234;

    // Prepare large arrays for FunctionTypes and ParameterCount
    cmsUInt32Number ftypes[ MAX_TYPES_IN_LCMS_PLUGIN ];
    cmsUInt32Number pcount[ MAX_TYPES_IN_LCMS_PLUGIN ];
    for (unsigned int i = 0; i < MAX_TYPES_IN_LCMS_PLUGIN; ++i) {
        ftypes[i] = i;
        pcount[i] = i + 1000; // some arbitrary distinct values
    }

    cmsPluginParametricCurves Plugin;
    Plugin.Evaluator = 7;
    Plugin.nFunctions = 100;          // > MAX_TYPES_IN_LCMS_PLUGIN
    Plugin.FunctionTypes = ftypes;
    Plugin.ParameterCount = pcount;

    cmsPluginBase* Data = (cmsPluginBase*) &Plugin;

    cmsBool ret = _cmsRegisterParametricCurvesPlugin(ctx, Data);
    CHECK(ret == TRUE, "Expected TRUE return even when nFunctions exceeds max, should cap");
    _cmsParametricCurvesCollection* fl = g_ctx.ParametricCurves;
    CHECK(fl != NULL, "ParametricCurves node should be allocated");
    if (fl != NULL) {
        CHECK(fl->nFunctions == MAX_TYPES_IN_LCMS_PLUGIN, "nFunctions should be capped at MAX_TYPES_IN_LCMS_PLUGIN");
        // Validate that the first few values were copied (0,1,2) given how we filled ftypes
        CHECK(fl->FunctionTypes[0] == 0 && fl->FunctionTypes[1] == 1 && fl->FunctionTypes[2] == 2,
              "First entries should be copied correctly after cap");
        // We don't check all 64 values to avoid overflow concerns
        CHECK(fl->Next == NULL, "When no prior list, Next should be NULL (since initial was NULL)");
    }

    if (g_failures.empty()) {
        std::cout << "[PASS] test_overflow_nFunctions\n";
    } else {
        for (auto &s : g_failures) std::cerr << s << "\n";
        std::cout << "[FAIL] test_overflow_nFunctions\n";
    }
}

static void test_malloc_failure() {
    reset_ctx();
    g_failures.clear();

    cmsContext ctx = (cmsContext)0x1234;

    g_failMalloc = 1; // force malloc to fail

    cmsPluginParametricCurves Plugin;
    Plugin.Evaluator = 9;
    Plugin.nFunctions = 2;
    cmsUInt32Number ftypes[2] = { 42, 43 };
    cmsUInt32Number pcount[2] = { 5, 6 };
    Plugin.FunctionTypes = ftypes;
    Plugin.ParameterCount = pcount;

    cmsPluginBase* Data = (cmsPluginBase*) &Plugin;

    cmsBool ret = _cmsRegisterParametricCurvesPlugin(ctx, Data);
    CHECK(ret == FALSE, "Expected FALSE return when memory allocation fails");
    CHECK(g_ctx.ParametricCurves == NULL, "ParametricCurves should remain NULL after malloc failure");

    if (g_failures.empty()) {
        std::cout << "[PASS] test_malloc_failure\n";
    } else {
        for (auto &s : g_failures) std::cerr << s << "\n";
        std::cout << "[FAIL] test_malloc_failure\n";
    }

    g_failMalloc = 0; // reset
}

// ----------------------
// Main entry: run all tests and display summary
// ----------------------

int main() {
    // Run tests
    test_null_data_branch();
    test_normal_input();
    test_overflow_nFunctions();
    test_malloc_failure();

    // Summary
    if (g_failures.empty()) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << g_failures.size() << " TEST(S) FAILED\n";
        for (const auto& s : g_failures) std::cout << s << "\n";
        return 1;
    }
}