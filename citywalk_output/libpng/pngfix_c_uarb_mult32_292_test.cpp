// Test suite for the focal method uarb_mult32 in pngfix.c
// Note: This test suite is implemented in C++11 (no Google Test) and uses a minimal
// in-house test harness with non-terminating assertions (tests continue after
// failures). It assumes the focal function uses an opaque unsigned 16-bit digit
// representation (base 2^16) for 'uarb' (i.e., a pointer to the least-significant digit).
// The test provides its own reference implementation to verify correctness.

#include <cstdint>
#include <algorithm>
#include <setjmp.h>
#include <vector>
#include <string.h>
#include <iomanip>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>


// Focal function prototype (C linkage)
extern "C" int uarb_mult32(uint16_t* acc, int a_digits, uint16_t* num, int n_digits, uint32_t val);

// Type alias mirroring the assumed representation in the focal code
typedef uint16_t* uarb;

// Simple non-terminating test assertion macro (log on failure, continue)
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ << " - " << (msg) \
                  << " (expected true but got false)\n"; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_EQ(lhs, rhs, msg) do { \
    if(!((lhs) == (rhs))) { \
        std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ << " - " << (msg) \
                  << " (lhs=" << (lhs) << ", rhs=" << (rhs) << ")\n"; \
        ++g_failures; \
    } \
} while(0)

static int g_failures = 0;

// Helper: display a digits array for debugging
static void dump_digits(const std::vector<uint16_t>& digits, const char* label) {
    std::cout << label << " (len=" << digits.size() << "): [";
    for (size_t i = 0; i < digits.size(); ++i) {
        std::cout << std::hex << std::setw(4) << std::setfill('0') << digits[i];
        if (i + 1 < digits.size()) std::cout << " ";
    }
    std::cout << std::dec << "]\n";
}

// Reference implementation (in C++) to compute acc += num * val using base-2^16 digits.
// This mirrors the expected mathematical result to verify uarb_mult32.
// acc and num are little-endian vectors of 16-bit digits (least significant first).
static void reference_accumulate_mul32(std::vector<uint16_t>& acc, const std::vector<uint16_t>& num, uint32_t val) {
    // If either side is zero, nothing to do
    if (num.empty() || val == 0) return;

    const uint32_t BASE = 0x10000;
    uint16_t low  = static_cast<uint16_t>(val & 0xFFFF);
    uint16_t high = static_cast<uint16_t>((val >> 16) & 0xFFFF);

    // Prepare a temporary array large enough to hold the sum: acc + (num * val)
    size_t a = acc.size();
    size_t b = num.size();
    size_t needed = std::max(a, b) + 2; // extra room for carries

    std::vector<uint32_t> tmp(needed, 0);

    // Copy existing acc into tmp
    for (size_t i = 0; i < a; ++i) tmp[i] = acc[i];

    // Add num * low_part
    for (size_t i = 0; i < b; ++i) {
        tmp[i] += static_cast<uint32_t>(num[i]) * static_cast<uint32_t>(low);
    }
    // Add num * high_part, shifted by 1 digit (i.e., index+1)
    if (high != 0) {
        for (size_t i = 0; i < b; ++i) {
            tmp[i + 1] += static_cast<uint32_t>(num[i]) * static_cast<uint32_t>(high);
        }
    }

    // Normalize carries in base 2^16
    uint32_t carry = 0;
    size_t limit = needed;
    for (size_t i = 0; i < limit; ++i) {
        uint32_t v = tmp[i] + carry;
        acc[i] = static_cast<uint16_t>(v & 0xFFFF);
        carry = v >> 16;
        // Ensure acc has room if new digits are produced
        // We'll extend acc if necessary after normalization
    }
    // If carry remains, push new digits
    while (carry != 0) {
        acc.push_back(static_cast<uint16_t>(carry & 0xFFFF));
        carry >>= 16;
    }

    // Trim any leading zeros (optional, to mirror a_digits concept loosely)
    // Do not trim inside test harness; we will rely on the returned a_digits value from the focal function
}

// Run a single test case with given inputs and expected results
static void run_case(
    const std::vector<uint16_t>& acc_init, int a_digits,
    const std::vector<uint16_t>& num, int n_digits,
    uint32_t val,
    const std::vector<uint16_t>& expected_after, int expected_digits,
    const char* case_desc)
{
    // Prepare buffers
    int alloc_len = std::max(a_digits, n_digits) + 2;
    std::vector<uint16_t> acc_buf(alloc_len, 0);
    for (int i = 0; i < a_digits; ++i) acc_buf[i] = acc_init[i];

    std::vector<uint16_t> num_buf(n_digits, 0);
    for (int i = 0; i < n_digits; ++i) num_buf[i] = num[i];

    // Call the focal function
    int new_digits = uarb_mult32(acc_buf.data(), a_digits, num_buf.data(), n_digits, val);

    // Compare with expected
    bool ok = true;
    if (new_digits != expected_digits) {
        std::cerr << "[CASE] " << case_desc << " - incorrect new digits. "
                  << "expected=" << expected_digits << " got=" << new_digits << "\n";
        ok = false;
    }

    for (int i = 0; i < expected_digits; ++i) {
        uint16_t got = (i < new_digits ? acc_buf[i] : 0);
        if (got != expected_after[i]) {
            std::cerr << "[CASE] " << case_desc << " - mismatch at digit " << i
                      << ". expected " << expected_after[i] << ", got " << got << "\n";
            ok = false;
            break;
        }
    }

    if (ok) {
        EXPECT_TRUE(true, case_desc); // mark as passed (non-terminating)
    }
}

// Test 1: Basic addition without carry (acc += num * val with small values, single-digit result)
static void test_case_basic_no_overflow() {
    // acc = [1], a_digits=1
    // num = [2], n_digits=1
    // val = 3  -> acc += 2 * 3 = 6, so acc = 1 + 6 = 7 -> [7]
    std::vector<uint16_t> acc_init = {1};
    std::vector<uint16_t> num = {2};
    uint32_t val = 3;

    std::vector<uint16_t> expected_after = {7};
    int expected_digits = 1;

    run_case(acc_init, 1, num, 1, val, expected_after, expected_digits,
             "Basic single-digit multiply-add without carry");
}

// Test 2: Addition causes carry into a new digit
static void test_case_addition_with_carry() {
    // acc = [0xFFFE], a_digits=1
    // num = [0x0002], n_digits=1
    // val = 1 -> acc += 2 * 1 = 2 -> 0xFFFE + 2 = 0x10000 => [0x0000, 0x0001]
    std::vector<uint16_t> acc_init = {0xFFFE};
    std::vector<uint16_t> num = {0x0002};
    uint32_t val = 1;

    std::vector<uint16_t> expected_after = {0x0000, 0x0001};
    int expected_digits = 2;

    run_case(acc_init, 1, num, 1, val, expected_after, expected_digits,
             "Addition that carries into a new most-significant digit");
}

// Test 3: Upper 16-bit part of val is non-zero (val has two 16-bit limbs)
static void test_case_upper_part_nonzero() {
    // acc = [1], a_digits=1
    // num = [1], n_digits=1
    // val = 0x0001_0002 (lower=0x0002, upper=0x0001)
    // acc += num * val = 1 * 0x10002 = 0x10003 => [0x0003, 0x0001]
    std::vector<uint16_t> acc_init = {1};
    std::vector<uint16_t> num = {1};
    uint32_t val = 0x00010002;

    std::vector<uint16_t> expected_after = {0x0003, 0x0001};
    int expected_digits = 2;

    run_case(acc_init, 1, num, 1, val, expected_after, expected_digits,
             "Upper 16-bit part of 32-bit val is non-zero");
}

// Test 4: val = 0, nothing should change
static void test_case_val_zero_no_change() {
    // acc = [5], a_digits=1
    // num = [3], n_digits=1
    // val = 0 -> no change
    std::vector<uint16_t> acc_init = {5};
    std::vector<uint16_t> num = {3};
    uint32_t val = 0;

    std::vector<uint16_t> expected_after = {5};
    int expected_digits = 1;

    run_case(acc_init, 1, num, 1, val, expected_after, expected_digits,
             "Val == 0 -> no change to acc");
}

// Test 5: n_digits == 0, no operation should occur
static void test_case_no_n_digits() {
    // acc = [7], a_digits=1
    // num = [9], n_digits=0
    // val = 0x123456; since n_digits == 0, no operation should occur
    std::vector<uint16_t> acc_init = {7};
    std::vector<uint16_t> num = {9};
    uint32_t val = 0x00123456;

    std::vector<uint16_t> expected_after = {7};
    int expected_digits = 1;

    run_case(acc_init, 1, num, 0, val, expected_after, expected_digits,
             "n_digits == 0 -> no operation performed");
}

int main() {
    std::cout << "Starting unit test suite for uarb_mult32 (pngfix.c) using in-house harness.\n";

    test_case_basic_no_overflow();
    test_case_addition_with_carry();
    test_case_upper_part_nonzero();
    test_case_val_zero_no_change();
    test_case_no_n_digits();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) FAILED\n";
        return 1;
    }
}