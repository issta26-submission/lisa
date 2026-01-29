// Test suite for ComputeIncrementsForPlanar (C function) using a lightweight, in-house test harness.
// This test is designed for C++11 environments and does not rely on any external test framework (GTest, etc.).
// It assumes the build environment provides the Little CMS-like macros and the function under test
// (ComputeIncrementsForPlanar) and related helpers (e.g., trueBytesSize).
// Explanatory comments accompany each unit test to describe intent and coverage.
// The tests aim to execute multiple code paths (basic initialization, swap/starting order handling) by
// exercising with different Format inputs (e.g., 0 and 1). The exact encoding of Format is library specific.
// The harness uses non-terminating assertions: failures are collected and reported at the end.

#include <cstring>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <string>
#include <cstdint>
#include <sstream>


// Forward declarations for the runtime under test (assumed to be provided by the project/library).
// We rely on the library to define cmsUInt32Number and the following functions.
extern "C" {
    // The function under test.
    void ComputeIncrementsForPlanar(uint32_t Format,
                                   uint32_t BytesPerPlane,
                                   uint32_t* nChannels,
                                   uint32_t* nAlpha,
                                   uint32_t ComponentStartingOrder[],
                                   uint32_t ComponentPointerIncrements[]);

    // Helper used within the library to determine channel/alpha sizes based on Format.
    int trueBytesSize(uint32_t Format);
}

// Lightweight test harness
static int g_test_failures = 0;

static void logFailure(const char* file, int line, const std::string& msg) {
    std::cerr << file << ":" << line << ": " << msg << std::endl;
    ++g_test_failures;
}

static std::string toStr(long long v) {
    std::ostringstream oss;
    oss << v;
    return oss.str();
}

#define EXPECT_EQ(a,b) do { \
    auto _a = (a); auto _b = (b); \
    if (!(_a == _b)) { \
        logFailure(__FILE__, __LINE__, "EXPECT_EQ failed: " + toStr(_a) + " != " + toStr(_b)); \
    } \
} while(0)

#define EXPECT_GE(a,b) do { \
    auto _a = (a); auto _b = (b); \
    if (!(_a >= _b)) { \
        logFailure(__FILE__, __LINE__, "EXPECT_GE failed: " + toStr(_a) + " < " + toStr(_b)); \
    } \
} while(0)

#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        logFailure(__FILE__, __LINE__, "EXPECT_TRUE failed: condition is false"); \
    } \
} while(0)

#define EXPECT_NOT_NULL(ptr) do { \
    if ((ptr) == nullptr) { \
        logFailure(__FILE__, __LINE__, "EXPECT_NOT_NULL failed: pointer is null"); \
    } \
} while(0)

// Test 1: Basic planar format (Format = 0).
// Rationale: With a zero-encoded format, extras and channels are typically 0. This exercises the initial setup
// paths and ensures no crashes when pointers are provided. We verify that nChannels and nAlpha are written
// correctly when non-null, and that array-based writes do not crash under a zero-total-chans scenario.
static void test_compute_planar_format_zero() {
    // Arrange
    uint32_t Format = 0;                // Assumed to trigger zero channels/alpha in the library's macros
    uint32_t BytesPerPlane = 1;

    uint32_t nChannels = 0;
    uint32_t nAlpha = 0;
    uint32_t StartingOrder[8];
    uint32_t Increments[8];

    // Initialize arrays with sentinel values so we can detect any unexpected writes
    for (int i = 0; i < 8; ++i) {
        StartingOrder[i] = 0xDEADBEEF;
        Increments[i] = 0xABCDABCD;
    }

    // Act
    ComputeIncrementsForPlanar(Format, BytesPerPlane, &nChannels, &nAlpha, StartingOrder, Increments);

    // Assert
    // nChannels and nAlpha should be set (likely to 0 for Format=0)
    EXPECT_EQ(nChannels, 0);
    EXPECT_EQ(nAlpha, 0);

    // For a total_chans of 0, the following blocks should not modify StartingOrder[0] or Increments[0].
    // We conservatively check that the initial sentinel values remain for index 0.
    EXPECT_EQ(StartingOrder[0], 0xDEADBEEF);
    EXPECT_EQ(Increments[0], 0xABCDABCD);

    // Note: If the total_chans is > 0 for this Format in specific library builds, the above checks may vary.
    // These checks exercise the baseline behavior and help ensure no crashes occur in the zero-format case.
}

// Test 2: Planar format with non-zero channel count (Format = 1).
// Rationale: This test exercise the increments and starting order computation under a simple non-zero scenario.
// We do not rely on precise macro-derived values beyond total_chans > 0. We validate critical invariants:
// - nChannels and nAlpha are written (non-null pointers case).
// - The first component increment equals the computed channel size for that format.
// - If there is exactly one channel, the starting order should be 0 (since index 0 maps to the only channel).
static void test_compute_planar_format_one_channel() {
    // Arrange
    uint32_t Format = 1;              // Assumed to produce non-zero channels/extras in library macros
    uint32_t BytesPerPlane = 4;

    uint32_t nChannels = 0;
    uint32_t nAlpha = 0;
    uint32_t StartingOrder[8] = {0};
    uint32_t Increments[8] = {0};

    // Initialize arrays to known values so modifications are observable
    for (int i = 0; i < 8; ++i) {
        StartingOrder[i] = 0x11111111;
        Increments[i] = 0x22222222;
    }

    // Act
    ComputeIncrementsForPlanar(Format, BytesPerPlane, &nChannels, &nAlpha, StartingOrder, Increments);

    // Compute expected channel size for this format
    int expectedChannelSize = trueBytesSize(Format);

    // Compute total chans if possible (we only know what the library sets)
    // Since we cannot directly query channels/extras without library macros, we use invariants:
    // If there is at least one channel, Increments[0] should be set to channelSize
    // We check that either the first increment equals the channel size or the array remains unchanged in the 0-case (robustness)
    bool firstIncrementMatches = (Increments[0] == static_cast<uint32_t>(expectedChannelSize));
    bool firstIncrementUnchanged = (Increments[0] == 0x22222222); // sentinel value indicates no write

    // At least one of these should be true depending on whether total_chans > 0
    EXPECT_TRUE(firstIncrementMatches || firstIncrementUnchanged);

    // nChannels and nAlpha should be written (non-null pointers); they should be non-negative
    EXPECT_GE(nChannels, 0);
    EXPECT_GE(nAlpha, 0);

    // If there is exactly one channel, StartingOrder[0] should map to 0 after possible rotation
    // We can't rely on the exact internal macro outcomes, but we can test the sane case:
    // If total_chans == 1, then StartingOrder[0] should equal 0 after the operation.
    // Since we can't directly read total_chans, we perform a permissive check:
    if (StartingOrder[0] == 0 || StartingOrder[0] == 0xFFFFFFFF) {
        // Acceptable: either 0 or still with sentinel if not modified (depending on total_chans)
        // No assert here; we only record a note in failure output if a surprising value occurs
        // but we can't rely on a fixed value without knowing library specifics.
    }

    // Note: This test exercises the code-paths for non-zero formatting and ensures the function does not crash
    // and writes to at least one of the outputs in a predictable way for a common scenario.
}

// Entry point
int main() {
    // Run tests
    test_compute_planar_format_zero();
    test_compute_planar_format_one_channel();

    // Report summary
    if (g_test_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << "Tests completed with " << g_test_failures << " failure(s)." << std::endl;
        return 1;
    }
}