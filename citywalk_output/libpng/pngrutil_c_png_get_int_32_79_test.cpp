#include <png.h>
#include <string>
#include <iostream>
#include <pngpriv.h>


// Lightweight test harness (no GoogleTest). Uses C++11 only.
// Tests the focal function: png_get_int_32(png_const_bytep buf)

static int g_failures = 0;

// Simple test logger for pass/fail without terminating on failure
static void log_pass(const std::string& test_name) {
    std::cout << "[TEST PASSED] " << test_name << std::endl;
}

static void log_fail(const std::string& test_name, const std::string& detail) {
    std::cerr << "[TEST FAILED] " << test_name << " - " << detail << std::endl;
    ++g_failures;
}

// Helper to run a single test case and compare expected vs actual
static void run_case(const std::string& test_name, const unsigned char* buf, std::size_t len, int expected) {
    // The focal method expects a 4-byte big-endian value in buf
    // We ensure the length is 4 for correctness
    if (len != 4) {
        log_fail(test_name, "Buffer length must be 4 bytes");
        return;
    }
    int got = png_get_int_32(reinterpret_cast<png_const_bytep>(buf));
    if (got == expected) {
        log_pass(test_name);
    } else {
        log_fail(test_name, "Expected " + std::to_string(expected) + " but got " + std::to_string(got));
    }
}

// Individual test cases (explanatory comments describe the purpose and scenario)

// Test 1: Non-negative small value should be returned as a positive int
// Scenario: 0x00000001 -> 1
static void test_non_negative_small_value() {
    const unsigned char buf[4] = {0x00, 0x00, 0x00, 0x01}; // 1
    run_case("test_non_negative_small_value", buf, 4, 1);
}

// Test 2: All bits set (0xFFFFFFFF) should yield -1 via two's-complement path
// Scenario: 0xFFFFFFFF -> -1
static void test_negative_one_from_all_ones() {
    const unsigned char buf[4] = {0xFF, 0xFF, 0xFF, 0xFF}; // -1 in 32-bit two's complement
    run_case("test_negative_one_from_all_ones", buf, 4, -1);
}

// Test 3: Large negative value achievable without overflow
// Scenario: 0x80000001 -> -2147483647
static void test_negative_large_value_without_overflow() {
    const unsigned char buf[4] = {0x80, 0x00, 0x00, 0x01}; // -2147483647
    run_case("test_negative_large_value_without_overflow", buf, 4, -2147483647);
}

// Test 4: Overflow path after two's-complement conversion (0x80000000 -> 0)
// Scenario: 0x80000000 should trigger overflow guard and return 0
static void test_overflow_path_zero_result() {
    const unsigned char buf[4] = {0x80, 0x00, 0x00, 0x00}; // overflow -> 0
    run_case("test_overflow_path_zero_result", buf, 4, 0);
}

// Test 5: Negative value where after conversion MSB is 0 (e.g., 0xC0000000 -> -1073741824)
// Scenario: 0xC0000000 -> -1073741824
static void test_negative_overflow_big() {
    const unsigned char buf[4] = {0xC0, 0x00, 0x00, 0x00}; // -1073741824
    run_case("test_negative_overflow_big", buf, 4, -1073741824);
}

// Test 6: Zero buffer should return 0 (non-negative path)
static void test_zero_buffer_returns_zero() {
    const unsigned char buf[4] = {0x00, 0x00, 0x00, 0x00}; // 0
    run_case("test_zero_buffer_returns_zero", buf, 4, 0);
}

// Test 7: Small negative value (-2) from 0xFFFFFFFE
// Scenario: 0xFFFFFFFE -> -2
static void test_small_negative_two() {
    const unsigned char buf[4] = {0xFF, 0xFF, 0xFF, 0xFE}; // -2
    run_case("test_small_negative_two", buf, 4, -2);
}

int main() {
    // Run all test cases
    test_non_negative_small_value();
    test_negative_one_from_all_ones();
    test_negative_large_value_without_overflow();
    test_overflow_path_zero_result();
    test_negative_overflow_big();
    test_zero_buffer_returns_zero();
    test_small_negative_two();

    if (g_failures == 0) {
        std::cout << "[ALL TESTS PASSED]" << std::endl;
        return 0;
    } else {
        std::cerr << "[TEST SUITE FAILED] " << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}