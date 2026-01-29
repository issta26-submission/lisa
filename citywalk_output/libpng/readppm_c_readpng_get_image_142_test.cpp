// C++11 test suite for the focal function readpng_get_image (from readppm.c)
// No GTest; a lightweight, self-contained test harness with main().
// The tests interact with the C code through its public interface defined in readpng.h.
// Extern "C" is used to ensure correct linkage for C functions when compiled as C++.

#include <vector>
#include <string>
#include <cstdlib>
#include <stdlib.h>
#include <cstdio>
#include <readpng.h>
#include <stdio.h>
#include <cstring>


// Include the C header for the focal function and related globals.
// Use extern "C" to prevent name mangling when compiling as C++.
extern "C" {
}

// Helper to create a temporary binary file with given data and return its path.
static std::string writeTempFile(const unsigned char* data, size_t len) {
    char tmpname[L_tmpnam];
    // Generate a unique temporary name
#if defined(_WIN32)
    // Windows has _tmpnam that may require different handling; tmpnam is portable enough for tests.
    std::tmpnam(tmpname);
#else
    std::tmpnam(tmpname);
#endif
    FILE* f = std::fopen(tmpname, "wb");
    if (!f) return "";
    std::fwrite(data, 1, len, f);
    std::fclose(f);
    return std::string(tmpname);
}

// Simple test harness state
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Convenience: report per-test result
static void report_result(const char* test_name, bool passed) {
    ++g_total_tests;
    if (passed) {
        std::printf("[OK]   %s\n", test_name);
    } else {
        ++g_failed_tests;
        std::printf("[FAILED] %s\n", test_name);
    }
}

// Test 1: Normal case - valid image data is read correctly
// - width=4, height=2, channels=3 -> rowbytes=12, total=24 bytes
// - verify returned pointer, row bytes, channels and data integrity
static bool test_readpng_get_image_normal() {
    // Setup globals expected by readpng_get_image
    width = 4;
    height = 2;
    channels = 3;

    // Prepare test data: 24 bytes with a known pattern
    const size_t total_bytes = static_cast<size_t>(width * height * channels);
    std::vector<unsigned char> file_data(total_bytes);
    for (size_t i = 0; i < total_bytes; ++i) {
        file_data[i] = static_cast<unsigned char>(i & 0xFF);
    }

    // Write to a temporary file and set saved_infile
    std::string tmp_path = writeTempFile(file_data.data(), file_data.size());
    if (tmp_path.empty()) {
        report_result("test_readpng_get_image_normal (setup: temp file)", false);
        return false;
    }
    FILE* f = std::fopen(tmp_path.c_str(), "rb");
    if (!f) {
        report_result("test_readpng_get_image_normal (setup: fopen)", false);
        return false;
    }
    saved_infile = f;

    // Call the focal function
    int pChannelsOut = 0;
    ulg pRowbytesOut = 0;
    uch *image = readpng_get_image(1.0, &pChannelsOut, &pRowbytesOut);

    bool ok = true;
    // Validate outputs
    if (!image) {
        ok = false;
    } else {
        if (pChannelsOut != channels) ok = false;
        if (pRowbytesOut != static_cast<ulg>(width * channels)) ok = false;
        // Check data integrity
        if (std::memcmp(image, file_data.data(), file_data.size()) != 0) ok = false;
        // Free the allocated image_data
        std::free(image);
    }

    // Cleanup: close and remove temp file
    std::fclose(f);
    std::remove(tmp_path.c_str());

    // Optional: ensure internal cleanup state if used
    readpng_cleanup(1);

    report_result("test_readpng_get_image_normal", ok);
    return ok;
}

// Test 2: Overflow guard - height too large should cause NULL return without allocation
// Use maximum safe rowbytes and set height to max + 1
static bool test_readpng_get_image_overflow_guard() {
    // Setup minimal plausible image dimensions to compute a row
    width = 4;
    channels = 3;
    // rowbytes = channels * width
    ulg rowbytes = static_cast<ulg>(channels * width);

    // Compute max height that would not overflow, then set height to max + 1
    size_t max_safe = ((size_t)(-1)) / static_cast<size_t>(rowbytes);
    ulg height_overflow = static_cast<ulg>(static_cast<unsigned long long>(max_safe) + 1ull);

    // Apply overflow scenario
    height = height_overflow;

    // Call the focal function without any file (overflow check happens before file I/O)
    int pChannelsOut = 0;
    ulg pRowbytesOut = 0;
    uch *image = readpng_get_image(1.0, &pChannelsOut, &pRowbytesOut);

    // Expect NULL due to overflow guard
    bool ok = (image == NULL);

    // Cleanup: reset height for subsequent tests
    height = 2;
    readpng_cleanup(1);

    report_result("test_readpng_get_image_overflow_guard", ok);
    return ok;
}

// Test 3: Read error - insufficient data in file triggers NULL return and proper cleanup
// - width=4, height=2, channels=3 (rowbytes=12, total=24 bytes required)
// - provide only a few bytes to trigger fread short-read handling
static bool test_readpng_get_image_read_error() {
    width = 4;
    height = 2;
    channels = 3;

    // Provide only 6 bytes instead of 24
    std::vector<unsigned char> insufficient(6);
    for (size_t i = 0; i < insufficient.size(); ++i) insufficient[i] = static_cast<unsigned char>(i);

    std::string tmp_path = writeTempFile(insufficient.data(), insufficient.size());
    if (tmp_path.empty()) {
        report_result("test_readpng_get_image_read_error (setup: temp file)", false);
        return false;
    }

    FILE* f = std::fopen(tmp_path.c_str(), "rb");
    if (!f) {
        std::remove(tmp_path.c_str());
        report_result("test_readpng_get_image_read_error (setup: fopen)", false);
        return false;
    }
    saved_infile = f;

    int pChannelsOut = 0;
    ulg pRowbytesOut = 0;
    uch *image = readpng_get_image(1.0, &pChannelsOut, &pRowbytesOut);

    bool ok = (image == NULL);

    // Cleanup: close file and remove temp
    std::fclose(f);
    std::remove(tmp_path.c_str());

    // Readpng should not leak memory in failure path; attempt cleanup guard
    readpng_cleanup(1);

    report_result("test_readpng_get_image_read_error", ok);
    return ok;
}

int main() {
    // Run tests
    bool all_ok = true;
    all_ok &= test_readpng_get_image_normal();
    all_ok &= test_readpng_get_image_overflow_guard();
    all_ok &= test_readpng_get_image_read_error();

    // Summary
    if (g_failed_tests == 0) {
        std::printf("All tests passed! (%d tests)\n", g_total_tests);
    } else {
        std::printf("Tests completed with failures: %d/%d\n", g_failed_tests, g_total_tests);
    }

    return g_failed_tests == 0 ? 0 : 1;
}