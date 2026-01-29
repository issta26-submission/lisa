// SpeedTest16bitsGray_test.cpp
// A custom C++11 unit test suite for SpeedTest16bitsGray (testcms2.c focal method).
// No GTest; uses a small in-file test harness with non-terminating assertions.
// Provides stub implementations for cms* library dependencies to enable isolated testing.

#include <cstring>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <testcms2.h>
#include <cstdlib>
#include <cstdarg>
#include <lcms2.h>
#include <cmath>


// Attempt to include the LittleCMS header to get the types.
// If unavailable in the target environment, this file may need adjustment.
// The test harness relies on these types (cmsHPROFILE, cmsHTRANSFORM, etc.)
// and the function prototype for SpeedTest16bitsGray.

extern "C" {
    // Forward declare the focal function under test (SpeedTest16bitsGray).
    void SpeedTest16bitsGray(const char * Title, cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut, cmsInt32Number Intent);

    // Dependency stubs required by the focal method and declared in <FOCAL_CLASS_DEP>.
    typedef void* cmsContext;
    // Typedefs come from lcms2.h; we rely on them as provided.

    // Die and memory helpers
    void Die(const char* Reason, ...);
    void* chknull(void* mem);
    cmsContext DbgThread(void);

    // Transform and profile handling
    cmsHTRANSFORM cmsCreateTransformTHR(cmsContext ContextID,
                                      cmsHPROFILE In, cmsUInt32Number InFormat,
                                      cmsHPROFILE Out, cmsUInt32Number OutFormat,
                                      cmsInt32Number Intent, cmsFlags Flags);
    void cmsCloseProfile(cmsHPROFILE h);
    void cmsDeleteTransform(cmsHTRANSFORM xform);
    void cmsDoTransform(cmsHTRANSFORM xform, const void* In, void* Out, cmsUInt32Number Size);

    // Misc helpers used by the test scaffolding
    void TitlePerformance(const char* Txt);
    void PrintPerformance(cmsUInt32Number Bytes, cmsUInt32Number SizeOfPixel, cmsFloat64Number diff);

    // A simple dummy function to generate a transform handle in tests
    // We'll provide a minimal reference object.
    // (The actual content is not relevant for test assertions.)
    // Note: prototypes match the usage in SpeedTest16bitsGray.
}

// Global test state for assertions and verifications
static int g_failures = 0;
static int g_createTransformCalls = 0;
static int g_deleteTransformCalls = 0;
static int g_closeProfileCalls = 0;
static bool g_transformWasInvoked = false;
static cmsUInt32Number g_lastPrintedBytes = 0;
static cmsUInt32Number g_lastPrintedSizeOfPixel = 0;
static double g_lastPrintDiff = 0.0;
static const char* g_lastTitlePrinted = nullptr;

// Simple assertion helpers (non-terminating)
#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "ASSERT_TRUE failed: " << (msg) << " (file " << __FILE__ << ", line " << __LINE__ << ")\n"; \
        ++g_failures; \
    } \
} while(0)

#define ASSERT_EQ(a,b, msg) do { \
    if(!((a) == (b))) { \
        std::cerr << "ASSERT_EQ failed: " #a " != " #b " (" << (a) << " != " << (b) << "). " << (msg) \
                  << " (file " << __FILE__ << ", line " << __LINE__ << ")\n"; \
        ++g_failures; \
    } \
} while(0)

// C linkage stubs implementing the dependencies

extern "C" {

// Die: instead of terminating, throw to let test catch expected errors
void Die(const char* Reason, ...) {
    // Build a reasonable message from variadic args
    va_list args;
    va_start(args, Reason);
    // Simple formatting: we won't attempt full vprintf here to avoid complexity
    // Just throw a generic exception indicating Die was called
    va_end(args);
    throw std::runtime_error("Die() called by focal method");
}

// chknull: basic null-check wrapper
void* chknull(void* mem) {
    if (mem == nullptr) {
        Die("Out of memory");
    }
    return mem;
}

// DbgThread: provide a dummy context
cmsContext DbgThread(void) {
    static int dummy = 0;
    return (cmsContext)&dummy;
}

// cmsCreateTransformTHR: stub that increases counter and returns a fake handle
typedef struct { int dummy; } _TransformStub;
static _TransformStub g_transformStub;
cmsHTRANSFORM cmsCreateTransformTHR(cmsContext ContextID,
                                  cmsHPROFILE In, cmsUInt32Number InFormat,
                                  cmsHPROFILE Out, cmsUInt32Number OutFormat,
                                  cmsInt32Number Intent, cmsFlags Flags)
{
    (void)ContextID; (void)In; (void)InFormat; (void)Out; (void)OutFormat;
    (void)Intent; (void)Flags;
    ++g_createTransformCalls;
    return (cmsHTRANSFORM)&g_transformStub;
}

// cmsCloseProfile: stub
void cmsCloseProfile(cmsHPROFILE h) {
    (void)h;
    ++g_closeProfileCalls;
}

// cmsDeleteTransform: stub
void cmsDeleteTransform(cmsHTRANSFORM xform) {
    (void)xform;
    ++g_deleteTransformCalls;
}

// cmsDoTransform: simple identity transform; also mark that transform occurred
void cmsDoTransform(cmsHTRANSFORM xform, const void* In, void* Out, cmsUInt32Number Size) {
    (void)xform;
    const cmsUInt16Number* in = (const cmsUInt16Number*)In;
    cmsUInt16Number* out = (cmsUInt16Number*)Out;
    // Mark that the transform was performed
    g_transformWasInvoked = true;
    // Simple copy to simulate a no-op transform
    for (cmsUInt32Number i = 0; i < Size; ++i) {
        out[i] = in[i];
    }
    (void)Size;
}

// TitlePerformance: capture last title
void TitlePerformance(const char* Txt) {
    g_lastTitlePrinted = Txt;
}

// PrintPerformance: capture size and timing.
void PrintPerformance(cmsUInt32Number Bytes, cmsUInt32Number SizeOfPixel, cmsFloat64Number diff) {
    (void)diff;
    g_lastPrintedBytes = Bytes;
    g_lastPrintedSizeOfPixel = SizeOfPixel;
    g_lastPrintDiff = (double)diff;
}

// Optional: a minimal external symbol to satisfy linker if necessary
// (Not strictly required by the focal method tests; provided for completeness)
}

// End of extern "C" block

// Helper reset function
static void ResetGlobals() {
    g_createTransformCalls = 0;
    g_deleteTransformCalls = 0;
    g_closeProfileCalls = 0;
    g_transformWasInvoked = false;
    g_lastPrintedBytes = 0;
    g_lastPrintedSizeOfPixel = 0;
    g_lastPrintDiff = 0.0;
    g_lastTitlePrinted = nullptr;
}

// Test 1: Normal flow with non-null profiles should call transform exactly once and print performance
static void Test_SpeedTest16bitsGray_NormalFlow() {
    ResetGlobals();

    // Dummy non-null profile handles (taking addresses of local objects to form pointers)
    int dummyIn = 1;
    int dummyOut = 2;
    cmsHPROFILE hlcmsProfileIn = (cmsHPROFILE)&dummyIn;
    cmsHPROFILE hlcmsProfileOut = (cmsHPROFILE)&dummyOut;

    try {
        SpeedTest16bitsGray("NormalFlowTest", hlcmsProfileIn, hlcmsProfileOut, 0);
    } catch (...) {
        ASSERT_TRUE(false, "SpeedTest16bitsGray unexpectedly threw an exception in NormalFlow");
        return;
    }

    // Assertions about internal behavior
    ASSERT_EQ(g_createTransformCalls, 1, "cmsCreateTransformTHR should be called exactly once");
    ASSERT_EQ(g_deleteTransformCalls, 1, "cmsDeleteTransform should be called exactly once");
    ASSERT_EQ(g_closeProfileCalls, 2, "cmsCloseProfile should be called for both input and output profiles");

    ASSERT_TRUE(g_transformWasInvoked, "cmsDoTransform should have been invoked");

    // Mb = 256*256*256 * sizeof(cmsUInt16Number) = 33554432
    const cmsUInt32Number ExpectedMb = 33554432u;
    const cmsUInt32Number ExpectedSizeOfPixel = sizeof(cmsUInt16Number);
    ASSERT_EQ(g_lastPrintedBytes, ExpectedMb, "Printed bytes should equal Mb (33554432)");
    ASSERT_EQ(g_lastPrintedSizeOfPixel, ExpectedSizeOfPixel, "Size of a single pixel should be 2 bytes for 16-bit gray");
    // diff is not asserted strictly; ensure we produced a non-negative value via the print path
}

// Test 2: Null input profile should trigger Die (exception)
static void Test_SpeedTest16bitsGray_NullInProfile() {
    ResetGlobals();
    int dummyIn = 1;
    cmsHPROFILE hlcmsProfileOut = (cmsHPROFILE)&dummyIn;

    bool caught = false;
    try {
        SpeedTest16bitsGray("NullInProfileTest", (cmsHPROFILE)nullptr, hlcmsProfileOut, 0);
    } catch (const std::exception& e) {
        caught = true;
        // Expected path
        (void)e;
    } catch (...) {
        caught = true;
    }
    ASSERT_TRUE(caught, "SpeedTest16bitsGray should throw when hlcmsProfileIn is NULL");
}

// Test 3: Null output profile should trigger Die (exception)
static void Test_SpeedTest16bitsGray_NullOutProfile() {
    ResetGlobals();
    int dummyIn = 1;
    cmsHPROFILE hlcmsProfileIn = (cmsHPROFILE)&dummyIn;

    bool caught = false;
    try {
        SpeedTest16bitsGray("NullOutProfileTest", hlcmsProfileIn, (cmsHPROFILE)nullptr, 0);
    } catch (const std::exception& e) {
        caught = true;
        (void)e;
    } catch (...) {
        caught = true;
    }
    ASSERT_TRUE(caught, "SpeedTest16bitsGray should throw when hlcmsProfileOut is NULL");
}

// Runner
static void RunAllTests() {
    std::cout << "Running SpeedTest16bitsGray unit tests...\n";

    Test_SpeedTest16bitsGray_NormalFlow();
    Test_SpeedTest16bitsGray_NullInProfile();
    Test_SpeedTest16bitsGray_NullOutProfile();

    if (g_failures == 0) {
        std::cout << "All tests PASSED.\n";
    } else {
        std::cout << g_failures << " test(s) FAILED.\n";
    }
}

// Main: entry point for the custom test harness
int main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    RunAllTests();
    return (g_failures == 0) ? 0 : 1;
}