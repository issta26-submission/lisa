// Test suite for GenerateCRD in psicc.c using a hand-written test framework (no GTest)

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdarg>
#include <utils.h>


// Forward declare the focal API (as it appears in psicc.c)
extern "C" {
    // Types used by GenerateCRD
    typedef void* cmsHPROFILE;
    typedef unsigned int cmsUInt32Number;

    // Global/test-controlled variables that GenerateCRD uses
    extern int cOutProf;
    extern int Intent;
    extern int BlackPointCompensation;
    extern int Undecorated;
    extern size_t NumOfGridPoints;

    // The file-wide output stream used by GenerateCRD
    extern FILE* OutFile;

    // The function under test (from psicc.c)
    void GenerateCRD(void);

    // Mockable dependencies (to be supplied by this test harness)
    cmsHPROFILE OpenStockProfile(int, int);
    size_t cmsGetPostScriptCRD(int, cmsHPROFILE, int, cmsUInt32Number, char*, cmsUInt32Number);
    void cmsCloseProfile(cmsHPROFILE);
    void FatalError(const char* fmt, ...);
}

// Lightweight test harness state (mocked dependencies)
namespace TestHarness {
    // Mock control flags
    bool mockOpenReturnNull = false;
    bool mockGetCRDReturnZeroFirstCall = false;

    // Captured information from GenerateCRD calls
    cmsUInt32Number lastDwFlags = 0;
    int lastIntent = -1;

    // Fatal error capture
    bool fatalErrorCalled = false;
    std::string fatalErrorMessage;

    // Utility to reset per-test state
    void ResetState() {
        mockOpenReturnNull = false;
        mockGetCRDReturnZeroFirstCall = false;
        lastDwFlags = 0;
        lastIntent = -1;
        fatalErrorCalled = false;
        fatalErrorMessage.clear();
    }

    // Mock OpenStockProfile
    extern "C" cmsHPROFILE OpenStockProfile(int, int) {
        (void)0; // silence unused if inlined
        if (mockOpenReturnNull) return NULL;
        // Return a non-NULL opaque handle
        return (cmsHPROFILE)(void*)0xdeadbeef;
    }

    // Mock cmsGetPostScriptCRD
    extern "C" size_t cmsGetPostScriptCRD(int, cmsHPROFILE, int IntentParam, cmsUInt32Number dwFlags,
                                         char* Buffer, cmsUInt32Number n) {
        // Capture call details for assertions
        lastDwFlags = dwFlags;
        lastIntent = IntentParam;

        // If test config says the first call with Buffer==NULL should yield zero, emulate that
        if (mockGetCRDReturnZeroFirstCall && Buffer == NULL && n == 0) {
            mockGetCRDReturnZeroFirstCall = false; // consume the flag
            return 0;
        }

        // Normal mock: report a fixed string when a buffer is provided
        const char* mockStr = "CRD_OUTPUT";
        size_t L = std::strlen(mockStr);
        if (Buffer == NULL) {
            // First call: return the required length
            return L;
        } else {
            // Copy content into Buffer with proper termination
            if (n > (cmsUInt32Number)L) {
                // Copy full string and terminate
                std::memcpy(Buffer, mockStr, L);
                Buffer[L] = '\0';
            } else {
                // Copy at most n chars and terminate within bounds
                std::memcpy(Buffer, mockStr, n);
                // Ensure termination after copied region
                Buffer[n] = '\0';
            }
            // In real cmsGetPostScriptCRD, the function would return the length of the string.
            // Return the actual string length for consistency.
            return L;
        }
    }

    // Mock cmsCloseProfile (no-op)
    extern "C" void cmsCloseProfile(cmsHPROFILE) {
        // nothing to do
    }

    // Mock FatalError (non-terminating)
    extern "C" void FatalError(const char* fmt, ...) {
        fatalErrorCalled = true;
        char buf[256];
        va_list args;
        va_start(args, fmt);
        vsnprintf(buf, sizeof(buf), fmt, args);
        va_end(args);
        fatalErrorMessage = buf;
    }

    // Helper: read OutFile contents into a std::string
    std::string ReadOutFileContents(FILE* f) {
        if (!f) return "";
        // Flush and seek to end to determine size
        fflush(f);
        if (fseek(f, 0, SEEK_END) != 0) return "";
        long end = ftell(f);
        if (end < 0) return "";
        fseek(f, 0, SEEK_SET);
        if (end == 0) return "";
        char* buf = new char[end + 1];
        size_t read = fread(buf, 1, (size_t)end, f);
        buf[read] = '\0';
        std::string content(buf);
        delete[] buf;
        // Restore position if needed
        fflush(f);
        fseek(f, 0, SEEK_SET);
        return content;
    }

    // Helper to create a fresh temporary output file
    FILE* CreateOutFile() {
        // tmpfile() creates a temporary binary file that is automatically removed on close
        return tmpfile();
    }
} // namespace TestHarness

// Bring mocks into the correct linkage for the focal file
using namespace TestHarness;

// Global declarations expected by psicc.c (test harness provides these via externs above)
int cOutProf = 0;
int Intent = 0;
int BlackPointCompensation = 0;
int Undecorated = 0;
size_t NumOfGridPoints = 0;
FILE* OutFile = NULL;

// Helper: run a single test and return pass/fail
#define RUN_TEST(testFunc) (testFunc() ? std::cout << "[PASS] " #testFunc << std::endl : std::cout << "[FAIL] " #testFunc << std::endl)

// Test 1: When OpenStockProfile returns NULL, GenerateCRD should not write output
bool test_OpenProfileNull_NoOutput() {
    ResetState();
    mockOpenReturnNull = true;

    // Prepare output
    OutFile = CreateOutFile();
    // Configure globals
    cOutProf = 0;
    Intent = 0;
    BlackPointCompensation = 0;
    Undecorated = 0;
    NumOfGridPoints = 0;

    // Execute
    GenerateCRD();

    // Read output
    std::string content = ReadOutFileContents(OutFile);

    // Cleanup
    if (OutFile) fclose(OutFile);
    OutFile = NULL;

    // Verify: no output written
    bool ok = content.empty() && !fatalErrorCalled;
    if (!ok) {
        std::cerr << "Test_OpenProfileNull_NoOutput failed: content='" << content
                  << "' fatalErrorCalled=" << fatalErrorCalled << std::endl;
    }
    return ok;
}

// Test 2: PrecalcMode 0 should set LOWRESPRECALC and write CRD_OUTPUT
bool test_PrecalcMode0_WritesOutput() {
    ResetState();
    mockOpenReturnNull = false;
    mockGetCRDReturnZeroFirstCall = false;

    OutFile = CreateOutFile();
    cOutProf = 0;
    Intent = 7;
    BlackPointCompensation = 1;
    Undecorated = 0;
    NumOfGridPoints = 0;
    TestHarness::mockGetCRDReturnZeroFirstCall = false;

    // Setup
    README_ME: ;
    // Apply test configuration
    // (the label above is harmless; it's just to ensure no unused label warning in some compilers)
    // Set PrecalcMode via global variable in the focal code
    // Note: GenerateCRD uses PrecalcMode, so set it via the global symbol
    extern int PrecalcMode;
    *(int*)&PrecalcMode = 0; // direct assign; handled by linker in C

    // Execute
    GenerateCRD();

    // Read output
    std::string content = ReadOutFileContents(OutFile);

    // Cleanup
    if (OutFile) fclose(OutFile);
    OutFile = NULL;

    // Verify: content equals CRD_OUTPUT and flags contain LOWRESPRECALC and BLACKPOINT
    bool ok = (content == "CRD_OUTPUT") && (lastDwFlags & 0x01) && (lastDwFlags & 0x04);
    if (!ok) {
        std::cerr << "Test_PrecalcMode0_WritesOutput failed: content='" << content
                  << "', lastDwFlags=0x" << std::hex << lastDwFlags << std::dec
                  << ", fatalErrorCalled=" << fatalErrorCalled << std::endl;
    }
    return ok;
}

// Test 3: PrecalcMode 2 should set HIGHRESPRECALC and write CRD_OUTPUT
bool test_PrecalcMode2_WritesOutput() {
    ResetState();
    OutFile = CreateOutFile();
    cOutProf = 0;
    Intent = 3;
    BlackPointCompensation = 0;
    Undecorated = 0;
    NumOfGridPoints = 0;

    extern int PrecalcMode;
    *(int*)&PrecalcMode = 2;

    GenerateCRD();

    std::string content = ReadOutFileContents(OutFile);

    if (OutFile) fclose(OutFile);
    OutFile = NULL;

    bool ok = (content == "CRD_OUTPUT") && (lastDwFlags & 0x02);
    if (!ok) {
        std::cerr << "Test_PrecalcMode2_WritesOutput failed: content='" << content
                  << "', lastDwFlags=0x" << std::hex << lastDwFlags << std::dec
                  << ", fatalErrorCalled=" << fatalErrorCalled << std::endl;
    }
    return ok;
}

// Test 4: PrecalcMode 1 with NumOfGridPoints > 0 should set GRIDPOINTS flags and write output
bool test_PrecalcMode1_WithGridPoints() {
    ResetState();
    OutFile = CreateOutFile();
    cOutProf = 0;
    Intent = 5;
    BlackPointCompensation = 0;
    Undecorated = 0;
    NumOfGridPoints = 5;

    extern int PrecalcMode;
    *(int*)&PrecalcMode = 1;

    GenerateCRD();

    std::string content = ReadOutFileContents(OutFile);

    if (OutFile) fclose(OutFile);
    OutFile = NULL;

    // Expect GRIDPOINTS flag pattern: 0x1000 | 5
    bool hasGrid = (lastDwFlags & 0x1000) && ((lastDwFlags & 5) == 5);
    bool ok = (content == "CRD_OUTPUT") && hasGrid;
    if (!ok) {
        std::cerr << "Test_PrecalcMode1_WithGridPoints failed: content='" << content
                  << "', lastDwFlags=0x" << std::hex << lastDwFlags << std::dec
                  << ", fatalErrorCalled=" << fatalErrorCalled << std::endl;
    }
    return ok;
}

// Test 5: Invalid PrecalcMode should call FatalError
bool test_InvalidPrecalcMode_CallsFatalError() {
    ResetState();
    OutFile = CreateOutFile();
    cOutProf = 0;
    Intent = 1;
    BlackPointCompensation = 0;
    Undecorated = 0;
    NumOfGridPoints = 0;

    extern int PrecalcMode;
    *(int*)&PrecalcMode = 999;

    GenerateCRD();

    std::string content = ReadOutFileContents(OutFile);
    if (OutFile) fclose(OutFile);
    OutFile = NULL;

    bool ok = fatalErrorCalled;
    if (!ok) {
        std::cerr << "Test_InvalidPrecalcMode_CallsFatalError failed: content='" << content
                  << "', fatalErrorCalled=" << fatalErrorCalled << std::endl;
    }
    return ok;
}

// Test 6: When cmsGetPostScriptCRD returns 0 on first call, GenerateCRD should not write output
bool test_CRDLengthZero_Path() {
    ResetState();
    mockGetCRDReturnZeroFirstCall = true; // first call will return 0 (simulate no CRD available)

    OutFile = CreateOutFile();
    cOutProf = 0;
    Intent = 1;
    BlackPointCompensation = 0;
    Undecorated = 0;
    NumOfGridPoints = 0;

    extern int PrecalcMode;
    *(int*)&PrecalcMode = 0;

    GenerateCRD();

    std::string content = ReadOutFileContents(OutFile);
    if (OutFile) fclose(OutFile);
    OutFile = NULL;

    bool ok = content.empty();
    if (!ok) {
        std::cerr << "Test_CRDLengthZero_Path failed: content='" << content
                  << "', fatalErrorCalled=" << fatalErrorCalled << std::endl;
    }
    return ok;
}

// Test 7: When OpenStockProfile returns NULL (null profile path) ensures no crash and no output
bool test_OpenProfileNull_Path_NoCrash() {
    ResetState();
    mockOpenReturnNull = true;

    OutFile = CreateOutFile();
    cOutProf = 0;
    Intent = 0;
    BlackPointCompensation = 0;
    Undecorated = 0;

    extern int PrecalcMode;
    *(int*)&PrecalcMode = 0;

    GenerateCRD();

    std::string content = ReadOutFileContents(OutFile);
    if (OutFile) fclose(OutFile);
    OutFile = NULL;

    bool ok = content.empty();
    if (!ok) {
        std::cerr << "Test_OpenProfileNull_Path_NoCrash failed: content='" << content
                  << "', fatalErrorCalled=" << fatalErrorCalled << std::endl;
    }
    return ok;
}

// Minimal main: run all tests
int main() {
    // Reset any static state
    TestHarness::ResetState();

    // Note: The PrecalcMode symbol is defined in psicc.c; we emulate changes via its address
    // If the linker provides a symbol with a different type, this may need adjustment.

    std::cout << "Starting tests for GenerateCRD (hand-written framework)" << std::endl;

    int passed = 0;
    int total = 0;

    auto run = [&](bool result){
        total++;
        if (result) ++passed;
        // else already logged inside the test
    };

    run(test_OpenProfileNull_NoOutput());
    run(test_PrecalcMode0_WritesOutput());
    run(test_PrecalcMode2_WritesOutput());
    run(test_PrecalcMode1_WithGridPoints());
    run(test_InvalidPrecalcMode_CallsFatalError());
    run(test_CRDLengthZero_Path());
    run(test_OpenProfileNull_Path_NoCrash());

    std::cout << "Tests passed: " << passed << "/" << total << std::endl;
    return (passed == total) ? 0 : 1;
}