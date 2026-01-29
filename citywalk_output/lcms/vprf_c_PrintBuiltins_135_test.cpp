/*
Unit test suite for the focal method: PrintBuiltins (located in vprf.c)

Approach:
- Capture the exact output sent to stderr when PrintBuiltins() is invoked.
- Compare the captured output against the expected literal text defined by the function.
- Use a lightweight C++11 test harness (no Google Test) with non-terminating assertions.
- Export the C function PrintBuiltins with C linkage for the test binary.

Notes:
- The function PrintBuiltins prints to stderr a fixed block describing "Built-in profiles".
- There are no conditional branches inside PrintBuiltins; thus, there are no true/false branches to cover within this function itself.
- This test focuses solely on correctness of the printed text and stream destination (stderr).
- The test assumes a POSIX-like environment (pipe, dup2, read, etc.). If run on non-POSIX platforms, adaptations may be needed.

Compile hint (example):
g++ -std=c++11 -I<path_to_headers> -L<path_to_obj_or_lib> -lvprf_test -o test_printbuiltins test_printbuiltins.cpp

The test calls PrintBuiltins through C linkage to ensure correct symbol resolution when linked with vprf.c.
*/

#include <cstring>
#include <cstdio>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <utils.h>


extern "C" void PrintBuiltins(void); // Ensure C linkage for the focal function

// Lightweight test framework (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define EXPECT_EQ_STR(expected, actual, test_name) \
    do { \
        ++g_tests_run; \
        if ((actual) == (expected)) { \
            ++g_tests_passed; \
            std::cout << "[PASS] " << test_name << std::endl; \
        } else { \
            ++g_tests_failed; \
            std::cout << "[FAIL] " << test_name << std::endl; \
            std::cout << "  Expected: [" << expected << "]" << std::endl; \
            std::cout << "  Actual  : [" << actual << "]" << std::endl; \
        } \
    } while (0)

// Capture the exact output of PrintBuiltins() to stderr.
// Returns true on success, false on failure to capture.
static bool capture_PrintBuiltinsOutput(std::string &output)
{
    int pipe_fd[2];
    if (pipe(pipe_fd) != 0) {
        perror("pipe");
        return false;
    }

    // Save original stderr
    int saved_stderr = dup(STDERR_FILENO);
    if (saved_stderr == -1) {
        perror("dup");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return false;
    }

    // Redirect stderr to the pipe's write end
    if (dup2(pipe_fd[1], STDERR_FILENO) == -1) {
        perror("dup2");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        close(saved_stderr);
        return false;
    }

    // Close the unused end as it's now duplicated
    close(pipe_fd[1]);

    // Invoke the focal function
    PrintBuiltins();

    // Flush and restore stderr
    fflush(stderr);
    if (dup2(saved_stderr, STDERR_FILENO) == -1) {
        perror("dup2 restore");
        close(pipe_fd[0]);
        close(saved_stderr);
        return false;
    }
    close(saved_stderr);

    // Read the captured output from the read end
    const size_t BUF_SIZE = 4096;
    char buf[BUF_SIZE];
    std::string result;
    ssize_t bytes;
    // Non-blocking read loop until pipe closes (PrintBuiltins should close after printing)
    while ((bytes = read(pipe_fd[0], buf, BUF_SIZE)) > 0) {
        result.append(buf, static_cast<size_t>(bytes));
    }
    close(pipe_fd[0]);

    output = result;
    return true;
}

/*
Test 1: Validate that PrintBuiltins prints the exact built-in profile list to stderr.

Explanation:
- Expected output is the exact string produced by the function, as defined by its two fprintf calls.
- The test captures the actual output and compares with the expected literal.
- Uses non-terminating assertion semantics: do not exit on mismatch; report status and continue.
*/
static void test_PrintBuiltins_PrintsExactText()
{
    std::string actual_output;
    bool ok = capture_PrintBuiltinsOutput(actual_output);
    if (!ok) {
        std::cout << "[ERROR] Failed to capture PrintBuiltins output" << std::endl;
        ++g_tests_run;
        ++g_tests_failed;
        return;
    }

    // Build the expected literal exactly as PrintBuiltins emits it.
    // Note: The string ends with two newline characters as in the source (\n\n).
    const char* expected_c = "\nBuilt-in profiles:\n\n"
                             "\t*Lab2  -- D50-based v2 CIEL*a*b\n"
                             "\t*Lab4  -- D50-based v4 CIEL*a*b\n"
                             "\t*Lab   -- D50-based v4 CIEL*a*b\n"
                             "\t*XYZ   -- CIE XYZ (PCS)\n"
                             "\t*sRGB  -- sRGB color space\n"
                             "\t*Gray22 - Monochrome of Gamma 2.2\n"
                             "\t*Gray30 - Monochrome of Gamma 3.0\n"
                             "\t*null   - Monochrome black for all input\n"
                             "\t*Lin2222- CMYK linearization of gamma 2.2 on each channel\n"
                             "\n";
    std::string expected(expected_c);

    // Use the non-terminating assertion style
    EXPECT_EQ_STR(expected, actual_output, "PrintBuiltins_PrintsExactText");
}

/*
Step 3: Test runner
- Executes the test(s) and prints a final summary.
- Returns non-zero if any test failed (for CI usage).
*/
int main()
{
    // Run tests
    test_PrintBuiltins_PrintsExactText();

    // Summary
    std::cout << "Tests run: " << g_tests_run
              << ", Passed: " << g_tests_passed
              << ", Failed: " << g_tests_failed
              << std::endl;

    // Return non-zero if there were failures to signal issues in CI systems
    return (g_tests_failed == 0) ? 0 : 1;
}