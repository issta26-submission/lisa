// Unit test suite for the focal method PrintFloatResults (transicc.c) using C++11
// This test harness assumes the following external symbols are provided by the
// project under test (linked together when building the test executable):
// - extern "C" void PrintFloatResults(cmsFloat64Number Value[]);
// - extern "C" cmsUInt32Number cmsChannelsOfColorSpace(cmsColorSpaceSignature space);
// - extern "C" void cmsNamedColorInfo(void* Colorant, cmsUInt32Number Index, char* ChannelName, void*, void*, void*, void*);
// - externs for the global configuration/state used by PrintFloatResults:
//     extern "C" cmsColorSpaceSignature OutputColorSpace;
//     extern "C" void* OutputColorant;
//     extern "C" cmsFloat64Number OutputRange;
//     extern "C" cmsBool lQuantize;
//     extern "C" cmsBool lUnbounded;
//     extern "C" int Verbose;
//
// The tests focus on exercising PrintFloatResults under controlled conditions, without
// requiring the full library to be executed. We rely on the real implementations
// of cmsChannelsOfColorSpace and other project components to exist in the linked
// object files. We only manipulate the public/global state to drive different branches.
//
// NOTE: We avoid using any testing framework (per instructions). Instead, we implement
// a lightweight assertion mechanism and a small harness that captures stdout
// produced by PrintFloatResults for precise comparison.
//
// The tests below assume a 3-channel color space (e.g., RGB). If the real project
// returns a different channel count, adjust the expected strings accordingly.

#include <cstring>
#include <cstdio>
#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <unistd.h>
#include <utils.h>
#include <sstream>


// Basic type aliases to match project types (placeholders for linking)
extern "C" {
    typedef unsigned int cmsUInt32Number;
    typedef unsigned short cmsUInt16Number;
    typedef double cmsFloat64Number;
    typedef int cmsBool;
    typedef unsigned long long cmsColorSpaceSignature;
}

// Declarations of focal method and dependencies (defined in the project under test)
extern "C" void PrintFloatResults(cmsFloat64Number Value[]);
extern "C" cmsUInt32Number cmsChannelsOfColorSpace(cmsColorSpaceSignature space);
extern "C" void cmsNamedColorInfo(void* Colorant, cmsUInt32Number Index, char* ChannelName,
                                void*, void*, void*, void*);

// Globals used by PrintFloatResults (externs; defined in the production code)
extern "C" cmsColorSpaceSignature OutputColorSpace;
extern "C" void* OutputColorant;
extern "C" cmsFloat64Number OutputRange;
extern "C" cmsBool lQuantize;
extern "C" cmsBool lUnbounded;
extern "C" int Verbose;

// Utility: capture stdout output from a callable to a string
static std::string captureOutput(const std::function<void()> &func) {
    // Create a pipe to capture stdout
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // If pipe creation fails, fall back to direct call (shouldn't happen in tests)
        func();
        return "";
    }

    // Save current stdout
    int savedStdout = dup(fileno(stdout));
    // Redirect stdout to pipe write end
    dup2(pipefd[1], fileno(stdout));
    // Close unused write end in this thread
    close(pipefd[1]);

    // Execute the function that prints to stdout
    func();

    // Restore stdout
    fflush(stdout);
    dup2(savedStdout, fileno(stdout));
    close(savedStdout);

    // Read from pipe
    std::string output;
    char buffer[256];
    ssize_t bytesRead;
    // Close read end of pipe eventually; read until EOF
    while ((bytesRead = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, static_cast<size_t>(bytesRead));
    }
    close(pipefd[0]);
    return output;
}

// Lightweight test harness
static int totalTests = 0;
static int failedTests = 0;

static void logFail(const std::string &testName, const std::string &expected, const std::string &actual) {
    std::cerr << "[FAIL] " << testName << "\n";
    std::cerr << "  Expected: \"" << expected << "\"\n";
    std::cerr << "  Actual  : \"" << actual << "\"\n";
    ++failedTests;
}

// Simple assertion that does not abort the program on failure
static void assertEquals(const std::string &testName, const std::string &expected, const std::string &actual) {
    ++totalTests;
    if (expected != actual) {
        logFail(testName, expected, actual);
    } else {
        std::cerr << "[PASS] " << testName << "\n";
    }
}

// Helper to run a test case expecting a specific 3-channel output
static void test_basic_no_quant_no_clamp_verbose0() {
    // Scenario:
    // - 3 channels expected (RGB)
    // - OutputColorant = NULL to exercise default channel naming (Channel #i)
    // - OutputRange = 1.0
    // - lQuantize = false
    // - lUnbounded = false (clamping enabled)
    // - Verbose = 0 (no channel names, numeric output)
    extern "C" { OutputColorant = NULL; }
    OutputRange = 1.0;
    lQuantize = 0;
    lUnbounded = 0;
    Verbose = 0;

    cmsFloat64Number Value[3] = {0.5, 1.2, -0.1}; // expected: 0.5000 1.0000 0.0000
    std::string out = captureOutput([&]{ PrintFloatResults(Value); });

    std::string expected = "0.5000 1.0000 0.0000 \n";
    assertEquals("basic_no_quant_no_clamp_verbose0", expected, out);
}

// Test that Verbose > 0 prints channel labels
static void test_verbose_mode_prints_channel_names() {
    extern "C" { OutputColorant = NULL; }
    OutputRange = 1.0;
    lQuantize = 0;
    lUnbounded = 0;
    Verbose = 1; // verbose prints "Channel #i=value"
    cmsFloat64Number Value[3] = {0.5, 1.0, 0.0}; // expected: Channel #1=0.5000 Channel #2=1.0000 Channel #3=0.0000
    std::string out = captureOutput([&]{ PrintFloatResults(Value); });

    std::string expected = "Channel #1=0.5000 Channel #2=1.0000 Channel #3=0.0000 \n";
    assertEquals("verbose_mode_prints_channel_names", expected, out);
}

// Test quantization path: lQuantize = true
static void test_quantization_true() {
    extern "C" { OutputColorant = NULL; }
    OutputRange = 1.0;
    lQuantize = 1;
    lUnbounded = 1; // unbounded disabled is not used when quantization is on, but keep simple
    Verbose = 0;

    cmsFloat64Number Value[3] = {0.6, 0.0, 0.0}; // quantizes 0.6 -> floor(0.6+0.5)=1
    std::string out = captureOutput([&]{ PrintFloatResults(Value); });

    std::string expected = "1.0000 0.0000 0.0000 \n";
    assertEquals("quantization_true", expected, out);
}

// Test clamping path: values outside [0, OutputRange] get clamped when lUnbounded is false
static void test_clamping_within_range() {
    extern "C" { OutputColorant = NULL; }
    OutputRange = 1.0;
    lQuantize = 0;
    lUnbounded = 0; // ensure clamping is active
    Verbose = 0;

    cmsFloat64Number Value[3] = {1.5, -0.2, 0.5}; // should clamp to 1.0, 0.0, 0.5
    std::string out = captureOutput([&]{ PrintFloatResults(Value); });

    std::string expected = "1.0000 0.0000 0.5000 \n";
    assertEquals("clamping_within_range", expected, out);
}

// Entry point: run all tests
int main() {
    std::cerr << "Starting PrintFloatResults unit tests...\n";

    test_basic_no_quant_no_clamp_verbose0();
    test_verbose_mode_prints_channel_names();
    test_quantization_true();
    test_clamping_within_range();

    std::cerr << "Tests completed. " << totalTests << " run, "
              << failedTests << " failed.\n";

    // Return non-zero if any test failed
    return (failedTests != 0) ? 1 : 0;
}