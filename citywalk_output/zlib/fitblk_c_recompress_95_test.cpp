// Unit test suite for the focal method recompress in fitblk.c
// This test suite is written in C++11 (no GTest) and relies on the zlib library.
// It exercises the recompress function by creating a known compressed input, invoking
// recompress, and verifying that the re-compressed output decompresses back to the
// original data. It also exercises two scenarios (small and large input) to cover
// different execution paths inside the loop (e.g., flush decisions).
//
// Notes derived from the focal code:
// - recompress inflates input data (inf) into a raw buffer, then deflates that buffer into def.
// - The function uses Z_NO_FLUSH, Z_FINISH, Z_STREAM_END and asserts various stream states.
// - We verify behavior by round-tripping: orig -> compress -> recompress -> decompress -> orig.
//
// This test does not rely on Google Test. It provides a minimal test harness that continues
// after failures and reports a summary at the end.

// Candidate Keywords and concepts derived from the focal method:
// - z_streamp infl / def, inflate/deflate calls
// - RAWLEN macro (internal raw buffer size used by the focal function)
// - Z_NO_FLUSH, Z_FINISH, Z_STREAM_END, Z_MEM_ERROR, Z_STREAM_ERROR, Z_DATA_ERROR, Z_NEED_DICT
// - memory buffers: raw (in the focal method), next_in, next_out, avail_in, avail_out
// - loop structure: do { ... } while (ret != Z_STREAM_END && def->avail_out != 0);
// - data flow: inflate -> deflate
// - return value: ret

#include <cstdio>
#include <cstring>
#include <assert.h>
#include <stdexcept>
#include <string>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <stdlib.h>
#include <cstdlib>


#ifdef __cplusplus
extern "C" {
#endif
// The focal code is C; expose recompress with C linkage for the test harness.
int recompress(z_streamp inf, z_streamp def);
#ifdef __cplusplus
}
#endif

// Simple non-terminating test harness (continues on failure, reports summary at end)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        std::cerr << "Test failed: " << (msg) << " (line " << __LINE__ << ")" << std::endl; \
        ++g_tests_failed; \
    } \
} while(0)


// Helper: compress a std::string into a vector<unsigned char> using zlib's compress().
static std::vector<unsigned char> compressString(const std::string& input) {
    uLongf bound = compressBound(static_cast<uLong>(input.size()));
    std::vector<unsigned char> out(bound);
    uLongf outLen = bound;
    int res = ::compress(out.data(), &outLen,
                         reinterpret_cast<const unsigned char*>(input.data()),
                         static_cast<uLong>(input.size()));
    if (res != Z_OK) {
        throw std::runtime_error("compressString: zlib compress failed");
    }
    out.resize(outLen);
    return out;
}

// Helper: Decompress a zlib-compressed buffer into a std::string (binary-safe)
static std::string decompressBuffer(const unsigned char* data, size_t len) {
    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));
    int ret = inflateInit(&strm);
    if (ret != Z_OK) {
        throw std::runtime_error("decompressBuffer: inflateInit failed");
    }

    const size_t CHUNK = 4096;
    unsigned char outBuf[CHUNK];

    std::string result;
    strm.next_in = const_cast<unsigned char*>(const_cast<const unsigned char*>(data));
    strm.avail_in = static_cast<uInt>(len);

    do {
        strm.next_out = outBuf;
        strm.avail_out = static_cast<uInt>(CHUNK);
        ret = inflate(&strm, Z_NO_FLUSH);
        if (ret != Z_OK && ret != Z_STREAM_END) {
            inflateEnd(&strm);
            throw std::runtime_error("decompressBuffer: inflate failed");
        }
        size_t have = CHUNK - strm.avail_out;
        if (have > 0) {
            result.append(reinterpret_cast<char*>(outBuf), have);
        }
    } while (ret != Z_STREAM_END);

    inflateEnd(&strm);
    return result;
}

// Test 1: Basic recompress with a small input.
// Steps:
// - Compress a small original string to compressed1.
// - Set up infl with compressed1, set up def with a reasonably large output buffer.
// - Call recompress(&inf, &def) and ensure it returns Z_STREAM_END or a successful end state.
// - Decompress the deflated output and verify it matches the original string.
static void test_recompress_small_input() {
    const std::string orig = "The quick brown fox jumps over the lazy dog";

    // Compress the original data to feed as input to recompress's inflate
    std::vector<unsigned char> compressed1 = compressString(orig);

    // Prepare zlib streams for recompress call
    z_stream infl;
    std::memset(&infl, 0, sizeof(infl));
    int r = inflateInit(&infl);
    TEST_ASSERT(r == Z_OK, "inflateInit failed in small-input test");

    infl.next_in = const_cast<unsigned char*>(compressed1.data());
    infl.avail_in = static_cast<uInt>(compressed1.size());

    // Prepare deflate stream with a reasonably large output buffer
    z_stream defl;
    std::memset(&defl, 0, sizeof(defl));
    r = deflateInit(&defl, Z_DEFAULT_COMPRESSION);
    TEST_ASSERT(r == Z_OK, "deflateInit failed in small-input test");

    const size_t DEF_BUF_SIZE = 65536; // large enough to hold re-compressed data
    std::vector<unsigned char> defBuf(DEF_BUF_SIZE);
    defl.next_out = defBuf.data();
    defl.avail_out = static_cast<uInt>(DEF_BUF_SIZE);

    // Call the focal function
    int ret = recompress(&infl, &defl);
    TEST_ASSERT(ret != Z_STREAM_ERROR, "recompress returned Z_STREAM_ERROR in small-input test");
    // Acceptable to be Z_STREAM_END when the process completes
    TEST_ASSERT(ret == Z_STREAM_END || ret == Z_OK, "recompress did not return expected end state in small-input test");

    // Ensure we produced some output
    size_t produced = static_cast<size_t>(defl.total_out);
    TEST_ASSERT(produced > 0, "recompress produced no output in small-input test");

    // Decompress the re-compressed output and compare to original
    std::string decomp = decompressBuffer(defBuf.data(), produced);
    TEST_ASSERT(decomp == orig, "Decompressed re-compressed data does not match original in small-input test");

    // Cleanup
    inflateEnd(&infl);
    deflateEnd(&defl);

    // If needed, show a quick success message
    if (g_tests_failed == 0) {
        std::cout << "[PASS] test_recompress_small_input" << std::endl;
    } else {
        std::cout << "[FAIL] test_recompress_small_input" << std::endl;
    }
}

// Test 2: Recompress with a larger input to exercise multiple iterations and different flush behavior.
// We'll generate a larger input by repeating a pattern to exceed RAWLEN and likely trigger multiple
// inflate/deflate cycles inside recompress.
static void test_recompress_large_input() {
    // Create a larger input payload
    std::string orig;
    orig.reserve(50000);
    const std::string chunk = "0123456789";
    for (int i = 0; i < 5000; ++i) { // 10 chars * 5000 = 50k
        orig += chunk;
    }

    // Compress the original data
    std::vector<unsigned char> compressed1 = compressString(orig);

    // Prepare zlib streams
    z_stream infl;
    std::memset(&infl, 0, sizeof(infl));
    int r = inflateInit(&infl);
    TEST_ASSERT(r == Z_OK, "inflateInit failed in large-input test");

    infl.next_in = const_cast<unsigned char*>(compressed1.data());
    infl.avail_in = static_cast<uInt>(compressed1.size());

    z_stream defl;
    std::memset(&defl, 0, sizeof(defl));
    r = deflateInit(&defl, Z_DEFAULT_COMPRESSION);
    TEST_ASSERT(r == Z_OK, "deflateInit failed in large-input test");

    const size_t DEF_BUF_SIZE = 131072; // larger buffer to accommodate bigger output
    std::vector<unsigned char> defBuf(DEF_BUF_SIZE);
    defl.next_out = defBuf.data();
    defl.avail_out = static_cast<uInt>(DEF_BUF_SIZE);

    int ret = recompress(&infl, &defl);
    TEST_ASSERT(ret != Z_STREAM_ERROR, "recompress returned Z_STREAM_ERROR in large-input test");
    TEST_ASSERT(ret == Z_STREAM_END || ret == Z_OK, "recompress did not end correctly in large-input test");

    size_t produced = static_cast<size_t>(defl.total_out);
    TEST_ASSERT(produced > 0, "recompress produced no output in large-input test");

    // Decompress the re-compressed output and verify equality with original
    std::string decomp = decompressBuffer(defBuf.data(), produced);
    TEST_ASSERT(decomp == orig, "Decompressed re-compressed data does not match original in large-input test");

    // Cleanup
    inflateEnd(&infl);
    deflateEnd(&defl);

    if (g_tests_failed == 0) {
        std::cout << "[PASS] test_recompress_large_input" << std::endl;
    } else {
        std::cout << "[FAIL] test_recompress_large_input" << std::endl;
    }
}

// Entry point for running tests
int main() {
    try {
        test_recompress_small_input();
        test_recompress_large_input();
    } catch (const std::exception& ex) {
        std::cerr << "Exception during tests: " << ex.what() << std::endl;
        ++g_tests_failed;
    }

    // Summary
    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << std::endl;
    return (g_tests_failed == 0) ? 0 : 1;
}