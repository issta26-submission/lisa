// Lightweight C++ test suite for the C function gp_rgb16(Pixel *p, png_const_voidp pb)
// This test avoids Google Test and uses a small in-house test harness.
// It assumes the existence of the gp_rgb16 symbol from the project under test
// and that a Pixel type with fields r,g,b,a exists in the tested code base.
// To maximize portability, we declare a local Pixel struct mirroring the expected layout.
// The tests focus on input coverage for the 3 color channels and the alpha being set to 65535.

#include <cstdint>
#include <stddef.h>
#include <string.h>
#include <../tools/sRGB.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <ctype.h>


// Domain knowledge note:
// - We provide a local Pixel layout that mirrors the expected fields (r,g,b,a) of the
//   compiled library's Pixel type. This is sufficient for passing the memory address
//   of a Pixel to gp_rgb16 as long as the layout matches the library's Pixel struct.
// - The function gp_rgb16 is expected to cast pb to a pointer to 16-bit values and
//   fill p->r, p->g, p->b from pb[0], pb[1], pb[2], and set p->a to 65535.
// - We declare the function with a compatible C linkage to avoid C++ name mangling issues.

extern "C" {

// Local Pixel layout used by tests (must match the library's layout for correctness)
struct Pixel {
    uint16_t r;
    uint16_t g;
    uint16_t b;
    uint16_t a;
};

// Prototype of the function under test. We use the Pixel type from this translation unit
// to build test inputs, passing its address to the function.
void gp_rgb16(Pixel *p, const void *pb);
}

// Helper function: run a single test and print a descriptive message.
// Returns true if the test passes, false otherwise.
static bool run_test_basic_rgb16_case(const char* test_name,
                                    const uint16_t in_r,
                                    const uint16_t in_g,
                                    const uint16_t in_b)
{
    // Arrange
    Pixel p{0, 0, 0, 0};
    uint16_t inputs[3] = {in_r, in_g, in_b};

    // Act
    gp_rgb16(&p, inputs);

    // Assert
    bool ok = (p.r == in_r) && (p.g == in_g) && (p.b == in_b) && (p.a == 65535);

    // Explanation for failure (non-terminating assertion style)
    if (!ok) {
        std::cerr << "Test '" << test_name << "' failed:\n";
        std::cerr << "  Expected: r=" << in_r << ", g=" << in_g << ", b=" << in_b
                  << ", a=65535\n";
        std::cerr << "  Got:      r=" << p.r << ", g=" << p.g << ", b=" << p.b
                  << ", a=" << p.a << "\n";
    }
    return ok;
}

// Test to ensure the alpha channel is always set to 65535 and does not inherit garbage.
static bool run_test_alpha_set_to_max()
{
    // Arrange
    Pixel p{0, 0, 0, 0};
    uint16_t inputs[3] = {123, 45, 67};

    // Act
    gp_rgb16(&p, inputs);

    // Assert
    bool ok = (p.a == 65535);

    if (!ok) {
        std::cerr << "Test 'alpha_set_to_max' failed: expected a=65535, got a="
                  << p.a << "\n";
    }
    // Also verify color channels are correctly assigned, using separate checks as well.
    ok = ok && (p.r == inputs[0]) && (p.g == inputs[1]) && (p.b == inputs[2]);
    if (!ok) {
        std::cerr << "Test 'alpha_set_to_max' color channel mismatch: "
                  << "r=" << p.r << ", g=" << p.g << ", b=" << p.b
                  << ", a=" << p.a << "\n";
    }

    return ok;
}

// Test to verify correct handling of boundary values (0 and 65535)
static bool run_test_boundary_values()
{
    // Boundary case 1: all zeros
    bool ok0 = run_test_basic_rgb16_case("boundary_zeroes", 0, 0, 0);

    // Boundary case 2: all maximum
    bool ok1 = run_test_basic_rgb16_case("boundary_max", 65535, 65535, 65535);

    return ok0 && ok1;
}

// Test to ensure gp_rgb16 does not modify the input buffer beyond the 3 color components.
// We place a guard value before the data and ensure it remains unchanged after the call.
static bool run_test_memory_guard()
{
    Pixel p{0, 0, 0, 0};
    uint16_t data[3] = {5, 6, 7};
    uint16_t guard = 0xABCD; // value should remain unchanged

    // Act
    gp_rgb16(&p, data);

    // Assert
    bool ok = (guard == 0xABCD);
    if (!ok) {
        std::cerr << "Test 'memory_guard' failed: data buffer was modified unexpectedly.\n";
    }

    // Still verify the color values were read correctly, as part of the same test.
    ok = ok && (p.r == data[0]) && (p.g == data[1]) && (p.b == data[2]) && (p.a == 65535);
    if (!ok) {
        std::cerr << "Test 'memory_guard' color read mismatch: r=" << p.r
                  << ", g=" << p.g << ", b=" << p.b << ", a=" << p.a
                  << ", guard=" << guard << "\n";
    }

    return ok;
}

// Entry point for a single cohesive test suite
int main(void)
{
    int failures = 0;

    // Stepwise tests covering core dependent components inferred from the focal method:
    // 1) Basic assignment for true positive values
    if (!run_test_basic_rgb16_case("basic_case_1_2_3", 1, 2, 3)) ++failures;
    if (!run_test_basic_rgb16_case("basic_case_1000_2000_3000", 1000, 2000, 3000)) ++failures;

    // 2) Alpha channel behavior and consistency with input colors
    if (!run_test_alpha_set_to_max()) ++failures;

    // 3) Boundary values to ensure correct handling at extremes
    if (!run_test_boundary_values()) ++failures;

    // 4) Memory guard to ensure no unintended writes to outside the expected input region
    if (!run_test_memory_guard()) ++failures;

    if (failures == 0) {
        std::cout << "All gp_rgb16 tests PASSED.\n";
        return 0;
    } else {
        std::cerr << failures << " gp_rgb16 test(s) FAILED.\n";
        return 1;
    }
}