// Test suite for the focal method: test_sync
// This test suite is designed to compile under C++11 without Google Test.
// It uses the project's focal function (declared extern "C") and exercises
// multiple input scenarios to improve coverage across the function's flow.
// Explanatory comments are provided for each unit test.

#include <cstdio>
#include <cstring>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <stdexcept>
#include <string>
#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <zlib.h>
#include <stdlib.h>
#include <cstdlib>


// Include zlib for on-the-fly compressed data generation

// Declaration of the focal function under test.
// The function signature is taken from the provided focal method.
// We declare it with C linkage to avoid name mangling issues when linking.
extern "C" int test_sync(unsigned char *compr, size_t comprLen, unsigned char *uncompr,
                         size_t uncomprLen);

// Simple non-terminating assertion helpers.
// They log failures but do not abort execution, allowing multiple tests to run.
#define EXPECT_TRUE(cond) \
    do { if(!(cond)) fprintf(stderr, "EXPECT_TRUE failed: %s:%d: %s\n", __FILE__, __LINE__, #cond); } while(0)

#define EXPECT_EQ(a, b) \
    do { if(((unsigned long long)(a)) != ((unsigned long long)(b))) \
        fprintf(stderr, "EXPECT_EQ failed: %s == %s (%llu != %llu) at %s:%d\n", #a, #b, (unsigned long long)(a), (unsigned long long)(b), __FILE__, __LINE__); } while(0)

// Helper: compress input data using zlib's compress2 to produce a valid "compr" payload.
// Returns a vector containing the compressed data.
static std::vector<unsigned char> compressData(const unsigned char* data, size_t len) {
    if (len == 0) {
        // For zero-length input, produce a minimal valid compressed block.
        // This is a safe fallback; it yields a tiny compressed output.
        // We rely on zlib to handle the edge case gracefully.
        len = 1;
        unsigned char tmp = 0;
        data = &tmp;
    }

    uLong bound = compressBound(len);
    std::vector<unsigned char> out(bound);
    uLongf outLen = bound;

    int res = compress2(out.data(), &outLen, data, len, Z_DEFAULT_COMPRESSION);
    if (res != Z_OK) {
        throw std::runtime_error("compress2 failed to generate test data");
    }
    out.resize(outLen);
    return out;
}

// Test 1: Validate test_sync with a real, valid zlib-compressed payload.
// Rationale: Generate a small, valid compressed stream and verify that the
// focal function executes through the standard inflate path using real data.
static bool runTest1() {
    // Prepare some input data to compress.
    const char *msg = "The quick brown fox jumps over the lazy dog";
    size_t msgLen = std::strlen(msg);

    // Create compressed data using zlib.
    std::vector<unsigned char> compr = compressData(reinterpret_cast<const unsigned char*>(msg), msgLen);

    // Allocate a reasonably sized output buffer for decompression.
    // The uncompressed size should be at least the original input length.
    std::vector<unsigned char> uncompr(msgLen * 2, 0);

    // Call the focal function.
    int ret = test_sync(compr.data(), compr.size(), uncompr.data(), uncompr.size());

    // We expect the function to return 0 as per its contract.
    EXPECT_EQ(ret, 0);

    // Return whether the expectation held.
    return (ret == 0);
}

// Test 2: Validate test_sync behavior when only the header (2 bytes) is provided.
// Rationale: Exercise the early path where only the zlib header is present and
// ensure the function handles minimal input without crashing.
static bool runTest2() {
    // Two-byte header mock (commonly used zlib header values).
    unsigned char headerOnly[2] = {0x78, 0x9C}; // Typical deflate with default compression
    size_t comprLen = 2;

    // Provide a reasonably sized output buffer.
    std::vector<unsigned char> uncompr(256, 0);

    int ret = test_sync(headerOnly, comprLen, uncompr.data(), uncompr.size());

    // The function should handle this gracefully and return 0.
    EXPECT_EQ(ret, 0);

    return (ret == 0);
}

// Test 3: Optional additional scenario using larger data to increase coverage.
// Rationale: Use a larger input to exercise additional code paths within the
// decompression logic, while still asserting non-crashing behavior.
static bool runTest3() {
    // Create a larger input string.
    const std::string big = "Pack my box with five dozen liquor jugs. Pack my box with five dozen liquor jugs.";
    std::vector<unsigned char> compr;
    try {
        compr = compressData(reinterpret_cast<const unsigned char*>(big.data()), big.size());
    } catch (...) {
        // If compression fails for any reason, conservatively skip this test.
        fprintf(stderr, "Test 3: compression failed, skipping test.\n");
        return true;
    }

    // Uncompressed buffer sized generously to hold the data.
    std::vector<unsigned char> uncompr(big.size() * 2, 0);

    int ret = test_sync(compr.data(), compr.size(), uncompr.data(), uncompr.size());

    // Expect successful path; if not, still report but do not terminate suite.
    EXPECT_EQ(ret, 0);

    return (ret == 0);
}

// Entry point for the test suite.
// Runs all unit tests and reports overall success/failure via exit code.
int main() {
    fprintf(stdout, "Starting test_sync unit test suite...\n");

    bool ok = true;

    fprintf(stdout, "Running Test 1: valid compressed payload\n");
    ok = ok && runTest1();

    fprintf(stdout, "Running Test 2: header-only input\n");
    ok = ok && runTest2();

    fprintf(stdout, "Running Test 3: larger input for extended coverage\n");
    ok = ok && runTest3();

    if (ok) {
        fprintf(stdout, "All test_sync unit tests completed successfully.\n");
        return 0;
    } else {
        fprintf(stderr, "Some test_sync unit tests failed. See logs above for details.\n");
        return 1;
    }
}