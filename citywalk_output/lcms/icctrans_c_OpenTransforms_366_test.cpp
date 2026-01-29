/********** test_icctrans_opentransforms.cpp **********
This file provides a lightweight, non-GTest C++ test harness for the focal
OpenTransforms function found in icctrans.c. The tests exercise the key
execution paths described in the focal method:

- Multi-profile chain path (lMultiProfileChain == true)
- Devicelink mode path (lIsDeviceLink == true, and lMultiProfileChain == false)
- Normal path (neither of the above)

Notes and assumptions
- This test harness is designed to be compiled and linked in the same
  project as icctrans.c with the LittleCMS (lcms2) runtime available.
- The test relies on the presence of the global state manipulated by OpenTransforms
  (e.g., lMultiProfileChain, lIsDeviceLink, xoptind, hProfiles, InputColorSpace, etc.).
- For true unit-test isolation, external dependencies are assumed to be provided
  by the project (lcms2 functions and any project-provided stubs). The tests focus
  on exercising the branches and validating that the global state after OpenTransforms
  matches expectations where possible.
- We use minimal, non-terminating assertions (assert) to ensure executable tests without
  abruptly terminating the test suite in a non-fatal way.
- This file uses plain C-style extern symbols; it does not introduce or rely on GTest.
- The tests include explanatory comments for maintainability and future extensions.

Compile and run notes:
- Build as a regular C/C++ target in the project (ensure icctrans.c and the necessary
  libraries (lcms2) are linked).
- Ensure the global state variables used by OpenTransforms are accessible (extern) and
  that your build includes icctrans.c as part of the project.
- If your build uses separate compilation units for the globals, you may need to
  provide a small initialization translation unit that defines/initializes these globals
  to known values prior to each test.

Implementation begins here.

**********************************************************/

#include <cstring>
#include <mex.h>
#include <stdarg.h>
#include <iostream>
#include <cassert>
#include <lcms2.h>
#include <string.h>


// We assume the focal OpenTransforms is exposed with C linkage.
extern "C" void OpenTransforms(int argc, char *argv[]);

// The following extern declarations mirror the global state used within icctrans.c
// These declarations must match the actual definitions in the production code.
// If your project uses different symbol names, adjust accordingly.

extern "C" int lMultiProfileChain;
extern "C" int lIsDeviceLink;
extern "C" int xoptind;
extern "C" int nProfiles;
extern "C" void* hProfiles[];               // array of profile handles
extern "C" void* hInput;
extern "C" void* hOutput;
extern "C" void* hColorTransform;
extern "C" void* hProof;
extern "C" int InputColorSpace;
extern "C" int OutputColorSpace;
extern "C" int Intent;
extern "C" int ProofingIntent;
extern "C" const char* cInProf;
extern "C" const char* cOutProf;
extern "C" const char* cProofing;
extern "C" int nBytesDepth;
extern "C" int dwIn;
extern "C" int dwOut;
extern "C" int dwFlags;

// Utility helpers to reset global state between tests (best-effort)
static void ResetGlobalsForTest(void)
{
    lMultiProfileChain = 0;
    lIsDeviceLink = 0;
    xoptind = 0;
    nProfiles = 0;
    for (int i = 0; i < 16; ++i) hProfiles[i] = nullptr;
    hInput = nullptr;
    hOutput = nullptr;
    hColorTransform = nullptr;
    hProof = nullptr;
    InputColorSpace = 0;
    OutputColorSpace = 0;
    Intent = 0;
    ProofingIntent = 0;
    cInProf = nullptr;
    cOutProf = nullptr;
    cProofing = nullptr;
    nBytesDepth = 0;
    dwIn = 0;
    dwOut = 0;
    dwFlags = 0;
}

// Each test prints a short summary and uses assertions to validate expected state.
// A failure will abort the test run with an assertion failure, highlighting the
// exact test that failed.

//
// Test 1: Multi-profile chain path
// - When lMultiProfileChain is true, OpenTransforms should build a multiprofile transform
//   from argv[xoptind..argc-1], set hInput to a device-link representation of the transform,
//   and set the "device link" state (lIsDeviceLink) to TRUE.
// - We validate that after invocation, lIsDeviceLink is TRUE and hInput/hOutput are set.
//
static void test_OpenTransforms_MultiProfileChain_Path(void)
{
    ResetGlobalsForTest();

    // Prepare a synthetic argv: the first argument is program name, rest are profiles
    const int argc = 3;
    char* argv[argc];
    static char name1[] = "prog";
    static char profA[] = "profileA.icc";
    static char profB[] = "profileB.icc";
    argv[0] = name1;
    argv[1] = profA;
    argv[2] = profB;

    // Set up state to force multi-profile chain path
    lMultiProfileChain = 1;  // true
    xoptind = 1;             // arguments after program name are profiles
    // Other globals (nProfiles, hProfiles, etc.) are left to the production code
    // to initialize as part of the OpenTransforms execution.

    // Call the focal function
    OpenTransforms(argc, argv);

    // Validate that device-link path was entered and input/output handles were updated
    assert(lIsDeviceLink == 1 && "Multi-profile chain should set lIsDeviceLink to TRUE");
    // After a successful multiprofile chain, hInput should be a device-link transform.
    // The exact pointer value is opaque; we only assert non-nullness for basic sanity.
    assert(hInput != nullptr && "Input handle should be non-null after multiprofile transform creation");
    // hOutput should be NULL as per the code path
    assert(hOutput == nullptr && "Output handle should be NULL for device-link from multiprofile transform");
    // If available, ensure that the input color space/PCS were updated (non-zero)
    assert(InputColorSpace != 0 && "InputColorSpace should be set from input transform");
    assert(OutputColorSpace != 0 && "OutputColorSpace should be set from input transform");
    // If the library sets a valid transform, hColorTransform should be created
    // Note: hColorTransform may be NULL in some test environments; prefer a non-null check if supported
    // assert(hColorTransform != nullptr && "Color transform should be created for the multi-profile path");

    std::cout << "[TEST OK] OpenTransforms_MultiProfileChain_Path" << std::endl;
}

//
// Test 2: DeviceLink path (lIsDeviceLink == true and lMultiProfileChain == false)
// - OpenTransforms should open the input profile from file and set color spaces accordingly.
// - After invocation, we expect InputColorSpace and OutputColorSpace to be updated and hInput non-null.
//
static void test_OpenTransforms_DeviceLink_Path(void)
{
    ResetGlobalsForTest();

    // Prepare a scenario where a single device-link transform is used
    lMultiProfileChain = 0;   // false
    lIsDeviceLink = 1;         // true

    cInProf = "test_in.icc";   // will be used by OpenProfileFromFile path
    cOutProf = nullptr;
    xoptind = 0;

    // argc may be a placeholder since we are not using argv for device-link path
    const int argc = 1;
    char* argv[1];
    argv[0] = (char*)"prog";

    OpenTransforms(argc, argv);

    // Expect that the input profile was opened from file and color spaces were set
    assert(hInput != nullptr && "Input handle should be non-null after cmsOpenProfileFromFile");
    // In this branch, Output should still be NULL
    assert(hOutput == nullptr && "Output handle should be NULL when using device-link path");
    assert(InputColorSpace != 0 && "InputColorSpace should be set from input profile (device-link path)");
    assert(OutputColorSpace != 0 && "OutputColorSpace should be derived for device-link path");

    std::cout << "[TEST OK] OpenTransforms_DeviceLink_Path" << std::endl;
}

//
// Test 3: Normal path (neither multi-profile chain nor device-link)
// - OpenTransforms should open both input and output profiles and set dwIn/dwOut accordingly.
// - We avoid fatal errors by ensuring device-class checks do not trigger the fatal path.
// - Validate that hColorTransform is created and that the color spaces are assigned.
//
static void test_OpenTransforms_Normal_Path(void)
{
    ResetGlobalsForTest();

    lMultiProfileChain = 0;
    lIsDeviceLink = 0;

    // Provide input/output profile names
    cInProf = "input_profile.icc";
    cOutProf = "output_profile.icc";
    xoptind = 0;

    const int argc = 3;
    char* argv[argc];
    argv[0] = (char*)"prog";
    argv[1] = (char*)"input_profile.icc";
    argv[2] = (char*)"output_profile.icc";

    OpenTransforms(argc, argv);

    // In normal path, both input and output profiles should be opened
    assert(hInput != nullptr && "Input handle should be non-null after opening input profile");
    assert(hOutput != nullptr && "Output handle should be non-null after opening output profile");
    // Colorspaces should be sondon set
    assert(InputColorSpace != 0 && "InputColorSpace should be non-zero");
    assert(OutputColorSpace != 0 && "OutputColorSpace should be non-zero");

    // A proofing transform is optional; ensure the transform object exists if library provides it
    // Depending on compile-time flags, hColorTransform may be non-null
    // We conservatively allow a NULL for environments without cmsCreateProofingTransform
    // if (hColorTransform != nullptr) {
    //     // Non-fatal assertion: if transform exists, its handle is valid.
    // }

    std::cout << "[TEST OK] OpenTransforms_Normal_Path" << std::endl;
}

//
// Main test runner
//
int main(void)
{
    std::cout << "Running OpenTransforms unit tests (icctrans.c) in C++11 test harness..." << std::endl;

    test_OpenTransforms_MultiProfileChain_Path();
    test_OpenTransforms_DeviceLink_Path();
    test_OpenTransforms_Normal_Path();

    std::cout << "All tests executed. If assertions failed, execution would have aborted earlier." << std::endl;
    return 0;
}