// Unit test suite for the focal method SpeedTest16 in threaded_testbed.c
// This test uses a lightweight, in-house test harness (no GTest) and
// mocks essential external dependencies to validate the behavior of SpeedTest16.
// The test suite focuses on verifying that:
// - SpeedTest16 creates and later deletes a cmsContext (noPlugin vs plugin mode)
// - SpeedTest16 reports exactly 10 performance measurements with correct titles
// - The first 5 measurements (noPlugin) have non-null context; the next 5 (plugin) use null context
// The approach intentionally avoids private members and uses only public interfaces mocked here.

#include <cstring>
#include <vector>
#include <threaded_internal.h>
#include <iostream>
#include <string>
#include <time.h>
#include <memory.h>
#include <stdlib.h>


// Step 1 (Candidate Keywords): SpeedTest16, SpeedTest16bitsRGB, SpeedTest16bitsCMYK,
// cmsCreateContext, cmsDeleteContext, trace, Performance, PROFILES_DIR, Scanline_rgb16bits,
// Scanline_cmyk16bits, test profiles (test5.icc, test3.icc, etc.)

// Domain-specific test harness and mocks for the focal function SpeedTest16.
// We assume the project provides the following C-compatible API; we recreate minimal
// stubs here to intercept and verify interactions when SpeedTest16 is executed.

extern "C" {

// Typedefs inferred to align with LittleCMS-like API used by SpeedTest16.
typedef void* cmsContext;
typedef void* cmsHPROFILE;
typedef double cmsFloat64Number;
typedef size_t cmsSize;

// perf_fn is the function pointer type used by Performance to run a timing test.
// The actual signature is: cmsFloat64Number fn(cmsContext, cmsHPROFILE, cmsHPROFILE)
typedef cmsFloat64Number (*perf_fn)(cmsContext, cmsHPROFILE, cmsHPROFILE);

// Prototypes of functions used by SpeedTest16 from the focal file.
// In the real project these would come from the project's headers.
cmsContext cmsCreateContext(int, int);
void cmsDeleteContext(cmsContext);
void trace(const char* frm, ...);
cmsFloat64Number Performance(const char* Title, perf_fn fn, cmsContext ct,
                             const char* inICC, const char* outICC,
                             size_t sz, cmsFloat64Number prev);

// SpeedTest16 is the focal function under test; it is defined in threaded_testbed.c.
// We declare it here so the test can invoke it.
void SpeedTest16(void);
}

// ---------------- Mock State for Verification ----------------

struct PerfCall {
    std::string Title;
    std::string InICC;
    std::string OutICC;
    void* ContextUsed; // Non-null for "noPlugin" calls; null for "plugin" calls
};

// Global state used by the mocks to capture interactions
static cmsContext g_createdContext = nullptr;
static bool g_deleteCalled = false;
static std::vector<PerfCall> g_perfCalls;

// Forward declaration of the real SpeedTest16 to ensure the linker resolves a symbol.
// The test harness provides the mock implementations of the dependencies (cmsCreateContext,
// cmsDeleteContext, trace, Performance) and then calls SpeedTest16.
extern "C" void SpeedTest16(void);

// Mock implementations with C linkage to intercept the focal method's dependencies

extern "C" cmsContext cmsCreateContext(int, int) {
    // Create a unique non-null pointer to simulate a context handle
    g_createdContext = reinterpret_cast<cmsContext>(new int(42));
    return g_createdContext;
}

extern "C" void cmsDeleteContext(cmsContext ctx) {
    // Mark deletion when the exact created context is provided
    if (ctx == g_createdContext && g_createdContext != nullptr) {
        g_deleteCalled = true;
        delete reinterpret_cast<int*>(ctx);
        g_createdContext = nullptr;
    }
}

extern "C" void trace(const char* /*frm*/, ...) {
    // No-op trace to avoid noisy test output; could be extended to log if needed
}

// Performance mock records each invocation for later verification.
// The actual signature matches the expected perf_fn type used by SpeedTest16.
extern "C" cmsFloat64Number Performance(const char* Title, perf_fn /*fn*/, cmsContext ct,
                                        const char* inICC, const char* outICC,
                                        size_t /*sz*/, cmsFloat64Number /*prev*/) {
    PerfCall pc;
    pc.Title    = (Title ? Title : "");
    pc.InICC    = (inICC ? inICC : "");
    pc.OutICC   = (outICC ? outICC : "");
    pc.ContextUsed = ct;
    g_perfCalls.push_back(std::move(pc));
    // Return a dummy value to simulate timing result; the exact value is not under test
    return 0.0;
}

// ---------------- Test Harness ----------------

#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)) { std::cerr << "ASSERT_TRUE FAILED: " << (msg) << " (line " << __LINE__ << ")" << std::endl; ok = false; } \
} while(0)

#define ASSERT_EQ(a, b, msg) do { \
    if(!((a) == (b))) { std::cerr << "ASSERT_EQ FAILED: " << (msg) << " | " #a " != " #b " (" << (a) << " != " << (b) << ") (line " << __LINE__ << ")" << std::endl; ok = false; } \
} while(0)

static bool run_SpeedTest16_mock_based_verification() {
    // Reset state
    g_perfCalls.clear();
    g_createdContext = nullptr;
    g_deleteCalled = false;

    // Execute the focal method
    SpeedTest16();

    // Validate that 10 performance measurements were recorded
    bool ok = true;
    ASSERT_EQ(g_perfCalls.size(), size_t(10), "SpeedTest16 should report 10 performance measurements");

    // Expected titles for the first block (noPlugin)
    const char* expTitlesBlock[5] = {
        "16 bits on CLUT profiles         ",
        "16 bits on Matrix-Shaper profiles",
        "16 bits on same Matrix-Shaper    ",
        "16 bits on curves                ",
        "16 bits on CMYK CLUT profiles    "
    };

    const char* expInBlock[5]  = {
        "test5.icc", "test5.icc", "test0.icc", "*curves", "test1.icc"
    };
    const char* expOutBlock[5] = {
        "test3.icc", "test0.icc", "test0.icc", "*curves", "test2.icc"
    };

    // Validate first block (indices 0..4)
    for (int i = 0; i < 5; ++i) {
        ASSERT_TRUE(i < (int)g_perfCalls.size(), "Insufficient perf calls for block 0");
        const PerfCall& pc = g_perfCalls[i];
        ASSERT_EQ(pc.Title, std::string(expTitlesBlock[i]), "Title mismatch in block 0, index " << i);
        ASSERT_EQ(pc.InICC, std::string(expInBlock[i]), "InICC mismatch in block 0, index " << i);
        ASSERT_EQ(pc.OutICC, std::string(expOutBlock[i]), "OutICC mismatch in block 0, index " << i);
        // In the noPlugin (ct != nullptr) case, we expect a non-null context
        ASSERT_TRUE(pc.ContextUsed != nullptr, "Expected non-null context in block 0, index " << i);
    }

    // Validate second block (indices 5..9) which uses the plugin context (ct == nullptr)
    for (int i = 0; i < 5; ++i) {
        const int idx = 5 + i;
        ASSERT_TRUE(idx < (int)g_perfCalls.size(), "Insufficient perf calls for block 1");
        const PerfCall& pc = g_perfCalls[idx];
        ASSERT_EQ(pc.Title, std::string(expTitlesBlock[i]), "Title mismatch in block 1, index " << i);
        ASSERT_EQ(pc.InICC, std::string(expInBlock[i]), "InICC mismatch in block 1, index " << i);
        ASSERT_EQ(pc.OutICC, std::string(expOutBlock[i]), "OutICC mismatch in block 1, index " << i);
        // In the plugin (ct == null) case, we expect a null context
        ASSERT_TRUE(pc.ContextUsed == nullptr, "Expected null context in block 1, index " << i);
    }

    // Ensure the context was deleted (noPlugin path)
    ASSERT_TRUE(g_deleteCalled, "cmsDeleteContext should have been called for noPlugin context");

    return ok;
}

// ---------------- Main Test Entry ----------------

int main() {
    std::cout << "Running SpeedTest16 unit test with mocked dependencies..." << std::endl;

    bool overall_ok = true;
    bool ok = run_SpeedTest16_mock_based_verification();
    if (!ok) {
        std::cerr << "Test failed: SpeedTest16 mocked verification did not pass." << std::endl;
        overall_ok = false;
    } else {
        std::cout << "Test passed: SpeedTest16 mocked verification succeeded." << std::endl;
    }

    // If more tests are added, run them here and accumulate results into overall_ok.

    if (!overall_ok) {
        std::cerr << "One or more tests FAILED." << std::endl;
        return 1;
    }

    std::cout << "All tests completed successfully." << std::endl;
    return 0;
}