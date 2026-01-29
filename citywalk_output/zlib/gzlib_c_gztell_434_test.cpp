/*
Unit test suite for the focal method gztell (wrapper around gztell64) from gzlib.c.

Assumptions and approach:
- The test is written in C++11 and uses only the C standard library and zlib's public API.
- We do not use GoogleTest or other test frameworks; instead, we implement a small,
  self-contained test harness with non-terminating EXPECT_* style assertions.
- The tests exercise realistic usage by creating small gzip files on disk, using gzopen/gzwrite
  to produce data, then opening them with gzopen/gzopen64 and validating gztell/gztell64
  return values as data is read.
- This targets the true-branch behavior of the conditional in gztell (ret == (z_off_t)ret).
  The false-branch would require a ret value not fitting into z_off_t, which depends on platform
  specifics; the tests focus on typical, portable behavior and cross-check consistency between
  gztell and gztell64 where possible.

Notes:
- Link with zlib (e.g., -lz) when compiling.
- The tests create and remove temporary gzip files in the current directory.

Compilation (example):
g++ -std=c++11 -O2 -Wall -Wextra test_gztell.cpp -lz -o test_gztell

*/

#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <gzguts.h>
#include <zlib.h>
#include <cstdlib>


extern "C" {
}

// Simple non-terminating assertion helpers
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_EQ(expected, actual, msg) do {                                 \
    ++g_total_tests;                                                             \
    if (static_cast<long long>(actual) != static_cast<long long>(expected)) {     \
        std::cerr << "[FAILED] " << msg                                             \
                  << " | Expected: " << (expected)                                    \
                  << " | Actual: " << (actual) << std::endl;                        \
        ++g_failed_tests;                                                        \
    } else {                                                                     \
        /* Optional: print pass message for verbose runs */                       \
        std::cout << "[PASSED] " << msg << std::endl;                            \
    }                                                                            \
} while (0)

#define EXPECT_GE(expected, actual, msg) do {                                   \
    ++g_total_tests;                                                             \
    if (static_cast<long long>(actual) < static_cast<long long>(expected)) {           \
        std::cerr << "[FAILED] " << msg                                             \
                  << " | Expected >= " << (expected)                                 \
                  << " | Actual: " << (actual) << std::endl;                        \
        ++g_failed_tests;                                                        \
    } else {                                                                     \
        std::cout << "[PASSED] " << msg << std::endl;                            \
    }                                                                            \
} while (0)

#define EXPECT_TRUE(cond, msg) do {                                             \
    ++g_total_tests;                                                             \
    if (!(cond)) {                                                               \
        std::cerr << "[FAILED] " << msg << " | Condition is false." << std::endl; \
        ++g_failed_tests;                                                        \
    } else {                                                                     \
        std::cout << "[PASSED] " << msg << std::endl;                            \
    }                                                                            \
} while (0)

// Helper to remove a file safely
static void remove_file_safe(const std::string &path) {
    std::remove(path.c_str());
}

// Test 1: Validate gztell with a small gzip file opened via gzopen (read path)
// - Creates a gzip file containing "abcdef" (6 bytes).
// - Opens it for reading, asserts initial offset is 0 (true-branch).
// - Reads 3 bytes, asserts offset becomes 3.
// - Reads remaining 3 bytes, asserts offset becomes 6.
static void test_gztell_basic_read_path() {
    std::string path = "test_gztell_basic_read_path.gz";

    // Create a small gzip file containing "abcdef"
    gzFile f = gzopen(path.c_str(), "wb");
    if (f == nullptr) {
        std::cerr << "[ERROR] Failed to create test gzip file: " << path << std::endl;
        return;
    }
    const char *data = "abcdef";
    int written = gzwrite(f, data, 6);
    if (written != 6) {
        std::cerr << "[ERROR] Failed to write test data to gzip file." << std::endl;
        gzclose(f);
        remove_file_safe(path);
        return;
    }
    gzclose(f);

    // Open for reading and test gztell behavior
    f = gzopen(path.c_str(), "rb");
    if (f == nullptr) {
        std::cerr << "[ERROR] Failed to open test gzip file for reading: " << path << std::endl;
        remove_file_safe(path);
        return;
    }

    // Offset before any read should be 0
    z_off_t off0 = gztell(f);
    EXPECT_EQ(0, off0, "gztell initial offset should be 0");

    // Read 3 bytes
    char buf[4] = {0};
    int r = gzread(f, buf, 3);
    if (r != 3) {
        std::cerr << "[ERROR] gzread did not return 3 as expected." << std::endl;
        gzclose(f);
        remove_file_safe(path);
        return;
    }

    // Offset after reading 3 bytes should be 3
    z_off_t off1 = gztell(f);
    EXPECT_EQ(3, off1, "gztell offset after reading 3 bytes should be 3");

    // Read remaining 3 bytes
    r = gzread(f, buf, 3);
    if (r != 3) {
        std::cerr << "[ERROR] gzread did not return 3 for the remaining data." << std::endl;
        gzclose(f);
        remove_file_safe(path);
        return;
    }

    // Offset after reading all data should be 6
    z_off_t off2 = gztell(f);
    EXPECT_EQ(6, off2, "gztell offset after reading all data should be 6");

    gzclose(f);
    remove_file_safe(path);
}

// Test 2: Validate gztell64 equivalence with gztell using gzopen64 when available
// - Creates another gzip file and opens with gzopen64.
// - Compares gztell and gztell64 after various reads to ensure consistency.
static void test_gztell64_consistency_with_open64() {
    // Some environments may optimize gzopen64 to same as gzopen; we handle both.
    std::string path = "test_gztell64_consistency.gz";

    // Create a small gzip file containing "123456"
    gzFile fwb = gzopen(path.c_str(), "wb");
    if (fwb == nullptr) {
        std::cerr << "[ERROR] Failed to create test gzip file for gzopen64: " << path << std::endl;
        return;
    }
    const char *data = "123456";
    if (gzwrite(fwb, data, 6) != 6) {
        std::cerr << "[ERROR] Failed to write test data for gzopen64 test." << std::endl;
        gzclose(fwb);
        remove_file_safe(path);
        return;
    }
    gzclose(fwb);

    // Try open with gzopen64; if unavailable, gracefully skip this portion
    gzFile f64 = gzopen64(path.c_str(), "rb");
    if (f64 == nullptr) {
        // If gzopen64 is not available, fallback to gzopen and skip the 64-bit path
        std::cout << "[INFO] gzopen64 not available; skipping 64-bit path consistency test." << std::endl;
        remove_file_safe(path);
        return;
    }

    // Initial offset should be 0 for 64-bit path as well
    z_off64_t off64_start = gztell64(f64);
    z_off_t off32_start = gztell(f64);
    EXPECT_EQ(0, off64_start, "gztell64 initial offset should be 0");
    EXPECT_EQ(0, off32_start, "gztell initial offset via gztell should be 0");

    // Read 2 bytes
    char buf[3] = {0};
    int r = gzread(f64, buf, 2);
    if (r != 2) {
        std::cerr << "[ERROR] gzread on 64-bit path did not return 2 as expected." << std::endl;
        gzclose(f64);
        remove_file_safe(path);
        return;
    }

    z_off64_t off64_after2 = gztell64(f64);
    z_off_t off32_after2 = gztell(f64);
    EXPECT_EQ(2, off64_after2, "gztell64 after reading 2 bytes should be 2");
    EXPECT_EQ(2, off32_after2, "gztell after reading 2 bytes should be 2");

    // Read remaining 4 bytes
    r = gzread(f64, buf, 4);
    if (r != 4) {
        std::cerr << "[ERROR] gzread on 64-bit path did not return 4 as expected for remaining data." << std::endl;
        gzclose(f64);
        remove_file_safe(path);
        return;
    }

    z_off64_t off64_after_all = gztell64(f64);
    z_off_t off32_after_all = gztell(f64);
    EXPECT_EQ(6, off64_after_all, "gztell64 after reading all data should be 6");
    EXPECT_EQ(6, off32_after_all, "gztell after reading all data should be 6");

    gzclose(f64);
    remove_file_safe(path);
}

// Entry point: Run tests
int main() {
    std::cout << "Starting gztell unit tests (no GTest)..." << std::endl;

    test_gztell_basic_read_path();
    test_gztell64_consistency_with_open64();

    std::cout << "Tests completed. Total: " << g_total_tests
              << ", Failed: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed
    return g_failed_tests;
}