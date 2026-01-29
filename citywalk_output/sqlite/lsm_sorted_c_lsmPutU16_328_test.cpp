/*
  Unit Test Suite for lsmPutU16 (C function) using C++11 (no GTest)

  Step guidance embedded as comments:
  - Step 1: Understand lsmPutU16 functionality: It writes a 16-bit value into a two-byte array in big-endian order:
      aOut[0] = high byte, aOut[1] = low byte
  - Step 2/3: Generate a compact test suite that exercises multiple 16-bit inputs to confirm correct high/low byte placement.
  - Step 3: Refine with multiple distinct test cases, including edge values like 0x0000 and 0xFFFF, plus representative values.

  Notes:
  - We declare the C function lsmPutU16 with C linkage for linking with the C source.
  - We define minimal test scaffolding (EXPECT-like) that does not terminate on failure to maximize coverage.
  - We exercise true/false “branch” coverage in the sense of different input values covering different byte patterns.
  - All tests are self-contained in this single file.
*/

#include <iostream>
#include <cstdio>
#include <cstdint>


// Step 1: Candidate type aliases consistent with the focal function signature
typedef unsigned char u8;
typedef unsigned short u16;

// Step 2: Declare the focal function with C linkage for linking with the C source file
#ifdef __cplusplus
extern "C" {
#endif
void lsmPutU16(u8 *aOut, u16 nVal);
#ifdef __cplusplus
}
#endif

// Simple non-terminating assertion framework
static int g_failed = 0;

static void log_fail(const char* desc, int actual, int expected) {
    std::cerr << "FAIL: " << desc << " - got " << actual << " expected " << expected << std::endl;
    ++g_failed;
}
static void log_pass(const char* desc) {
    std::cout << "PASS: " << desc << std::endl;
}
static void expect_eq_u8(const char* desc, uint8_t actual, uint8_t expected) {
    if (actual != expected) {
        log_fail(desc, static_cast<int>(actual), static_cast<int>(expected));
    } else {
        log_pass(desc);
    }
}
static void expect_eq_u16(const char* desc, uint16_t actual, uint16_t expected) {
    if (actual != expected) {
        log_fail(desc, static_cast<int>(actual), static_cast<int>(expected));
    } else {
        log_pass(desc);
    }
}

// Helper to run a single test case for lsmPutU16
static void test_case_lsmPutU16(const char* name, u16 val, uint8_t expected_hi, uint8_t expected_lo) {
    u8 out[2] = {0, 0};
    lsmPutU16(out, val);
    // High byte check
    char hiDesc[256];
    std::snprintf(hiDesc, sizeof(hiDesc), "%s high byte", name);
    expect_eq_u8(hiDesc, out[0], expected_hi);
    // Low byte check
    char loDesc[256];
    std::snprintf(loDesc, sizeof(loDesc), "%s low byte", name);
    expect_eq_u8(loDesc, out[1], expected_lo);
}

// Step 3: Comprehensive test set
static void run_all_tests() {
    // Test 1: Typical 16-bit value with non-zero high and low bytes
    test_case_lsmPutU16("Test 0xABCD", 0xABCD, 0xAB, 0xCD);

    // Test 2: Zero value (both bytes zero)
    test_case_lsmPutU16("Test 0x0000", 0x0000, 0x00, 0x00);

    // Test 3: Maximum 16-bit value (0xFFFF)
    test_case_lsmPutU16("Test 0xFFFF", 0xFFFF, 0xFF, 0xFF);

    // Test 4: High byte non-zero, low byte non-zero (0x1234)
    test_case_lsmPutU16("Test 0x1234", 0x1234, 0x12, 0x34);

    // Test 5: High byte zero, low byte non-zero (0x00FF)
    test_case_lsmPutU16("Test 0x00FF", 0x00FF, 0x00, 0xFF);
}

// Entry point
int main() {
    run_all_tests();

    if (g_failed) {
        std::cerr << "Summary: " << g_failed << " test(s) failed." << std::endl;
        return 1;
    } else {
        std::cout << "Summary: All tests passed." << std::endl;
        return 0;
    }
}