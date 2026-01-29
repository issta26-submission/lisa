// Test suite for the focal function sRGB_XYZ_roundtrip found in fast_float_testbed.c
// This test suite is designed for C++11 and does not rely on Google Test.
// It uses the LittleCMS API (lcms2.h) to perform the same color-transform operations
// as the focal function, and validates its behavior via two independent tests.
//
// Step alignment notes (embedded as comments):
// - Candidate Keywords: sRGB, XYZ, cmsCreate_sRGBProfile, cmsCreateXYZProfile, cmsCreateTransform,
//   cmsDoTransform, cmsDeleteTransform, cmsCloseProfile, TYPE_RGB_FLT, TYPE_XYZ_FLT,
//   INTENT_PERCEPTUAL
// - Domain knowledge: uses standard C++11, no gmock/gmock; uses a small stdout-capture test
//   to assert the printed delta values, and a direct numerical check with random samples.
// - Static scope functions in the focal file are not accessed; we exercise the exported API only.

#include <cstring>
#include <cstdio>
#include <fast_float_internal.h>
#include <functional>
#include <iostream>
#include <string>
#include <lcms2.h>
#include <sys/types.h>
#include <random>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <cmath>



extern "C" void sRGB_XYZ_roundtrip(void); // Focal function under test (C linkage)

/*
Helper: capture stdout during a function call.
We redirect stdout to a pipe, invoke the function, then read the produced output.
Note: This relies on POSIX pipes (works on Linux/macOS; non-POSIX platforms would require a different approach).
*/
static std::string capture_stdout(const std::function<void()> &fn) {
    int pipe_fd[2];
    if (pipe(pipe_fd) != 0) {
        return "";
    }

    // Save current stdout
    int saved_stdout = dup(fileno(stdout));
    if (saved_stdout == -1) {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return "";
    }

    // Redirect stdout to pipe
    if (dup2(pipe_fd[1], fileno(stdout)) == -1) {
        close(pipe_fd[0]); close(pipe_fd[1]);
        close(saved_stdout);
        return "";
    }
    close(pipe_fd[1]); // the write end is now stdout

    // Run the function whose output we want to capture
    fn();

    // Flush and restore stdout
    fflush(stdout);
    dup2(saved_stdout, fileno(stdout));
    close(saved_stdout);

    // Read from the pipe
    std::string out;
    char buffer[4096];
    ssize_t n;
    // The read loop will end when the producer closes the write end
    while ((n = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
        out.append(buffer, static_cast<size_t>(n));
    }
    close(pipe_fd[0]);
    return out;
}

/*
Test 1: Verify that sRGB_XYZ_roundtrip prints the expected header and
parses three finite numbers representing the per-channel maximum differences.
We also impose a conservative threshold to ensure the values are reasonable.
*/
static bool test_srgb_xyz_roundtrip_stdout() {
    std::cout << "Running test_srgb_xyz_roundtrip_stdout...\n";

    std::string output = capture_stdout([](){
        sRGB_XYZ_roundtrip();
    });

    const char* marker = "sRGB XYZ roundtrip differences:";
    size_t pos = output.find(marker);
    if (pos == std::string::npos) {
        std::cerr << "Test 1 Failure: stdout did not contain expected header '"
                  << marker << "'\nOutput:\n" << output << "\n";
        return false;
    }

    // Parse the three numeric values that follow the header
    const char* after = output.c_str() + pos + std::strlen(marker);
    float d0 = 0.0f, d1 = 0.0f, d2 = 0.0f;
    // The original printf emits: "sRGB XYZ roundtrip differences: %f %f %f\n"
    // Using a flexible sscanf to tolerate leading whitespace/newlines
    if (sscanf(after, " %f %f %f", &d0, &d1, &d2) != 3) {
        std::cerr << "Test 1 Failure: Failed to parse three differences from stdout.\n"
                  << "Output snippet:\n" << after << "\n";
        return false;
    }

    // Basic sanity checks
    if (!std::isfinite(d0) || !std::isfinite(d1) || !std::isfinite(d2)) {
        std::cerr << "Test 1 Failure: Non-finite difference values: "
                  << d0 << ", " << d1 << ", " << d2 << "\n";
        return false;
    }

    // Conservative threshold (values are expected to be very small for a color round-trip)
    const float MAX_DIFF = 1e-3f;
    bool ok = (d0 <= MAX_DIFF) && (d1 <= MAX_DIFF) && (d2 <= MAX_DIFF);
    if (!ok) {
        std::cerr << "Test 1 Failure: Differences too large: "
                  << d0 << ", " << d1 << ", " << d2
                  << " (threshold " << MAX_DIFF << ")\n";
    }

    return ok;
}

/*
Test 2: Numerical sanity check using a reasonably sized random sample.
We perform forward (sRGB -> XYZ) and reverse (XYZ -> sRGB) transforms on random RGB triples
and verify that the recovered input is very close to the original input.
This exercises the core transform path without requiring the full exhaustive space.
*/
static bool test_srgb_xyz_roundtrip_random_samples() {
    std::cout << "Running test_srgb_xyz_roundtrip_random_samples...\n";

    // Create profiles and transforms (same as the focal routine)
    cmsHPROFILE hsRGB = cmsCreate_sRGBProfile();
    cmsHPROFILE hXYZ = cmsCreateXYZProfile();
    if (!hsRGB || !hXYZ) {
        std::cerr << "Test 2 Failure: Could not create CMS profiles.\n";
        if (hsRGB) cmsCloseProfile(hsRGB);
        if (hXYZ) cmsCloseProfile(hXYZ);
        return false;
    }

    cmsHTRANSFORM hform_forward = cmsCreateTransform(hsRGB, TYPE_RGB_FLT, hXYZ, TYPE_XYZ_FLT, INTENT_PERCEPTUAL, 0);
    cmsHTRANSFORM hform_reverse = cmsCreateTransform(hXYZ, TYPE_XYZ_FLT, hsRGB, TYPE_RGB_FLT, INTENT_PERCEPTUAL, 0);

    bool ok = true;

    if (!hform_forward || !hform_reverse) {
        std::cerr << "Test 2 Failure: Failed to create CMS transforms.\n";
        ok = false;
    }

    if (ok) {
        // 2k random samples is a good balance between coverage and test time
        const int N = 2000;
        std::mt19937 rng(12345);
        std::uniform_real_distribution<float> dist(0.0f, 1.0f);

        float max_diff = 0.0f;

        for (int i = 0; i < N; ++i) {
            float in[3] = { dist(rng), dist(rng), dist(rng) }; // input RGB in [0,1]
            float xyz[3];
            float out[3];

            cmsDoTransform(hform_forward, in, xyz, 1);
            cmsDoTransform(hform_reverse, xyz, out, 1);

            for (int k = 0; k < 3; ++k) {
                float d = std::fabs(out[k] - in[k]);
                if (d > max_diff) max_diff = d;
                // Check that the transformed-out values remain in a reasonable range [−0.01, 1.01]
                if (out[k] < -0.01f || out[k] > 1.01f) {
                    std::cerr << "Test 2 Warning: Recovered RGB component out of expected range: "
                              << out[k] << " (component " << k << ", iteration " << i << ")\n";
                    // Not fatal; continue to evaluate overall error
                }
            }
        }

        // Cleanup resources before final assertion
        cmsDeleteTransform(hform_forward);
        cmsDeleteTransform(hform_reverse);
        cmsCloseProfile(hXYZ);
        cmsCloseProfile(hsRGB);

        // If max_diff is extremely small, the transform is effectively perfect within float precision.
        // A conservative threshold is used to avoid flakiness across platforms.
        const float THRESH = 1e-4f;
        if (!(max_diff <= THRESH)) {
            std::cerr << "Test 2 Failure: Maximum round-trip difference across samples = "
                      << max_diff << " (threshold " << THRESH << ")\n";
            ok = false;
        }
    } else {
        // Cleanup partial allocations if any
        if (hform_forward) cmsDeleteTransform(hform_forward);
        if (hform_reverse) cmsDeleteTransform(hform_reverse);
        cmsCloseProfile(hXYZ);
        cmsCloseProfile(hsRGB);
    }

    return ok;
}

int main() {
    bool all_ok = true;

    // Run tests in a lightweight, standalone manner (no GTest)
    if (!test_srgb_xyz_roundtrip_stdout()) {
        std::cerr << "ERROR: Test 1 (stdout parsing) failed.\n";
        all_ok = false;
    }
    if (!test_srgb_xyz_roundtrip_random_samples()) {
        std::cerr << "ERROR: Test 2 (random sample round-trip) failed.\n";
        all_ok = false;
    }

    if (all_ok) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << "SOME TESTS FAILED\n";
        return 1;
    }
}