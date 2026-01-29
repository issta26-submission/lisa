// Test suite for GetGridpoints(dwFlags) from fast_float_lab.c
// This test suite is designed for C++11 compilation without GoogleTest.
// It uses a lightweight, non-terminating assertion mechanism to maximize code coverage.

#include <iostream>
#include <string>
#include <cstdint>
#include <fast_float_internal.h>


// Type alias expected by the focal function.
// If the actual project uses a different typedef for cmsUInt32Number,
// this alias should still map to a 32-bit unsigned integer on common platforms.
typedef uint32_t cmsUInt32Number;

// Prototype of the focal function under test.
// The function is defined in fast_float_lab.c (C language linkage).
extern "C" int GetGridpoints(cmsUInt32Number dwFlags);

// If the project defines cmsFLAGS_HIGHRESPRECALC or cmsFLAGS_LOWRESPRECALC
// in an external header, they will be used by the actual implementation.
// To enable test compilation in environments where those macros are not yet
// defined, provide conservative fallback definitions. If the real values differ,
// the tests may need adjustment accordingly.
#ifndef cmsFLAGS_HIGHRESPRECALC
#define cmsFLAGS_HIGHRESPRECALC 0x00000100  // Assumed bit for High-Resolution Precalc
#endif

#ifndef cmsFLAGS_LOWRESPRECALC
#define cmsFLAGS_LOWRESPRECALC 0x00000080   // Assumed bit for Low-Resolution Precalc
#endif

// Simple, non-terminating test harness
static int gTotalTests = 0;
static int gFailedTests = 0;

// Helper to log failures with contextual test name
static void logFailure(const std::string& testName, const std::string& message, int got, int expected)
{
   ++gTotalTests;
    ++gFailedTests;
    std::cerr << "[FAILED] " << testName << ": " << message
              << " | got=" << got << " expected=" << expected << "\n";
}

// Helper to log passes
static void logPass(const std::string& testName)
{
    ++gTotalTests;
    std::cout << "[PASSED] " << testName << "\n";
}

// Assert equality for integers (non-terminating)
static void EXPECT_EQ_INT(const std::string& testName, int a, int b)
{
    if (a == b) {
        logPass(testName);
    } else {
        logFailure(testName, "values differ", a, b);
    }
}

// Test 1: First-branch extraction when 0x00FF0000 mask is set.
// Expected: return ((dwFlags >> 16) & 0xFF)
static void test_extraction_first_branch()
{
    // dwFlags with 0x00FF0000 bits set, expecting extraction of 0xAA
    cmsUInt32Number flags = 0x00AA0000; // (0x00AA0000 & 0x00FF0000) != 0
    int result = GetGridpoints(flags);
    EXPECT_EQ_INT("Extraction_FirstBranch", result, 0xAA);
}

// Test 2: First-branch should take precedence even if HighRes/LowRes flags are also set.
// Here we set first-branch bits together with cmsFLAGS_HIGHRESPRECALC.
// Expected: still extraction of the 0xFF-range value (not 66)
static void test_extraction_first_branch_with_other_bits()
{
    cmsUInt32Number flags = 0x00BB0000 | cmsFLAGS_HIGHRESPRECALC; // ensure first branch is chosen
    int result = GetGridpoints(flags);
    EXPECT_EQ_INT("Extraction_With_Other_Bits_FirstBranch", result, 0xBB);
}

// Test 3: HighResPrecalc path (no 0x00FF0000 bits set, high-res bit active).
// Expected: 66
static void test_highres_path()
{
    cmsUInt32Number flags = cmsFLAGS_HIGHRESPRECALC;
    // Ensure first-branch is not taken: clear 0x00FF0000 region (we rely on absence of those bits)
    // If cmsFLAGS_HIGHRESPRECALC accidentally shares bits with 0x00FF0000, this test could fail.
    // The test assumes the library's macro usage matches the development environment.
    // Note: If the real value differs, adjust cmsFLAGS_HIGHRESPRECALC accordingly.
    int result = GetGridpoints(flags);
    EXPECT_EQ_INT("HighResPath", result, 66);
}

// Test 4: LowResPrecalc path (no 0x00FF0000 bits set, low-res bit active, high-res not set).
// Expected: 33
static void test_lowres_path()
{
    cmsUInt32Number flags = cmsFLAGS_LOWRESPRECALC;
    int result = GetGridpoints(flags);
    EXPECT_EQ_INT("LowResPath", result, 33);
}

// Test 5: Default path (no special flags set).
// Expected: 51
static void test_default_path()
{
    cmsUInt32Number flags = 0;
    int result = GetGridpoints(flags);
    EXPECT_EQ_INT("DefaultPath", result, 51);
}

// Test 6: First-branch takes precedence over high/low-res when both are present.
// Expect extraction value (not 66/33) due to the 0x00FF0000 mask being set.
static void test_first_branch_overrides_rest()
{
    cmsUInt32Number flags = 0x00120000 | cmsFLAGS_HIGHRESPRECALC | cmsFLAGS_LOWRESPRECALC;
    int result = GetGridpoints(flags);
    // (dwFlags >> 16) & 0xFF -> (0x0012) & 0xFF = 0x12 = 18
    EXPECT_EQ_INT("FirstBranchOverridesRest", result, 0x12);
}

// Run all tests and print summary
int main()
{
    std::cout << "Starting GetGridpoints unit tests...\n";

    test_extraction_first_branch();
    test_extraction_first_branch_with_other_bits();
    test_highres_path();
    test_lowres_path();
    test_default_path();
    test_first_branch_overrides_rest();

    std::cout << "Unit tests completed. "
              << "Total: " << gTotalTests
              << ", Failures: " << gFailedTests << "\n";

    // Return non-zero if any test failed
    return (gFailedTests == 0) ? 0 : 1;
}