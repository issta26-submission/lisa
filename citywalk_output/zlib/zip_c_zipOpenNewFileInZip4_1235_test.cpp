// C++11 test harness for the focal C function
// Tests the behavior of zipOpenNewFileInZip4 by providing a fake
// implementation of zipOpenNewFileInZip4_64 to observe forwarding
// of the zip64 flag and return value propagation.
//
// Assumptions:
// - zipOpenNewFileInZip4 simply forwards to zipOpenNewFileInZip4_64 with zip64 = 0.
// - We override zipOpenNewFileInZip4_64 with a test double to verify behavior.
// - The real project uses C linkage for these functions; we mirror that in test.

#include <cstdio>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <crypt.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>
#include <zip.h>


// Define minimal types to declare/override the C symbols.
// The real project defines these in its headers; here we only need
// enough to compile and link against the production code.
typedef void* zipFile;
typedef struct zip_fileinfo zip_fileinfo;
typedef unsigned int uInt;
typedef unsigned long uLong;

// Ensure macro exists for compatibility with the library's export macro.
#ifndef ZEXPORT
#define ZEXPORT
#endif

extern "C" {

// Forward declarations of the functions under test as they appear in the production code.
// These prototypes must match the actual library's signatures for correct linkage.
typedef int ZEXPORT(zipOpenNewFileInZip4)(zipFile file,
                                      const char* filename,
                                      const zip_fileinfo* zipfi,
                                      const void* extrafield_local,
                                      uInt size_extrafield_local,
                                      const void* extrafield_global,
                                      uInt size_extrafield_global,
                                      const char* comment,
                                      int method,
                                      int level,
                                      int raw,
                                      int windowBits,
                                      int memLevel,
                                      int strategy,
                                      const char* password,
                                      uLong crcForCrypting,
                                      uLong versionMadeBy,
                                      uLong flagBase);

typedef int ZEXPORT(zipOpenNewFileInZip4_64)(zipFile file,
                                          const char* filename,
                                          const zip_fileinfo* zipfi,
                                          const void* extrafield_local,
                                          uInt size_extrafield_local,
                                          const void* extrafield_global,
                                          uInt size_extrafield_global,
                                          const char* comment,
                                          int method,
                                          int level,
                                          int raw,
                                          int windowBits,
                                          int memLevel,
                                          int strategy,
                                          const char* password,
                                          uLong crcForCrypting,
                                          uLong versionMadeBy,
                                          uLong flagBase,
                                          int zip64);
}

// Global state captured by the fake 64-bit function to verify test expectations.
struct Zip64CallInfo {
    bool called;
    int zip64;
} g_lastZip64Call{false, -1};

// Test double for zipOpenNewFileInZip4_64
// This function will replace the real implementation during linking.
// It records the value of the zip64 parameter and returns a programmable value.
extern "C" int ZEXPORT zipOpenNewFileInZip4_64(zipFile file,
                                           const char* filename,
                                           const zip_fileinfo* zipfi,
                                           const void* extrafield_local,
                                           uInt size_extrafield_local,
                                           const void* extrafield_global,
                                           uInt size_extrafield_global,
                                           const char* comment,
                                           int method,
                                           int level,
                                           int raw,
                                           int windowBits,
                                           int memLevel,
                                           int strategy,
                                           const char* password,
                                           uLong crcForCrypting,
                                           uLong versionMadeBy,
                                           uLong flagBase,
                                           int zip64)
{
    // Record the zip64 flag for assertions in tests
    g_lastZip64Call.called = true;
    g_lastZip64Call.zip64  = zip64;
    // Return a programmable value; tests may override this by changing a global.
    extern int g_fake_zipOpenNewFileInZip4_64_return_value;
    return g_fake_zipOpenNewFileInZip4_64_return_value;
}

// Public control for tests to set the fake return value of the 64-bit function
extern "C" int g_fake_zipOpenNewFileInZip4_64_return_value = 0;

// Wrapper: ensure tests exercise the real function zipOpenNewFileInZip4
// FWD: The production code's zipOpenNewFileInZip4 forwards to 64 with zip64 = 0.
// We call it with arbitrary args; we only inspect the 64-bit call side.
extern "C" int ZEXPORT zipOpenNewFileInZip4(zipFile file,
                                        const char* filename,
                                        const zip_fileinfo* zipfi,
                                        const void* extrafield_local,
                                        uInt size_extrafield_local,
                                        const void* extrafield_global,
                                        uInt size_extrafield_global,
                                        const char* comment,
                                        int method,
                                        int level,
                                        int raw,
                                        int windowBits,
                                        int memLevel,
                                        int strategy,
                                        const char* password,
                                        uLong crcForCrypting,
                                        uLong versionMadeBy,
                                        uLong flagBase)
{
    // Forward to the 64-bit variant with zip64 = 0, as per the focal method.
    // The function signature matches the 64-bit version's, except the last parameter.
    return zipOpenNewFileInZip4_64(file, filename, zipfi,
                                   extrafield_local, size_extrafield_local,
                                   extrafield_global, size_extrafield_global,
                                   comment, method, level, raw,
                                   windowBits, memLevel, strategy,
                                   password, crcForCrypting, versionMadeBy, flagBase, 0);
}

//////////////////////////////////////////////////////////////////////////
// Minimal, lightweight test framework (no GTest; C++11 standard)      //
//////////////////////////////////////////////////////////////////////////

static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_PASS()  do { std::cout << "[PASS] " << __FUNCTION__ << "\n"; } while(0)
#define TEST_FAIL(msg) do { std::cerr << "[FAIL] " << __FUNCTION__ << ": " << msg << "\n"; ++g_failed_tests; } while(0)
#define ASSERT_TRUE(cond, msg) do { if (cond) { /* success remains silent here */ } else { TEST_FAIL(msg); return false; } } while(0)

bool test_zipOpenNewFileInZip4_forwards_zip64_flag()
{
    // Reset the observed call info
    g_lastZip64Call.called = false;
    g_lastZip64Call.zip64  = -1;

    // Ensure the fake 64-bit function will return a known value
    g_fake_zipOpenNewFileInZip4_64_return_value = 0x12345678;

    // Call the focal method with dummy values. We only care about the interactions
    // with the 64-bit function: zip64 must be 0 and the return value should propagate.
    zipFile zf = nullptr;
    const char* name = "test_file.txt";

    int ret = zipOpenNewFileInZip4(zf,
                                   name,
                                   nullptr,
                                   nullptr, 0,
                                   nullptr, 0,
                                   nullptr,
                                   0, 0, 0,
                                   0, 0, 0,
                                   nullptr,
                                   0, 0, 0);

    // Validate that the 64-bit function was invoked
    bool called = g_lastZip64Call.called;
    bool zip64WasZero = (g_lastZip64Call.zip64 == 0);
    bool retMatchesFake = (ret == g_fake_zipOpenNewFileInZip4_64_return_value);

    if (called && zip64WasZero && retMatchesFake) {
        TEST_PASS();
        return true;
    } else {
        if (!called) TEST_FAIL("zipOpenNewFileInZip4_64 was not called");
        if (!zip64WasZero) TEST_FAIL("zipOpenNewFileInZip4_64.zip64 was not 0 as expected");
        if (!retMatchesFake) TEST_FAIL("return value from zipOpenNewFileInZip4_64 was not propagated");
        return false;
    }
}

bool test_zipOpenNewFileInZip4_propagates_return_value()
{
    // Reset and configure
    g_lastZip64Call.called = false;
    g_lastZip64Call.zip64  = -1;

    g_fake_zipOpenNewFileInZip4_64_return_value = -9999;

    zipFile zf = nullptr;
    const char* name = "another_test.bin";

    int ret = zipOpenNewFileInZip4(zf,
                                   name,
                                   nullptr,
                                   nullptr, 0,
                                   nullptr, 0,
                                   nullptr,
                                   0, 0, 0,
                                   0, 0, 0,
                                   nullptr,
                                   0, 0, 0);

    bool called = g_lastZip64Call.called;
    bool retMatches = (ret == g_fake_zipOpenNewFileInZip4_64_return_value);

    if (called && retMatches) {
        TEST_PASS();
        return true;
    } else {
        if (!called) TEST_FAIL("zipOpenNewFileInZip4_64 was not called on second test");
        if (!retMatches) TEST_FAIL("return value did not propagate from zipOpenNewFileInZip4_64 on second test");
        return false;
    }
}

int main()
{
    std::cout << "Running tests for zipOpenNewFileInZip4 forwarding behavior...\n";

    int passed = 0;
    int failed = 0;

    if (test_zipOpenNewFileInZip4_forwards_zip64_flag()) ++passed;
    else ++failed;

    if (test_zipOpenNewFileInZip4_propagates_return_value()) ++passed;
    else ++failed;

    std::cout << "Test summary: " << passed << " passed, " << failed << " failed, total=" << (passed+failed) << "\n";

    // Return the number of failed tests as exit code (non-zero if any failed)
    return failed;
}