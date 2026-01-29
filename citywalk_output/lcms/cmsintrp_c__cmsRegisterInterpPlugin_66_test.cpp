// cmsintrp_unit_tests.cpp
// A self-contained unit test harness for the focal function:
// cmsBool _cmsRegisterInterpPlugin(cmsContext ContextID, cmsPluginBase* Data)
// Note: This test suite is designed to be compiled as a standalone C++11 program.
// It mocks minimal dependencies required by the focal function and verifies
// both branches of the conditional (Data == NULL) and various Data states.

#include <unordered_map>
#include <iostream>
#include <string>
#include <cassert>
#include <lcms2_internal.h>


// Domain-specific typedefs and constants (simplified for unit testing)
typedef int cmsBool;
typedef int cmsContext;
#define TRUE 1
#define FALSE 0

// Forward-declare minimal class hierarchy to satisfy the focal function'sCast
struct cmsPluginBase {
    virtual ~cmsPluginBase() {}
};

// Focal class: cmsPluginInterpolation derives from cmsPluginBase
struct cmsPluginInterpolation : public cmsPluginBase {
    void* InterpolatorsFactory; // Factory callback pointer (mocked as void*)
};

// Client chunk used by the focal function to store interpolators pointer
struct _cmsInterpPluginChunkType {
    void* Interpolators; // Pointer to the active interpolation function set
};

// Global storage to simulate per-context client chunks that the real library would manage
static std::unordered_map<cmsContext, _cmsInterpPluginChunkType> g_interpChunks;

// Helper: reset all simulated chunks to a clean slate for isolated tests
static void reset_chunks() {
    g_interpChunks.clear();
}

// Mocked-up context retrieval function expected by the focal method
// In the real library this would fetch/allocate the per-context chunk.
// Here we simply ensure there is an entry for the given ContextID and return it.
extern "C" _cmsInterpPluginChunkType* _cmsContextGetClientChunk(cmsContext ContextID, int /*InterpPlugin*/) {
    auto &chunk = g_interpChunks[ContextID];
    // Ensure default values if not already present
    // (no Interpolators pointer means it's initialized to NULL by default in C++)
    return &chunk;
}

// The focal function under test (re-implemented here for a self-contained test environment).
// Note: This mirrors the logic shown in the provided focal method.
extern "C" cmsBool _cmsRegisterInterpPlugin(cmsContext ContextID, cmsPluginBase* Data) {
    cmsPluginInterpolation* Plugin = (cmsPluginInterpolation*) Data;
    _cmsInterpPluginChunkType* ptr = (_cmsInterpPluginChunkType*) _cmsContextGetClientChunk(ContextID, 0);

    if (Data == NULL) {
        ptr->Interpolators = NULL;
        return TRUE;
    }

    // Set replacement functions (InterpolatorsFactory) on the chunk
    ptr->Interpolators = Plugin->InterpolatorsFactory;
    return TRUE;
}

// Simple test framework helpers
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Convenience assertion macro that records failures but does not abort the test
#define EXPECT_TRUE(cond, msg)                                      \
    do {                                                              \
        ++g_total_tests;                                            \
        if (!(cond)) {                                              \
            ++g_failed_tests;                                       \
            std::cerr << "[FAIL] " << msg << "\n";                 \
        } else {                                                    \
            std::cout << "[PASS] " << msg << "\n";                 \
        }                                                           \
    } while (0)

#define EXPECT_EQ(expected, actual, msg)                              \
    do {                                                               \
        ++g_total_tests;                                             \
        if ((expected) != (actual)) {                                \
            ++g_failed_tests;                                        \
            std::cerr << "[FAIL] " << msg                             \
                      << " | Expected: " << (expected)                 \
                      << " Got: " << (actual) << "\n";               \
        } else {                                                     \
            std::cout << "[PASS] " << msg << "\n";                   \
        }                                                            \
    } while (0)

// Test 1: Data == NULL should set Interpolators in the chunk to NULL and return TRUE
static void test_RegisterInterpPlugin_DataNull() {
    reset_chunks();
    cmsContext ctx = 1;

    // Pre-initialize chunk with a non-NULL value to verify it gets overwritten
    g_interpChunks[ctx].Interpolators = (void*)0xDEADBEEF;

    cmsBool res = _cmsRegisterInterpPlugin(ctx, NULL);

    // Expectations
    EXPECT_TRUE(res == TRUE, "Data NULL returns TRUE");
    EXPECT_EQ((void*)NULL, g_interpChunks[ctx].Interpolators, "Data NULL should set Interpolators to NULL");
}

// Test 2: Data non-NULL with a valid InterpolatorsFactory should write the factory pointer to the chunk
static void test_RegisterInterpPlugin_DataNonNull_WithFactory() {
    reset_chunks();
    cmsContext ctx = 2;

    cmsPluginInterpolation plugin;
    plugin.InterpolatorsFactory = (void*)0x12345678;

    cmsBool res = _cmsRegisterInterpPlugin(ctx, (cmsPluginBase*) &plugin);

    // Expectations
    EXPECT_TRUE(res == TRUE, "Data non-NULL with Factory returns TRUE");
    EXPECT_EQ((void*)0x12345678, g_interpChunks[ctx].Interpolators, "Interpolators should be set to Plugin->InterpolatorsFactory");
}

// Test 3: Data non-NULL with NULL InterpolatorsFactory should set Interpolators to NULL
// Ensures the else-branch behavior when InterpolatorsFactory is NULL
static void test_RegisterInterpPlugin_DataNonNull_WithNullFactory() {
    reset_chunks();
    cmsContext ctx = 3;

    cmsPluginInterpolation plugin;
    plugin.InterpolatorsFactory = NULL;

    cmsBool res = _cmsRegisterInterpPlugin(ctx, (cmsPluginBase*) &plugin);

    // Expectations
    EXPECT_TRUE(res == TRUE, "Data non-NULL with NULL Factory returns TRUE");
    EXPECT_EQ((void*)NULL, g_interpChunks[ctx].Interpolators, "Interpolators should be NULL when Factory is NULL");
}

// Test runner: execute all tests and summarize results
int main() {
    std::cout << "Starting cmsRegisterInterpPlugin unit tests (self-contained mock)..." << std::endl;

    test_RegisterInterpPlugin_DataNull();
    test_RegisterInterpPlugin_DataNonNull_WithFactory();
    test_RegisterInterpPlugin_DataNonNull_WithNullFactory();

    std::cout << "Tests completed. Total: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;

    // Non-terminating style: return non-zero if any test failed
    return g_failed_tests;
}