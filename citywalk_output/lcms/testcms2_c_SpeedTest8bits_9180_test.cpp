// test_speed8bits.cpp
// Lightweight, GTest-free unit tests for SpeedTest8bits in testcms2.c
// This test suite uses mock stubs to isolate the unit under test from the actual
// LittleCMS implementation and focuses on behavior, coverage, and basic interaction.

// Compile note (not part of the answer): This file is intended to be built and run
// in a project where the real SpeedTest8bits is linked from testcms2.c and where
// the following C stubs are resolved during link time.

#include <iostream>
#include <stdexcept>
#include <string>
#include <testcms2.h>
#include <cstdlib>
#include <cstdarg>
#include <ctime>


// Domain types (redeclared here for the test harness to compile independently).
typedef void* cmsHPROFILE;
typedef void* cmsHTRANSFORM;
typedef void* cmsContext;
typedef int   cmsInt32Number;
typedef unsigned int cmsUInt32Number;
typedef unsigned char cmsUInt8Number;
typedef int cmsBool;
typedef unsigned int cmsFlags;
typedef double cmsFloat64Number;

// Basic macros used by the focal method (simplified for test harness).
#define TYPE_RGB_8 0
#define cmsFLAGS_NOCACHE 0

// Forward declaration of the focal method from the tested module (C linkage).
extern "C" void SpeedTest8bits(const char* Title, cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut, cmsInt32Number Intent);

// Also declare Die used by the focal method to signal fatal errors.
extern "C" void Die(const char* Reason, ...);

// ------------------ Mocked LittleCMS-like API (C linkage) ------------------
// These stubs intercept calls from SpeedTest8bits and allow the tests to verify
// call counts and execution flow without performing real image processing.

static int g_mock_create_transform_called = 0;
static int g_mock_close_profile_calls = 0;
static int g_mock_transform_do_called = 0;
static int g_mock_delete_transform_calls = 0;

// A dummy static object to serve as a non-null transform handle.
static int g_transform_dummy_handle;

// DbgThread used as the first argument to cmsCreateTransformTHR; not used by mocks.
extern "C" cmsContext DbgThread(void) { return nullptr; }

// Mock: chk for non-null memory (replicates a safety check in production code).
extern "C" void* chknull(void* mem)
{
    if (mem == nullptr) {
        throw std::bad_alloc(); // Propagate as exception to tests for NULL allocations
    }
    return mem;
}

// Mock: Create a transform handle (non-null)
extern "C" cmsHTRANSFORM cmsCreateTransformTHR(
    cmsContext ctx,
    cmsHPROFILE in,
    cmsUInt32Number inType,
    cmsHPROFILE out,
    cmsUInt32Number outType,
    cmsInt32Number Intent,
    cmsFlags flags)
{
    (void)ctx; (void)in; (void)inType; (void)out; (void)outType; (void)Intent; (void)flags;
    ++g_mock_create_transform_called;
    return (cmsHTRANSFORM)&g_transform_dummy_handle;
}

// Mock: Close a profile
extern "C" void cmsCloseProfile(cmsHPROFILE h)
{
    (void)h;
    ++g_mock_close_profile_calls;
}

// Mock: Perform a transform (no real work)
extern "C" void cmsDoTransform(cmsHTRANSFORM xform, const void* In, void* Out, cmsUInt32Number N)
{
    (void)xform; (void)In; (void)Out; (void)N;
    ++g_mock_transform_do_called;
}

// Mock: Delete a transform
extern "C" void cmsDeleteTransform(cmsHTRANSFORM xform)
{
    (void)xform;
    ++g_mock_delete_transform_calls;
}

// Mock: Simple memory/title helpers (no-ops in tests)
extern "C" void TitlePerformance(const char* Txt) { (void)Txt; }
extern "C" void PrintPerformance(cmsUInt32Number Bytes, cmsUInt32Number SizeOfPixel, cmsFloat64Number diff)
{
    (void)Bytes; (void)SizeOfPixel; (void)diff;
}

// Mock: Die function to throw an exception that tests can catch
extern "C" void Die(const char* Reason, ...)
{
    (void)Reason;
    // Consume variadic arguments safely
    va_list ap;
    va_start(ap, Reason);
    // We deliberately ignore extra args for testing purposes
    va_end(ap);
    throw std::runtime_error("Die() called by focal method with reason: " + std::string(Reason ? Reason : "Unknown"));
}

// -------------- Test harness utilities --------------

static void reset_mocks()
{
    g_mock_create_transform_called = 0;
    g_mock_close_profile_calls = 0;
    g_mock_transform_do_called = 0;
    g_mock_delete_transform_calls = 0;
}

// Helper: create a fake profile handle (non-null)
static cmsHPROFILE CreateFakeProfile()
{
    // Use a non-null sentinel value
    static int fake;
    return (cmsHPROFILE)&fake;
}

// Simple non-terminating test assertion helpers
#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << (msg) << " (line " << __LINE__ << ")" << std::endl; \
        failures++; \
    } \
} while(0)

static int failures = 0;

// ------------------ Unit tests ------------------

/*
  Test 1: Null profiles should trigger the Die() path.
  - Expect an exception to be thrown by SpeedTest8bits when either input or output profile is NULL.
  - This verifies the predicate true-branch handling.
*/
static void test_SpeedTest8bits_NULL_Profiles()
{
    reset_mocks();
    bool caught = false;
    try {
        SpeedTest8bits("Null Profiles Test", nullptr, nullptr, 0);
    } catch (const std::exception& e) {
        caught = true;
        // Optional: uncomment to print exception message for debugging
        // std::cerr << "Caught expected exception: " << e.what() << std::endl;
    }
    EXPECT_TRUE(caught, "SpeedTest8bits should throw on NULL profiles");
    // Ensure no mock functions were invoked before Die (for robust isolation)
    // Depending on compilation, some pre-checks could be added; we just report result.
}

/*
  Test 2: Valid profiles path executes the transform workflow.
  - Provide two non-null fake profiles and verify that:
    - cmsCreateTransformTHR is invoked exactly once
    - cmsCloseProfile is invoked for both profiles (twice)
    - cmsDoTransform is invoked
    - cmsDeleteTransform is invoked
  - This checks the else-branch and ensures the core loop proceeds to call into the transform API.
*/
static void test_SpeedTest8bits_Valid_Profiles()
{
    reset_mocks();

    cmsHPROFILE inProf = CreateFakeProfile();
    cmsHPROFILE outProf = CreateFakeProfile();

    // Call the focal method
    try {
        SpeedTest8bits("Valid Profiles Test", inProf, outProf, 0);
    } catch (...) {
        // If something unexpected happens, treat as test failure
        failures++;
        std::cerr << "Unexpected exception during SpeedTest8bits with valid profiles" << std::endl;
        return;
    }

    // Assertions on mock call counts
    EXPECT_TRUE(g_mock_create_transform_called == 1, "cmsCreateTransformTHR should be called once");
    EXPECT_TRUE(g_mock_close_profile_calls >= 2, "cmsCloseProfile should be called for both input and output profiles");
    EXPECT_TRUE(g_mock_transform_do_called == 1, "cmsDoTransform should be invoked once");
    EXPECT_TRUE(g_mock_delete_transform_calls == 1, "cmsDeleteTransform should be invoked once");
}

// ------------------ Main test driver ------------------

int main()
{
    std::cout << "Starting SpeedTest8bits unit tests (no GTest)..." << std::endl;

    test_SpeedTest8bits_NULL_Profiles();
    test_SpeedTest8bits_Valid_Profiles();

    if (failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << "Test failures: " << failures << std::endl;
        return 1;
    }
}