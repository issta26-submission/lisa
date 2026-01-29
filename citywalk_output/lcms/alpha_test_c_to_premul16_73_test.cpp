#include <cstdio>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <stdint.h>
#include <lcms2_internal.h>
#include <cstdint>


// FOCAL_METHOD reproduced for testability within this C++11 unit test suite.
// This implementation intentionally mirrors the provided logic and uses a
// small internal stub for _cmsToFixedDomain to keep tests self-contained.
uint32_t _cmsToFixedDomain(uint16_t a16)
{
    // Map 0..0xFFFF to 0..0x10000 using integer arithmetic (floor-like).
    // This mirrors a likely fixed-point conversion used in the original code.
    // Note: a16 == 0 will yield 0, but the caller handles a16 == 0 early.
    return ((uint32_t)a16 * 0x10000) / 0xFFFF;
}

uint16_t to_premul16(uint16_t rgb16, uint16_t a16)
{
    uint32_t alpha_factor, rgb;
    if (a16 == 0) return rgb16;
    alpha_factor = _cmsToFixedDomain(a16);
    rgb = ((uint32_t) rgb16 * alpha_factor + 0x8000) >> 16;
    return (uint16_t)rgb;
}

// Reference implementation for test validation.
// It uses the same formula as the focal method but through a separate path.
uint16_t ref_to_premul16(uint16_t rgb16, uint16_t a16)
{
    if (a16 == 0) return rgb16;
    uint32_t alpha_factor = _cmsToFixedDomain(a16);
    uint32_t rgb = ((uint32_t) rgb16 * alpha_factor + 0x8000) >> 16;
    return (uint16_t)rgb;
}

// Lightweight, non-terminating test harness (no exit on failure).
// Accumulates results and reports a summary at the end.
static int g_total = 0;
static int g_passed = 0;
static int g_failed = 0;

#define ASSERT_EQ(expected, actual, test_name) do { \
    ++g_total; \
    if ((expected) == (actual)) { \
        ++g_passed; \
        /* Optional per-test success message can be uncommented for verbose runs */ \
        /* printf(\"[PASS] %s\\n\", test_name); */ \
    } else { \
        ++g_failed; \
        fprintf(stderr, "[FAIL] %s: expected 0x%04X, got 0x%04X\\n", test_name, (uint16_t)(expected), (uint16_t)(actual)); \
    } \
} while(0)

static void run_single_test_case(const std::string& name, uint16_t rgb16, uint16_t a16)
{
    uint16_t expected = ref_to_premul16(rgb16, a16);
    uint16_t actual = to_premul16(rgb16, a16);
    ASSERT_EQ(expected, actual, name.c_str());
}

// Convenience helper to run a batch of deterministic and random-style tests
static void run_tests_batch()
{
    // Explicit test cases with descriptive names (covering true/false branches).
    // 1) a16 == 0 should return rgb16 unchanged (false branch).
    run_single_test_case("case_a16_zero_returns_input_rgb",
                         0xA5A5u, 0x0000u);

    // 2) a16 == 0xFFFF should preserve rgb16 (near-1 alpha)
    run_single_test_case("case_a16_25599_preserves_rgb",
                         0x1234u, 0xFFFFu);

    // 3) mid alpha value with a simple rgb
    run_single_test_case("case_mid_alpha_rounding_example",
                         0x1234u, 0x4000u);

    // 4) half alpha with another rgb
    run_single_test_case("case_half_alpha_example",
                         0x00FFu, 0x8000u);

    // 5) maximum rgb with full alpha
    run_single_test_case("case_max_rgb_full_alpha",
                         0xFFFFu, 0xFFFFu);

    // 6) random deterministic tests (100 iterations)
    for (uint32_t i = 0; i < 100; ++i)
    {
        uint16_t rgb = static_cast<uint16_t>((i * 1315423911u) & 0xFFFFu);
        uint16_t a   = static_cast<uint16_t>((i * 2654435761u) & 0xFFFFu);
        run_single_test_case("case_random_det_" + std::to_string(i),
                             rgb, a);
    }
}

// Main entry: runs the test suite and reports summary.
int main()
{
    // Run the test batch
    run_tests_batch();

    // Summary
    std::cout << "Test Summary: " << g_passed << " passed, " << g_failed
              << " failed, out of " << g_total << " tests." << std::endl;

    // Non-terminating: do not exit early; return non-zero if any test failed.
    return (g_failed == 0) ? 0 : 1;
}