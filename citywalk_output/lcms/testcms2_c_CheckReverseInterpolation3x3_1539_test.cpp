// Test suite for CheckReverseInterpolation3x3 function in testcms2.c
// This C++11 test harness is designed to run without Google Test (GTest)
// and uses a lightweight, non-terminating assertion approach.
// It imports the focal function via C linkage and exercises its behavior.
//
// Step 1 (Candidate Keywords for understanding):
// - Lut (cmsPipeline) and clut (cmsStage) are created and linked.
// - Target, Result, Hint arrays hold RGB triplets (3 channels used here).
// - Table data defines a 2x3x3 CLUT (16-bit) used for reverse interpolation.
// - The function runs an evaluation loop from 0 to 100 to compute max error.
// - Branch coverage focuses on the early "zero" check and the final max-precision check.
// - Key local variables: Target, Result, Hint, max, err, i.
// These keywords help us validate that the test suite exercises the focal path.

#include <cstdint>
#include <iostream>
#include <cmath>
#include <testcms2.h>


// Access the focal C function from testcms2.c
extern "C" int CheckReverseInterpolation3x3(void);

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;
#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "[FAIL] " << __func__ << ": " << (msg) << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define ASSERT_FALSE(cond, msg) do { \
    if(cond) { \
        std::cerr << "[FAIL] " << __func__ << ": " << (msg) << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define ASSERT_EQ(a, b, msg) do { \
    if(!((a) == (b))) { \
        std::cerr << "[FAIL] " << __func__ << ": " << (msg) \
                  << " (expected " << (b) << ", got " << (a) << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

// Test 1: Basic sanity check - the focal function should return a non-zero value
// indicating a successful reverse interpolation check.
void test_CheckReverseInterpolation3x3_ReturnsNonZero()
{
    // Step 2: Validate true path is taken for standard environment
    int res = CheckReverseInterpolation3x3();
    ASSERT_TRUE(res != 0, "CheckReverseInterpolation3x3 should return non-zero on success");
}

// Test 2: Repeated invocations to ensure no state leakage or crashes across calls.
// The function should consistently return non-zero on successive runs.
void test_CheckReverseInterpolation3x3_MultipleCalls()
{
    for (int k = 0; k < 5; ++k) {
        int res = CheckReverseInterpolation3x3();
        ASSERT_TRUE(res != 0, "Repeated Call #" << k+1 << " should return non-zero");
    }
}

// Test 3: Access via function pointer to verify linkage and calling convention.
// This also asserts the function is callable through a pointer type.
void test_CheckReverseInterpolation3x3_ViaFunctionPointer()
{
    typedef int (*CheckFn)(void);
    CheckFn pf = &CheckReverseInterpolation3x3;
    int res = pf();
    ASSERT_TRUE(res != 0, "Function pointer call to CheckReverseInterpolation3x3 should return non-zero");
}

// Entry point for the small test suite
int main()
{
    std::cout << "Running tests for CheckReverseInterpolation3x3 ..." << std::endl;

    test_CheckReverseInterpolation3x3_ReturnsNonZero();
    test_CheckReverseInterpolation3x3_MultipleCalls();
    test_CheckReverseInterpolation3x3_ViaFunctionPointer();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " TEST(S) FAILED" << std::endl;
        return g_failures;
    }
}