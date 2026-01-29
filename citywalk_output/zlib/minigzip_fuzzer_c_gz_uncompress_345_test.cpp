// Test suite for gz_uncompress function (minigzip_fuzzer.c) using a small custom test harness.
// This test is written for C++11 and does not rely on GoogleTest.

// We compile this test together with the minigzip_fuzzer.c source to ensure access to gz_uncompress
// and its dependencies (zlib). The test creates a small gzip input using zlib's API and verifies
// that gz_uncompress produces the original data in a file detector.

// Important: This test uses a minimal non-terminating assertion style (returning bool from tests)
// to maximize code execution and coverage without terminating on first failure.

#define BUFLENW 4096 // Ensure BUFLENW is defined for the included focal method.
#include <cstdio>
#include <cstring>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <cassert>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <minigzip_fuzzer.c>
#include <zlib.h>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <cstdlib>



// Include the focal method file to ensure gz_uncompress is linked in this translation unit.
// We assume minigzip_fuzzer.c can be included safely. BUFLENW is defined above to satisfy
// internal usage inside gz_uncompress.

// Simple test harness (non-GTest): non-terminating assertions via boolean returns.

static int tests_run = 0;
static int tests_failed = 0;

// Helper assertion: non-terminating style
static bool assert_true(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "Assertion failed: " << msg << std::endl;
        return false;
    }
    return true;
}

// Helper: compress a string into a gzip file using zlib (gzopen/gzwrite/gzclose)
static bool gzip_compress_string(const std::string& data, const std::string& gz_path) {
    gzFile gzf = gzopen(gz_path.c_str(), "wb");
    if (!gzf) {
        std::cerr << "Failed to open gzip for writing: " << gz_path << std::endl;
        return false;
    }
    int written = 0;
    if (!data.empty()) {
        written = gzwrite(gzf, data.data(), static_cast<unsigned int>(data.size()));
        if (written != static_cast<int>(data.size())) {
            std::cerr << "gzwrite wrote incorrect number of bytes" << std::endl;
            gzclose(gzf);
            return false;
        }
    }
    int close_res = gzclose(gzf);
    if (close_res != Z_OK) {
        std::cerr << "gzclose failed with code: " << close_res << std::endl;
        return false;
    }
    return true;
}

// Test 1: Basic round-trip decompression
// - Compress a known string into a gzip file.
// - Use gz_uncompress to decompress into an output file.
// - Read back and compare with the original string.
// This covers the main success path of gz_uncompress (len > 0, successful fwrite, fclose, gzclose).
static bool test_gz_uncompress_basic_roundtrip() {
    const std::string original = "The quick brown fox jumps over the lazy dog. 0123456789";
    const char* gz_path = "tmp_basic_roundtrip.gz";
    const char* out_path = "tmp_basic_roundtrip_out.txt";

    // Ensure clean slate
    std::remove(gz_path);
    std::remove(out_path);

    // Step 1: Create gzip input
    if (!gzip_compress_string(original, gz_path)) {
        std::cerr << "Test 1: Failed to create gzip input" << std::endl;
        return false;
    }

    // Step 2: Decompress using gz_uncompress (the focal method under test)
    gzFile in = gzopen(gz_path, "rb");
    if (!in) {
        std::cerr << "Test 1: Failed to open gzip input for reading" << std::endl;
        std::remove(gz_path);
        return false;
    }
    FILE* out = fopen(out_path, "wb");
    if (!out) {
        std::cerr << "Test 1: Failed to open output file for writing" << std::endl;
        gzclose(in);
        std::remove(gz_path);
        return false;
    }

    // Call the focal method
    gz_uncompress(in, out);

    // Cleanup input/output handles
    gzclose(in);
    fclose(out);

    // Step 3: Validate output content matches original
    std::ifstream fin(out_path, std::ios::binary);
    std::string decompressed((std::istreambuf_iterator<char>(fin)),
                           std::istreambuf_iterator<char>());
    fin.close();

    bool ok = (decompressed == original);
    if (!ok) {
        std::cerr << "Test 1: Decompressed data does not match original" << std::endl;
    }

    // Cleanup
    std::remove(gz_path);
    std::remove(out_path);
    return ok;
}

// Test 2: Decompress an empty gzip input (zero-length data)
// - Create an empty gzip file (header/trailer with no payload) and decompress.
// - Expect empty output file content.
static bool test_gz_uncompress_empty_input() {
    const std::string original = ""; // empty
    const char* gz_path = "tmp_empty_input.gz";
    const char* out_path = "tmp_empty_input_out.txt";

    std::remove(gz_path);
    std::remove(out_path);

    // Create an empty gzip file by opening and closing without writing payload
    {
        gzFile gzf = gzopen(gz_path, "wb");
        if (!gzf) {
            std::cerr << "Test 2: Failed to create empty gzip file" << std::endl;
            return false;
        }
        // Do not write any payload; just close
        gzclose(gzf);
    }

    // Decompress using gz_uncompress
    gzFile in = gzopen(gz_path, "rb");
    if (!in) {
        std::cerr << "Test 2: Failed to open empty gzip input for reading" << std::endl;
        std::remove(gz_path);
        return false;
    }
    FILE* out = fopen(out_path, "wb");
    if (!out) {
        std::cerr << "Test 2: Failed to open output file for writing" << std::endl;
        gzclose(in);
        std::remove(gz_path);
        return false;
    }

    gz_uncompress(in, out);

    gzclose(in);
    fclose(out);

    // Validate output is empty
    std::ifstream fin(out_path, std::ios::binary);
    std::string decompressed((std::istreambuf_iterator<char>(fin)),
                           std::istreambuf_iterator<char>());
    fin.close();

    bool ok = decompressed.empty();
    if (!ok) {
        std::cerr << "Test 2: Expected empty output for empty input, got length " << decompressed.size() << std::endl;
    }

    // Cleanup
    std::remove(gz_path);
    std::remove(out_path);
    return ok;
}

// Helper: Run a single test and report result
static void run_test(bool (*test_func)(), const char* name) {
    ++tests_run;
    bool result = test_func();
    if (!result) {
        ++tests_failed;
        std::cerr << "[FAILED] " << name << std::endl;
    } else {
        std::cout << "[PASSED] " << name << std::endl;
    }
}

// Main entry: execute all tests
int main() {
    // Run tests
    run_test(test_gz_uncompress_basic_roundtrip, "gz_uncompress_basic_roundtrip");
    run_test(test_gz_uncompress_empty_input, "gz_uncompress_empty_input");

    // Summary
    std::cout << "Tests run: " << tests_run << ", Passed: " 
              << (tests_run - tests_failed) << ", Failed: " << tests_failed << std::endl;

    // Return non-zero if any test failed
    return (tests_failed == 0) ? 0 : 1;
}