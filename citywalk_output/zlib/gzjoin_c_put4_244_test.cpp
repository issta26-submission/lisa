/*
Unit test suite for the focal method put4(unsigned long val, FILE *out)
- Purpose: verify that put4 writes the 4 least-significant bytes of val to the output
  stream in little-endian order (LSB first): byte0 = val & 0xff, byte1 = (val>>8)&0xff, etc.
- Notes:
  - This test suite assumes the symbol put4 is accessible (extern "C") in the test
    environment. If the original code marks put4 as static (file-scoped), this test
    will not link against it. The tests are written to work when put4 has external linkage.
  - Tests use tmpfile() to capture binary output reliably across platforms that support it.
  - No GoogleTest or other frameworks are used; a small in-house test harness is provided.
  - All tests attempt to continue executing even if one fails to maximize coverage.
*/

#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <limits>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>


// Declaration of the focal method (assumed to be externally linked in the tested project)
extern "C" void put4(unsigned long val, FILE *out);

static bool test_put4_with_value(unsigned long val)
{
    // Create a temporary file to capture the 4-byte output
    FILE *f = tmpfile();
    if (!f) {
        std::cerr << "Failed to create temporary file for value 0x" << std::hex << val << std::dec << "\n";
        return false;
    }

    // Call the focal method
    put4(val, f);

    // Read back the 4 bytes written
    rewind(f);
    unsigned char buf[4] = {0, 0, 0, 0};
    size_t read_bytes = fread(buf, 1, 4, f);
    fclose(f);

    if (read_bytes != 4) {
        std::cerr << "Expected 4 bytes written for value 0x" << std::hex << val << std::dec
                  << ", but read " << read_bytes << "\n";
        return false;
    }

    // Compute expected bytes: little-endian order
    unsigned char expected[4];
    expected[0] = static_cast<unsigned char>(val & 0xff);
    expected[1] = static_cast<unsigned char>((val >> 8) & 0xff);
    expected[2] = static_cast<unsigned char>((val >> 16) & 0xff);
    expected[3] = static_cast<unsigned char>((val >> 24) & 0xff);

    // Compare byte-by-byte (address/content equality)
    bool ok = std::memcmp(buf, expected, 4) == 0;
    if (!ok) {
        std::cerr << "Mismatch for value 0x" << std::hex << val << std::dec << ": "
                  << "expected {"
                  << +expected[0] << ", " << +expected[1] << ", "
                  << +expected[2] << ", " << +expected[3] << "}, "
                  << "got {"
                  << +buf[0] << ", " << +buf[1] << ", "
                  << +buf[2] << ", " << +buf[3] << "}\n";
    }
    return ok;
}

// Test 1: Basic 32-bit value with distinct bytes to verify LSB-first ordering
static bool test_put4_basic_endianness()
{
    // Value 0x01020304 => expected bytes {0x04, 0x03, 0x02, 0x01}
    unsigned long val = 0x01020304UL;
    return test_put4_with_value(val);
}

// Test 2: Another 32-bit value with different byte pattern
static bool test_put4_another_pattern()
{
    // Value 0x12345678 => expected bytes {0x78, 0x56, 0x34, 0x12}
    unsigned long val = 0x12345678UL;
    return test_put4_with_value(val);
}

// Test 3: Boundary value 0 => all bytes should be zero
static bool test_put4_zero()
{
    unsigned long val = 0x0UL;
    return test_put4_with_value(val);
}

// Test 4: Maximum 32-bit pattern in 4-byte span: 0xFFFFFFFF
static bool test_put4_all_ones()
{
    unsigned long val = 0xFFFFFFFFUL;
    return test_put4_with_value(val);
}

// Test 5 (optional on platforms with 64-bit unsigned long): verify lower 4 bytes are written
// when val exceeds 32-bit range (e.g., 0x100000000). If unsigned long is 32-bit, this test is skipped.
static bool test_put4_with_high_bits_truncated()
{
#if ULONG_MAX > 0xFFFFFFFFUL
    // Use a value whose lower 32 bits are 0x00000000 and higher bits are 0x00000001
    // Expected 4-byte output: {0x00, 0x00, 0x00, 0x01}
    unsigned long val = 0x100000000UL;
    return test_put4_with_value(val);
#else
    // Environment does not allow testing with >32-bit unsigned long
    return true; // considered passed as not applicable
#endif
}

// Simple test harness: run all tests and print a summary
int main(int argc, char **argv)
{
    (void)argc; (void)argv; // suppress unused parameter warnings

    int total = 0;
    int passed = 0;

    struct TestEntry {
        const char* name;
        bool (*fn)();
    };

    TestEntry tests[] = {
        { "test_put4_basic_endianness", test_put4_basic_endianness },
        { "test_put4_another_pattern", test_put4_another_pattern },
        { "test_put4_zero", test_put4_zero },
        { "test_put4_all_ones", test_put4_all_ones },
        { "test_put4_with_high_bits_truncated", test_put4_with_high_bits_truncated },
    };

    const int ntests = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < ntests; ++i) {
        total++;
        bool ok = false;
        try {
            ok = tests[i].fn();
        } catch (...) {
            ok = false;
        }
        if (ok) {
            passed++;
            std::cout << "[PASS] " << tests[i].name << "\n";
        } else {
            std::cout << "[FAIL] " << tests[i].name << "\n";
        }
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}