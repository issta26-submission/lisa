// C++11 test suite for cmsInt32Number checkSingleComputeIncrements in fast_float_testbed.c
// This test suite uses the wrap technique to mock the internal helper _cmsComputeComponentIncrements.
// To build and run:
//   g++ -std=c++11 -Wall -Wextra -O2 -Wl,--wrap=_cmsComputeComponentIncrements -o cms_check_test cms_check_test.cpp
// Then run: ./cms_check_test
// Note: The wrapper above relies on linker option -Wl,--wrap=_cmsComputeComponentIncrements.
// The wrapper implements deterministic scenarios to exercise true and false branches of checkSingleComputeIncrements.

#include <cstring>
#include <cstdio>
#include <fast_float_internal.h>
#include <iostream>
#include <cstdarg>
#include <cstdlib>
#include <memory.h>
#include <stdlib.h>
#include <cstdint>


// Typedefs matching the expected CMS types used by the focal method
typedef unsigned int cmsUInt32Number;
typedef int cmsInt32Number;

// The focal function under test (C linkage)
extern "C" cmsInt32Number checkSingleComputeIncrements(cmsUInt32Number Format,
                                                    cmsUInt32Number planeStride,
                                                    cmsUInt32Number ExpectedChannels,
                                                    cmsUInt32Number ExpectedAlpha,
                                                    ...);

// The wrapper for _cmsComputeComponentIncrements (to be used with linker wrap)
#define cmsMAXCHANNELS 16  // Used for wrapper array sizing; actual size is ignored in function prototype

extern "C" void __wrap__cmsComputeComponentIncrements(cmsUInt32Number Format,
                                                   cmsUInt32Number planeStride,
                                                   cmsUInt32Number* nChannels,
                                                   cmsUInt32Number* nAlpha,
                                                   cmsUInt32Number ComponentStartingOrder[cmsMAXCHANNELS],
                                                   cmsUInt32Number ComponentPointerIncrements[cmsMAXCHANNELS]);

// Global to select test case in the wrapper
enum TestCaseID {
    T_MATCH1 = 0,
    T_MISMATCH_CHANNELS,
    T_MISMATCH_ALPHA,
    T_MISMATCH_ORDER,
    T_MISMATCH_INCR,
    T_MATCH2,
    T_TOTAL
};

static int g_currentTest = -1;
static void SetTestCase(TestCaseID id) { g_currentTest = static_cast<int>(id); }

// Simple assertion helper
static void fail(const char* msg) {
    std::cerr << "TEST FAILED: " << msg << std::endl;
}

// Test helper to print summary
static int g_total = 0;
static int g_failed = 0;
static void record_pass() { ++g_total; }
static void record_fail() { ++g_total; ++g_failed; }

// Implementation of the wrapper to control _cmsComputeComponentIncrements output
extern "C" void __wrap__cmsComputeComponentIncrements(cmsUInt32Number Format,
                                                   cmsUInt32Number planeStride,
                                                   cmsUInt32Number* nChannels,
                                                   cmsUInt32Number* nAlpha,
                                                   cmsUInt32Number ComponentStartingOrder[cmsMAXCHANNELS],
                                                   cmsUInt32Number ComponentPointerIncrements[cmsMAXCHANNELS])
{
    // Choose deterministic behavior per test case
    switch (g_currentTest) {
        case T_MATCH1:
            // Expect 2 channels, 1 alpha, total 3 components
            *nChannels = 2;
            *nAlpha = 1;
            // Starting order for 3 components
            ComponentStartingOrder[0] = 0;
            ComponentStartingOrder[1] = 1;
            ComponentStartingOrder[2] = 2;
            // Increments for each component
            ComponentPointerIncrements[0] = 4;
            ComponentPointerIncrements[1] = 8;
            ComponentPointerIncrements[2] = 16;
            break;

        case T_MISMATCH_CHANNELS:
            // Simulate mismatch: actual channels = 3, alpha = 1
            *nChannels = 3;
            *nAlpha = 1;
            ComponentStartingOrder[0] = 0;
            ComponentStartingOrder[1] = 1;
            ComponentStartingOrder[2] = 2;
            ComponentPointerIncrements[0] = 4;
            ComponentPointerIncrements[1] = 8;
            ComponentPointerIncrements[2] = 16;
            break;

        case T_MISMATCH_ALPHA:
            // Simulate mismatch in alpha: actual channels = 2, alpha = 2
            *nChannels = 2;
            *nAlpha = 2;
            ComponentStartingOrder[0] = 0;
            ComponentStartingOrder[1] = 1;
            ComponentStartingOrder[2] = 2; // padding
            ComponentPointerIncrements[0] = 4;
            ComponentPointerIncrements[1] = 8;
            ComponentPointerIncrements[2] = 16;
            break;

        case T_MISMATCH_ORDER:
            // Correct counts but wrong starting order
            *nChannels = 2;
            *nAlpha = 1;
            ComponentStartingOrder[0] = 0;
            ComponentStartingOrder[1] = 2; // swapped order at position 1
            ComponentStartingOrder[2] = 1;
            ComponentPointerIncrements[0] = 4;
            ComponentPointerIncrements[1] = 8;
            ComponentPointerIncrements[2] = 16;
            break;

        case T_MISMATCH_INCR:
            // Correct counts and order but mismatch in increments last value
            *nChannels = 2;
            *nAlpha = 1;
            ComponentStartingOrder[0] = 0;
            ComponentStartingOrder[1] = 1;
            ComponentStartingOrder[2] = 2;
            ComponentPointerIncrements[0] = 4;
            ComponentPointerIncrements[1] = 8;
            ComponentPointerIncrements[2] = 17; // mismatch
            break;

        case T_MATCH2:
            // Another valid scenario: 1 channel, 0 alpha
            *nChannels = 1;
            *nAlpha = 0;
            ComponentStartingOrder[0] = 0;
            ComponentPointerIncrements[0] = 8;
            break;

        default:
            // Default safe values
            *nChannels = 0;
            *nAlpha = 0;
            break;
    }
}

// Simple test runner
int main() {
    std::cout << "Starting checkSingleComputeIncrements test suite (no GTest, using custom main).\n";

    // Test 1: Positive path - formats and counts align, sequences match
    SetTestCase(T_MATCH1);
    {
        cmsUInt32Number Format = 0;
        cmsUInt32Number planeStride = 0;
        cmsUInt32Number ExpectedChannels = 2;
        cmsUInt32Number ExpectedAlpha = 1;
        int rc = checkSingleComputeIncrements(Format, planeStride, ExpectedChannels, ExpectedAlpha,
                                              // StartingOrder: 0,1,2
                                              0, 1, 2,
                                              // Increments: 4,8,16
                                              4, 8, 16);
        if (rc == 1) {
            record_pass();
            std::cout << "PASS: Test 1 (matching channels/alpha and sequence) returned 1 as expected.\n";
        } else {
            record_fail();
            fail("Test 1 expected rc==1 but got rc==0");
        }
    }

    // Test 2: Negative path - mismatch channels
    SetTestCase(T_MISMATCH_CHANNELS);
    {
        cmsUInt32Number Format = 0;
        cmsUInt32Number planeStride = 0;
        cmsUInt32Number ExpectedChannels = 2; // will be mismatched by wrapper
        cmsUInt32Number ExpectedAlpha = 1;
        int rc = checkSingleComputeIncrements(Format, planeStride, ExpectedChannels, ExpectedAlpha);
        if (rc == 0) {
            record_pass();
            std::cout << "PASS: Test 2 (mismatched channels) returned 0 as expected.\n";
        } else {
            record_fail();
            fail("Test 2 expected rc==0 but got rc==1");
        }
    }

    // Test 3: Negative path - mismatch alpha
    SetTestCase(T_MISMATCH_ALPHA);
    {
        cmsUInt32Number Format = 0;
        cmsUInt32Number planeStride = 0;
        cmsUInt32Number ExpectedChannels = 2;
        cmsUInt32Number ExpectedAlpha = 1; // wrapper will produce nAlpha=2
        int rc = checkSingleComputeIncrements(Format, planeStride, ExpectedChannels, ExpectedAlpha,
                                              // Provide some args, though function should exit early
                                              0, 1, 2,
                                              4, 8, 16);
        if (rc == 0) {
            record_pass();
            std::cout << "PASS: Test 3 (mismatched alpha) returned 0 as expected.\n";
        } else {
            record_fail();
            fail("Test 3 expected rc==0 but got rc==1");
        }
    }

    // Test 4: Negative path - mismatched starting order
    SetTestCase(T_MISMATCH_ORDER);
    {
        cmsUInt32Number Format = 0;
        cmsUInt32Number planeStride = 0;
        cmsUInt32Number ExpectedChannels = 2;
        cmsUInt32Number ExpectedAlpha = 1;
        int rc = checkSingleComputeIncrements(Format, planeStride, ExpectedChannels, ExpectedAlpha,
                                              // StartingOrder intentionally mismatched
                                              0, 1, 2,
                                              4, 8, 16);
        if (rc == 0) {
            record_pass();
            std::cout << "PASS: Test 4 (mismatched starting order) returned 0 as expected.\n";
        } else {
            record_fail();
            fail("Test 4 expected rc==0 but got rc==1");
        }
    }

    // Test 5: Negative path - mismatched increments
    SetTestCase(T_MISMATCH_INCR);
    {
        cmsUInt32Number Format = 0;
        cmsUInt32Number planeStride = 0;
        cmsUInt32Number ExpectedChannels = 2;
        cmsUInt32Number ExpectedAlpha = 1;
        int rc = checkSingleComputeIncrements(Format, planeStride, ExpectedChannels, ExpectedAlpha,
                                              // StartingOrder correct
                                              0, 1, 2,
                                              // Increments mismatched at last value
                                              4, 8, 17);
        if (rc == 0) {
            record_pass();
            std::cout << "PASS: Test 5 (mismatched increments) returned 0 as expected.\n";
        } else {
            record_fail();
            fail("Test 5 expected rc==0 but got rc==1");
        }
    }

    // Test 6: Positive path - another valid scenario (single channel, no alpha)
    SetTestCase(T_MATCH2);
    {
        cmsUInt32Number Format = 0;
        cmsUInt32Number planeStride = 0;
        cmsUInt32Number ExpectedChannels = 1;
        cmsUInt32Number ExpectedAlpha = 0;
        int rc = checkSingleComputeIncrements(Format, planeStride, ExpectedChannels, ExpectedAlpha,
                                              // StartingOrder: 0
                                              0,
                                              // Increments: 8
                                              8);
        if (rc == 1) {
            record_pass();
            std::cout << "PASS: Test 6 (single channel, no alpha) returned 1 as expected.\n";
        } else {
            record_fail();
            fail("Test 6 expected rc==1 but got rc==0");
        }
    }

    // Summary
    std::cout << "Test results: total=" << g_total << ", passed=" << (g_total - g_failed)
              << ", failed=" << g_failed << "\n";

    // Return non-zero if any test failed
    return g_failed != 0 ? 1 : 0;
}