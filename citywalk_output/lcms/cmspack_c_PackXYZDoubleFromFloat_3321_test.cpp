// High-level unit tests for PackXYZDoubleFromFloat in cmsPack (cmspack.c)
//
// This test file is designed to be compiled in a C++11 environment without a
// full GoogleTest/GMock framework. It implements a lightweight test harness
// with non-terminating assertions to maximize code coverage.
// Note: The exact OutputFormat constants (PLANAR vs NON-PLANAR) depend on the
// upstream library's definitions. Replace PLANAR_OUTPUT_FORMAT and NON_PLANAR_OUTPUT_FORMAT
// with the actual formats from your build (e.g., TYPE_XYZ_PLANAR, TYPE_XYZ_8_PLANAR, etc.).
//
// Key dependencies (Candidate Keywords) considered from the focal method:
// - Info->OutputFormat, T_PLANAR(), PixelSize(), MAX_ENCODEABLE_XYZ, T_EXTRA()
// - wOut[] input values, Stride, output buffer layout for planar vs chunky
// - Return pointer correctness: output + sizeof(cmsFloat64Number) for planar,
//   or output + (3*sizeof(cmsFloat64Number) + T_EXTRA(OutputFormat)*sizeof(cmsFloat64Number)) for chunky
// - cmsFloat32Number (input), cmsFloat64Number (internal double representation), cmsUInt8Number buffer
//
// How to adapt for your environment:
// - Define PLANAR_OUTPUT_FORMAT and NON_PLANAR_OUTPUT_FORMAT (or the actual
//   constants your lcms2 build uses) to reflect a planar and a chunky format.
// - Ensure PixelSize(OutputFormat) and T_EXTRA(OutputFormat) macros are evaluated
//   correctly by including the same lcms2_internal.h header as the target code.
// - Use the same MAX_ENCODEABLE_XYZ constant as in the production code.

#include <cstring>
#include <iostream>
#include <cmath>
#include <lcms2_internal.h>
#include <cstdint>


// Lightweight test harness
static int tests_run = 0;
static int tests_pass = 0;

#define EPS_REL 1e-6
#define EXPECT_TRUE(cond, msg) do {                               \
    ++tests_run;                                                  \
    if (!(cond)) {                                               \
        std::cerr << "[FAIL] " << msg << "\n";                   \
    } else {                                                     \
        ++tests_pass;                                            \
    }                                                            \
} while (0)

#define EXPECT_NEAR(a, b, tol, msg) do {                         \
    ++tests_run;                                                 \
    double _a = static_cast<double>(a);                          \
    double _b = static_cast<double>(b);                          \
    double _diff = std::abs(_a - _b);                            \
    bool _ok = false;                                            \
    double _maxab = std::max(std::abs(_a), std::abs(_b));        \
    if (_maxab < 1e-12) {                                      \
        _ok = (_diff < tol);                                    \
    } else {                                                     \
        _ok = (_diff <= _maxab * tol);                          \
    }                                                            \
    if (!_ok) {                                                  \
        std::cerr << "[FAIL] " << msg << " | " \
                  << "Expected ~" << b << ", got " << a << "\n"; \
    } else {                                                     \
        ++tests_pass;                                            \
    }                                                            \
} while (0)

// Helper to run a single test and print summary at the end
static void report_summary() {
    std::cout << "Tests run: " << tests_run << ", Passed: " << tests_pass
              << ", Failed: " << (tests_run - tests_pass) << "\n";
}

// Test 1: Planar path - Validate outputs are placed at 0, Stride, Stride*2
// and the return pointer is exactly after the first double (sizeof(cmsFloat64Number)).
static void test_PackXYZDoubleFromFloat_Planar()
{
    // NOTE: Replace with the actual planar OutputFormat constant used by your build.
    // e.g., PLANAR_XYZ_8 or TYPE_XYZ_PLANAR. The test relies on T_PLANAR(Info->OutputFormat) == true.
    const cmsUInt32Number PLANAR_OUTPUT_FORMAT = PLANAR_OUTPUT_FORMAT; // Replace macro if needed

    _cmsTRANSFORM Info;
    std::memset(&Info, 0, sizeof(Info));
    // Force planar configuration for the test
    Info.OutputFormat = PLANAR_OUTPUT_FORMAT;

    cmsFloat32Number wOut[3] = { 1.0f, 2.0f, 3.0f };

    // Allocate a buffer large enough to cover 3 doubles and some extra, plus enough space for indexing
    const size_t BUF_SIZE = 7 * sizeof(cmsFloat64Number);
    cmsUInt8Number buffer[BUF_SIZE];
    std::memset(buffer, 0, BUF_SIZE);

    // Use a Stride value that will result in distinct memory indices for (0, Stride, Stride*2)
    // For example, Stride = 3 and PixelSize(OutputFormat) = 1
    cmsUInt32Number Stride = 3;

    cmsUInt8Number* ret = PackXYZDoubleFromFloat(&Info, wOut, buffer, Stride);

    // Validate return pointer advancement
    EXPECT_TRUE(ret == buffer + sizeof(cmsFloat64Number),
                "Planar path: Return pointer should advance by sizeof(cmsFloat64Number).");

    // Access the underlying double array
    cmsFloat64Number* Out = reinterpret_cast<cmsFloat64Number*>(buffer);

    // Compute the stride used after PixelSize division
    cmsUInt32Number strideUsed = Stride / PixelSize(Info.OutputFormat);

    cmsFloat64Number expected0 = static_cast<cmsFloat64Number>(wOut[0]) * MAX_ENCODEABLE_XYZ;
    cmsFloat64Number expected1 = static_cast<cmsFloat64Number>(wOut[1]) * MAX_ENCODEABLE_XYZ;
    cmsFloat64Number expected2 = static_cast<cmsFloat64Number>(wOut[2]) * MAX_ENCODEABLE_XYZ;

    // Expect: Out[0] == wOut[0] * MAX_ENCODEABLE_XYZ
    EXPECT_NEAR(Out[0], expected0, 0, "Planar: Out[0] should contain wOut[0] * MAX_ENCODEABLE_XYZ.");

    // In planar mode, values for channel 2 should be placed at Out[Stride] and Out[Stride*2]
    EXPECT_NEAR(Out[strideUsed], expected1, 0,
                "Planar: Out[strideUsed] should contain wOut[1] * MAX_ENCODEABLE_XYZ.");
    EXPECT_NEAR(Out[strideUsed * 2], expected2, 0,
                "Planar: Out[strideUsed*2] should contain wOut[2] * MAX_ENCODEABLE_XYZ.");
}

// Test 2: Chunky path - Validate outputs are placed at 0, 1, 2
// and the return pointer is exactly after 3 doubles plus T_EXTRA(OutputFormat) doubles.
static void test_PackXYZDoubleFromFloat_Chunky()
{
    // NOTE: Replace with the actual non-planar OutputFormat constant used by your build.
    // e.g., CHUNKY_XYZ_8 or TYPE_XYZ_CHUNKY. The test relies on T_PLANAR(Info->OutputFormat) == false.
    const cmsUInt32Number NON_PLANAR_OUTPUT_FORMAT = NON_PLANAR_OUTPUT_FORMAT; // Replace macro if needed

    _cmsTRANSFORM Info;
    std::memset(&Info, 0, sizeof(Info));
    // Force non-planar (chunky) configuration for the test
    Info.OutputFormat = NON_PLANAR_OUTPUT_FORMAT;

    cmsFloat32Number wOut[3] = { 4.0f, 5.0f, 6.0f };

    // Buffer large enough to hold 3 doubles and potential extra space
    const size_t BUF_SIZE = 8 * sizeof(cmsFloat64Number);
    cmsUInt8Number buffer[BUF_SIZE];
    std::memset(buffer, 0, BUF_SIZE);

    cmsUInt32Number Stride = 3; // Stride is not used for calculation in this path except for return offset

    cmsUInt8Number* ret = PackXYZDoubleFromFloat(&Info, wOut, buffer, Stride);

    // Validate return pointer offset: 3 doubles + T_EXTRA(OutputFormat) doubles
    cmsUInt8Number* expectedRet = buffer +
        (sizeof(cmsFloat64Number) * 3) +
        (T_EXTRA(Info.OutputFormat) * sizeof(cmsFloat64Number));

    EXPECT_TRUE(ret == expectedRet,
                "Chunky path: Return pointer should equal buffer + (3 + T_EXTRA) * sizeof(double).");

    // Access the underlying double array
    cmsFloat64Number* Out = reinterpret_cast<cmsFloat64Number*>(buffer);

    cmsFloat64Number expected0 = static_cast<cmsFloat64Number>(wOut[0]) * MAX_ENCODEABLE_XYZ;
    cmsFloat64Number expected1 = static_cast<cmsFloat64Number>(wOut[1]) * MAX_ENCODEABLE_XYZ;
    cmsFloat64Number expected2 = static_cast<cmsFloat64Number>(wOut[2]) * MAX_ENCODEABLE_XYZ;

    // In chunky path, values should be placed at Out[0], Out[1], Out[2]
    EXPECT_NEAR(Out[0], expected0, 0, "Chunky: Out[0] should contain wOut[0] * MAX_ENCODEABLE_XYZ.");
    EXPECT_NEAR(Out[1], expected1, 0, "Chunky: Out[1] should contain wOut[1] * MAX_ENCODEABLE_XYZ.");
    EXPECT_NEAR(Out[2], expected2, 0, "Chunky: Out[2] should contain wOut[2] * MAX_ENCODEABLE_XYZ.");
}

// Entry point
int main()
{
    // Run tests
    test_PackXYZDoubleFromFloat_Planar();
    test_PackXYZDoubleFromFloat_Chunky();

    // Report results
    report_summary();
    return (tests_run == tests_pass) ? 0 : 1;
}