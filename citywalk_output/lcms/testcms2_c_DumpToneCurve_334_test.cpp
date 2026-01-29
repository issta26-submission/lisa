// Tiny C++11 test suite for the DumpToneCurve function.
// This test does not rely on Google Test or any external test framework.
// It uses a lightweight test harness with non-terminating assertions to
// maximize code coverage when running in environments without a test runner.
//
// Notes:
// - The test relies on the project providing the LittleCMS types and the
//   external functions used by DumpToneCurve (cmsIT8Alloc, cmsIT8SaveToFile, etc.).
// - We exercise two branches of the DumpToneCurve loop by providing 3 entries and 0 entries.
// - We verify that a file is created and that it is non-empty as a basic sanity check.
// - All tests are contained in a single file to satisfy environments that compile
//   tests as standalone units integrated with the focal code base.
//
// Assumptions based on the provided focal code:
// - cmsToneCurve has fields: InterpParams (pointer to cmsInterpParams), nEntries (cmsUInt32Number), Table16 (cmsUInt16Number*).
// - cmsInterpParams has a field ContextID (cmsContext / void*).
// - DumpToneCurve has the signature: void DumpToneCurve(cmsToneCurve* gamma, const char* FileName).
// - The project provides the relevant LittleCMS-style types and the cmsIT8* API.

#include <cstring>
#include <cstdio>
#include <fstream>
#include <vector>
#include <iostream>
#include <testcms2.h>
#include <cstdlib>



extern "C" void DumpToneCurve(cmsToneCurve* gamma, const char* FileName);

// Lightweight test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if (!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "Test failed: " << (msg) << std::endl; \
    } \
} while(0)

#define EXPECT_FALSE(cond, msg) EXPECT_TRUE(!(cond), msg)

static bool FileExistsAndNotEmpty(const char* path) {
    std::ifstream f(path, std::ios::binary | std::ios::ate);
    if (!f.good()) return false;
    std::streampos sz = f.tellg();
    f.close();
    return sz > 0;
}

// Helper to build a gamma structure compatible with the DumpToneCurve expectations
static cmsToneCurve* BuildGamma(int nEntries, const std::vector<cmsUInt16Number>& values) {
    // Allocate the gamma structure
    cmsToneCurve* gamma = new cmsToneCurve;
    // Initialize InterpParams with a dummy context id. The actual value is not validated here.
    gamma->InterpParams = new cmsInterpParams;
    gamma->InterpParams->ContextID = reinterpret_cast<void*>(0x1234); // non-null dummy context

    gamma->nEntries = static_cast<cmsUInt32Number>(nEntries);

    if (nEntries > 0) {
        gamma->Table16 = new cmsUInt16Number[nEntries];
        for (int i = 0; i < nEntries && i < static_cast<int>(values.size()); ++i) {
            gamma->Table16[i] = values[i];
        }
    } else {
        gamma->Table16 = nullptr;
    }

    return gamma;
}

static void CleanupGamma(cmsToneCurve* gamma) {
    if (!gamma) return;
    if (gamma->InterpParams) {
        delete gamma->InterpParams;
    }
    delete[] gamma->Table16;
    delete gamma;
}

// Test 1: Basic sanity with a small gamma (nEntries > 0)
static void Test_DumpToneCurve_BasicSmallGamma(void) {
    // Arrange
    std::vector<cmsUInt16Number> vals = { 0x0000, 0x0123, 0xABCD };
    cmsToneCurve* gamma = BuildGamma(3, vals);
    const char* fname = "test_dumptone_basic.it8";

    // Act
    DumpToneCurve(gamma, fname);

    // Assert: file exists and is non-empty
    bool fileOk = FileExistsAndNotEmpty(fname);

    // Cleanup
    CleanupGamma(gamma);
    std::remove(fname);

    // Report
    EXPECT_TRUE(fileOk, "DumpToneCurve should create a non-empty IT8 file for a small gamma (nEntries=3).");
}

// Test 2: Edge case with zero entries (should still create a file, but loop is skipped)
static void Test_DumpToneCurve_ZeroEntries(void) {
    // Arrange
    cmsToneCurve* gamma = BuildGamma(0, std::vector<cmsUInt16Number>{});
    const char* fname = "test_dumptone_zero.it8";

    // Act
    DumpToneCurve(gamma, fname);

    // Assert: file exists and is non-empty (depending on the library, this could be small or non-zero)
    bool fileOk = FileExistsAndNotEmpty(fname);

    // Cleanup
    CleanupGamma(gamma);
    std::remove(fname);

    // Report
    EXPECT_TRUE(fileOk, "DumpToneCurve should handle nEntries=0 without crashing and produce a (non-empty) IT8 file.");
}

// Test 3: Large gamma to exercise more iterations (ensures loop coverage for higher counts)
static void Test_DumpToneCurve_LargerGamma(void) {
    // Arrange
    std::vector<cmsUInt16Number> vals;
    const int N = 10;
    for (int i = 0; i < N; ++i) {
        vals.push_back(static_cast<cmsUInt16Number>(i * 0x1111));
    }
    cmsToneCurve* gamma = BuildGamma(N, vals);
    const char* fname = "test_dumptone_large.it8";

    // Act
    DumpToneCurve(gamma, fname);

    // Assert: file exists and is non-empty
    bool fileOk = FileExistsAndNotEmpty(fname);

    // Cleanup
    CleanupGamma(gamma);
    std::remove(fname);

    // Report
    EXPECT_TRUE(fileOk, "DumpToneCurve should handle larger nEntries (e.g., 10) and produce a valid IT8 file.");
}

// Runner
static void RunAllTests(void) {
    Test_DumpToneCurve_BasicSmallGamma();
    Test_DumpToneCurve_ZeroEntries();
    Test_DumpToneCurve_LargeGamma();

    // Summary
    std::cout << "Total tests run: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;
}

int main() {
    // Run the tests and return non-zero if any failed
    RunAllTests();
    return g_failed_tests ? 1 : 0;
}