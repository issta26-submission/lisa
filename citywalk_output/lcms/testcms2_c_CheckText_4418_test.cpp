// testcms2_cpp_suite.cpp
// A C++-based unit test harness for the focal method CheckText
// Uses the provided C API (LittleCMS-like) to verify correct behavior.
// No GTest is used. A lightweight, non-terminating assertion approach is employed.

#include <cstring>
#include <iostream>
#include <testcms2.h>
#include <lcms2.h>
#include <cstdint>


// Include the library headers (C API)
extern "C" {
}

// Include test helpers (as provided in the project)
extern "C" {
    // Focal method under test (defined in testcms2.c as per the focal method)
    cmsInt32Number CheckText(cmsInt32Number Pass, cmsHPROFILE hProfile, cmsTagSignature tag);

    // Test helper to create a sample profile for tests
    cmsHPROFILE Create_Gray22(void);
    void cmsCloseProfile(cmsHPROFILE);

    // Some test library may provide more helpers; the above are sufficient for this suite.
}

// Lightweight non-terminating assertion/logging helpers
static int g_test_failures = 0;

static void log_result(const char* test_name, bool ok)
{
    if (ok) {
        std::cout << "[OK]   " << test_name << std::endl;
    } else {
        std::cerr << "[FAILED] " << test_name << std::endl;
        ++g_test_failures;
    }
}

// Helper to construct an MLU tag signature for "mluc" (MultiLocalizedUnicode)
static cmsTagSignature MLUC_TAG(void)
{
    // Construct tag signature for 'mluc' in a portable way:
    // Using little-endian layout assumption commonly used in ICC profiles:
    // Signature = ('m') | ('l' << 8) | ('u' << 16) | ('c' << 24)
    return (cmsTagSignature)(
        (static_cast<uint32_t>('m')) |
        (static_cast<uint32_t>('l') << 8)  |
        (static_cast<uint32_t>('u') << 16) |
        (static_cast<uint32_t>('c') << 24)
    );
}

// Test 1: Verify that Pass = 1 writes a MultiLocalizedUnicode tag successfully
// Expected: non-zero return value indicates success.
static bool test_CheckText_Pass1_WritesMLU(void)
{
    // Create a test profile
    cmsHPROFILE hProfile = Create_Gray22();
    if (hProfile == nullptr) {
        log_result("CheckText Pass1 - profile creation", false);
        return false;
    }

    // Use the MLUC tag for the MLU storage
    cmsTagSignature tag = MLUC_TAG();

    // Invoke the focal method with Pass = 1
    cmsInt32Number rc = CheckText(1, hProfile, tag);

    bool ok = (rc != 0);

    log_result("CheckText Pass1_WritesMLU", ok);

    // Cleanup
    cmsCloseProfile(hProfile);
    return ok;
}

// Test 2: Verify that Pass = 2 reads back the previously written MLU data correctly
// Expected: returns TRUE (non-zero) when data is present.
static bool test_CheckText_Pass2_ReadBack(void)
{
    cmsHPROFILE hProfile = Create_Gray22();
    if (hProfile == nullptr) {
        log_result("CheckText Pass2_ReadBack - profile creation", false);
        return false;
    }

    cmsTagSignature tag = MLUC_TAG();

    // First, write the MLU tag
    cmsInt32Number rc_write = CheckText(1, hProfile, tag);
    bool wrote = (rc_write != 0);

    // Then, read back
    cmsInt32Number rc_read = CheckText(2, hProfile, tag);
    bool read_success = (rc_read != 0);

    bool ok = wrote && read_success;

    log_result("CheckText Pass2_ReadBack", ok);

    cmsCloseProfile(hProfile);
    return ok;
}

// Test 3: Verify that Pass = 2 on a profile with no MLU tag set returns 0 (failure to read)
// Expected: false (rc == 0)
static bool test_CheckText_Pass2_NoTag(void)
{
    cmsHPROFILE hProfile = Create_Gray22();
    if (hProfile == nullptr) {
        log_result("CheckText Pass2_NoTag - profile creation", false);
        return false;
    }

    cmsTagSignature tag = MLUC_TAG();

    // Do not write first; attempt to read
    cmsInt32Number rc_read = CheckText(2, hProfile, tag);
    bool ok = (rc_read == 0);

    log_result("CheckText Pass2_NoTag (read without prior write)", ok);

    cmsCloseProfile(hProfile);
    return ok;
}

// Test 4: Verify that an unknown Pass value results in 0 (default case)
static bool test_CheckText_UnknownPass(void)
{
    cmsHPROFILE hProfile = Create_Gray22();
    if (hProfile == nullptr) {
        log_result("CheckText UnknownPass - profile creation", false);
        return false;
    }

    cmsTagSignature tag = MLUC_TAG();

    cmsInt32Number rc = CheckText(9999, hProfile, tag);
    bool ok = (rc == 0);

    log_result("CheckText UnknownPass (default path)", ok);

    cmsCloseProfile(hProfile);
    return ok;
}

// Main test runner
int main(void)
{
    std::cout << "Running CheckText unit tests (C API via C++ harness)..." << std::endl;

    bool t1 = test_CheckText_Pass1_WritesMLU();
    bool t2 = test_CheckText_Pass2_ReadBack();
    bool t3 = test_CheckText_Pass2_NoTag();
    bool t4 = test_CheckText_UnknownPass();

    // Summarize
    int total = 4;
    int passed = (!g_test_failures); // Not strictly accurate since g_test_failures is aggregated; recompute:
    // Recompute accurately
    // We'll simply statically count based on per-test returns
    int success_count = 0;
    success_count += t1 ? 1 : 0;
    success_count += t2 ? 1 : 0;
    success_count += t3 ? 1 : 0;
    success_count += t4 ? 1 : 0;

    std::cout << "Tests passed: " << success_count << " / " << total << std::endl;

    if (g_test_failures > 0) {
        std::cerr << g_test_failures << " test(s) failed." << std::endl;
        return 1;
    }
    return 0;
}