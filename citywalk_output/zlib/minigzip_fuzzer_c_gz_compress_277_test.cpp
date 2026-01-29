// Unit test suite for gz_compress using real zlib (no Google Test). 
// This test suite exercises the normal path of gz_compress by feeding
// real input data via standard FILE* and writing output to a gzFile opened
// with gzopen. It relies on the project being compilable with C++11 and
// linked against zlib (libz).
//
// The tests focus on functional correctness and basic coverage of the
// method's core I/O loop: reading from input in chunks, writing to output,
// and handling end-of-file and errors in a controlled manner.
//
// Note: We intentionally avoid private members and focus on public behavior.
// Domain knowledge: use only standard library plus provided gz_compress and zlib
// facilities. Tests are non-terminating: failures are logged but do not abort
// the test run, allowing multiple tests to execute in a single run.

#include <cstdio>
#include <cstring>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <cassert>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>


// Forward declaration of focal function. The actual implementation is
// provided in the project (minigzip_fuzzer.c). We assume the function uses
// zlib's gzFile type for the out parameter.
extern "C" int gz_compress(FILE *in, gzFile out);

// Simple testing macros (non-terminating): log failures but continue.
#define LOG(msg) do { std::cerr << "[LOG] " << msg << std::endl; } while(0)
#define FAIL(msg) do { std::cerr << "[FAIL] " << msg << std::endl; } while(0)
#define EXPECT_TRUE(cond) do { if(!(cond)) { FAIL("EXPECT_TRUE failed: " #cond); } } while(0)
#define EXPECT_EQ(a,b) do { if((a)!=(b)) { FAIL("EXPECT_EQ failed: " #a " != " #b ": " + std::to_string(a) + " != " + std::to_string(b)); } } while(0)
#define EXPECT_GE(a,b) do { if((a) < (b)) { FAIL("EXPECT_GE failed: " #a " < " #b); } } while(0)
#define EXPECT_LE(a,b) do { if((a) > (b)) { FAIL("EXPECT_LE failed: " #a " > " #b); } } while(0)

// Helper: create a unique temporary file path
static std::string temp_path(const std::string& prefix) {
    char tmpl[] = "/tmp/tmp_gztest_XXXXXX";
    std::string name = tmpl;
    int fd = mkstemp(tmpl);
    if (fd >= 0) {
        close(fd);
        // Return the populated path; keep the file around for test to use
        return std::string(tmpl);
    } else {
        // Fallback to a simple named file
        static int counter = 0;
        char buf[256];
        snprintf(buf, sizeof(buf), "/tmp/%s_test_%d.bin", prefix.c_str(), counter++);
        return std::string(buf);
    }
}

// Helper: write data to a file path
static bool write_file(const std::string& path, const std::string& data) {
    std::ofstream ofs(path, std::ios::binary);
    if (!ofs) return false;
    ofs.write(data.data(), data.size());
    return true;
}

// Helper: read file size
static size_t file_size(const std::string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) != 0) return 0;
    return static_cast<size_t>(st.st_size);
}

// Test 1: Basic compression of small data
// - Create an input file with known content
// - Run gz_compress against a gzFile opened with gzopen
// - Expect non-zero compressed output size and zero/expected return from gz_compress
static void test_gz_compress_small_input() {
    // Prepare input data
    const std::string input_data = "Hello, gzip compression test!";
    const std::string in_path = temp_path("gz_in_small");
    const std::string out_path = temp_path("gz_out_small.gz");

    // Ensure clean slate
    remove(in_path.c_str());
    remove(out_path.c_str());

    // Write input
    if (!write_file(in_path, input_data)) {
        LOG("Failed to write small input file");
        return;
    }

    // Open input as FILE*
    FILE* in = fopen(in_path.c_str(), "rb");
    if (!in) {
        LOG("Failed to open input file for small test");
        return;
    }

    // Open output gzip file
    gzFile out = gzopen(out_path.c_str(), "wb9");
    if (!out) {
        LOG("Failed to open output gzip file for small test");
        fclose(in);
        return;
    }

    // Call the function under test
    int result = gz_compress(in, out);

    // Do not double-close out here; gz_compress will close if successful.
    // If it returns non-zero, attempt to close to avoid resource leak.
    if (gzclose(out) != Z_OK) {
        // Best-effort close
        LOG("Warning: gzclose on out during small test failed or redundant");
    }

    // Validate
    EXPECT_EQ(result, 0);
    size_t out_size = file_size(out_path);
    EXPECT_TRUE(out_size > 0);
    // Clean up
    remove(in_path.c_str());
    remove(out_path.c_str());
    // Note: input file was closed by gz_compress as per implementation
}

// Test 2: Compression with larger data to ensure multiple reads/write cycles
// - Generate data significantly larger than BUFLEN to force multiple fread calls
// - Validate a non-empty output file is produced
static void test_gz_compress_large_input() {
    // Generate large data
    const size_t data_size = 256 * 1024; // 256 KB
    std::string input_data;
    input_data.assign(data_size, 'A'); // repetitive data

    const std::string in_path = temp_path("gz_in_large");
    const std::string out_path = temp_path("gz_out_large.gz");

    remove(in_path.c_str());
    remove(out_path.c_str());

    if (!write_file(in_path, input_data)) {
        LOG("Failed to write large input file");
        return;
    }

    FILE* in = fopen(in_path.c_str(), "rb");
    if (!in) {
        LOG("Failed to open input file for large test");
        return;
    }

    gzFile out = gzopen(out_path.c_str(), "wb9");
    if (!out) {
        LOG("Failed to open output gzip file for large test");
        fclose(in);
        return;
    }

    int result = gz_compress(in, out);
    if (gzclose(out) != Z_OK) {
        LOG("Warning: gzclose on out during large test failed or redundant");
    }

    EXPECT_EQ(result, 0);
    size_t out_size = file_size(out_path);
    EXPECT_TRUE(out_size > 0);
    // Clean up
    remove(in_path.c_str());
    remove(out_path.c_str());
}

// Test 3: Empty input file should still produce a valid gzip stream or at least not crash
// - The gzip format often emits a header even for empty inputs; we just check that output exists.
static void test_gz_compress_empty_input() {
    const std::string in_path = temp_path("gz_in_empty");
    const std::string out_path = temp_path("gz_out_empty.gz");

    remove(in_path.c_str());
    remove(out_path.c_str());

    // Create empty file
    {
        std::ofstream ofs(in_path, std::ios::binary);
        // nothing written
    }

    FILE* in = fopen(in_path.c_str(), "rb");
    if (!in) {
        LOG("Failed to open input file for empty test");
        return;
    }

    gzFile out = gzopen(out_path.c_str(), "wb9");
    if (!out) {
        LOG("Failed to open output gzip file for empty test");
        fclose(in);
        return;
    }

    int result = gz_compress(in, out);
    if (gzclose(out) != Z_OK) {
        LOG("Warning: gzclose on out during empty test failed or redundant");
    }

    EXPECT_EQ(result, 0);
    size_t out_size = file_size(out_path);
    EXPECT_TRUE(out_size > 0); // likely non-zero due to gzip header
    remove(in_path.c_str());
    remove(out_path.c_str());
}

// Main: run all tests
int main() {
    LOG("Starting gz_compress test suite (C++11, with zlib)");
    test_gz_compress_small_input();
    test_gz_compress_large_input();
    test_gz_compress_empty_input();
    LOG("gz_compress test suite completed");
    return 0;
}