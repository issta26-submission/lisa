// PixelSize Unit Test Suite (C++11)
// This test targets the focal method PixelSize defined in cmsxform.c.
// It relies on the T_BYTES macro and cmsUInt32Number / cmsUInt64Number types
// from the project's headers to determine the expected behavior.
// The test does not terminate on failure and reports a summary at the end.

#include <vector>
#include <lcms2_internal.h>
#include <iostream>


// Import the necessary C headers/types and declare the focal function.
// We wrap the include in extern "C" to ensure C linkage in the C++ test.
extern "C" {
}

// Declare the focal function with C linkage for linking against cmsxform.c
extern "C" cmsUInt32Number PixelSize(cmsUInt32Number Format);

int main() {
    // Simple, non-terminating test harness
    int totalTests = 0;
    int failedTests = 0;

    // Local helper to run a single test for a given Format value.
    auto runTestForFormat = [&](cmsUInt32Number Format) {
        ++totalTests;
        // Expected result logic mirrors PixelSize:
        // If T_BYTES(Format) == 0, expect sizeof(cmsUInt64Number); otherwise expect T_BYTES(Format).
        cmsUInt32Number tb = T_BYTES(Format);
        cmsUInt32Number expected = (tb == 0) ? sizeof(cmsUInt64Number) : tb;
        cmsUInt32Number actual = PixelSize(Format);
        if (actual != expected) {
            std::cerr << "PixelSize(CMS Format 0x" 
                      << std::hex << Format << std::dec 
                      << ") => expected " << expected
                      << ", got " << actual << "\n";
            ++failedTests;
        } else {
            std::cout << "PixelSize test passed for Format 0x"
                      << std::hex << Format << std::dec
                      << " (value " << actual << ").\n";
        }
    };

    // Step 2: Candidate Formats
    // Include a mix of values to exercise both branches if possible.
    // We rely on T_BYTES macro to decide which branch is taken inside PixelSize.
    std::vector<cmsUInt32Number> candidateFormats = {
        0x0,        // Likely to exercise the "zero bytes" branch in some builds
        0x1,        // Small non-zero value
        0x2,        // Another small non-zero value
        0x3,        // Yet another small non-zero
        0x100,      // Medium value
        0x2000,     // Larger value
        0x12345678  // Arbitrary large value to exercise macro parsing
    };

    // Run tests for all candidates
    for (const auto& fmt : candidateFormats) {
        runTestForFormat(fmt);
    }

    // Summary
    if (failedTests == 0) {
        std::cout << "All PixelSize tests passed. Total: " 
                  << totalTests << " tests.\n";
    } else {
        std::cerr << failedTests << " PixelSize tests FAILED out of " 
                  << totalTests << ".\n";
    }

    // Non-terminating assertion style: return non-zero on failure to indicate issues.
    return failedTests;
}