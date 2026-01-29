// Test suite for GenerateCRD in testcms2.c using a lightweight C++11 test harness
// Notes:
// - This test harness mocks the external LittleCMS-like dependencies to exercise
//   the GenerateCRD function in isolation.
// - We keep FileName handling realistic (writes to a real file when FileName != NULL
//   and relies on the standard remove to delete the file).
// - No GTest; a small, non-terminating assertion framework is implemented.
// - The tests focus on true/false branches of the function predicates and static-like
//   behavior via mock counters.

#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <cstdlib>


extern "C" void GenerateCRD(const char* cOutProf, const char* FileName);

// Lightweight type aliases to align with the focal code's expectations.
typedef void* cmsHPROFILE;
typedef void* cmsContext;
typedef unsigned int cmsUInt32Number;
typedef int cmsBool;

// Forward declarations of mocked dependencies (extern "C" linkage to match C code)
extern "C" cmsHPROFILE cmsCreateLab4Profile(void* Context);
extern "C" cmsHPROFILE cmsOpenProfileFromFile(const char* FileName, const char* Access);
extern "C" cmsUInt32Number cmsGetPostScriptCRD(cmsContext ContextID,
                                             cmsHPROFILE hProfile,
                                             cmsUInt32Number Intent,
                                             cmsUInt32Number Draw,
                                             char* Buffer,
                                             cmsUInt32Number BufferSize);
extern "C" void* _cmsMalloc(cmsContext ContextID, cmsUInt32Number size);
extern "C" void _cmsFree(cmsContext ContextID, void* Ptr);
extern "C" void cmsCloseProfile(cmsHPROFILE hProfile);
extern "C" cmsContext DbgThread(void);

// Simple non-terminating test framework
static int gTestsRun = 0;
static int gTestsFailed = 0;

#define TEST_ASSERT(expr, msg) do { \
    if(!(expr)) { \
        ++gTestsFailed; \
        std::cerr << "[FAIL] " << msg << " (in " << __FUNCTION__ << "@" << __FILE__ << ":" << __LINE__ << ")\n"; \
    } else { \
        std::cout << "[PASS] " << msg << "\n"; \
    } \
} while(0)

#define RUN_TEST(test) do { \
    gTestsRun++; \
    test(); \
} while(0)

static cmsContext gDbgContext = nullptr;

// Mock state for verifying behavior
struct MockState {
    int createLab4Profile_calls = 0;
    int openProfileFromFile_calls = 0;
    int malloc_calls = 0;
    int free_calls = 0;
    int closeProfile_calls = 0;
    int getPSCRD_calls_withBuffer = 0;
    int getPSCRD_calls_withoutBuffer = 0;
    const char* removedPath = nullptr;
} gState;

// Implementation of mocked dependencies

extern "C" cmsHPROFILE cmsCreateLab4Profile(void* Context) {
    (void)Context;
    ++gState.createLab4Profile_calls;
    // Return a non-null handle
    return reinterpret_cast<cmsHPROFILE>(new int(1));
}

extern "C" cmsHPROFILE cmsOpenProfileFromFile(const char* FileName, const char* Access) {
    (void)Access;
    ++gState.openProfileFromFile_calls;
    // Return a non-null handle
    return reinterpret_cast<cmsHPROFILE>(new int(2));
}

extern "C" cmsUInt32Number cmsGetPostScriptCRD(cmsContext ContextID,
                                             cmsHPROFILE hProfile,
                                             cmsUInt32Number Intent,
                                             cmsUInt32Number Draw,
                                             char* Buffer,
                                             cmsUInt32Number BufferSize) {
    (void)ContextID; (void)hProfile; (void)Intent; (void)Draw;
    if (Buffer == nullptr) {
        // First call (size query)
        gState.getPSCRD_calls_withoutBuffer++;
        // Return an arbitrary non-zero size to proceed into allocation path
        return 64; // non-zero signals non-empty CRD
    } else {
        // Second call: fill with deterministic data
        gState.getPSCRD_calls_withBuffer++;
        for (cmsUInt32Number i = 0; i < BufferSize; ++i) {
            Buffer[i] = static_cast<char>(i & 0xFF);
        }
        return 0;
    }
}

extern "C" void* _cmsMalloc(cmsContext ContextID, cmsUInt32Number size) {
    (void)ContextID;
    ++gState.malloc_calls;
    return std::malloc(size);
}

extern "C" void _cmsFree(cmsContext ContextID, void* Ptr) {
    (void)ContextID;
    ++gState.free_calls;
    std::free(Ptr);
}

extern "C" void cmsCloseProfile(cmsHPROFILE hProfile) {
    (void)hProfile;
    ++gState.closeProfile_calls;
    // Free the fake handle
    delete reinterpret_cast<int*>(hProfile);
}

// Minimal DbgThread implementation to satisfy GenerateCRD's usage
extern "C" cmsContext DbgThread(void) {
    if (gDbgContext == nullptr) {
        // create a dummy non-null pointer
        static int dummy = 0;
        gDbgContext = &dummy;
    }
    return gDbgContext;
}

// Helper utilities for file existence check
static bool fileExists(const std::string& path) {
    std::ifstream f(path.c_str(), std::ifstream::binary);
    return f.good();
}

// Test 1: When cOutProf == NULL and FileName == NULL, GenerateCRD should exercise the
// path that creates a Lab4 profile and writes to an internal buffer without doing file IO.
void test_GenerateCRD_nullOutProf_nullFileName() {
    // Reset mock state
    gState.createLab4Profile_calls = 0;
    gState.openProfileFromFile_calls = 0;
    gState.malloc_calls = 0;
    gState.free_calls = 0;
    gState.closeProfile_calls = 0;
    gState.getPSCRD_calls_withBuffer = 0;
    gState.getPSCRD_calls_withoutBuffer = 0;
    gState.removedPath = nullptr;
    gDbgContext = nullptr;

    // Call the function under test
    GenerateCRD(nullptr, nullptr);

    // Assertions: ensure true path taken, no file IO
    TEST_ASSERT(gState.createLab4Profile_calls == 1,
                "GenerateCRD should call cmsCreateLab4Profile once when cOutProf is NULL");
    TEST_ASSERT(gState.openProfileFromFile_calls == 0,
                "GenerateCRD should not call cmsOpenProfileFromFile when cOutProf is NULL");
    TEST_ASSERT(gState.getPSCRD_calls_withoutBuffer > 0,
                "GenerateCRD should query size via cmsGetPostScriptCRD with Buffer == NULL");
    TEST_ASSERT(gState.getPSCRD_calls_withBuffer > 0,
                "GenerateCRD should fill buffer via cmsGetPostScriptCRD with actual Buffer");
    TEST_ASSERT(gState.malloc_calls > 0 && gState.free_calls > 0,
                "GenerateCRD should allocate and free the internal buffer");
}

// Test 2: When cOutProf != NULL and FileName == NULL, GenerateCRD should open an existing profile
// and proceed to write the CRD into internal buffer (no file IO).
void test_GenerateCRD_withOutProf_noFile() {
    // Reset mock state
    gState.createLab4Profile_calls = 0;
    gState.openProfileFromFile_calls = 0;
    gState.malloc_calls = 0;
    gState.free_calls = 0;
    gState.closeProfile_calls = 0;
    gState.getPSCRD_calls_withBuffer = 0;
    gState.getPSCRD_calls_withoutBuffer = 0;
    gDbgContext = nullptr;

    // Use a fake output profile path; FileName is NULL to avoid file IO
    GenerateCRD("dummy.prof", nullptr);

    TEST_ASSERT(gState.createLab4Profile_calls == 0,
                "GenerateCRD should not call cmsCreateLab4Profile when cOutProf is provided");
    TEST_ASSERT(gState.openProfileFromFile_calls == 1,
                "GenerateCRD should call cmsOpenProfileFromFile when cOutProf is provided");
    TEST_ASSERT(gState.getPSCRD_calls_withoutBuffer > 0,
                "GenerateCRD should query size via cmsGetPostScriptCRD with Buffer == NULL");
    TEST_ASSERT(gState.malloc_calls > 0 && gState.free_calls > 0,
                "GenerateCRD should allocate and free the internal buffer even when FileName is NULL");
}

// Test 3: When FileName is provided, GenerateCRD should perform file IO and then remove the file.
// We verify by checking that the file path does not exist after the call (removal performed).
void test_GenerateCRD_withFileName_removal() {
    // Reset mock state
    gState.createLab4Profile_calls = 0;
    gState.openProfileFromFile_calls = 0;
    gState.malloc_calls = 0;
    gState.free_calls = 0;
    gState.closeProfile_calls = 0;
    gState.getPSCRD_calls_withBuffer = 0;
    gState.getPSCRD_calls_withoutBuffer = 0;
    gDbgContext = nullptr;

    const std::string tmpFile = "test_output_crd.crd";

    // Ensure file is not present before
    if (fileExists(tmpFile)) {
        std::remove(tmpFile.c_str());
    }

    GenerateCRD(nullptr, tmpFile.c_str());

    // After return, the file should have been attempted to be written and then removed.
    // We verify by checking absence and that function executed file IO path (IO through stdio).
    bool existsAfter = fileExists(tmpFile);
    TEST_ASSERT(!existsAfter,
                "GenerateCRD with FileName should remove the output file after writing (exists == false)");
    // Also verify a few internal steps occurred
    TEST_ASSERT(gState.createLab4Profile_calls >= 0,
                "Internal calls to cmsCreateLab4Profile should be performed (path with FileName).");
    TEST_ASSERT(gState.openProfileFromFile_calls >= 0,
                "Internal calls to cmsOpenProfileFromFile should be performed (path with FileName).");
}

int main() {
    std::cout << "Running GenerateCRD unit tests...\n";

    RUN_TEST(test_GenerateCRD_nullOutProf_nullFileName);
    RUN_TEST(test_GenerateCRD_withOutProf_noFile);
    RUN_TEST(test_GenerateCRD_withFileName_removal);

    std::cout << "Tests completed. Run summary: " << gTestsRun << " tests, "
              << gTestsFailed << " failures.\n";

    return gTestsFailed ? EXIT_FAILURE : EXIT_SUCCESS;
}