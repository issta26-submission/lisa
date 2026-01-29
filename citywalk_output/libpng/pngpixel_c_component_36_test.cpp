// pngpixel.cpp test suite (C++11, no GTest)
// This test suite exercises the focal function `component` from pngpixel.c.
// It covers various bit depths (1, 2, 4, 8, 16) and validates the precise bit extraction logic.
// It also exercises the default (invalid bit depth) path via a separate process to avoid terminating the test runner.
// Note: This test assumes a POSIX environment for fork/waitpid.

#include <cstdint>
#include <../../png.h>
#include <setjmp.h>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>


// Prototype of the focal function (C linkage) from pngpixel.c
// We declare it with explicit fixed-width types to match typical 32-bit/packed usage.
extern "C" int component(const unsigned char* row, uint32_t x, uint32_t c,
                         uint32_t bit_depth, uint32_t channels);

// Helper to log test results
static void log_pass(const char* name) {
    std::cout << "[PASS] " << name << "\n";
}
static void log_fail(const char* name, const std::string& msg) {
    std::cerr << "[FAIL] " << name << ": " << msg << "\n";
}

// Test 1: bit_depth = 8, simple direct mapping: pixel value is the first byte of the row
// Expectation: component(row, 0, 0, 8, 1) == row[0]
static bool test_bit_depth_8() {
    const char* name = "test_bit_depth_8";
    std::vector<uint8_t> row = { 0xAB }; // 171 decimal
    int val = component(row.data(), 0, 0, 8, 1);
    if (static_cast<uint32_t>(val) != 0xAB) {
        log_fail(name, "expected 0xAB (171), got " + std::to_string(val));
        return false;
    }
    log_pass(name);
    return true;
}

// Test 2: bit_depth = 16, simple mapping: two bytes form a 16-bit value
// Expectation: component(row, 0, 0, 16, 1) == (row[0] << 8) | row[1]
static bool test_bit_depth_16() {
    const char* name = "test_bit_depth_16";
    std::vector<uint8_t> row = { 0x12, 0x34 }; // 0x1234
    int val = component(row.data(), 0, 0, 16, 1);
    uint32_t expected = (0x12u << 8) + 0x34u;
    if (static_cast<uint32_t>(val) != expected) {
        log_fail(name, "expected 0x1234 (4660), got " + std::to_string(val));
        return false;
    }
    log_pass(name);
    return true;
}

// Test 3: bit_depth = 1, x = 0, c = 0, high bit (MSB) set
// Expectation: (row[0] >> 7) & 0x01
static bool test_bit_depth_1_x0_msb1() {
    const char* name = "test_bit_depth_1_x0_msb1";
    std::vector<uint8_t> row = { 0x80 }; // MSB = 1
    int val = component(row.data(), 0, 0, 1, 1);
    if (val != 1) {
        log_fail(name, "expected 1 for MSB set, got " + std::to_string(val));
        return false;
    }
    log_pass(name);
    return true;
}

// Test 4: bit_depth = 1, x = 1, c = 0, bit at position corresponding to x=1
// Expectation: (row[0] >> (7 - 1)) & 0x01 == (row[0] >> 6) & 0x01
static bool test_bit_depth_1_x1_shift() {
    const char* name = "test_bit_depth_1_x1_shift";
    std::vector<uint8_t> row = { 0x40 }; // bit 6 is 1
    int val = component(row.data(), 1, 0, 1, 1);
    if (val != 1) {
        log_fail(name, "expected 1 for bit at x=1, got " + std::to_string(val));
        return false;
    }
    log_pass(name);
    return true;
}

// Test 5: bit_depth = 2, x = 1, c = 0, verify non-zero within top two bits
// Expectation: (row[0] >> 5) & 0x03
static bool test_bit_depth_2_x1() {
    const char* name = "test_bit_depth_2_x1";
    std::vector<uint8_t> row = { 0xE0 }; // 1110 0000; (>>5) = 0x07, &0x03 -> 3
    int val = component(row.data(), 1, 0, 2, 1);
    if (static_cast<uint32_t>(val) != 3) {
        log_fail(name, "expected 3, got " + std::to_string(val));
        return false;
    }
    log_pass(name);
    return true;
}

// Test 6: bit_depth = 4, x = 3, c = 0, non-zero nibble extraction when bit_offset_lo = 3
// Expectation: (row[0] >> (4 - 3)) & 0x0F = (row[0] >> 1) & 0x0F
static bool test_bit_depth_4_x3() {
    const char* name = "test_bit_depth_4_x3";
    std::vector<uint8_t> row = { 0x7F }; // 0111 1111; (>>1) = 0x3F; &0x0F -> 0x0F (15)
    int val = component(row.data(), 3, 0, 4, 1);
    if (static_cast<uint32_t>(val) != 0x0F) {
        log_fail(name, "expected 0x0F (15), got " + std::to_string(val));
        return false;
    }
    log_pass(name);
    return true;
}

// Test 7: invalid bit_depth (default branch) should exit with status 1
// This test runs the call in a child process to avoid terminating the test runner.
// It expects the child to exit with code 1 due to the default branch in the switch.
static bool test_invalid_bit_depth_via_fork() {
    const char* name = "test_invalid_bit_depth_via_fork";
    pid_t pid = fork();
    if (pid == -1) {
        log_fail(name, "fork() failed");
        return false;
    }
    if (pid == 0) {
        // Child process: trigger invalid bit depth path
        unsigned char row = 0;
        component(&row, 0, 0, 3, 1); // 3 is not a supported bit depth
        // If component unexpectedly returns, exit with code 0 (should not happen)
        _exit(0);
    } else {
        int status = 0;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == 1) {
            log_pass(name);
            return true;
        } else {
            log_fail(name, "child did not exit with code 1 as expected");
            return false;
        }
    }
}

// Main function: runs all tests in a single process.
// According to the Domain Knowledge, test methods can be invoked from main when GTest is not used.
int main() {
    bool ok = true;
    ok &= test_bit_depth_8();
    ok &= test_bit_depth_16();
    ok &= test_bit_depth_1_x0_msb1();
    ok &= test_bit_depth_1_x1_shift();
    ok &= test_bit_depth_2_x1();
    ok &= test_bit_depth_4_x3();
    ok &= test_invalid_bit_depth_via_fork();

    std::cout << "pngpixel test suite " << (ok ? "PASSED" : "FAILED") << std::endl;
    return ok ? 0 : 1;
}