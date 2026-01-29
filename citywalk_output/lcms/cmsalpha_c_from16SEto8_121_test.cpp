/*
Unit test suite for focal method: from16SEto8
Context:
- Method under test is in cmsalpha.c:
  void from16SEto8(void* dst, const void* src)
  {
  {
      cmsUInt16Number n = *(cmsUInt16Number*)src;
      *(cmsUInt8Number*)dst = FROM_16_TO_8(CHANGE_ENDIAN(n));
  }
  }

- Core behavior (based on typical Little CMS macros):
  - Reads a 16-bit value n from src (host endianness applies).
  - Reverses byte order: CHANGE_ENDIAN(n) -> swap high/low bytes.
  - Converts 16-bit value to 8-bit using FROM_16_TO_8.
  - Writes a single 8-bit value to dst.

- Testing approach:
  - Since macros CHANGE_ENDIAN and FROM_16_TO_8 are not exposed here, we simulate expectations using the known endianness behavior:
    expected = high byte of the swapped value, which is equivalent to the low byte of the original 16-bit value stored at src when viewed on typical little-endian hosts.
  - We test multiple input byte pairs ensuring both typical and edge values are covered.
  - We also verify that only dst[0] is written and dst[1] remains unchanged, validating the non-overwriting behavior.

Notes:
- This test is written in standard C++11 (no GTest). It imports the focal function via an extern "C" declaration.
- It uses a small non-terminating assertion approach: tests continue even if a case fails, and a summary is emitted at the end.
- The test relies on typical host endianness (x86-like) for deterministic interpretation of two-byte sequences as a 16-bit value. This aligns with how the focal method reads cmsUInt16Number from a void* src.

Usage:
- Compile with a C++11 compiler and link against the cmsalpha.c (or its object) providing the implementation of from16SEto8.

*/

/* Minimal unit test harness for from16SEto8 without GTest */
#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


extern "C" void from16SEto8(void* dst, const void* src);

/* Simple non-terminating test logger */
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_PASS() \
    do { ++g_tests_run; std::cout << "[PASS] " << __FUNCTION__ << "\n"; } while(0)

#define TEST_FAIL(msg) \
    do { ++g_tests_run; ++g_tests_failed; std::cout << "[FAIL] " << __FUNCTION__ << ": " << msg << "\n"; } while(0)

#define EXPECT_EQ_UINT8(expected, actual) \
    do { \
        if (static_cast<uint8_t>(expected) != static_cast<uint8_t>(actual)) { \
            TEST_FAIL("expected " + std::to_string(static_cast<uint8_t>(expected)) + \
                      ", got " + std::to_string(static_cast<uint8_t>(actual))); \
        } else { \
            TEST_PASS(); \
        } \
    } while(0)

#define EXPECT_EQ_PTR(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            TEST_FAIL("pointer mismatch"); \
        } else { \
            TEST_PASS(); \
        } \
    } while(0)

/* Test 1: Basic conversion with distinct bytes
   - src bytes: {0xAB, 0xCD}
   - Expected dst[0] = 0xAB (low byte of original n after endianness swap)
   - dst[1] should remain unchanged
*/
static void test_from16SEto8_basic_case1() {
    uint8_t dst[2] = { 0x00, 0x5A };  // dst[1] should remain unchanged
    uint8_t src[2] = { 0xAB, 0xCD }; // n = 0xCDAB on little-endian host

    from16SEto8(dst, src);

    // Expected: dst[0] == 0xAB, dst[1] == 0x5A
    if (dst[0] != 0xAB || dst[1] != 0x5A) {
        // Print a detailed diagnostic
        std::cout << "[DIAG] test_from16SEto8_basic_case1: dst = {"
                  << std::hex << int(dst[0]) << ", " << int(dst[1]) << "}\n";
        TEST_FAIL("case1 mismatch");
    } else {
        TEST_PASS();
    }
}

/* Test 2: Edge case with zero value
   - src bytes: {0x00, 0x01}  -> n = 0x0100
   - After endianness swap: 0x0001, high byte 0x00
   - dst[1] should remain unchanged
*/
static void test_from16SEto8_basic_case2() {
    uint8_t dst[2] = { 0x77, 0x88 };
    uint8_t src[2] = { 0x00, 0x01 };

    from16SEto8(dst, src);

    // Expected: dst[0] == 0x00, dst[1] == 0x88
    if (dst[0] != 0x00 || dst[1] != 0x88) {
        std::cout << "[DIAG] test_from16SEto8_basic_case2: dst = {"
                  << std::hex << int(dst[0]) << ", " << int(dst[1]) << "}\n";
        TEST_FAIL("case2 mismatch");
    } else {
        TEST_PASS();
    }
}

/* Test 3: Edge case with all-ones
   - src bytes: {0xFF, 0xFF}  -> n = 0xFFFF
   - After endianness swap: 0xFFFF, high byte 0xFF
*/
static void test_from16SEto8_basic_case3() {
    uint8_t dst[2] = { 0x01, 0x02 };
    uint8_t src[2] = { 0xFF, 0xFF };

    from16SEto8(dst, src);

    // Expected: dst[0] == 0xFF, dst[1] == 0x02
    if (dst[0] != 0xFF || dst[1] != 0x02) {
        std::cout << "[DIAG] test_from16SEto8_basic_case3: dst = {"
                  << std::hex << int(dst[0]) << ", " << int(dst[1]) << "}\n";
        TEST_FAIL("case3 mismatch");
    } else {
        TEST_PASS();
    }
}

int main() {
    std::cout << "Starting test suite for from16SEto8...\n";

    test_from16SEto8_basic_case1();
    test_from16SEto8_basic_case2();
    test_from16SEto8_basic_case3();

    std::cout << "Test summary: " << g_tests_run << " tests executed, "
              << g_tests_failed << " failures.\n";

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}