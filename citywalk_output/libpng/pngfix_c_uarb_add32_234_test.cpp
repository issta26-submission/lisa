// C++11 test harness for the focal method uarb_add32 in pngfix.c
// The tests are designed to be executable without a test framework (no gtest/gmock).
// We rely on the actual implementation in pngfix.c (linked during build) and
// treat uarb as a 16-bit limb array (uint16_t[]) where num[0] holds the
// least-significant limb and num[1] the next limb.
// We exercise two branches: in_digits == 0 (true path uses uarb_set) and
// in_digits > 0 (false path uses the two-part addition across two limbs).
//
// Important: This test assumes the external library uses a 16-bit limb based
// uarb representation and that uarb_add32 writes across num[0] and num[1] for a
// 32-bit addition. We validate by inspecting the two limbs after the call.

#include <cstdint>
#include <setjmp.h>
#include <string.h>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <ctype.h>


extern "C" {
}

// We declare the focal function with C linkage to match the library's symbol.
// We model uarb as a pointer to the 16-bit limbs (uint16_t*). The real library
// uses a typedef (uarb) that is compatible with a pointer to 16-bit data.
typedef uint16_t* uarb;
uarb uarb_add32(uarb num, int in_digits, png_uint_32 add);

// Simple test harness (non-terminating assertions): track failures and continue.
static int g_failures = 0;

static void log_fail(const char* test_name, const char* message)
{
    std::cerr << "[FAIL] " << test_name << ": " << message << "\n";
    ++g_failures;
}

static void log_pass(const char* test_name)
{
    std::cout << "[PASS] " << test_name << "\n";
}

// Test 1: in_digits == 0 should delegate to uarb_set and store the full 32-bit value
// in two limbs: num[0] = lower 16 bits, num[1] = upper 16 bits.
bool test_add32_in_digits_zero_sets_full_value()
{
    const char* test_name = "test_add32_in_digits_zero_sets_full_value";

    uint16_t digits[4] = {0, 0, 0, 0}; // ensure clean initial state
    uarb base = digits;

    const png_uint_32 add = 0xAABBCCDD; // lower16=0xCCDD, upper16=0xAABB

    // Call the focal function with in_digits = 0 (branch: uses uarb_set)
    uarb_add32(base, 0, add);

    // Verify the two least significant limbs reflect the 32-bit value
    const uint16_t expected_lo  = static_cast<uint16_t>(add & 0xFFFF); // 0xCCDD
    const uint16_t expected_hi  = static_cast<uint16_t>(add >> 16);     // 0xAABB

    bool ok = (digits[0] == expected_lo) && (digits[1] == expected_hi);
    if (!ok) {
        log_fail(test_name, "digits do not match expected 32-bit value after in_digits==0 add");
        // Provide more context
        std::cerr << "Expected: [" << std::hex << expected_hi << " " << expected_lo << "]\n"
                  << "Got:      [" << digits[1] << " " << digits[0] << "]\n";
    } else {
        log_pass(test_name);
    }
    return ok;
}

// Test 2: in_digits > 0 path should correctly distribute 32-bit add across two limbs
// Starting state: num[0] = 0x1111, num[1] = 0x2222, add = 0x00030004
// Expected final: num[0] = 0x1115, num[1] = 0x2225
bool test_add32_in_digits_positive_distributes_across_limbs()
{
    const char* test_name = "test_add32_in_digits_positive_distributes_across_limbs";

    uint16_t digits[4] = {0x1111, 0x2222, 0, 0}; // initial value = 0x22221111
    uarb base = digits;

    const png_uint_32 add = 0x00030004; // lower16=0x0004, upper16=0x0003

    // Call the focal function with in_digits = 2 (branch: uses uarb_inc chain)
    uarb_add32(base, 2, add);

    // Expected final value: 0x22221111 + 0x00030004 = 0x22251115
    // So limbs: lo = 0x1115, hi = 0x2225
    const uint16_t expected_lo = static_cast<uint16_t>(0x1115);
    const uint16_t expected_hi = static_cast<uint16_t>(0x2225);

    bool ok = (digits[0] == expected_lo) && (digits[1] == expected_hi);
    if (!ok) {
        log_fail(test_name, "digits do not match expected after add across two limbs with in_digits>0");
        std::cerr << "Expected: [" << std::hex << expected_hi << " " << expected_lo << "]\n"
                  << "Got:      [" << digits[1] << " " << digits[0] << "]\n";
    } else {
        log_pass(test_name);
    }
    return ok;
}

int main()
{
    // Run tests
    bool t1 = test_add32_in_digits_zero_sets_full_value();
    bool t2 = test_add32_in_digits_positive_distributes_across_limbs();

    // Summary
    std::cout << "\nSummary: " << (t1 ? "OK" : "FAIL") << " / "
              << (t1 ? 1 : 0) << " | " << (t2 ? "OK" : "FAIL") << " / "
              << (t2 ? 1 : 0) << "\n";
    if (g_failures > 0) {
        std::cerr << "Total failures: " << g_failures << "\n";
        return 1;
    }

    // If all tests pass, return 0
    return 0;
}