/*
Unit test suite for the focal method GenerateCSA in psicc.c
- No GoogleTest; a lightweight custom test harness is provided.
- Mocks for dependent C functions (OpenStockProfile, cmsGetPostScriptCSA, cmsCloseProfile)
  are provided in the same translation unit to allow isolated testing without the real library.
- Tests cover: NULL profile path, zero-length CSA path, and successful CSA generation.
- The tests use non-terminating EXPECT-like macros to exercise code paths and maximize coverage.
- Output is captured via a temporary FILE* OutFile used by the focal function.
- This test file is designed to be compiled together with psicc.c under C++11.
*/

// Step 3 (Domain Knowledge): Build a small, self-contained C++ test harness that provides
// mocks matching the signatures used by the focal code, and a minimal harness to run tests.

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <cassert>
#include <cstdlib>
#include <utils.h>


// Domain knowledge: The focal code uses the following external symbols/types:
//   - cmsHPROFILE (opaque pointer type for a profile)
//   - OpenStockProfile(int, void*)
//   - cmsGetPostScriptCSA(int, cmsHPROFILE, int, int, char*, cmsUInt32Number)
//   - cmsCloseProfile(cmsHPROFILE)
//   - OutFile (FILE*), cInProf (void*), and Intent (int)
typedef void* cmsHPROFILE;
typedef unsigned int cmsUInt32Number;

// Forward declarations to satisfy the linker for the focal function
extern "C" {
    cmsHPROFILE OpenStockProfile(int, void*);
    cmsUInt32Number cmsGetPostScriptCSA(int, cmsHPROFILE, int, int, char*, cmsUInt32Number);
    void cmsCloseProfile(cmsHPROFILE);
}

// Global variables expected by GenerateCSA (to be provided by test harness)
FILE* OutFile = nullptr;        // OUTPUT sink used by GenerateCSA
void* cInProf = nullptr;          // Input profile descriptor (mock)
int Intent = 0;                   // CSA "intent" parameter (mock)

// ---------- Mock implementations for dependencies (C linkage) ----------
static int g_testCase = 0; // 0 = default, 1 = NullProfile, 2 = ZeroLengthCSA, 3 = Success
static const char* MOCK_CSA_STR = "MOCK_CSA!"; // 9 characters
static const size_t MOCK_CSA_LEN = 9;

// Mock: OpenStockProfile
extern "C" cmsHPROFILE OpenStockProfile(int, void*) {
    // Behavior controlled by test case
    if (g_testCase == 1) {
        // Simulate failure to obtain a stock profile
        return NULL;
    }
    // Non-NULL handle can be any non-NULL pointer (mock)
    return (cmsHPROFILE) (void*) 0x1;
}

// Mock: cmsGetPostScriptCSA
extern "C" cmsUInt32Number cmsGetPostScriptCSA(int, cmsHPROFILE, int, int, char* Buffer, cmsUInt32Number n) {
    if (Buffer == NULL && n == 0) {
        // First call: return required length
        if (g_testCase == 3) {
            return (cmsUInt32Number) MOCK_CSA_LEN;
        } else {
            // For other test cases, indicate no data
            return (cmsUInt32Number) 0;
        }
    } else if (Buffer != NULL) {
        // Second call: fill buffer with CSA content
        // Ensure we don't overflow if n is smaller than content
        size_t copyLen = MOCK_CSA_LEN;
        if (n < copyLen) copyLen = n;
        memcpy(Buffer, MOCK_CSA_STR, copyLen);
        // No explicit return value used by caller; return 0 to indicate success
        return (cmsUInt32Number) 0;
    }
    return (cmsUInt32Number) 0;
}

// Mock: cmsCloseProfile
extern "C" void cmsCloseProfile(cmsHPROFILE) {
    // No resources to free in mock
}

// ---------- Utilities for test harness ----------
static std::string readOutFileContent(FILE* f) {
    if (f == nullptr) return "";
    long pos = ftell(f);
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    std::string s;
    s.resize((size_t)len);
    if (len > 0) {
        size_t readBytes = fread(&s[0], 1, (size_t)len, f);
        (void)readBytes;
    }
    // Restore position for potential further reads
    fseek(f, pos, SEEK_SET);
    return s;
}

// Simple non-terminating assertion helpers (EXPECT_* style)
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[EXPECT_TRUE FAILED] " << msg << " (in " << __FUNCTION__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_EQ_STR(actual, expected, msg) do { \
    if (std::string((actual)) != std::string((expected))) { \
        std::cerr << "[EXPECT_EQ_STR FAILED] " << msg << " | got: \"" << (actual) << "\", expected: \"" << (expected) << "\" (in " << __FUNCTION__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_EQ_INT(actual, expected, msg) do { \
    if ((int)(actual) != (int)(expected)) { \
        std::cerr << "[EXPECT_EQ_INT FAILED] " << msg << " | got: " << (actual) << ", expected: " << (expected) << " (in " << __FUNCTION__ << ")" << std::endl; \
        ++g_failures; \
    } \
} while (0)

// ---------- Test cases for GenerateCSA ----------
// Step 2: Unit Test Generation
// - Test 1: hProfile == NULL path -> GenerateCSA should return without writing anything.
// - Test 2: hProfile valid but first cmsGetPostScriptCSA call returns 0 -> no output.
// - Test 3: hProfile valid and data length > 0 -> output equals MOCK_CSA_STR.

static void test_GenerateCSA_NullProfile(void) {
    // Prepare: configure mock to return NULL for profile
    g_testCase = 1;
    OutFile = tmpfile(); // capture output
    if (OutFile == nullptr) {
        std::cerr << "Failed to create temporary file for OutFile" << std::endl;
        ++g_failures;
        return;
    }

    // Execute
    GenerateCSA(); // focal method, defined in psicc.c

    // Verify: no output written
    fseek(OutFile, 0, SEEK_END);
    long len = ftell(OutFile);
    std::string content = readOutFileContent(OutFile);
    EXPECT_EQ_INT((int)len, 0, "Output length should be 0 when profile is NULL");
    EXPECT_EQ_STR(content, "", "Output content should be empty when profile is NULL");

    fclose(OutFile);
    OutFile = nullptr;
}

static void test_GenerateCSA_ZeroLengthCSA(void) {
    // Prepare: configure mock to return non-NULL profile but 0 length
    g_testCase = 2;
    OutFile = tmpfile();
    if (OutFile == nullptr) {
        std::cerr << "Failed to create temporary file for OutFile" << std::endl;
        ++g_failures;
        return;
    }

    // Execute
    GenerateCSA();

    // Verify: still no output
    fseek(OutFile, 0, SEEK_END);
    long len = ftell(OutFile);
    std::string content = readOutFileContent(OutFile);
    EXPECT_EQ_INT((int)len, 0, "Output length should be 0 when CSA length is 0");
    EXPECT_EQ_STR(content, "", "Output content should be empty when CSA length is 0");

    fclose(OutFile);
    OutFile = nullptr;
}

static void test_GenerateCSA_Success(void) {
    // Prepare: configure mock to return non-NULL profile and non-zero length
    g_testCase = 3;
    OutFile = tmpfile();
    if (OutFile == nullptr) {
        std::cerr << "Failed to create temporary file for OutFile" << std::endl;
        ++g_failures;
        return;
    }

    // Execute
    GenerateCSA();

    // Verify: output equals MOCK_CSA_STR
    fseek(OutFile, 0, SEEK_END);
    long len = ftell(OutFile);
    std::string content = readOutFileContent(OutFile);

    EXPECT_EQ_INT((int)len, (int)MOCK_CSA_LEN, "Output length should equal MOCK_CSA_LEN");
    EXPECT_EQ_STR(content, MOCK_CSA_STR, "Output content should match the mocked CSA string");

    fclose(OutFile);
    OutFile = nullptr;
}

// ---------- Main: Run tests ----------
int main(void) {
    // Step 1-3: Understand and drive tests
    std::cout << "Starting GenerateCSA unit tests (no GTest)..." << std::endl;

    test_GenerateCSA_NullProfile();
    test_GenerateCSA_ZeroLengthCSA();
    test_GenerateCSA_Success();

    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}

// Explanatory notes:
// - The test harness provides minimal C-compatible mocks for the dependencies used by GenerateCSA.
// - It exercises the three critical paths indicated by the instruction: NULL profile, zero-length CSA, and success.
// - OutFile is captured using a temporary file to verify side effects without modifying the production code.
// - The test framework uses non-terminating assertions (EXPECT_* macros) to keep the test suite running after failures.