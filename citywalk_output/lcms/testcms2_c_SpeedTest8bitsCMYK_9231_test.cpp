/*
  Unit test suite for SpeedTest8bitsCMYK (from testcms2.c) using a lightweight
  C++11 harness without GoogleTest or GMock.

  Strategy:
  - Provide lightweight C-callable stubs that override the LittleCMS runtime
    functions used by SpeedTest8bitsCMYK to make the test deterministic and fast.
  - Test the two branches of the NULL-check predicate:
      - TRUE branch: hlcmsProfileIn == NULL || hlcmsProfileOut == NULL
        Expect Die(...) to be invoked.
      - FALSE branch: both profiles non-NULL
        Expect the function to execute the transform path (via stubs) and to
        invoke the overridden transformer lifecycle functions.
  - Use the main() to invoke tests (as per domain knowledge requirement when gtest
    is not allowed).

  Notes:
  - We override:
      - Die(...) to flip a flag instead of exiting
      - cmsCreateTransformTHR / cmsCloseProfile / cmsDoTransform / cmsDeleteTransform
        to set and check simple lifecycle flags without performing real image processing.
      - TitlePerformance / PrintPerformance to no-ops
      - chknull to a simple passthrough
      - DbgThread to a benign pointer
      - malloc / free to use standard allocation so the 64 MB In buffer can be allocated safely
  - The test relies on the public API and types from the included test headers
    (testcms2.h / LittleCMS headers). The stubs assume that Scanline_rgba8 is the
    same layout as used by SpeedTest8bitsCMYK in testcms2.c.

  Build note:
  - Ensure you compile and link this test together with testcms2.c and the LittleCMS headers.
  - No external test framework is used; tests are invoked from main().

  This file provides: 
  - stubbed library calls
  - two tests
  - a simple test runner with explanatory comments
*/

#include <cstring>
#include <cstdio>
#include <iostream>
#include <testcms2.h>
#include <cassert>
#include <cstdlib>
#include <cstdarg>


// Include the project's test header to align types (Scanline_rgba8, etc.)
// This header is expected to define or declare things used by SpeedTest8bitsCMYK.
// If your environment uses a different path, adjust accordingly.

// Expose LittleCMS-style types and the SpeedTest8bitsCMYK symbol used in production code
extern "C" {

// Prototype of the focal method under test
void SpeedTest8bitsCMYK(const char * Title, cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut);

// Stubs for dependencies (overriding real implementations in testcms2.c)
typedef void* cmsHTRANSFORM;
typedef void* cmsHPROFILE;
typedef void* cmsContext;

// Canonical LittleCMS-like signatures (match the expectations in testcms2.c)
cmsHTRANSFORM cmsCreateTransformTHR(cmsContext ContextID, cmsHPROFILE InProfile, cmsColorSpaceSignature InType,
                                   cmsHPROFILE OutProfile, cmsColorSpaceSignature OutType,
                                   cmsInt32Number Intent, cmsFlags Flags);

void cmsCloseProfile(cmsHPROFILE hProfile);
void cmsDeleteTransform(cmsHTRANSFORM hTransform);
void cmsDoTransform(cmsHTRANSFORM hTransform, const void* In, void* Out, cmsUInt32Number size);

void* chknull(void* mem);
cmsContext DbgThread(void);
void* DebugMalloc(cmsContext ContextID, cmsUInt32Number size);
void DebugFree(cmsContext ContextID, void* Ptr);
void DebugMemPrintTotals(void);

void TitlePerformance(const char* Txt);
void PrintPerformance(cmsUInt32Number Bytes, cmsUInt32Number SizeOfPixel, cmsFloat64Number diff);

// We need a simple implementation to satisfy the 256^3 loops in the function
// The exact implementations in testcms2.h/test cms are not required for this test
// since we override the behavior to be deterministic.
}

// Global flags used by tests to verify which code path was taken
static bool DieCalledFlag = false;
static bool TransformCreatedFlag = false;
static bool DoTransformCalledFlag = false;
static bool TransformDeletedFlag = false;
static int ProfilesClosedCount = 0;

// Stub implementations

extern "C" {

// Die override: record the invocation but do not exit the process
void Die(const char* Reason, ...)
{
    DieCalledFlag = true;
    // Consume varargs to avoid warnings
    (void)Reason;
    va_list ap;
    va_start(ap, Reason);
    (void)ap;
    va_end(ap);
    // Do not exit to allow test to observe the branch behavior
}

// chknull passthrough (may be used by production code to check allocations)
void* chknull(void* mem)
{
    // Pass through; in tests we rely on malloc to provide real memory
    return mem;
}

// DbgThread used by the transform factory (return a benign non-null context)
cmsContext DbgThread(void)
{
    return reinterpret_cast<cmsContext>(1);
}

// A minimal implementation for malloc/free to ensure 64MB In buffer works
// The real program uses malloc/memory; here we delegate to libc for simplicity
void* malloc(size_t size)
{
    return std::malloc(size);
}

void free(void* p)
{
    std::free(p);
}

// Transform creation: record that a transform was created and return a dummy handle
cmsHTRANSFORM cmsCreateTransformTHR(cmsContext ContextID, cmsHPROFILE InProfile, cmsColorSpaceSignature InType,
                                   cmsHPROFILE OutProfile, cmsColorSpaceSignature OutType,
                                   cmsInt32Number Intent, cmsFlags Flags)
{
    (void)ContextID; (void)InProfile; (void)InType;
    (void)OutProfile; (void)OutType; (void)Intent; (void)Flags;
    TransformCreatedFlag = true;
    return reinterpret_cast<cmsHTRANSFORM>(static_cast<uintptr_t>(0xABCD));
}

// Close profile: track it, but do not perform any real action
void cmsCloseProfile(cmsHPROFILE hProfile)
{
    (void)hProfile;
    ProfilesClosedCount++;
}

// DoTransform: mark as called; avoid real processing for determinism
void cmsDoTransform(cmsHTRANSFORM hTransform, const void* In, void* Out, cmsUInt32Number size)
{
    (void)hTransform; (void)In;
    DoTransformCalledFlag = true;
    // The test does not rely on real transform results; keep memory safe
    if (Out && size > 0) {
        // Each Scanline_rgba8 is 4 bytes (r,g,b,a). size is number of pixels.
        size_t bytes = static_cast<size_t>(size) * sizeof(Scanline_rgba8);
        std::memset(Out, 0, bytes);
    }
}

// Delete transform: mark the event
void cmsDeleteTransform(cmsHTRANSFORM hTransform)
{
    (void)hTransform;
    TransformDeletedFlag = true;
}

// Title/Print performance: no-ops for unit tests
void TitlePerformance(const char* Txt) { (void)Txt; }
void PrintPerformance(cmsUInt32Number Bytes, cmsUInt32Number SizeOfPixel, cmsFloat64Number diff)
{
    (void)Bytes; (void)SizeOfPixel; (void)diff;
}
}

// Tests

// Simple assertion helper
#define ASSERT_TRUE(cond, msg) do { if(!(cond)) { std::cerr << "ASSERTION FAILED: " << (msg) << "\n"; exit(1); } } while(0)
#define ASSERT_FALSE(cond, msg) do { if((cond)) { std::cerr << "ASSERTION FAILED (expected false): " << (msg) << "\n"; exit(1); } } while(0)
#define ASSERT_EQ(a, b, msg) do { if(((a)!=(b))) { std::cerr << "ASSERTION FAILED: " << (msg) << " (" << (a) << " != " << (b) << ")\n"; exit(1); } } while(0)

// Test 1: Ensure true branch (NULL profiles) is taken
void test_SpeedTest8bitsCMYK_NullProfiles()
{
    DieCalledFlag = false;
    TransformCreatedFlag = false;
    DoTransformCalledFlag = false;
    TransformDeletedFlag = false;
    ProfilesClosedCount = 0;
    // Fresh in-memory buffers will be ignored due to NULL profiles check
    SpeedTest8bitsCMYK("Null Profiles Test", nullptr, nullptr);

    // We expect Die to be called
    ASSERT_TRUE(DieCalledFlag, "Die should be called when profiles are NULL");
    // The rest of the function may or may not proceed depending on environment.
    // Our stubs guard against crashes; we verify the path was exercised.
}

// Test 2: Ensure false branch (non-NULL profiles) runs through the transform lifecycle
void test_SpeedTest8bitsCMYK_NonNullProfiles()
{
    DieCalledFlag = false;
    TransformCreatedFlag = false;
    DoTransformCalledFlag = false;
    TransformDeletedFlag = false;
    ProfilesClosedCount = 0;

    // Create dummy non-null profile handles (the real content is ignored by stubs)
    cmsHPROFILE inProf = reinterpret_cast<cmsHPROFILE>(reinterpret_cast<uintptr_t>(1));
    cmsHPROFILE outProf = reinterpret_cast<cmsHPROFILE>(reinterpret_cast<uintptr_t>(2));

    SpeedTest8bitsCMYK("NonNull Profiles Test", inProf, outProf);

    // Verify that the transform path was invoked
    ASSERT_TRUE(TransformCreatedFlag, "Transform should be created for non-null profiles");
    ASSERT_TRUE(DoTransformCalledFlag, "cmsDoTransform should be invoked during the transform");
    ASSERT_TRUE(TransformDeletedFlag, "Transform should be deleted after use");

    // Ensure profiles were closed
    ASSERT_EQ(ProfilesClosedCount, 2, "Two profile closes should have been invoked (In and Out)");
}

// Minimal wrapper to expose test entry points as C-linkable, if needed by build system
extern "C" int main(int argc, char* argv[])
{
    (void)argc; (void)argv;
    std::cout << "Running SpeedTest8bitsCMYK unit tests (mocked environment)..." << std::endl;

    try {
        test_SpeedTest8bitsCMYK_NullProfiles();
        std::cout << "PASS: Null profiles test" << std::endl;

        test_SpeedTest8bitsCMYK_NonNullProfiles();
        std::cout << "PASS: Non-null profiles test" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "TEST FAILED with exception: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "TEST FAILED with unknown exception" << std::endl;
        return 1;
    }

    std::cout << "All tests completed." << std::endl;
    return 0;
}