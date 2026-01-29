/*
  Unit test suite for DecideLUTtypeA2B in cmstypes.c
  - Uses a lightweight in-process test harness (no GoogleTest)
  - Covers all branches of DecideLUTtypeA2B:
      ICCVersion < 4.0 with Lut.SaveAs8Bits = true  -> cmsSigLut8Type
      ICCVersion < 4.0 with Lut.SaveAs8Bits = false -> cmsSigLut16Type
      ICCVersion >= 4.0 (any Lut.SaveAs8Bits)       -> cmsSigLutAtoBType
  - Tests are implemented as standalone procedures invoked from main().
  - All tests use non-terminating assertions (log failures and continue).
  - Assumes lcms2 public/API headers are available (lcms2.h).
  - This file should compile with C++11 and link against the lcms2 library.
*/

#include <lcms2_internal.h>
#include <iostream>
#include <lcms2.h>
#include <cstring>


// Domain knowledge: include the public LCMS API to access necessary types/constants.
// The library is C, so ensure proper linkage in C++.
extern "C" {
}

// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_EQ(actual, expected, msg) do { \
    ++g_tests_run; \
    if ((actual) != (expected)) { \
        ++g_tests_failed; \
        std::cerr << "[FAIL] " << (msg) << " | Expected: " << (expected) \
                  << ", Actual: " << (actual) << std::endl; \
    } \
} while (0)

#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "[FAIL] " << (msg) << std::endl; \
    } \
} while (0)

// Forward declare the focal function (from library).
// The header lcms2.h generally provides this prototype with C linkage.
// We assert the function uses C linkage via extern "C" automatically by the header.
extern cmsTagTypeSignature DecideLUTtypeA2B(cmsFloat64Number ICCVersion, const void *Data);

void test_DecideLUTtypeA2B_Pre4_8bit() {
    // Arrange: create a cmsPipeline-like object and set SaveAs8Bits = true
    cmsPipeline Lut;
    std::memset(&Lut, 0, sizeof(Lut));
    // Access to the member SaveAs8Bits depends on LCMS version;
    // assume it is a writable integral/bool-like field as used by the focal method.
    Lut.SaveAs8Bits = 1;  // true

    // Act
    cmsTagTypeSignature res = DecideLUTtypeA2B(3.5, &Lut);

    // Assert
    EXPECT_EQ(res, cmsSigLut8Type, "ICCVersion 3.5 with SaveAs8Bits=true should yield LUT8 type");
}

void test_DecideLUTtypeA2B_Pre4_16bit() {
    // Arrange: Lut with SaveAs8Bits = false
    cmsPipeline Lut;
    std::memset(&Lut, 0, sizeof(Lut));
    Lut.SaveAs8Bits = 0;  // false

    // Act
    cmsTagTypeSignature res = DecideLUTtypeA2B(3.0, &Lut);

    // Assert
    EXPECT_EQ(res, cmsSigLut16Type, "ICCVersion 3.0 with SaveAs8Bits=false should yield LUT16 type");
}

void test_DecideLUTtypeA2B_Post4_AtoB() {
    // Arrange: ICCVersion >= 4.0, Lut.SaveAs8Bits can be either value
    cmsPipeline Lut;
    std::memset(&Lut, 0, sizeof(Lut));
    Lut.SaveAs8Bits = 1;  // 8-bit path should be ignored in >=4.0

    // Act
    cmsTagTypeSignature res1 = DecideLUTtypeA2B(4.0, &Lut);
    cmsTagTypeSignature res2 = DecideLUTtypeA2B(4.2, &Lut);

    // Assert
    EXPECT_EQ(res1, cmsSigLutAtoBType, "ICCVersion 4.0 should yield LUT A-to-B type");
    EXPECT_EQ(res2, cmsSigLutAtoBType, "ICCVersion 4.2 should yield LUT A-to-B type");
}

int main() {
    // Run tests
    test_DecideLUTtypeA2B_Pre4_8bit();
    test_DecideLUTtypeA2B_Pre4_16bit();
    test_DecideLUTtypeA2B_Post4_AtoB();

    // Summary
    std::cout << "Total tests run: " << g_tests_run << std::endl;
    if (g_tests_failed == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cerr << g_tests_failed << " test(s) FAILED." << std::endl;
        return 1;
    }
}