/*
   Unit test suite for cmsBool WriteOutputLUT (cmsps2.c)

   - Purpose: exercise the focal method WriteOutputLUT using the real LittleCMS interfaces.
   - Approach: implement a lightweight, non-terminating test harness in C++11 that
     uses the actual LittleCMS API to construct a minimal environment and invoke
     WriteOutputLUT with several flag variants to exercise related branches.
   - Notes:
     - No GTest/GMock is used. A small custom assertion/diagnostic framework is implemented.
     - The tests rely on the public LittleCMS API (lcms2.h) and do not rely on private/static test hooks.
     - Each test prints a brief explanation and records failures, but continues execution to maximize coverage.
     - Management of resources (profiles, IO handlers, context) is performed to avoid leaks during tests.
*/

#include <vector>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <lcms2.h>


// Include LittleCMS header (C API accessible from C++)

// Lightweight, non-terminating assertion helpers
static int g_failure_count = 0;
#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        ++g_failure_count; \
        std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " Expected true: " #cond << std::endl; \
    } \
} while (0)

#define EXPECT_FALSE(cond) do { \
    if ((cond)) { \
        ++g_failure_count; \
        std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " Expected false: " #cond << std::endl; \
    } \
} while (0)

#define EXPECT_EQ(a, b) do { \
    if (!((a) == (b))) { \
        ++g_failure_count; \
        std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " Expected " #a " == " #b << " (actual: " << (a) << " vs " << (b) << ")" << std::endl; \
    } \
} while (0)

#define EXPECT_NEQ(a, b) do { \
    if ((a) == (b)) { \
        ++g_failure_count; \
        std::cerr << "[FAILED] " << __FILE__ << ":" << __LINE__ \
                  << " Expected " #a " != " #b << std::endl; \
    } \
} while (0)

using namespace std;

/*
   Test 1: WriteOutputLUT with default flags (dwFlags = 0)
   - Expectation: function returns TRUE and prints a RenderTable with basic structure.
*/
bool Test_WriteOutputLUT_DefaultFlags()
{
    // Create a context for printing/logical separation
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (ctx == nullptr) {
        std::cerr << "[TEST] Failed to create CMS context" << std::endl;
        g_failure_count++;
        return false;
    }

    // Create an in-memory IO handler to capture generated PostScript/CRD content
    cmsIOHANDLER* memIo = cmsOpenIOhandlerFromNULL(ctx);
    if (memIo == nullptr) {
        std::cerr << "[TEST] Failed to create in-memory IO handler" << std::endl;
        g_failure_count++;
        cmsDeleteContext(ctx);
        return false;
    }

    // Use a standard sRGB profile as the destination profile
    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == nullptr) {
        std::cerr << "[TEST] Failed to create sRGB profile" << std::endl;
        g_failure_count++;
        cmsCloseIOhandler(memIo);
        cmsDeleteContext(ctx);
        return false;
    }

    // Call the focal function with default flags
    cmsBool result = WriteOutputLUT(memIo, hProfile, INTENT_RELATIVE_COLORIMETRIC, 0);

    // Assertions
    EXPECT_TRUE(result == TRUE);

    // Cleanup resources
    cmsCloseIOhandler(memIo);
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);

    return (g_failure_count == 0);
}

/*
   Test 2: WriteOutputLUT with Black Point Compensation flag enabled
   - Purpose: exercise the lDoBPC path (cmsFLAGS_BLACKPOINTCOMPENSATION)
   - Expectation: function returns TRUE and writes output without fatal error.
*/
bool Test_WriteOutputLUT_WithBlackPointCompensation()
{
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (ctx == nullptr) {
        std::cerr << "[TEST] Failed to create CMS context (BPComp)" << std::endl;
        g_failure_count++;
        return false;
    }

    cmsIOHANDLER* memIo = cmsOpenIOhandlerFromNULL(ctx);
    if (memIo == nullptr) {
        std::cerr << "[TEST] Failed to create in-memory IO handler (BPComp)" << std::endl;
        g_failure_count++;
        cmsDeleteContext(ctx);
        return false;
    }

    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == nullptr) {
        std::cerr << "[TEST] Failed to create sRGB profile (BPComp)" << std::endl;
        g_failure_count++;
        cmsCloseIOhandler(memIo);
        cmsDeleteContext(ctx);
        return false;
    }

    // Enable Black Point Compensation
    cmsUInt32Number flags = cmsFLAGS_BLACKPOINTCOMPENSATION;

    cmsBool result = WriteOutputLUT(memIo, hProfile, INTENT_RELATIVE_COLORIMETRIC, flags);

    EXPECT_TRUE(result == TRUE);

    cmsCloseIOhandler(memIo);
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);

    return (g_failure_count == 0);
}

/*
   Test 3: WriteOutputLUT with NowWhiteOnWhiteFixup flag enabled
   - Purpose: exercise the lFixWhite path (dwFlags controls NowWhiteFixup)
   - Expectation: function returns TRUE and outputs content (no fatal error)
*/
bool Test_WriteOutputLUT_WithNowWhiteOnWhiteFixup()
{
    cmsContext ctx = cmsCreateContext(nullptr, nullptr);
    if (ctx == nullptr) {
        std::cerr << "[TEST] Failed to create CMS context (NoWhiteFixup)" << std::endl;
        g_failure_count++;
        return false;
    }

    cmsIOHANDLER* memIo = cmsOpenIOhandlerFromNULL(ctx);
    if (memIo == nullptr) {
        std::cerr << "[TEST] Failed to create in-memory IO handler (NoWhiteFixup)" << std::endl;
        g_failure_count++;
        cmsDeleteContext(ctx);
        return false;
    }

    cmsHPROFILE hProfile = cmsCreate_sRGBProfile();
    if (hProfile == nullptr) {
        std::cerr << "[TEST] Failed to create sRGB profile (NoWhiteFixup)" << std::endl;
        g_failure_count++;
        cmsCloseIOhandler(memIo);
        cmsDeleteContext(ctx);
        return false;
    }

    // Enable NOWHITEONWHITEFIXUP flag
    cmsUInt32Number flags = cmsFLAGS_NOWHITEONWHITEFIXUP;

    cmsBool result = WriteOutputLUT(memIo, hProfile, INTENT_RELATIVE_COLORIMETRIC, flags);

    EXPECT_TRUE(result == TRUE);

    cmsCloseIOhandler(memIo);
    cmsCloseProfile(hProfile);
    cmsDeleteContext(ctx);

    return (g_failure_count == 0);
}

/*
   Main test runner
*/
int main()
{
    cout << "Running WriteOutputLUT test suite (cmsps2.c) - C++11 harness" << endl;

    // Execute tests
    bool ok1 = Test_WriteOutputLUT_DefaultFlags();
    bool ok2 = Test_WriteOutputLUT_WithBlackPointCompensation();
    bool ok3 = Test_WriteOutputLUT_WithNowWhiteOnWhiteFixup();

    if (ok1 && ok2 && ok3 && g_failure_count == 0) {
        cout << "[TESTS PASSED] All WriteOutputLUT tests succeeded." << endl;
        return 0;
    } else {
        cout << "[TESTS FAILED] Failures: " << g_failure_count
             << " out of 3 tests reported." << endl;
        // Non-zero exit code to indicate test failures
        return 1;
    }
}