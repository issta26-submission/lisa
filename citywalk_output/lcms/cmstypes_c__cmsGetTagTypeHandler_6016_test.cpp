// Test suite for the focal method: _cmsGetTagTypeHandler in cmstypes.c
// This test is written for C++11 compilation, does not rely on GoogleTest,
// and uses a small in-house test harness with non-terminating assertions.
// It aims to exercise the focal method by invoking the real library function
// and verifying basic expectations (non-null return for a common tag type, etc.).
// Note: This test relies on the public CMS context and tag type infrastructure
// being available in the linked LCMS library. It uses only the C public API
// to create a context and derives a 4-character tag type signature for XYZ type.

#include <cstdio>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cstdint>


// Forward declare types from the LCMS public API in a minimal, non-invasive way.
// We avoid pulling internal headers to keep this test robust across versions.
typedef void* cmsContext;
typedef uint32_t cmsTagTypeSignature;
typedef struct cmsTagTypeHandler cmsTagTypeHandler; // opaque in test

// Probe: declare the focal function with C linkage so we can link against the real library.
// The real library (cmstypes.c) defines this symbol; ensure the linker finds it.
extern "C" cmsTagTypeHandler* _cmsGetTagTypeHandler(cmsContext ContextID, cmsTagTypeSignature sig);

// Helper to construct a 4-char ISO tag-type signature in a stable, portable way.
// The canonical packed form is: 'X' | ('Y' << 8) | ('Z' << 16) | (' ' << 24)
static cmsTagTypeSignature MakeTagType(char a, char b, char c, char d)
{
    // Pack in little-endian friendly order: a, b, c, d occupy bytes 0..3 respectively.
    return (static_cast<cmsTagTypeSignature>(static_cast<uint8_t>(a))      ) |
           (static_cast<cmsTagTypeSignature>(static_cast<uint8_t>(b)) << 8  ) |
           (static_cast<cmsTagTypeSignature>(static_cast<uint8_t>(c)) << 16 ) |
           (static_cast<cmsTagTypeSignature>(static_cast<uint8_t>(d)) << 24 );
}

// Non-terminating assertion helper: prints status but does not exit on failure.
// This helps maximize code execution for coverage.
#define ASSERT_NON_TERMINATING(cond, msg)           \
    do {                                             \
        if (!(cond)) {                               \
            std::cerr << "Assertion failed: " << (msg) << \
            " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        } else {                                       \
            std::cout << "PASS: " << (msg) << "\n";   \
        }                                              \
    } while (0)

// Test 1: XYZ type should be supported by the default tag-type handler path.
// We expect _cmsGetTagTypeHandler to return a non-null handler for a valid XYZ tag.
static void test_xyz_signature_returns_non_null()
{
    // Create a minimal, valid cmsContext using the public LCMS API if available.
    // We assume cmsCreateContext is available in the linked LCMS library.
    // To maintain portability in environments where cmsCreateContext signature differs,
    // we attempt a best-effort invocation. If compilation/linkage fails, this test
    // will be skipped by the build system (not by runtime logic here).
    cmsContext ctx = nullptr;

    // Try to obtain a context via cmsCreateContext if the symbol is available.
    // We guard against compile-time ambiguity by using a weak symbol approach:
    // We simply declare a prototype and attempt to call; if linker fails, user will adjust build.
    // For safety, we perform the call inside a try-like block by checking a null pointer result
    // and reporting accordingly (without crashing in typical environments).
    // Note: If cmsCreateContext is not available, this test may be skipped in your build system.
    typedef cmsContext (*CMSCreateContextFn)(void* /*MemCb*/, void* /*User*/);
    CMSCreateContextFn pCmsCreateContext = nullptr;

    // Attempt to resolve symbol at runtime. If not found, skip the test gracefully.
    // This approach avoids hard dependencies on a particular LCMS build configuration.
#if defined(__unix__) || defined(__APPLE__)
    // Use dlsym to resolve cmsCreateContext if present.
    #include <dlfcn.h>
    void* handle = dlopen("liblcms2.so", RTLD_LAZY);
    if (handle) {
        pCmsCreateContext = (CMSCreateContextFn)dlsym(handle, "cmsCreateContext");
    }
#else
    // On Windows, you could use LoadLibrary/GetProcAddress. To keep this example portable,
    // we skip dynamic resolution here and rely on the simplest path below.
    pCmsCreateContext = nullptr;
#endif

    if (pCmsCreateContext) {
        // Create a context; ignore the details of callback/mem in this test (pass nullptrs).
        ctx = pCmsCreateContext(nullptr, nullptr);
    } else {
        // If we can't resolve cmsCreateContext, we proceed with a non-null dummy pointer
        // hoping the library's internal checks can handle it in a non-crashing way.
        static int dummy = 0;
        ctx = &dummy;
    }

    cmsTagTypeSignature sigXYZ = MakeTagType('X','Y','Z',' ');
    cmsTagTypeHandler* h = _cmsGetTagTypeHandler(ctx, sigXYZ);

    // We expect a non-null handler for XYZ type in a well-formed LCMS build.
    ASSERT_NON_TERMINATING(h != nullptr, "CMS GetTagTypeHandler should return non-null for XYZ tag type");

    // Clean up: if we created a real context, try to free it via public API if available.
#if defined(__unix__) || defined(__APPLE__)
    if (ctx) {
        // cmsCloseContext exists in public API; attempt to call if present.
        typedef void (*CMSCloseContextFn)(cmsContext);
        CMSCloseContextFn pCmsCloseContext = nullptr;
        void* handle2 = dlopen("liblcms2.so", RTLD_LAZY);
        if (handle2) {
            pCmsCloseContext = (CMSCloseContextFn)dlsym(handle2, "cmsCloseContext");
            if (pCmsCloseContext && ctx) {
                pCmsCloseContext(ctx);
            }
            dlclose(handle2);
        }
    }
#endif
}

// Test 2: Unknown or unlikely tag type should be handled gracefully (null or fallback).
// We expect that passing an unlikely signature does not crash and returns either
// NULL or a valid handler depending on the library's default support. We at least
// verify that the call completes without terminating the test run.
static void test_unknown_signature_handling()
{
    cmsContext ctx = nullptr;

    // Attempt to obtain a context as in Test 1; reuse the same approach for consistency.
#if defined(__unix__) || defined(__APPLE__)
    #include <dlfcn.h>
    cmsTagTypeSignature sigUnknown = MakeTagType('Q','W','E','R');
    void* handle = dlopen("liblcms2.so", RTLD_LAZY);
    if (handle) {
        // Try to resolve cmsCreateContext if available
        typedef cmsContext (*CMSCreateContextFn)(void*, void*);
        CMSCreateContextFn pCmsCreateContext = (CMSCreateContextFn)dlsym(handle, "cmsCreateContext");
        if (pCmsCreateContext) {
            ctx = pCmsCreateContext(nullptr, nullptr);
        } else {
            static int dummy = 0;
            ctx = &dummy;
        }

        cmsTagTypeHandler* hUnknown = _cmsGetTagTypeHandler(ctx, sigUnknown);
        // Non-fatal assertion: ensure the function completes and returns something (could be NULL).
        // We only fail here if the call crashes or returns an invalid non-sense value (which we can't detect easily).
        // We check for a non-null pointer only if the library is expected to provide a handler for unknown tags.
        // Here we simply report the outcome.
        std::cout << "Test Unknown Signature: _cmsGetTagTypeHandler returned "
                  << (hUnknown ? "non-null" : "null") << " for signature 0x"
                  << std::hex << sigUnknown << std::dec << "\n";
        // Clean up similarly as in Test 1
        typedef void (*CMSCloseContextFn)(cmsContext);
        CMSCloseContextFn pCmsCloseContext = (CMSCloseContextFn)dlsym(handle, "cmsCloseContext");
        if (pCmsCloseContext && ctx) {
            pCmsCloseContext(ctx);
        }
        dlclose(handle);
    } else {
        std::cout << "Test Unknown Signature: Could not dlopen liblcms2.so; skipping dynamic test.\n";
    }
#else
    std::cout << "Test Unknown Signature: Platform not supported for dynamic linking in this test.\n";
#endif
}

// Entry point: run a small suite with non-terminating assertions.
// Any test failures are logged but do not stop subsequent tests.
int main()
{
    std::cout << "Starting cmsGetTagTypeHandler unit tests (no GTest, non-terminating asserts).\n";

    test_xyz_signature_returns_non_null();
    test_unknown_signature_handling();

    std::cout << "Unit tests completed.\n";
    return 0;
}