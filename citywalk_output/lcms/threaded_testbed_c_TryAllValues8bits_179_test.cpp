/*
Step 1 (Program Understanding) Summary (embedded as comments):
- Focal method: TryAllValues8bits
  - Creates two cmsContext objects (Raw, Plugin) and two transforms (xformRaw, xformPlugin) from hlcmsProfileIn/Out with TYPE_RGBA_8 and a given Intent.
  - Closes input/output profiles.
  - Verifies both transforms are non-NULL; on NULL it calls Fail and would proceed to subsequent operations (which would be unsafe if NULL).
  - Allocates three large buffers for 256*256*256 pixels (~16.7 million pixels) of RGBA8 data.
  - Fills input buffer with every possible 24-bit RGB combination with alpha 0xff.
  - Runs the two transforms (xformRaw and xformPlugin) on the input to separate output buffers.
  - Compares outputs byte-by-byte; on mismatch, calls Fail with detailed message.
  - Frees buffers, deletes transforms and contexts.
- Key dependent components to cover in tests:
  - cmsCreateContext, cmsCreateTransformTHR, cmsCreateTransformTHR (non-NULL transforms), cmsDoTransform, buffer allocation, per-pixel comparison, Fail path.
  - Resource cleanup (free/memory, cmsDeleteTransform, cmsDeleteContext).
- Domain scope notes:
  - Tests should be executable in C++11 without GTest.
  - No private members access; static functions at file scope are internal to the translation unit (we focus on TryAllValues8bits).
  - Use a minimal, non-terminating test harness (no termination on assertion failures) and rely on the function’s internal Fail path to indicate mismatch.
  - We will attempt to isolate the test to a normal successful path (identity-ish ICC profiles) to avoid extremely long runtimes in unit tests.

Step 2 (Unit Test Generation) Plan implemented in C++ test code:
- Provide a lightweight test harness (no GTest) to exercise TryAllValues8bits with valid ICC profiles that support RGBA_8 transforms (e.g., sRGB profiles).
- Use a signal-based guard to catch unexpected crashes (segmentation faults) originating from internal failures in TryAllValues8bits (e.g., NULL transforms, memory issues). This helps distinguish a hard crash from a testable success path.
- Two tests are described/implemented:
  - Test 1: Normal execution path using two valid ICC profiles (src = dst = sRGB). Expect TryAllValues8bits to run to completion without crashing.
  - Test 2: Sanity check for crash avoidance by running again with the same profiles but a repeated call to ensure non-crashing behavior across runs (to increase coverage of initialization/cleanup).

Note: The extremely large inner loop (16.7 million pixels) is part of the focal method; running this in unit tests will be costly. The test harness uses identity-like ICC profiles so that the transforms are well-defined and the test focuses on exercising the code path up to completion. If the test environment imposes time constraints, this test may be adjusted to use profiling or a smaller dataset path in a later refactor (but is kept faithful to the focal code path here).

Step 3 (Test Case Refinement) domain considerations applied:
- Use only standard C++11 facilities and the provided LittleCMS API (no GTest).
- The test harness is self-contained and prints results to stdout.
- Tests avoid private internals; they drive TryAllValues8bits with public APIs and rely on Fail as the error-reporting mechanism used by the focal code.
- Comments explain what each test asserts and how it guards against crashes.

Now, the test code (C++11) follows. It relies on LittleCMS headers (lcms2.h) and the sRGB ICC profile factory function cmsCreate_sRGBProfile. It uses a minimal signal-based crash guard to detect abnormal termination and prints a test result summary.

Code begins here:

*/

// Test suite for TryAllValues8bits using C++11 (no GTest)

#include <csignal>
#include <threaded_internal.h>
#include <iostream>
#include <time.h>
#include <setjmp.h>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>
#include <lcms2.h>


// Include LittleCMS header (adjust the path if necessary in your build setup)

// Forward declaration to ensure we can reference the focal function.
// The focal function is defined in threaded_testbed.c (provided in the project).
// We assume the function signature matches the one in the prompt.
extern "C" void TryAllValues8bits(cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut, cmsInt32Number Intent);

// Lightweight test harness (no terminating assertions) and crash guard
static volatile sig_atomic_t g_crashDetected = 0;
static jmp_buf g_jmpEnv;

static void segv_handler(int) {
    // Jump back to test harness if a segfault occurs inside TryAllValues8bits
    g_crashDetected = 1;
    longjmp(g_jmpEnv, 1);
}

// Helper to run a single test case guarded against crashes
static bool RunTestGuarded(const char* testName, cmsHPROFILE inProf, cmsHPROFILE outProf, cmsInt32Number intent) {
    g_crashDetected = 0;
    struct sigaction oldSegv, newSegv;
    newSegv.sa_handler = segv_handler;
    sigemptyset(&newSegv.sa_mask);
    newSegv.sa_flags = 0;
    sigaction(SIGSEGV, &newSegv, &oldSegv);

    bool passed = true;
    if (setjmp(g_jmpEnv) == 0) {
        // Execute the focal method with provided profiles and intent
        TryAllValues8bits(inProf, outProf, intent);
        // If we return here, we assume normal completion (no crash)
    } else {
        // We were longjumped from a crash
        passed = false;
    }

    // Restore previous signal handler
    sigaction(SIGSEGV, &oldSegv, nullptr);

    // If a crash was detected, mark as failed
    if (g_crashDetected) {
        passed = false;
    }

    std::cout << "[TEST] " << testName << " -> " << (passed ? "PASSED" : "FAILED (crash or exception)" ) << std::endl;
    return passed;
}

// Helper to create two identity-like ICC profiles for testing
static cmsHPROFILE CreateIdentityProfiles(void) {
    cmsHPROFILE inProf = cmsCreate_sRGBProfile();   // common 3-channel sRGB profile
    cmsHPROFILE outProf = cmsCreate_sRGBProfile();  // destination profile (also sRGB)
    if (inProf == nullptr || outProf == nullptr) {
        std::cerr << "Failed to create test ICC profiles." << std::endl;
        if (inProf) cmsCloseProfile(inProf);
        if (outProf) cmsCloseProfile(outProf);
        return nullptr;
    }
    return reinterpret_cast<cmsHPROFILE>(new std::pair<cmsHPROFILE, cmsHPROFILE>(inProf, outProf));
}

// Overloaded to free two profiles created in CreateIdentityProfiles
static void FreeIdentityProfiles(void* p) {
    auto pairPtr = reinterpret_cast<std::pair<cmsHPROFILE, cmsHPROFILE>*>(p);
    if (pairPtr) {
        if (pairPtr->first) cmsCloseProfile(pairPtr->first);
        if (pairPtr->second) cmsCloseProfile(pairPtr->second);
        delete pairPtr;
    }
}

int main() {
    std::cout << "Starting TryAllValues8bits unit tests (C++11, no GTest)" << std::endl;

    // Test 1: Normal execution path with two valid ICC profiles
    // This exercises the full code path, including context creation, transform creation, memory allocation,
    // per-pixel processing, and comparison. The goal is to ensure no crash and no unexpected failure.
    cmsHPROFILE inOutPair = nullptr;
    // We wrap in a small block to ensure resources are freed properly if test fails early.
    {
        auto pairPtrBox = CreateIdentityProfiles();
        if (pairPtrBox == nullptr) {
            std::cout << "[TEST] NormalPath: Failed to create test ICC profiles -> MARKING AS FAILED" << std::endl;
        } else {
            auto pairPtr = reinterpret_cast<std::pair<cmsHPROFILE, cmsHPROFILE>*>(pairPtrBox);
            inOutPair = pairPtr->first; // For readability, we reuse inProf as input
            cmsHPROFILE outProf = pairPtr->second;
            // Run the test guarded against crashes
            bool ok = RunTestGuarded("NormalExecution_TryAllValues8bits", inOutPair, outProf, CMS_INTENT_PERCEPTUAL);
            // Free resources
            FreeIdentityProfiles(pairPtrBox);
            // The test result is already printed in RunTestGuarded
            if (!ok) {
                std::cout << "[TEST] NormalExecution_TryAllValues8bits: Unexpected crash during test." << std::endl;
            }
        }
    }

    // Test 2: Re-run with the same identity profiles to increase path coverage and detection of crashes
    {
        auto pairPtrBox = CreateIdentityProfiles();
        if (pairPtrBox == nullptr) {
            std::cout << "[TEST] ReRun_TryAllValues8bits: Failed to create test ICC profiles -> MARKING AS FAILED" << std::endl;
        } else {
            auto pairPtr = reinterpret_cast<std::pair<cmsHPROFILE, cmsHPROFILE>*>(pairPtrBox);
            cmsHPROFILE inProf = pairPtr->first;
            cmsHPROFILE outProf = pairPtr->second;
            bool ok = RunTestGuarded("ReRun_TryAllValues8bits", inProf, outProf, CMS_INTENT_RELATIVE_COLORIMETRIC);
            FreeIdentityProfiles(pairPtrBox);
            if (!ok) {
                std::cout << "[TEST] ReRun_TryAllValues8bits: Unexpected crash during test." << std::endl;
            }
        }
    }

    std::cout << "Finished TryAllValues8bits unit tests." << std::endl;
    return 0;
}

/*
Notes:
- The tests aim to exercise TryAllValues8bits with realistic ICC profiles (sRGB) to validate the normal execution path.
- A crash guard via SIGSEGV is used to catch unexpected terminations inside the focal function.
- The tests print explicit PASS/FAIL lines for quick human interpretation.
- Since the focal method performs a heavy 256^3 pixel transformation, these tests may be time-consuming on slower environments. If you need faster feedback during development, consider parameterizing the test to use smaller datasets by temporarily editing the focal method (not recommended for a true unit test, but acceptable for quick iteration in a controlled environment).
- This test suite avoids GTest and follows a simple, self-contained C++11 test approach as requested.
*/