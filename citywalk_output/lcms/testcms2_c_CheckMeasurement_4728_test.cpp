// Minimal C++11 test harness for the focal function CheckMeasurement in testcms2.c
// This test suite uses the existing C helpers in testcms2.h (imported dependencies) and
// validates the behavior of CheckMeasurement without terminating on failures.
//
// Notes:
// - Uses simple non-terminating EXPECT-like assertions to maximize code coverage.
// - Calls the focal function CheckMeasurement with representative Pass values (1, 2, 3).
// - Uses real profile creation helpers (Create_AboveRGB, Create_Gray22) to obtain cmsHPROFILE.
// - The tag used for testing is cmsSigMeasurementConditions (common ICC tag for measurement data).

#include <cstring>
#include <cstdio>
#include <testcms2.h>
#include <cstdlib>
#include <cstdarg>


// Import the provided dependencies and test harness declarations.
// This header is assumed to expose the relevant types (cmsInt32Number, cmsHPROFILE, cmsTagSignature, etc.)
// and the helper functions used below (Create_AboveRGB, Create_Gray22, cmsCloseProfile, etc.).

extern "C" {

// Focal function under test (C linkage to avoid name mangling in C++).
cmsInt32Number CheckMeasurement(cmsInt32Number Pass,  cmsHPROFILE hProfile, cmsTagSignature tag);

} // extern "C"

static int g_failures = 0;

// Lightweight non-terminating assertion macro (does not exit on failure; records the failure).
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        fprintf(stderr, "EXPECT_TRUE failed: %s, at %s:%d\n", #cond, __FILE__, __LINE__); \
        ++g_failures; \
    } \
} while(0)

int main(int argc, char** argv)
{
    // Acquire a test profile to use for Pass=1/Pass=2 tests.
    // Using Create_AboveRGB to obtain a standard RGB profile from the test harness.
    cmsHPROFILE hProfileWrite = Create_AboveRGB();
    if (hProfileWrite == nullptr) {
        fprintf(stderr, "Failed to create test profile via Create_AboveRGB.\n");
        return 1;
    }

    // Acquire a second, clean profile to test Pt==NULL path (no tag present).
    cmsHPROFILE hProfileNoTag = Create_Gray22();
    if (hProfileNoTag == nullptr) {
        fprintf(stderr, "Failed to create test profile via Create_Gray22.\n");
        cmsCloseProfile(hProfileWrite);
        return 1;
    }

    // Tag identifier used by CheckMeasurement. This tag must be the same for both write and read.
    // cmsSigMeasurementConditions is the standard tag for measurement data in ICC specs.
    const cmsTagSignature tag = (cmsTagSignature) cmsSigMeasurementConditions;

    // 1) Test that Pass == 1 writes a valid measurement structure to the profile.
    // This exercises the write path and ensures the function returns a non-zero value on success.
    {
        // "Write" path: CheckMeasurement with Pass = 1 should return non-zero when writing succeeds.
        cmsInt32Number resWrite = CheckMeasurement(1, hProfileWrite, tag);
        // Expect success (non-zero). If this fails, subsequent tests depending on the write will be invalid.
        EXPECT_TRUE(resWrite != 0);
    }

    // 2) Test that Pass == 2 reads back the previously written tag and validates its fields.
    // This exercises the read/validate path and should return 1 when the stored data matches expectations.
    {
        cmsInt32Number resRead = CheckMeasurement(2, hProfileWrite, tag);
        EXPECT_TRUE(resRead == 1);
    }

    // 3) Test that an invalid Pass value falls through to the default case.
    // This ensures we cover the default path returning 0.
    {
        cmsInt32Number resDefault = CheckMeasurement(3, hProfileWrite, tag);
        EXPECT_TRUE(resDefault == 0);
    }

    // 4) Test Pt == NULL path by using a profile that has no such tag written yet.
    // This ensures the function returns 0 when the expected tag is absent.
    {
        cmsInt32Number resNoTag = CheckMeasurement(2, hProfileNoTag, tag);
        EXPECT_TRUE(resNoTag == 0);
    }

    // Cleanup
    cmsCloseProfile(hProfileWrite);
    cmsCloseProfile(hProfileNoTag);

    // Summary
    if (g_failures > 0) {
        fprintf(stderr, "CheckMeasurement test completed with %d failure(s).\n", g_failures);
        return 1;
    } else {
        fprintf(stdout, "CheckMeasurement test completed successfully with all tests passing.\n");
        return 0;
    }
}