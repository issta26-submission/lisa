// Test suite for the focal method uncompress (wrapper around uncompress2 in zlib's
// internal implementation). This test is written for C++11 without GTest and uses
// a small, self-contained test harness that reports pass/fail counts.

#include <iostream>
#include <zlib.h>
#include <cstdlib>
#include <cstring>


// Include zlib header (C API). The header is compatible with C++ and provides
// compress/uncompress functions used in these tests.

// Simple non-terminating test harness
static int g_total_tests = 0;
static int g_passed_tests = 0;

static void log_result(const char* test_name, bool passed) {
    ++g_total_tests;
    if (passed) {
        ++g_passed_tests;
        // Non-terminating: print success message
        std::cout << "[PASS] " << test_name << "\n";
    } else {
        std::cerr << "[FAIL] " << test_name << "\n";
    }
}

// Helper: compare two buffers
static bool buffers_equal(const Bytef* a, const Bytef* b, size_t len) {
    return std::memcmp(a, b, len) == 0;
}

// Test 1: Valid uncompress with sufficient destination buffer
// - Compress a known plaintext string using zlib's compress
// - Uncompress it back using uncompress and verify content and length
static bool test_uncompress_valid() {
    const char* input = "The quick brown fox jumps over the lazy dog. 0123456789";
    uLong inputLen = static_cast<uLong>(std::strlen(input));

    // Allocate a buffer large enough for compressed data
    uLongf compressedBound = compressBound(inputLen);
    Bytef* compressed = static_cast<Bytef*>(std::malloc(compressedBound));
    if (!compressed) {
        std::cerr << "Memory allocation failed for compressed buffer\n";
        return false;
    }

    // Compress input into compressed buffer
    int ret = compress(compressed, &compressedBound,
                       reinterpret_cast<const Bytef*>(input), inputLen);
    if (ret != Z_OK) {
        std::cerr << "Compression failed in test_uncompress_valid: " << ret << "\n";
        std::free(compressed);
        return false;
    }

    // Prepare destination buffer for uncompress
    Bytef* destination = static_cast<Bytef*>(std::malloc(inputLen));
    if (!destination) {
        std::cerr << "Memory allocation failed for destination buffer\n";
        std::free(compressed);
        return false;
    }

    uLongf destLen = inputLen;

    // Call the focal uncompress wrapper
    ret = uncompress(destination, &destLen, compressed, compressedBound);
    // Expect Z_OK, destLen equal to inputLen, and content identical
    bool ok = (ret == Z_OK) && (destLen == inputLen) && buffers_equal(
        destination, reinterpret_cast<const Bytef*>(input), inputLen);

    std::free(compressed);
    std::free(destination);
    return ok;
}

// Test 2: Uncompress with too-small destination buffer should return Z_BUF_ERROR
// - Use the same valid compressed data from Test 1 but provide a 1-byte destination buffer
// - Expect the function to signal insufficient destination space
static bool test_uncompress_small_dest() {
    const char* input = "abcdef";
    uLong inputLen = static_cast<uLong>(std::strlen(input));

    uLongf compressedBound = compressBound(inputLen);
    Bytef* compressed = static_cast<Bytef*>(std::malloc(compressedBound));
    if (!compressed) {
        std::cerr << "Memory allocation failed for compressed buffer (small dest test)\n";
        return false;
    }

    int ret = compress(compressed, &compressedBound,
                       reinterpret_cast<const Bytef*>(input), inputLen);
    if (ret != Z_OK) {
        std::cerr << "Compression failed in test_uncompress_small_dest: " << ret << "\n";
        std::free(compressed);
        return false;
    }

    // Small destination buffer: 1-byte
    Bytef* destination = static_cast<Bytef*>(std::malloc(1));
    if (!destination) {
        std::cerr << "Memory allocation failed for small destination buffer\n";
        std::free(compressed);
        return false;
    }

    uLongf destLen = 1;
    ret = uncompress(destination, &destLen, compressed, compressedBound);

    // Expect a buffer error because destination is too small
    bool ok = (ret == Z_BUF_ERROR);

    std::free(compressed);
    std::free(destination);
    return ok;
}

// Test 3: Uncompress with corrupted compressed data should yield Z_DATA_ERROR
// - Take a valid compressed buffer and deliberately corrupt a byte
static bool test_uncompress_corrupted() {
    const char* input = "Guard this data as it will be corrupted";
    uLong inputLen = static_cast<uLong>(std::strlen(input));

    uLongf compressedBound = compressBound(inputLen);
    Bytef* compressed = static_cast<Bytef*>(std::malloc(compressedBound));
    if (!compressed) {
        std::cerr << "Memory allocation failed for corrupted data test\n";
        return false;
    }

    int ret = compress(compressed, &compressedBound,
                       reinterpret_cast<const Bytef*>(input), inputLen);
    if (ret != Z_OK) {
        std::cerr << "Compression failed in test_uncompress_corrupted: " << ret << "\n";
        std::free(compressed);
        return false;
    }

    // Corrupt the first byte of the compressed data
    if (compressedBound > 0) {
        compressed[0] ^= 0xFF;
    }

    Bytef* destination = static_cast<Bytef*>(std::malloc(inputLen));
    if (!destination) {
        std::cerr << "Memory allocation failed for destination buffer (corrupted test)\n";
        std::free(compressed);
        return false;
    }

    uLongf destLen = inputLen;
    ret = uncompress(destination, &destLen, compressed, compressedBound);

    bool ok = (ret == Z_DATA_ERROR);
    std::free(compressed);
    std::free(destination);
    return ok;
}

int main() {
    // Run tests (Step 3: Test Case Refinement: we provide multiple cases to exercise
    // both successful path and error paths of the uncompress wrapper.)
    log_result("test_uncompress_valid", test_uncompress_valid());
    log_result("test_uncompress_small_dest", test_uncompress_small_dest());
    log_result("test_uncompress_corrupted", test_uncompress_corrupted());

    // Summary
    std::cout << "Test Summary: " << g_passed_tests << " / " << g_total_tests
              << " tests passed.\n";

    // Exit code 0 if all tests pass, else non-zero
    return (g_passed_tests == g_total_tests) ? 0 : 1;
}