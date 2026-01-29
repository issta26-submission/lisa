// Lightweight C++11 unit test harness for the focal function:
// from_premul16(uint16_t rgb16, uint16_t a16)
// This standalone test reproduces the environment by providing a minimal
// _cmsToFixedDomain and the function under test, then exercises several
// scenarios to verify correct behavior without using GTest.

#include <math.h>
#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <iomanip>
#include <lcms2_internal.h>
#include <cstdint>


// Forward declaration of helper (mock) used by from_premul16.
// In the original project this would be provided by the library header.
// We implement a simple pass-through to enable deterministic, testable behavior.
static inline uint32_t _cmsToFixedDomain(uint16_t a16) {
    // Minimal mock: treat a16 as the fixed-domain factor directly.
    // This mirrors the usage pattern in the focal method.
    return static_cast<uint32_t>(a16);
}

// Focal method under test copied and adapted for the test environment.
// It computes the "from premultiplied 16-bit" color channel value.
uint16_t from_premul16(uint16_t rgb16, uint16_t a16)
{
{
    uint32_t alpha_factor, rgb;
    if (a16 == 0) return rgb16;
    alpha_factor = _cmsToFixedDomain(a16);    
    rgb = (((uint32_t) rgb16) << 16) / alpha_factor;
    if (rgb > 0xffff) rgb = 0xffff;
    return (uint16_t)rgb;
}
}

// Simple test harness helpers
static int g_failures = 0;

// Print hex helper for readability
static inline std::string to_hex(uint32_t v, int width = 4) {
    std::ostringstream oss;
    oss << "0x" << std::setfill('0') << std::setw(width) << std::hex << std::uppercase << (v & 0xFFFF);
    return oss.str();
}

// Non-terminating assertion macro: reports failure but continues execution.
#define EXPECT_EQ(expected, actual, msg) do { \
    if ((uint32_t)(expected) != (uint32_t)(actual)) { \
        std::cerr << "FAIL: " << msg << " | expected " << to_hex((expected), 4) \
                  << ", got " << to_hex((actual), 4) << "\n"; \
        ++g_failures; \
    } \
} while (0)

// Test 1: a16 == 0 should return rgb16 unchanged (base case)
void test_from_premul16_alpha_zero() {
    // Candidate rgb16 values exercising typical, boundary, and max cases
    const uint16_t test_values[] = { 0x0000, 0x1234, 0xABCD, 0xFFFF };
    for (uint16_t rgb16 : test_values) {
        uint16_t result = from_premul16(rgb16, 0);
        EXPECT_EQ(rgb16, result, "a16=0 must return input rgb16 unchanged");
    }
}

// Test 2: a16 == 1 (very small alpha factor) should yield premultiplied result:
// - rgb16 == 0 => 0
// - rgb16 > 0  => 0xFFFF (clamped because rgb = rgb16 << 16)
void test_from_premul16_alpha_one() {
    EXPECT_EQ(0x0000, from_premul16(0x0000, 1), "a16=1, rgb16=0 should be 0");
    EXPECT_EQ(0xFFFF, from_premul16(0x0001, 1), "a16=1, rgb16>0 should clamp to 0xFFFF");
    EXPECT_EQ(0xFFFF, from_premul16(0xABCD, 1), "a16=1, rgb16>0 should clamp to 0xFFFF");
    EXPECT_EQ(0xFFFF, from_premul16(0xFFFF, 1), "a16=1, rgb16 max should clamp to 0xFFFF");
}

// Test 3: a16 == 0x8000 (half of the domain) behaves like rgb = rgb16 * 2 with clamping
void test_from_premul16_alpha_half() {
    // rgb16 = 0 => 0
    EXPECT_EQ(0x0000, from_premul16(0x0000, 0x8000), "a16=0x8000, rgb16=0 should be 0");
    // rgb16 = 1 => 2
    EXPECT_EQ(0x0002, from_premul16(0x0001, 0x8000), "a16=0x8000, rgb16=1 should be 2");
    // rgb16 = 0x7FFF => 0xFFFE
    EXPECT_EQ(0xFFFE, from_premul16(0x7FFF, 0x8000), "a16=0x8000, rgb16=0x7FFF should be 0xFFFE");
    // rgb16 = 0x8000 => 0xFFFF (clamped)
    EXPECT_EQ(0xFFFF, from_premul16(0x8000, 0x8000), "a16=0x8000, rgb16=0x8000 should be 0xFFFF");
    // rgb16 = 0xFFFF => 0xFFFF (clamped)
    EXPECT_EQ(0xFFFF, from_premul16(0xFFFF, 0x8000), "a16=0x8000, rgb16=0xFFFF should be 0xFFFF");
}

// Test 4: a16 == 0xFFFF (full domain) should return rgb16 unchanged due to multiplier ~1
// Considered values: 0, 1, 0x1234, 0xFFFF
void test_from_premul16_alpha_full() {
    // In this setup, floor((rgb16 << 16) / 65535) equals rgb16 for the tested values
    EXPECT_EQ(0x0000, from_premul16(0x0000, 0xFFFF), "a16=0xFFFF, rgb16=0 should be 0");
    EXPECT_EQ(0x0001, from_premul16(0x0001, 0xFFFF), "a16=0xFFFF, rgb16=1 should be 1");
    EXPECT_EQ(0x1234, from_premul16(0x1234, 0xFFFF), "a16=0xFFFF, rgb16=0x1234 should be 0x1234");
    EXPECT_EQ(0xFFFF, from_premul16(0xFFFF, 0xFFFF), "a16=0xFFFF, rgb16=0xFFFF should be 0xFFFF");
}

// Entry point: run all test cases and report summary
int main() {
    // Execute tests
    test_from_premul16_alpha_zero();
    test_from_premul16_alpha_one();
    test_from_premul16_alpha_half();
    test_from_premul16_alpha_full();

    // Summary
    std::cout << "Test completed with " << g_failures << " failure(s).\n";
    return (g_failures != 0) ? 1 : 0;
}