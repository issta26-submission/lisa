// Test suite for PackEncodedBytesLabV2FromFloat in cmspack.c
// This test uses the actual Little CMS types and macros.
// It does not rely on any testing framework (GTest) and uses a lightweight
// in-code assertion mechanism to allow non-terminating checks and further
// test execution on failure.
//
// Build recommendation (assuming typical build environment):
// g++ -std=c++11 -c test_pack_lab_v2.cpp
// g++ -std=c++11 test_pack_lab_v2.o cmspack.o -llcms2 -o test_pack_lab_v2
//
// Notes:
// - The tests rely on the real macros T_PLANAR, PixelSize, T_EXTRA, etc., from
//   the included lcms headers. The exact numeric values for formats are not
//   hard-coded; instead, the tests compute expected behavior using the same
//   macros and rely on their runtime evaluation to drive the assertions.

#include <cstring>
#include <cstdint>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cmath>



// Lightweight non-terminating assertion helper
static bool g_tests_passed = true;
#define TEST_CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test failure: " << (msg) << "\n"; \
        g_tests_passed = false; \
    } \
} while(0)

// Helper to compute the Lab to encoded 16-bit values for given wOut
static void ComputeWLabFromOut(const cmsFloat32Number wOut[3], cmsUInt16Number wlab[3])
{
    cmsCIELab Lab;
    Lab.L = static_cast<cmsFloat64Number>(wOut[0] * 100.0);
    Lab.a = static_cast<cmsFloat64Number>(wOut[1] * 255.0 - 128.0);
    Lab.b = static_cast<cmsFloat64Number>(wOut[2] * 255.0 - 128.0);
    cmsFloat2LabEncoded(wlab, &Lab);
}

// Test wrapper to exercise PackEncodedBytesLabV2FromFloat for a given OutputFormat.
// It computes expected results using the same internal encoding logic to avoid
// dependency on internal wlab values, and uses the same macros as the focal code.
static void RunTestForFormat(cmsUInt32Number testFormat, const char* testName)
{
    // Prepare Info with a specific OutputFormat
    _cmsTRANSFORM Info;
    // Initialize to zero to avoid undefined fields; then set required member
    std::memset(&Info, 0, sizeof(Info));
    Info.OutputFormat = testFormat;

    // Prepare input and output buffers
    cmsFloat32Number wOut[3] = { 0.0f, 0.0f, 0.0f }; // L=0 => Lab.L=0, a/b near -128
    cmsUInt8Number output[256];
    std::memset(output, 0, sizeof(output));

    // Choose an arbitrary Stride; the function will adjust it by PixelSize
    cmsUInt32Number StrideInput = 4;

    // Compute reference wlab using library's encoder
    cmsUInt16Number wlabRef[3];
    ComputeWLabFromOut(wOut, wlabRef);
    cmsUInt16Number wlabEnc0 = wlabRef[0];
    cmsUInt16Number wlabEnc1 = wlabRef[1];
    cmsUInt16Number wlabEnc2 = wlabRef[2];
    cmsUInt8Number expected0 = static_cast<cmsUInt8Number>(wlabEnc0 >> 8);
    cmsUInt8Number expected1 = static_cast<cmsUInt8Number>(wlabEnc1 >> 8);
    cmsUInt8Number expected2 = static_cast<cmsUInt8Number>(wlabEnc2 >> 8);

    // Call the focal function
    cmsUInt8Number* ret = PackEncodedBytesLabV2FromFloat(&Info, wOut, output, StrideInput);

    // Determine finalStride used inside the function after division
    cmsUInt32Number finalStride = 0;
    cmsUInt32Number pxSize = PixelSize(Info.OutputFormat);
    if (pxSize != 0) finalStride = StrideInput / pxSize;

    // Determine which branch was taken by the focal function
    cmsBool isPlanar = T_PLANAR(Info.OutputFormat);

    if (isPlanar) {
        // Planar: output[0], output[Stride], output[Stride*2] are written
        TEST_CHECK(output[0] == expected0, testName);
        TEST_CHECK(output[finalStride] == expected1, testName);
        TEST_CHECK(output[finalStride * 2] == expected2, testName);

        // Returned pointer should be output + 1
        TEST_CHECK(static_cast<void*>(ret) == static_cast<void*>(output + 1), testName);
    } else {
        // Chunky: output[0], output[1], output[2] are written
        TEST_CHECK(output[0] == expected0, testName);
        TEST_CHECK(output[1] == expected1, testName);
        TEST_CHECK(output[2] == expected2, testName);

        // Returned pointer should be output + (3 + T_EXTRA(OutputFormat))
        cmsUInt32Number expectedReturnOffset = 3 + T_EXTRA(Info.OutputFormat);
        TEST_CHECK(static_cast<void*>(ret) == static_cast<void*>(output + expectedReturnOffset), testName);
    }

    // Optional: Report per-test completion
    std::cout << "Test '" << testName << "' completed. Branch planar=" << (isPlanar ? "true" : "false") 
              << "; returnOffset=" << (ret - output) << "\n";
}

int main()
{
    // Run test for a given format; the exact value is library-defined.
    // We rely on the library's macros to determine whether this is planar or chunky.
    // The test will adapt expectations accordingly.

    // Example test: use an arbitrary non-zero format value
    RunTestForFormat(0x01020304, "PackEncodedBytesLabV2FromFloat - testFormat 0x01020304");

    // Another test with a different format value to broaden coverage
    RunTestForFormat(0x05060708, "PackEncodedBytesLabV2FromFloat - testFormat 0x05060708");

    if (g_tests_passed) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << "SOME TESTS FAILED\n";
        return 1;
    }
}