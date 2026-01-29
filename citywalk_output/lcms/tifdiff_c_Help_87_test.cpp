// A lightweight C++11 test suite for the Help() function in tifdiff.c
// Strategy: Override the C standard library exit function to prevent process termination,
// capture stderr output via a pipe, and verify the exact help text is printed.
// This approach keeps tests executable without modifying tifdiff.c.
//
// Notes:
// - The tests assume a POSIX-like environment (pipe, dup, dup2, read, etc.).
// - We link this test with tifdiff.c (compiled with a C compiler) and run the resulting binary.
// - We rely on the absence of private/private-like restrictions for accessing Help() via extern "C".
// - Tests are manually orchestrated (no GTest) and output PASS/FAIL per test.

#include <cstring>
#include <cstdio>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <sys/types.h>
#include <cstdlib>
#include <unistd.h>
#include <utils.h>
#include <tiffio.h>


// Declare the focal function from the C source as non-mangled C symbol.
extern "C" void Help();

// Global state to observe the overridden exit() behavior
static bool g_exit_called = false;
static int  g_exit_status = -1;

// Override the C library exit function to prevent actual termination.
// The override must have C linkage to override the symbol used by tifdiff.c.
extern "C" void exit(int status) {
    g_exit_called = true;
    g_exit_status = status;
    // Do not terminate the process; return to caller for test evaluation.
}

// Helper: capture what Help() writes to stderr.
// It redirects stderr to a pipe, invokes Help(), then reads the pipe content.
static std::string CaptureHelpOutput() {
    g_exit_called = false;
    g_exit_status = -1;

    int pipefd[2];
    if (pipe(pipefd) != 0) {
        return std::string("PIPE_CREATE_FAILED");
    }

    int saved_stderr = dup(STDERR_FILENO);
    if (saved_stderr == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return std::string("DUP_FAILED");
    }

    // Redirect stderr to the write end of the pipe
    if (dup2(pipefd[1], STDERR_FILENO) == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stderr);
        return std::string("REDIR_FAILED");
    }

    // We're done with the original write end in this process; close it.
    close(pipefd[1]);

    // Call the focal method
    Help();

    // Ensure all buffers are flushed
    fflush(stderr);

    // Read the captured output
    std::string output;
    char buffer[4096];
    ssize_t bytes;
    // Close the write end of the pipe in the reader side to signal EOF after writing
    while ((bytes = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, buffer + bytes);
    }

    // Restore stderr and close handles
    dup2(saved_stderr, STDERR_FILENO);
    close(saved_stderr);
    close(pipefd[0]);

    return output;
}

/*
Test 1: Help_PrintsExpectedContent
- Verifies that Help() prints the expected header, usage line, and option descriptions.
- Ensures static strings appear in the output, indicating correct formatting.
- Also asserts that Help() calls exit(0) (intercepted by our override).
*/
static bool TestHelp_PrintsExpectedContent() {
    std::string out = CaptureHelpOutput();

    bool ok = true;

    if (out.find("Little CMS TIFF compare utility. v1.1") == std::string::npos) {
        fprintf(stderr, "ERROR: Missing header line.\n");
        ok = false;
    }
    if (out.find("usage: tiffdiff [flags] input.tif output.tif") == std::string::npos) {
        fprintf(stderr, "ERROR: Missing usage line.\n");
        ok = false;
    }
    if (out.find("-o<tiff>   - Output TIFF file") == std::string::npos) {
        fprintf(stderr, "ERROR: Missing -o option line.\n");
        ok = false;
    }
    if (out.find("-g<CGATS>  - Output results in CGATS file") == std::string::npos) {
        fprintf(stderr, "ERROR: Missing -g option line.\n");
        ok = false;
    }
    if (out.find("-v - Verbose (show warnings)") == std::string::npos) {
        fprintf(stderr, "ERROR: Missing -v option line.\n");
        ok = false;
    }
    if (out.find("-h - This help") == std::string::npos) {
        fprintf(stderr, "ERROR: Missing -h option line.\n");
        ok = false;
    }

    if (!g_exit_called) {
        fprintf(stderr, "ERROR: exit(0) was not called as expected.\n");
        ok = false;
    } else if (g_exit_status != 0) {
        fprintf(stderr, "ERROR: exit called with non-zero status (%d).\n", g_exit_status);
        ok = false;
    }

    if (ok) {
        printf("PASSED: TestHelp_PrintsExpectedContent\n");
    } else {
        printf("FAILED: TestHelp_PrintsExpectedContent\n");
    }
    return ok;
}

/*
Test 2: Help_ExitStatusZero
- Verifies that the exit status captured from Help() is zero.
- This reinforces that the call path ends with exit(0) as intended.
*/
static bool TestHelp_ExitStatusZero() {
    // Re-run to ensure exit(0) is captured
    std::string out = CaptureHelpOutput();

    bool ok = (g_exit_called && g_exit_status == 0);

    if (ok) {
        printf("PASSED: TestHelp_ExitStatusZero\n");
    } else {
        printf("FAILED: TestHelp_ExitStatusZero\n");
        if (!g_exit_called) {
            fprintf(stderr, "Reason: exit() was not invoked by Help().\n");
        } else {
            fprintf(stderr, "Reason: exit() invoked with status %d (expected 0).\n", g_exit_status);
        }
    }

    // Optional: ensure some output was produced to aid debugging if needed
    (void)out; // suppress unused variable warning in release builds
    return ok;
}

/*
Step 3: Test Runner
- Runs both tests and prints a concise summary.
- This approach avoids external testing frameworks per constraints.
*/
int main() {
    printf("Starting unit tests for Help() in tifdiff.c\n");

    bool t1 = TestHelp_PrintsExpectedContent();
    bool t2 = TestHelp_ExitStatusZero();

    int total = 2;
    int passed = (t1 ? 1 : 0) + (t2 ? 1 : 0);
    printf("Summary: %d/%d tests passed\n", passed, total);

    return (passed == total) ? 0 : 1;
}