// Unit test suite for the focal method compress2 (C interface) using C++11 (no GTest allowed).
// The tests are designed to be executable by a C++11 compiler and link against zlib.
// Tests avoid private members/methods and rely on the public API provided by the focal code.
// Explanatory comments are provided for each test to describe intent and coverage.

#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <zlib.h>


extern "C" int ZEXPORT compress2(Bytef *dest, uLongf *destLen, const Bytef *source,
                                uLong sourceLen, int level);
extern "C" int ZEXPORT uncompress(Bytef *dest, uLongf *destLen, const Bytef *source, uLong sourceLen);

// Non-terminating assertion helper to maximize test coverage while continuing execution.
// Reports failures to stderr and keeps a global failure count.
static int g_failures = 0;
static void expect(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "Test failure: " << msg << std::endl;
        ++g_failures;
    }
}

// Test 1: Round-trip compression/decompression for a small, typical ASCII input.
// Intent: Validate that compress2 returns Z_OK, writes a valid compressed payload,
// and that uncompress successfully restores the original data.
static void test_compress2_round_trip_small() {
    const std::string input = "The quick brown fox jumps over the lazy dog";
    const Bytef* source = reinterpret_cast<const Bytef*>(input.data());
    const uLong sourceLen = static_cast<uLong>(input.size());

    // Destination buffer: sufficiently large to hold compressed data (worst-case small input).
    std::vector<Bytef> destBuf(1024);
    uLongf destLen = static_cast<uLongf>(destBuf.size());

    // Use a reasonable compression level.
    int level = Z_DEFAULT_COMPRESSION;

    // Call the focal method.
    int ret = compress2(destBuf.data(), &destLen, source, sourceLen, level);
    expect(ret == Z_OK, "compress2 should return Z_OK for small input with adequate dest buffer");

    // Decompress and verify integrity.
    std::vector<Bytef> outBuf(sourceLen);
    uLongf outLen = static_cast<uLongf>(outBuf.size());

    int retDec = uncompress(outBuf.data(), &outLen, destBuf.data(), destLen);
    expect(retDec == Z_OK, "uncompress should succeed on valid compressed data");

    expect(outLen == sourceLen, "uncompress should yield original length");
    bool equal = (std::memcmp(outBuf.data(), source, static_cast<size_t>(sourceLen)) == 0);
    expect(equal, "uncompress output should match the original input");
}

// Test 2: Insufficient destination buffer should cause a non-Z_OK return.
// Intent: Ensure that the function detects a too-small destination and returns an error
// (typically Z_BUF_ERROR) rather than silently succeeding or corrupting data.
static void test_compress2_insufficient_dest() {
    const std::string input = "abc";
    const Bytef* source = reinterpret_cast<const Bytef*>(input.data());
    const uLong sourceLen = static_cast<uLong>(input.size());

    // Intentionally tiny destination buffer to trigger Z_BUF_ERROR path.
    std::vector<Bytef> destBuf(2);
    uLongf destLen = static_cast<uLongf>(destBuf.size());

    int level = Z_DEFAULT_COMPRESSION;
    int ret = compress2(destBuf.data(), &destLen, source, sourceLen, level);

    // We expect a non-Z_OK return due to insufficient destination space.
    expect(ret != Z_OK, "compress2 should not return Z_OK when destination buffer is too small");
}

// Test 3: Round-trip compression/decompression for a larger, highly repetitive input.
// Intent: Exercise multiple internal deflate cycles by using a longer input and verify correctness.
static void test_compress2_round_trip_large_repetitive() {
    std::string input(10000, 'A'); // large repetitive data that should compress well
    const Bytef* source = reinterpret_cast<const Bytef*>(input.data());
    const uLong sourceLen = static_cast<uLong>(input.size());

    // Destination buffer sized to comfortably hold compressed data.
    // For repetitive data, compressed size should be <= original size.
    std::vector<Bytef> destBuf(20000);
    uLongf destLen = static_cast<uLongf>(destBuf.size());

    int level = Z_DEFAULT_COMPRESSION;
    int ret = compress2(destBuf.data(), &destLen, source, sourceLen, level);
    expect(ret == Z_OK, "compress2 should return Z_OK for large repetitive input with adequate dest buffer");

    // Decompress and verify integrity.
    std::vector<Bytef> outBuf(sourceLen);
    uLongf outLen = static_cast<uLongf>(outBuf.size());

    int retDec = uncompress(outBuf.data(), &outLen, destBuf.data(), destLen);
    expect(retDec == Z_OK, "uncompress should succeed on valid compressed data (large input)");

    expect(outLen == sourceLen, "uncompress should yield original length for large input");
    bool equal = (std::memcmp(outBuf.data(), source, static_cast<size_t>(sourceLen)) == 0);
    expect(equal, "uncompress output should match the original large input for large test");
}

// Main test runner
int main() {
    std::cout << "Starting compress2 unit tests (no GTest, non-terminating assertions)." << std::endl;

    test_compress2_round_trip_small();
    test_compress2_insufficient_dest();
    test_compress2_round_trip_large_repetitive();

    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}