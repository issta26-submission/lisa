// Minimal C++11 unit tests for CheckProofingIntersection in testcms2.c
// This test suite uses manual mocks (no gtest) to exercise both success and failure paths
// of the focal function by providing controlled aliases for required C library functions.

#include <iostream>
#include <testcms2.h>


// Domain-specific: provide C linkage for production functions used by CheckProofingIntersection
extern "C" {

// Typedefs to mirror the production environment (opaque pointers for mocks)
typedef void* cmsHPROFILE;
typedef void* cmsHTRANSFORM;
typedef void* cmsContext;
typedef unsigned int cmsUInt32Number;
typedef int cmsInt32Number;

// Macros used by the focal function
#define TYPE_RGB_FLT 0
#define TYPE_GRAY_FLT 0
#define INTENT_ABSOLUTE_COLORIMETRIC 0
#define cmsFLAGS_GAMUTCHECK 0x1
#define cmsFLAGS_SOFTPROOFING 0x2

// Prototypes of dependencies (as seen by the focal method)
cmsHPROFILE cmsCreate_sRGBProfile(void);
cmsHPROFILE Create_AboveRGB(void);
cmsHPROFILE cmsCreateNULLProfileTHR(cmsContext ContextID);
cmsHTRANSFORM cmsCreateProofingTransformTHR(
    cmsContext ContextID,
    cmsHPROFILE Input, int InputType,
    cmsHPROFILE Output, int OutputType,
    cmsUInt32Number Intent, cmsUInt32Number PrintIntent,
    unsigned long Flags);
void cmsCloseProfile(cmsHPROFILE h);
void cmsDeleteTransform(cmsHTRANSFORM xform);
cmsContext DbgThread(void);

// The focal function itself (declared so we can call it in tests)
int CheckProofingIntersection(void);
}

// ---------------- Mock Infrastructure ----------------
// Global flags and counters to verify behavior in tests
static bool g_mock_transform_return_null = false;

static int g_cmsCreateSrgbCalled = 0;
static int g_cmsCreateNULLCalled = 0;
static int g_CreateAboveRGBCalled = 0;
static int g_ProofingTransformCalled = 0;
static int g_CloseProfileCalled = 0;
static int g_DeleteTransformCalled = 0;

// Simple unique dummy objects to simulate opaque handles
static int g_dummy_srgb;
static int g_dummy_null;
static int g_dummy_above_rgb;
static int g_dummy_transform;

// Mocked function implementations (C linkage)
extern "C" {

// Mock: cmsCreate_sRGBProfile
cmsHPROFILE cmsCreate_sRGBProfile(void) {
    ++g_cmsCreateSrgbCalled;
    return (cmsHPROFILE)&g_dummy_srgb;
}

// Mock: Create_AboveRGB (above RGB gamut)
cmsHPROFILE Create_AboveRGB(void) {
    ++g_CreateAboveRGBCalled;
    return (cmsHPROFILE)&g_dummy_above_rgb;
}

// Mock: cmsCreateNULLProfileTHR
cmsHPROFILE cmsCreateNULLProfileTHR(cmsContext ContextID) {
    (void)ContextID; // unused in mock
    ++g_cmsCreateNULLCalled;
    return (cmsHPROFILE)&g_dummy_null;
}

// Mock: cmsCreateProofingTransformTHR
cmsHTRANSFORM cmsCreateProofingTransformTHR(
    cmsContext ContextID,
    cmsHPROFILE Input, int InputType,
    cmsHPROFILE Output, int OutputType,
    cmsUInt32Number Intent, cmsUInt32Number PrintIntent,
    unsigned long Flags) {

    (void)ContextID;
    (void)Input; (void)InputType;
    (void)Output; (void)OutputType;
    (void)Intent; (void)PrintIntent;
    (void)Flags;

    ++g_ProofingTransformCalled;
    if (g_mock_transform_return_null) {
        return (cmsHTRANSFORM)NULL;
    }
    return (cmsHTRANSFORM)&g_dummy_transform;
}

// Mock: cmsCloseProfile
void cmsCloseProfile(cmsHPROFILE h) {
    (void)h;
    ++g_CloseProfileCalled;
}

// Mock: cmsDeleteTransform
void cmsDeleteTransform(cmsHTRANSFORM xform) {
    (void)xform;
    ++g_DeleteTransformCalled;
}

// Mock: DbgThread
cmsContext DbgThread(void) {
    static int dummy = 0;
    (void)dummy;
    // Return a non-null context-like value
    return (cmsContext)&dummy;
}

} // extern "C"

// ---------------- Test Harness ----------------

static void reset_counters() {
    g_cmsCreateSrgbCalled = 0;
    g_cmsCreateNULLCalled = 0;
    g_CreateAboveRGBCalled = 0;
    g_ProofingTransformCalled = 0;
    g_CloseProfileCalled = 0;
    g_DeleteTransformCalled = 0;
    g_mock_transform_return_null = false;
    g_dummy_srgb = 0;
    g_dummy_null = 0;
    g_dummy_above_rgb = 0;
    g_dummy_transform = 0;
}

// Simple non-terminating assertion helper
static void expect_eq_int(const char* test, const char* expr, int a, int b, bool* ok) {
    if (a != b) {
        std::cerr << "FAIL " << test << ": " << expr << " expected " << b << " but got " << a << "\n";
        *ok = false;
    }
}

// Test 1: Success path when transform creation returns non-NULL
// Rationale: Verify that CheckProofingIntersection returns 1 and all involved APIs are exercised.
bool test_CheckProofingIntersection_Success() {
    reset_counters();
    g_mock_transform_return_null = false;

    int res = CheckProofingIntersection();

    bool ok = true;
    // Result should be success
    if (res != 1) {
        std::cerr << "Error: Expected CheckProofingIntersection() to return 1 on success, got " << res << "\n";
        ok = false;
    }

    // Validate that all dependent calls occurred exactly once (as per normal flow)
    if (g_cmsCreateSrgbCalled != 1) {
        std::cerr << "Mismatch: cmsCreate_sRGBProfile called " << g_cmsCreateSrgbCalled << " times\n";
        ok = false;
    }
    if (g_CreateAboveRGBCalled != 1) {
        std::cerr << "Mismatch: Create_AboveRGB called " << g_CreateAboveRGBCalled << " times\n";
        ok = false;
    }
    if (g_cmsCreateNULLCalled != 1) {
        std::cerr << "Mismatch: cmsCreateNULLProfileTHR called " << g_cmsCreateNULLCalled << " times\n";
        ok = false;
    }
    if (g_ProofingTransformCalled != 1) {
        std::cerr << "Mismatch: cmsCreateProofingTransformTHR called " << g_ProofingTransformCalled << " times\n";
        ok = false;
    }
    if (g_CloseProfileCalled != 3) {
        std::cerr << "Mismatch: cmsCloseProfile called " << g_CloseProfileCalled << " times\n";
        ok = false;
    }
    if (g_DeleteTransformCalled != 1) {
        std::cerr << "Mismatch: cmsDeleteTransform called " << g_DeleteTransformCalled << " times\n";
        ok = false;
    }

    return ok;
}

// Test 2: Failure path when transform creation returns NULL
// Rationale: Verify that CheckProofingIntersection returns 0 and does not call deleteTransform in this branch.
bool test_CheckProofingIntersection_Fail() {
    reset_counters();
    g_mock_transform_return_null = true;

    int res = CheckProofingIntersection();

    bool ok = true;
    // Result should be failure
    if (res != 0) {
        std::cerr << "Error: Expected CheckProofingIntersection() to return 0 on failure, got " << res << "\n";
        ok = false;
    }

    // Validate that the transform creation was attempted
    if (g_ProofingTransformCalled != 1) {
        std::cerr << "Mismatch: cmsCreateProofingTransformTHR called " << g_ProofingTransformCalled << " times\n";
        ok = false;
    }

    // Ensure deleteTransform was not called in failure path
    if (g_DeleteTransformCalled != 0) {
        std::cerr << "Error: cmsDeleteTransform should not be called when transform is NULL\n";
        ok = false;
    }

    // Still close profiles as per function's cleanup
    if (g_CloseProfileCalled != 3) {
        std::cerr << "Mismatch: cmsCloseProfile called " << g_CloseProfileCalled << " times (expected 3)\n";
        ok = false;
    }

    return ok;
}

// ---------------- Main: Run tests ----------------
int main() {
    int total = 0;
    int failures = 0;

    std::cout << "Running tests for CheckProofingIntersection...\n";

    // Test 1: Success path
    ++total;
    if (test_CheckProofingIntersection_Success()) {
        std::cout << "[OK] Test_CheckProofingIntersection_Success\n";
        ++failures;
    } else {
        std::cout << "[FAIL] Test_CheckProofingIntersection_Success\n";
    }

    // Test 2: Failure path
    ++total;
    if (test_CheckProofingIntersection_Fail()) {
        std::cout << "[OK] Test_CheckProofingIntersection_Fail\n";
        ++failures;
    } else {
        std::cout << "[FAIL] Test_CheckProofingIntersection_Fail\n";
    }

    if (failures == 0) {
        std::cout << "All tests passed (" << total << " tests).\n";
        return 0;
    } else {
        std::cout << failures << " test(s) failed out of " << total << ".\n";
        return 1;
    }
}