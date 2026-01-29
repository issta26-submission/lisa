// Automated C++11 unit test suite for png_gamma_8bit_correct
// This test suite is designed to be compiled without GoogleTest.
// It uses a lightweight, non-terminating assertion approach and a simple main() harness.
//
// The tests exercise the behavior of png_gamma_8bit_correct(unsigned int value, png_fixed_point gamma_val)
// as implemented in the provided FOCAL_METHOD. The test relies on the binary interface of the function
// and assumes the common libpng-type aliases (png_byte as unsigned char and png_fixed_point as int).
// The tests cover boundary cases, the gamma=1.0 identity path (floating-point branch), and a gamma=0 path.
// Explanatory comments are added for each test case.

#include <iostream>
#include <pngpriv.h>


// Forward declaration of the focal function with C linkage.
// We assume the actual libpng type aliases are compatible with these C++ types.
// If your build uses different typedefs for png_byte or png_fixed_point, adjust accordingly.
extern "C" {
    // png_byte is typically unsigned char; png_fixed_point is typically int.
    // Signature observed from the focal method:
    // png_byte png_gamma_8bit_correct(unsigned int value, png_fixed_point gamma_val);
    unsigned char png_gamma_8bit_correct(unsigned int value, int gamma_val);
}

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;

#define EXPECT_EQ(expected, actual) do { \
    if (static_cast<unsigned long long>(static_cast<int>(actual)) != static_cast<unsigned long long>(expected)) { \
        ++g_failures; \
        std::cerr << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ \
                  << "  expected=" << (expected) << "  actual=" << static_cast<int>(actual) << std::endl; \
    } \
} while(false)

// Test 1: Boundary values (0, 255) should bypass gamma adjustment and return value masked to 0xff
// This verifies the gate condition (value > 0 && value < 255) is not entered for 0 and 255.
void test_boundary_values() {
    unsigned int v0 = 0;
    unsigned int v255 = 255;

    unsigned char r0 = png_gamma_8bit_correct(v0, 100000);     // gamma_val = 1.0 (100000)
    unsigned char r255 = png_gamma_8bit_correct(v255, 100000);

    // Expect direct return of masked value
    EXPECT_EQ(0, r0);
    EXPECT_EQ(255, r255);

    // Also ensure a value just outside the gamma-range wraps correctly
    unsigned char r256 = png_gamma_8bit_correct(256, 100000);
    EXPECT_EQ(0, r256); // 256 & 0xff == 0
}

// Test 2: Identity behavior for gamma = 1.0 (floating-point path should yield output == input for 1..254)
// With gamma_val = 100000 (gamma = 1.0), and value in 1..254, result should equal value.
void test_gamma_identity_gamma1() {
    int gamma_one = 100000; // 1.0 in fixed-point representation expected by the code
    int test_values[] = {1, 2, 3, 10, 50, 100, 150, 200, 254};
    for (int val : test_values) {
        unsigned char out = png_gamma_8bit_correct(static_cast<unsigned int>(val), gamma_one);
        EXPECT_EQ(static_cast<unsigned int>(val), static_cast<unsigned int>(out));
    }
}

// Test 3: Gamma = 0.0 path (to exercise the non-linear branch if executed)
// For gamma_val = 0, the computation effectively uses pow((value/255.), 0) = 1.0
// which should yield 255 after the floor+0.5 operation, for 1..254.
void test_gamma_zero_path() {
    int gamma_zero = 0; // gamma = 0.0
    unsigned char out1 = png_gamma_8bit_correct(1, gamma_zero);
    unsigned char out254 = png_gamma_8bit_correct(254, gamma_zero);
    EXPECT_EQ(255, out1);
    EXPECT_EQ(255, out254);
}

// Test 4: Value wrapping behavior for values beyond 255 (e.g., 256, 511)
// The implementation falls back to value & 0xff when not in (0,255) exclusive range.
void test_value_wrapping() {
    unsigned char r256 = png_gamma_8bit_correct(256, 100000);
    unsigned char r511 = png_gamma_8bit_correct(511, 100000);
    EXPECT_EQ(0, r256); // 256 & 0xff == 0
    EXPECT_EQ(255, r511); // 511 & 0xff == 255
}

// Test 5: Additional random checks for values in 1..254 with gamma=1.0 to reinforce identity behavior
// This provides broader coverage for the identity path.
void test_more_identity_values_gamma1() {
    int gamma_one = 100000;
    int values[] = {4, 7, 13, 42, 199, 253};
    for (int v : values) {
        unsigned char out = png_gamma_8bit_correct(static_cast<unsigned int>(v), gamma_one);
        EXPECT_EQ(static_cast<unsigned int>(v), static_cast<unsigned int>(out));
    }
}

// Runner that invokes all tests and reports summary
void run_all_tests() {
    test_boundary_values();
    test_gamma_identity_gamma1();
    test_gamma_zero_path();
    test_value_wrapping();
    test_more_identity_values_gamma1();
}

int main() {
    run_all_tests();

    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return g_failures;
    }
}