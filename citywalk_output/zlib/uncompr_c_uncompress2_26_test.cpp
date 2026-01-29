/*
Step 1 - Program Understanding (Keywords extracted for test design)
Candidate Keywords:
- uncompress2, uncompress, z_stream, inflateInit, inflate, inflateEnd
- Z_OK, Z_BUF_ERROR, Z_DATA_ERROR, Z_STREAM_END, Z_NEED_DICT, Z_NO_FLUSH
- Bytef, uLong, uLongf, z_const, alloc_func, free_func
- source, dest, destLen, sourceLen, stream.total_out, stream.avail_in, stream.avail_out
- allocation/initialization patterns, cleanup via inflateEnd
- path variants: destLen initially non-zero vs zero, dest pointing to real buffer vs internal buf
- edge handling of incomplete streams and output buffering logic
This test suite targets uncompress2 from uncompr.c, directly driving inflate-based decompression and exercising both branches of key conditionals, input/output length handling, and return-status translation.
*/

#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <algorithm>
#include <cstdlib>


// Include zlib types and declarations

// Provide C-linkage for the focal function
extern "C" int ZEXPORT uncompress2(Bytef *dest, uLongf *destLen, const Bytef *source, uLong *sourceLen);

// Lightweight, self-contained test harness (non-terminating assertions)
static int gFailures = 0;

#define TEST_PASS(msg)        do { printf("PASS: %s\n", msg); } while (0)
#define TEST_FAIL(msg)        do { printf("FAIL: %s\n", msg); ++gFailures; } while (0)
#define EXPECT_TRUE(cond, msg) do { if (cond) TEST_PASS(msg); else { printf("  Expected true: %s\n", msg); TEST_FAIL("Condition failed"); } } while (0)
#define EXPECT_EQ(a, b, msg)  do { if ((a) == (b)) TEST_PASS(msg); else { printf("  Expected equal: %s (got %ld vs %ld)\n", msg, (long)(a), (long)(b)); TEST_FAIL("Equality mismatch"); } } while (0)
#define EXPECT_NE(a, b, msg)  do { if ((a) != (b)) TEST_PASS(msg); else { printf("  Expected not equal: %s (value %ld)\n", msg, (long)(a)); TEST_FAIL("Inequality mismatch"); } } while (0)

// Helper: compress a string using zlib to generate test input for uncompress2
static std::vector<unsigned char> compress_string(const std::string& input) {
    uLongf bound = compressBound(static_cast<uLong>(input.size()));
    std::vector<unsigned char> out(bound);
    int ret = compress2(out.data(), &bound,
                        reinterpret_cast<const Bytef*>(input.data()),
                        static_cast<uLong>(input.size()),
                        Z_BEST_COMPRESSION);
    if (ret != Z_OK) {
        throw std::runtime_error("Compression in test harness failed");
    }
    out.resize(bound);
    return out;
}

// Test 1: Basic successful uncompression with ample destination buffer.
// Verifies: Z_OK is returned, destination contains original data, and sourceLen is reduced to zero.
static void test_uncompress2_basic_success() {
    const std::string input = "The quick brown fox jumps over the lazy dog. Uncompress2 test string for unit tests.";
    auto compressed = compress_string(input);

    // Prepare destination buffer large enough to hold the uncompressed data
    std::vector<unsigned char> dest(input.size() + 16); // extra space
    uLongf destLen = static_cast<uLongf>(dest.size());
    uLong sourceLen = static_cast<uLong>(compressed.size());

    // Call the focal function
    int ret = uncompress2(dest.data(), &destLen,
                          compressed.data(), &sourceLen);

    // Validate outcomes
    EXPECT_TRUE(ret == Z_OK, "uncompress2 basic: return Z_OK on valid compressed input");
    // destLen should reflect actual decompressed size
    EXPECT_EQ(static_cast<long>(destLen), static_cast<long>(input.size()),
              "uncompress2 basic: decompressed length equals original length");
    // Null-terminate and compare content
    if (destLen > 0 && destLen <= dest.size()) {
        dest[destLen] = '\0';
        std::string recovered(reinterpret_cast<char*>(dest.data()), destLen);
        EXPECT_TRUE(recovered == input, "uncompress2 basic: decompressed content matches input");
    } else {
        TEST_FAIL("uncompress2 basic: decompressed length invalid for content verification");
    }
    // sourceLen should be reduced by consumed input length (len + avail_in)
    EXPECT_TRUE(sourceLen == 0, "uncompress2 basic: sourceLen reduced to 0 after full consumption");
}

// Test 2: Trigger the else-branch (destLen == 0) path to exercise detection buffer logic.
// We ensure the function still completes and returns a valid status code.
static void test_uncompress2_destLen_zero_path() {
    const std::string input = "Test for destLen == 0 path.";
    auto compressed = compress_string(input);

    // dest points to a tiny buffer; destLen is explicitly zero to trigger the internal buf path
    unsigned char tinyBuf[1] = {0};
    uLongf destLen = 0;
    uLong sourceLen = static_cast<uLong>(compressed.size());

    int ret = uncompress2(tinyBuf, &destLen, compressed.data(), &sourceLen);

    // We expect a well-defined status; exact value may depend on internal buffering when dest is tiny.
    // Accept any non-crash, non-undefined behavior outcome, but ensure function executed.
    EXPECT_TRUE(ret == Z_OK || ret == Z_BUF_ERROR || ret == Z_DATA_ERROR,
                "uncompress2 destLen==0 path: returns a valid zlib status code");
    // sourceLen should still reflect consumption / partial consumption semantics
    EXPECT_TRUE(sourceLen <= static_cast<uLong>(compressed.size()), "uncompress2 destLen==0 path: sourceLen updated appropriately");
}

// Test 3: Invalid input data should yield a non-Z_OK return code.
// This exercises error paths (e.g., Z_DATA_ERROR) for corrupted or non-compressed data.
static void test_uncompress2_invalid_input() {
    // Create clearly invalid compressed data (random bytes)
    std::vector<unsigned char> junk(64, 0xAA);
    std::vector<unsigned char> dest(128);
    uLongf destLen = static_cast<uLongf>(dest.size());
    uLong sourceLen = static_cast<uLong>(junk.size());

    int ret = uncompress2(dest.data(), &destLen, junk.data(), &sourceLen);

    // Expect not Z_OK for invalid input
    EXPECT_TRUE(ret != Z_OK, "uncompress2 invalid input: non-Z_OK status returned");
}

// Test 4: Large input to exercise multiple inflate iterations and typical buffering behavior.
// Verifies that large payload decompresses correctly and consumes all input.
static void test_uncompress2_large_input() {
    // Generate a larger input
    std::string base = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. ";
    std::string input;
    for (int i = 0; i < 500; ++i) input += base;

    auto compressed = compress_string(input);

    // Destination buffer large enough
    std::vector<unsigned char> dest(input.size() + 100);
    uLongf destLen = static_cast<uLongf>(dest.size());
    uLong sourceLen = static_cast<uLong>(compressed.size());

    int ret = uncompress2(dest.data(), &destLen, compressed.data(), &sourceLen);

    EXPECT_TRUE(ret == Z_OK, "uncompress2 large input: return Z_OK on valid compressed data");
    EXPECT_EQ(static_cast<long>(destLen), static_cast<long>(input.size()), "uncompress2 large input: decompressed length matches input size");
    if (destLen > 0 && destLen <= dest.size()) {
        dest[destLen] = '\0';
        std::string recovered(reinterpret_cast<char*>(dest.data()), destLen);
        EXPECT_TRUE(recovered == input, "uncompress2 large input: content matches original");
    } else {
        TEST_FAIL("uncompress2 large input: invalid decompressed length for content check");
    }
    EXPECT_TRUE(sourceLen == 0, "uncompress2 large input: sourceLen reduced to 0 after full consumption");
}

// Test runner
static void run_all_tests() {
    printf("Starting unit tests for uncompress2 (C++11 test harness, no GTest)\n");
    test_uncompress2_basic_success();
    test_uncompress2_destLen_zero_path();
    test_uncompress2_invalid_input();
    test_uncompress2_large_input();
    if (gFailures == 0) {
        printf("ALL TESTS PASSED\n");
    } else {
        printf("TOTAL FAILURES: %d\n", gFailures);
    }
}

int main() {
    run_all_tests();
    return gFailures ? 1 : 0;
}