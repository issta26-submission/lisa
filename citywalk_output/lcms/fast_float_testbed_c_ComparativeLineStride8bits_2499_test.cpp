// Lightweight C++11 unit test harness for the focal C function
// ComparativeLineStride8bits from fast_float_testbed.c
// The tests are designed to be non-terminating (continue on failure)
// and to verify basic observable behavior (stdout output) without
// depending on private/internal state.
//
// Notes:
// - The tests call the C function ComparativeLineStride8bits via extern "C".
// - stdout is captured to verify that key strings are produced.
// - This harness does not rely on GTest; it uses a tiny custom framework.

#include <cstdio>
#include <fast_float_internal.h>
#include <sys/stat.h>
#include <fstream>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>


// Forward declare the focal C function
extern "C" void ComparativeLineStride8bits(void);

// Simple non-terminating test harness
static int g_failures = 0;

#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[TEST FAILURE] " << (msg) << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
        ++g_failures; \
    } \
} while(0)

static bool contains(const std::string& s, const std::string& sub) {
    return s.find(sub) != std::string::npos;
}

// Capture stdout output of a function that takes no arguments.
// It redirects stdout to a temporary file, invokes the function, then restores stdout
static std::string captureOutputOf(void (*func)()) {
    const char* tmpFile = "cmss_output_capture.txt";

    // Flush and save current stdout
    fflush(stdout);
    int stdoutBak = dup(fileno(stdout));
    if (stdoutBak == -1) {
        return "";
    }

    // Redirect stdout to a temporary file
    if (freopen(tmpFile, "w", stdout) == NULL) {
        // Restore stdout if redirection failed
        dup2(stdoutBak, fileno(stdout));
        close(stdoutBak);
        return "";
    }

    // Invoke the focal function
    func();

    // Ensure all output is flushed
    fflush(stdout);

    // Restore original stdout
    dup2(stdoutBak, fileno(stdout));
    close(stdoutBak);

    // Read captured output
    std::ifstream in(tmpFile);
    std::stringstream buffer;
    buffer << in.rdbuf();
    in.close();

    // Clean up
    remove(tmpFile);
    return buffer.str();
}

/*
Test 1: Verify header and primary description prints.
Rationale:
- The focal function prints a header line containing "C O M P A R A T I V E cmsDoTransform() vs. cmsDoTransformLineStride()".
- Ensures that the function begins execution and prints the expected overview.
*/
static void test_header_prints() {
    std::string out = captureOutputOf(ComparativeLineStride8bits);

    ASSERT_TRUE(!out.empty(), "Output should not be empty after ComparativeLineStride8bits call.");
    ASSERT_TRUE(contains(out,
                       "C O M P A R A T I V E cmsDoTransform() vs. cmsDoTransformLineStride()"),
                "Header text missing in output of ComparativeLineStride8bits.");
}

/*
Test 2: Verify that profile file references appear in the output.
Rationale:
- The function calls ComparativeCt with various profile paths (e.g., test5.icc, test3.icc, test0.icc).
- Ensures that at least some of these file paths are present in the output.
*/
static void test_profiles_paths_present() {
    std::string out = captureOutputOf(ComparativeLineStride8bits);

    // Expect that at least one of the known profile filenames appears in the output
    bool hasTest5 = contains(out, "test5.icc");
    bool hasTest3 = contains(out, "test3.icc");
    bool hasTest0 = contains(out, "test0.icc");

    ASSERT_TRUE(hasTest5 || hasTest3 || hasTest0,
               "Profile file paths (test5.icc/test3.icc/test0.icc) should appear in output.");
}

/*
Test 3: Verify that a 'curves' entry is present in the output.
Rationale:
- One of the test cases uses inICC/outICC as NULL with label "curves".
- This checks that the code path with NULL profiles still prints meaningful information.
*/
static void test_curves_case_present() {
    std::string out = captureOutputOf(ComparativeLineStride8bits);

    ASSERT_TRUE(contains(out, "curves"),
                "Output should contain a 'curves' test case description.");
}

/*
Test 4: Execute multiple times to ensure stability and non-crash behavior.
Rationale:
- Running the focal function repeatedly should not crash or cause teardown issues.
- This helps exercise any static initialization/cleanup paths.
*/
static void test_multiple_calls_no_crash() {
    // First call (warm-up)
    ComparativeLineStride8bits();

    // Subsequent calls and capture
    std::string out = captureOutputOf(ComparativeLineStride8bits);

    ASSERT_TRUE(!out.empty(), "Output should not be empty on repeated ComparativeLineStride8bits call.");
}

/*
Test runner: executes all tests and reports summary.
*/
static void run_all_tests() {
    // Run individual tests
    test_header_prints();
    test_profiles_paths_present();
    test_curves_case_present();
    test_multiple_calls_no_crash();
}

int main() {
    // Run the lightweight test suite
    run_all_tests();

    // Summary
    if (g_failures == 0) {
        std::cout << "[ALL TESTS PASSED]" << std::endl;
        return 0;
    } else {
        std::cerr << "[TEST SUITE FAILED] " << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}