// Test suite for the focal method crc_word in crc32.c
// This test relies on the public CRC32 API exposed by the project (crc32.*).
// The focal method is static/private in its translation unit, so we cannot call it directly
// from tests. Instead, we validate the observable behavior that relies on crc_word
// by comparing the library's public CRC32 results against a reference CRC32 implementation.
// This provides coverage for end-to-end usage of the core logic and related code paths.

#include <cstring>
#include <cstdio>
#include <stdatomic.h>
#include <cstdint>
#include <iostream>
#include <vector>
#include <zutil.h>
#include <crc32.h>


// Import C headers with C linkage
extern "C" {
}

// Lightweight, independent reference CRC-32 (IEEE 802.3) implementation.
// This uses the standard bitwise algorithm with polynomial 0xEDB88320.
static uint32_t crc32_ref(const uint8_t* data, size_t len) {
    const uint32_t POLY = 0xEDB88320u;
    uint32_t crc = 0xFFFFFFFFu;
    for (size_t i = 0; i < len; ++i) {
        crc ^= static_cast<uint32_t>(data[i]);
        for (int b = 0; b < 8; ++b) {
            if (crc & 1u)
                crc = (crc >> 1) ^ POLY;
            else
                crc >>= 1;
        }
    }
    return crc ^ 0xFFFFFFFFu;
}

// Utility to format 32-bit value as hex string for debugging output
static std::string to_hex(uint32_t v) {
    char buf[11];
    std::snprintf(buf, sizeof(buf), "0x%08X", v);
    return std::string(buf);
}

// Simple non-terminating test assertion framework
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_fail(const std::string& desc, uint32_t expected, uint32_t actual) {
    ++g_total_tests;
    ++g_failed_tests;
    std::cerr << "[FAIL] " << desc
              << " | expected " << to_hex(expected)
              << " but got " << to_hex(actual) << "\n";
}

static void log_pass(const std::string& desc, uint32_t expected, uint32_t actual) {
    ++g_total_tests;
    // Accept a pass if actual equals expected; do not terminate test execution.
    if (actual == expected) {
        std::cout << "[PASS] " << desc
                  << " (expected " << to_hex(expected) << ", got " << to_hex(actual) << ")\n";
    } else {
        log_fail(desc, expected, actual);
    }
}

// Helper to run a single test against the CRC32 public API
static void run_crc32_test(const unsigned char* data, size_t len, const std::string& description) {
    uint32_t expected = crc32_ref(data, len);
    unsigned long actual = crc32(0, data, static_cast<uInt>(len)); // public API: initial CRC 0
    log_pass(description, expected, static_cast<uint32_t>(actual));
}

// Test 1: Empty input should yield CRC of 0 (with the standard CRC32 initialization)
static void test_empty_input() {
    // Empty payload
    const unsigned char empty[] = {};
    uint32_t expected = crc32_ref(empty, 0);
    unsigned long actual = crc32(0, empty, 0);
    std::string desc = "crc32 with empty input yields 0 (ref=" + to_hex(expected) + ", got=" + to_hex(static_cast<uint32_t>(actual)) + ")";
    log_pass(desc, expected, static_cast<uint32_t>(actual));
    // Also verify against the canonical expected value 0x00000000 for empty input
    if (static_cast<uint32_t>(actual) != 0x00000000u) {
        log_fail("crc32(empty) == 0x00000000", 0x00000000u, static_cast<uint32_t>(actual));
    } else {
        ++g_total_tests;
        std::cout << "[PASS] crc32(empty) == 0x00000000\n";
    }
}

// Test 2: Known ASCII input "123456789" -> canonical CRC-32 value 0xCBF43926
static void test_known_ascii_123456789() {
    const unsigned char data[] = "123456789";
    uint32_t expected = 0xCBF43926u;
    unsigned long actual = crc32(0, data, 9);
    log_pass("crc32(\"123456789\") matches canonical 0xCBF43926", expected, static_cast<uint32_t>(actual));
}

// Test 3: Known ASCII input "The quick brown fox..." -> 0x414FA339
static void test_known_ascii_quick_brown() {
    const unsigned char data[] =
        "The quick brown fox jumps over the lazy dog";
    uint32_t expected = 0x414FA339u;
    unsigned long actual = crc32(0, data, std::strlen(reinterpret_cast<const char*>(data)));
    log_pass("crc32(\"The quick brown fox jumps over the lazy dog\") matches canonical 0x414FA339",
             expected, static_cast<uint32_t>(actual));
}

// Test 4: Binary data input (non-text bytes)
static void test_binary_data() {
    const unsigned char data[] = { 0x00, 0x01, 0x02, 0xFF, 0xFE, 0x80 };
    uint32_t expected = crc32_ref(data, sizeof(data));
    unsigned long actual = crc32(0, data, sizeof(data));
    std::string desc = "crc32(binary data {00 01 02 FF FE 80}) matches reference";
    log_pass(desc, expected, static_cast<uint32_t>(actual));
}

// Test 5: Incremental computation vs single-shot for "123456789"
static void test_incremental_vs_single_shot() {
    const unsigned char part1[] = "12345";
    const unsigned char part2[] = "6789";
    // Single-shot
    const unsigned char full[] = "123456789";
    uint32_t expected = crc32_ref(full, 9);

    // Incremental: crc32(0, part1, 5) -> crc32(prev, part2, 4)
    unsigned long crc1 = crc32(0, part1, 5);
    unsigned long crc2 = crc32(crc1, part2, 4);

    std::string desc = "crc32 incremental vs single-shot for '123456789'";
    log_pass(desc, expected, static_cast<uint32_t>(crc2));

    // Also ensure incremental diverges from single-shot when fed partial data incorrectly
    // (This is a sanity check; we expect match for proper incremental processing)
    if (static_cast<uint32_t>(crc2) != expected) {
        log_fail("crc32 incremental should equal single-shot for identical data", expected, static_cast<uint32_t>(crc2));
    } else {
        ++g_total_tests;
        std::cout << "[PASS] incremental-into-series matches single-shot\n";
    }
}

// Test 6: Large payload to exercise multiple bytes processing
static void test_large_payload() {
    // Create 1024 bytes of 'A'
    std::vector<unsigned char> large(1024, 'A');
    for (size_t i = 0; i < large.size(); ++i) large[i] = static_cast<unsigned char>('A');
    uint32_t expected = crc32_ref(large.data(), large.size());
    unsigned long actual = crc32(0, large.data(), static_cast<uInt>(large.size()));
    log_pass("crc32(1024 bytes of 'A') matches reference", expected, static_cast<uint32_t>(actual));
}

// Main entry point: run all tests
int main() {
    // Explain intent (as comments) for maintainers
    // Step 2/3 alignment: We exercise the observable API to validate the internal crc_word behavior
    // through end-to-end CRC32 results, including chunked processing and large payloads.

    test_empty_input();
    test_known_ascii_123456789();
    test_known_ascii_quick_brown();
    test_binary_data();
    test_incremental_vs_single_shot();
    test_large_payload();

    // Summary
    std::cout << "\nTest Summary: " << g_total_tests << " tests executed, "
              << g_failed_tests << " failures\n";

    // Return non-zero if any test failed
    return (g_failed_tests == 0) ? 0 : 1;
}