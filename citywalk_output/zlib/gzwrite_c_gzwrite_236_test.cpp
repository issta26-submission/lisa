#include <cstdio>
#include <cstring>
#include <stdarg.h>
#include <iostream>
#include <gzguts.h>
#include <zlib.h>
#include <cstdlib>


// Simple non-terminating test harness (no GTest) with verbose output.
// This suite focuses on testing the focal gzwrite method behavior in gzwrite.c.
//
// Coverage goals derived from the focal method (Step 1):
// - Handle NULL file input (file == NULL) -> should return 0.
// - Handle invalid internal state (state->mode != GZ_WRITE or state->err != Z_OK) -> should return 0.
// - Handle length that does not fit in an int ((int)len < 0) -> should report error and return 0.
// - Optional: normal path when a valid gzFile and length are provided. This test uses the real zlib library.
//
// Notes:
// - We rely on the public zlib API (gzopen, gzwrite, gzerror, gzclose) to exercise
//   the gzwrite path without exposing internal static state.
// - The negative-length test uses gzerror to verify that Z_DATA_ERROR is recorded.
// - All tests are designed to be non-fatal: they report pass/fail but continue to run.
// - The tests do not depend on private/internal headers (they use the public zlib API).

static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_EQ(a, b, msg) do {                               \
    ++g_total_tests;                                              \
    if ((a) != (b)) {                                           \
        std::cerr << "TEST FAILED: " << (msg)                    \
                  << " | Expected: " << (b)                     \
                  << ", Got: " << (a) << std::endl;            \
        ++g_failed_tests;                                       \
    }                                                           \
} while (0)

#define EXPECT_TRUE(cond, msg) do {                               \
    ++g_total_tests;                                               \
    if (!(cond)) {                                                 \
        std::cerr << "TEST FAILED: " << (msg) << std::endl;      \
        ++g_failed_tests;                                        \
    }                                                            \
} while (0)

static void test_gzwrite_null_file() {
    // Test 1: NULL file should yield 0 (no operation performed)
    // Rationale: The focal method checks file == NULL early and returns 0.
    const unsigned char data[] = "hello";
    int ret = gzwrite(nullptr, data, sizeof(data) - 1);
    EXPECT_EQ(ret, 0, "gzwrite should return 0 when file is NULL");
}

static void test_gzwrite_negative_len() {
    // Test 2: Negative length path (via unsigned large value causing (int)len < 0)
    // We open a real gz file and attempt to write with a length that overflows int.
    // The expected behavior is that gzwrite returns 0 and gzerror reports Z_DATA_ERROR.
    const char filename[] = "test_negative_len.gz";

    gzFile f = gzopen(filename, "wb");
    if (f == nullptr) {
        std::cerr << "SKIP: Could not open test file for negative-length test\n";
        ++g_total_tests;
        return;
    }

    const unsigned char data[] = "ABCD";
    // Force (int)len < 0 by using a large unsigned value (0x80000000)
    unsigned big_len = 0x80000000u;
    int ret = gzwrite(f, data, big_len);

    // Expect 0 because length doesn't fit in int
    EXPECT_EQ(ret, 0, "gzwrite should return 0 when length does not fit in int");

    // gzerror should indicate Z_DATA_ERROR
    int err = Z_OK;
    const char* msg = gzerror(f, &err);
    EXPECT_EQ(err, Z_DATA_ERROR, "gzerror should report Z_DATA_ERROR for invalid length");

    gzclose(f);
    std::remove(filename);
}

static void test_gzwrite_small_data() {
    // Test 3: Normal usage with a small, valid write.
    // Open a gz file, write a known small payload, ensure the return value matches payload size.
    const char filename[] = "test_small_write.gz";

    gzFile f = gzopen(filename, "wb");
    if (f == nullptr) {
        std::cerr << "SKIP: Could not open test file for small-write test\n";
        ++g_total_tests;
        return;
    }

    const unsigned char data[] = "TEST"; // 4 bytes
    int ret = gzwrite(f, data, sizeof(data) - 1);

    EXPECT_EQ(ret, 4, "gzwrite should return number of bytes written for small payload");

    gzclose(f);
    std::remove(filename);
}

static void test_gzwrite_zero_length() {
    // Test 4: Writing zero bytes should return 0 and not fail.
    const char filename[] = "test_zero_write.gz";

    gzFile f = gzopen(filename, "wb");
    if (f == nullptr) {
        std::cerr << "SKIP: Could not open test file for zero-length write test\n";
        ++g_total_tests;
        return;
    }

    const unsigned char data[] = "DATA";
    int ret = gzwrite(f, data, 0);

    EXPECT_EQ(ret, 0, "gzwrite with zero length should return 0");

    gzclose(f);
    std::remove(filename);
}

int main() {
    std::cout << "Running gzwrite.c focused unit tests (via zlib public API)..." << std::endl;

    test_gzwrite_null_file();
    test_gzwrite_negative_len();
    test_gzwrite_small_data();
    test_gzwrite_zero_length();

    std::cout << "Tests run: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return g_failed_tests ? 1 : 0;
}