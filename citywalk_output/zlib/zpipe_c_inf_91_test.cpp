// C++11 test suite for the focal method: inf(FILE *source, FILE *dest) in zpipe.c
// Notes:
// - This test suite uses a pure C++ test harness (no Google Test).
// - It links against the existing zpipe.c implementation (which provides inf).
// - It relies on zlib being available (link with -lz).
// - CHUNK is defined in the zpipe.c; for test compilation we also define CHUNK if needed.

#include <cstdio>
#include <cstring>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <fstream>
#include <unistd.h>
#include <cstdlib>


extern "C" int inf(FILE *source, FILE *dest); // Focal method under test (C linkage)

#ifndef CHUNK
#define CHUNK 16384
#endif

// Simple test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;
static std::vector<std::string> g_failures;

// Helpers: lightweight assertion/macros
#define EXPECT_EQ(actual, expected, msg) do { \
    ++g_tests_run; \
    if ((actual) != (expected)) { \
        ++g_tests_failed; \
        g_failures.push_back(std::string("FAIL: ") + (msg) + "\n  Actual: " + std::to_string((long long)(actual)) + "\n  Expected: " + std::to_string((long long)(expected))); \
    } \
} while (0)

#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        ++g_tests_failed; \
        g_failures.push_back(std::string("FAIL: ") + (msg)); \
    } \
} while (0)

// Utility: write a string to a binary file
static bool writeToFile(const std::string& path, const std::string& data) {
    FILE* f = std::fopen(path.c_str(), "wb");
    if (!f) return false;
    size_t written = std::fwrite(data.data(), 1, data.size(), f);
    std::fclose(f);
    return written == data.size();
}

// Utility: read entire file into string
static std::string readFromFile(const std::string& path) {
    std::string result;
    FILE* f = std::fopen(path.c_str(), "rb");
    if (!f) return result;
    std::fseek(f, 0, SEEK_END);
    long sz = std::ftell(f);
    if (sz > 0) {
        result.resize(static_cast<size_t>(sz));
        std::fseek(f, 0, SEEK_SET);
        std::fread(&result[0], 1, result.size(), f);
    }
    std::fclose(f);
    return result;
}

// Utility: compress a string to a file using zlib (deflate)
static bool compressToFile(const std::string& input, const std::string& filepath) {
    // Use zlib deflate to create a compressed representation
    // Return true on success
    #include <zlib.h>
    FILE* f = std::fopen(filepath.c_str(), "wb");
    if (!f) return false;
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));

    int ret = deflateInit(&strm, Z_DEFAULT_COMPRESSION);
    if (ret != Z_OK) {
        std::fclose(f);
        return false;
    }

    strm.next_in = reinterpret_cast<Bytef*>(const_cast<char*>(input.data()));
    strm.avail_in = static_cast<uInt>(input.size());

    unsigned char out[CHUNK];

    // Compress until input exhausted
    int deflate_ret;
    do {
        strm.next_out = out;
        strm.avail_out = CHUNK;
        deflate_ret = deflate(&strm, (strm.avail_in == 0) ? Z_FINISH : Z_NO_FLUSH);
        if (deflate_ret == Z_STREAM_ERROR) {
            deflateEnd(&strm);
            std::fclose(f);
            return false;
        }
        size_t have = CHUNK - strm.avail_out;
        if (have > 0) {
            if (std::fwrite(out, 1, have, f) != have) {
                deflateEnd(&strm);
                std::fclose(f);
                return false;
            }
        }
    } while (deflate_ret != Z_STREAM_END);

    deflateEnd(&strm);
    std::fclose(f);
    return true;
}

// Utility: create a large string of repeated chars
static std::string makeLargeBuffer(size_t size, char c) {
    return std::string(size, c);
}

// Test 1: Basic decompression path with small data
static void test_inf_basic_decompression() {
    // Prepare original data
    const std::string original = "The quick brown fox jumps over the lazy dog";

    // Prepare source/compressed file
    const std::string srcPath = "inf_test_src_basic.bin";
    const std::string dstPath = "inf_test_dst_basic.bin";

    // Ensure clean slate
    remove(srcPath.c_str());
    remove(dstPath.c_str());

    bool ok = compressToFile(original, srcPath);
    EXPECT_TRUE(ok, "Compression for basic test should succeed");
    if (!ok) return;

    // Run inf
    FILE* src = std::fopen(srcPath.c_str(), "rb");
    FILE* dst = std::fopen(dstPath.c_str(), "wb");
    int ret = Z_ERRNO;
    if (src && dst) {
        ret = inf(src, dst);
        std::fclose(src);
        std::fclose(dst);
    } else {
        if (src) std::fclose(src);
        if (dst) std::fclose(dst);
    }

    // Expect successful return
    EXPECT_EQ(ret, Z_OK, "inf should return Z_OK on successful decompression");

    // Verify decompressed content
    std::string decompressed = readFromFile(dstPath);
    EXPECT_EQ(decompressed, original, "Decompressed data should match original");

    // Cleanup
    remove(srcPath.c_str());
    remove(dstPath.c_str());
}

// Test 2: Empty input should yield Z_DATA_ERROR
static void test_inf_empty_input() {
    const std::string srcPath = "inf_test_src_empty.bin";
    const std::string dstPath = "inf_test_dst_empty.bin";

    // Create an empty source file
    {
        std::ofstream ofs(srcPath, std::ios::binary);
        ofs.close();
    }

    FILE* src = std::fopen(srcPath.c_str(), "rb");
    FILE* dst = std::fopen(dstPath.c_str(), "wb");
    int ret = Z_ERRNO;
    if (src && dst) {
        ret = inf(src, dst);
        std::fclose(src);
        std::fclose(dst);
    }

    // Expect Z_DATA_ERROR per the implementation
    EXPECT_EQ(ret, Z_DATA_ERROR, "inf should return Z_DATA_ERROR for empty input");

    remove(srcPath.c_str());
    remove(dstPath.c_str());
}

// Test 3: Corrupted input (truncate the compressed data) should yield Z_DATA_ERROR
static void test_inf_corrupted_input() {
    const std::string original = "Corruption test data for inf";

    const std::string srcPath = "inf_test_src_corrupt.bin";
    const std::string dstPath = "inf_test_dst_corrupt.bin";

    // Compress original data
    bool ok = compressToFile(original, srcPath);
    EXPECT_TRUE(ok, "Compression for corrupted-input test should succeed");
    if (!ok) return;

    // Load compressed data and truncate last byte to simulate corruption
    std::string data = readFromFile(srcPath);
    if (!data.empty()) {
        data.pop_back(); // remove last byte
        writeToFile(srcPath, data);
    } else {
        // If somehow empty, this test isn't meaningful
        EXPECT_TRUE(false, "Corrupted data generation failed (empty after compress)");
        remove(srcPath.c_str());
        remove(dstPath.c_str());
        return;
    }

    FILE* src = std::fopen(srcPath.c_str(), "rb");
    FILE* dst = std::fopen(dstPath.c_str(), "wb");
    int ret = Z_ERRNO;
    if (src && dst) {
        ret = inf(src, dst);
        std::fclose(src);
        std::fclose(dst);
    }

    // Expect Z_DATA_ERROR due to corrupted input
    EXPECT_EQ(ret, Z_DATA_ERROR, "inf should return Z_DATA_ERROR for corrupted input");

    remove(srcPath.c_str());
    remove(dstPath.c_str());
}

// Test 4: Destination write failure should yield Z_ERRNO
static void test_inf_destination_write_error() {
    const std::string original = "Write-error test data";

    const std::string srcPath = "inf_test_src_writeerror.bin";
    const std::string dstPath = "inf_test_dst_writeerror.bin";

    bool ok = compressToFile(original, srcPath);
    EXPECT_TRUE(ok, "Compression for write-error test should succeed");
    if (!ok) return;

    // Open destination in read-only mode to force write failure
    FILE* src = std::fopen(srcPath.c_str(), "rb");
    FILE* dst = std::fopen(dstPath.c_str(), "rb"); // read-only to force write error
    int ret = Z_ERRNO;
    if (src && dst) {
        ret = inf(src, dst);
        std::fclose(src);
        std::fclose(dst);
        // Ensure we cleanup created file
        remove(dstPath.c_str());
    } else {
        if (src) std::fclose(src);
        if (dst) std::fclose(dst);
    }

    // Expect Z_ERRNO due to write error
    EXPECT_EQ(ret, Z_ERRNO, "inf should return Z_ERRNO when destination write fails");

    remove(srcPath.c_str());
    remove(dstPath.c_str());
}

// Test 5: Large data to exercise chunked processing and ensure correctness
static void test_inf_large_data() {
    // Build large input (e.g., 100 KB)
    std::string largeInput = makeLargeBuffer(100 * 1024, 'A');

    const std::string srcPath = "inf_test_src_large.bin";
    const std::string dstPath = "inf_test_dst_large.bin";

    bool ok = compressToFile(largeInput, srcPath);
    EXPECT_TRUE(ok, "Compression for large data test should succeed");
    if (!ok) return;

    FILE* src = std::fopen(srcPath.c_str(), "rb");
    FILE* dst = std::fopen(dstPath.c_str(), "wb");
    int ret = Z_ERRNO;
    if (src && dst) {
        ret = inf(src, dst);
        std::fclose(src);
        std::fclose(dst);
    }

    EXPECT_EQ(ret, Z_OK, "inf should return Z_OK for successful decompression of large input");

    // Verify decompressed content
    std::string decompressed = readFromFile(dstPath);
    EXPECT_EQ(decompressed, largeInput, "Decompressed large data should match original");

    remove(srcPath.c_str());
    remove(dstPath.c_str());
}

int main() {
    // Run tests
    test_inf_basic_decompression();
    test_inf_empty_input();
    test_inf_corrupted_input();
    test_inf_destination_write_error();
    test_inf_large_data();

    // Summary
    std::cout << "Tests run: " << g_tests_run << "\n";
    std::cout << "Tests failed: " << g_tests_failed << "\n";
    if (!g_failures.empty()) {
        std::cout << "Failure details:\n";
        for (const auto& msg : g_failures) {
            std::cout << msg << "\n";
        }
    }

    return g_tests_failed ? 1 : 0;
}