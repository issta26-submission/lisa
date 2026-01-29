// C++11 unit test suite for PixelTypeFromChanCount (vprf.c)
// This test suite avoids GTest and uses a lightweight, non-terminating assertion approach.
// It targets the mapping from color channel count to pixel type constants (PT_*).
// The test relies on the presence of vprf.h which defines PixelTypeFromChanCount and PT_*
// constants. If your environment provides a different header, adjust the include accordingly.

#include <vector>
#include <iostream>
#include <string>
#include <vprf.h>
#include <utils.h>


// Include the focal header that exposes PixelTypeFromChanCount and PT_* constants.
// If your project uses a different header path, modify accordingly.

// Note: We assume that the FatalError function used inside PixelTypeFromChanCount is a
// no-op in the test environment (as suggested by the provided source snippet).
// Therefore tests focus on the correctness of the returned PixelType values.

// Helper: non-terminating assertion log
static void logFail(const std::string& testName, const std::string& message) {
    std::cerr << "TEST FAIL: " << testName << " - " << message << "\n";
}

// Test 1: Validate that ColorChannels 1..15 map to the expected PT_* constants.
// This covers all valid branches of the switch in PixelTypeFromChanCount.
static bool test_valid_channel_mappings() {
    // Expected mapping for color channels 1..15 using PT_* constants.
    // The 0th index is unused (we start from 1 for clarity).
    static const int expected[16] = {
        0,                  // placeholder for index 0
        PT_GRAY,            // 1
        PT_MCH2,             // 2
        PT_MCH3,             // 3
        PT_CMYK,             // 4
        PT_MCH5,             // 5
        PT_MCH6,             // 6
        PT_MCH7,             // 7
        PT_MCH8,             // 8
        PT_MCH9,             // 9
        PT_MCH10,            // 10
        PT_MCH11,            // 11
        PT_MCH12,            // 12
        PT_MCH13,            // 13
        PT_MCH14,            // 14
        PT_MCH15             // 15
    };

    bool all_ok = true;
    for (int ch = 1; ch <= 15; ++ch) {
        int got = PixelTypeFromChanCount(ch);
        int exp = expected[ch];
        if (got != exp) {
            logFail("test_valid_channel_mappings", "ColorChannels=" + std::to_string(ch) +
                    " expected " + std::to_string(exp) +
                    " but got " + std::to_string(got));
            all_ok = false;
        }
    }
    return all_ok;
}

// Test 2: Validate default/edge-case handling for invalid channel counts.
// For values outside 1..15, the function should hit the default branch.
static bool test_invalid_channel_defaults() {
    bool ok = true;

    int r0 = PixelTypeFromChanCount(0);
    if (r0 != -1) {
        logFail("test_invalid_channel_defaults", "Expected -1 for ColorChannels=0, got " + std::to_string(r0));
        ok = false;
    }

    int r16 = PixelTypeFromChanCount(16);
    if (r16 != -1) {
        logFail("test_invalid_channel_defaults", "Expected -1 for ColorChannels=16, got " + std::to_string(r16));
        ok = false;
    }

    return ok;
}

// Test 3: Validate behavior for a negative channel count input.
// Negative values are outside the valid range and should map to the default case.
static bool test_negative_channel_input() {
    int r = PixelTypeFromChanCount(-1);
    if (r != -1) {
        logFail("test_negative_channel_input", "Expected -1 for ColorChannels=-1, got " + std::to_string(r));
        return false;
    }
    return true;
}

// Main test runner
int main() {
    std::vector<std::string> testNames;
    int failures = 0;

    // Run Test 1
    if (test_valid_channel_mappings()) {
        testNames.push_back("test_valid_channel_mappings: PASSED");
    } else {
        testNames.push_back("test_valid_channel_mappings: FAILED");
        ++failures;
    }

    // Run Test 2
    if (test_invalid_channel_defaults()) {
        testNames.push_back("test_invalid_channel_defaults: PASSED");
    } else {
        testNames.push_back("test_invalid_channel_defaults: FAILED");
        ++failures;
    }

    // Run Test 3
    if (test_negative_channel_input()) {
        testNames.push_back("test_negative_channel_input: PASSED");
    } else {
        testNames.push_back("test_negative_channel_input: FAILED");
        ++failures;
    }

    // Output test summary
    std::cout << "\nTest Summary:\n";
    for (const auto& line : testNames) {
        std::cout << " - " << line << "\n";
    }
    std::cout << "Total failures: " << failures << "\n";

    return failures;
}