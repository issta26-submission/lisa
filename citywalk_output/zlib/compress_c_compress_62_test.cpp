// Test suite for the focal method: compress (wrapper around compress2 with Z_DEFAULT_COMPRESSION)
// This test is designed to be self-contained, compile under C++11, and avoid using Google Test.
// It relies on the actual implementation of compress from zlib (linked in the build environment).
// The test uses a lightweight, non-terminating assertion mechanism to maximize code coverage.

#include <iostream>
#include <zlib.h>
#include <cstring>


// Provide a minimal macro for compatibility if ZEXPORT is not defined by the build system.
// In the actual project, ZEXPORT is typically defined in zlib's headers; here we neutralize it.
#ifndef ZEXPORT
#define ZEXPORT
#endif

// Declare the focal function from compress.c so we can call it in tests.
// We use primitive C types here (unsigned char / unsigned long) which are compatible with zlib's Bytef / uLong types.
extern "C" int ZEXPORT compress(unsigned char *dest, unsigned long *destLen,
                               const unsigned char *source, unsigned long sourceLen);

// Simple non-terminating assertion framework
static int g_test_count = 0;
static int g_failed_count = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_test_count; \
    if(!(cond)) { \
        ++g_failed_count; \
        std::cerr << "TEST FAIL: " << (msg) \
                  << " [at " << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
    } \
} while(0)


// Test 1: Basic success path
// - Purpose: Ensure compress returns Z_OK (success) when dest buffer is large enough.
// - Rationale: This exercises the default successful path of the wrapper around compress2.
void test_compress_basic_success() {
    // Short, common text input
    static const unsigned char src[] = "The quick brown fox jumps over the lazy dog";
    const unsigned long srcLen = static_cast<unsigned long>(strlen(reinterpret_cast<const char*>(src)));

    // Large-enough destination buffer
    unsigned char dest[2048];
    unsigned long destLen = sizeof(dest);

    // Call the focal function
    int ret = compress(dest, &destLen, src, srcLen);

    // Validate: success, and destLen updated to a positive length not exceeding the buffer
    // Z_OK is defined by zlib as 0; we rely on this convention.
    EXPECT_TRUE(ret == 0, "compress should return 0 (Z_OK) for sufficient dest buffer");
    EXPECT_TRUE(destLen > 0 && destLen <= sizeof(dest),
                "destLen should be updated to a positive compressed size not exceeding buffer");
}

// Test 2: Buffer too small path
// - Purpose: Ensure compress returns a non-zero error code when destination buffer is too small.
// - Rationale: This checks the failure branch of the underlying compression routine due to insufficient space.
void test_compress_buffer_too_small() {
    // Basic input
    static const unsigned char src[] = "Sample data to trigger buffer size check";
    const unsigned long srcLen = static_cast<unsigned long>(strlen(reinterpret_cast<const char*>(src)));

    // Deliberately undersized destination buffer
    unsigned char dest[1];
    unsigned long destLen = sizeof(dest);

    int ret = compress(dest, &destLen, src, srcLen);

    // Expect a non-zero error code indicating insufficient buffer (Z_BUF_ERROR in zlib, typically -5)
    EXPECT_TRUE(ret != 0, "compress should return a non-zero error when destination buffer is too small");
}

// Test 3: Zero-length source
// - Purpose: Ensure compress handles empty input gracefully.
// - Rationale: Some libraries treat empty input as a no-op and return Z_OK with destLen == 0.
void test_compress_zero_length_source() {
    static const unsigned char emptySrc[] = "";
    const unsigned long srcLen = 0;

    unsigned char dest[256];
    unsigned long destLen = sizeof(dest);

    int ret = compress(dest, &destLen, emptySrc, srcLen);

    // Expect success with zero compressed output length
    EXPECT_TRUE(ret == 0, "compress should return 0 (Z_OK) for empty input");
    EXPECT_TRUE(destLen == 0, "destLen should be 0 when source length is 0");
}

// Test 4: Large input with adequate destination buffer
// - Purpose: Validate behavior on larger input and ensure proper destLen handling.
// - Rationale: Confirms that the function remains functional beyond trivial inputs.
void test_compress_large_input() {
    // Create a moderately large repetitive input to encourage compression
    const size_t repeats = 400; // 400 * 50 = 20000 bytes if repeated, but we will cap to 4000 for safety
    const unsigned char pattern[] = "ABCD";
    unsigned char src[repeats * (sizeof(pattern) - 1)];
    size_t pos = 0;
    for(size_t i = 0; i < repeats; ++i) {
        std::memcpy(src + pos, pattern, sizeof(pattern) - 1);
        pos += (sizeof(pattern) - 1);
    }
    // Ensure we have a well-defined source length
    const unsigned long srcLen = static_cast<unsigned long>(pos);

    // Destination buffer large enough to hold the compressed data
    unsigned char dest[4096];
    unsigned long destLen = sizeof(dest);

    int ret = compress(dest, &destLen, src, srcLen);

    // Validate: success and destLen within buffer
    EXPECT_TRUE(ret == 0, "compress should return 0 (Z_OK) for large input with adequate buffer");
    EXPECT_TRUE(destLen > 0 && destLen <= sizeof(dest),
                "destLen should reflect the compressed size and fit within the provided buffer");
}


// Entry point: run all tests and report summary
int main() {
    std::cout << "Starting unit tests for compress wrapper..." << std::endl;

    test_compress_basic_success();
    test_compress_buffer_too_small();
    test_compress_zero_length_source();
    test_compress_large_input();

    int total = g_test_count;
    int failures = g_failed_count;

    std::cout << "Test summary: " << total << " tests run, "
              << failures << " failures." << std::endl;

    // Non-terminating tests: return non-zero if any test failed
    return failures ? 1 : 0;
}