// C++11 test suite for the focal method test_dict_deflate (as declared in example_dict_fuzzer.c)
// This test suite is lightweight (no GTest) and uses a tiny custom assertion framework.
// It exercises multiple branches by varying the first byte of the injected data (which
// influences level, windowBits, memLevel, and strategy via data[0] % ...).
// It relies on the external definitions used by test_dict_deflate (data, dataLen,
// dictionaryLen, dictId, and the extern function test_dict_deflate). The test cases
// configure those extern symbols via a buffer that simulates the fuzzer input.

#include <cstdio>
#include <cstring>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <stdlib.h>
#include <cstdlib>


// Include zlib headers to ensure compatibility with the focal method's usage.
// The focal method uses zlib deflate APIs (deflateInit2, deflateSetDictionary, deflate, deflateEnd, deflateBound).

// Forward declaration of the focal method under test.
// It is expected to be defined in example_dict_fuzzer.c and linked during build.
// We pass in a buffer via compr/comprLen as in the original code.
extern "C" int test_dict_deflate(unsigned char **compr, size_t *comprLen);

// The focal code relies on the following global symbols. We declare them as extern here
// so that the linker can resolve them to the actual definitions in the project under test.
// The test harness will set these symbols (where possible) via a buffer to exercise different paths.
extern unsigned char *data;
extern size_t dataLen;
extern size_t dictionaryLen;
extern unsigned long dictId;

// Lightweight non-terminating assertion helpers.
// They accumulate test results but do not abort on failure, enabling broader coverage.
static int g_total = 0;
static int g_passed = 0;

#define EXPECT_TRUE(cond) do { \
    ++g_total; \
    if (cond) { ++g_passed; } else { \
        fprintf(stderr, "Expectation failed: %s at %s:%d\n", #cond, __FILE__, __LINE__); \
    } \
} while (0)

#define EXPECT_NOT_NULL(ptr) do { \
    ++g_total; \
    if ((ptr) != nullptr) { ++g_passed; } else { \
        fprintf(stderr, "Expectation failed: %s is null at %s:%d\n", #ptr, __FILE__, __LINE__); \
    } \
} while (0)

#define EXPECT_GT(a, b) do { \
    ++g_total; \
    if ((a) > (b)) { ++g_passed; } else { \
        fprintf(stderr, "Expectation failed: %s > %s (%lld > %lld) at %s:%d\n", #a, #b, (long long)(a), (long long)(b), __FILE__, __LINE__); \
    } \
} while (0)

// Free function to satisfy the deflate stream's memory handling in test environments.
// We provide zalloc/zfree compatible wrappers expected by the focal code.
// These wrappers are simple and safe for unit testing purposes.
static voidpf zalloc(voidpf opaque, uInt items, uInt size) {
    (void)opaque;
    return (voidpf)calloc(items, size);
}
static void zfree(voidpf opaque, voidpf address) {
    (void)opaque;
    free(address);
}

// Helper to run a single test case by constructing a buffer that encodes both
// the dictionary (first dictionaryLen bytes) and the payload (the rest is data to be
// compressed, but the entire buffer is sent as input since test_dict_deflate uses dataLen
// as avail_in and dictionaryLen as the dictionary size).
static bool runTestCase(int firstByte, size_t dictLen, size_t payloadLen) {
    // Build a single buffer: [dictionary (dictLen bytes)] + [payload (payloadLen bytes)]
    size_t totalLen = dictLen + payloadLen;
    if (totalLen == 0) return false;

    std::vector<unsigned char> buf(totalLen);
    if (buf.empty()) return false;

    // Set the first byte used to influence the compression level/strategy branches.
    buf[0] = (unsigned char)firstByte;

    // Initialize dictionary portion with deterministic data (non-zero variety).
    for (size_t i = 0; i < dictLen; ++i) {
        buf[i] = (unsigned char)((i + 1) & 0xFF);
    }

    // Initialize payload portion with some repeating pattern to simulate input data.
    for (size_t i = dictLen; i < totalLen; ++i) {
        buf[i] = (unsigned char)(0x40 + (i % 26)); // ASCII-like letters
    }

    // Expose the buffer to the focal function via the extern variables.
    data = buf.data();
    dataLen = totalLen;
    dictionaryLen = dictLen;
    dictId = 0; // reset to a known state

    // Call the focal function under test.
    unsigned char *compr = nullptr;
    size_t comprLen = 0;
    int ret = test_dict_deflate(&compr, &comprLen);

    // Validate basic post-conditions and cleanup.
    EXPECT_TRUE(ret == 0);
    if (ret == 0) {
        EXPECT_NOT_NULL(compr);
        EXPECT_GT(comprLen, (size_t)0);
        // If compression succeeded, the dictionary adler hash should be captured in dictId.
        EXPECT_GT(dictId, (unsigned long)0);
        if (compr) free(compr);
    } else {
        // In case of failure, still attempt to clean up if memory was allocated unexpectedly.
        if (compr) free(compr);
    }

    return ret == 0;
}

// Simple helper to print a summary at the end.
static void printSummary(const char* title) {
    printf("=== %s ===\n", title);
    printf("Passed %d / %d tests\n", g_passed, g_total);
}

// Main function to drive the test suite.
// We intentionally exercise multiple branches by varying the firstByte and ensuring
// different code paths in test_dict_deflate get hit.
int main() {
    printf("Starting test suite for test_dict_deflate (C++11 harness)\n");

    // Common configuration: dictionary length and payload length.
    const size_t dictLen = 16;      // non-trivial dictionary length
    const size_t payloadLen = 256;  // payload length to stress the compressor

    // Test Case 1: Baseline (firstByte = 0) to exercise typical path (level = -1)
    // This should hit the "level" computation path without forcing the -1 override.
    runTestCase(0, dictLen, payloadLen);

    // Test Case 2: Trigger level adjustment branch (firstByte results in level == 0, then set to -1)
    // Using firstByte = 1 affects level = 1 % 11 - 1 = 0 -> code sets level to -1.
    runTestCase(1, dictLen, payloadLen);

    // Test Case 3: Another branch scenario (firstByte yields level not equal to 0/1, e.g., 3)
    // This exercises the non-overridden path for level determination.
    runTestCase(3, dictLen, payloadLen);

    printSummary("test_dict_deflate coverage results");

    // Return non-zero if any test failed to indicate issues to a test harness.
    return (g_total == g_passed) ? 0 : 1;
}