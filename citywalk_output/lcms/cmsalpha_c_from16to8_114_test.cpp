// CMS From16to8 unit tests (C++11) without GoogleTest.
// This test harness calls the focal function from16to8 and validates its behavior
// against an assumed 16-bit to 8-bit mapping based on the high-byte extraction.
// The test avoids terminating early on failures and reports a summary at the end.

#include <lcms2_internal.h>
#include <cstdio>
#include <cstdint>


// Declare the focal function with C linkage to ensure proper linkage in C++ test.
extern "C" void from16to8(void* dst, const void* src);

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_passed_tests = 0;
static int g_failed_tests = 0;

// Helper to report a single test result without stopping the suite
static void log_test_result(const char* test_name, bool passed,
                            cmsUInt16Number src, cmsUInt8Number dst,
                            cmsUInt8Number expected)
{
    if (passed) {
        printf("PASS: %s (src=0x%04X, dst=0x%02X, expected=0x%02X)\n",
               test_name, static_cast<unsigned>(src),
               static_cast<unsigned>(dst),
               static_cast<unsigned>(expected));
        g_passed_tests++;
    } else {
        printf("FAIL: %s (src=0x%04X, dst=0x%02X, expected=0x%02X)\n",
               test_name, static_cast<unsigned>(src),
               static_cast<unsigned>(dst),
               static_cast<unsigned>(expected));
        g_failed_tests++;
    }
    g_total_tests++;
}

// Convenience wrapper for a single test case
static void run_case(const char* name,
                     cmsUInt16Number src_value,
                     cmsUInt8Number expected_dst)
{
    cmsUInt8Number dst_value = 0;
    // Call the focal function under test
    from16to8(&dst_value, &src_value);

    // Compare actual vs expected
    bool ok = (dst_value == expected_dst);
    log_test_result(name, ok, src_value, dst_value, expected_dst);
}

// Test suite: verify from16to8 maps 16-bit input to 8-bit output using high-byte extraction.
// Expected mapping: dst = (src_value >> 8) & 0xFF
// Note: This test assumes the macro FROM_16_TO_8(n) is implemented as a high-byte extraction.
// If the actual macro uses rounding or different scaling, adjust the expected values accordingly.
static void test_from16to8_high_byte_mapping()
{
    // Boundary and representative cases to cover various input ranges
    // The expected_dst is computed as (src_value >> 8)
    run_case("zero_map",      0x0000,        static_cast<cmsUInt8Number>(0x00));
    run_case("low_boundary",  0x00FF,        static_cast<cmsUInt8Number>(0x00));
    run_case("low_mid",       0x0123,        static_cast<cmsUInt8Number>(0x01));
    run_case("mid_high",      0x1234,        static_cast<cmsUInt8Number>(0x12));
    run_case("high_mid",      0xF123,        static_cast<cmsUInt8Number>(0xF1));
    run_case("high_boundary", 0xFF00,        static_cast<cmsUInt8Number>(0xFF));
    run_case("max_value",     0xFFFF,        static_cast<cmsUInt8Number>(0xFF));

    // Random additional checks
    run_case("random1",       0x0A1B,        static_cast<cmsUInt8Number>(0x0A));
    run_case("random2",       0xABCD,        static_cast<cmsUInt8Number>(0xAB));
}

// Entry point for the test suite
int main()
{
    // Step 2: Generate tests for the focal method from16to8 in cmsalpha.c
    // Note: The test relies on high-byte mapping assumption for FROM_16_TO_8(n).

    printf("Starting unit tests for from16to8...\n");
    test_from16to8_high_byte_mapping();

    // Summary
    printf("Test summary: total=%d, passed=%d, failed=%d\n",
           g_total_tests, g_passed_tests, g_failed_tests);

    // Return non-zero if any test failed
    return (g_failed_tests > 0) ? 1 : 0;
}