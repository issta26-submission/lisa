/*
Unit test suite for the focal method: fromDBLto8 in cmsalpha.c

Test goals:
- Validate that fromDBLto8 correctly converts a cmsFloat64Number input
  to a cmsUInt8Number output by computing n * 255.0 and saturating/clamping to [0, 255].
- Verify boundary and representative cases, including negative, zero, middle, and saturating inputs.
- Ensure the function does not modify the source value (src) passed in.
- Use a lightweight, non-GTest C++11 test harness with clear per-test comments.

Notes:
- We include the internal cms header to obtain the cms* typedefs.
- We declare the focal function with C linkage to avoid name mangling when invoked from C++.
- Expected results are computed by applying the same saturating/clip behavior used by the implementation: clamp to [0, 255] after multiplying by 255.0, then cast to unsigned char (i.e., truncation for fractional parts).
- The tests use non-terminating assertions: they log failures but continue executing to maximize coverage.

Code begins
*/

#include <iostream>
#include <string>
#include <iomanip>
#include <lcms2_internal.h>
#include <cmath>


// Import necessary dependencies (types and internal helpers)

// Ensure correct linkage for the focal function when compiled in C++
extern "C" void fromDBLto8(void* dst, const void* src);

// Helper: compute the expected saturated 8-bit value for a given double input.
// This follows the same semantics as the actual converter: result = clamp(n * 255.0, 0, 255) and cast to unsigned 8-bit.
static unsigned char expected_fromDBLto8(double input)
{
    double v = input * 255.0;
    if (v <= 0.0) return 0;
    if (v >= 255.0) return 255;
    // Cast truncates toward zero for positive values
    return static_cast<unsigned char>(v);
}

// Simple non-terminating assertion/logger
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_pass(const std::string& msg)
{
    ++g_total_tests;
    std::cout << "[PASS] " << msg << std::endl;
}

static void log_fail(const std::string& msg)
{
    ++g_total_tests;
    ++g_failed_tests;
    std::cerr << "[FAIL] " << msg << std::endl;
}

// Test 1: Basic mapping from a few representative inputs
// - Negative input should saturate to 0
// - Zero input should map to 0
// - 0.5 maps to floor(0.5 * 255) = 127
// - 0.999 maps to floor(0.999 * 255) = 254
// - 1.0 maps to 255
// - 2.0 saturates to 255
static void test_fromDBLto8_basic_mappings()
{
    // Array of test inputs with descriptive messages
    const struct { double input; const char* desc; } tests[] = {
        {-1.0, "Negative input should saturate to 0"},
        {0.0,  "Zero input should map to 0"},
        {0.5,  "Half (0.5) maps to floor(127.5) => 127"},
        {0.999, "Just below 1.0 maps to floor(254.745) => 254"},
        {1.0,  "One maps to 255"},
        {2.0,  "Greater than 1.0 saturates to 255"}
    };

    for (const auto& t : tests)
    {
        unsigned char dst = 0xAA; // non-zero initial value to ensure write occurred
        cmsFloat64Number src = t.input;
        fromDBLto8(&dst, &src);
        unsigned char expected = expected_fromDBLto8(t.input);

        // Non-terminating assertion: report but continue
        if (dst != expected)
        {
            std::ostringstream oss;
            oss << "fromDBLto8(input=" << t.input << ") expected dst=" << int(expected)
                << " but got dst=" << int(dst);
            log_fail(oss.str());
        }
        else
        {
            std::ostringstream oss;
            oss << "fromDBLto8(input=" << t.input << ") produced dst=" << int(dst);
            log_pass(oss.str());
        }
    }
}

// Test 2: Ensure the source value (src) is not modified by fromDBLto8
// This checks the function's const-correctness behavior on the input.
// If it accidentally writes to src, the test will fail.
static void test_fromDBLto8_src_not_modified()
{
    double input = 0.75; // 0.75 * 255 = 191.25 -> 191
    cmsFloat64Number src = input;
    unsigned char dst = 0x00;

    // Call function
    fromDBLto8(&dst, &src);

    // Verify src remains unchanged
    if (src != input)
    {
        log_fail("fromDBLto8 modified the input src value unexpectedly.");
        // Also report the actual and expected values
        std::ostringstream oss;
        oss << "src changed from " << input << " to " << src;
        log_fail(oss.str());
    }
    else
    {
        // Also verify dst correctness with the same saturating rule
        unsigned char expected = expected_fromDBLto8(input);
        if (dst != expected)
        {
            std::ostringstream oss;
            oss << "fromDBLto8 with src=" << input << " produced dst=" << int(dst)
                << ", expected=" << int(expected);
            log_fail(oss.str());
        }
        else
        {
            std::ostringstream oss;
            oss << "fromDBLto8 did not modify src and produced dst=" << int(dst)
                << " as expected.";
            log_pass(oss.str());
        }
    }
}

// Test 3: Randomized couple of additional values to improve coverage
static void test_fromDBLto8_additional_values()
{
    const double inputs[] = { -5.0, 0.1, 0.25, 0.6, 0.98, 1.234, 10.0 };
    for (double in : inputs)
    {
        unsigned char dst = 0;
        cmsFloat64Number src = in;
        fromDBLto8(&dst, &src);
        unsigned char expected = expected_fromDBLto8(in);
        if (dst != expected)
        {
            std::ostringstream oss;
            oss << "fromDBLto8(input=" << in << ") mismatch: dst=" << int(dst)
                << " expected=" << int(expected);
            log_fail(oss.str());
        }
        else
        {
            std::ostringstream oss;
            oss << "fromDBLto8(input=" << in << ") produced expected dst=" << int(dst);
            log_pass(oss.str());
        }

        // Also ensure src unchanged in this case
        if (src != in)
        {
            std::ostringstream oss;
            oss << "fromDBLto8 modified src for input " << in
                << " (src became " << src << ")";
            log_fail(oss.str());
        }
    }
}

int main()
{
    std::cout << std::boolalpha;
    std::cout << "Running fromDBLto8 unit tests (CMS alpha) using C++11 harness" << std::endl;

    // Run tests
    test_fromDBLto8_basic_mappings();
    test_fromDBLto8_src_not_modified();
    test_fromDBLto8_additional_values();

    // Summary
    std::cout << "Total tests attempted: " << g_total_tests << std::endl;
    std::cout << "Total tests failed:  " << g_failed_tests << std::endl;
    if (g_failed_tests == 0)
    {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    }
    else
    {
        std::cout << "Some tests FAILED." << std::endl;
        return 1;
    }
}

/*
Notes for maintainers:
- This test suite focuses exclusively on the focal method fromDBLto8 as requested.
- It uses a non-terminating assertion style to maximize code-path coverage in a single run.
- It relies on the header and symbols from the cmsalpha/cmslcms project (lcms2_internal.h, etc.). Ensure the test is linked with the compiled cmsalpha.c and appropriate library paths.
- If the internal saturate function semantics differ (e.g., rounding instead of truncation), adjust the expected_fromDBLto8 helper accordingly.
*/