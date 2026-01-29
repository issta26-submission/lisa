// cmsalpha_planar_increment_tests.cpp
//
// A lightweight C++11 test suite for the focal function:
//   cmsBool ComputeIncrementsForPlanar(cmsUInt32Number Format,
//                                        cmsUInt32Number BytesPerPlane,
//                                        cmsUInt32Number ComponentStartingOrder[], 
//                                        cmsUInt32Number ComponentPointerIncrements[])
//
// Approach:
// - Do not rely on any external test framework (GTest not used).
// - Use a minimal test harness that reports OK/FAIL for each test case.
// - Leverage the library-provided macros T_EXTRA, T_CHANNELS, T_DOSWAP, T_SWAPFIRST
//   and function trueBytesSize(Format) to derive expected behavior.
// - Implement a small reference routine in the test to reproduce the core
//   algorithm using the same macros, so we can compare the actual function's
//   outputs against a known-good calculation.
// - Verify important branches:
//     * total_chans <= 0 or total_chans >= cmsMAXCHANNELS -> FALSE
//     * extra channel increments are set to channelSize (trueBytesSize(Format))
//     * StartingOrder for extra channels matches the computed channel ordering
// - Use the Format parameter only through the provided macros to derive expected
//   values (so tests adapt to the actual Format encoding used by the library).
//
// Notes for integrators:
// - This test assumes the project provides the necessary cms types and macros
//   (cmsBool, cmsUInt32Number, cmsMAXCHANNELS, TRUE, FALSE, etc.) via
//   the included headers (e.g., lcms2.h). If your environment uses different
//   names for the TRUE/FALSE constants, adjust the comparison accordingly.
// - The test calls trueBytesSize(Format) to obtain channelSize, mirroring the
//   production path.
//

#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#include <lcms2_internal.h>
#include <lcms2.h>


// Include library headers to get type definitions and macros.
// This assumes the environment has Little CMS headers available.
extern "C" {
}

// Prototype of the focal function under test (C linkage)
extern "C" cmsBool ComputeIncrementsForPlanar(cmsUInt32Number Format,
                                           cmsUInt32Number BytesPerPlane,
                                           cmsUInt32Number ComponentStartingOrder[],
                                           cmsUInt32Number ComponentPointerIncrements[]);

// Extraneous function used in the production path (provided in headers as well).
// We declare it here to enable reference-based testing.
extern "C" cmsUInt32Number trueBytesSize(cmsUInt32Number Format);

// Reference implementation to reproduce the expected outputs for a given Format.
// It uses the same macros as the production code to ensure fidelity.
static void ComputeIncrementsForPlanar_Reference(cmsUInt32Number Format,
                                              cmsUInt32Number BytesPerPlane,
                                              cmsUInt32Number ComponentStartingOrder[],
                                              cmsUInt32Number ComponentPointerIncrements[])
{
    cmsUInt32Number channels[cmsMAXCHANNELS];
    cmsUInt32Number extra = T_EXTRA(Format);
    cmsUInt32Number nchannels = T_CHANNELS(Format);
    cmsUInt32Number total_chans = nchannels + extra;
    cmsUInt32Number i;
    cmsUInt32Number channelSize = trueBytesSize(Format);

    // Sanity check: mirror production path
    if (total_chans <= 0 || total_chans >= cmsMAXCHANNELS) {
        // Do nothing; caller checks validity via return value as well.
        return;
    }

    // Initialize local channel indices
    memset(channels, 0, sizeof(channels));

    // Separation for extra channels
    for (i = 0; i < extra; i++)
        ComponentPointerIncrements[i] = channelSize;

    // Build channel ordering
    for (i = 0; i < total_chans; i++) {
        if (T_DOSWAP(Format))
            channels[i] = total_chans - i - 1;
        else
            channels[i] = i;
    }

    // Optional swap first (rotate left)
    if (T_SWAPFIRST(Format) && total_chans > 0) {
        cmsUInt32Number tmp = channels[0];
        for (i = 0; i < total_chans - 1; i++)
            channels[i] = channels[i + 1];
        channels[total_chans - 1] = tmp;
    }

    // Scale by per-plane byte size
    for (i = 0; i < total_chans; i++) {
        channels[i] *= BytesPerPlane;
    }

    // Fill starting order for extra channels
    for (i = 0; i < extra; i++)
        ComponentStartingOrder[i] = channels[i + nchannels];
}

// Simple test harness
struct TestCase {
    std::string name;
    bool passed;
};

static bool test_Format_against_reference(cmsUInt32Number Format)
{
    const cmsUInt32Number BytesPerPlane = 1; // Use a simple per-plane size for determinism
    cmsUInt32Number ComponentStartingOrder[64];
    cmsUInt32Number ComponentPointerIncrements[64];

    // Initialize arrays to a sentinel value to detect unintended writes
    for (int i = 0; i < 64; ++i) {
        ComponentStartingOrder[i] = 0xFFFFFFFF;
        ComponentPointerIncrements[i] = 0xFFFFFFFF;
    }

    // Actual function call
    cmsBool rc = ComputeIncrementsForPlanar(Format, BytesPerPlane,
                                            ComponentStartingOrder, ComponentPointerIncrements);
    bool actualResult = (rc != 0);

    // Reference-based expectations
    cmsUInt32Number extra = T_EXTRA(Format);
    cmsUInt32Number nchannels = T_CHANNELS(Format);
    cmsUInt32Number total_chans = nchannels + extra;
    cmsUInt32Number channelSize = trueBytesSize(Format);
    bool expectedResult = (total_chans > 0 && total_chans < cmsMAXCHANNELS);

    // If the expected result is false, only verify that the function returned FALSE
    if (!expectedResult) {
        if (actualResult != false) {
            std::cerr << "Test failed: Format 0x" << std::hex << Format
                      << " expected FALSE return, got TRUE." << std::endl;
            return false;
        }
        return true;
    }

    // For formats where we expect a valid operation, verify the key invariants:
    // 1) Extra channel increments are set to channelSize
    // 2) StartingOrder for extra channels matches reference calculation
    // 3) The computed StartingOrder values are consistent with the reference

    // 1) Increments for extra channels
    for (cmsUInt32Number i = 0; i < extra; ++i) {
        if (ComponentPointerIncrements[i] != channelSize) {
            std::cerr << "Test failed: Format 0x" << std::hex << Format
                      << ", extra channel " << i
                      << " has incorrect ComponentPointerIncrements: expected "
                      << channelSize << ", got "
                      << ComponentPointerIncrements[i] << std::dec << std::endl;
            return false;
        }
    }

    // 2) StartingOrder for extra channels (compare with reference)
    cmsUInt32Number refStartingOrder[64];
    memset(refStartingOrder, 0xFF, sizeof(refStartingOrder));
    // Re-run the reference calculation to fill refStartingOrder
    ComputeIncrementsForPlanar_Reference(Format, 1, refStartingOrder, reinterpret_cast<cmsUInt32Number*>(nullptr));

    // The reference function above doesn't fill StartingOrder array here when providing nullptr;
    // Instead, compute the expected values directly into a local array using the same logic.
    // We'll implement a small inline computation here mirroring the reference logic.
    cmsUInt32Number channels[cmsMAXCHANNELS];
    cmsUInt32Number i;
    // Recompute for expected values
    // Note: extra, nchannels, total_chans, channelSize are already known.
    for (i = 0; i < total_chans; i++) {
        if (T_DOSWAP(Format)) {
            channels[i] = total_chans - i - 1;
        } else {
            channels[i] = i;
        }
    }
    if (T_SWAPFIRST(Format) && total_chans > 0) {
        cmsUInt32Number tmp = channels[0];
        for (i = 0; i < total_chans - 1; i++)
            channels[i] = channels[i + 1];
        channels[total_chans - 1] = tmp;
    }
    for (i = 0; i < total_chans; i++) {
        channels[i] *= channelSize;
    }

    // Compare the derived StartingOrder for extra channels with the actual results
    for (i = 0; i < extra; ++i) {
        cmsUInt32Number expected = channels[i + nchannels];
        // In production, StartingOrder is multiplied by BytesPerPlane;
        // We already applied channelSize = trueBytesSize(Format) to channels[], so
        // the expected value is already in bytes.
        if (ComponentStartingOrder[i] != expected) {
            std::cerr << "Test failed: Format 0x" << std::hex << Format
                      << ", extra channel " << i
                      << " StartingOrder mismatch: expected " << expected
                      << ", got " << ComponentStartingOrder[i] << std::dec << std::endl;
            return false;
        }
    }

    // All checks passed for this format
    return actualResult == true;
}

int main()
{
    std::vector<TestCase> results;

    // Candidate Format values to exercise the logic.
    // Note: These values rely on the library's encoding of formats via macros:
    // T_EXTRA(Format), T_CHANNELS(Format), T_DOSWAP(Format),
    // T_SWAPFIRST(Format), and trueBytesSize(Format).
    // You may need to adjust the list to reflect the actual formats supported by
    // your cmsalpha/cms2 installation.
    const cmsUInt32Number FormatsToTest[] = {
        0x00000000,     // Likely 0 channels (edge case: total_chans <= 0)
        0x00010000,     // Example: 1 channel with potential extra; adjust as needed
        0x00030001,     // Example: 3 channels with 1 extra; adjust as needed
        0x00FF00FF,     // Arbitrary large pattern
        0xFFFFFFFF,     // Max value to try triggering cmsMAXCHANNELS bound
        0x12345678,     // Random value to exercise various macro outcomes
    };

    int totalPassed = 0;
    int totalTests = sizeof(FormatsToTest) / sizeof(FormatsToTest[0]);

    for (size_t idx = 0; idx < totalTests; ++idx) {
        cmsUInt32Number fmt = FormatsToTest[idx];
        bool pass = test_Format_against_reference(fmt);
        TestCase tc;
        tc.name = "ComputeIncrementsForPlanar Format 0x" + std::to_string(fmt);
        tc.passed = pass;
        results.push_back(tc);
        if (pass) ++totalPassed;
    }

    // Report
    std::cout << "=====================================" << std::endl;
    std::cout << "CmsAlpha ComputeIncrementsForPlanar tests" << std::endl;
    std::cout << "Total: " << results.size() << ", Passed: " << totalPassed
              << ", Failed: " << (results.size() - totalPassed) << std::endl;

    for (const auto& r : results) {
        std::cout << (r.passed ? "[OK]  " : "[FAIL]") << " " << r.name << std::endl;
    }

    return (results.size() - totalPassed) == 0 ? 0 : 1;
}