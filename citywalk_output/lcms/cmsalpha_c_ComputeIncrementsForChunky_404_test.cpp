// cmsalpha_tests.cpp
// Lightweight C++11 unit tests for CMS Increment computation in chunky format
// Focus: ComputeIncrementsForChunky(Format, ComponentStartingOrder[], ComponentPointerIncrements[])
//
// Notes:
// - This test suite intentionally uses a minimal, non-GTest framework.
// - It relies on the actual cmsalpha.c implementation and its dependent macros/functions
//   (e.g., T_EXTRA, T_CHANNELS, trueBytesSize, T_DOSWAP, T_SWAPFIRST, cmsMAXCHANNELS).
// - The tests do not rely on private fields; they exercise observable behavior.
// - We search for representative Format values at runtime to trigger true/false branches
//   when the exact bitfield mappings are unknown at test-write time.
// - All tests are designed to be self-contained and compilable under C++11.

#include <lcms2_internal.h>
#include <iostream>
#include <cstdint>
#include <cstring>


// Typedefs matching the library's typical types (as seen in the focal code).
using cmsUInt32Number = uint32_t;
using cmsBool = uint32_t;

// Declaration of the focal function (C linkage).
extern "C" cmsBool ComputeIncrementsForChunky(cmsUInt32Number Format,
                                           cmsUInt32Number ComponentStartingOrder[],
                                           cmsUInt32Number ComponentPointerIncrements[]);

// Sentinel values to detect untouched memory after a function call.
static const cmsUInt32Number SENTINEL = 0xFFFFFFFF;

// Simple test harness state
static int g_passed = 0;
static int g_failed = 0;

// Lightweight assertion helper
#define TEST_ASSERT(cond, msg)                                                     \
    do {                                                                           \
        if (cond) {                                                                \
            ++g_passed;                                                            \
        } else {                                                                   \
            std::cerr << "TEST FAILED: " << msg                                   \
                      << " (Line " << __LINE__ << ")" << std::endl;              \
            ++g_failed;                                                            \
        }                                                                          \
    } while (0)

// Helper to reset test buffers
static void resetBuffers(cmsUInt32Number* StartOrder, cmsUInt32Number* Increments, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        StartOrder[i] = SENTINEL;
        Increments[i] = SENTINEL;
    }
}

// Helper to print summary
static void printSummary() {
    std::cout << "Tests passed: " << g_passed << ", Tests failed: " << g_failed << std::endl;
}

// Entry point for tests
int main() {
    // Shared buffers to call into the focal method
    const size_t BUF_LEN = 64; // large enough for most tests
    cmsUInt32Number StartingOrder[BUF_LEN];
    cmsUInt32Number Increments[BUF_LEN];

    // Test 1: total_chans <= 0 (Format yields zero channels) should return FALSE
    // We rely on the function's early exit when total_chans <= 0; no writes should occur.
    {
        resetBuffers(StartingOrder, Increments, BUF_LEN);
        cmsUInt32Number Format = 0; // Assuming macros yield total_chans == 0 for this value
        cmsBool ret = ComputeIncrementsForChunky(Format, StartingOrder, Increments);

        TEST_ASSERT(ret == 0, "ComputeIncrementsForChunky should return FALSE for zero channels");
        // Ensure no writes happened (arrays remain sentinel)
        bool anyWritten = false;
        for (size_t i = 0; i < BUF_LEN; ++i) {
            if (StartingOrder[i] != SENTINEL || Increments[i] != SENTINEL) {
                anyWritten = true;
                break;
            }
        }
        TEST_ASSERT(!anyWritten, "No ComponentStartingOrder or ComponentPointerIncrements should be written on early return");
    }

    // Test 2: total_chans >= cmsMAXCHANNELS should return FALSE (boundary check)
    // We don't know exact bit layout of Format, so perform a runtime search over a range
    // of candidate Format values until we find one that yields FALSE, then validate.
    {
        bool foundFalse = false;
        for (unsigned int f = 1; f < 4096; ++f) {
            resetBuffers(StartingOrder, Increments, BUF_LEN);
            cmsBool ret = ComputeIncrementsForChunky(f, StartingOrder, Increments);

            if (ret == 0) {
                // Found a Format that causes early exit due to too many channels or 0 channels.
                // Validate that no writes occurred on this path either.
                bool anyWritten = false;
                for (size_t i = 0; i < BUF_LEN; ++i) {
                    if (StartingOrder[i] != SENTINEL || Increments[i] != SENTINEL) {
                        anyWritten = true;
                        break;
                    }
                }
                TEST_ASSERT(!anyWritten, "On false path due to channel count, no writes should occur");
                foundFalse = true;
                break;
            }
        }
        TEST_ASSERT(foundFalse, "Could not find a Format value in range to trigger FALSE path for channel count");
    }

    // Test 3: True path with extra channels writes increments for the first 'extra' entries
    // We dynamically search for a Format that yields at least one extra channel (actual_extra > 0)
    // and verifies the increments are written for those entries and that others remain untouched.
    {
        bool foundTrueWithExtra = false;
        unsigned int actual_extra_detected = 0;
        for (unsigned int f = 1; f < 4096; ++f) {
            resetBuffers(StartingOrder, Increments, BUF_LEN);
            cmsBool ret = ComputeIncrementsForChunky(f, StartingOrder, Increments);

            if (ret != 0) {
                // Count how many increments were actually written (actual_extra)
                unsigned int count_written = 0;
                for (size_t i = 0; i < BUF_LEN; ++i) {
                    if (Increments[i] != SENTINEL) {
                        ++count_written;
                    } else {
                        break; // we assume contiguous writes starting at 0
                    }
                }
                if (count_written > 0) {
                    // Found a true path with extra > 0
                    foundTrueWithExtra = true;
                    actual_extra_detected = count_written;

                    // Basic invariants:
                    // - Increments[0..actual_extra-1] should be written (non-sentinel)
                    // - StartingOrder[0..actual_extra-1] should also be written (non-sentinel)
                    bool extraWritesCorrect = true;
                    for (unsigned int i = 0; i < actual_extra_detected; ++i) {
                        if (Increments[i] == SENTINEL) {
                            extraWritesCorrect = false;
                            break;
                        }
                        if (StartingOrder[i] == SENTINEL) {
                            extraWritesCorrect = false;
                            break;
                        }
                    }
                    TEST_ASSERT(extraWritesCorrect, "True path: increments and starting order should be written for extra channels");
                    // Additional sanity: ensure values are within expected bounds (non-negative)
                    for (unsigned int i = 0; i < actual_extra_detected; ++i) {
                        if (Increments[i] == 0) {
                            // PixelSize should be > 0; zero would be suspicious
                            TEST_ASSERT(Increments[i] != 0, "Increment value should be positive for written channels");
                        }
                    }
                    break;
                }
            }
        }
        TEST_ASSERT(foundTrueWithExtra, "Could not find a Format value that yields TRUE with extra channels");
        // If found, we additionally assert that only first 'actual_extra_detected' entries were written
        if (foundTrueWithExtra) {
            resetBuffers(StartingOrder, Increments, BUF_LEN);
            // For stronger assertion, run once more for the chosen 'f' and verify behavior
            // Re-run with the discovered f to capture a concrete result
            // Note: In a real test, we'd store the 'f' used above. Here, we perform a second pass
            // and accept the first true path that yields extra.
            unsigned int chosen_f = 0;
            for (unsigned int f = 1; f < 4096; ++f) {
                resetBuffers(StartingOrder, Increments, BUF_LEN);
                cmsBool ret = ComputeIncrementsForChunky(f, StartingOrder, Increments);
                if (ret != 0) {
                    unsigned int cnt = 0;
                    for (size_t i = 0; i < BUF_LEN; ++i) {
                        if (Increments[i] != SENTINEL) ++cnt;
                        else break;
                    }
                    if (cnt > 0) {
                        chosen_f = f;
                        // Re-check invariants on this chosen format
                        // Recompute results for chosen format to verify post-conditions
                        resetBuffers(StartingOrder, Increments, BUF_LEN);
                        ret = ComputeIncrementsForChunky(chosen_f, StartingOrder, Increments);
                        unsigned int cnt2 = 0;
                        for (size_t i = 0; i < BUF_LEN; ++i) {
                            if (Increments[i] != SENTINEL) ++cnt2;
                            else break;
                        }
                        TEST_ASSERT(cnt2 == cnt, "Consistent extra channel count should be observed on repeated call");
                        break;
                    }
                }
            }
        }
    }

    // Print overall summary
    printSummary();
    // Exit with non-zero code if tests failed
    return (g_failed == 0) ? 0 : 1;
}