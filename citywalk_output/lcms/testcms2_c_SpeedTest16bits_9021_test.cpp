// Lightweight C++11 test harness for SpeedTest16bits in testcms2.c
// This harness provides minimal stubs for external LittleCMS-like functions
// so that SpeedTest16bits can be executed in isolation without a full LMS/CMS runtime.
// It exercises true/false branches and ensures executable test paths without terminating.

#include <cstring>
#include <iostream>
#include <testcms2.h>
#include <cstdarg>
#include <ctime>


// Domain primitives (mocked to isolate the focal method)
typedef void* cmsHPROFILE;
typedef void* cmsContext;
typedef void* cmsHTRANSFORM;
typedef int   cmsInt32Number;
typedef unsigned int cmsUInt32Number;
typedef unsigned short cmsUInt16Number;
typedef double cmsFloat64Number;

#define TYPE_RGB_16 0
#define cmsFLAGS_NOCACHE 0

// Forward declaration of the focal function to be tested (from testcms2.c)
extern "C" void SpeedTest16bits(const char * Title, cmsHPROFILE hlcmsProfileIn, cmsHPROFILE hlcmsProfileOut, cmsInt32Number Intent);

// Lightweight "public" API used by SpeedTest16bits (stubs provided below)
extern "C" void Die(const char* Reason, ...);
extern "C" void* chknull(void* mem);
extern "C" cmsContext DbgThread(void);
extern "C" void TitlePerformance(const char* Txt);
extern "C" void PrintPerformance(cmsUInt32Number Bytes, cmsUInt32Number SizeOfPixel, cmsFloat64Number diff);

extern "C" cmsHTRANSFORM cmsCreateTransformTHR(cmsContext ContextID, cmsHPROFILE Input, cmsUInt32Number InputFormat,
                                            cmsHPROFILE Output, cmsUInt32Number OutputFormat,
                                            cmsInt32Number Intent, unsigned long Flags);
extern "C" void cmsCloseProfile(cmsHPROFILE h);
extern "C" void cmsDeleteTransform(cmsHTRANSFORM xform);
extern "C" void cmsDoTransform(cmsHTRANSFORM xform, const void* In, void* Out, cmsUInt32Number Size);

// Stubs + test hooks
static bool die_called = false;
static int  create_transform_calls = 0;
static int  close_profile_calls = 0;
static int  delete_transform_calls = 0;
static int  do_transform_calls = 0;

// Stub implementations to intercept and simulate the runtime environment

extern "C" void Die(const char* Reason, ...) {
    // Do not terminate test run; simply note the invocation
    die_called = true;
}

// Simply return the provided pointer; used to mimic allocation checks
extern "C" void* chknull(void* mem) {
    return mem;
}

extern "C" cmsContext DbgThread(void) {
    return nullptr;
}

extern "C" void TitlePerformance(const char* Txt) {
    // No-op for tests
}

extern "C" void PrintPerformance(cmsUInt32Number Bytes, cmsUInt32Number SizeOfPixel, cmsFloat64Number diff) {
    // No-op for tests
}

extern "C" cmsHTRANSFORM cmsCreateTransformTHR(cmsContext ContextID, cmsHPROFILE Input, cmsUInt32Number InputFormat,
                                            cmsHPROFILE Output, cmsUInt32Number OutputFormat,
                                            cmsInt32Number Intent, unsigned long Flags) {
    // Simulate creating a transform handle
    (void)ContextID; (void)Input; (void)InputFormat; (void)Output; (void)OutputFormat;
    (void)Intent; (void)Flags;
    create_transform_calls++;
    return (cmsHTRANSFORM) (void*)0x1; // opaque handle
}

extern "C" void cmsCloseProfile(cmsHPROFILE h) {
    (void)h;
    close_profile_calls++;
}

extern "C" void cmsDeleteTransform(cmsHTRANSFORM xform) {
    (void)xform;
    delete_transform_calls++;
}

extern "C" void cmsDoTransform(cmsHTRANSFORM xform, const void* In, void* Out, cmsUInt32Number Size) {
    (void)xform;
    (void)In;
    (void)Out;
    (void)Size;
    do_transform_calls++;
    // Intentionally do nothing to keep tests fast and deterministic
}

// Helper to reset per-test counters
static void ResetCounters() {
    die_called = false;
    create_transform_calls = 0;
    close_profile_calls = 0;
    delete_transform_calls = 0;
    do_transform_calls = 0;
}

// Test 1: Both input and output profiles are NULL -> should trigger Die path
static void Test_NullProfiles_BothNull() {
    ResetCounters();
    // Call with both NULL to exercise the error branch
    SpeedTest16bits("Test Null Profiles (Both NULL)", nullptr, nullptr, 0);

    if (die_called) {
        std::cout << "[PASS] Null profiles (both NULL) correctly invoked Die()." << std::endl;
    } else {
        std::cout << "[FAIL] Null profiles (both NULL) did not invoke Die()." << std::endl;
    }

    // Reset for next tests
    ResetCounters();
}

// Test 2: Valid (non-NULL) profiles -> exercise the transform path
static void Test_ValidProfiles() {
    ResetCounters();

    // Use non-NULL dummy profile handles
    cmsHPROFILE in  = reinterpret_cast<cmsHPROFILE>(0xA1);
    cmsHPROFILE out = reinterpret_cast<cmsHPROFILE>(0xB2);

    SpeedTest16bits("Test Valid Profiles", in, out, 0);

    // Validate that transform path was exercised and clean-up occurred
    if (create_transform_calls > 0 && close_profile_calls >= 2 && delete_transform_calls == 1) {
        // do_transform should also be invoked at least once in the non-empty run
        if (do_transform_calls > 0) {
            std::cout << "[PASS] Valid profiles path exercised (transform created, profiles closed, transform deleted, DoTransform called)." << std::endl;
        } else {
            std::cout << "[WARN] Valid profiles path did not invoke DoTransform, which may be okay for stubbed behavior." << std::endl;
        }
    } else {
        std::cout << "[FAIL] Valid profiles path did not trigger expected stub interactions." << std::endl;
    }

    ResetCounters();
}

// Test 3: One NULL profile (either input or output) -> should trigger Die path
static void Test_OneNullProfile() {
    ResetCounters();

    // NULL input, valid output -> should trigger Die
    SpeedTest16bits("Test One Null Profile (Input NULL)", nullptr, reinterpret_cast<cmsHPROFILE>(0xDE), 0);

    if (die_called) {
        std::cout << "[PASS] One NULL profile (input NULL) correctly invoked Die()." << std::endl;
    } else {
        std::cout << "[FAIL] One NULL profile (input NULL) did not invoke Die()." << std::endl;
    }

    ResetCounters();
}

// Entry point
int main() {
    // Explain tests
    std::cout << "Starting unit tests for SpeedTest16bits (test harness with stubs)." << std::endl;

    Test_NullProfiles_BothNull();
    Test_ValidProfiles();
    Test_OneNullProfile();

    std::cout << "Unit tests completed." << std::endl;
    return 0;
}