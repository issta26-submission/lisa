// This test suite targets the focal method Help() found in wtpt.c.
// It validates that Help prints the expected usage information to stderr
// and then exits. Because Help() calls exit(0), the test runs Help in a
// separate child process and captures stderr via a pipe.
//
// The tests are written for C++11 without GTest. A small, non-terminating
// assertion mechanism is used to accumulate failures and report a final
// summary. The test harness uses POSIX facilities (fork, pipe, dup2) to
// isolate the test from the main process.
//
// Step 1: Candidate Keywords (derived from the focal method's behavior and dependencies)
// - Help, fprintf, stderr
// - LCMS_VERSION macro and LittleCMS reference
// - usage: wtpt, flags:, l - CIE Lab, c - CIE LCh, x - Don't show XYZ
// - HTML/URL references: http://www.littlecms.com
// - info@littlecms.com
// - exit(0) / _exit(0) to terminate the process
// - External linkage: the C function Help() to be invoked from C++

// Step 2 & 3: Unit Test Generation and Refinement
// - Test Help by executing in a forked child process and capturing stderr output.
// - Validate presence of key substrings in the captured output.
// - Ensure the child exits with status 0 and that the captured text includes
//   the expected help/usage content.

#include <cstring>
#include <cstdio>
#include <vector>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <utils.h>


// Linkage to the focal C function Help() in wtpt.c
extern "C" void Help(void);

// Simple non-terminating assertion macro for test logging
#define EXPECT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "[TEST FAILURE] " << (msg) << "\n"; \
            ++g_failures; \
        } \
    } while (0)

static int g_failures = 0;

// Helper: Run Help() in a separate process and capture its stderr output.
// Returns true if the child was created and finished, and fills 'output'
// with the captured text and 'exitStatus' with the child's exit code.
static bool RunHelpAndCaptureOutput(std::string& output, int& exitStatus) {
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        std::perror("pipe");
        return false;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return false;
    }

    if (pid == 0) {
        // Child process: redirect stderr to pipe and invoke Help()
        // Ensure the write end of the pipe is used for stderr
        if (dup2(pipefd[1], STDERR_FILENO) == -1) {
            std::perror("dup2");
            _exit(1);
        }
        // Close unused ends
        close(pipefd[0]);
        close(pipefd[1]);
        // Call the focal method; it will exit(0) on success
        Help();
        // If Help() returns for any reason, exit explicitly
        _exit(0);
    } else {
        // Parent process: read from pipe and wait for child
        close(pipefd[1]); // Close write end in parent
        char buffer[4096];
        std::string tmp;
        ssize_t n;
        // Read all data from the pipe
        while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            tmp.append(buffer, buffer + n);
        }
        close(pipefd[0]);
        int status = 0;
        if (waitpid(pid, &status, 0) == -1) {
            std::perror("waitpid");
            return false;
        }
        exitStatus = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
        output = tmp;
        return true;
    }
}

// Test 1: Ensure Help prints the expected usage/help text and exits with code 0
static bool TestHelpOutputsExpectedText() {
    std::string captured;
    int exitCode = -1;
    bool ok = RunHelpAndCaptureOutput(captured, exitCode);
    if (!ok) {
        std::cerr << "[TEST ERROR] Failed to run Help() in a separate process.\n";
        g_failures++;
        return false;
    }

    // Expected exit code
    bool exitedCleanly = (exitCode == 0);
    EXPECT_TRUE(exitedCleanly, "Help() should exit with code 0");

    // Substrings we expect to appear in the help text.
    // We use a fairly inclusive list to guard against small changes in formatting.
    std::vector<std::string> required = {
        "little CMS ICC white point utility",
        "wtpt",
        "usage: wtpt",
        "flags:",
        "l - CIE Lab",
        "CIE Lab",
        "CIE LCh",
        "Don't show XYZ",
        "info@littlecms.com",
        "http:" // the link form in the file
    };

    bool all_present = true;
    for (const auto& substr : required) {
        if (captured.find(substr) == std::string::npos) {
            all_present = false;
            std::cerr << "[TEST WARNING] Missing expected substring: \"" << substr << "\"\n";
        }
    }

    EXPECT_TRUE(all_present, "Help() output should contain all key usage/help substrings");
    return (exitedCleanly && all_present);
}

// Additional test: Ensure Help is non-crashing and that output is non-empty when invoked.
// This complements TestHelpOutputsExpectedText by validating that some content is printed.
static bool TestHelpOutputNonEmpty() {
    std::string captured;
    int exitCode = -1;
    bool ok = RunHelpAndCaptureOutput(captured, exitCode);
    if (!ok) {
        std::cerr << "[TEST ERROR] Failed to run Help() for non-empty output test.\n";
        g_failures++;
        return false;
    }

    bool nonEmpty = !captured.empty();
    EXPECT_TRUE(nonEmpty, "Help() should produce non-empty output on stderr");
    bool exitIsZero = (exitCode == 0);
    EXPECT_TRUE(exitIsZero, "Help() should exit with code 0 for non-empty test");
    return nonEmpty && exitIsZero;
}

int main() {
    // Run tests
    bool t1 = TestHelpOutputsExpectedText();
    bool t2 = TestHelpOutputNonEmpty();

    // Summary
    if (g_failures == 0) {
        std::cout << "[TEST SUITE] All tests passed.\n";
        return 0;
    } else {
        std::cerr << "[TEST SUITE] " << g_failures << " test(s) failed.\n";
        return 1;
    }
}