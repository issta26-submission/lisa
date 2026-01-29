// cmsalpha_test.cpp
// Simple C++11 unit tests for the focal method fromHLFto16SE located in cmsalpha.c
// This test targets two primary execution branches by exercising representative inputs.
// It avoids GTest and uses a lightweight, non-terminating assertion mechanism.

#include <cstring>
#include <iostream>
#include <iomanip>
#include <lcms2_internal.h>
#include <cstdint>


// Forward declaration of the focal function under test.
// The function is defined in cmsalpha.c and uses C linkage.
extern "C" void fromHLFto16SE(void* dst, const void* src);

// Lightweight non-terminating assertion macro.
// Prints a message on failure but does not abort the entire test suite,
// allowing multiple tests to run in a single execution.
#define TEST_ASSERT(cond, msg)                                    \
    do {                                                            \
        if (!(cond)) {                                              \
            std::cerr << "[TEST FAILED] " << msg                   \
                      << " (in " << __FILE__ << ":" << __LINE__    \
                      << ")\n";                                   \
            /* Do not return; continue running other tests for coverage */ \
        }                                                           \
    } while (0)

// Helper: dump a 16-bit value in hex
static void dump16(uint16_t v) {
    std::cout << "0x" << std::setw(4) << std::setfill('0')
              << std::hex << static_cast<uint32_t>(v) << std::dec << "\n";
}

// Test Case 1: Zero input half (0x0000) should map to zero after conversion.
// Path: fromHLFto16SE reads a 16-bit half (0x0000) -> 0.0f, scales to 0, endian-swaps -> 0.
static bool test_fromHLFto16SE_zero_input() {
    uint16_t src = 0x0000;   // half-precision zero
    uint16_t dst = 0;

    fromHLFto16SE(&dst, &src);

    bool ok = (dst == 0x0000);
    TEST_ASSERT(ok, "fromHLFto16SE with 0x0000 input should produce 0x0000 in dst");
    if (!ok) {
        std::cout << "  Input (src): 0x" << std::setw(4) << std::setfill('0')
                  << std::hex << src << std::dec << "\n";
        std::cout << "  Output (dst): ";
        dump16(dst);
    }
    return ok;
}

// Test Case 2: Half value of 1.0f (0x3C00) should map to maximum 65535 after scaling.
// Representation: 1.0 in half-precision is 0x3C00. n = 1.0, i = 65535, endian-swapped -> 0xFFFF.
static bool test_fromHLFto16SE_one_input() {
    uint16_t src = 0x3C00; // half-precision 1.0
    uint16_t dst = 0;

    fromHLFto16SE(&dst, &src);

    bool ok = (dst == 0xFFFF);
    TEST_ASSERT(ok, "fromHLFto16SE with 0x3C00 input should produce 0xFFFF in dst");
    if (!ok) {
        std::cout << "  Input (src): 0x" << std::setw(4) << std::setfill('0')
                  << std::hex << src << std::dec << "\n";
        std::cout << "  Output (dst): ";
        dump16(dst);
    }
    return ok;
}

// Optional: Additional test to exercise a simple complementary scenario.
// This helps increase branch coverage by exercising non-boundary input.
// For 0x0001 (tiny positive near zero), we expect a very small value after scaling (non-zero).
static bool test_fromHLFto16SE_tiny_positive_input() {
    uint16_t src = 0x0001; // tiny positive half (~0.000... something)
    uint16_t dst = 0;

    fromHLFto16SE(&dst, &src);

    // We can't precisely predict the exact value without the exact half-to-float mapping,
    // but we can ensure that a non-zero input yields a non-zero output (in typical implementations).
    bool ok = (dst != 0x0000);
    TEST_ASSERT(ok, "fromHLFto16SE with 0x0001 input should yield a non-zero dst (non-deterministic value depending on half-to-float mapping)");
    if (!ok) {
        std::cout << "  Input (src): 0x" << std::setw(4) << std::setfill('0')
                  << std::hex << src << std::dec << "\n";
        std::cout << "  Output (dst): ";
        dump16(dst);
    }
    return ok;
}

// Main entry point: run all tests and report aggregate status.
// As per domain knowledge, test methods are invoked directly in main when gtest isn't used.
int main() {
    std::cout << "Running cmsalpha.fromHLFto16SE unit tests (C++11, no GTest)\n";

    bool pass = true;
    pass &= test_fromHLFto16SE_zero_input();
    pass &= test_fromHLFto16SE_one_input();
    pass &= test_fromHLFto16SE_tiny_positive_input();

    // Summary
    std::cout << "\nTest suite completed. Summary:\n";
    if (pass) {
        std::cout << "ALL TESTS PASSED (best-effort, with non-terminating assertions).\n";
    } else {
        std::cout << "Some tests reported failures. See above for details.\n";
    }

    // Return code can be non-zero to indicate some failures if desired.
    // However, per guideline, we avoid hard-terminating signals in test harness.
    return pass ? 0 : 1;
}