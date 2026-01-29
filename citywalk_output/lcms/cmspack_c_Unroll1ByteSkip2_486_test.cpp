/*
   Unit test suite for Unroll1ByteSkip2 from cmsUnroll suite.
   - This test is designed for a C library (lcms) function Unroll1ByteSkip2
     and is built to run under a C++11 environment without Google Test.
   - The tests exercise the core behavior of Unroll1ByteSkip2:
       * It reads a single 8-bit value from accum, converts it to 16-bit via FROM_8_TO_16,
         assigns that 16-bit value to wIn[0], wIn[1], and wIn[2].
       * It then increments accum by 1 and advances accum by 2 more (total +3).
       * It returns the updated accum pointer.
   - We verify that:
       * All three wIn entries receive FROM_8_TO_16(initial_accum_value).
       * The returned accum pointer is exactly 3 bytes past the initial accum.
   - We also test with non-null info and non-zero Stride to ensure unused-parameter behavior does not affect results.
*/

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Import the internal CMS types and definitions used by the focal method.
// The actual environment must provide lcms2_internal.h with all necessary typedefs/macros.
// If your project uses a different path, adjust accordingly.

// Forward declaration of the focal function to ensure linkage in C++ tests.
// The function uses C linkage; wrapping in extern "C" for correct symbol handling.
extern "C" {
    cmsUInt8Number* Unroll1ByteSkip2(CMSREGISTER _cmsTRANSFORM* info,
                                     CMSREGISTER cmsUInt16Number wIn[],
                                     CMSREGISTER cmsUInt8Number* accum,
                                     CMSREGISTER cmsUInt32Number Stride);
}

// Simple test harness (non-terminating assertions) to maximize test coverage.
static int tests_run = 0;
static int tests_failed = 0;

static void report_failure(const char* test_name, const char* message) {
    std::cerr << "[FAILED] " << test_name << ": " << message << std::endl;
    ++tests_failed;
}

static void report_success(const char* test_name) {
    std::cout << "[PASSED] " << test_name << std::endl;
    ++tests_run;
}

// Test 1: Basic behavior with a non-zero initial accum value (e.g., 0x12)
static void test_Unroll1ByteSkip2_basic_case() {
    const char* test_name = "test_Unroll1ByteSkip2_basic_case";

    cmsUInt16Number wIn[3] = {0, 0, 0};
    cmsUInt8Number accumBuf[8];
    std::memset(accumBuf, 0, sizeof(accumBuf));

    // Initial 8-bit value to convert
    accumBuf[0] = 0x12; // 18 decimal
    cmsUInt8Number* accum = accumBuf;
    cmsUInt32Number Stride = 0;
    // The info parameter is unused; pass nullptr
    cmsUInt8Number* end = Unroll1ByteSkip2(nullptr, wIn, accum, Stride);

    cmsUInt16Number expected = FROM_8_TO_16(accumBuf[0]); // macro defined in cms headers
    bool wIn_all_equal = (wIn[0] == expected) && (wIn[1] == expected) && (wIn[2] == expected);
    if (!wIn_all_equal) {
        report_failure(test_name, "wIn elements do not all equal FROM_8_TO_16(initial accum[0]).");
        report_failure(test_name, "Expected all three wIn values to match FROM_8_TO_16(accumBuf[0]).");
        return;
    }

    if (end != accumBuf + 3) {
        report_failure(test_name, "Returned accum pointer was not advanced by 3 as expected.");
        return;
    }

    report_success(test_name);
}

// Test 2: Basic behavior with a zero initial accum value (0x00)
static void test_Unroll1ByteSkip2_zero_case() {
    const char* test_name = "test_Unroll1ByteSkip2_zero_case";

    cmsUInt16Number wIn[3] = {0, 0, 0};
    cmsUInt8Number accumBuf[8];
    std::memset(accumBuf, 0, sizeof(accumBuf));

    accumBuf[0] = 0x00; // 0 decimal
    cmsUInt8Number* accum = accumBuf;
    cmsUInt32Number Stride = 0;
    cmsUInt8Number* end = Unroll1ByteSkip2(nullptr, wIn, accum, Stride);

    cmsUInt16Number expected = FROM_8_TO_16(accumBuf[0]);
    bool wIn_all_zero = (wIn[0] == expected) && (wIn[1] == expected) && (wIn[2] == expected);
    if (!wIn_all_zero) {
        report_failure(test_name, "wIn elements should all be FROM_8_TO_16(0) when initial accum is 0.");
        return;
    }

    if (end != accumBuf + 3) {
        report_failure(test_name, "Returned accum pointer was not advanced by 3 for zero input.");
        return;
    }

    report_success(test_name);
}

// Test 3: Verify that non-null info and non-zero Stride are ignored (since parameters are marked unused)
static void test_Unroll1ByteSkip2_nonnull_info_and_stride() {
    const char* test_name = "test_Unroll1ByteSkip2_nonnull_info_and_stride";

    cmsUInt16Number wIn[3] = {0, 0, 0};
    cmsUInt8Number accumBuf[8];
    std::memset(accumBuf, 0, sizeof(accumBuf));

    accumBuf[0] = 0xFF; // 255 decimal
    cmsUInt8Number dummyInfo;
    cmsUInt8Number* accum = accumBuf;
    cmsUInt32Number Stride = 12345; // arbitrary non-zero stride
    // Pass a non-null pointer for info to ensure it's ignored
    cmsUInt8Number* end = Unroll1ByteSkip2(reinterpret_cast<_cmsTRANSFORM*>(&dummyInfo), wIn, accum, Stride);

    cmsUInt16Number expected = FROM_8_TO_16(accumBuf[0]);
    bool wIn_all_equal = (wIn[0] == expected) && (wIn[1] == expected) && (wIn[2] == expected);
    if (!wIn_all_equal) {
        report_failure(test_name, "wIn values should be FROM_8_TO_16(initial accum[0]) when info/stride are non-default.");
        return;
    }

    if (end != accumBuf + 3) {
        report_failure(test_name, "Returned accum pointer should still be advanced by 3 even with non-null info and non-zero stride.");
        return;
    }

    report_success(test_name);
}

int main() {
    // Run all unit tests
    test_Unroll1ByteSkip2_basic_case();
    test_Unroll1ByteSkip2_zero_case();
    test_Unroll1ByteSkip2_nonnull_info_and_stride();

    // Summary
    std::cout << "Tests run: " << tests_run << ", Failures: " << tests_failed << std::endl;
    return (tests_failed == 0) ? 0 : 1;
}