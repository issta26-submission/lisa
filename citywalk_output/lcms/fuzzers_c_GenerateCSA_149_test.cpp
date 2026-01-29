// Test suite for the focal method GenerateCSA in fuzzers.c
// This test suite does not rely on Google Test. It uses a minimal
// custom test harness and simple assertions to maximize code coverage.
// The tests mock the external CMS library interactions (cmsGetPostScriptCSA,
// _cmsMalloc, _cmsFree) by providing stubs in this translation unit.
// The goal is to validate true/false branches around the PostScript CSA buffer
// handling in GenerateCSA.
//
// Important: The test suite expects to be linked with the fuzzers.c object file
// (containing GenerateCSA) in the final binary. We override the external
// dependencies by providing compatible C-callable implementations here.
// We do not modify the original fuzzers.c logic; we only simulate its runtime
// environment for controlled testing.

#include <cstring>
#include <lcms2_plugin.h>
#include <vector>
#include <iostream>
#include <string>
#include <stdint.h>
#include <cstdlib>


// ---------------------------------------------------------------------------
// Mocked "lcms2_plugin.h" definitions (minimal shim for compilation of fuzzers.c)
// We present the necessary types and function declarations that fuzzers.c uses.
// In the actual build, fuzzers.c will see these declarations from its header.
// By placing them here and compiling this file together with fuzzers.c, we ensure
// that the linker resolves cmsGetPostScriptCSA, _cmsMalloc, and _cmsFree to our
// test-defined stubs.
// ---------------------------------------------------------------------------

// Typedefs to resemble library definitions
typedef void* cmsHPROFILE;
typedef unsigned int cmsUInt32Number;

// Function prototypes (extern "C" to ensure C linkage for the fuzzers.c symbol names)
extern "C" cmsUInt32Number cmsGetPostScriptCSA(cmsHPROFILE, cmsHPROFILE, cmsUInt32Number,
                                             cmsUInt32Number, char*, cmsUInt32Number);
extern "C" void* _cmsMalloc(void*, size_t);
extern "C" void _cmsFree(void*, void*);

// Publicly callable focal entry (as declared in fuzzers.c)
extern "C" void GenerateCSA(void* hProfile);

// ---------------------------------------------------------------------------
// Test harness state (mocks and trackers)
// ---------------------------------------------------------------------------

// Global test state to control mock behavior and collect results
static int g_call_index = 0;                 // Tracks number of cmsGetPostScriptCSA invocations
static int g_scenario = 0;                   // 0 => test: n == 0; 1 => test: n != 0
static int g_malloc_calls = 0;               // Count of _cmsMalloc invocations
static int g_free_calls = 0;                 // Count of _cmsFree invocations
static void* g_last_alloc_ptr = nullptr;     // Pointer returned by _cmsMalloc
static void* g_last_free_ptr = nullptr;      // Pointer passed to _cmsFree
static std::string g_last_written_content;   // Content written by mock to Buffer

// ---------------------------------------------------------------------------
// Mocks: cmsGetPostScriptCSA, _cmsMalloc, _cmsFree
// These are C-callable implementations that simulate the real library calls
// but are controlled from within the test to verify GenerateCSA behavior.
// ---------------------------------------------------------------------------

extern "C" cmsUInt32Number cmsGetPostScriptCSA(cmsHPROFILE /*src*/, cmsHPROFILE /*dst*/,
                                             cmsUInt32Number /*a*/, cmsUInt32Number /*b*/,
                                             char* Buffer, cmsUInt32Number Size) {
    // First invocation in GenerateCSA is with Buffer==NULL and Size==0 to query size.
    if (Buffer == nullptr && Size == 0) {
        // Test configuration:
        // - Scenario 0: simulate 0-length CSA (branch false)
        // - Scenario 1: simulate non-zero length CSA (branch true)
        if (g_scenario == 0) {
            g_call_index++;
            return 0;
        } else {
            // Return a non-zero size for the buffer allocation path
            g_call_index++;
            return 4; // small, deterministic size
        }
    } else {
        // Second invocation: Buffer is provided with Size indicating the buffer length.
        if (Size > 0 && Buffer != nullptr) {
            // Fill the buffer with known data and record the content for verification
            const char* content = "ABCD";
            size_t len = 4;
            if (Size < (cmsUInt32Number)len) len = static_cast<size_t>(Size);
            if (len > 0) {
                std::memcpy(Buffer, content, len);
                g_last_written_content.assign(content, content + len);
            } else {
                g_last_written_content.clear();
            }
        }
        g_call_index++;
        // Return value is not used by GenerateCSA after the second call; keep simple.
        return 4;
    }
}

extern "C" void* _cmsMalloc(void* /*ctx*/, size_t Size) {
    // Track allocation and return a raw memory buffer
    g_malloc_calls++;
    void* p = std::malloc(Size);
    g_last_alloc_ptr = p;
    return p;
}

extern "C" void _cmsFree(void* /*ctx*/, void* ptr) {
    // Track deallocation and free memory
    g_free_calls++;
    g_last_free_ptr = ptr;
    std::free(ptr);
}

// ---------------------------------------------------------------------------
// Lightweight unit test helpers
// ---------------------------------------------------------------------------

static void log_msg(const std::string& msg) {
    std::cout << msg << std::endl;
}

static void expect_true(bool cond, const std::string& msg) {
    if (!cond) {
        log_msg("  [FAIL] " + msg);
    } else {
        log_msg("  [OK]   " + msg);
    }
}

// Test 1: Ensure GenerateCSA returns early when n == 0 (no allocation, no free, no buffer handling)
static bool test_GenerateCSA_n_zero_no_alloc_no_free() {
    // Prepare: scenario 0 means first call to cmsGetPostScriptCSA returns 0
    g_scenario = 0;
    g_call_index = 0;
    g_malloc_calls = 0;
    g_free_calls = 0;
    g_last_alloc_ptr = nullptr;
    g_last_free_ptr = nullptr;
    g_last_written_content.clear();

    // Call focal method with a null profile handle (as in production usage)
    GenerateCSA(nullptr);

    // Validate: no allocation and no free should have occurred
    bool pass = (g_malloc_calls == 0) && (g_free_calls == 0);
    if (!pass) {
        log_msg("test_GenerateCSA_n_zero_no_alloc_no_free: unexpected alloc/free.");
    }
    // Also ensure that second path was not taken (no buffer writes)
    // We can't read Buffer, but we can verify we didn't go into allocation path by checking counters
    expect_true(pass, "GCSA should return early when n == 0 (no buffer/allocations).");
    return pass;
}

// Test 2: Ensure GenerateCSA allocates, writes data, and frees when n != 0
static bool test_GenerateCSA_n_nonzero_alloc_and_free_and_content() {
    // Prepare: scenario 1 means first cmsGetPostScriptCSA returns a non-zero size on first call
    g_scenario = 1;
    g_call_index = 0;
    g_malloc_calls = 0;
    g_free_calls = 0;
    g_last_alloc_ptr = nullptr;
    g_last_free_ptr = nullptr;
    g_last_written_content.clear();

    // Use a dummy non-null hProfile to simulate a real call
    GenerateCSA(reinterpret_cast<void*>(0xDEADBEEF));

    // Validate allocation and deallocation occurred exactly once each
    bool alloc_ok = (g_malloc_calls == 1);
    bool free_ok  = (g_free_calls == 1);

    // Validate that the buffer allocated was freed and the pointer passed to free
    bool pointer_consistency = (g_last_alloc_ptr == g_last_free_ptr) && (g_last_alloc_ptr != nullptr);

    // Validate that the mock wrote the expected content to Buffer
    bool content_ok = (g_last_written_content == "ABCD");

    expect_true(alloc_ok, "GCSA should call _cmsMalloc exactly once for non-zero n.");
    expect_true(free_ok, "GCSA should call _cmsFree exactly once after use.");
    expect_true(pointer_consistency, "GCSA should free the same buffer that was allocated.");
    expect_true(content_ok, "Buffer should be filled with expected content by cmsGetPostScriptCSA.");
    bool pass = alloc_ok && free_ok && pointer_consistency && content_ok;
    if (!pass) {
        log_msg("test_GenerateCSA_n_nonzero_alloc_and_content: one or more conditions failed.");
    }
    return pass;
}

// ---------------------------------------------------------------------------
// Test runner
// ---------------------------------------------------------------------------

static void run_all_tests() {
    log_msg("Running tests for GenerateCSA (fuzzers.c focal method) with mocked cmsGetPostScriptCSA/_cmsMalloc/_cmsFree...");

    int total = 0;
    int passed = 0;

    auto t1 = test_GenerateCSA_n_zero_no_alloc_no_free();
    total += 1;
    if (t1) passed += 1;

    auto t2 = test_GenerateCSA_n_nonzero_alloc_and_free_and_content();
    total += 1;
    if (t2) passed += 1;

    log_msg("--------------------------------------------------");
    log_msg("Test results: " + std::to_string(passed) + " / " + std::to_string(total) + " tests passed.");
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------

int main() {
    run_all_tests();
    return 0;
}