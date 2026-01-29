// Test suite for the focal method: CheckLabV2encoding (from testcms2.c)
// This test suite is implemented in C++11 without GTest, using a minimal
// home-grown assertion and test harness. It exercises CheckLabV2encoding
// and related dependency functions to maximize code coverage.
//
// Key dependencies and components (Candidate Keywords):
// - cmsLabEncoded2FloatV2: converts encoded Lab (16-bit values) to Lab structure.
// - cmsFloat2LabEncodedV2: converts Lab structure back to 16-bit encoded Lab values.
// - cmsCIELab Lab: Lab color representation used in conversion round-trips.
// - Inw/aw arrays: input/output 3-channel 16-bit vectors for Lab encoding.
// - Iteration over 0..65535 to validate full-range round-trip behavior.
// - Return value of CheckLabV2encoding indicates success (non-zero) or failure (zero).
//
// Notes:
// - This test relies on the external C functions declared in testcms2.h.
// - To compile with a C++11 compiler, wrap the C headers with extern "C" when including.

#include <chrono>
#include <iostream>
#include <testcms2.h>


extern "C" {
}

// Simple test harness state
static int g_failures = 0;

// Lightweight assertion/report helper
static void report(bool ok, const char* testName, const char* detail = nullptr) {
    if (ok) {
        std::cout << "[PASS] " << testName << "\n";
        if (detail) std::cout << "       " << detail << "\n";
    } else {
        std::cerr << "[FAIL] " << testName << "\n";
        if (detail) std::cerr << "       " << detail << "\n";
        ++g_failures;
    }
}

// Test 1: Full-range CheckLabV2encoding
// Purpose: Ensure that the focal method CheckLabV2encoding returns true
// for the entire 0..65535 input domain, indicating a perfect
// round-trip through cmsLabEncoded2FloatV2 and cmsFloat2LabEncodedV2.
// Coverage: exercises the nested loop, the comparison logic, and the
// conversion pathways used by the focal method.
static void test_CheckLabV2encoding_FullRange() {
    const char* name = "CheckLabV2encoding_FullRange";

    // Optional: measure execution time to guard against hangs
    auto t0 = std::chrono::high_resolution_clock::now();
    cmsInt32Number res = CheckLabV2encoding();
    auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = t1 - t0;

    bool ok = (res != 0);
    // Enforce a sane upper bound for sanity (adjustable)
    bool timeOk = (elapsed.count() < 60.0);

    if (!timeOk) {
        report(false, name, "Execution time exceeded 60 seconds (potential hang).");
    } else {
        report(ok && timeOk, name,
               elapsed.count() >= 0.0 ? "Completed within expected time." : nullptr);
    }
}

// Test 2: Round-trip sanity for core conversion helpers (indirectly supports the focal method)
// Purpose: Validate that the pair cmsLabEncoded2FloatV2 -> cmsFloat2LabEncodedV2
// round-trips a few representative boundary values. While this does not replace
// the focal method test, it improves confidence in the underlying conversion
// primitives used by CheckLabV2encoding and broadens coverage of related code.
static void test_roundtrip_SampleValues() {
    const char* name = "CheckLabV2encoding_roundtrip_sample_values";

    cmsUInt16Number Inw[3];
    cmsUInt16Number aw[3];
    cmsCIELab Lab;

    // Representative boundary and mid-range samples
    int samples[] = {0, 1, 32768, 65535};
    bool all_ok = true;

    for (int idx = 0; idx < 4; ++idx) {
        cmsUInt16Number j = (cmsUInt16Number) samples[idx];
        Inw[0] = Inw[1] = Inw[2] = j;

        cmsLabEncoded2FloatV2(&Lab, Inw);
        cmsFloat2LabEncodedV2(aw, &Lab);

        for (int i = 0; i < 3; ++i) {
            if (aw[i] != j) {
                all_ok = false;
                std::cerr << "[FAIL] " << name
                          << ": mismatch at sample " << j
                          << " channel " << i
                          << " got " << aw[i]
                          << " expected " << j << "\n";
            }
        }
    }

    report(all_ok, name);
}

// Main entry point: run the focused test(s) and report overall status.
// Per Domain Knowledge: "If gtest isn't allowed, call test methods from the main function."
int main(int argc, char* argv[]) {
    // Step 2: Build a minimal, focused test suite for CheckLabV2encoding
    // Step 3: Add tests for coverage and basic correctness of core
    // conversion pathways that feed into the focal method.

    std::cout << "Starting test suite for CheckLabV2encoding...\n";

    test_CheckLabV2encoding_FullRange();
    test_roundtrip_SampleValues();

    if (g_failures) {
        std::cerr << "Total failures: " << g_failures << "\n";
        return 1;
    }

    std::cout << "All tests passed.\n";
    return 0;
}