// Test suite for GetDevicelinkColorSpace in jpgicc.c using lightweight C++11 tests
// This test uses a mock fallback for cmsGetPCS and _cmsLCMScolorSpace to isolate
// the focal method from external dependencies.
// The tests are designed to run without Google Test (GTest) and rely on a simple
// runtime test harness in main().

#include <iccjpeg.h>
#include <cstdio>
#include <jpeglib.h>
#include <iostream>
#include <utils.h>
#include <cstdint>


// Forward declare the types used by the focal function.
// We assume cmsHPROFILE is an opaque pointer type in the real library.
typedef struct cmsHPROFILE_s* cmsHPROFILE;
typedef unsigned int cmsColorSpaceSignature;

// Provide C linkage for the mocks and the function under test (from jpgicc.c)
extern "C" {

// Prototypes expected by the focal method (as provided by the library headers)
cmsColorSpaceSignature cmsGetPCS(cmsHPROFILE hProfile);
int _cmsLCMScolorSpace(cmsColorSpaceSignature ProfileSpace);

// Prototype of the function under test (linking will resolve to jpgicc.c)
int GetDevicelinkColorSpace(cmsHPROFILE hProfile);

} // extern "C"

///////////////////////////////////////////////////////////////
// Mock implementations to isolate GetDevicelinkColorSpace
// These mocks map specific handle values to PCS signatures and
// map PCS signatures to device-link color space integers.
///////////////////////////////////////////////////////////////

// A simple helper to convert the cmsHPROFILE handle (opaque) to a small integer
static inline uintptr_t HandleToIndex(cmsHPROFILE h) {
    return reinterpret_cast<uintptr_t>(h);
}

// Mock: cmsGetPCS(hProfile) -> returns a PCS signature based on the handle value
extern "C" cmsColorSpaceSignature cmsGetPCS(cmsHPROFILE hProfile) {
    switch (HandleToIndex(hProfile)) {
        case 1:  // emulate a profile whose PCS is mapped to 0x01
            return 0x01;
        case 2:  // emulate a profile whose PCS is mapped to 0x02
            return 0x02;
        default:
            // Unknown handle -> default PCS
            return 0x00;
    }
}

// Mock: _cmsLCMScolorSpace(ProfileSpace) -> maps PCS to a device-space int
extern "C" int _cmsLCMScolorSpace(cmsColorSpaceSignature ProfileSpace) {
    switch (ProfileSpace) {
        case 0x01: return 42;   // arbitrary mapping for test verification
        case 0x02: return 99;   // another mapping for verification
        default:   return -1;   // indicate an unknown/unsupported PCS
    }
}

///////////////////////////////////////////////////////////////
// Lightweight test harness (no GTest)
///////////////////////////////////////////////////////////////

static int g_passed = 0;
static int g_failed = 0;

// Simple non-terminating assertion helper.
// Logs a message on failure and continues.
static void ASSERT_TRUE(bool cond, const char* test_desc) {
    if (cond) {
        std::cout << "[PASS] " << test_desc << std::endl;
        ++g_passed;
    } else {
        std::cerr << "[FAIL] " << test_desc << std::endl;
        ++g_failed;
    }
}

// Test 1: Ensure mapping for handle 1 yields 42 (via PCS 0x01 -> 42)
static void test_devicelinkcolorspace_handle1_maps_to_42() {
    cmsHPROFILE h = (cmsHPROFILE)(void*)1;
    int result = GetDevicelinkColorSpace(h);
    ASSERT_TRUE(result == 42, "GetDevicelinkColorSpace(handle=1) should map to 42");
}

// Test 2: Ensure mapping for handle 2 yields 99 (via PCS 0x02 -> 99)
static void test_devicelinkcolorspace_handle2_maps_to_99() {
    cmsHPROFILE h = (cmsHPROFILE)(void*)2;
    int result = GetDevicelinkColorSpace(h);
    ASSERT_TRUE(result == 99, "GetDevicelinkColorSpace(handle=2) should map to 99");
}

// Test 3: Ensure unknown PCS maps to -1 (unknown/default case)
static void test_devicelinkcolorspace_unknown_handles_maps_to_minus1() {
    cmsHPROFILE h = (cmsHPROFILE)(void*)3; // unknown handle -> PCS 0x00 -> -1
    int result = GetDevicelinkColorSpace(h);
    ASSERT_TRUE(result == -1, "GetDevicelinkColorSpace(handle=3) should map to -1 (unknown PCS)");
}

// Run all tests and report summary
static void run_all_tests() {
    std::cout << "Starting GetDevicelinkColorSpace unit tests (no GTest)..." << std::endl;
    test_devicelinkcolorspace_handle1_maps_to_42();
    test_devicelinkcolorspace_handle2_maps_to_99();
    test_devicelinkcolorspace_unknown_handles_maps_to_minus1();
    std::cout << "Tests passed: " << g_passed << ", failed: " << g_failed << std::endl;
}

// Entry point
int main() {
    run_all_tests();
    // Return non-zero if any test failed
    return (g_failed == 0) ? 0 : 1;
}