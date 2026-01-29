// cmspack_test.cpp
// Lightweight unit tests for Pack1ByteSkip1SwapFirst in cmspack.c
// This test is written to be compiled with C++11, without GoogleTest.
// It relies on the external cmspack.c being available and linked.
// The test uses a small, non-terminating assertion framework and prints a summary.

#include <lcms2_internal.h>
#include <cstdio>
#include <cstdint>
#include <cstddef>


// Lightweight testing primitives (non-terminating)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_BEGIN(name) \
    do { printf("Running test: %s\n", name); } while(0)

#define EXPECT_TRUE(cond) \
    do { \
        if(!(cond)) { \
            printf("  [FAIL] %s: condition is false\n", __func__); \
            ++g_tests_failed; \
        } else { \
            /* comment: pass-through */ \
        } \
        ++g_tests_run; \
    } while(0)

#define EXPECT_EQ(a, b) \
    do { \
        if(!((a) == (b))) { \
            printf("  [FAIL] %s: Expected (%s) == (%s), actual (%ld) != (%ld)\n", __func__, #a, #b, static_cast<long>( (a) ), static_cast<long>( (b) )); \
            ++g_tests_failed; \
        } else { \
            /* matched */ \
        } \
        ++g_tests_run; \
    } while(0)

#define EXPECT_NE(a, b) \
    do { \
        if((a) == (b)) { \
            printf("  [FAIL] %s: Expected (%s) != (%s), but both are equal to %ld\n", __func__, #a, #b, static_cast<long>( (a) ); \
            ++g_tests_failed; \
        } else { \
            /* different */ \
        } \
        ++g_tests_run; \
    } while(0)

// ------------------------------------------------------------------------------------
// Per-project types and macros (minimal stubs for compilation with cmspack.c)
// These definitions are provided so the test can be compiled standalone in a
// development environment. The real project will provide the actual definitions
// via its headers, and cmspack.c will be linked accordingly.
// ------------------------------------------------------------------------------------

typedef unsigned char cmsUInt8Number;
typedef unsigned short cmsUInt16Number;
typedef unsigned int cmsUInt32Number;

// CMSREGISTER is a no-op macro in modern compilers; keep for compatibility
#ifndef CMSREGISTER
#define CMSREGISTER
#endif

// Forward declaration of the _cmsTRANSFORM type used by Pack1ByteSkip1SwapFirst.
// The actual layout is not needed for this unit test since we don't dereference
// info in this particular focal function.
typedef struct _cmsTRANSFORM _cmsTRANSFORM;

// Declare the focal function from cmspack.c
extern "C" cmsUInt8Number* Pack1ByteSkip1SwapFirst(CMSREGISTER _cmsTRANSFORM* info,
                                                  CMSREGISTER cmsUInt16Number wOut[],
                                                  CMSREGISTER cmsUInt8Number* output,
                                                  CMSREGISTER cmsUInt32Number Stride);

// ------------------------------------------------------------------------------------
// Test helpers
static void test_pack1byte_skip1_swap_first_basic()
{
    TEST_BEGIN("Pack1ByteSkip1SwapFirst_basic");

    // Setup: wOut[0] = 0x1234; expected written byte = 0x34 (low 8 bits)
    cmsUInt16Number wOut[1] = { static_cast<cmsUInt16Number>(0x1234) };

    // Output buffer large enough to hold two writes (we increment output twice)
    unsigned char buf[4] = { 0, 0, 0, 0 };

    // Initial output pointer points to buf[0]
    unsigned char* outPtr = buf;

    // Dummy info and Stride values (unused by the focal function)
    _cmsTRANSFORM* info = nullptr;
    cmsUInt32Number Stride = 0;

    cmsUInt8Number* ret = Pack1ByteSkip1SwapFirst(info, wOut, outPtr, Stride);

    // Expect: buf[1] == 0x34 and ret == buf + 2
    EXPECT_TRUE(buf[0] == 0);              // untouched
    EXPECT_EQ(static_cast<long>(buf[1]), static_cast<long>(0x34));
    EXPECT_EQ(static_cast<long>(ret - buf), 2);

    // Additional checks for robustness
    // Ensure second byte is not overwritten by accident
    EXPECT_TRUE(buf[2] == 0);
    EXPECT_TRUE(buf[3] == 0);

    // Ensure the function returns a non-null pointer
    EXPECT_TRUE(ret != nullptr);
}

// Test with a different 16-bit input, e.g., 0xABCD -> expect 0xCD
static void test_pack1byte_skip1_swap_first_wrap()
{
    TEST_BEGIN("Pack1ByteSkip1SwapFirst_wrap");

    cmsUInt16Number wOut[1] = { static_cast<cmsUInt16Number>(0xABCD) };
    unsigned char buf[4] = { 0, 0, 0, 0 };
    unsigned char* outPtr = buf;
    _cmsTRANSFORM* info = nullptr;
    cmsUInt32Number Stride = 0;

    cmsUInt8Number* ret = Pack1ByteSkip1SwapFirst(info, wOut, outPtr, Stride);

    // Expect: buf[1] == 0xCD
    EXPECT_EQ(static_cast<long>(buf[1]), static_cast<long>(0xCD));
    EXPECT_EQ(static_cast<long>(ret - buf), 2);
}

// Test multiple sequential invocations on the same buffer
static void test_pack1byte_skip1_swap_first_multiple_calls()
{
    TEST_BEGIN("Pack1ByteSkip1SwapFirst_multiple_calls");

    cmsUInt16Number wOut1[1] = { static_cast<cmsUInt16Number>(0x1111) };
    cmsUInt16Number wOut2[1] = { static_cast<cmsUInt16Number>(0x2222) };

    unsigned char buf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    unsigned char* outPtr = buf;
    _cmsTRANSFORM* info = nullptr;
    cmsUInt32Number Stride = 0;

    // First call
    cmsUInt8Number* ret1 = Pack1ByteSkip1SwapFirst(info, wOut1, outPtr, Stride);
    // Second call using the returned pointer as the new output position
    cmsUInt8Number* ret2 = Pack1ByteSkip1SwapFirst(info, wOut2, ret1, Stride);

    // First call wrote at buf[1] = 0x11
    EXPECT_EQ(static_cast<long>(buf[1]), static_cast<long>(0x11));
    // Second call wrote at buf[3] = 0x22
    EXPECT_EQ(static_cast<long>(buf[3]), static_cast<long>(0x22));

    // Check pointer arithmetic: ret1 should be buf+2, ret2 should be buf+4
    EXPECT_EQ(static_cast<long>(ret1 - buf), 2);
    EXPECT_EQ(static_cast<long>(ret2 - buf), 4);
}

// ------------------------------------------------------------------------------------
// Main entry
int main()
{
    // Run tests
    test_pack1byte_skip1_swap_first_basic();
    test_pack1byte_skip1_swap_first_wrap();
    test_pack1byte_skip1_swap_first_multiple_calls();

    // Summary
    printf("Tests run: %d, Failures: %d\n", g_tests_run, g_tests_failed);
    // Return non-zero if any test failed to signal issues during automated runs
    return (g_tests_failed == 0) ? 0 : 1;
}