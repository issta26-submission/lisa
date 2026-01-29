// Lightweight C++11 test harness for png_icc_check_header (no GTest, no GMock)
// This test suite focuses on exercising the focal method with a variety of input
// profiles by supplying crafted ICC header-like data. It uses only the C++
 // standard library and minimal, non-intrusive stubs for required symbols.
// The goal is to validate return paths (0 for error, 1 for success) and ensure
// that true/false branches of predicate logic are exercised.

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Domain-adapted lightweight declarations to be compatible with the focal method's
// expected external interfaces. We assume the real implementation is linked in
// (e.g., libpng) and available at link time. We provide minimal test doubles for
// dependencies that the focal method uses for its internal checks.

// Types typically found in libpng-like environments (simplified for tests)
typedef unsigned char png_byte;
typedef png_byte* png_bytep;
typedef const png_byte* png_const_bytep;
typedef unsigned int png_uint_32;
typedef const char* png_const_charp;

// Forward declaration of the function under test (as provided by the project under test)
extern "C" int png_icc_check_header(png_const_structrp png_ptr, png_const_charp name,
   png_uint_32 profile_length,
   png_const_bytep profile, int color_type);

// Opaque struct pointer types (as libpng uses)
typedef struct png_struct png_struct;
typedef png_struct* png_structrp;
typedef const png_struct* png_const_structrp;

// Local, lightweight helpers to assemble ICC-like header data for tests

// Big-endian 32-bit read helper (mock for png_get_uint_32)
static inline png_uint_32 png_get_uint_32(png_const_bytep p)
{
    return ((png_uint_32)p[0] << 24) |
           ((png_uint_32)p[1] << 16) |
           ((png_uint_32)p[2] << 8)  |
           ((png_uint_32)p[3]);
}

// D50 illuminant placeholder (12 bytes) used by the header checker.
// The actual value in libpng might differ; tests tolerate a mismatch here since
// mismatches trigger a warning, not a hard failure in the code path we test.
static const png_byte D50_nCIEXYZ[12] = {
    0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00, 0x00,0x00,0x00,0x00
};

// PNG color mask constant (as used in libpng). We only rely on literal values in tests.
static const unsigned int PNG_COLOR_MASK_COLOR = 0x02;

// PNG sRGB intent limit (as used for a range check in code)
static const unsigned int PNG_sRGB_INTENT_LAST = 0xFFFF;

// Mocked error reporting hook used by the focal function.
// In the real project, png_icc_profile_error reports errors and often aborts
// longjmp; here we emulate it by returning 0 (failure) and allowing tests to
// observe that the function reported an error via return value.
static int g_last_error_encountered = 0;
static std::string g_last_error_reason;

extern "C" int png_icc_profile_error(png_const_structrp png_ptr, png_const_charp name,
   png_uint_32 value, png_const_charp reason)
{
    (void)png_ptr; (void)name; (void)value;
    if (reason) {
        g_last_error_reason = reason;
    } else {
        g_last_error_reason.clear();
    }
    g_last_error_encountered = 1;
    // Return 0 to indicate an error (consistent with how libpng typically reports errors)
    return 0;
}

// Helper to reset the error capture state before each test
static void reset_error_state() {
    g_last_error_encountered = 0;
    g_last_error_reason.clear();
}

// Convenience to build 32-bit values into profile buffer (big-endian)
static void put_uint32_be(std::vector<unsigned char>& buf, size_t offset, uint32_t v)
{
    buf[offset + 0] = (unsigned char)((v >> 24) & 0xFF);
    buf[offset + 1] = (unsigned char)((v >> 16) & 0xFF);
    buf[offset + 2] = (unsigned char)((v >> 8) & 0xFF);
    buf[offset + 3] = (unsigned char)(v & 0xFF);
}

// Test 1: Success path - minimal header with RGB color space on RGB-like color type
// Expect: png_icc_check_header returns 1 (success) under non-fatal warnings.
static bool test_success_path_rgb_on_rgb() {
    reset_error_state();

    // Prepare a 144-byte profile (132 header + 12 for 1 tag)
    const uint32_t profile_length = 144;
    std::vector<unsigned char> profile(profile_length, 0);

    // 0-3: length
    put_uint32_be(profile, 0, profile_length);
    // 8: reserved/predicted usage - set to 0 (temp <= 3)
    profile[8] = 0;

    // 128-131: tag count (temp)
    put_uint32_be(profile, 128, 1);

    // 64-67: rendering intent
    put_uint32_be(profile, 64, 0x00000000); // within valid range (<0xffff)

    // 36-39: signature 'ascp'
    profile[36] = 'a'; profile[37] = 's'; profile[38] = 'c'; profile[39] = 'p';

    // 68-79: illuminant (12 bytes) – copy from D50_nCIEXYZ to simulate a match
    std::memcpy(profile.data() + 68, D50_nCIEXYZ, 12);

    // 16-19: data colour space field -> 'RGB '
    put_uint32_be(profile, 16, 0x52474220);

    // 12-15: device class -> 'scnr' (scanner)
    put_uint32_be(profile, 12, 0x73636e72);

    // 20-23: PCS encoding -> 'XYZ '
    put_uint32_be(profile, 20, 0x58595a20);

    // 0-3: ensure length matches (already set), others left zero.

    // Call the focal method (color_type uses RGB color)
    int ret = png_icc_check_header(nullptr, "test", profile_length, profile.data(), 2 /* RGB */);

    if (ret == 1) {
        std::cout << "test_success_path_rgb passed: returned 1\n";
        return true;
    } else {
        std::cout << "test_success_path_rgb failed: returned " << ret << "\n";
        return false;
    }
}

// Test 2: Mismatched length reported by header (length check)
// Expect: returns 0 (error) due to length mismatch.
static bool test_length_mismatch() {
    reset_error_state();

    const uint32_t profile_length = 200;
    std::vector<unsigned char> profile(profile_length, 0);

    // 0-3: length mismatch (store 199 but length is 200)
    put_uint32_be(profile, 0, 199);

    // Minimal valid fields to reach length check:
    // 8: set to 0 (not >3)
    profile[8] = 0;

    // 36-39: signature must be valid to avoid earlier exit
    profile[36] = 'a'; profile[37] = 's'; profile[38] = 'c'; profile[39] = 'p';
    // 16-19: color space 'RGB ' (to avoid color-space errors)
    put_uint32_be(profile, 16, 0x52474220);

    // 12-15: device class 'scnr'
    put_uint32_be(profile, 12, 0x73636e72);

    // 20-23: PCS encoding 'XYZ '
    put_uint32_be(profile, 20, 0x58595a20);

    int ret = png_icc_check_header(nullptr, "test_len_mismatch", profile_length, profile.data(), 2);

    if (ret == 0) {
        std::cout << "test_length_mismatch passed: returned 0 as expected\n";
        return true;
    } else {
        std::cout << "test_length_mismatch failed: returned " << ret << "\n";
        return false;
    }
}

// Test 3: Invalid length due to profile_length not being aligned with 4-byte rule when temp > 3
// Expect: returns 0 (error).
static bool test_invalid_length_due_to_alignment() {
    reset_error_state();

    // profile_length not multiple of 4; set to 133
    const uint32_t profile_length = 133;
    std::vector<unsigned char> profile(profile_length, 0);

    // 0-3: length
    put_uint32_be(profile, 0, profile_length);

    // 8: set to 4 to trigger "temp > 3" condition
    profile[8] = 4; // triggers alignment check when profile_length & 3 != 0

    // Minimal valid fields to avoid earlier exits except alignment:
    // 36-39: signature 'ascp'
    profile[36] = 'a'; profile[37] = 's'; profile[38] = 'c'; profile[39] = 'p';
    // 16-19: color space 'RGB '
    put_uint32_be(profile, 16, 0x52474220);
    // 12-15: device class 'scnr'
    put_uint32_be(profile, 12, 0x73636e72);
    // 20-23: PCS encoding 'XYZ '
    put_uint32_be(profile, 20, 0x58595a20);

    // Call
    int ret = png_icc_check_header(nullptr, "test_invalid_align", profile_length, profile.data(), 2);

    if (ret == 0) {
        std::cout << "test_invalid_length_due_to_alignment passed: returned 0\n";
        return true;
    } else {
        std::cout << "test_invalid_length_due_to_alignment failed: returned " << ret << "\n";
        return false;
    }
}

// Test 4: Invalid signature (checksum/signature mismatch) should fail early
// Expect: returns 0 (error) without requiring D50 correctness.
static bool test_invalid_signature() {
    reset_error_state();

    const uint32_t profile_length = 144;
    std::vector<unsigned char> profile(profile_length, 0);

    // 0-3: length
    put_uint32_be(profile, 0, profile_length);

    // 8: 0 (no alignment trigger)
    profile[8] = 0;

    // 36-39: invalid signature
    profile[36] = 'b'; profile[37] = 'a'; profile[38] = 'd'; profile[39] = 'x';

    // 16-19: color space 'RGB '
    put_uint32_be(profile, 16, 0x52474220);
    // 12-15: device class 'scnr'
    put_uint32_be(profile, 12, 0x73636e72);
    // 20-23: PCS encoding 'XYZ '
    put_uint32_be(profile, 20, 0x58595a20);

    int ret = png_icc_check_header(nullptr, "test_invalid_signature", profile_length, profile.data(), 2);

    if (ret == 0) {
        std::cout << "test_invalid_signature passed: returned 0\n";
        return true;
    } else {
        std::cout << "test_invalid_signature failed: returned " << ret << "\n";
        return false;
    }
}

// Test 5: Invalid embedded Abstract ICC profile class (abst) should error
// Expect: returns 0 (error).
static bool test_embedded_abstract_profile_class() {
    reset_error_state();

    const uint32_t profile_length = 144;
    std::vector<unsigned char> profile(profile_length, 0);

    put_uint32_be(profile, 0, profile_length);
    profile[8] = 0;
    put_uint32_be(profile, 36, 0x61637370); // 'ascp'
    put_uint32_be(profile, 12, 0x61627374); // 'abst' (invalid embedded Abstract ICC profile)

    // 16-19: color space 'RGB '
    put_uint32_be(profile, 16, 0x52474220);
    // 20-23: PCS 'XYZ '
    put_uint32_be(profile, 20, 0x58595a20);

    int ret = png_icc_check_header(nullptr, "test_absent_or_abst", profile_length, profile.data(), 2);

    if (ret == 0) {
        std::cout << "test_embedded_abstract_profile_class passed: returned 0\n";
        return true;
    } else {
        std::cout << "test_embedded_abstract_profile_class failed: returned " << ret << "\n";
        return false;
    }
}

// Test 6: NamedColor class (nmcl) should emit a warning but not fail the check
// Expect: returns 1 (success) under the test construct.
static bool test_named_color_class_warns_but_passes() {
    reset_error_state();

    const uint32_t profile_length = 144;
    std::vector<unsigned char> profile(profile_length, 0);

    put_uint32_be(profile, 0, profile_length);
    profile[8] = 0;
    put_uint32_be(profile, 36, 0x61637370); // 'ascp'
    put_uint32_be(profile, 12, 0x6e6d636c); // 'nmcl'
    // 16-19: RGB space
    put_uint32_be(profile, 16, 0x52474220);
    // 20-23: PCS 'XYZ '
    put_uint32_be(profile, 20, 0x58595a20);

    int ret = png_icc_check_header(nullptr, "test_nmcl_warns", profile_length, profile.data(), 2);

    if (ret == 1) {
        std::cout << "test_named_color_class_warns_but_passes passed: returned 1\n";
        return true;
    } else {
        std::cout << "test_named_color_class_warns_but_passes failed: returned " << ret << "\n";
        return false;
    }
}

// Test 7: Unrecognized ICC profile class should not crash; treat as pass in this test.
// Expect: returns 1 (success) if other fields are set to pass.
static bool test_unrecognized_profile_class_passes() {
    reset_error_state();

    const uint32_t profile_length = 144;
    std::vector<unsigned char> profile(profile_length, 0);

    put_uint32_be(profile, 0, profile_length);
    profile[8] = 0;
    // Unknown class 0x12345678
    put_uint32_be(profile, 12, 0x12345678);
    // 36-39: signature
    profile[36] = 'a'; profile[37] = 's'; profile[38] = 'c'; profile[39] = 'p';
    // 16-19: color space 'RGB '
    put_uint32_be(profile, 16, 0x52474220);
    // 20-23: PCS 'XYZ '
    put_uint32_be(profile, 20, 0x58595a20);

    int ret = png_icc_check_header(nullptr, "test_unrecognized_class", profile_length, profile.data(), 2);

    if (ret == 1) {
        std::cout << "test_unrecognized_profile_class_passes passed: returned 1\n";
        return true;
    } else {
        std::cout << "test_unrecognized_profile_class_passes failed: returned " << ret << "\n";
        return false;
    }
}

int main() {
    // Run each test in sequence, reporting outcomes.
    bool all_passed = true;

    all_passed &= test_success_path_rgb_on_rgb();
    all_passed &= test_length_mismatch();
    all_passed &= test_invalid_length_due_to_alignment();
    all_passed &= test_invalid_signature();
    all_passed &= test_embedded_abstract_profile_class();
    all_passed &= test_named_color_class_warns_but_passes();
    all_passed &= test_unrecognized_profile_class_passes();

    if (all_passed) {
        std::cout << "\nAll tests passed.\n";
        return 0;
    } else {
        std::cout << "\nSome tests FAILED. See output above for details.\n";
        return 1;
    }
}