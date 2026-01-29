// Auto-generated C++11 unit test suite for the focal method read32 in jpgicc.c
// This test suite is designed to be compiled alongside the project (no GTest).
// It uses a lightweight test harness with non-terminating checks to maximize coverage.

#include <iccjpeg.h>
#include <functional>
#include <jpeglib.h>
#include <cstddef>
#include <vector>
#include <iostream>
#include <string>
#include <utils.h>
#include <cstdint>


// Declaration of the focal C function under test (C linkage)
extern "C" uint32_t read32(uint8_t* arr, size_t pos, int swapBytes, size_t max);

// Lightweight test harness
static int g_total = 0;
static int g_failed = 0;

#define TEST_LOG_PASS(name) \
    do { \
        ++g_total; \
        std::cout << "[PASSED] " << (name) << "\n"; \
    } while (0)

#define TEST_LOG_FAIL(name, exp, act) \
    do { \
        ++g_total; \
        ++g_failed; \
        std::cout << "[FAILED] " << (name) << " | expected: " << (exp) << ", actual: " << (act) << "\n"; \
    } while (0)

#define TEST_EQ(name, actual, expected) \
    do { \
        if ((actual) == (expected)) { \
            TEST_LOG_PASS(name); \
        } else { \
            TEST_LOG_FAIL(name, (expected), (actual)); \
        } \
    } while (0)

// Helper to run all tests
static void RunAllTests();

// Test cases for read32

// Test 1: Boundary condition - when pos + 4 >= max, craftedFile() is invoked and 0 is returned
void test_read32_boundary_returns_zero() {
    // Arrange
    uint8_t data[4] = {0x01, 0x02, 0x03, 0x04};
    size_t pos = 0;
    int swapBytes = 0;
    size_t max = 4; // pos + 4 == max -> trigger boundary branch

    // Act
    uint32_t result = read32(data, pos, swapBytes, max);

    // Assert
    TEST_EQ("read32_boundary_returns_zero", result, 0u);
}

// Test 2: Big-endian path - swapBytes == 0, general case with four bytes starting at pos=0
void test_read32_big_endian() {
    // Arrange
    uint8_t data[4] = {0x01, 0x02, 0x03, 0x04};
    size_t pos = 0;
    int swapBytes = 0; // big-endian
    size_t max = 5;    // pos + 4 < max

    // Act
    uint32_t result = read32(data, pos, swapBytes, max);

    // Assert
    uint32_t expected = (0x01u << 24) | (0x02u << 16) | (0x03u << 8) | 0x04u;
    TEST_EQ("read32_big_endian", result, expected);
}

// Test 3: Little-endian path - swapBytes != 0, general case with four bytes starting at pos=0
void test_read32_little_endian() {
    // Arrange
    uint8_t data[4] = {0x01, 0x02, 0x03, 0x04};
    size_t pos = 0;
    int swapBytes = 1; // little-endian
    size_t max = 5;

    // Act
    uint32_t result = read32(data, pos, swapBytes, max);

    // Assert
    uint32_t expected = 0x04030201u;
    TEST_EQ("read32_little_endian", result, expected);
}

// Test 4: Positive offset with big-endian, ensuring correct indexing from non-zero pos
void test_read32_big_endian_with_offset() {
    // Arrange
    uint8_t data[5] = {0x00, 0x11, 0x22, 0x33, 0x44}; // bytes at indices 1..4 are relevant
    size_t pos = 1;
    int swapBytes = 0; // big-endian
    size_t max = 10;

    // Act
    uint32_t result = read32(data, pos, swapBytes, max);

    // Assert
    uint32_t expected = (0x11u << 24) | (0x22u << 16) | (0x33u << 8) | 0x44u;
    TEST_EQ("read32_big_endian_with_offset", result, expected);
}

// Test 5: Boundary condition with non-zero pos triggering craftedFile() path
void test_read32_boundary_nonzero_pos() {
    // Arrange
    uint8_t data[6] = {0x00, 0x00, 0xAA, 0xBB, 0xCC, 0xDD}; // indices 2..5
    size_t pos = 2;
    int swapBytes = 0;
    size_t max = 6; // pos + 4 == max -> boundary trigger

    // Act
    uint32_t result = read32(data, pos, swapBytes, max);

    // Assert
    TEST_EQ("read32_boundary_nonzero_pos", result, 0u);
}

// Test 6: Little-endian with non-zero offset
void test_read32_little_endian_with_offset() {
    // Arrange
    uint8_t data[5] = {0xFF, 0x01, 0x02, 0x03, 0x04}; // pos=1 uses 1..4
    size_t pos = 1;
    int swapBytes = 1; // little-endian
    size_t max = 8;

    // Act
    uint32_t result = read32(data, pos, swapBytes, max);

    // Assert
    uint32_t expected = 0x04030201u;
    TEST_EQ("read32_little_endian_with_offset", result, expected);
}

static void RunAllTests() {
    test_read32_boundary_returns_zero();
    test_read32_big_endian();
    test_read32_little_endian();
    test_read32_big_endian_with_offset();
    test_read32_boundary_nonzero_pos();
    test_read32_little_endian_with_offset();

    // Summary
    std::cout << "Test Summary: Total=" << g_total << " Failed=" << g_failed
              << " Passed=" << (g_total - g_failed) << "\n";

    // Exit status: non-zero if any test failed
    if (g_failed > 0) {
        // Provide non-zero exit code to indicate failure in automated scripts
        std::exit(1);
    }
}

int main() {
    // Run all tests
    RunAllTests();
    return 0;
}