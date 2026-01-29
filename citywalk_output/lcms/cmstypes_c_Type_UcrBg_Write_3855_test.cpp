// High-level unit test scaffold for the focal method Type_UcrBg_Write
// This test suite is designed for C++11 without a test framework (no GTest).
// It focuses on exercising the true-path and a single false-path of the method
// Type_UcrBg_Write located in cmstypes.c, using a minimal fake cmsIOHANDLER.
// The tests assume the public LittleCMS style type names (cmsUcr, cmsBg, cmsUcrBg, cmsMLU, etc.)
// and rely on the library's internal memory allocator reported via _cmsMalloc/_cmsFree.
// The tests compile against the project as a whole (cmstypes.c) and the public lcms2.h header(s).

#include <cstring>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>


// Forward declaration of types and function from LittleCMS headers.
// The exact headers and paths may vary per project layout.
// Adjust the include path as needed by your build system.
extern "C" {

 // The focal function under test
 cmsBool Type_UcrBg_Write(struct _cms_typehandler_struct* self, cmsIOHANDLER* io, void* Ptr, cmsUInt32Number nItems);

 // Internal memory/context related helpers (provided by the library)
 // We rely on the actual definitions inside cmstypes.c/.h at link time.
}

// Global state to control the fake IO behavior during tests
static int g_fakeWriteCallCount = 0;
static bool g_fakeWriteFailFirstCall = false;

// Fake Write callback to simulate cmsIOHANDLER behavior.
// The signature must match the library's cmsIOHANDLER Write function.
// We assume the library defines: cmsBool Write(struct _cmsIOHANDLER* io, cmsUInt32Number size, void* Buffer)
extern "C" cmsBool FakeWrite(struct _cmsIOHANDLER* io, cmsUInt32Number size, void* Buffer)
{
    // Fail on the first call if requested by the test
    if (g_fakeWriteFailFirstCall && g_fakeWriteCallCount == 0) {
        ++g_fakeWriteCallCount;
        return (cmsBool)0; // FALSE
    }
    // Track calls for debugging/coverage (no functional requirement)
    ++g_fakeWriteCallCount;
    return (cmsBool)1; // TRUE
}

// Test 1: Basic success path
// - Builds a valid cmsUcrBg structure with non-empty Ucr and Bg tables.
// - Desc is omitted (NULL) to simplify the encoding path.
// - Uses a fake IO that always returns TRUE on Write.
// - Expects Type_UcrBg_Write to return TRUE (success path taken).
static bool test_Type_UcrBg_Write_Success()
{
    // Reset fake IO state
    g_fakeWriteCallCount = 0;
    g_fakeWriteFailFirstCall = false;

    // Allocate and populate Ucr (Under color removal) and Bg (Black generation)
    cmsUcr* Ucr = (cmsUcr*)std::malloc(sizeof(cmsUcr));
    if (!Ucr) return false;
    Ucr->nEntries = 2;
    Ucr->Table16 = (cmsUInt16Number*)std::malloc(sizeof(cmsUInt16Number) * Ucr->nEntries);
    Ucr->Table16[0] = 11;
    Ucr->Table16[1] = 22;

    cmsBg* Bg = (cmsBg*)std::malloc(sizeof(cmsBg));
    if (!Bg) { std::free(Ucr); return false; }
    Bg->nEntries = 3;
    Bg->Table16 = (cmsUInt16Number*)std::malloc(sizeof(cmsUInt16Number) * Bg->nEntries);
    Bg->Table16[0] = 33;
    Bg->Table16[1] = 44;
    Bg->Table16[2] = 55;

    // Value container (cmsUcrBg) - must match the library's layout: Ucr/Bg/Desc pointers
    cmsUcrBg* Value = (cmsUcrBg*)std::malloc(sizeof(cmsUcrBg));
    if (!Value) { std::free(Ucr); std::free(Bg); return false; }
    Value->Ucr = Ucr;
    Value->Bg = Bg;
    Value->Desc = NULL; // We omit Desc for this test to avoid MLU setup complexity

    // Fake _cms_typehandler_struct instance (self) with a ContextID field.
    // We declare just enough to allow Type_UcrBg_Write to read ContextID.
    struct _cms_typehandler_struct fakeSelf;
    fakeSelf.ContextID = NULL; // Use default allocator (internal lib malloc) if NULL is allowed

    // Fake IO instance
    cmsIOHANDLER fakeIo;
    // Initialize to zero to mimic a real struct's state
    std::memset(&fakeIo, 0, sizeof(fakeIo));
    // Point to our single Write callback
    fakeIo.Write = &FakeWrite;

    // Call the function under test
    cmsBool result = Type_UcrBg_Write(&fakeSelf, &fakeIo, Value, 1);

    // Cleanup
    std::free(Ucr->Table16);
    std::free(Ucr);
    std::free(Bg->Table16);
    std::free(Bg);
    std::free(Value);

    // Validate result
    if (result != (cmsBool)1) {
        std::cerr << "test_Type_UcrBg_Write_Success: Type_UcrBg_Write returned FALSE on success path\n";
        return false;
    }
    return true;
}

// Test 2: First write failure should cause Type_UcrBg_Write to return FALSE
// - Uses the same data as the success test.
// - Configures the fake IO to fail on the first Write attempt (simulating a write error in the stream).
static bool test_Type_UcrBg_Write_FirstWriteFails()
{
    // Reset fake IO state to fail on first call
    g_fakeWriteCallCount = 0;
    g_fakeWriteFailFirstCall = true;

    // Allocate Ucr and Bg similarly to Test 1
    cmsUcr* Ucr = (cmsUcr*)std::malloc(sizeof(cmsUcr));
    if (!Ucr) return false;
    Ucr->nEntries = 2;
    Ucr->Table16 = (cmsUInt16Number*)std::malloc(sizeof(cmsUInt16Number) * Ucr->nEntries);
    Ucr->Table16[0] = 101;
    Ucr->Table16[1] = 202;

    cmsBg* Bg = (cmsBg*)std::malloc(sizeof(cmsBg));
    if (!Bg) { std::free(Ucr); return false; }
    Bg->nEntries = 2;
    Bg->Table16 = (cmsUInt16Number*)std::malloc(sizeof(cmsUInt16Number) * Bg->nEntries);
    Bg->Table16[0] = 303;
    Bg->Table16[1] = 404;

    cmsUcrBg* Value = (cmsUcrBg*)std::malloc(sizeof(cmsUcrBg));
    if (!Value) { std::free(Ucr); std::free(Bg); return false; }
    Value->Ucr = Ucr;
    Value->Bg = Bg;
    Value->Desc = NULL;

    struct _cms_typehandler_struct fakeSelf;
    fakeSelf.ContextID = NULL;

    cmsIOHANDLER fakeIo;
    std::memset(&fakeIo, 0, sizeof(fakeIo));
    fakeIo.Write = &FakeWrite;

    cmsBool result = Type_UcrBg_Write(&fakeSelf, &fakeIo, Value, 1);

    // Cleanup
    std::free(Ucr->Table16);
    std::free(Ucr);
    std::free(Bg->Table16);
    std::free(Bg);
    std::free(Value);

    // Expect FALSE due to first write failure
    if (result != (cmsBool)0) {
        std::cerr << "test_Type_UcrBg_Write_FirstWriteFails: expected FALSE, got TRUE\n";
        return false;
    }
    return true;
}

// Simple driver
int main() {
    bool all_passed = true;

    if (!test_Type_UcrBg_Write_Success()) {
        std::cerr << "Test 1 failed.\n";
        all_passed = false;
    } else {
        std::cout << "Test 1 passed: Type_UcrBg_Write_Success\n";
    }

    // Run the false-path test
    if (!test_Type_UcrBg_Write_FirstWriteFails()) {
        std::cerr << "Test 2 failed.\n";
        all_passed = false;
    } else {
        std::cout << "Test 2 passed: Type_UcrBg_Write_FirstWriteFails\n";
    }

    if (!all_passed) {
        std::cerr << "One or more tests FAILED.\n";
        return 1;
    }
    std::cout << "All tests PASSED.\n";
    return 0;
}

// Notes for maintainers:
// - The test intentionally avoids constructing a full cmsMLU Desc (text) structure to keep
//   the test focused on the core write-paths and to minimize complexity related to locale/text handling.
// - If needed, a more thorough test could add a Desc via cmsMLUalloc/cmsMLUsetASCII (public APIs) and
//   test the text-writing branch as well. This would require additional setup of a cmsMLU instance.
// - This test uses a minimal fake cmsIOHANDLER Write callback to simulate write success/failure.
// - Static/private members in the focal library should be left untouched; tests access only the public API
//   used by Type_UcrBg_Write. If the private layout of _cms_typehandler_struct changes, the test's
//   cast-to-Fake structure approach may need adjustment.