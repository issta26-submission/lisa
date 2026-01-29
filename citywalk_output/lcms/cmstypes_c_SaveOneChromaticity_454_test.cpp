/*
  Test suite for the focal method:
  cmsBool SaveOneChromaticity(cmsFloat64Number x, cmsFloat64Number y, cmsIOHANDLER* io)

  Notes:
  - No GTest is used. A small, self-contained test harness is implemented.
  - We focus on the two-branch behavior of the implementation:
      - Both writes succeed -> returns TRUE.
      - Any write fails -> returns FALSE.
  - We verify behavior by capturing write calls through a fake cmsIOHANDLER Write callback.
  - Endianness-agnostic checks: we assert the number of write calls and total bytes written,
    rather than exact byte patterns, to remain robust across platforms.
  - Tests are written using small, non-terminating assertions (EXPECT_* style) implemented as macros.
  - Tests can be run by compiling this file with the rest of the project (no GTest required).
*/

#include <cstring>
#include <vector>
#include <iostream>
#include <lcms2_internal.h>
#include <lcms2.h>
#include <cstdint>


// Include public LCMS types to match the project environment

// Forward declaration of the focal function (C linkage)
extern "C" cmsBool SaveOneChromaticity(cmsFloat64Number x, cmsFloat64Number y, cmsIOHANDLER* io);

// ---------------------------------------------------------------------------
// Lightweight test harness (non-terminating assertions)
// ---------------------------------------------------------------------------

static int g_total_failures = 0;
#define EXPECT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << (msg) << std::endl; \
        ++g_total_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
    if(!((a) == (b))) { \
        std::cerr << "EXPECT_EQ failed: " << (msg) << " (actual: " << (a) << ", expected: " << (b) << ")" << std::endl; \
        ++g_total_failures; \
    } \
} while(0)

#define EXPECT_GE(a, b, msg) do { \
    if(!((a) >= (b))) { \
        std::cerr << "EXPECT_GE failed: " << (msg) << " (actual: " << (a) << ", min: " << (b) << ")" << std::endl; \
        ++g_total_failures; \
    } \
} while(0)


// ---------------------------------------------------------------------------
// Fake io Write callback to capture behavior of _cmsWriteUInt32Number(io,...)
// ---------------------------------------------------------------------------

// Global test state for the fake IO write path
static std::vector<uint8_t> g_written_bytes;
static int g_write_calls;                    // number of times the Write callback was invoked
static bool g_fail_on_next_write;             // fail on the immediate next Write call
static bool g_fail_on_second_write;           // fail specifically on the second Write call

extern "C" cmsBool TestWriteCallback(struct cmsIOHANDLER* io, cmsUInt32Number Size, const cmsUInt8Number* Buffer)
{
    (void)io; // not used in this simplified test harness

    if (g_fail_on_next_write) {
        g_fail_on_next_write = false;
        return FALSE;
    }

    if (g_fail_on_second_write && g_write_calls == 1) {
        // Fail on the second call
        ++g_write_calls;
        return FALSE;
    }

    // Record the bytes written (size must be 4 for a 32-bit value)
    g_write_calls++;
    g_written_bytes.insert(g_written_bytes.end(), Buffer, Buffer + Size);

    return TRUE;
}

// Helper to reset test state
static void reset_test_state() {
    g_written_bytes.clear();
    g_write_calls = 0;
    g_fail_on_next_write = false;
    g_fail_on_second_write = false;
}

// ---------------------------------------------------------------------------
// Test helpers for the focal method
// ---------------------------------------------------------------------------

// Helper to setup a cmsIOHANDLER with our TestWriteCallback
static void setup_io_with_test_writer(cmsIOHANDLER& io) {
    io.Write = &TestWriteCallback;
    io.Read = nullptr; // not used in these tests
}

// Test 1: Both writes succeed -> SaveOneChromaticity should return TRUE and perform two writes
bool test_SaveOneChromaticity_Succeeds() {
    reset_test_state();

    cmsIOHANDLER io;
    setup_io_with_test_writer(io);

    cmsBool result = SaveOneChromaticity(0.5, -0.25, &io);

    // Expectations
    EXPECT_TRUE(result, "SaveOneChromaticity should return TRUE when both writes succeed");
    EXPECT_EQ(g_write_calls, 2, "SaveOneChromaticity should perform exactly two write operations");
    EXPECT_GE(g_written_bytes.size(), 8, "Two 32-bit values should be written (at least 8 bytes total)");
    // We avoid strict endianness checks to maintain portability; however we can ensure two 4-byte writes occurred.
    return (g_write_calls == 2) && (result == TRUE);
}

// Test 2: First write fails -> SaveOneChromaticity should return FALSE and perform no writes
bool test_SaveOneChromaticity_FirstWriteFails() {
    reset_test_state();

    cmsIOHANDLER io;
    setup_io_with_test_writer(io);

    // Configure to fail on the next write (first call)
    g_fail_on_next_write = true;

    cmsBool result = SaveOneChromaticity(0.3, 0.7, &io);

    // Expectations
    EXPECT_TRUE(result == FALSE, "SaveOneChromaticity should return FALSE when the first write fails");
    EXPECT_EQ(g_write_calls, 0, "No write should have been committed when the first write fails");
    return (result == FALSE) && (g_write_calls == 0);
}

// Test 3: Second write fails -> SaveOneChromaticity should return FALSE after one successful write
bool test_SaveOneChromaticity_SecondWriteFails() {
    reset_test_state();

    cmsIOHANDLER io;
    setup_io_with_test_writer(io);

    // Configure to fail specifically on the second write
    g_fail_on_second_write = true;

    cmsBool result = SaveOneChromaticity(0.1, -0.9, &io);

    // Expectations
    EXPECT_TRUE(result == FALSE, "SaveOneChromaticity should return FALSE when the second write fails");
    EXPECT_EQ(g_write_calls, 2, "Two write attempts should have occurred (first succeeds, second fails)");
    return (result == FALSE) && (g_write_calls == 2);
}

// ---------------------------------------------------------------------------
// Main test runner
// ---------------------------------------------------------------------------

int main() {
    int tests_run = 0;
    int tests_failed = 0;

    // Run Test 1
    bool t1 = test_SaveOneChromaticity_Succeeds();
    tests_run++;
    if (!t1) ++tests_failed;

    // Run Test 2
    bool t2 = test_SaveOneChromaticity_FirstWriteFails();
    tests_run++;
    if (!t2) ++tests_failed;

    // Run Test 3
    bool t3 = test_SaveOneChromaticity_SecondWriteFails();
    tests_run++;
    if (!t3) ++tests_failed;

    // Summary
    std::cout << "Unit test summary: " << (tests_run - tests_failed) << " / " << tests_run << " tests passed." << std::endl;
    if (tests_failed > 0) {
        std::cerr << tests_failed << " test(s) failed." << std::endl;
    }

    // Return number of failures as exit status for easy scripting
    return tests_failed;
}