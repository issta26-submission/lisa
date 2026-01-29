/*
Unit Test Suite for: cmsBool validDeviceClass(cmsProfileClassSignature cl)

What this test covers:
- Validates the behavior of validDeviceClass for all known and unknown class signatures.
- Ensures 0 is treated as TRUE (compatibility with older lcms versions).
- Verifies that all seven known class signatures return TRUE.
- Verifies that arbitrary/invalid values return FALSE.

Candidate Keywords (from the focal method and its immediate dependencies):
- cmsProfileClassSignature, cmsBool, TRUE, FALSE
- cmsSigInputClass, cmsSigDisplayClass, cmsSigOutputClass, cmsSigLinkClass
- cmsSigAbstractClass, cmsSigColorSpaceClass, cmsSigNamedColorClass
- (int)cl comparison, switch-case default handling

Dependencies:
- Requires the LittleCMS public API header to obtain type definitions and public tag constants:
  - cmsBool, TRUE, FALSE
  - cmsProfileClassSignature
  - cmsSigInputClass, cmsSigDisplayClass, cmsSigOutputClass, cmsSigLinkClass
  - cmsSigAbstractClass, cmsSigColorSpaceClass, cmsSigNamedColorClass

Notes:
- This test uses a minimal in-tree harness (no Google Test) and relies on the public C API.
- The test is written for C++11 compatibility; it uses simple, non-terminating assertions (EXPECT_*) that log failures but do not abort execution.
- If the implementation of cmsSig* constants differs across environments, adjust accordingly; the test asserts that these constants are recognized as valid class signatures by validDeviceClass.

Compilation guidance:
- Compile by linking against the library that provides the implementation of validDeviceClass (e.g., liblcms2.a/.so) and include its headers (lcms2.h).
- Example (conceptual): g++ -std=c++11 cmsio0_test.cpp -lcms2 -o cmsio0_test

Code:

#include <lcms2_internal.h>
#include <iostream>
#include <lcms2.h>
#include <cstdint>


// Public API of LittleCMS (defines cmsBool, TRUE/FALSE, cmsProfileClassSignature, and class sig constants)

// Ensure we reference the focal function with C linkage
extern "C" cmsBool validDeviceClass(cmsProfileClassSignature cl);

/*
 Simple non-terminating test harness (EXPECT_* style) for C++11.
 These do not abort on failure; they accumulate a count of failures and print diagnostics.
*/
static int g_failures = 0;

#define EXPECT_TRUE(expr) \
    do { if(!(expr)) { \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << " EXPECT_TRUE(" #expr ") failed\n"; \
        ++g_failures; \
    } } while(0)

#define EXPECT_FALSE(expr) \
    do { if((expr)) { \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << " EXPECT_FALSE(" #expr ") failed\n"; \
        ++g_failures; \
    } } while(0)

#define EXPECT_EQ(a, b) \
    do { auto _a = (a); auto _b = (b); \
         if(!(_a == _b)) { \
             std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ \
                       << " EXPECT_EQ(" #a ", " #b ") failed: " << _a << " != " << _b << "\n"; \
             ++g_failures; \
         } \
    } while(0)

int main() {
    // Test 1: 0 should be treated as TRUE per the implementation note
    // This covers the explicit cast check (int)cl == 0 path.
    EXPECT_TRUE(validDeviceClass(0));

    // Test 2: All known valid class signatures should return TRUE
    // We enumerate the seven documented valid class signatures.
    const cmsProfileClassSignature knowns[] = {
        cmsSigInputClass,
        cmsSigDisplayClass,
        cmsSigOutputClass,
        cmsSigLinkClass,
        cmsSigAbstractClass,
        cmsSigColorSpaceClass,
        cmsSigNamedColorClass
    };
    for (const auto& sig : knowns) {
        EXPECT_TRUE(validDeviceClass(sig));
    }

    // Test 3: Unknown/invalid values should return FALSE
    // Use a clearly invalid value (e.g., all bits set) to exercise default branch
    EXPECT_FALSE(validDeviceClass((cmsProfileClassSignature)0xFFFFFFFFu));

    // Test 4: Another arbitrary non-listed value should also yield FALSE
    EXPECT_FALSE(validDeviceClass((cmsProfileClassSignature)0x12345678u));

    // Optional: Re-check that 0 still yields TRUE for completeness
    EXPECT_TRUE(validDeviceClass((cmsProfileClassSignature)0));

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests passed for validDeviceClass.\n";
    } else {
        std::cout << "validDeviceClass test finished with " << g_failures << " failure(s).\n";
    }

    return g_failures;
}