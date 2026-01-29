/*
Unit tests for SetCGATSfld in transicc.c
- No GoogleTest: a lightweight, self-contained test harness
- Tests cover: quantization on/off, and data-set failure path
- Expects the real transicc.c symbols to be linked in (SetCGATSfld, lQuantize, hIT8out, CGATSPatch, CGATSoutFilename, cmsIT8SetDataDbl, FatalError)
- Mocks provided for cmsIT8SetDataDbl and FatalError to observe behavior without terminating tests
- All tests run from main; assertions are non-terminating and report results
Note: This harness assumes the external symbols exist in the linked object and are compatible with the declared signatures.
*/

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <cstdarg>
#include <utils.h>
#include <cmath>


// Typedefs matching the focal code's usage (simplified for testing)
using cmsFloat64Number = double;
using cmsBool = bool;

// Forward declaration of the function under test (from transicc.c)
extern "C" void SetCGATSfld(const char* Col, cmsFloat64Number Val);

// External variables and functions expected from the focal code's environment.
// We declare them as extern so we can exercise the real implementation when linked.
extern "C" cmsBool lQuantize;
extern "C" void* hIT8out;
extern "C" void* CGATSPatch;
extern "C" const char* CGATSoutFilename;

// The real cmsIT8SetDataDbl and FatalError would be linked from the project.
// We provide mocks here to observe behavior without terminating tests.
extern "C" cmsBool cmsIT8SetDataDbl(void* /*hIT8out*/, void* /*CGATS*/ /*patch*/, const char* /*Col*/, cmsFloat64Number /*Val*/);

// Mock control for cmsIT8SetDataDbl
static cmsBool mockCmsIT8SetDataDbl_ReturnValue = true;
static const char* mockCmsIT8SetDataDbl_LastCol = nullptr;
static cmsFloat64Number mockCmsIT8SetDataDbl_LastVal = 0.0;
static int mockCmsIT8SetDataDbl_CallCount = 0;

// Mock implementation: observe input and return predefined value
extern "C" cmsBool cmsIT8SetDataDbl(void* /*hIT8out*/, void* /*CGATS*/ /*patch*/, const char* Col, cmsFloat64Number Val) {
    mockCmsIT8SetDataDbl_LastCol = Col;
    mockCmsIT8SetDataDbl_LastVal = Val;
    ++mockCmsIT8SetDataDbl_CallCount;
    return mockCmsIT8SetDataDbl_ReturnValue;
}

// Mock FatalError: do not terminate; just record that it was called
static bool mockFatalError_WasCalled = false;
static std::string mockFatalError_LastMessage;

// Variadic C function for FatalError
extern "C" void FatalError(const char* fmt, ...) {
    mockFatalError_WasCalled = true;
    // Capture the message (best-effort)
    char buffer[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    mockFatalError_LastMessage = buffer;
    // Do not terminate; allow test to continue
}

// Helper assertion utilities (non-terminating)
static int g_TotalTests = 0;
static int g_PassedTests = 0;
static int g_FailedTests = 0;

#define ASSERT_TRUE(cond, msg) do { \
    ++g_TotalTests; \
    if (cond) { \
        ++g_PassedTests; \
    } else { \
        ++g_FailedTests; \
        std::cerr << "[FAIL] " << msg << " (line " << __LINE__ << ")\n"; \
    } \
} while(0)

#define ASSERT_EQ(a, b, msg) do { \
    ++g_TotalTests; \
    if ((a) == (b)) { \
        ++g_PassedTests; \
    } else { \
        ++g_FailedTests; \
        std::cerr << "[FAIL] " << msg << " | Expected: " << (b) << " Got: " << (a) << " (line " << __LINE__ << ")\n"; \
    } \
} while(0)

#define ASSERT_NEAR(a, b, eps, msg) do { \
    ++g_TotalTests; \
    if (std::fabs((a) - (b)) <= (eps)) { \
        ++g_PassedTests; \
    } else { \
        ++g_FailedTests; \
        std::cerr << "[FAIL] " << msg << " | Expected near: " << (b) << " Got: " << (a) << " (eps=" << (eps) << ", line " << __LINE__ << ")\n"; \
    } \
} while(0)

// Helpers to expose test controls
static void resetMocks() {
    mockCmsIT8SetDataDbl_ReturnValue = true;
    mockCmsIT8SetDataDbl_LastCol = nullptr;
    mockCmsIT8SetDataDbl_LastVal = 0.0;
    mockCmsIT8SetDataDbl_CallCount = 0;
    mockFatalError_WasCalled = false;
    mockFatalError_LastMessage.clear();
}

// Test 1: Quantization enabled -> Val should be quantized to nearest integer (Val + 0.5 floor)
static void test_SetCGATSfld_Quantize_On() {
    resetMocks();
    // Ensure environment uses quantization
    lQuantize = true;

    // Test value that should quantize from 2.3 -> floor(2.3+0.5) = floor(2.8) = 2
    SetCGATSfld("ChannelA", 2.3);

    // Verify that cmsIT8SetDataDbl() was called
    ASSERT_TRUE(mockCmsIT8SetDataDbl_CallCount > 0, "cmsIT8SetDataDbl should be called when SetCGATSfld runs");

    // Verify that quantization happened as expected
    ASSERT_EQ(static_cast<int>(mockCmsIT8SetDataDbl_LastVal), static_cast<int>(std::floor(2.3 + 0.5)),
              "Quantization should floor(Val+0.5)");
    // Additionally, check the value passed to the function equals quantized value
    ASSERT_EQ(mockCmsIT8SetDataDbl_LastVal, std::floor(2.3 + 0.5),
              "Passed Val should be quantized (floor(Val+0.5))");
}

// Test 2: Quantization disabled -> Val should be passed as-is
static void test_SetCGATSfld_Quantize_Off() {
    resetMocks();
    lQuantize = false;

    SetCGATSfld("ChannelB", 7.75);

    // Verify cmsIT8SetDataDbl was called and value is unmodified
    ASSERT_TRUE(mockCmsIT8SetDataDbl_CallCount > 0, "cmsIT8SetDataDbl should be called when SetCGATSfld runs with quantization off");
    ASSERT_EQ(mockCmsIT8SetDataDbl_LastVal, 7.75, "Value should be passed without quantization when lQuantize is false");
}

// Test 3: cmsIT8SetDataDbl returns failure -> FatalError should be invoked
static void test_SetCGATSfld_Failure_Path() {
    resetMocks();
    lQuantize = true;
    mockCmsIT8SetDataDbl_ReturnValue = false;

    // Call should trigger FatalError, but not terminate test
    SetCGATSfld("ChannelC", 5.5);

    // Verify that FatalError was invoked
    ASSERT_TRUE(mockFatalError_WasCalled, "FatalError should be called when cmsIT8SetDataDbl fails");
    // Optional: verify the message mentions the Col and CGATSoutFilename if available
    // We can't assert on CGATSoutFilename content reliably here, but we can ensure a message was formed
    ASSERT_TRUE(!mockFatalError_LastMessage.empty(), "FatalError should be called with a message");
}

// Simple runner
int main() {
    std::cout << "Running SetCGATSfld unit tests (no GTest, basic harness)" << std::endl;

    test_SetCGATSfld_Quantize_On();
    test_SetCGATSfld_Quantize_Off();
    test_SetCGATSfld_Failure_Path();

    std::cout << "Tests completed. Summary: "
              << g_PassedTests << " passed, "
              << g_FailedTests << " failed, "
              << g_TotalTests << " total." << std::endl;

    // If any tests failed, return non-zero to signal failure
    if (g_FailedTests > 0) return 1;
    return 0;
}