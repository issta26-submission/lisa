// C++11 unit test suite for the focal method:
// cmsTagTypeSignature _cmsGetTagTrueType(cmsHPROFILE hProfile, cmsTagSignature sig)
//
// Notes:
// - This test constructs a minimal in-memory _cmsICCPROFILE instance that mimics
//   the parts touched by _cmsGetTagTrueType.
// - We avoid any external test framework (GTest) per requirements.
// - We provide two tests to cover true and false branches of the search predicate.
// - All tests run from main() and print a summary. Non-terminating assertions are emulated
//   by counting passed/failed checks and continuing execution.
//
// This code expects the internal cms structures to be available via cmsio0 internal headers.
// It includes lcms2_internal.h to access _cmsICCPROFILE, cmsTagTypeHandler, etc.

#include <cstring>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include internal headers to access internal CMS structures.
// The exact include paths may vary by project setup.
// If the environment uses a different layout, adjust accordingly.
extern "C" {

// Declaration of the focal function (to be linked from the library)
cmsTagTypeSignature _cmsGetTagTrueType(cmsHPROFILE hProfile, cmsTagSignature sig);
}

// Simple test harness (non-GTest) with non-terminating assertions.

// Global counters for test results
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

// Helper macros for non-terminating checks
#define TEST_OK(cond, msg) \
    do { \
        ++g_tests_run; \
        if (cond) { \
            ++g_tests_passed; \
        } else { \
            ++g_tests_failed; \
            std::cerr << "TEST FAILED: " << msg << std::endl; \
        } \
    } while (0)


// Helper function: create a minimal in-memory ICC profile object sufficient for the test.
// It populates TagNames and TagTypeHandlers so that _cmsGetTagTrueType can operate.
// The function returns a pointer to a _cmsICCPROFILE allocated with new, so it can be deleted by the caller.
static _cmsICCPROFILE* makeMinimalIccForTest(cmsTagSignature sigToFind, cmsTagTypeSignature typeSig)
{
    // Allocate the internal ICC profile structure
    _cmsICCPROFILE* Icc = new _cmsICCPROFILE();

    // Allocate arrays with one entry (single tag)
    Icc->TagNames = new cmsTagSignature[1];
    Icc->TagTypeHandlers = new cmsTagTypeHandler*[1];
    Icc->TagPtrs = new void*[1];
    Icc->TagSizes = new cmsUInt32Number[1];
    Icc->TagOffsets = new cmsUInt32Number[1];
    Icc->TagLinked = new cmsTagSignature[1];
    Icc->TagSaveAsRaw = new cmsBool[1];

    // Set the tag that _cmsSearchTag will look for
    Icc->TagNames[0] = sigToFind;

    // Allocate and populate a TypeHandler with the desired signature.
    // The code under test only reads TypeHandler->Signature, so other fields are ignored.
    cmsTagTypeHandler* th = (cmsTagTypeHandler*) std::malloc(sizeof(cmsTagTypeHandler));
    if (th != nullptr) {
        // The member name is assumed to be 'Signature' by the focal code.
        // Cast to the correct type and set the signature.
        th->Signature = typeSig;
    }
    Icc->TagTypeHandlers[0] = th;

    // Initialize other tag arrays
    Icc->TagPtrs[0] = nullptr;
    Icc->TagSizes[0] = 0;
    Icc->TagOffsets[0] = 0;
    Icc->TagLinked[0] = (cmsTagSignature)0;
    Icc->TagSaveAsRaw[0] = FALSE;

    // Other required fields in _cmsICCPROFILE (ContextID, IOhandler, etc.)
    // are not used by _cmsGetTagTrueType, so we leave them default/uninitialized.

    return Icc;
}

// Helper function: free the memory allocated for the test ICC profile
static void freeIcc(_cmsICCPROFILE* Icc)
{
    if (Icc == nullptr) return;

    if (Icc->TagNames) {
        delete[] Icc->TagNames;
        Icc->TagNames = nullptr;
    }
    if (Icc->TagLinked) {
        delete[] Icc->TagLinked;
        Icc->TagLinked = nullptr;
    }
    if (Icc->TagSaveAsRaw) {
        delete[] Icc->TagSaveAsRaw;
        Icc->TagSaveAsRaw = nullptr;
    }
    if (Icc->TagSizes) {
        delete[] Icc->TagSizes;
        Icc->TagSizes = nullptr;
    }
    if (Icc->TagOffsets) {
        delete[] Icc->TagOffsets;
        Icc->TagOffsets = nullptr;
    }
    if (Icc->TagPtrs) {
        delete[] Icc->TagPtrs;
        Icc->TagPtrs = nullptr;
    }
    if (Icc->TagTypeHandlers) {
        // Free each handler allocated
        if (Icc->TagTypeHandlers[0]) {
            std::free(Icc->TagTypeHandlers[0]);
            Icc->TagTypeHandlers[0] = nullptr;
        }
        delete[] Icc->TagTypeHandlers;
        Icc->TagTypeHandlers = nullptr;
    }

    delete Icc;
}

// Test 1: Found scenario - the profile contains the requested tag and a valid TypeHandler.
// Expected: _cmsGetTagTrueType returns the Signature of the TypeHandler.
static void test_getTagTrueType_found()
{
    // Choose a fake tag and a distinct true-type signature
    cmsTagSignature tagToFind = (cmsTagSignature)0x11111111;
    cmsTagTypeSignature expectedType = (cmsTagTypeSignature)0x22222222;

    _cmsICCPROFILE* Icc = makeMinimalIccForTest(tagToFind, expectedType);
    if (Icc == nullptr) {
        std::cerr << "Test setup failed: could not allocate ICC profile." << std::endl;
        ++g_tests_failed;
        return;
    }

    // Call the focal function
    cmsTagTypeSignature res = _cmsGetTagTrueType((cmsHPROFILE)Icc, tagToFind);

    // Validate: should equal expectedType
    TEST_OK(res == expectedType, "Found: _cmsGetTagTrueType returns the tag's true-type signature");

    // Cleanup
    freeIcc(Icc);
}

// Test 2: Not-found scenario - the profile does not contain the requested tag.
// Expected: _cmsGetTagTrueType returns 0 (NULL-like signature).
static void test_getTagTrueType_notFound()
{
    // Tag to find that is NOT in the TagNames array
    cmsTagSignature tagToFind = (cmsTagSignature)0xDEADDEAD;
    // The typeSig value here is irrelevant since the tag won't be found; use a dummy
    cmsTagTypeSignature dummyType = (cmsTagTypeSignature)0xABCDABCD;

    // Profile with a different tag present
    cmsTagSignature presentTag = (cmsTagSignature)0xFEFEFEFE;
    _cmsICCPROFILE* Icc = makeMinimalIccForTest(presentTag, dummyType);
    if (Icc == nullptr) {
        std::cerr << "Test setup failed: could not allocate ICC profile." << std::endl;
        ++g_tests_failed;
        return;
    }

    // Call the focal function with a tag not present in Icc
    cmsTagTypeSignature res = _cmsGetTagTrueType((cmsHPROFILE)Icc, tagToFind);

    // Validate: should be 0
    TEST_OK(res == 0, "Not-found: _cmsGetTagTrueType returns 0 when tag is not present");

    // Cleanup
    freeIcc(Icc);
}

// Entry point: run tests and print summary
int main()
{
    test_getTagTrueType_found();
    test_getTagTrueType_notFound();

    std::cout << "Unit test summary: "
              << g_tests_passed << " passed, "
              << g_tests_failed << " failed, "
              << g_tests_run << " total." << std::endl;

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}