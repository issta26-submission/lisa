// test_packfloatfrom16.cpp
// Lightweight unit tests for PackFloatFrom16 function (cmspack.c) using C++11.
// No GTest/GMock; a small custom test harness is implemented that continues
// execution even after assertion failures to maximize coverage.
// The tests rely on internal CMS data structures defined in lcms2_internal.h.

#include <cstring>
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <lcms2_internal.h>
#include <cmath>


// Include internal/common headers to access internal types/macros used by cms PackFloatFrom16.

// Forward declaration of the focal function (C linkage to match implementation).
extern "C" cmsUInt8Number* PackFloatFrom16(CMSREGISTER _cmsTRANSFORM* info,
                                           CMSREGISTER cmsUInt16Number wOut[],
                                           CMSREGISTER cmsUInt8Number* output,
                                           CMSREGISTER cmsUInt32Number Stride);

// Simple, non-terminating test assertion helpers
static int g_tests_run = 0;
static int g_tests_passed = 0;

#define ASSERT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << ": " << (msg) << "\n"; \
    } else { \
        ++g_tests_passed; \
    } \
} while (0)

#define ASSERT_NEQ(a, b, msg) do { \
    ++g_tests_run; \
    if ((a) == (b)) { \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << ": " << (msg) \
                  << " (values equal: " << (a) << " == " << (b) << ")" << "\n"; \
    } else { \
        ++g_tests_passed; \
    } \
} while (0)

#define EXPECT_FLOAT_EQ(a, b, tol, msg) do { \
    ++g_tests_run; \
    float _a = (a); \
    float _b = (b); \
    if (std::fabs(_a - _b) > (tol)) { \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                  << ": " << (msg) << " | expected: " << _b << ", got: " << _a << "\n"; \
    } else { \
        ++g_tests_passed; \
    } \
} while (0)

// Helper to reinterpret the 4-byte float from the output buffer
static inline float read_float_from_buf(const unsigned char* buf) {
    float f;
    std::memcpy(&f, buf, sizeof(float));
    return f;
}

// Test 1: Basic single-channel packing (no swap/reverse/planar) for 16-bit input.
// This checks that a middle gray (32768) maps to ~0.5 in the output float.
// We rely on IsInkSpace being false (maximum = 65535.0) for this test.
static void test_packfloatfrom16_basic_single_channel() {
    std::cout << "Running test_packfloatfrom16_basic_single_channel...\n";

    // Allocate and configure internal transform container
    _cmsTRANSFORM* info = (_cmsTRANSFORM*)std::malloc(sizeof(_cmsTRANSFORM));
    std::memset(info, 0, sizeof(_cmsTRANSFORM));

    // Err on the side of a conservative assumption: OutputFormat 1 yields 1 channel.
    info->OutputFormat = 1;

    // Input: single 16-bit word
    cmsUInt16Number wOut[1];
    wOut[0] = 32768; // approximately half of 65535

    // Output buffer: enough space for at least 2 floats (we'll read the first)
    cmsUInt8Number output[32];
    std::memset(output, 0, sizeof(output));

    // Call function under test
    cmsUInt8Number* ret = PackFloatFrom16(info, wOut, output, /*Stride*/ 4);

    // Expectation: since nChan=1 and Extra=0, return should be output+sizeof(float)
    cmsUInt8Number* expected_ret = output + sizeof(float);
    ASSERT_TRUE(ret == expected_ret, "Return pointer should advance by one float for 1 channel.");

    // Read the first packed float
    float actual = read_float_from_buf(output);
    double expected = (double)wOut[0] / 65535.0; // ~0.5
    EXPECT_FLOAT_EQ(actual, (float)expected, 1e-5f,
                    "Packed value for 32768 should be approximately 0.5 when not inkspace.");

    std::free(info);
}

// Test 2: Basic two-channel packing (non-planar, non-swap) to verify multiple channels.
// This exercises the loop for i = 0 and i = 1.
static void test_packfloatfrom16_two_channels() {
    std::cout << "Running test_packfloatfrom16_two_channels...\n";

    _cmsTRANSFORM* info = (_cmsTRANSFORM*)std::malloc(sizeof(_cmsTRANSFORM));
    std::memset(info, 0, sizeof(_cmsTRANSFORM));

    // Attempt to request 2 channels. If the macro resolves differently in your build,
    // this test may need adjustment. We assume OutputFormat=2 yields nChan == 2.
    info->OutputFormat = 2;

    // Two input samples
    cmsUInt16Number wOut[2];
    wOut[0] = 32768;
    wOut[1] = 32768;

    // Output buffer
    cmsUInt8Number output[64];
    std::memset(output, 0, sizeof(output));

    // Call function under test
    cmsUInt8Number* ret = PackFloatFrom16(info, wOut, output, /*Stride*/ 4);

    // Expectation: two channels, two floats stored at output[0] and output[4] due to two channels.
    cmsUInt8Number* expected_ret = output + 2 * sizeof(float);
    ASSERT_TRUE(ret == expected_ret, "Return pointer should advance by (nChan + Extra) floats for 2 channels when Extra=0.");

    // Read the two floats
    float v0 = read_float_from_buf(output);
    float v1 = read_float_from_buf(output + sizeof(float) * 1);

    double expected = (double)32768 / 65535.0; // ~0.5
    EXPECT_FLOAT_EQ(v0, (float)expected, 1e-5f, "First channel packed value should be ~0.5.");
    EXPECT_FLOAT_EQ(v1, (float)expected, 1e-5f, "Second channel packed value should be ~0.5.");

    std::free(info);
}

// Test 3: Basic verify that zero input yields zero in output (non-inkspace maximum path).
static void test_packfloatfrom16_zero_input() {
    std::cout << "Running test_packfloatfrom16_zero_input...\n";

    _cmsTRANSFORM* info = (_cmsTRANSFORM*)std::malloc(sizeof(_cmsTRANSFORM));
    std::memset(info, 0, sizeof(_cmsTRANSFORM));

    info->OutputFormat = 1; // 1 channel

    cmsUInt16Number wOut[1];
    wOut[0] = 0;

    cmsUInt8Number output[32];
    std::memset(output, 0, sizeof(output));

    cmsUInt8Number* ret = PackFloatFrom16(info, wOut, output, /*Stride*/ 4);

    cmsUInt8Number* expected_ret = output + sizeof(float);
    ASSERT_TRUE(ret == expected_ret, "Return pointer should advance by one float for 1 channel.");

    float actual = read_float_from_buf(output);
    EXPECT_FLOAT_EQ(actual, 0.0f, 1e-6f, "Zero input should produce zero output.");

    std::free(info);
}

int main() {
    std::cout << "Starting PackFloatFrom16 test suite (minimal C++11 harness)\n";

    test_packfloatfrom16_basic_single_channel();
    test_packfloatfrom16_two_channels();
    test_packfloatfrom16_zero_input();

    std::cout << "Tests run: " << g_tests_run << ", Passed: " << g_tests_passed << "\n";
    if (g_tests_run == g_tests_passed) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << "Some tests failed. See details above.\n";
        return 1;
    }
}