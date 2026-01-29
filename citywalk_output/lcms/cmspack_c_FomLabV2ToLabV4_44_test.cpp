/*
CMSS Test Suite for FomLabV2ToLabV4 (cmspack.c focal method)
- This test harness is designed to be compiled with C++11 without GTest.
- It tests the focal method FomLabV2ToLabV4 as defined below.
- Coverage notes:
  - Verifies the identity behavior for 16-bit inputs (true path).
  - Explains the (unreachable) true-branch condition a > 0xffff and why it cannot be exercised with 16-bit input.
  - Uses non-terminating EXPECT_* style assertions to maximize code execution and coverage.
- Candidate Keywords derived from the focal method:
  FomLabV2ToLabV4, cmsUInt16Number, 0xffff boundary, 16-bit identity, bit-twiddling (x << 8 | x) >> 8.
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cstdio>
#include <cstdint>


// Lightweight test harness (non-terminating assertions)
static int g_test_total = 0;
static int g_test_failed = 0;

#define HEX(x) (static_cast<unsigned int>(x))
#define EXPECT_TRUE(cond, msg) do { \
    ++g_test_total; \
    if (!(cond)) { \
        ++g_test_failed; \
        std::cerr << "[FAILED] " << msg << " [Value: " << std::hex << HEX(cond ? 1 : 0) << "]" << std::dec << "\n"; \
    } \
} while (0)

#define EXPECT_EQ(expected, actual, msg) do { \
    ++g_test_total; \
    if ((expected) != (actual)) { \
        ++g_test_failed; \
        std::cerr << "[FAILED] " << msg \
                  << " Expected: " << std::hex << static_cast<unsigned int>(expected) \
                  << " Actual: " << static_cast<unsigned int>(actual) << std::dec << "\n"; \
    } \
} while (0)

// Domain-specific types (matching the CMS internal typedefs)
using cmsUInt16Number = uint16_t;
using cmsUInt8Number  = uint8_t;
using cmsUInt32Number = uint32_t;

// Minimal INLINE macro replacement
#define cmsINLINE inline

// Focal method under test (copied/adapted for standalone compilation)
cmsINLINE cmsUInt16Number FomLabV2ToLabV4(cmsUInt16Number x)
{
{
    int a = (x << 8 | x) >> 8;  // * 257 / 256
    if ( a > 0xffff) return 0xffff;
    return (cmsUInt16Number) a;
}
}

// Test case 1: Identity behavior for several representative 16-bit inputs.
// Rationale: For all 0 <= x <= 0xFFFF, FomLabV2ToLabV4(x) should return x.
// Note: The true-branch (a > 0xffff) is unreachable with 16-bit inputs.
// This test confirms the expected identity mapping.
void test_identity_behaviour() {
    // Test a few representative values
    cmsUInt16Number inputs[] = {0x0000, 0x0001, 0x00FF, 0x1234, 0xABCD, 0xFFFF};
    for (cmsUInt16Number v : inputs) {
        cmsUInt16Number out = FomLabV2ToLabV4(v);
        char msg[128];
        std::snprintf(msg, sizeof(msg),
                      "FomLabV2ToLabV4 should return input for x=0x%04X", static_cast<unsigned int>(v));
        EXPECT_EQ(v, out, msg);
    }
}

// Test case 2: Check boundary conditions and that the function does not spuriously
// hit the (a > 0xffff) branch for 16-bit inputs.
// This test asserts that for all tested inputs, output is never 0xFFFF unless input is 0xFFFF.
void test_boundary_branch_not_taken() {
    cmsUInt16Number inputs[] = {0x0000, 0x00AA, 0x2345, 0x7F00, 0xFFFE, 0xFFFF};
    for (cmsUInt16Number v : inputs) {
        cmsUInt16Number out = FomLabV2ToLabV4(v);
        // If the branch were taken, the function would return 0xFFFF regardless of input.
        // Since the identity property holds, the only time we observe 0xFFFF is when v is 0xFFFF.
        if (v != 0xFFFF) {
            char msg[128];
            std::snprintf(msg, sizeof(msg),
                          "For x=0x%04X, output should not be forced to 0xFFFF", static_cast<unsigned int>(v));
            EXPECT_TRUE(out != 0xFFFF, msg);
        } else {
            // v == 0xFFFF should yield 0xFFFF due to identity
            EXPECT_EQ(0xFFFF, out, "For x=0xFFFF, output should be 0xFFFF");
        }
    }
}

// Test case 3: Verify static-like behavior through direct function call.
// This ensures the function remains accessible and consistent when compiled in a standalone test.
void test_direct_call_consistency() {
    // Re-test a couple of values to ensure no regression
    EXPECT_EQ(0, FomLabV2ToLabV4(0), "Direct call: FomLabV2ToLabV4(0) should be 0");
    EXPECT_EQ(0xFFFF, FomLabV2ToLabV4(0xFFFF), "Direct call: FomLabV2ToLabV4(0xFFFF) should be 0xFFFF");
}

// Entry point for running all tests
int main() {
    test_identity_behaviour();
    test_boundary_branch_not_taken();
    test_direct_call_consistency();

    std::cout << "Test Summary: Total=" << g_test_total << " Failed=" << g_test_failed << std::endl;
    if (g_test_failed == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}