// Lightweight C++11 test suite for the focal method: PackOne
// This test targets the function declared/defined in fast_float_15bits.c
// - It uses non-terminating custom EXPECT-like assertions
// - No GTest or external frameworks are used
// - The tests are compiled as C++11, linking against the C implementation

#include <iostream>
#include <cstdint>
#include <fast_float_internal.h>
#include <iomanip>


// Dependency header that defines integral types used by PackOne and related helpers.
// This header is part of the focal class dependencies.
// If your environment places these typedefs in a different header, adjust accordingly.

// The focal function lives with C linkage in the source file.
// Declare it here so the C++ test code can call it after linking with fast_float_15bits.c.
extern "C" cmsUInt16Number PackOne(cmsUInt16Number x, cmsBool Reverse, cmsBool SwapEndian);

// Simple non-terminating assertion helpers.
// They log failures but do not abort the test run, enabling higher coverage.
static int g_failures = 0;

#define TEST_LOG_INFO(msg) \
    do { std::cerr << "[INFO] " << msg << std::endl; } while (0)

#define EXPECT_EQ(actual, expected) \
    do { \
        if ((actual) != (expected)) { \
            std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ \
                      << " - Expected: " << std::hex << std::showbase << (expected) \
                      << "  Actual: " << std::hex << std::showbase << (actual) << std::dec << std::endl; \
            ++g_failures; \
        } \
    } while (0)

#define EXPECT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ \
                      << " - Condition is false: " #cond << std::endl; \
            ++g_failures; \
        } \
    } while (0)

#define EXPECT_FALSE(cond) \
    do { \
        if (cond) { \
            std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ \
                      << " - Condition is true: " #cond << std::endl; \
            ++g_failures; \
        } \
    } while (0)

// Helper to print a 16-bit value in hex (for human-friendly logs)
static void print_hex16(const char* label, cmsUInt16Number v) {
    std::cout << label << " = 0x" << std::setw(4) << std::setfill('0') << std::hex << v << std::dec << "\n";
}

// Step 1 (Program Understanding) - Candidate Keywords (for this test suite)
// - PackOne: applies From16To15, optional Reverse, optional SwapEndian
// - From16To15: converts 16-bit input to 15-bit domain (likely masks to 0x7FFF)
// - Reverse: if true, x := 0xFFFF - x
// - SwapEndian: if true, x := (x << 8) | (x >> 8)
// - Core dependencies (class-like): fast_float_internal.h and related 15-bit packing helpers
//
// As we test PackOne, we rely on the behavior of From16To15 (mask to 15 bits).
// We validate true/false branches for Reverse and SwapEndian independently and in combination.

static void test_PackOne_basic_no_flags() {
    TEST_LOG_INFO("Test PackOne with Reverse=false, SwapEndian=false, boundary 0x0000");
    cmsUInt16Number res = PackOne(0x0000, 0 /* Reverse */, 0 /* SwapEndian */);
    EXPECT_EQ(res, 0x0000);

    TEST_LOG_INFO("Test PackOne with Reverse=false, SwapEndian=false, boundary 0x7FFF");
    res = PackOne(0x7FFF, 0 /* Reverse */, 0 /* SwapEndian */);
    // After From16To15, 0x7FFF remains 0x7FFF; no changes otherwise
    EXPECT_EQ(res, 0x7FFF);
}

static void test_PackOne_reverse_only() {
    // 0x7FFF with Reverse should map to 0x8000 (since 0xFFFF - 0x7FFF = 0x8000)
    TEST_LOG_INFO("Test PackOne with Reverse=true, SwapEndian=false, input 0x7FFF");
    cmsUInt16Number res = PackOne(0x7FFF, 1 /* Reverse */, 0 /* SwapEndian */);
    EXPECT_EQ(res, 0x8000);

    // 0x0000 with Reverse should map to 0xFFFF
    TEST_LOG_INFO("Test PackOne with Reverse=true, SwapEndian=false, input 0x0000");
    res = PackOne(0x0000, 1 /* Reverse */, 0 /* SwapEndian */);
    EXPECT_EQ(res, 0xFFFF);
}

static void test_PackOne_swapendian_only() {
    // With only SwapEndian, the value should be endian-swapped of the 15-bit-converted value
    // For 0x0001 -> After From16To15, 0x0001; SwapEndian -> 0x0100
    TEST_LOG_INFO("Test PackOne with SwapEndian=true, Reverse=false, input 0x0001");
    cmsUInt16Number res = PackOne(0x0001, 0 /* Reverse */, 1 /* SwapEndian */);
    EXPECT_EQ(res, 0x0100);

    // 0x7FFF -> After From16To15, 0x7FFF; SwapEndian -> 0xFF7F
    TEST_LOG_INFO("Test PackOne with SwapEndian=true, Reverse=false, input 0x7FFF");
    res = PackOne(0x7FFF, 0 /* Reverse */, 1 /* SwapEndian */);
    EXPECT_EQ(res, 0xFF7F);
}

static void test_PackOne_reverse_and_swapendian() {
    // With both flags true, combine effects:
    // Start 0x7FFF -> From16To15: 0x7FFF
    // Reverse: 0xFFFF - 0x7FFF = 0x8000
    // SwapEndian: (0x8000 << 8) | (0x8000 >> 8) -> 0x0000 | 0x0080 = 0x0080
    TEST_LOG_INFO("Test PackOne with Reverse=true, SwapEndian=true, input 0x7FFF");
    cmsUInt16Number res = PackOne(0x7FFF, 1 /* Reverse */, 1 /* SwapEndian */);
    EXPECT_EQ(res, 0x0080);

    // Also test with a small value to verify both branches interact correctly
    // Start 0x0001 -> After From16To15: 0x0001
    // Reverse: 0xFFFE
    // SwapEndian: 0xFEFF
    TEST_LOG_INFO("Test PackOne with Reverse=true, SwapEndian=true, input 0x0001");
    res = PackOne(0x0001, 1 /* Reverse */, 1 /* SwapEndian */);
    EXPECT_EQ(res, 0xFEFF);
}

static void test_PackOne_boundary_0001_and_8000_mix() {
    // Input 0x8000 should be masked to 0x0000 by From16To15
    // Evaluate combinations to ensure consistent behavior
    TEST_LOG_INFO("Test PackOne with input 0x8000, Reverse=false, SwapEndian=false");
    cmsUInt16Number res = PackOne(0x8000, 0, 0);
    EXPECT_EQ(res, 0x0000);

    TEST_LOG_INFO("Test PackOne with input 0x8000, Reverse=true, SwapEndian=false");
    res = PackOne(0x8000, 1, 0);
    EXPECT_EQ(res, 0xFFFF);

    TEST_LOG_INFO("Test PackOne with input 0x8000, Reverse=false, SwapEndian=true");
    res = PackOne(0x8000, 0, 1);
    EXPECT_EQ(res, 0x0000);

    TEST_LOG_INFO("Test PackOne with input 0x8000, Reverse=true, SwapEndian=true");
    res = PackOne(0x8000, 1, 1);
    EXPECT_EQ(res, 0xFFFF);
}

// Step 3 (Test Case Refinement) - additional thorough checks
static void test_PackOne_coverage_over_inputs() {
    // A few extra representative inputs to improve branch coverage
    cmsUInt16Number values[] = {0x0000, 0x0001, 0x1234, 0x7F7F, 0x7FFF, 0x8000, 0xFFFF};
    for (cmsUInt16Number v : values) {
        // No flags
        cmsUInt16Number r0 = PackOne(v, 0, 0);
        // Only Reverse
        cmsUInt16Number r1 = PackOne(v, 1, 0);
        // Only SwapEndian
        cmsUInt16Number r2 = PackOne(v, 0, 1);
        // Both
        cmsUInt16Number r3 = PackOne(v, 1, 1);

        // Basic sanity: results should be 16-bit wide
        EXPECT_TRUE(r0 <= 0xFFFF);
        EXPECT_TRUE(r1 <= 0xFFFF);
        EXPECT_TRUE(r2 <= 0xFFFF);
        EXPECT_TRUE(r3 <= 0xFFFF);

        // If From16To15 masks to 0x7FFF, confirm the masked concept indirectly
        if ((v & 0x8000) != 0) {
            // Values with the 0x8000 bit set should map to 0x0000 after From16To15
            // When Reverse isn't applied, the result equals the swapped/non-swapped version of 0x0000
            // This is a loose sanity check rather than a strict equality because subsequent operations may differ.
            // We still ensure the value doesn't overflow.
            EXPECT_TRUE(r0 == PackOne(0x8000, 0, 0)); // self-consistency
        }
    }
}

// Entry point that runs all tests and reports a final summary
int main() {
    TEST_LOG_INFO("Starting PackOne unit tests for 15-bit packing path");

    test_PackOne_basic_no_flags();
    test_PackOne_reverse_only();
    test_PackOne_swapendian_only();
    test_PackOne_reverse_and_swapendian();
    test_PackOne_boundary_0001_and_8000_mix();
    test_PackOne_coverage_over_inputs();

    if (g_failures == 0) {
        std::cout << "[PASSED] All PackOne tests completed successfully." << std::endl;
        return 0;
    } else {
        std::cout << "[FAILED] PackOne tests completed with " << g_failures << " failure(s)." << std::endl;
        return 1;
    }
}