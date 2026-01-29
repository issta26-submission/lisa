// Lightweight C++11 unit tests for the MemSize function from threaded_split.c
// This test suite does not rely on Google Test. It uses a small, self-contained test harness
// with non-terminating assertions that report failures but continue execution.
// The tests assume the presence of the LittleCMS-like helpers declared in threaded_internal.h.

#include <threaded_internal.h>
#include <cstdint>
#include <iostream>
#include <cstdlib>


extern "C" {
    // Declaration of the focal function under test.
    // The actual implementation is provided in threaded_split.c / the linked C source.
    cmsUInt32Number MemSize(cmsUInt32Number format,
                          cmsUInt32Number PixelsPerLine,
                          cmsUInt32Number LineCount,
                          cmsUInt32Number* BytesPerLine,
                          cmsUInt32Number BytesPerPlane);

    // The following helpers/macros are assumed to exist in the project as per the provided
    // <FOCAL_CLASS_DEP> block. They help determine format properties and component sizes.
    cmsINLINE cmsUInt32Number ComponentSize(cmsUInt32Number format);
    cmsINLINE cmsUInt32Number PixelSpacing(cmsUInt32Number format);
    cmsINLINE cmsUInt32Number minimum(cmsUInt32Number a, cmsUInt32Number b);

    // Macros to query format properties used by MemSize
    // These are typically defined as macros in the project, e.g.:
    // #define T_PLANAR(fmt) (...) etc.
    // We rely on their availability in the test environment.
    #ifndef T_PLANAR
    bool T_PLANAR(cmsUInt32Number format);
    #endif
    #ifndef T_CHANNELS
    cmsUInt32Number T_CHANNELS(cmsUInt32Number format);
    #endif
    #ifndef T_EXTRA
    cmsUInt32Number T_EXTRA(cmsUInt32Number format);
    #endif
}

// Lightweight test harness: non-terminating assertions that report failures but continue.
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_EQ(actual, expected) \
    do { \
        ++g_total_tests; \
        if ((actual) != (expected)) { \
            std::cerr << "[FAIL] " << __func__ << ":" \
                      << " line " << __LINE__ << ": " \
                      << #actual << " == " << actual << " differs from " \
                      << #expected << " == " << expected << std::endl; \
            ++g_failed_tests; \
        } \
    } while (0)

#define EXPECT_TRUE(cond) \
    do { \
        ++g_total_tests; \
        if (!(cond)) { \
            std::cerr << "[FAIL] " << __func__ << ":" << __LINE__ \
                      << ": condition " << #cond << " is false" << std::endl; \
            ++g_failed_tests; \
        } \
    } while (0)

#define EXPECT_NOT_THROW(statement) \
    do { \
        try { statement; } \
        catch (...) { \
            std::cerr << "[FAIL] " << __func__ << ":" << __LINE__ \
                      << ": unexpected exception thrown by " << #statement << std::endl; \
            ++g_failed_tests; \
        } \
    } while (0)


// Helper to run a single test and print its name
#define RUN_TEST(test_fn) \
    do { \
        std::cout << "Running " << #test_fn << "..." << std::endl; \
        test_fn(); \
        std::cout << "Finished " << #test_fn << std::endl; \
    } while (0)


// Test 1: Planar format with BytesPerLine initially 0 should initialize BytesPerLine to ComponentSize(format) * PixelsPerLine
void test_Planar_BytesPerLineZero_SetsValue() {
    // Choose a planar format value. The exact numeric value is not important as long as the
    // T_PLANAR macro returns true for it.
    const cmsUInt32Number fmtPlanar = 0x010000; // arbitrary, expected to be planar by the macro in the environment

    cmsUInt32Number BytesPerLine = 0;
    cmsUInt32Number BytesPerPlane = 4;
    cmsUInt32Number PixelsPerLine = 8;
    cmsUInt32Number LineCount = 3;

    cmsUInt32Number expectedBytesPerLine = ComponentSize(fmtPlanar) * PixelsPerLine;
    cmsUInt32Number expectedMemSize = (T_PLANAR(fmtPlanar) ? (T_CHANNELS(fmtPlanar) + T_EXTRA(fmtPlanar)) * BytesPerPlane
                                                           : LineCount * expectedBytesPerLine);

    // Call MemSize
    cmsUInt32Number mem = MemSize(fmtPlanar, PixelsPerLine, LineCount, &BytesPerLine, BytesPerPlane);

    // Validate
    EXPECT_EQ(BytesPerLine, expectedBytesPerLine);
    EXPECT_EQ(mem, expectedMemSize);
}

// Test 2: Planar format with non-zero BytesPerLine should leave BytesPerLine unchanged
void test_Planar_BytesPerLineNonZero_Unchanged() {
    const cmsUInt32Number fmtPlanar = 0x010000; // same as above; planar
    cmsUInt32Number BytesPerLine = 1234; // non-zero pre-set
    cmsUInt32Number BytesPerPlane = 4;
    cmsUInt32Number PixelsPerLine = 8;
    cmsUInt32Number LineCount = 3;

    cmsUInt32Number expectedBytesPerLine = BytesPerLine;
    cmsUInt32Number expectedMemSize = (T_PLANAR(fmtPlanar) ? (T_CHANNELS(fmtPlanar) + T_EXTRA(fmtPlanar)) * BytesPerPlane
                                                         : LineCount * BytesPerLine);

    cmsUInt32Number mem = MemSize(fmtPlanar, PixelsPerLine, LineCount, &BytesPerLine, BytesPerPlane);

    EXPECT_EQ(BytesPerLine, expectedBytesPerLine);
    EXPECT_EQ(mem, expectedMemSize);
}

// Test 3: Non-planar format with BytesPerLine initially 0 should compute based on ComponentSize * (Channels + Extra) * PixelsPerLine
void test_NonPlanar_BytesPerLineZero_SetsValue() {
    const cmsUInt32Number fmtNonPlanar = 0x020000; // arbitrary; expected non-planar by macro
    cmsUInt32Number BytesPerLine = 0;
    cmsUInt32Number BytesPerPlane = 0; // not used in non-planar path
    cmsUInt32Number PixelsPerLine = 5;
    cmsUInt32Number LineCount = 10;

    cmsUInt32Number expectedBytesPerLine = ComponentSize(fmtNonPlanar) * (T_CHANNELS(fmtNonPlanar) + T_EXTRA(fmtNonPlanar)) * PixelsPerLine;
    cmsUInt32Number expectedMemSize = LineCount * expectedBytesPerLine;

    cmsUInt32Number mem = MemSize(fmtNonPlanar, PixelsPerLine, LineCount, &BytesPerLine, /*BytesPerPlane*/ 1);

    // In non-planar branch, MemSize computes BytesPerLine when zero and returns LineCount * BytesPerLine
    EXPECT_EQ(BytesPerLine, expectedBytesPerLine);
    EXPECT_EQ(mem, expectedMemSize);
}

// Test 4: Non-planar format with non-zero BytesPerLine should leave BytesPerLine unchanged
void test_NonPlanar_BytesPerLineNonZero_Unchanged() {
    const cmsUInt32Number fmtNonPlanar = 0x020000; // non-planar
    cmsUInt32Number BytesPerLine = 7;
    cmsUInt32Number BytesPerPlane = 1;
    cmsUInt32Number PixelsPerLine = 5;
    cmsUInt32Number LineCount = 10;

    cmsUInt32Number expectedBytesPerLine = BytesPerLine;
    cmsUInt32Number expectedMemSize = LineCount * BytesPerLine;

    cmsUInt32Number mem = MemSize(fmtNonPlanar, PixelsPerLine, LineCount, &BytesPerLine, BytesPerPlane);

    EXPECT_EQ(BytesPerLine, expectedBytesPerLine);
    EXPECT_EQ(mem, expectedMemSize);
}


// Main runner
int main() {
    std::cout << "Starting MemSize unit tests (non-GTest, self-contained)..." << std::endl;

    RUN_TEST(test_Planar_BytesPerLineZero_SetsValue);
    RUN_TEST(test_Planar_BytesPerLineNonZero_Unchanged);
    RUN_TEST(test_NonPlanar_BytesPerLineZero_SetsValue);
    RUN_TEST(test_NonPlanar_BytesPerLineNonZero_Unchanged);

    std::cout << "MemSize unit tests finished. Total: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;

    // Return non-zero if any test failed to aid integration with build systems.
    return (g_failed_tests == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}