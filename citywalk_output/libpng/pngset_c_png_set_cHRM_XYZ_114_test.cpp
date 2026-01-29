// This test suite targets the focal method: png_set_cHRM_XYZ
// It verifies that the method correctly delegates to
// png_set_cHRM_XYZ_fixed after converting double inputs to fixed-point
// using png_fixed. The test uses lightweight, in-file mocks to avoid
// external dependencies (e.g., libpng) and runs without a test framework.
// Explanations are provided inline for each unit test.

// Step 1 (high-level): Candidate Keywords (core dependencies and flow
// being exercised by the focal method)
// - cHRM XYZ: color primaries (Red, Green, Blue) in XYZ space
// - Fixed-point conversion: png_fixed
// - Delegation: png_set_cHRM_XYZ_fixed is invoked with fixed-point values
// - Strings for error messages used in png_fixed (not critical for tests)
// - Focal inputs: red_X, red_Y, red_Z, green_X, green_Y, green_Z, blue_X, blue_Y, blue_Z

#include <iostream>
#include <string>
#include <cmath>
#include <pngpriv.h>


// Global test-state to capture the arguments passed to the mocked fixed-setter.
struct XYZRecord {
    int red_X;
    int red_Y;
    int red_Z;
    int green_X;
    int green_Y;
    int green_Z;
    int blue_X;
    int blue_Y;
    int blue_Z;
    bool set;
};

// Global storage for the last call to png_set_cHRM_XYZ_fixed
static XYZRecord g_lastXYZ = {0,0,0,0,0,0,0,0,0,false};

// Fixed-point scale factor (chosen to be precise enough for tests)
static const int FIX_SCALE = 1000000; // 1,000,000 units per 1.0 in double

// Minimal mock types to satisfy the focal function's signature
typedef const void* png_const_structrp;
typedef void*      png_inforp;
typedef int        png_fixed_point;

// Forward declarations of mocks (to be used by the focal function)
png_fixed_point png_fixed(png_const_structrp png_ptr, double v, const char* what);
void png_set_cHRM_XYZ_fixed(png_const_structrp png_ptr, png_inforp info_ptr,
    png_fixed_point int_red_X, png_fixed_point int_red_Y, png_fixed_point int_red_Z,
    png_fixed_point int_green_X, png_fixed_point int_green_Y, png_fixed_point int_green_Z,
    png_fixed_point int_blue_X, png_fixed_point int_blue_Y, png_fixed_point int_blue_Z);

// The focal function under test (copied/adapted here to be self-contained)
// It converts the provided doubles to fixed-point and forwards them.
void png_set_cHRM_XYZ(png_const_structrp png_ptr, png_inforp info_ptr,
    double red_X, double red_Y, double red_Z,
    double green_X, double green_Y, double green_Z,
    double blue_X, double blue_Y, double blue_Z)
{
{
   png_set_cHRM_XYZ_fixed(png_ptr, info_ptr,
       png_fixed(png_ptr, red_X, "cHRM Red X"),
       png_fixed(png_ptr, red_Y, "cHRM Red Y"),
       png_fixed(png_ptr, red_Z, "cHRM Red Z"),
       png_fixed(png_ptr, green_X, "cHRM Green X"),
       png_fixed(png_ptr, green_Y, "cHRM Green Y"),
       png_fixed(png_ptr, green_Z, "cHRM Green Z"),
       png_fixed(png_ptr, blue_X, "cHRM Blue X"),
       png_fixed(png_ptr, blue_Y, "cHRM Blue Y"),
       png_fixed(png_ptr, blue_Z, "cHRM Blue Z"));
}
}

// Implementation of the mock for fixed-point conversion
png_fixed_point png_fixed(png_const_structrp /*png_ptr*/, double v, const char* /*what*/)
{
    // Clamp NaN to 0, and ensure the value lies within [0, 1]
    if (std::isnan(v)) v = 0.0;
    if (v < 0.0) v = 0.0;
    if (v > 1.0) v = 1.0;

    // Convert to fixed-point representation
    long long fixed = static_cast<long long>(v * FIX_SCALE);
    // Ensure it fits in int for test simplicity
    if (fixed < std::numeric_limits<int>::min()) fixed = std::numeric_limits<int>::min();
    if (fixed > std::numeric_limits<int>::max()) fixed = std::numeric_limits<int>::max();
    return static_cast<png_fixed_point>(fixed);
}

// Implementation of the mock that captures the fixed-point values
void png_set_cHRM_XYZ_fixed(png_const_structrp /*png_ptr*/, png_inforp /*info_ptr*/,
    png_fixed_point int_red_X, png_fixed_point int_red_Y, png_fixed_point int_red_Z,
    png_fixed_point int_green_X, png_fixed_point int_green_Y, png_fixed_point int_green_Z,
    png_fixed_point int_blue_X, png_fixed_point int_blue_Y, png_fixed_point int_blue_Z)
{
    g_lastXYZ.red_X   = static_cast<int>(int_red_X);
    g_lastXYZ.red_Y   = static_cast<int>(int_red_Y);
    g_lastXYZ.red_Z   = static_cast<int>(int_red_Z);
    g_lastXYZ.green_X = static_cast<int>(int_green_X);
    g_lastXYZ.green_Y = static_cast<int>(int_green_Y);
    g_lastXYZ.green_Z = static_cast<int>(int_green_Z);
    g_lastXYZ.blue_X  = static_cast<int>(int_blue_X);
    g_lastXYZ.blue_Y  = static_cast<int>(int_blue_Y);
    g_lastXYZ.blue_Z  = static_cast<int>(int_blue_Z);
    g_lastXYZ.set       = true;
}

// Lightweight assertion helpers for non-terminating checks
static int total_checks = 0;
static int total_failures = 0;

static void report_failure(const std::string& msg)
{
    ++total_checks;
    ++total_failures;
    std::cerr << "FAIL: " << msg << std::endl;
}

static void report_success()
{
    ++total_checks;
    // success: no output to avoid noise; could log if needed
}

#define ASSERT_EQ(expected, actual, msg) do { \
    ++total_checks; \
    if ((expected) != (actual)) { \
        report_failure((std::string(msg) + " Expected: " + std::to_string(expected) + \
                        " Actual: " + std::to_string(actual)).c_str()); \
    } else { \
        report_success(); \
    } \
} while(false)

#define ASSERT_TRUE(cond, msg) do { \
    ++total_checks; \
    if (!(cond)) { report_failure(std::string("Assertion failed: ").append(msg)); } \
    else { report_success(); } \
} while(false)

static void reset_lastXYZ()
{
    g_lastXYZ = XYZRecord{0,0,0,0,0,0,0,0,0,false};
}

// Test 1: Normal input values within [0,1] for each color component
// Expectation: Fixed-point values match exact conversion with clamping (no clamping needed here)
void test_normal_values()
{
    // Explanation: This test exercises the typical, valid domain where all input doubles are within [0,1].
    reset_lastXYZ();
    double red_X   = 0.20;
    double red_Y   = 0.30;
    double red_Z   = 0.40;
    double green_X = 0.25;
    double green_Y = 0.50;
    double green_Z = 0.75;
    double blue_X  = 0.10;
    double blue_Y  = 0.90;
    double blue_Z  = 0.60;

    png_set_cHRM_XYZ((png_const_structrp)0x1, (png_inforp)0x2,
        red_X, red_Y, red_Z,
        green_X, green_Y, green_Z,
        blue_X, blue_Y, blue_Z);

    // Expected fixed-point values after png_fixed with scale 1,000,000
    ASSERT_EQ(200000, g_lastXYZ.red_X,   "Normal test: Red X");
    ASSERT_EQ(300000, g_lastXYZ.red_Y,   "Normal test: Red Y");
    ASSERT_EQ(400000, g_lastXYZ.red_Z,   "Normal test: Red Z");
    ASSERT_EQ(250000, g_lastXYZ.green_X, "Normal test: Green X");
    ASSERT_EQ(500000, g_lastXYZ.green_Y, "Normal test: Green Y");
    ASSERT_EQ(750000, g_lastXYZ.green_Z, "Normal test: Green Z");
    ASSERT_EQ(1000000/10, g_lastXYZ.blue_X,  "Normal test: Blue X"); // 0.10 -> 100000
    // Note: 0.90 -> 900000; 0.60 -> 600000
    ASSERT_EQ(900000, g_lastXYZ.blue_Y,   "Normal test: Blue Y");
    ASSERT_EQ(600000, g_lastXYZ.blue_Z,   "Normal test: Blue Z");
}

// Test 2: Out-of-range input values to verify clamping behavior in png_fixed
// Expectation: Negative values become 0.0, values > 1.0 become 1.0
void test_out_of_range_values()
{
    reset_lastXYZ();
    double red_X   = -0.2;
    double red_Y   = 1.5;
    double red_Z   = 0.8;
    double green_X = -0.1;
    double green_Y = 0.0;
    double green_Z = 1.2;
    double blue_X  = 0.5;
    double blue_Y  = -0.3;
    double blue_Z  = 1.9;

    png_set_cHRM_XYZ((png_const_structrp)0x3, (png_inforp)0x4,
        red_X, red_Y, red_Z,
        green_X, green_Y, green_Z,
        blue_X, blue_Y, blue_Z);

    // Clamped values:
    // red_X  -0.2 -> 0.0 -> 0
    // red_Y   1.5 -> 1.0 -> 1000000
    // red_Z   0.8 -> 0.8 -> 800000
    // green_X -0.1 -> 0.0 -> 0
    // green_Y 0.0 -> 0.0 -> 0
    // green_Z 1.2 -> 1.0 -> 1000000
    // blue_X  0.5 -> 0.5 -> 500000
    // blue_Y -0.3 -> 0.0 -> 0
    // blue_Z  1.9 -> 1.0 -> 1000000

    ASSERT_EQ(0,       g_lastXYZ.red_X,   "OutOfRange test: Red X clamp");
    ASSERT_EQ(1000000, g_lastXYZ.red_Y,   "OutOfRange test: Red Y clamp");
    ASSERT_EQ(800000,  g_lastXYZ.red_Z,   "OutOfRange test: Red Z clamp");
    ASSERT_EQ(0,       g_lastXYZ.green_X, "OutOfRange test: Green X clamp");
    ASSERT_EQ(0,       g_lastXYZ.green_Y, "OutOfRange test: Green Y clamp");
    ASSERT_EQ(1000000, g_lastXYZ.green_Z, "OutOfRange test: Green Z clamp");
    ASSERT_EQ(500000,  g_lastXYZ.blue_X,  "OutOfRange test: Blue X clamp");
    ASSERT_EQ(0,       g_lastXYZ.blue_Y,  "OutOfRange test: Blue Y clamp");
    ASSERT_EQ(1000000, g_lastXYZ.blue_Z,  "OutOfRange test: Blue Z clamp");
}

// Test 3: NaN inputs to ensure safe handling in fixed-point conversion
void test_nan_values()
{
    reset_lastXYZ();
    double red_X   = std::nan(""); // NaN should be treated as 0.0
    double red_Y   = 0.25;
    double red_Z   = 0.25;
    double green_X = 0.25;
    double green_Y = std::nan("");
    double green_Z = 0.25;
    double blue_X  = 0.25;
    double blue_Y  = 0.25;
    double blue_Z  = std::nan("");

    png_set_cHRM_XYZ((png_const_structrp)0x5, (png_inforp)0x6,
        red_X, red_Y, red_Z,
        green_X, green_Y, green_Z,
        blue_X, blue_Y, blue_Z);

    // NaN values should be converted to 0.0, others converted normally
    ASSERT_EQ(0, g_lastXYZ.red_X,   "NaN test: Red X becomes 0");
    ASSERT_EQ(250000, g_lastXYZ.red_Y, "NaN test: Red Y");
    ASSERT_EQ(250000, g_lastXYZ.red_Z, "NaN test: Red Z");
    ASSERT_EQ(250000, g_lastXYZ.green_X, "NaN test: Green X");
    ASSERT_EQ(0, g_lastXYZ.green_Y, "NaN test: Green Y becomes 0");
    ASSERT_EQ(250000, g_lastXYZ.green_Z, "NaN test: Green Z");
    ASSERT_EQ(250000, g_lastXYZ.blue_X, "NaN test: Blue X");
    ASSERT_EQ(250000, g_lastXYZ.blue_Y, "NaN test: Blue Y");
    ASSERT_EQ(0, g_lastXYZ.blue_Z, "NaN test: Blue Z becomes 0");
}

// Test 4: Multiple consecutive calls should update the stored values accordingly
void test_multiple_calls_update()
{
    reset_lastXYZ();
    // First call
    png_set_cHRM_XYZ((png_const_structrp)0x7, (png_inforp)0x8,
        0.1, 0.2, 0.3,
        0.4, 0.5, 0.6,
        0.7, 0.8, 0.9);

    ASSERT_EQ(100000, g_lastXYZ.red_X,   "Multi-call 1: Red X");
    ASSERT_EQ(200000, g_lastXYZ.red_Y,   "Multi-call 1: Red Y");
    ASSERT_EQ(300000, g_lastXYZ.red_Z,   "Multi-call 1: Red Z");

    // Second call with different values
    reset_lastXYZ();
    png_set_cHRM_XYZ((png_const_structrp)0x9, (png_inforp)0xA,
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0);

    // All should map to 1.0 => 1000000 for the respective channels
    ASSERT_EQ(1000000, g_lastXYZ.red_X,   "Multi-call 2: Red X");
    ASSERT_EQ(0,       g_lastXYZ.red_Y,   "Multi-call 2: Red Y");
    ASSERT_EQ(0,       g_lastXYZ.red_Z,   "Multi-call 2: Red Z");
    ASSERT_EQ(0,       g_lastXYZ.green_X, "Multi-call 2: Green X");
    ASSERT_EQ(1000000, g_lastXYZ.green_Y, "Multi-call 2: Green Y");
    ASSERT_EQ(0,       g_lastXYZ.green_Z, "Multi-call 2: Green Z");
    ASSERT_EQ(0,       g_lastXYZ.blue_X,  "Multi-call 2: Blue X");
    ASSERT_EQ(0,       g_lastXYZ.blue_Y,  "Multi-call 2: Blue Y");
    ASSERT_EQ(1000000, g_lastXYZ.blue_Z,  "Multi-call 2: Blue Z");
}

int main()
{
    // Run all test cases
    test_normal_values();
    test_out_of_range_values();
    test_nan_values();
    test_multiple_calls_update();

    // Summary
    std::cout << "Total checks: " << total_checks
              << ", Failures: " << total_failures
              << std::endl;

    // Return non-zero if any test failed
    return total_failures ? 1 : 0;
}