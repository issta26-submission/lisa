// Unit test suite for SpeedTest16bitsCMYK (from testcms2.c) using lightweight,
// hand-rolled stubs (no external test framework).
// This suite targets C++11 and does not rely on GTest. It mocks the minimal
// CMS API surface required by SpeedTest16bitsCMYK to compile and run in a
// controlled environment, enabling focused, repeatable checks.

#include <cstring>
#include <cstdio>
#include <iostream>
#include <stdexcept>
#include <testcms2.h>
#include <cassert>
#include <cstdlib>
#include <cstdint>


// Forward declarations to mirror the focal function's ambient CMS types
// (we only need the surface for linking with SpeedTest16bitsCMYK).
extern "C" {
    // Pseudo-C typedefs to match the CMS API surface used by SpeedTest16bitsCMYK.
    typedef void* cmsHPROFILE;
    typedef void* cmsHTRANSFORM;
    typedef void* cmsContext;
    typedef int   cmsInt32Number;
    typedef unsigned int cmsUInt32Number;
    typedef unsigned short cmsUInt16Number;
    typedef double cmsFloat64Number;

    // Macros used by the focal function (values chosen to be harmless in tests)
    #define TYPE_CMYK_16 0
    #define INTENT_PERCEPTUAL 0
    #define cmsFLAGS_NOCACHE 0

    // The focal function we want to exercise
    void SpeedTest16bitsCMYK(const char* Title, cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut);
    
    // The real code expects cubic-color data construction for 256^3 combinations.
    // We do not redefine the type of Scanline_rgba16 here; the function is exercised
    // through the external interface only, i.e., it accepts opaque pointers.
    
    // Lightweight, test-only CMS API stubs (implemented below in C++).
    cmsContext DbgThread(void);
    cmsHTRANSFORM cmsCreateTransformTHR(cmsContext ContextID,
                                        cmsHPROFILE In, cmsUInt32Number inType,
                                        cmsHPROFILE Out, cmsUInt32Number outType,
                                        cmsInt32Number Intent, cmsUInt32Number Flags);
    void cmsCloseProfile(cmsHPROFILE h);
    void cmsDeleteTransform(cmsHTRANSFORM xform);
    void cmsDoTransform(cmsHTRANSFORM xform, const void* In, void* Out, cmsUInt32Number size);
    void* chknull(void* mem);

    // Helpers used by the focal code
    void TitlePerformance(const char* Txt);
    void PrintPerformance(cmsUInt32Number Bytes, cmsUInt32Number SizeOfPixel, cmsFloat64Number diff);

    // Error signaling: in the real project, Die(...) aborts; for tests we throw.
    // NOTE: We avoid defining Die here to prevent potential symbol clashes with
    // the real test file in environments where both are linked.
}

// Test-scoped flags to verify that the focal method exercised the expected code paths.
static bool g_transformCreated = false;
static bool g_transformDoCalled = false;
static cmsHPROFILE g_lastInProfile = nullptr;
static cmsHPROFILE g_lastOutProfile = nullptr;

// Minimal, C-linkage implementations to satisfy the focal function's calls
extern "C" {

// A dummy DbgThread() context handle.
cmsContext DbgThread(void) {
    return nullptr;
}

// Simulated transform handle allocator (non-null to indicate success).
cmsHTRANSFORM cmsCreateTransformTHR(cmsContext /*ContextID*/,
                                    cmsHPROFILE In, cmsUInt32Number /*inType*/,
                                    cmsHPROFILE Out, cmsUInt32Number /*outType*/,
                                    cmsInt32Number /*Intent*/, cmsUInt32Number /*Flags*/) {
    g_transformCreated = true;
    g_lastInProfile = In;
    g_lastOutProfile = Out;
    // Return a non-null opaque handle
    return reinterpret_cast<cmsHTRANSFORM>(static_cast<void*>( reinterpret_cast<void*>(0xDEADBEEF) ));
}

// No-op: pretend to close a profile
void cmsCloseProfile(cmsHPROFILE /*h*/) {
    // no-op in tests
}

// No-op: pretend to delete a transform
void cmsDeleteTransform(cmsHTRANSFORM /*xform*/) {
    // no-op in tests
}

// Minimal transform driver: just mark as called; do not mutate memory.
void cmsDoTransform(cmsHTRANSFORM /*xform*/, const void* /*In*/, void* /*Out*/, cmsUInt32Number /*size*/) {
    g_transformDoCalled = true;
}

// Basic memory check: ensure non-null is returned; if NULL, abort to catch misuse early.
void* chknull(void* mem) {
    if (mem == nullptr) {
        fprintf(stderr, "Fatal: allocation returned NULL in test harness.\n");
        std::abort();
    }
    return mem;
}

// Decorative helpers (no-op in tests)
void TitlePerformance(const char* /*Txt*/) {
    // Could print progress in real tests; suppressed here
}
void PrintPerformance(cmsUInt32Number /*Bytes*/, cmsUInt32Number /*SizeOfPixel*/, cmsFloat64Number /*diff*/) {
    // No-op
}
}

// Lightweight test harness (no GoogleTest). Uses simple assertions and a small runner.
int main() {
    int totalTests = 0;
    int failedTests = 0;

    auto logTestResult = [&](const char* testName, bool passed) {
        totalTests++;
        if (!passed) {
            failedTests++;
            std::cerr << "[FAILED] " << testName << std::endl;
        } else {
            std::cout << "[PASSED] " << testName << std::endl;
        }
    };

    // Test 1: Ensure non-null profiles lead to a transform being created and a transform operation invoked.
    // Rationale: This validates the "happy path" where both profiles are present and the
    // SpeedTest16bitsCMYK function proceeds through its main workload.
    //
    // Important: The function constructs a 256^3 array and invokes cmsDoTransform.
    // Our test harness uses lightweight stubs that mark invocation without performing any real
    // color processing, ensuring fast, repeatable tests.
    {
        // Prepare fake profile handles (non-null)
        cmsHPROFILE inProf = reinterpret_cast<cmsHPROFILE>(0x1234);
        cmsHPROFILE outProf = reinterpret_cast<cmsHPROFILE>(0xABCD);

        // Reset state
        g_transformCreated = false;
        g_transformDoCalled = false;
        g_lastInProfile = nullptr;
        g_lastOutProfile = nullptr;

        // Call the focal method
        SpeedTest16bitsCMYK("unit test: valid profiles", inProf, outProf);

        // Verify that a transform was requested and transform did occur in some form
        bool passed = true;
        if (!g_transformCreated) {
            passed = false;
            std::cerr << "Transform was not created (expected in valid-profile path)." << std::endl;
        }
        if (!g_transformDoCalled) {
            // Depending on the stubbed cmsDoTransform, this may be set; ensure it's invoked.
            passed = false;
            std::cerr << "cmsDoTransform was not invoked (expected in valid-profile path)." << std::endl;
        }

        // Extra sanity: verify profiles fed into the transform were the ones we provided
        if (g_lastInProfile != inProf || g_lastOutProfile != outProf) {
            passed = false;
            std::cerr << "Transform was created with unexpected input/output profiles." << std::endl;
        }

        logTestResult("SpeedTest16bitsCMYK - valid profiles path", passed);
    }

    // Note on coverage for the false branch:
    // The focal code checks if hlcmsProfileIn == NULL || hlcmsProfileOut == NULL and would call Die(...)
    // in that case. Reproducing and testing that path cleanly requires hooking the Die()
    // implementation inside the same translation unit as SpeedTest16bitsCMYK or using a test
    // harness provided by the original project (which isn't available in this isolated setup).
    // To keep the test self-contained and non-terminating, we intentionally omit this path here.
    // If needed, a separate integration test with a real Die() handler could be added.

    // Final summary
    std::cout << "Tests run: " << totalTests << ", Failures: " << failedTests << std::endl;

    // Return non-zero if any test failed
    return failedTests != 0;
}

// Explanation notes (embedded as comments):
//
// - The test harness above provides a minimal C++11-based test environment without GTest.
// - It mocks essential CMS API functions used by SpeedTest16bitsCMYK with lightweight, safe stubs.
// - The "valid profiles" test exercises the core path where both input and output profiles are provided.
//   It asserts that a transform is created and the transform operation is invoked via the fake cmsDoTransform.
// - The heavy memory footprint (256^3 pixel data) is preserved in the focal function; the test harness
//   relies on the mocked cmsDoTransform to avoid any real color processing while still exercising
//   the code flow through the function call and memory allocation steps.
// - Coverage for the false branch (NULL profiles) would require intercepting Die(...) within the focal file.
//   This environment uses a separate test harness for that scenario if desired.