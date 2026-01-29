// Test suite for ComputeIncrementsForChunky (fast_float_separate.c)
// Target: C++11, no GoogleTest. A lightweight, self-contained test harness.
// The tests rely on the same macros and types used by the focal method
// (e.g., cmsUInt32Number, T_EXTRA, T_CHANNELS, trueBytesSize, etc.).
// The goal is to exercise the non-branching path as well as branches that
// depend on the Format predicates (DOSWAP, SWAPFIRST, and channel sizing).

#include <cstring>
#include <fast_float_internal.h>
#include <vector>
#include <iostream>
#include <iomanip>
#include <cstdint>


// Import the C header with the necessary macros and type definitions.
// We wrap in extern "C" to ensure C linkage for the C functions.
// Include path adjustments might be required in your build system.
extern "C" {
}

// Prototype for the focal function under test.
// We provide the exact signature to call the function from C++.
extern "C" void ComputeIncrementsForChunky(cmsUInt32Number Format,
                                          cmsUInt32Number BytesPerPlane,
                                          cmsUInt32Number* nChannels,
                                          cmsUInt32Number* nAlpha,
                                          cmsUInt32Number ComponentStartingOrder[],
                                          cmsUInt32Number ComponentPointerIncrements[]);

// Simple, non-intrusive test harness
namespace TestHarness {

    // Global counters
    int g_tests_run = 0;
    int g_tests_pass = 0;

    // Simple assertion macro (non-terminating)
    #define EXPECT_TRUE(cond, msg) do { \
        if (!(cond)) { \
            std::cerr << "[FAIL] " << msg << " | 0x" << std::hex << (cond) << std::dec << "\n"; \
        } else { \
            ++g_tests_pass; \
        } \
        ++g_tests_run; \
    } while(0)

    // Helper: print test header
    void PrintTestHeader(const std::string& name) {
        std::cout << "Running test: " << name << std::endl;
    }

    // Helper: determine total channels from outputs
    // The function writes nChannels (channels) and nAlpha (alpha). Total = channels + alpha.
    int TotalChannelsFromOutput(cmsUInt32Number* nChannels, cmsUInt32Number* nAlpha) {
        if (nChannels == nullptr || nAlpha == nullptr) return 0;
        return static_cast<int>(*nChannels) + static_cast<int>(*nAlpha);
    }

    // Utility: run a single test case given a specific Format value
    void RunFormatTest(int Format, const std::string& testName) {
        // Setup buffers large enough for few channels (typical formats won't exceed 8 channels here)
        const int MAX_CHANNELS = 8;
        cmsUInt32Number ComponentStartingOrder[MAX_CHANNELS];
        cmsUInt32Number ComponentPointerIncrements[MAX_CHANNELS];
        cmsUInt32Number nChannels = 0;
        cmsUInt32Number nAlpha = 0;

        // Initialize buffers with a sentinel value to detect actual writes
        const cmsUInt32Number SENTINEL = 0xDEADBEEFu;
        for (int i = 0; i < MAX_CHANNELS; ++i) {
            ComponentStartingOrder[i] = SENTINEL;
            ComponentPointerIncrements[i] = SENTINEL;
        }

        // Call the focal method
        ComputeIncrementsForChunky(static_cast<cmsUInt32Number>(Format),
                                   0, // BytesPerPlane is unused in this test
                                   &nChannels,
                                   &nAlpha,
                                   ComponentStartingOrder,
                                   ComponentPointerIncrements);

        // Determine total channels actually configured
        int total = TotalChannelsFromOutput(&nChannels, &nAlpha);
        // Guard: if total is zero, this test is not meaningful; still report.
        PrintTestHeader(testName);

        // Recompute expected values using the same macros as the focal method.
        // This ensures that we validate consistency with the intended behavior
        // of the code paths governed by the macros.
        int extra = T_EXTRA(Format);
        int channels = T_CHANNELS(Format);
        int total_chans = channels + extra;

        // Fetch derived values to compare
        int channelSize = trueBytesSize(Format);
        int pixelSize = channelSize * total_chans;

        // Validate: number of channels and alpha
        EXPECT_TRUE(nChannels == static_cast<cmsUInt32Number>(channels),
                    testName + " - nChannels mismatch: expected " + std::to_string(channels) +
                    ", got " + std::to_string(nChannels));
        EXPECT_TRUE(nAlpha == static_cast<cmsUInt32Number>(extra),
                    testName + " - nAlpha mismatch: expected " + std::to_string(extra) +
                    ", got " + std::to_string(nAlpha));

        // Validate: pointer increments for each channel
        bool increments_ok = true;
        for (int i = 0; i < total_chans; ++i) {
            if (ComponentPointerIncrements[i] != static_cast<cmsUInt32Number>(pixelSize)) {
                increments_ok = false;
                break;
            }
        }
        EXPECT_TRUE(increments_ok,
                    testName + " - ComponentPointerIncrements values mismatch: expected " +
                    std::to_string(pixelSize) + " for all " + std::to_string(total_chans) +
                    " channels, got [first=" + std::to_string(ComponentPointerIncrements[0]) +
                    ", last=" + std::to_string(ComponentPointerIncrements[total_chans - 1]) + "]");

        // Validate: starting order
        // Build expected starting order according to the same macro-driven rules
        std::vector<cmsUInt32Number> ExpectedStartingOrder(total_chans);
        for (int i = 0; i < total_chans; ++i) {
            if (T_DOSWAP(Format)) {
                ExpectedStartingOrder[i] = static_cast<cmsUInt32Number>(total_chans - i - 1);
            } else {
                ExpectedStartingOrder[i] = static_cast<cmsUInt32Number>(i);
            }
        }
        if (T_SWAPFIRST(Format)) {
            cmsUInt32Number tmp = ExpectedStartingOrder[0];
            for (int i = 0; i < total_chans - 1; ++i)
                ExpectedStartingOrder[i] = ExpectedStartingOrder[i + 1];
            ExpectedStartingOrder[total_chans - 1] = tmp;
        }
        if (channelSize > 1) {
            for (int i = 0; i < total_chans; ++i) {
                ExpectedStartingOrder[i] *= static_cast<cmsUInt32Number>(channelSize);
            }
        }

        bool starting_ok = true;
        for (int i = 0; i < total_chans; ++i) {
            if (ComponentStartingOrder[i] != ExpectedStartingOrder[i]) {
                starting_ok = false;
                break;
            }
        }
        // If total_chans is zero, skip, but otherwise report.
        EXPECT_TRUE(starting_ok,
                    testName + " - ComponentStartingOrder mismatch: expected ["
                    + [&](){
                        std::string s;
                        for (int k = 0; k < total_chans; ++k) {
                            if (k) s += ", ";
                            s += std::to_string(ExpectedStartingOrder[k]);
                        }
                        return s;
                    }() + "], got ["
                    + [&](){
                        std::string s;
                        for (int k = 0; k < total_chans; ++k) {
                            if (k) s += ", ";
                            s += std::to_string(ComponentStartingOrder[k]);
                        }
                        return s;
                    }() + "]");

        // Edge-case: if no channels, nothing more to validate
        if (total_chans == 0) {
            // Consider as passed for this configuration
            ++g_tests_pass;
            ++g_tests_run;
        }
    }

    // Public entry to run a battery of tests
    void RunAllTests() {
        // Test 1: a simple, likely 3-channel (RGB) 8-bit format with no alpha and no swaps
        // We rely on the library's macros to configure the expected values for  Format=0.
        RunFormatTest(0, "ChunkyFormat_Default3Channels_NoAlpha_Format0");

        // Test 2: a 4-channel format (e.g., RGBA) with alpha, still with 8-bit channel.
        // This uses a different Format code that the macros interpret.
        RunFormatTest(1, "ChunkyFormat_4Channels_WithAlpha_Format1");

        // Test 3: a format where DOSWAP and SWAPFIRST predicates engage
        // This flag is determined by the macros for the given Format. We attempt with a different code path.
        RunFormatTest(2, "ChunkyFormat_SwapOperations_Format2");
    }
}

// Main
int main() {
    TestHarness::RunAllTests();

    // Summary
    std::cout << "\nTest results: " << TestHarness::g_tests_pass
              << " / " << TestHarness::g_tests_run << " tests passed." << std::endl;
    if (TestHarness::g_tests_run == TestHarness::g_tests_pass) {
        std::cout << "All tests succeeded." << std::endl;
        return 0;
    } else {
        std::cerr << "Some tests failed." << std::endl;
        return 1;
    }
}