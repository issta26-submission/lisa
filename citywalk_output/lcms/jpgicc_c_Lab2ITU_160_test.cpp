// Unit test suite for Lab2ITU function in jpgicc.c
// This test uses the actual cmsCIELab and cmsUInt16Number types from Little CMS (lcms2).
// It calls Lab2ITU with a variety of Lab inputs and verifies the 3-output values
// against the exact calculation performed by the function (floor of each formula).
// No GTest is used; a lightweight in-process test harness is provided.

#include <array>
#include <iccjpeg.h>
#include <cstdint>
#include <jpeglib.h>
#include <iostream>
#include <string>
#include <lcms2.h>
#include <utils.h>
#include <cmath>


// Candidate Keywords (Step 1): key components involved in Lab2ITU
// - Lab (cmsCIELab structure with fields L, a, b)
// - Out (array of cmsUInt16Number with 3 elements)
// - Conversion constants: 100, 170, 200, 65535, 32768, 24576
// - Functions/types: cmsCIELab, cmsUInt16Number

// Include Little CMS header for correct type definitions

// The Lab2ITU function under test is defined in jpgicc.c
extern "C" void Lab2ITU(const cmsCIELab* Lab, cmsUInt16Number Out[3]);

// Simple non-terminating assertion helper
#define ASSERT_EQ(actual, expected, msg) do { \
    if ((actual) != (expected)) { \
        std::cout << "  [FAIL] " << msg << "\n" \
                  << "    Expected: " << +static_cast<uint32_t>(expected) \
                  << ", Actual: " << +static_cast<uint32_t>(actual) << "\n"; \
        test_failures++; \
    } else { \
        std::cout << "  [PASS] " << msg << "\n"; \
        test_passes++; \
    } \
} while (0)

static int test_passes = 0;
static int test_failures = 0;

// Compute the expected Out values using the exact same formulas as Lab2ITU.
// This ensures the test aligns with the function's implemented behavior.
static std::array<cmsUInt16Number, 3> ComputeExpectedOut(double L, double A, double B)
{
    double v0 = (L / 100.0) * 65535.0;
    double v1 = (A / 170.0) * 65535.0 + 32768.0;
    double v2 = (B / 200.0) * 65535.0 + 24576.0;

    cmsUInt16Number o0 = static_cast<cmsUInt16Number>(std::floor(v0));
    cmsUInt16Number o1 = static_cast<cmsUInt16Number>(std::floor(v1));
    cmsUInt16Number o2 = static_cast<cmsUInt16Number>(std::floor(v2));

    return { o0, o1, o2 };
}

int main()
{
    std::cout << "Lab2ITU Unit Test Suite (C++11, single translation unit) - Start\n";

    // Test Case 1: Central zero
    // Lab = { L=0, a=0, b=0 } => Out = {0, 32768, 24576}
    {
        cmsCIELab Lab;
        Lab.L = 0.0;
        Lab.a = 0.0;
        Lab.b = 0.0;

        cmsUInt16Number Out[3];
        Lab2ITU(&Lab, Out);

        auto Exp = ComputeExpectedOut(0.0, 0.0, 0.0);
        ASSERT_EQ(Out[0], Exp[0], "Test1 Out[0] equals floor((L/100)*65535) with L=0");
        ASSERT_EQ(Out[1], Exp[1], "Test1 Out[1] equals floor((a/170)*65535 + 32768) with a=0");
        ASSERT_EQ(Out[2], Exp[2], "Test1 Out[2] equals floor((b/200)*65535 + 24576) with b=0");
    }

    // Test Case 2: Upper L bound
    // Lab = { L=100, a=0, b=0 } => Out0 = 65535, Out1=32768, Out2=24576
    {
        cmsCIELab Lab;
        Lab.L = 100.0;
        Lab.a = 0.0;
        Lab.b = 0.0;

        cmsUInt16Number Out[3];
        Lab2ITU(&Lab, Out);

        auto Exp = ComputeExpectedOut(100.0, 0.0, 0.0);
        ASSERT_EQ(Out[0], Exp[0], "Test2 Out[0] equals floor((L/100)*65535) with L=100");
        ASSERT_EQ(Out[1], Exp[1], "Test2 Out[1] equals floor((a/170)*65535 + 32768) with a=0");
        ASSERT_EQ(Out[2], Exp[2], "Test2 Out[2] equals floor((b/200)*65535 + 24576) with b=0");
    }

    // Test Case 3: Mid L value
    // Lab = { L=50, a=0, b=0 } => Out0 ≈ floor(0.5*65535) = 32767
    {
        cmsCIELab Lab;
        Lab.L = 50.0;
        Lab.a = 0.0;
        Lab.b = 0.0;

        cmsUInt16Number Out[3];
        Lab2ITU(&Lab, Out);

        auto Exp = ComputeExpectedOut(50.0, 0.0, 0.0);
        ASSERT_EQ(Out[0], Exp[0], "Test3 Out[0] equals floor((L/100)*65535) with L=50");
        ASSERT_EQ(Out[1], Exp[1], "Test3 Out[1] equals floor((a/170)*65535 + 32768) with a=0");
        ASSERT_EQ(Out[2], Exp[2], "Test3 Out[2] equals floor((b/200)*65535 + 24576) with b=0");
    }

    // Test Case 4: Positive a beyond offset to test wrap behavior
    // Lab = { L=0, a=170, b=0 } => Out1 = floor((170/170)*65535 + 32768) = floor(98303) -> 32767 after uint16 wrap
    {
        cmsCIELab Lab;
        Lab.L = 0.0;
        Lab.a = 170.0;
        Lab.b = 0.0;

        cmsUInt16Number Out[3];
        Lab2ITU(&Lab, Out);

        auto Exp = ComputeExpectedOut(0.0, 170.0, 0.0);
        ASSERT_EQ(Out[0], Exp[0], "Test4 Out[0] equals floor((L/100)*65535) with L=0");
        ASSERT_EQ(Out[1], Exp[1], "Test4 Out[1] equals floor((a/170)*65535 + 32768) with a=170");
        ASSERT_EQ(Out[2], Exp[2], "Test4 Out[2] equals floor((b/200)*65535 + 24576) with b=0");
    }

    // Test Case 5: Negative a value to exercise wrap-around
    // Lab = { L=0, a=-170, b=0 }
    {
        cmsCIELab Lab;
        Lab.L = 0.0;
        Lab.a = -170.0;
        Lab.b = 0.0;

        cmsUInt16Number Out[3];
        Lab2ITU(&Lab, Out);

        auto Exp = ComputeExpectedOut(0.0, -170.0, 0.0);
        ASSERT_EQ(Out[0], Exp[0], "Test5 Out[0] equals floor((L/100)*65535) with L=0");
        ASSERT_EQ(Out[1], Exp[1], "Test5 Out[1] equals floor((a/170)*65535 + 32768) with a=-170");
        ASSERT_EQ(Out[2], Exp[2], "Test5 Out[2] equals floor((b/200)*65535 + 24576) with b=0");
    }

    // Test Case 6: Positive b beyond offset
    // Lab = { L=0, a=0, b=200 } => Out2 = floor((200/200)*65535 + 24576) = floor(90111) -> 90111-65536 = 24575
    {
        cmsCIELab Lab;
        Lab.L = 0.0;
        Lab.a = 0.0;
        Lab.b = 200.0;

        cmsUInt16Number Out[3];
        Lab2ITU(&Lab, Out);

        auto Exp = ComputeExpectedOut(0.0, 0.0, 200.0);
        ASSERT_EQ(Out[0], Exp[0], "Test6 Out[0] equals floor((L/100)*65535) with L=0");
        ASSERT_EQ(Out[1], Exp[1], "Test6 Out[1] equals floor((a/170)*65535 + 32768) with a=0");
        ASSERT_EQ(Out[2], Exp[2], "Test6 Out[2] equals floor((b/200)*65535 + 24576) with b=200");
    }

    // Test Case 7: Mixed nonzero values to exercise all three channels
    // Lab = { L=100, a=127, b=-128 }
    {
        cmsCIELab Lab;
        Lab.L = 100.0;
        Lab.a = 127.0;
        Lab.b = -128.0;

        cmsUInt16Number Out[3];
        Lab2ITU(&Lab, Out);

        auto Exp = ComputeExpectedOut(100.0, 127.0, -128.0);
        ASSERT_EQ(Out[0], Exp[0], "Test7 Out[0] equals floor((L/100)*65535) with L=100");
        ASSERT_EQ(Out[1], Exp[1], "Test7 Out[1] equals floor((a/170)*65535 + 32768) with a=127");
        ASSERT_EQ(Out[2], Exp[2], "Test7 Out[2] equals floor((b/200)*65535 + 24576) with b=-128");
    }

    // Test Case 8: Upper-bound-like mixed values
    // Lab = { L=100, a=-128, b=200 }
    {
        cmsCIELab Lab;
        Lab.L = 100.0;
        Lab.a = -128.0;
        Lab.b = 200.0;

        cmsUInt16Number Out[3];
        Lab2ITU(&Lab, Out);

        auto Exp = ComputeExpectedOut(100.0, -128.0, 200.0);
        ASSERT_EQ(Out[0], Exp[0], "Test8 Out[0] equals floor((L/100)*65535) with L=100");
        ASSERT_EQ(Out[1], Exp[1], "Test8 Out[1] equals floor((a/170)*65535 + 32768) with a=-128");
        ASSERT_EQ(Out[2], Exp[2], "Test8 Out[2] equals floor((b/200)*65535 + 24576) with b=200");
    }

    std::cout << "Lab2ITU Unit Test Suite - Summary: "
              << test_passes << " passed, "
              << test_failures << " failed.\n";

    // Return non-zero if any test failed
    return (test_failures == 0) ? 0 : 1;
}