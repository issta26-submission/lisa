// test_emit_cie_based_abc.cpp
// Unit tests for EmitCIEBasedABC in cmsps2.c
// Notes:
// - This test suite is designed for a C++11 environment without Google Test.
// - It uses a lightweight internal test harness with non-terminating assertions.
// - It relies on the project exposing the types and the function:
//     int EmitCIEBasedABC(cmsIOHANDLER* m, cmsFloat64Number* Matrix, cmsToneCurve** CurveSet, cmsCIEXYZ* BlackPoint)
// - To capture the output produced by _cmsIOPrintf used inside EmitCIEBasedABC, we provide
//   a test-specific _cmsIOPrintf that appends to a global buffer. This requires that the
//   linker resolves to this symbol (override via linking order in the test harness).

#include <cstring>
#include <cstdio>
#include <vector>
#include <iostream>
#include <string>
#include <cstdarg>
#include <lcms2_internal.h>
#include <lcms2.h>


// Include common LittleCMS types if available in your environment.
// If cms2 headers are in the include path, this should compile.

// Forward declaration of the focal function (assumed to be provided by cmsps2.c)
extern "C" int EmitCIEBasedABC(cmsIOHANDLER* m, cmsFloat64Number* Matrix, cmsToneCurve** CurveSet, cmsCIEXYZ* BlackPoint);

// Global buffer to capture all printed output via _cmsIOPrintf
static std::string g_output_buffer;

// Helper: simple output capture function to override the real _cmsIOPrintf
// The real signature in cmsps2.c is: void _cmsIOPrintf(cmsIOHANDLER* m, const char* fmt, ...);
// We provide an extern "C" definition so the linker can resolve to this symbol.
extern "C" void _cmsIOPrintf(cmsIOHANDLER* m, const char* fmt, ...) {
    // Format the variable arguments into a local buffer and append to global buffer.
    char local[4096];
    va_list args;
    va_start(args, fmt);
    int n = vsnprintf(local, sizeof(local), fmt, args);
    va_end(args);
    if (n > 0) {
        // Guard against potential overflow
        if (n >= (int)sizeof(local)) {
            g_output_buffer.append(local, sizeof(local) - 1);
        } else {
            g_output_buffer.append(local, n);
        }
    }
}

// Simple lightweight test framework
struct TestCase {
    std::string name;
    bool passed;
    std::string message;
};

static std::vector<TestCase> g_tests;

// Assertion helpers (non-terminating)
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        g_tests.push_back({__FUNCTION__": " msg, false, (msg)}); \
    } else { \
        g_tests.push_back({__FUNCTION__": OK", true, ""}); \
    } \
} while (false)

#define ASSERT_CONTAINS(hay, needle, msg) do { \
    if ((hay).find((needle)) == std::string::npos) { \
        g_tests.push_back({__FUNCTION__": " msg, false, (msg)}); \
    } else { \
        g_tests.push_back({__FUNCTION__": OK", true, ""}); \
    } \
} while (false)

static void test_emit_cie_based_abc_identity_matrix_outputs()
{
    // Test that EmitCIEBasedABC prints the expected header, 3x3 matrix,
    // and known static segments (RangeLMN, DecodeABC, etc.)
    // We don't rely on actual tone curves; CurveSet can be nulls.
    cmsFloat64Number Matrix[9] = {
        1.0, 2.0, 3.0,
        4.0, 5.0, 6.0,
        7.0, 8.0, 9.0
    };

    cmsToneCurve* CurveSet[3] = { nullptr, nullptr, nullptr };
    cmsCIEXYZ BlackPoint = {0.0, 0.0, 0.0};

    g_output_buffer.clear();

    // Call the focal method. m can be NULL; the test harness uses _cmsIOPrintf to capture output.
    int rc = EmitCIEBasedABC(nullptr, Matrix, CurveSet, &BlackPoint);

    // Basic sanity
    ASSERT_TRUE(rc == 1, "EmitCIEBasedABC should return 1 on success");

    // Verify output contains key tokens and formatting
    std::string out = g_output_buffer;

    ASSERT_CONTAINS(out, "[ /CIEBasedABC", "Output should contain CIEBasedABC header");
    ASSERT_CONTAINS(out, "DecodeABC", "Output should contain DecodeABC section");
    ASSERT_CONTAINS(out, "MatrixABC [ ", "Output should contain MatrixABC header");
    // Expect 9 numbers in row-major order (1,2,3,4,5,6,7,8,9) printed with 6 decimals
    const std::string expectedNumbers =
        "1.000000 2.000000 3.000000 4.000000 5.000000 6.000000 7.000000 8.000000 9.000000";
    ASSERT_CONTAINS(out, expectedNumbers, "Matrix values should be printed in expected order with 6 decimals");
    ASSERT_CONTAINS(out, "RangeLMN [ 0.0 0.9642 0.0 1.0000 0.0 0.8249 ]", "Output should contain RangeLMN constants");
    // The test harness prints something for "INTENT_PERCEPTUAL" if implemented; allow for presence
    // If not present, at least ensure we did not crash and header blocks exist.
}

static void test_emit_cie_based_abc_different_values()
{
    // Test that different matrix values produce corresponding prints in the matrix block.
    cmsFloat64Number Matrix[9] = {
        9.0, 8.0, 7.0,
        6.0, 5.0, 4.0,
        3.0, 2.0, 1.0
    };

    cmsToneCurve* CurveSet[3] = { nullptr, nullptr, nullptr };
    cmsCIEXYZ BlackPoint = {0.123, 0.456, 0.789};

    g_output_buffer.clear();

    int rc = EmitCIEBasedABC(nullptr, Matrix, CurveSet, &BlackPoint);

    ASSERT_TRUE(rc == 1, "EmitCIEBasedABC should return 1 on success");

    std::string out = g_output_buffer;
    // Ensure the first 3 printed values reflect 9,8,7
    // Expect sequence "9.000000 8.000000 7.000000 ..."
    ASSERT_CONTAINS(out, "9.000000 8.000000 7.000000", "Matrix first row should reflect provided matrix values");
    // Confirm the rest of the matrix continues with 6,5,4 and 3,2,1
    ASSERT_CONTAINS(out, "6.000000 5.000000 4.000000", "Matrix second row should reflect provided matrix values");
    ASSERT_CONTAINS(out, "3.000000 2.000000 1.000000", "Matrix third row should reflect provided matrix values");
    // RangeLMN constant should still be present
    ASSERT_CONTAINS(out, "RangeLMN [ 0.0 0.9642 0.0 1.0000 0.0 0.8249 ]", "Output should contain RangeLMN constants");
}

// Entry point for tests
int main() {
    // Run tests
    test_emit_cie_based_abc_identity_matrix_outputs();
    test_emit_cie_based_abc_different_values();

    // Aggregate results
    bool all_passed = true;
    for (const auto& t : g_tests) {
        if (!t.passed) {
            all_passed = false;
            std::cerr << "Test failed: " << t.name << " - " << t.message << "\n";
        }
    }

    // Print a concise report
    if (g_tests.empty()) {
        std::cout << "No tests executed.\n";
    } else {
        int failures = 0;
        for (const auto& t : g_tests)
            if (!t.passed) ++failures;
        std::cout << "Tests run: " << g_tests.size() << ", Failures: " << failures << "\n";
    }

    return all_passed ? 0 : 1;
}