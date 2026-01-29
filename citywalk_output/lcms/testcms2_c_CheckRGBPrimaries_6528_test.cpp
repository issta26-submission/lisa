/*
   Lightweight C++11 test suite for the focal method CheckRGBPrimaries
   without using any external test framework (GTest, etc.).
   The tests rely on stubs for the external dependencies used by
   the focal method in testcms2.c to achieve deterministic behavior.

   How this works:
   - We provide minimal C/C++ compliant stubs for the CMS library calls
     used by CheckRGBPrimaries (types, functions, and a tiny fake backend).
   - We implement two core scenarios:
     1) True path: primaries match expected values, returns 1.
     2) False path: force a failure in one of the IsGoodFixed15_16 checks, returns 0.
   - We also include a third test to simulate profile creation failure,
     ensuring the function returns 0 early.
   - Each test is documented with comments describing intent and coverage.
   - The test harness prints per-test results and a final summary.

   Important: This file should be compiled together with testcms2.c
   (which contains CheckRGBPrimaries) in a C++11-capable toolchain.
*/

#include <cstring>
#include <cstdio>
#include <testcms2.h>
#include <cassert>
#include <limits.h>
#include <cstdarg>
#include <cmath>


// Forward declaration of the focal function under test (from testcms2.c)
extern "C" int CheckRGBPrimaries(void);

// Domain-specific minimal type/macro shims to mirror the CMS API used by CheckRGBPrimaries.
extern "C" {

// Basic typedefs used by the focal code
typedef void* cmsContext;
typedef void* cmsHPROFILE;
typedef unsigned int cmsUInt32Number;
typedef int cmsBool;

#define INTENT_ABSOLUTE_COLORIMETRIC 0
#define TRUE 1
#define FALSE 0

// CMS color-space structures (minimal subset for tests)
struct cmsCIEXYZ {
    double X, Y, Z;
};
struct cmsCIEXYZTRIPLE {
    struct cmsCIEXYZ Red;
    struct cmsCIEXYZ Green;
    struct cmsCIEXYZ Blue;
};
struct cmsCIExyY {
    double x, y, Y;
};
struct cmsCIExyYTRIPLE {
    struct cmsCIExyY Red;
    struct cmsCIExyY Green;
    struct cmsCIExyY Blue;
};

// Function prototypes expected by the focal method
void cmsSetAdaptationState(int);
cmsContext DbgThread(void);
cmsHPROFILE cmsCreate_sRGBProfileTHR(void*);
void cmsCloseProfile(cmsHPROFILE);
cmsBool GetProfileRGBPrimaries(cmsHPROFILE hProfile,
                               cmsCIEXYZTRIPLE* result,
                               cmsUInt32Number intent);
void cmsXYZ2xyY(struct cmsCIExyY* xyY, const struct cmsCIEXYZ* XYZ);

// Auxiliary helpers used by the focal method
cmsBool IsGoodFixed15_16(const char *title, double in, double out);
void Fail(const char* frm, ...);

} // extern "C"

// Global controls for test stubs (to drive deterministic behavior)
static cmsBool g_primariesAvailable = TRUE;      // controls GetProfileRGBPrimaries success
static cmsBool g_createProfileSucceeds = TRUE;   // controls cmsCreate_sRGBProfileTHR success
static int     g_forceFix15_16_at_call = 0;      // 0 = no force; otherwise force at given call index (1-based)
static int     g_current_IsGood_call = 0;          // counts calls to IsGoodFixed15_16 per test
static int     g_last_fail_len = 0;
static char    g_last_fail_msg[256];

// Test primaries chosen to yield exact xy values when converted.
// We want: Red.x=0.64, Red.y=0.33; Green.x=0.30, Green.y=0.60; Blue.x=0.15, Blue.y=0.06
// This corresponds to XYZ sets summing to 1.0 for each primary.
static struct cmsCIEXYZ g_PrimaryRed   = {0.64, 0.33, 0.03};
static struct cmsCIEXYZ g_PrimaryGreen = {0.30, 0.60, 0.10};
static struct cmsCIEXYZ g_PrimaryBlue  = {0.15, 0.06, 0.79};

// Local storage of primary XYZ triplet used by GetProfileRGBPrimaries
static cmsCIEXYZTRIPLE g_TestTripXYZ;

// Implementations of the stubs (C linkage to match testcms2.c)

extern "C" {

void cmsSetAdaptationState(int s) {
    // No-op for test; placeholder to mirror API
    (void)s;
}

cmsContext DbgThread(void) {
    // Return a non-null dummy context
    return reinterpret_cast<cmsContext>(0x1);
}

cmsHPROFILE cmsCreate_sRGBProfileTHR(void* /*thread*/) {
    // Return a non-null profile handle if allowed
    if (!g_createProfileSucceeds) return nullptr;
    return reinterpret_cast<cmsHPROFILE>(reinterpret_cast<void*>(0xDEADBEEF));
}

void cmsCloseProfile(cmsHPROFILE) {
    // No-op for test; placeholder
}

cmsBool GetProfileRGBPrimaries(cmsHPROFILE /*hProfile*/,
                               cmsCIEXYZTRIPLE* result,
                               cmsUInt32Number /*intent*/) {
    if (!g_primariesAvailable) return FALSE;
    // Copy prepared test primaries to the result
    if (result) {
        result->Red   = g_TestTripXYZ.Red;
        result->Green = g_TestTripXYZ.Green;
        result->Blue  = g_TestTripXYZ.Blue;
    }
    return TRUE;
}

void cmsXYZ2xyY(struct cmsCIExyY* xyY, const struct cmsCIEXYZ* XYZ) {
    if (!xyY || !XYZ) return;
    double denom = XYZ->X + XYZ->Y + XYZ->Z;
    if (denom == 0.0) {
        xyY->x = 0.0;
        xyY->y = 0.0;
        xyY->Y = 0.0;
        return;
    }
    xyY->x = XYZ->X / denom;
    xyY->y = XYZ->Y / denom;
    // Y component is not used by the primary checks, but we set it sensibly
    xyY->Y = XYZ->Y;
}

cmsBool IsGoodFixed15_16(const char * /*title*/, double in, double out) {
    // Reset counter per test if needed (not strictly necessary here)
    (void)out;
    // Increment call counter
    g_current_IsGood_call++;

    // Optional forced failure to exercise false branch during tests
    if (g_forceFix15_16_at_call > 0 && g_current_IsGood_call == g_forceFix15_16_at_call) {
        return FALSE;
    }

    // Normal behavior: accept small tolerance around the expected value
    double diff = std::fabs(in - out);
    const double kTol = 1e-3;
    return (diff <= kTol) ? TRUE : FALSE;
}

void Fail(const char* frm, ...) {
    // Capture last failure string (non-terminating)
    va_list args;
    va_start(args, frm);
    vsnprintf(g_last_fail_msg, sizeof(g_last_fail_msg), frm, args);
    va_end(args);
    g_last_fail_len = (int)std::strlen(g_last_fail_msg);
}

} // extern "C"

// Helper to set the test primaries to the known-good set
static void SetTestPrimaries() {
    g_TestTripXYZ.Red = g_PrimaryRed;
    g_TestTripXYZ.Green = g_PrimaryGreen;
    g_TestTripXYZ.Blue = g_PrimaryBlue;
}

// Simple per-test assert helper (non-terminating)
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT(cond, msg) do { \
    if (!(cond)) { \
        std::printf("ASSERTION FAILED: %s\n", (msg)); \
        ++g_tests_failed; \
    } else { \
        ++g_tests_passed; \
    } \
} while(0)


// Test 1: Verify that the true path returns TRUE (1) and does not trigger Fail
static void test_CheckRGBPrimaries_true_path() {
    // Prepare environment for a clean, successful run
    g_primariesAvailable = TRUE;
    g_createProfileSucceeds = TRUE;
    g_forceFix15_16_at_call = 0;
    g_current_IsGood_call = 0;
    g_last_fail_len = 0;
    g_last_fail_msg[0] = '\0';
    g_TestTripXYZ = g_TestTripXYZ; // silence unused warning

    SetTestPrimaries();
    // Also ensure the function will not falsely fail due to creation
    // (no forced failures)
    int ret = CheckRGBPrimaries();
    TEST_ASSERT(ret == 1, "CheckRGBPrimaries should return TRUE on good primaries");
    TEST_ASSERT(g_last_fail_len == 0, "No Fail() should be invoked on success");
}

// Test 2: Verify that forcing a particular IsGoodFixed15_16 call to fail triggers FALSE path
static void test_CheckRGBPrimaries_false_path_due_to_primaries() {
    // Prepare environment
    g_primariesAvailable = TRUE;
    g_createProfileSucceeds = TRUE;
    g_forceFix15_16_at_call = 3; // Force a failure on the 3rd IsGoodFixed15_16 call
    g_current_IsGood_call = 0;
    g_last_fail_len = 0;
    g_last_fail_msg[0] = '\0';

    SetTestPrimaries();
    int ret = CheckRGBPrimaries();
    TEST_ASSERT(ret == 0, "CheckRGBPrimaries should return FALSE when a primary fails its check");
    // We do not rely on the exact message, but ensure a Fail() happened (non-terminating)
    TEST_ASSERT(g_last_fail_len > 0, "Fail() should have been invoked on failure");
}

// Test 3: Verify that creation of the sRGB profile failure yields FALSE early
static void test_CheckRGBPrimaries_false_path_due_to_create_failure() {
    // Simulate profile creation failure
    g_primariesAvailable = TRUE;
    g_createProfileSucceeds = FALSE; // creation fails
    g_forceFix15_16_at_call = 0;
    g_current_IsGood_call = 0;
    g_last_fail_len = 0;
    g_last_fail_msg[0] = '\0';

    SetTestPrimaries();
    int ret = CheckRGBPrimaries();
    TEST_ASSERT(ret == 0, "CheckRGBPrimaries should return FALSE when profile creation fails");
}

// Run all tests and report summary
int main(void) {
    // Initialize primaries for a valid run
    SetTestPrimaries();

    // Execute tests
    test_CheckRGBPrimaries_true_path();
    test_CheckRGBPrimaries_false_path_due_to_primaries();
    test_CheckRGBPrimaries_false_path_due_to_create_failure();

    // Print summary
    std::printf("Test Summary: %d passed, %d failed\n", g_tests_passed, g_tests_failed);

    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}