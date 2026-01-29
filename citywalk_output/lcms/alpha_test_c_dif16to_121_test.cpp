/*
Step 0 (Contextual Overview)
This test suite targets the focal method: dif16to(void) and its class dependencies
as provided in the excerpt. The method relies on two external functions
to_premul16_float and to_premul16, and prints diagnostic messages via printf.
To create a focused, executable unit test suite without using GTest, we will:

- Provide a small, self-contained C++11 test harness that invokes the C-style
  function dif16to and captures its stdout output.
- Verify that the expected header line ("Premul TO diff") and the final
  diagnostic line ("Error max=... on pre:... pre1:...") are emitted.
- Do not modify the production code. We rely on the existing (possibly stubbed)
  implementations of to_premul16_float and to_premul16 provided with the focal
  class dependencies.

Important notes:
- The test harness uses POSIX pipes to capture print output from printf.
- We declare the focal function and its dependencies with extern "C" to ensure
  correct linkage when building with a C++11 test host.
- This approach avoids requiring GTest and adheres to the constraints in the
  domain knowledge section. It provides comments explaining the purpose of each
  test for maintainability and coverage reasoning.

Step 1: Candidate Keywords extracted from the focal method (for test design)
- Premul TO diff, diff computation, 16-bit premultiplication
- to_premul16_float (gpremul), to_premul16 (gpremul1)
- Maximum difference tracking: max, max1, max2
- Loop structure: a ∈ [0, 65535] step 255, g ∈ [0, 65535]
- Diagnostic output: "Error max=%d on pre:%d pre1:%d"

Step 2/3: Test Suite (C++11, no GTest) with explanatory comments
- Tests:
  1) test_dif16to_outputs_expected_header_and_summary
     - Captures stdout from calling dif16to and asserts that the header line
       "Premul TO diff" and the summary line starting with "Error max=" are present.
     - This ensures the function reaches its print statements and the basic
       reporting path is exercised, regardless of the exact numeric values produced
       by the (stubbed) to_premul16_float and to_premul16 implementations.
  2) test_dif16to_consistency_of_output_format
     - Invokes the focal function twice and asserts that both outputs contain
       the same key header line, ensuring consistent formatting across runs
       (given the same inputs). This helps detect accidental changes to print
       behavior and maintains coverage of the printf call sites.

- The tests are implemented in a single C++11 source file suitable for compilation
  together with the provided alpha_test.c (which contains the focal method and
  its dependencies). The tests do not depend on private members and respect
  domain knowledge constraints (no GMock, only standard facilities, and simple
  main-based test execution).

Code:
*/

// Candidate Keywords and test rationale are described above in comments.
// The code below implements the tests described.

#include <cstring>
#include <cstdio>
#include <sys/stat.h>
#include <math.h>
#include <fcntl.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <string>
#include <stdint.h>
#include <sys/types.h>
#include <lcms2_internal.h>
#include <unistd.h>


// Step 2/3: External C function declarations (link against production code in alpha_test.c)
extern "C" {
    // Focal method under test
    void dif16to(void);

    // Dependencies (produced in the same compilation unit as production code)
    // We declare them as extern to rely on the production implementations.
    uint16_t to_premul16_float(uint16_t rgb16, uint16_t a16);
    uint16_t to_premul16(uint16_t rgb16, uint16_t a16);
}

// Helper: Capture stdout output produced by calling a C function
std::string capture_dif16to_output() {
    // Create a pipe to capture stdout
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return std::string();
    }

    // Save current stdout
    int saved_stdout = dup(fileno(stdout));
    if (saved_stdout == -1) {
        perror("dup stdout");
        close(pipefd[0]);
        close(pipefd[1]);
        return std::string();
    }

    // Redirect stdout to pipe's write end
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        perror("dup2");
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdout);
        return std::string();
    }

    // Close the write end in the parent, as it's now dup-ed to stdout
    close(pipefd[1]);

    // Call the focal method
    dif16to();

    // Restore stdout
    fflush(stdout);
    if (dup2(saved_stdout, fileno(stdout)) == -1) {
        perror("restore stdout");
        close(pipefd[0]);
        close(saved_stdout);
        return std::string();
    }
    close(saved_stdout);

    // Read captured output
    std::string output;
    char buffer[4096];
    ssize_t n;
    // Read from the read end
    while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, static_cast<size_t>(n));
    }
    close(pipefd[0]);
    return output;
}

// Simple assertion helper
bool assert_true(bool condition, const char* message) {
    if (!condition) {
        std::cerr << "ASSERTION FAILED: " << message << std::endl;
        return false;
    }
    return true;
}

// Test 1: Verify header line and final error summary line exist in output
bool test_dif16to_outputs_expected_header_and_summary() {
    std::string out = capture_dif16to_output();
    // Debug: you can uncomment to see captured output during local runs
    // std::cerr << "Captured output:\n" << out << "\n";

    bool hasHeader = (out.find("Premul TO diff") != std::string::npos);
    bool hasErrorLine = (out.find("Error max=") != std::string::npos);

    bool ok = true;
    ok &= assert_true(hasHeader, "Output should contain header line 'Premul TO diff'.");
    ok &= assert_true(hasErrorLine, "Output should contain summary line starting with 'Error max='.");
    return ok;
}

// Test 2: Consistency check of output format across runs
bool test_dif16to_consistency_of_output_format() {
    std::string out1 = capture_dif16to_output();
    std::string out2 = capture_dif16to_output();

    bool header1 = (out1.find("Premul TO diff") != std::string::npos);
    bool header2 = (out2.find("Premul TO diff") != std::string::npos);

    // Basic consistency: both runs should print the header if the function executes
    bool ok = true;
    ok &= assert_true(header1, "First run should contain header line 'Premul TO diff'.");
    ok &= assert_true(header2, "Second run should contain header line 'Premul TO diff'.");
    return ok;
}

int main() {
    // Step 3: Run tests and report results
    bool all_passed = true;

    std::cout << "Running test: test_dif16to_outputs_expected_header_and_summary" << std::endl;
    if (!test_dif16to_outputs_expected_header_and_summary()) {
        all_passed = false;
        std::cout << "Test FAILED: test_dif16to_outputs_expected_header_and_summary" << std::endl;
    } else {
        std::cout << "Test PASSED: test_dif16to_outputs_expected_header_and_summary" << std::endl;
    }

    std::cout << "Running test: test_dif16to_consistency_of_output_format" << std::endl;
    if (!test_dif16to_consistency_of_output_format()) {
        all_passed = false;
        std::cout << "Test FAILED: test_dif16to_consistency_of_output_format" << std::endl;
    } else {
        std::cout << "Test PASSED: test_dif16to_consistency_of_output_format" << std::endl;
    }

    if (!all_passed) {
        std::cerr << "Some tests failed. See output above for details." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "All tests passed." << std::endl;
    return EXIT_SUCCESS;
}