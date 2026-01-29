/*
Step 1 - Program Understanding (high-level)
- Focal method: fromDBLto16(void* dst, const void* src)
  - Reads a cmsFloat64Number (double) from src.
  - Multiplies by 65535.0f and passes the result to _cmsQuickSaturateWord to clamp to [0, 65535].
  - Writes the resulting 16-bit value into dst as a cmsUInt16Number.
- Core behavior: converts a double in the range and saturates to a 16-bit unsigned value in the 0..65535 range.
- Candidate Keywords (representing core behavior/dependencies):
  - fromDBLto16, 65535, saturate, saturateWord, cmsUInt16Number, cmsFloat64Number, double input, dst write, saturation boundaries, memory write, pointer cast, boundary conditions (negatives, large positives).
- Dependencies: function is externally linked; types cmsUInt16Number and cmsFloat64Number are used in the project. For test purposes, we keep prototype as extern "C" void fromDBLto16(void* dst, const void* src) to avoid typedef mismatches.

Step 2 - Unit Test Generation (for fromDBLto16)
- We will create a small C++11 test harness (no GTest) that:
  - Declares the function with C linkage.
  - Tests key scenarios including: negative input, zero, fractional values, 1.0, large inputs, and multiple memory bytes (memory safety).
  - Uses "non-terminating" checks by collecting failures and continuing test execution.
- Test coverage goals:
  - Negative inputs clamp to 0.
  - 0.0 maps to 0.
  - fractional values map via truncation after multiplication (e.g., 0.5 -> 32767).
  - 1.0 maps to 65535.
  - Large input maps to 65535 (saturation).
  - dst memory safety: only two bytes touched; adjacent memory unchanged.

Step 3 - Test Case Refinement (domain knowledge)
- Use correct C++11 standard library facilities only.
- Do not rely on private/internal details beyond the provided function.
- Use direct memory-based tests to ensure proper write behavior and to detect potential out-of-bounds writes.
- Provide explanatory comments for each unit test.

Code (C++11 test harness for fromDBLto16)
*/
#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <cstdint>
#include <sstream>


// Step 1: Candidate Keywords explained above (in comments).
//
// fromDBLto16: converts a double src to a 16-bit unsigned value in dst by:
//   dst = saturate( src * 65535.0 )
// where saturate clamps to [0, 65535] and truncates toward zero.
// The exact saturating function (_cmsQuickSaturateWord) is internal, so we
// test by reproducing the expected numeric result and compare.

extern "C" void fromDBLto16(void* dst, const void* src);

static std::vector<std::string> g_failLogs;

static void logFail(const std::string& s) {
    g_failLogs.push_back(s);
}

// Helper to compute expected value according to the observed saturate behavior
// in fromDBLto16: t = src * 65535.0; clamp to [0, 65535]; cast to uint16_t.
static uint16_t expectedFromDouble(double v) {
    double t = v * 65535.0;
    if (t < 0.0) t = 0.0;
    if (t > 65535.0) t = 65535.0;
    return static_cast<uint16_t>(t); // truncates toward zero
}

static void test_fromDBLto16_basic_values() {
    // Test a variety of representative inputs, including negatives, zero, fractional, and large values.
    double inputs[] = { -1.0, 0.0, 0.2, 0.5, 0.9999, 1.0, 2.0, 100.0, 0.0 };
    const size_t n = sizeof(inputs) / sizeof(inputs[0]);
    for (size_t i = 0; i < n; ++i) {
        double srcVal = inputs[i];
        uint16_t dstVal = 0;
        fromDBLto16(&dstVal, &srcVal);
        uint16_t expected = expectedFromDouble(srcVal);

        if (dstVal != expected) {
            std::ostringstream oss;
            oss << "FromDBLto16 basic_values: input=" << srcVal
                << " -> dst=" << dstVal << " (expected " << expected << ")";
            logFail(oss.str());
        } else {
            // Optional verbose success (commented out to reduce noise)
            // std::cout << "PASS: input=" << srcVal << " -> dst=" << dstVal << "\n";
        }
    }
}

static void test_fromDBLto16_memory_safety() {
    // Ensure only the intended 2 bytes are written, leaving adjacent memory intact.
    uint16_t arr[2] = { 0xAAAA, 0xBBBB };
    double srcVal = 0.0; // Expect 0 written to arr[0]
    fromDBLto16(&arr[0], &srcVal);

    // Check first element changed to 0
    if (arr[0] != 0) {
        std::ostringstream oss;
        oss << "MemorySafety: arr[0] after write = " << arr[0] << ", expected 0";
        logFail(oss.str());
    }

    // Check second element remains unchanged
    if (arr[1] != 0xBBBB) {
        std::ostringstream oss;
        oss << "MemorySafety: arr[1] modified unexpectedly, found " << std::hex << arr[1] << std::dec;
        logFail(oss.str());
    }
}

static void test_fromDBLto16_large_and_negative_values() {
    // Large positive should saturate to max
    {
        double srcVal = 1000.0;
        uint16_t dstVal = 0;
        fromDBLto16(&dstVal, &srcVal);
        uint16_t expected = expectedFromDouble(srcVal);
        if (dstVal != expected) {
            std::ostringstream oss;
            oss << "LargeValue: input=" << srcVal << " -> dst=" << dstVal << " (expected " << expected << ")";
            logFail(oss.str());
        }
    }

    // Negative large should saturate to 0
    {
        double srcVal = -123.456;
        uint16_t dstVal = 0;
        fromDBLto16(&dstVal, &srcVal);
        uint16_t expected = expectedFromDouble(srcVal);
        if (dstVal != expected) {
            std::ostringstream oss;
            oss << "NegativeValue: input=" << srcVal << " -> dst=" << dstVal << " (expected " << expected << ")";
            logFail(oss.str());
        }
    }
}

static void test_fromDBLto16_frac_and_boundary() {
    // Specific boundary case: 0.5 should yield 32767 (0.5 * 65535 = 32767.5 -> trunc to 32767)
    {
        double srcVal = 0.5;
        uint16_t dstVal = 0;
        fromDBLto16(&dstVal, &srcVal);
        uint16_t expected = expectedFromDouble(srcVal);
        if (dstVal != expected) {
            std::ostringstream oss;
            oss << "BoundaryHalf: input=" << srcVal << " -> dst=" << dstVal << " (expected " << expected << ")";
            logFail(oss.str());
        }
    }
}

int main() {
    // Run all tests
    test_fromDBLto16_basic_values();
    test_fromDBLto16_memory_safety();
    test_fromDBLto16_large_and_negative_values();
    test_fromDBLto16_frac_and_boundary();

    // Report results
    if (g_failLogs.empty()) {
        std::cout << "All tests passed for fromDBLto16.\n";
        return 0;
    } else {
        std::cout << "fromDBLto16: " << g_failLogs.size() << " test(s) failed.\n";
        for (size_t i = 0; i < g_failLogs.size(); ++i) {
            std::cout << "Failure " << (i + 1) << ": " << g_failLogs[i] << "\n";
        }
        return 1;
    }
}