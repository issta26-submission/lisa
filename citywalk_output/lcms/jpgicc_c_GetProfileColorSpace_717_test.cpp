// Test suite for the focal method GetProfileColorSpace in jpgicc.c
// This test uses simple, self-contained mocks to override external C functions
// (cmsGetColorSpace and _cmsLCMScolorSpace) so that we can verify the behavior
// of GetProfileColorSpace without requiring the actual Little CMS library.
//
// Design notes aligned with the provided domain knowledge:
// - Import and rely only on standard library (i.e., std::cout, etc.).
// - Provide true/false branch coverage by exercising known, unknown, and null inputs.
// - Use non-terminating assertions (custom EXPECT_EQ macro) to continue tests on failure.
// - Access the focal function via C linkage to match the C implementation in jpgicc.c.

#include <jpeglib.h>
#include <iostream>
#include <iccjpeg.h>
#include <utils.h>


// Domain: Provide C linkage and type aliases matching the focal function's interface.
typedef void* cmsHPROFILE;
typedef unsigned int cmsColorSpaceSignature;

// Forward declaration of the function under test (C linkage)
extern "C" int GetProfileColorSpace(cmsHPROFILE hProfile);

// Mocks: declare the external functions that GetProfileColorSpace relies on.
// We implement these in this translation unit to control behavior for tests.
extern "C" cmsColorSpaceSignature cmsGetColorSpace(cmsHPROFILE hProfile);
extern "C" int _cmsLCMScolorSpace(cmsColorSpaceSignature cs);

// Local Mock Implementations

// Map given hProfile values to deterministic "signature" codes.
extern "C" cmsColorSpaceSignature cmsGetColorSpace(cmsHPROFILE hProfile) {
    // Test mapping strategy:
    // - hProfile == (void*)1  -> signature 12345
    // - hProfile == (void*)2  -> signature 200
    // - otherwise              -> signature 0
    if (hProfile == reinterpret_cast<void*>(1)) {
        return 12345u;
    } else if (hProfile == reinterpret_cast<void*>(2)) {
        return 200u;
    } else {
        return 0u;
    }
}

// Map the signature to the expected output code from the internal LCMS converter.
// - 12345  -> 7
// - 200    -> 3
// - others -> -1
extern "C" int _cmsLCMScolorSpace(cmsColorSpaceSignature cs) {
    if (cs == 12345u) return 7;
    if (cs == 200u) return 3;
    return -1;
}

// Lightweight test harness (non-terminating assertions)

static int g_failures = 0;
#define EXPECT_EQ(a, b, name) do {                                  \
    auto _a = (a); auto _b = (b);                                    \
    if (_a != _b) {                                                 \
        std::cerr << "[FAIL] " << name << ": " << _a                 \
                  << " != " << _b                                   \
                  << " (File: " << __FILE__ << ", Line: " << __LINE__ << ")\n"; \
        ++g_failures;                                               \
    } else {                                                        \
        std::cout << "[OK]   " << name << "\n";                    \
    }                                                               \
} while(0)

// Test 1: Profile 1 maps to signature 12345, which should map to 7.
void test_GetProfileColorSpace_Profile1() {
    // Arrange
    cmsHPROFILE p1 = reinterpret_cast<cmsHPROFILE>(1);
    // Act
    int result = GetProfileColorSpace(p1);
    // Assert
    EXPECT_EQ(result, 7, "GetProfileColorSpace(Profile1) should return 7");
}

// Test 2: Profile 2 maps to signature 200, which should map to 3.
void test_GetProfileColorSpace_Profile2() {
    // Arrange
    cmsHPROFILE p2 = reinterpret_cast<cmsHPROFILE>(2);
    // Act
    int result = GetProfileColorSpace(p2);
    // Assert
    EXPECT_EQ(result, 3, "GetProfileColorSpace(Profile2) should return 3");
}

// Test 3: Null profile should result in a fallback (-1) as no valid signature is present.
void test_GetProfileColorSpace_NullProfile() {
    // Arrange
    cmsHPROFILE nullP = nullptr;
    // Act
    int result = GetProfileColorSpace(nullP);
    // Assert
    EXPECT_EQ(result, -1, "GetProfileColorSpace(NullProfile) should return -1");
}

// Test 4: Unknown profile input should map to -1 (unrecognized signature).
void test_GetProfileColorSpace_UnknownProfile() {
    // Arrange
    cmsHPROFILE unknownP = reinterpret_cast<cmsHPROFILE>(999);
    // Act
    int result = GetProfileColorSpace(unknownP);
    // Assert
    EXPECT_EQ(result, -1, "GetProfileColorSpace(UnknownProfile) should return -1");
}

// Entry point to run all tests
int main() {
    std::cout << "Running GetProfileColorSpace unit tests (no GTest)..." << std::endl;

    test_GetProfileColorSpace_Profile1();
    test_GetProfileColorSpace_Profile2();
    test_GetProfileColorSpace_NullProfile();
    test_GetProfileColorSpace_UnknownProfile();

    std::cout << "Testing completed. Total failures: " << g_failures << std::endl;
    return g_failures; // Non-zero indicates some tests failed
}