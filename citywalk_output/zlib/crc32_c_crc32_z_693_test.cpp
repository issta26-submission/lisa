// Test suite for crc32_z function from crc32.c
// This test uses a lightweight, self-contained harness (no GTest) and
// performs a set of checks to ensure correctness and branch coverage.
//
// Note: The tests rely on the external C function:
//   unsigned long ZEXPORT crc32_z(unsigned long crc, const unsigned char *buf, size_t len);
// and the standard types/macros (Z_NULL, z_size_t, etc.) provided by the project.
// To minimize dependencies, we declare a C linkage prototype using size_t for the
// length parameter. The tests compare against a reference CRC-32 implementation
// written in pure C++ (bit-by-bit method) to validate correctness.

#include <cstring>
#include <stdatomic.h>
#include <string>
#include <cstdint>
#include <cstddef>
#include <iostream>
#include <zutil.h>


// Compatibility macros (in case ZEXPORT is not defined in test compile env)
#ifndef ZEXPORT
#define ZEXPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Prototype for the focal crc32_z function (externally linked from crc32.c)
unsigned long ZEXPORT crc32_z(unsigned long crc, const unsigned char* buf, size_t len);

#ifdef __cplusplus
}
#endif

// Lightweight reference CRC-32 implementation (IEEE 802.3, polynomial 0xEDB88320)
static uint32_t crc32_ref(uint32_t crc, const unsigned char* buf, size_t len) {
    // Initial and final XOR with 0xFFFFFFFF to match standard CRC32 behavior.
    crc = ~crc;
    for (size_t i = 0; i < len; ++i) {
        crc ^= static_cast<uint32_t>(buf[i]);
        for (int b = 0; b < 8; ++b) {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320U;
            else
                crc >>= 1;
        }
    }
    return ~crc;
}

// Simple test harness
static int g_total = 0;
static int g_failed = 0;

static void test_passed(const std::string& name) {
    std::cout << "[OK] " << name << "\n";
}
static void test_failed(const std::string& name, const std::string& reason) {
    std::cerr << "[FAILED] " << name << ": " << reason << "\n";
    g_failed++;
}
static void expect_eq_u32(const std::string& test_name, uint32_t expected, uint32_t actual) {
    g_total++;
    if (expected != actual) {
        char buf[128];
        std::snprintf(buf, sizeof(buf),
                      "expected 0x%08X, got 0x%08X",
                      static_cast<unsigned int>(expected),
                      static_cast<unsigned int>(actual));
        test_failed(test_name, std::string(buf));
    } else {
        test_passed(test_name);
    }
}
static void expect_true(const std::string& test_name, bool cond) {
    g_total++;
    if (!cond) {
        test_failed(test_name, "condition is false");
    } else {
        test_passed(test_name);
    }
}
static void report_summary() {
    std::cout << "\nTest Summary: " << g_total << " tests, "
              << (g_failed ? std::to_string(g_failed) + " failures" : "0 failures")
              << "\n";
}

// Test 1: Null buffer should return 0 (true branch: buf == Z_NULL)
static void test_null_buffer_returns_zero() {
    // The termination condition in crc32_z should trigger early return when buf is null.
    unsigned long res = crc32_z(0, nullptr, 10);
    // Expect 0 per the early return contract.
    expect_eq_u32("crc32_z/null-buffer should return 0", 0u, static_cast<uint32_t>(res));
}

// Test 2: Empty input (non-null pointer, zero length) should yield 0
static void test_empty_input_yields_zero() {
    const unsigned char data[] = "Hi"; // data content irrelevant since len == 0
    unsigned long res = crc32_z(0, data, 0);
    expect_eq_u32("crc32_z/empty-input yields 0", 0u, static_cast<uint32_t>(res));
}

// Test 3: Known string "Hello" yields CRC-32 = 0x3610A686
static void test_known_hello() {
    const unsigned char data[] = "Hello";
    unsigned long res = crc32_z(0, data, 5);

    // Compare against reference implementation
    uint32_t expected = crc32_ref(0, data, 5);
    expect_eq_u32("crc32_z/Hello == CRC32(\"Hello\")", expected, static_cast<uint32_t>(res));
    // Also verify against the well-known CRC32 value for "Hello"
    // 0x3610A686 is the commonly cited CRC32 for "Hello" with initial 0.
    // Use either the reference value or explicit literal for redundancy.
    if (static_cast<uint32_t>(res) == 0x3610A686u) {
        test_passed("crc32_z/Hello matches known constant 0x3610A686");
    } else {
        test_failed("crc32_z/Hello matches known constant 0x3610A686",
                    "value did not match 0x3610A686");
    }
}

// Test 4: Known string "123456789" yields CRC-32 = 0xCBF43926
static void test_known_123456789() {
    const unsigned char data[] = "123456789";
    unsigned long res = crc32_z(0, data, 9);
    uint32_t expected = crc32_ref(0, data, 9);
    expect_eq_u32("crc32_z/123456789 == CRC32(\"123456789\")", expected, static_cast<uint32_t>(res));
    if (static_cast<uint32_t>(res) == 0xCBF43926u) {
        test_passed("crc32_z/123456789 matches known constant 0xCBF43926");
    } else {
        test_failed("crc32_z/123456789 matches known constant 0xCBF43926",
                    "value did not match 0xCBF43926");
    }
}

// Test 5: Incremental processing should match full data processing
// Verify that processing "Hello" then "World" in two calls yields the same result as processing "HelloWorld" in one call.
static void test_incremental_vs_full() {
    const unsigned char part1[] = "Hello";
    const unsigned char part2[] = "World";
    const unsigned char full[]  = "HelloWorld";

    unsigned long res_full = crc32_z(0, full, 10);
    unsigned long crc_after_part1 = crc32_z(0, part1, 5);
    unsigned long res_incremental = crc32_z(crc_after_part1, part2, 5);

    expect_eq_u32("crc32_z/incremental_vs_full", static_cast<uint32_t>(res_full),
                  static_cast<uint32_t>(res_incremental));
}

// Test 6: Random data sanity check against reference implementation
static void test_random_data_against_reference() {
    const unsigned char data[] = {0x00, 0xFF, 0x12, 0x34, 0x56, 0x78, 0x9A, 0xBC, 0xDE, 0xF0};
    size_t len = sizeof(data);
    unsigned long res = crc32_z(0, data, len);
    uint32_t expected = crc32_ref(0, data, len);
    expect_eq_u32("crc32_z/random_data == CRC32(reference)", expected, static_cast<uint32_t>(res));

    // Also try with a non-zero initial CRC to ensure proper incremental behavior
    unsigned long res_with_init = crc32_z(0xFFFFFFFF, data, len);
    uint32_t expected_with_init = crc32_ref(0xFFFFFFFF, data, len);
    expect_eq_u32("crc32_z/random_data_with_init == CRC32(reference with init)",
                  expected_with_init, static_cast<uint32_t>(res_with_init));
}

int main() {
    // Run all tests
    test_null_buffer_returns_zero();
    test_empty_input_yields_zero();
    test_known_hello();
    test_known_123456789();
    test_incremental_vs_full();
    test_random_data_against_reference();

    // Report results
    report_summary();

    // Return non-zero if any tests failed
    return (g_failed == 0) ? 0 : 1;
}