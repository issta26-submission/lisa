/*
 * Unit test suite for the focal method:
 *   void testPrintUsage(const char *zArgs)
 * from the provided C code (lsmtest_util.c).
 *
 * Approach:
 * - We treat testPrintUsage as a black-box that prints:
 *     "Usage: <g.argv[0]> <g.argv[1]> <zArgs>\n"
 *   by delegating the actual printing to testPrintError (provided in the C file).
 * - We capture stdout and stderr (in case testPrintError prints to either) using a POSIX pipe.
 * - We verify:
 *     a) Output starts with "Usage: "
 *     b) Output ends with " <zArgs>\n" (i.e., the final argument matches zArgs)
 * - We avoid depending on the exact values of g.argv[0] and g.argv[1], only asserting on the suffix.
 * - Non-terminating assertions are implemented via a lightweight EXPECT_TRUE macro-like helper.
 * - The tests are designed to be compiled in a C++11 environment without GoogleTest (GTest not used).
 *
 * Notes:
 * - The test framework assumes the runtime environment provides a valid g.argv[0] and g.argv[1]
 *   as used by testPrintUsage, and that testPrintError prints to stdout or stderr.
 * - This test is written to be portable on POSIX systems (Linux/macOS) where pipe/dup2/read are available.
 * - The test calls the focal function directly (extern "C" linkage) so no modification to the production code is required.
 */

#include <string.h>
#include <lsmtest.h>
#include <vector>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <string>
#include <unistd.h>
#include <iostream>


// Declaration of the focal function from the C file.
// Use C linkage to match the symbol in the C file.
extern "C" void testPrintUsage(const char *zArgs);

/* Lightweight test failure counter and message printer.
 * We use non-terminating assertions: tests will print a message and continue. */
static int g_failures = 0;

static void EXPECT_TRUE(bool condition, const std::string &message) {
    if (!condition) {
        std::cerr << "TEST FAILURE: " << message << std::endl;
        ++g_failures;
    }
}

/* Capture the output produced by testPrintUsage(zArgs).
 * Redirect both stdout and stderr to a pipe to avoid missing prints
 * if testPrintError writes to either stream. */
static std::string captureOutput(const char *zArgs) {
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        std::cerr << "Failed to create pipe for capturing output." << std::endl;
        return "";
    }

    int stdout_fd = dup(fileno(stdout));
    int stderr_fd = dup(fileno(stderr));
    if (stdout_fd == -1 || stderr_fd == -1) {
        std::cerr << "Failed to duplicate stdout/stderr." << std::endl;
        return "";
    }

    // Redirect stdout and stderr to the write end of the pipe
    dup2(pipefd[1], fileno(stdout));
    dup2(pipefd[1], fileno(stderr));
    // We no longer need the original write end in this process
    close(pipefd[1]);

    // Invoke the focal function which will write to stdout/stderr
    testPrintUsage(zArgs);

    // Ensure all output is flushed
    fflush(stdout);
    fflush(stderr);

    // Restore original stdout/stderr
    dup2(stdout_fd, fileno(stdout));
    dup2(stderr_fd, fileno(stderr));
    close(stdout_fd);
    close(stderr_fd);

    // Read all data from the read end
    std::string output;
    char buffer[1024];
    ssize_t n;
    // After restoring, the read end may still have data, so read until EOF
    while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, static_cast<size_t>(n));
    }
    close(pipefd[0]);

    return output;
}

/* Test case 1: Basic usage with a simple argument.
 * Expects the produced string to start with "Usage: "
 * and end with " <zArgs>\n" where zArgs == "ARG1". */
static void testPrintUsage_BasicArg() {
    const char *zArgs = "ARG1";

    std::string out = captureOutput(zArgs);

    // Existence and prefix check
    EXPECT_TRUE(!out.empty(), "Output should not be empty for ARG1.");
    EXPECT_TRUE(out.find("Usage: ") == 0, "Output should start with 'Usage: ' for ARG1.");

    // Suffix check: the last part should be " <zArgs>\n"
    std::string expectedSuffix = std::string(" ") + zArgs + "\n";
    bool endsWithSuffix = false;
    if (out.size() >= expectedSuffix.size()) {
        endsWithSuffix = (out.compare(out.size() - expectedSuffix.size(),
                                    expectedSuffix.size(), expectedSuffix) == 0);
    }
    EXPECT_TRUE(endsWithSuffix,
                "Output should end with ' " + std::string(zArgs) + "\\n'.");
}

/* Test case 2: Usage with a multi-word argument to ensure proper formatting.
 * Expects the produced string to start with "Usage: "
 * and end with " <zArgs>\n" where zArgs == "ARG WITH SPACES". */
static void testPrintUsage_MultiWordArg() {
    const char *zArgs = "ARG WITH SPACES";

    std::string out = captureOutput(zArgs);

    // Existence and prefix check
    EXPECT_TRUE(!out.empty(), "Output should not be empty for multi-word ARG.");
    EXPECT_TRUE(out.find("Usage: ") == 0, "Output should start with 'Usage: ' for multi-word ARG.");

    // Suffix check
    std::string expectedSuffix = std::string(" ") + zArgs + "\n";
    bool endsWithSuffix = false;
    if (out.size() >= expectedSuffix.size()) {
        endsWithSuffix = (out.compare(out.size() - expectedSuffix.size(),
                                    expectedSuffix.size(), expectedSuffix) == 0);
    }
    EXPECT_TRUE(endsWithSuffix,
                "Output should end with ' " + std::string(zArgs) + "\\n'.");
}

/* Run all tests and report final result. */
int main() {
    // Run test cases
    testPrintUsage_BasicArg();
    testPrintUsage_MultiWordArg();

    // Final result: non-zero exit code if any test failed
    if (g_failures != 0) {
        std::cerr << "Total test failures: " << g_failures << std::endl;
        return 1;
    }
    return 0;
}