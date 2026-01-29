/*
Unit test suite for the focal method crc32_combine_op (Step 2 focused on crc32.c).

Context and dependencies (as inferred from the focal class/file section):
- focal function under test: crc32_combine_op(uLong crc1, uLong crc2, uLong op)
- used internal helper: multmodp(z_crc_t a, z_crc_t b) (static in the CRC file)
- polynomial constant: POLY = 0xedb88320
- dependencies include zutil.h and CRC-related types (uLong, z_crc_t, etc.)
- The test targets true/false branches reachable via the inputs to crc32_combine_op and its internal multmodp
- This test is written to compile in a C++11 project without GTest; a lightweight harness is provided.

Notes:
- We treat crc32_combine_op as an extern "C" function (as it is intended to be in C) and call it from C++.
- To verify correctness without duplicating internal static logic, we implement an independent reference for multmodp in the test (ref_multmodp) that mirrors the same bitwise algorithm but resides in the test code. Then we validate crc32_combine_op(op, crc1, crc2) by comparing to ref_multmodp(op, crc1) ^ (crc2 & 0xffffffff).
- We exercise several cases to cover code paths in multmodp (via different bit patterns in op and crc1) and ensure the lower 32 bits of crc2 are XORed as expected.

Compilation note:
- Build with both crc32.c (containing crc32_combine_op and multmodp) and this test file in the same project/target to resolve symbols correctly.

Code (test harness with explanations for each test):
*/

#include <limits>
#include <stdatomic.h>
#include <cstdint>
#include <iostream>
#include <zutil.h>
#include <iomanip>


extern "C" {
    // Declaration of the focal function under test.
    // The actual implementation resides in crc32.c.
    unsigned long crc32_combine_op(unsigned long crc1, unsigned long crc2, unsigned long op);
}

/*
Reference implementation for multmodp to enable independent verification
of crc32_combine_op results. This mirrors the internal algorithm used by
crc32.c (bit-oriented polynomial reduction with POLY 0xedb88320).
This is used solely in tests to compute an expected value against which
crc32_combine_op is compared.
*/
static const unsigned long POLY = 0xEDB88320UL;

static unsigned long ref_multmodp(unsigned long a, unsigned long b) {
    unsigned long m = 1UL << 31; // MSB first
    unsigned long p = 0;
    for (;;) {
        if (a & m) {
            p ^= b;
            if ((a & (m - 1)) == 0)
                break;
        }
        m >>= 1;
        // Reduce b when the least significant bit is 1
        b = (b & 1) ? (b >> 1) ^ POLY : (b >> 1);
    }
    return p;
}

// Simple test harness helpers
static bool check_equal(const char* test_name, unsigned long expected, unsigned long actual) {
    if (expected != actual) {
        std::cerr << std::hex << std::showbase
                  << "FAIL: " << test_name
                  << " - Expected 0x" << expected
                  << " but got 0x" << actual << std::dec << std::endl;
        return false;
    } else {
        std::cout << std::hex << std::showbase
                  << "PASS: " << test_name
                  << " - Value 0x" << actual << std::dec << std::endl;
        return true;
    }
}

int main() {
    int failures = 0;

    // Test 1: op = 1
    // Expected: crc32_combine_op(crc1, crc2, 1) == crc1 ^ (crc2 & 0xffffffff)
    // This tests a basic, well-defined path and masks the lower 32 bits of crc2.
    {
        const unsigned long crc1 = 0x12345678UL;
        const unsigned long crc2 = 0x9ABCDEF0UL;
        const unsigned long op   = 1UL;
        const unsigned long expected = crc1 ^ (crc2 & 0xffffffffUL);
        const unsigned long got = crc32_combine_op(crc1, crc2, op);
        if (!check_equal("crc32_combine_op(op=1) basic xor", expected, got)) {
            ++failures;
        }
    }

    // Test 2: op = 0xFFFFFFFF
    // Use a reference multmodp to compute expected, then XOR with crc2's lower 32 bits.
    {
        const unsigned long crc1 = 0x12345678UL;
        const unsigned long crc2 = 0x9ABCDEF0UL;
        const unsigned long op   = 0xFFFFFFFFUL;
        const unsigned long expected = ref_multmodp(op, crc1) ^ (crc2 & 0xffffffffUL);
        const unsigned long got = crc32_combine_op(crc1, crc2, op);
        if (!check_equal("crc32_combine_op(op=0xFFFFFFFF)", expected, got)) {
            ++failures;
        }
    }

    // Test 3: op = 0x80000001 (diverse bit pattern to exercise multmodp branches)
    {
        const unsigned long crc1 = 0xA5A5A5A5UL;
        const unsigned long crc2 = 0x5A5A5A5AUL;
        const unsigned long op   = 0x80000001UL;
        const unsigned long expected = ref_multmodp(op, crc1) ^ (crc2 & 0xffffffffUL);
        const unsigned long got = crc32_combine_op(crc1, crc2, op);
        if (!check_equal("crc32_combine_op(op=0x80000001)", expected, got)) {
            ++failures;
        }
    }

    // Test 4: op = 0x7FFFFFFF (another mixed bit pattern)
    {
        const unsigned long crc1 = 0x0F0F0F0FUL;
        const unsigned long crc2 = 0xF0F0F0F0UL;
        const unsigned long op   = 0x7FFFFFFFUL;
        const unsigned long expected = ref_multmodp(op, crc1) ^ (crc2 & 0xffffffffUL);
        const unsigned long got = crc32_combine_op(crc1, crc2, op);
        if (!check_equal("crc32_combine_op(op=0x7FFFFFFF)", expected, got)) {
            ++failures;
        }
    }

    if (failures > 0) {
        std::cerr << std::dec << failures << " test(s) failed." << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
}