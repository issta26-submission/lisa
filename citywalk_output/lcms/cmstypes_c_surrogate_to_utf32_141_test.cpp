// Test suite for the focal method: surrogate_to_utf32
// This test is designed to be compiled alongside the target source (cmstypes.c)
// and linked in a C++11 environment without using GoogleTest.
// It exercises the surrogate_to_utf32 function via direct calls and asserts
// correctness using a lightweight, non-terminating assertion style.
// Notes:
// - The function surrogate_to_utf32 performs a 32-bit unsigned arithmetic:
//   (high << 10) + low - 0x035FDC00
// - We validate typical, boundary, and wrap-around scenarios to improve coverage.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>


// Candidate Keywords (for Step 1 understanding):
// - surrogate_to_utf32: core computation combining high and low parts into a UTF-32 code point.
// - 0x35fdc00: the fixed offset used in the surrogate-to-UTF-32 transformation.
// -  cmsUInt32Number: 32-bit unsigned integer type used by the CMS codebase.
// -  unsigned arithmetic wrap-around: important for validating modulo-2^32 behavior.

// Domain knowledge hints applied by tests:
// - Use 32-bit unsigned arithmetic with wrap-around semantics to reflect the actual behavior.
// - Include boundary and wrap scenarios to maximize coverage.
// - Do not rely on private/private-like internal APIs; test via the focal function prototype.
// - Provide clear comments for each test about the scenario being validated.

// We declare the focal function with C linkage to ensure proper symbol resolution
// when linked with the C-based cmstypes.c implementation.
typedef uint32_t cmsUInt32Number;

#ifdef __cplusplus
extern "C" {
#endif
// Function prototype (assumed to be available in cmstypes.c)
cmsUInt32Number surrogate_to_utf32(cmsUInt32Number high, cmsUInt32Number low);
#ifdef __cplusplus
}
#endif

// Lightweight test harness (non-terminating; accumulates results)
static int g_passes = 0;
static int g_fails  = 0;

#define CHECK_EQ(name, expected, actual) do { \
    if ((uint32_t)(expected) == (uint32_t)(actual)) { \
        ++g_passes; \
    } else { \
        ++g_fails; \
        std::cerr << "FAIL: " name " - expected 0x" \
                  << std::hex << static_cast<uint32_t>(expected) \
                  << ", got 0x" << static_cast<uint32_t>(actual) << std::dec << "\n"; \
    } \
} while (0)

// Test 1: Zero inputs to verify wrap-around behavior from the subtraction
//         0 - 0x035FDC00 under unsigned 32-bit arithmetic.
static void test_case_zero_inputs() {
    const char* name = "test_case_zero_inputs";
    cmsUInt32Number high = 0;
    cmsUInt32Number low  = 0;
    uint32_t expected = (static_cast<uint32_t>(high) << 10) + static_cast<uint32_t>(low);
    expected -= 0x035FDC00; // unsigned wrap
    cmsUInt32Number actual = surrogate_to_utf32(high, low);
    CHECK_EQ(name, expected, actual);
}

// Test 2: Typical non-wrapping region (with wrap likely due to subtraction)
static void test_case_minimal_positive_values() {
    const char* name = "test_case_minimal_positive_values";
    cmsUInt32Number high = 0x3FF; // 1023
    cmsUInt32Number low  = 0x3FF; // 1023
    uint32_t expected = (static_cast<uint32_t>(high) << 10) + static_cast<uint32_t>(low);
    expected -= 0x035FDC00;
    cmsUInt32Number actual = surrogate_to_utf32(high, low);
    CHECK_EQ(name, expected, actual);
}

// Test 3: Random values to exercise normal operation (including potential overflow in left-shift)
static void test_case_random_values() {
    const char* name = "test_case_random_values";
    cmsUInt32Number high = 123456789;
    cmsUInt32Number low  = 654321;
    uint32_t expected = (static_cast<uint32_t>(high) << 10) + static_cast<uint32_t>(low);
    expected -= 0x035FDC00;
    cmsUInt32Number actual = surrogate_to_utf32(high, low);
    CHECK_EQ(name, expected, actual);
}

// Test 4: All-ones scenario to provoke substantial wrap-around
static void test_case_all_ones_wrap() {
    const char* name = "test_case_all_ones_wrap";
    cmsUInt32Number high = 0xFFFFFFFF;
    cmsUInt32Number low  = 0xFFFFFFFF;
    // Use 64-bit intermediate to reflect behavior consistent with 32-bit unsigned wrap.
    uint64_t t = (static_cast<uint64_t>(high) << 10) + static_cast<uint64_t>(low);
    uint32_t expected = static_cast<uint32_t>(t - 0x035FDC00);
    cmsUInt32Number actual = surrogate_to_utf32(high, low);
    CHECK_EQ(name, expected, actual);
}

// Entry point
int main() {
    // Step 2/3: Generate and run the test suite
    test_case_zero_inputs();
    test_case_minimal_positive_values();
    test_case_random_values();
    test_case_all_ones_wrap();

    // Summary
    std::cout << "surrogate_to_utf32: " << g_passes << " passed, " << g_fails << " failed." << std::endl;

    // Non-terminating test approach: return non-zero if any test failed
    return (g_fails == 0) ? 0 : 1;
}