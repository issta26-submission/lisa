// This test suite targets the focal method:
 // print_one(const char *leader, double err)
 // which prints the string when err != -1.0 using the format:
 // " %s %.5f\n"
 // The leading space is part of the output, e.g. " L 0.12345\n"
 //
 // Test plan (mapped to {Candidate Keywords} from Step 1):
 // - Dependency: printf/stdout I/O (capturing stdout)
 // - Behavior: branch when err == -1.0 (no output)
 // - Behavior: branch when err != -1.0 (formatted output)
 // - Variation: different leader strings (to verify formatting with various lengths)
 // - Static-like behavior: none needed here, but we treat the function as a callable C symbol
 //
 // Domain knowledge notes (Step 3):
 // - Use POSIX facilities to capture stdout (pipe + dup2)
 // - Implement a minimal, non-terminating test harness (no GTest)
 // - Tests are invoked from main
 // - Only standard library + provided method (print_one) are used

#include <unistd.h>
#include <cassert>
#include <signal.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <float.h>
#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <cstdio>
#include <sys/types.h>
#include <stdio.h>
#include <cstring>
#include <math.h>


// Declaration of the focal C function to be tested.
// It is defined in pngvalid.c (C linkage).
extern "C" void print_one(const char *leader, double err);

#ifdef _WIN32
  #error This test harness targets POSIX environment for stdout capture (pipe/dup2).
#endif

// Capture stdout produced by print_one by temporarily redirecting stdout to a pipe.
static std::string capture_print(const char* leader, double err)
{
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // If pipe creation fails, return empty string to indicate error.
        return "";
    }

    // Save current stdout
    int saved_stdout = dup(fileno(stdout));
    if (saved_stdout == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Redirect stdout to the write end of the pipe
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdout);
        return "";
    }

    // Close the unused write end in the original process after redirect
    close(pipefd[1]);

    // Call the focal function
    print_one(leader, err);

    // Ensure all output is flushed
    fflush(stdout);

    // Restore original stdout
    if (dup2(saved_stdout, fileno(stdout)) == -1) {
        // In case of failure, still attempt to read what was captured so far
        // Cleanup
        close(saved_stdout);
        close(pipefd[0]);
        return "";
    }
    close(saved_stdout);

    // Read all data from the read end of the pipe
    std::string output;
    char buffer[256];
    ssize_t n;
    // Close read end after reading to EOF
    while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, static_cast<size_t>(n));
    }
    close(pipefd[0]);
    return output;
}

// Test 1: When err == -1.0, there should be no output.
static bool test_print_one_does_not_print_when_minus_one()
{
    // Act
    std::string out = capture_print("L", -1.0);

    // Assert (non-terminating): check that nothing was printed
    if (!out.empty()) {
        std::cerr << "Test failed: expected no output when err == -1.0, got: \""
                  << out << "\"\n";
        return false;
    }

    // Pass
    return true;
}

// Test 2: When err has a normal value, verify the exact formatted output.
// Expected output: " L <value_with_5_decimals>\n"
static bool test_print_one_prints_formatted_value()
{
    const char* leader = "L";
    double value = 0.12345;
    std::string out = capture_print(leader, value);
    std::string expected = " L 0.12345\n";

    if (out != expected) {
        std::cerr << "Test failed: expected output \"" << expected
                  << "\" but got \"" << out << "\"\n";
        return false;
    }
    return true;
}

// Test 3: Validate formatting with a longer leader string.
static bool test_print_one_handles_long_leader()
{
    const char* leader = "LEADER123";
    double value = 2.5;
    std::string out = capture_print(leader, value);
    std::string expected = " LEADER123 2.50000\n";

    if (out != expected) {
        std::cerr << "Test failed: expected output \"" << expected
                  << "\" but got \"" << out << "\"\n";
        return false;
    }
    return true;
}

// Test 4: Extreme but valid value to ensure proper rounding of 5 decimals.
static bool test_print_one_rounding_behavior()
{
    const char* leader = "X";
    // This value should format to 1.23046 when rounded to 5 decimals
    double value = 1.230455; // rounds to 1.23046
    std::string out = capture_print(leader, value);
    std::string expected = " X 1.23046\n";

    if (out != expected) {
        std::cerr << "Test failed: expected output \"" << expected
                  << "\" but got \"" << out << "\"\n";
        return false;
    }
    return true;
}

// Helper to run all tests and report summary
static int run_all_tests()
{
    int passed = 0;
    int failed = 0;

    if (test_print_one_does_not_print_when_minus_one()) {
        ++passed;
    } else {
        ++failed;
    }

    if (test_print_one_prints_formatted_value()) {
        ++passed;
    } else {
        ++failed;
    }

    if (test_print_one_handles_long_leader()) {
        ++passed;
    } else {
        ++failed;
    }

    if (test_print_one_rounding_behavior()) {
        ++passed;
    } else {
        ++failed;
    }

    std::cerr << "Unit tests completed. Passed: " << passed
              << ", Failed: " << failed << std::endl;

    return failed ? 1 : 0;
}

// Main entry point: Step 3 requires invoking tests from main
int main()
{
    return run_all_tests();
}