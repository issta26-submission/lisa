// Test suite for CheckGBD() focal method
// This test suite is written for C++11, does not rely on GTest, and uses
// a lightweight custom assertion mechanism (EXPECT_*) to keep tests non-terminating.
// It also demonstrates how to exercise different branches by overriding key
// library functions via link-time interposition (wrappers in this test file).

#include <iostream>
#include <cstdio>
#include <testcms2.h>
#include <cstdlib>


// Import necessary declarations for the focal method and types.
// The project under test provides testcms2.h (as per the repository's structure).
// Including it here allows us to reference cms* types and CheckGBD().
// If the real environment places testcms2.h elsewhere, adjust the include path accordingly.

extern "C" {
    // Forward declarations for the focal function under test.
    // We assume its prototype matches the one in the repository.
    int CheckGBD(void);
}

// Lightweight test harness state to control interposition behavior.
// testMode semantics:
// 0 - normal operation (default): all interposed functions report success
// 1 - cmsGBDAlloc returns NULL (simulate allocation failure)
// 3 - cmsGDBAddPoint returns failure (simulate failure during RAW gamut fill)
static int testMode = 0;

// Simple non-terminating assertions with centralized reporting
static int gFailures = 0;
#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        ++gFailures; \
        std::cerr << "[FAIL] " #cond " at " __FILE__ ":" << __LINE__ << "\n"; \
    } \
} while(false)


// Interposed (fake) library functions to steer the behavior of CheckGBD().
// We provide C linkage so they shadow the real library implementations during linking.

extern "C" {

// Note: The exact typedefs (cmsHANDLE, cmsContext, cmsCIELab, etc.) come from testcms2.h.
// We rely on those to keep signatures correct for the focal method.
cmsHANDLE cmsGBDAlloc(cmsContext ContextID)
{
    // Simulate allocation failure when requested
    if (testMode == 1) return NULL;
    static int dummy;
    // Return a non-null pseudo-handle
    return (cmsHANDLE)(&dummy);
}

cmsBool cmsGDBAddPoint(cmsHANDLE h, const cmsCIELab* Lab)
{
    // Simulate failure during RAW gamut filling when requested
    if (testMode == 3) return 0; // false
    return 1; // success
}

cmsBool cmsGDBCompute(cmsHANDLE h, cmsInt32Number Flags)
{
    // Normal operation: pretend computation succeeds
    return 1;
}

cmsBool cmsGDBCheckPoint(cmsHANDLE h, const cmsCIELab* Lab)
{
    // Normal operation: always succeed in this mock
    return 1;
}

void cmsGBDFree(cmsHANDLE h)
{
    // No-op in the test interposition
}

cmsHPROFILE cmsCreate_sRGBProfile(void)
{
    // Return a non-null dummy profile handle
    static int dummy;
    return (cmsHPROFILE)(&dummy);
}

cmsHPROFILE cmsCreateLab4Profile(void* /* userdata */)
{
    static int dummy;
    return (cmsHPROFILE)(&dummy);
}

cmsHTRANSFORM cmsCreateTransform(cmsHPROFILE in, cmsUInt32Number inType,
                                 cmsHPROFILE out, cmsUInt32Number outType,
                                 cmsInt32Number Intent, cmsUInt32Number Flags)
{
    static int dummy;
    return (cmsHTRANSFORM)(&dummy);
}

void cmsCloseProfile(cmsHPROFILE h)
{
    // no-op
}

void cmsDeleteTransform(cmsHTRANSFORM xform)
{
    // no-op
}

void cmsDoTransform(cmsHTRANSFORM xform,
                    const cmsUInt8Number In[], cmsCIELab* Lab, cmsUInt32Number N)
{
    // Minimal transform: fill Lab with a stable, simple value to keep
    // subsequent gamut checks meaningful in the real library path.
    if (Lab != nullptr && N > 0) {
        Lab[0].L = 50;
        Lab[0].a = 0;
        Lab[0].b = 0;
    }
}

void cmsLCh2Lab(cmsCIELab* Lab, const cmsCIELCh* LCh)
{
    // Simple mapping for test interposition: LCh -> Lab with a=b=0
    if (Lab && LCh) {
        Lab->L = LCh->L;
        Lab->a = 0;
        Lab->b = 0;
    }
}

} // extern "C"

// Helper function to run a named test and report results.
static void RunTest(const char* name, void (*testFn)()) {
    std::cout << "Running test: " << name << std::endl;
    testFn();
    if (gFailures == 0) {
        std::cout << "  [PASS] " << name << "\n";
    } else {
        std::cout << "  [FAIL] " << name << " (failures: " << gFailures << ")\n";
    }
}

// Test 1: Normal flow should return 1 (success path)
void Test_CheckGBD_NormalPath(void) {
    gFailures = 0;
    testMode = 0; // enable normal behavior for interposed functions
    int res = CheckGBD();
    EXPECT_TRUE(res == 1);
}

// Test 2: Allocation failure should cause CheckGBD to return 0
void Test_CheckGBD_AllocFailure(void) {
    gFailures = 0;
    testMode = 1; // cmsGBDAlloc returns NULL
    int res = CheckGBD();
    EXPECT_TRUE(res == 0);
}

// Test 3: Failure during RAW gamut point addition should cause CheckGBD to return 0
void Test_CheckGBD_AddPointFailure(void) {
    gFailures = 0;
    testMode = 3; // cmsGDBAddPoint returns 0 on first call
    int res = CheckGBD();
    // Expect early return due to failure in AddPoint
    EXPECT_TRUE(res == 0);
}

int main(int argc, char* argv[]) {
    (void)argc; (void)argv;

    // Run a small battery of tests to exercise various control paths of CheckGBD.
    RunTest("CheckGBD_NormalPath", Test_CheckGBD_NormalPath);
    RunTest("CheckGBD_AllocFailure", Test_CheckGBD_AllocFailure);
    RunTest("CheckGBD_AddPointFailure", Test_CheckGBD_AddPointFailure);

    // Summary
    int totalTests = 3;
    if (gFailures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << gFailures << " test(s) failed out of " << totalTests << ".\n";
        return 1;
    }
}