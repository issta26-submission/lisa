// cmsopt_unit_test.cpp
// Lightweight unit tests for cmsBool _cmsRegisterOptimizationPlugin in cmsopt.c
// This test suite uses the real Little CMS internal interfaces exposed by lcms2_internal.h.
// It is designed to be compiled together with cmsopt.c (and the library core) in a C++11 project.
// Note: This file uses a minimal custom test harness to avoid external testing frameworks.
// Explanatory comments are provided for each test case.

#include <lcms2_internal.h>
#include <iostream>
#include <string>
#include <lcms2.h>


extern "C" {
}

// Simple non-terminating test harness
static int g_failures = 0;

static void fail(const std::string& msg) {
    std::cerr << "Test failure: " << msg << std::endl;
    ++g_failures;
}

// Macro: if condition is false, record a failure with a message; continue tests
#define CHECK(cond, msg) do { if(!(cond)) fail(msg); } while(0)


// Helper to fetch the internal OptimizationPlugin chunk for a given context.
// This relies on the internal API (_cmsContextGetClientChunk) and the internal type
// _cmsOptimizationPluginChunkType defined in lcms2_internal.h.
static _cmsOptimizationPluginChunkType* getOptimizationChunk(cmsContext ctx) {
    return ( _cmsOptimizationPluginChunkType*) _cmsContextGetClientChunk(ctx, OptimizationPlugin);
}


// Dummy optimize callback functions to simulate real plugin behavior.
static cmsBool MyOptimizePtr1(CMSREGISTER cmsPipeline* Lut,
                              CMSREGISTER cmsUInt32Number Intent,
                              CMSREGISTER cmsUInt32Number* InputFormat,
                              CMSREGISTER cmsUInt32Number* OutputFormat,
                              CMSREGISTER cmsUInt32Number* dwFlags)
{
    // Simple stub: claim success
    return TRUE;
}

static cmsBool MyOptimizePtr2(CMSREGISTER cmsPipeline* Lut,
                              CMSREGISTER cmsUInt32Number Intent,
                              CMSREGISTER cmsUInt32Number* InputFormat,
                              CMSREGISTER cmsUInt32Number* OutputFormat,
                              CMSREGISTER cmsUInt32Number* dwFlags)
{
    // Simple stub: claim success
    return TRUE;
}


// Test 1: Data == NULL should reset the head to NULL and return TRUE.
static void test_register_null_data_sets_head_to_null() {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    _cmsOptimizationPluginChunkType* chunk = getOptimizationChunk(ctx);
    if (chunk == NULL) {
        fail("Test 1: could not obtain optimization plugin chunk from context.");
        cmsDeleteContext(ctx);
        return;
    }

    // Ensure current head is non-NULL by registering a dummy plugin first (to avoid tail assumptions)
    cmsPluginOptimization dummy;
    dummy.OptimizePtr = MyOptimizePtr1;
    _cmsRegisterOptimizationPlugin(ctx, (cmsPluginBase*) &dummy);

    // Now call with NULL data
    cmsBool res = _cmsRegisterOptimizationPlugin(ctx, NULL);

    CHECK(res == TRUE, "Test 1: _cmsRegisterOptimizationPlugin(NULL) should return TRUE.");
    CHECK(chunk->OptimizationCollection == NULL, "Test 1: OptimizationCollection should be NULL after registering NULL data.");

    cmsDeleteContext(ctx);
}

// Test 2: Data != NULL but Plugin.OptimizePtr == NULL should return FALSE.
static void test_null_optimizer_ptr_returns_false() {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        fail("Test 2: failed to create CMS context.");
        return;
    }

    cmsPluginOptimization Plugin;
    Plugin.OptimizePtr = NULL; // Simulate missing optimizer callback

    cmsBool res = _cmsRegisterOptimizationPlugin(ctx, (cmsPluginBase*) &Plugin);

    CHECK(res == FALSE, "Test 2: _cmsRegisterOptimizationPlugin with NULL OptimizePtr must return FALSE.");

    cmsDeleteContext(ctx);
}

// Test 3: Data != NULL and valid OptimizePtr should allocate a collection node at head
// and store the pointer to the plugin's OptimizePtr.
static void test_valid_plugin_allocates_head() {
    cmsContext ctx = cmsCreateContext(NULL, NULL);
    if (ctx == NULL) {
        fail("Test 3: failed to create CMS context.");
        return;
    }

    _cmsOptimizationPluginChunkType* chunk = getOptimizationChunk(ctx);
    if (chunk == NULL) {
        fail("Test 3: could not obtain optimization plugin chunk from context.");
        cmsDeleteContext(ctx);
        return;
    }

    // Prepare first plugin
    cmsPluginOptimization Plugin1;
    Plugin1.OptimizePtr = MyOptimizePtr1;

    cmsBool res1 = _cmsRegisterOptimizationPlugin(ctx, (cmsPluginBase*) &Plugin1);
    CHECK(res1 == TRUE, "Test 3: First plugin registration should succeed.");

    _cmsOptimizationCollection* head1 = chunk->OptimizationCollection;
    CHECK(head1 != NULL, "Test 3: Head should be non-NULL after first registration.");
    CHECK(head1->OptimizePtr == Plugin1.OptimizePtr, "Test 3: Head's OptimizePtr should match Plugin1.OptimizePtr.");

    // Prepare second plugin
    cmsPluginOptimization Plugin2;
    Plugin2.OptimizePtr = MyOptimizePtr2;

    cmsBool res2 = _cmsRegisterOptimizationPlugin(ctx, (cmsPluginBase*) &Plugin2);
    CHECK(res2 == TRUE, "Test 3: Second plugin registration should succeed.");

    _cmsOptimizationCollection* head2 = chunk->OptimizationCollection;
    CHECK(head2 != NULL, "Test 3: Head should be non-NULL after second registration.");
    CHECK(head2->OptimizePtr == Plugin2.OptimizePtr, "Test 3: New head's OptimizePtr should match Plugin2.OptimizePtr.");
    CHECK(head2->Next == head1, "Test 3: New head's Next should point to the previous head (head1).");

    cmsDeleteContext(ctx);
}


// Test runner
static void run_all_tests() {
    test_register_null_data_sets_head_to_null();
    test_null_optimizer_ptr_returns_false();
    test_valid_plugin_allocates_head();
}

int main() {
    // Run tests and report results
    run_all_tests();

    if (g_failures == 0) {
        std::cout << "All cmsRegisterOptimizationPlugin tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " cmsRegisterOptimizationPlugin tests FAILED." << std::endl;
        return 1;
    }
}