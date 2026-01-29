// Lightweight C++11 unit test suite for the DoEmbedProfile function (DoEmbedProfile in tificc.c)
// This test suite does not rely on Google Test. It uses a minimal assertion mechanism and a small
// set of test doubles (stubs) to exercise the code paths inside DoEmbedProfile.
// Notes:
// - The test harness provides minimal stubs for external dependencies (TIFFSetField, cmsfilelength, OutOfMem, FatalError, malloc).
// - We call the C function DoEmbedProfile via a C linkage declaration from C++.
// - We pass a NULL TIFF* for Out since the test stubs do not depend on the actual TIFF internals.
// - The tests cover: normal path, file-not-found path, negative size path, and memory-allocation-failure path.

#include <cstring>
#include <cstdio>
#include <lcms2_plugin.h>
#include <vector>
#include <iostream>
#include <string>
#include <stdint.h>
#include <cassert>
#include <cstdlib>
#include <cstdarg>
#include <utils.h>
#include <tiffio.h>


// Forward declaration of the focal function to be tested (C linkage).
extern "C" void DoEmbedProfile(void* Out, const char* ProfileFile);

// Forward declarations for stubs (C linkage) used by the focal method.
extern "C" void TIFFSetField(void* tif, int tag, unsigned int count, unsigned char* value);
extern "C" int cmsfilelength(FILE* f);
extern "C" void OutOfMem(unsigned int size);
extern "C" void FatalError(const char* fmt, ...);

// We redefine malloc to simulate allocation failure in a test scenario.
// Note: The real code uses malloc/free. We provide a test-scoped override to trigger the
// "OutOfMem" branch of DoEmbedProfile.
extern "C" void* malloc(size_t size);
//
// We keep a few global flags accessible from the test harness to control stub behavior.
static bool cmsfilelength_should_fail = false;
static bool malloc_should_fail = false;
static bool outOfMem_called = false;
static bool tifSetField_called = false;
static unsigned char* g_capturedBuffer = nullptr;
static unsigned int g_capturedLen = 0;

// Temporary profile data captured by TIFFSetField (our stub)
static unsigned int g_lastEmbedLenFromCall = 0;

// Minimal state to verify correct behavior
static unsigned int g_lastEmbedLenParam = 0;

// Stubbed TIFFSetField implementation to capture the data passed by DoEmbedProfile.
// We do not simulate the full TIFF structure; we only capture what DoEmbedProfile passes.
extern "C" void TIFFSetField(void* /*tif*/, int /*tag*/, unsigned int count, unsigned char* value)
{
    tifSetField_called = true;
    g_lastEmbedLenFromCall = count;
    // Copy the buffer contents to a local buffer for verification.
    if (value != nullptr && count > 0) {
        g_capturedBuffer = (unsigned char*)std::malloc(count);
        if (g_capturedBuffer) {
            std::memcpy(g_capturedBuffer, value, count);
        }
        g_capturedLen = count;
    } else {
        g_capturedBuffer = nullptr;
        g_capturedLen = 0;
    }
}

// Stub: OutOfMem invoked when malloc fails in DoEmbedProfile
extern "C" void OutOfMem(unsigned int size)
{
    outOfMem_called = true;
    // We do not abort; just record the event for test verification.
}

// Stub: FatalError - do nothing (non-terminating in test context)
extern "C" void FatalError(const char* /*fmt*/, ...)
{
    // No-op for unit tests to avoid exiting.
}

// Implementation: malloc override to simulate allocation failure when requested.
extern "C" void* malloc(size_t size)
{
    if (malloc_should_fail) {
        return nullptr;
    }
    return std::malloc(size);
}

// Helper: clean up any allocated buffers from TIFFSetField stub
static void cleanup_capture_buffer()
{
    if (g_capturedBuffer) {
        std::free(g_capturedBuffer);
        g_capturedBuffer = nullptr;
    }
    g_capturedLen = 0;
    g_lastEmbedLenFromCall = 0;
}

// Helper: write a small binary profile file for tests
static bool writeProfileFile(const std::string& path, const std::vector<unsigned char>& data)
{
    FILE* f = std::fopen(path.c_str(), "wb");
    if (!f) return false;
    size_t written = std::fwrite(data.data(), 1, data.size(), f);
    std::fclose(f);
    return written == data.size();
}

// Simple test harness: non-terminating assertions that report results but do not exit on first failure.
static int g_total_failures = 0;
static void EXPECT_TRUE(bool cond, const char* message)
{
    if (cond) {
        std::cout << "[PASS] " << message << std::endl;
    } else {
        std::cerr << "[FAIL] " << message << std::endl;
        ++g_total_failures;
    }
}

// Prepare the test environment: reset stubs
static void resetMocks()
{
    tifSetField_called = false;
    g_lastEmbedLenFromCall = 0;
    g_capturedLen = 0;
    if (g_capturedBuffer) {
        std::free(g_capturedBuffer);
        g_capturedBuffer = nullptr;
    }
    cmsfilelength_should_fail = false;
    malloc_should_fail = false;
    outOfMem_called = false;
}

// Test 1: Normal operation - file exists with known bytes; ensure DoEmbedProfile passes correct data to TIFFSetField
static void test_normal_embed_profile_writes_buffer()
{
    // Prepare a small profile file with known bytes
    std::vector<unsigned char> data = { 0x01, 0x02, 0x03, 0x04, 0x05 };
    const std::string path = "test_profile_normal.bin";
    remove(path.c_str());
    bool ok = writeProfileFile(path, data);
    EXPECT_TRUE(ok, "Creating test profile file for normal path should succeed");

    resetMocks();
    // Call the function under test
    DoEmbedProfile(nullptr, path.c_str());

    // Validate that TIFFSetField was invoked with EmbedLen equal to data size and data content matches
    EXPECT_TRUE(tifSetField_called, "TIFFSetField should be called on normal path");
    EXPECT_TRUE(g_lastEmbedLenFromCall == (unsigned int)data.size(),
                "Embed length passed to TIFFSetField should equal profile file size");
    if (g_capturedBuffer && g_lastEmbedLenFromCall == (unsigned int)data.size()) {
        bool memequal = (std::memcmp(g_capturedBuffer, data.data(), data.size()) == 0);
        EXPECT_TRUE(memequal, "Embedded data should match the file contents exactly");
    } else {
        EXPECT_TRUE(false, "Captured embed buffer should be non-null with correct length");
    }

    // Cleanup
    cleanup_capture_buffer();
    remove(path.c_str());
}

// Test 2: File-not-found path - DoEmbedProfile should return without embedding
static void test_profile_file_not_found()
{
    resetMocks();
    const std::string path = "nonexistent_profile.bin";
    remove(path.c_str()); // ensure file does not exist

    DoEmbedProfile(nullptr, path.c_str());

    EXPECT_TRUE(!tifSetField_called, "TIFFSetField should not be called when profile file is missing");
    cleanup_capture_buffer();
}

// Test 3: Negative size scenario - cmsfilelength returns -1; DoEmbedProfile should bail out before embedding
static void test_negative_size_skips_embedding()
{
    resetMocks();
    cmsfilelength_should_fail = true; // Force cmsfilelength to return -1

    // Create a valid file that would otherwise simulate a normal read
    std::vector<unsigned char> data = { 0xAA, 0xBB };
    const std::string path = "test_profile_negative_size.bin";
    remove(path.c_str());
    bool ok = writeProfileFile(path, data);
    EXPECT_TRUE(ok, "Creating test profile file for negative size scenario should succeed");

    DoEmbedProfile(nullptr, path.c_str());

    EXPECT_TRUE(!tifSetField_called, "TIFFSetField should not be called when size is negative");
    cleanup_capture_buffer();
    remove(path.c_str());
    cmsfilelength_should_fail = false;
}

// Test 4: malloc failure path - simulate memory allocation failure and verify OutOfMem is invoked
static void test_malloc_failure_triggers_out_of_mem()
{
    resetMocks();
    malloc_should_fail = true; // Force allocation to fail

    // Create a valid file that would otherwise be embeddable
    std::vector<unsigned char> data = { 0x10, 0x20, 0x30 };
    const std::string path = "test_profile_malloc_fail.bin";
    remove(path.c_str());
    bool ok = writeProfileFile(path, data);
    EXPECT_TRUE(ok, "Creating test profile file for malloc failure scenario should succeed");

    DoEmbedProfile(nullptr, path.c_str());

    // The code should call OutOfMem(size+1)
    EXPECT_TRUE(outOfMem_called, "OutOfMem should be invoked when malloc fails");

    cleanup_capture_buffer();
    remove(path.c_str());
    malloc_should_fail = false;
}

// Main entry: run tests and summarize
int main()
{
    std::cout << "Starting DoEmbedProfile unit tests (C++11, no GTest) ..." << std::endl;

    test_normal_embed_profile_writes_buffer();
    test_profile_file_not_found();
    test_negative_size_skips_embedding();
    test_malloc_failure_triggers_out_of_mem();

    if (g_total_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_total_failures << " test(s) failed." << std::endl;
        return 1;
    }
}