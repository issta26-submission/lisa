// Test suite for optimize_cmf in pngwutil.c
// This file provides a small, self-contained C++11 test harness (no GoogleTest)
// that exercises the optimize_cmf function. It follows the constraints:
// - Uses non-terminating assertions (logs failures but continues)
// - Includes explanations per test
// - Assumes optimize_cmf has C linkage: extern "C" void optimize_cmf(unsigned char*, size_t)
// - Uses standard library only for testing (no GMock/GTest)

// Step 1: Program Understanding (captured as comments for maintainers)
// - Core logic operates on the first two bytes of the data buffer (data[0], data[1])
// - Only executes when data_size <= 16384
// - Checks z_cmf = data[0]; requires (z_cmf & 0x0f) == 8 and (z_cmf & 0xf0) <= 0x70
// - Extracts z_cinfo from the high nibble; computes half_z_window_size = 1 << (z_cinfo + 7)
// - If data_size <= half_z_window_size, it reduces z_cinfo while data_size <= half_z_window_size
// - Writes back z_cmf = (z_cmf & 0x0f) | (z_cinfo << 4) and updates data[0]
// - Updates data[1] to tmp = (data[1] & 0xe0) + 0x1f - ((z_cmf << 8) + tmp) % 0x1f
// - All branches testable via carefully chosen data_size and z_cmf values
// Step 2: Unit Test Generation (candidate test cases described below implement the key branches)
// - Test A: data_size > 16384 -> no changes (outermost guard)
// - Test B: data_size <= 16384 and z_cmf has proper nibble values; data_size <= half_z_window_size -> shrink via loop to z_cinfo reduced
// - Test C: data_size <= half_z_window_size after shrink loop to different z_cinfo
// - Test D: z_cinfo = 1 case to verify single iteration without underflow
// Step 3: Test Case Refinement
// - Covers true/false branches of critical predicates
// - Provides explicit expected values for deterministic inputs
// - Uses non-terminating assertions; tests can run sequentially in main()

#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>
#include <cstddef>


// Declaration of the focal function (assumed to be linked from pngwutil.c)
extern "C" void optimize_cmf(unsigned char* data, size_t data_size);

// Simple non-terminating test harness (logs and continues)
static int g_failures = 0;

#define EXPECT_TRUE(cond, desc) \
    do { if(!(cond)) { std::cerr << "TEST FAILURE: " << desc << "\n"; ++g_failures; } } while(0)

// Test A: data_size exceeds 16384, function should not modify data
void test_optimize_cmf_no_change_large()
{
    // Candidate Keywords: data_size, branch for > 16384, no nibbles change
    unsigned char data[4] = { 0x58, 0x00, 0xAA, 0x55 }; // initial bytes
    const size_t size = 20000; // > 16384
    unsigned char before0 = data[0];
    unsigned char before1 = data[1];

    optimize_cmf(data, size);

    EXPECT_TRUE(data[0] == before0, "Data[0] should remain unchanged when data_size > 16384");
    EXPECT_TRUE(data[1] == before1, "Data[1] should remain unchanged when data_size > 16384");
}

// Test B: data_size <= 16384, proper z_cmf, and data_size <= half_z_window_size triggers shrink
void test_optimize_cmf_shrink_case_2000()
{
    // z_cmf = 0x58 -> z_cinfo = 5, half_z_window_size = 1<<(5+7) = 4096
    // data_size = 2000 <= 4096 triggers shrink: z_cinfo reduces to 3, z_cmf becomes 0x38
    // data[1] recomputed accordingly
    unsigned char data[2] = { 0x58, 0x00 };
    optimize_cmf(data, 2000);

    EXPECT_TRUE(data[0] == 0x38, "z_cmf should shrink from 0x58 to 0x38 (z_cinfo:5->3)");
    // data[1] calculation: tmp starts as (0 & 0xE0) = 0
    // tmp += 0x1f - ((z_cmf << 8) + tmp) % 0x1f
    // z_cmf after update = 0x38; (0x38 << 8) = 0x3800; 0x3800 % 0x1f = 0x0E (14)
    // 0x1f - 14 = 17 (0x11)
    EXPECT_TRUE(data[1] == 0x11, "data[1] should be updated to 0x11 after CMF correction");
}

// Test C: data_size triggers shrink to a different z_cinfo value (e.g., 4)
void test_optimize_cmf_shrink_case_3000()
{
    // z_cmf = 0x58 -> z_cinfo = 5, half 4096
    // data_size = 3000 <= 4096 triggers shrink: after loop z_cinfo becomes 4
    // z_cmf becomes 0x48, data[1] recomputed
    unsigned char data[2] = { 0x58, 0x00 };
    optimize_cmf(data, 3000);

    EXPECT_TRUE(data[0] == 0x48, "z_cmf should shrink from 0x58 to 0x48 (z_cinfo:5->4)");
    // data[1] calculation with z_cmf after update 0x48:
    // (0x48 << 8) = 0x4800; 0x4800 % 0x1f = 0x12 (18)
    // 0x1f - 18 = 13 (0x0D)
    EXPECT_TRUE(data[1] == 0x0D, "data[1] should be updated to 0x0D after CMF correction");
}

// Test D: Test path with z_cinfo = 1 (no underflow) to ensure single iteration behavior
void test_optimize_cmf_shrink_case_minimal_zinfo1()
{
    // Choose z_cmf = 0x18 (z_cinfo = 1), data_size = 50 (<= 256)
    // After one iteration: z_cinfo becomes 0, z_cmf becomes 0x08
    // data[1] recomputed with z_cmf = 0x08
    unsigned char data[2] = { 0x18, 0x00 };
    optimize_cmf(data, 50);

    EXPECT_TRUE(data[0] == 0x08, "z_cmf should become 0x08 (z_cinfo:1->0)");
    // (0x08 << 8) = 0x0800; 0x0800 % 31 = 2; 31-2 = 29 (0x1D)
    EXPECT_TRUE(data[1] == 0x1D, "data[1] should be updated to 0x1D after CMF correction");
}

// Runner
int main()
{
    // Run all tests
    test_optimize_cmf_no_change_large();
    test_optimize_cmf_shrink_case_2000();
    test_optimize_cmf_shrink_case_3000();
    test_optimize_cmf_shrink_case_minimal_zinfo1();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed\n";
        return 1;
    }
}