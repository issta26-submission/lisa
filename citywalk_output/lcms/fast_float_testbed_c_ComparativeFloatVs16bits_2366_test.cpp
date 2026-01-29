// Lightweight C++11 test harness for the focal method ComparativeFloatVs16bits
// This test suite targets fast_float_testbed.c's ComparativeFloatVs16bits()
// and its minimal class dependencies as exposed in the provided <FOCAL_CLASS_DEP> block.
// The aim is to validate that the function executes, produces output to stdout,
// and handles the NULL profile case without crashing.
// Note: This test avoids Google Test/GMock and uses simple, non-terminating assertions.

#include <sys/stat.h>
#include <fast_float_internal.h>
#include <fcntl.h>
#include <vector>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <algorithm>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>


#ifdef __linux__
#else
// Minimal stubs for non-POSIX systems; capturing stdout is not implemented.
#endif

// Step 1: Declare the focal function (C linkage) to be tested.
// The function resides in fast_float_testbed.c and is compiled together with
// the testbed sources. We declare it here so the C++ test harness can call it.
extern "C" void ComparativeFloatVs16bits(void);

// Utility macro for non-terminating assertions
#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "ASSERT FAILED: " << (msg) << " [" << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
        g_failures++; \
    } \
} while(0)

static int g_failures = 0;

// Helper: count occurrences of a substring within a string
static int count_occurrences(const std::string& s, const std::string& sub) {
    if (sub.empty()) return 0;
    int count = 0;
    std::size_t pos = 0;
    while ((pos = s.find(sub, pos)) != std::string::npos) {
        ++count;
        pos += sub.length();
    }
    return count;
}

// Step 3: Test harness utilities
// Capture stdout produced by a function call (POSIX path; Windows not supported here)
#ifndef _WIN32
static std::string capture_stdout(void (*func)()) {
    std::string output;

    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // Pipe creation failed; run function without capture
        func();
        return "[capture_failed]";
    }

    // Save current stdout
    int saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1) {
        // Fallback: run normally
        close(pipefd[0]);
        close(pipefd[1]);
        func();
        return "[capture_failed]";
    }

    // Redirect stdout to pipe
    if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
        // Fallback
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdout);
        func();
        return "[capture_failed]";
    }
    // Close the write end in this process; stdout now refers to pipe
    close(pipefd[1]);

    // Run the function
    func();

    // Ensure all output is flushed
    fflush(stdout);

    // Restore original stdout
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    // Read from the pipe
    const std::size_t BUF_SIZE = 4096;
    char buffer[BUF_SIZE];
    ssize_t n = 0;
    // Move to the pipe's read end
    while ((n = read(pipefd[0], buffer, BUF_SIZE)) > 0) {
        output.append(buffer, buffer + n);
    }
    // Cleanup
    close(pipefd[0]);
    return output;
}
#else
// Windows placeholder (basic behavior): run the function without stdout capture.
static std::string capture_stdout(void (* /*func*/ )()) {
    // Non-POSIX environments: stdout capture not implemented in this test.
    return "";
}
#endif

// Step 2: Unit Test Suite for ComparativeFloatVs16bits
// Candidate Keywords (core dependencies) extracted from the focal method
// - Comparative (function used to perform the comparison)
// - SpeedTestFloatByUsing16BitsRGB (speed test variant for 16-bit path)
// - SpeedTestFloatRGB (speed test variant for floating point path)
// - PROFILES_DIR (path prefix for ICC profiles)
// These keywords help reason about the test intent and coverage.

static void test_header_and_basic_output_present() {
    // Test that the function prints the header and at least one comparison line.
    std::string output = capture_stdout(reinterpret_cast<void (*)()>(&ComparativeFloatVs16bits));

    // Look for the header title line emitted by the trace calls in ComparativeFloatVs16bits
    const std::string header_line = "C O M P A R A T I V E  converting to 16 bit vs. using float plug-in.";
    bool has_header = output.find(header_line) != std::string::npos;

    // Look for at least one of the "Floating point ..." lines to confirm a comparison was invoked
    const std::string first_comparison = "Floating point on CLUT profiles  ";
    bool has_comparison = output.find(first_comparison) != std::string::npos;

    ASSERT_TRUE(has_header, "Header line for ComparativeFloatVs16bits was not printed");
    ASSERT_TRUE(has_comparison, "First comparison line not found in output");
    // Do not stop execution on failure; aggregate results via g_failures
}

static void test_null_profiles_handling() {
    // Specifically exercise the final call where NULL in/out profiles are passed
    std::string output = capture_stdout(reinterpret_cast<void (*)()>(&ComparativeFloatVs16bits));

    // Expect a line corresponding to the curves call which passes NULLs
    const std::string curves_line = "Floating point on curves         ";
    bool has_curves = output.find(curves_line) != std::string::npos;

    ASSERT_TRUE(has_curves, "NULL profile path (curves) case not exercised in output");
}

static void test_multiple_invocations_do_not_crash_and_produce_output() {
    // Call the focal method twice and verify that we see header lines in both outputs
    std::string out1 = capture_stdout(reinterpret_cast<void (*)()>(&ComparativeFloatVs16bits));
    std::string out2 = capture_stdout(reinterpret_cast<void (*)()>(&ComparativeFloatVs16bits));

    const std::string header_line = "C O M P A R A T I V E  converting to 16 bit vs. using float plug-in.";

    int count1 = count_occurrences(out1, header_line);
    int count2 = count_occurrences(out2, header_line);

    // Each invocation should emit the header at least once
    ASSERT_TRUE(count1 >= 1, "First invocation did not produce header output");
    ASSERT_TRUE(count2 >= 1, "Second invocation did not produce header output");

    // Optional: ensure that repeated invocations do not crash and produce outputs
    ASSERT_TRUE((count1 + count2) >= 2, "Total header occurrences across two runs is unexpectedly low");
}

// Runner
int main() {
    std::cout << "Running tests for ComparativeFloatVs16bits (fast_float_testbed.c) under C++11 test harness" << std::endl;

    test_header_and_basic_output_present();
    test_null_profiles_handling();
    test_multiple_invocations_do_not_crash_and_produce_output();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}

/*
Notes for future extension:
- If more granular verification of internal flows becomes necessary, consider adding a lightweight
  wrapper around Comparative to log its arguments; however, given the constraints (no GTest/GMock),
  and since the focal method relies on external free functions (not easily mockable), stdout-based
  verification provides a robust, portable approach for this scenario.
- The tests intentionally avoid private members and focus on observable behavior (output), which aligns
  with the provided domain knowledge constraints and static analysis considerations.
*/