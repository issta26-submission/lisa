// TimePNG unit test suite for the focal method usage(FILE *fp)
// This test suite is self-contained and uses POSIX primitives (fork/exec, pipe).
// It does not rely on GTest. It runs the focal function in a separate process
// to avoid exiting the test runner via exit(99) inside usage().
//
// Key design notes (mapped to the instructions' steps):
// - Step 1 (Program Understanding): The focal function usage(FILE *fp) closes a non-NULL
//   file pointer if provided, then prints a detailed usage/help message to stderr
//   and terminates the process with exit(99). The test suite targets the behavior
//   of that function, including both the NULL and non-NULL fp branches.
// - Step 2 (Unit Test Generation): Tests cover true/false paths by launching a separate
//   child process that executes usage via a small CLI wrapper (--child-usage). The
//   wrapper path re-executes the same binary in child mode and invokes usage with either
//   NULL or an opened temporary FILE*. Both child executions have their stderr redirected
//   to a pipe so the parent can verify the output text and exit code.
// - Step 3 (Test Case Refinement): The tests are designed to maximize coverage of the usage()
//   behavior with minimal surface area, focusing on the critical condition (fp != NULL) and
//   the exact output contract. Assertions are non-terminating (using simple checks) and the
//   tests are invoked from main().
// - All code is contained in a single file for simplicity and compilation under C++11.

#include <unistd.h>
#include <vector>
#include <string.h>
#include <time.h>
#include <string>
#include <limits.h>
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <cstring>


// Candidate Keywords extracted from the focal method and its dependencies
// - usage, fprintf, fclose, exit
// - PNG transforms and image-read paths (e.g., --by-image, png_read_png, png_read_row)
// - assembly/disassembly terminology (assembly, --assemble, --dissemble)
// - File IO helpers (tmpfile, FILE*, NULL)
// - stderr, Filesystem error messages, program usage semantics

// Forward declaration of the focal method to be tested.
// We implement a C-like usage() function with C linkage to mirror the provided code.
extern "C" void usage(FILE *fp);

// Global program path (set in main) for child exec wrapper.
static const char *g_program_path = nullptr;

// A tiny assertion helper to keep test code readable.
#define TEST_ASSERT(condition, message)                          \
    do {                                                         \
        if (!(condition)) {                                      \
            std::cerr << "ASSERTION FAILED: " << (message)       \
                      << " (in " << __FILE__ << ":" << __LINE__ \
                      << ")\n";                                 \
            tests_failed = true;                               \
        }                                                        \
    } while (0)

// Capture result of a child process that runs the usage() function.
// The child is executed via an execl call with arguments:
//   --child-usage <mode>
// where mode is "null" (usage(NULL)) or "fp" (usage(tmpfile())).
bool run_child_usage(bool with_fp, std::string &output, int &exit_code) {
    int pipe_fd[2];
    if (pipe(pipe_fd) != 0) {
        std::cerr << "Failed to create pipe for IPC\n";
        return false;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "fork() failed\n";
        return false;
    }

    if (pid == 0) {
        // Child process
        // Redirect child stderr to write end of the pipe
        dup2(pipe_fd[1], STDERR_FILENO);
        // Close unused ends
        close(pipe_fd[0]);
        close(pipe_fd[1]);

        // Exec a new instance of this program in child-usage mode
        const char *mode = with_fp ? "fp" : "null";
        // argv[0] is the program path (set by the test harness in main)
        execl(g_program_path, g_program_path, "--child-usage", mode, (char*)nullptr);
        // If exec fails, exit with error code
        _exit(2);
    } else {
        // Parent process
        close(pipe_fd[1]); // Close write end; we'll read from read end

        // Read all data from the pipe (stderr) into a string
        char buffer[4096];
        ssize_t n;
        std::string collected;
        while ((n = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
            collected.append(buffer, static_cast<size_t>(n));
        }
        close(pipe_fd[0]);

        // Wait for child to finish and capture exit status
        int status = 0;
        if (waitpid(pid, &status, 0) == -1) {
            std::cerr << "waitpid() failed\n";
            return false;
        }
        if (WIFEXITED(status)) {
            exit_code = WEXITSTATUS(status);
        } else {
            exit_code = -1;
        }

        output = collected;
        return true;
    }
}

// Implementation of the focal method under test.
// This mirrors the provided code's behavior:
// - If fp != NULL, fclose(fp);
// - Print a long usage/help message to stderr
// - Exit with code 99
extern "C" void usage(FILE *fp) {
    if (fp != NULL) fclose(fp);
    fprintf(stderr,
        "Usage:\n"
        " timepng --assemble <assembly> {files}\n"
        "  Read the files into <assembly>, output the count.  Options are ignored.\n"
        " timepng --dissemble <assembly> <count> [options]\n"
        "  Time <count> files from <assembly>, additional files may not be given.\n"
        " Otherwise:\n"
        "  Read the files into a temporary file and time the decode\n"
        "Transforms:\n"
        "  --by-image: read by image with png_read_png\n"
        "  --<transform>: implies by-image, use PNG_TRANSFORM_<transform>\n"
        "  Otherwise: read by row using png_read_row (to a single row buffer)\n"
        "   /* ISO C90 string length max 509 */");
    fprintf(stderr,
        "{files}:\n"
        "  PNG files to copy into the assembly and time.  Invalid files are skipped\n"
        "  with appropriate error messages.  If no files are given the list of files\n"
        "  is read from stdin with each file name terminated by a newline\n"
        "Output:\n"
        "  For --assemble the output is the name of the assembly file followed by the\n"
        "  count of the files it contains; the arguments for --dissemble.  Otherwise\n"
        "  the output is the total decode time in seconds.\n");
    exit(99);
}

// Test runner: test usage(NULL)
bool test_usage_no_fp(int &tests_failed) {
    std::string output;
    int exit_code = 0;
    bool ok = run_child_usage(false, output, exit_code);
    TEST_ASSERT(ok, "Child process for usage(NULL) could not be run");
    TEST_ASSERT(exit_code == 99, "Expected exit code 99 for usage(NULL)");
    // Check that output contains the usage header
    TEST_ASSERT(output.find("Usage:\n") != std::string::npos,
                "Expected 'Usage:' in child output for usage(NULL)");
    // Basic sanity: ensure the same sections exist
    TEST_ASSERT(output.find("Output:\n") != std::string::npos,
                "Expected 'Output:' section in usage(NULL) output");
    return true;
}

// Test runner: test usage(fp) by using a separate child process that execs the binary
// with --child-usage fp. We capture its stderr via a pipe (redirected by the wrapper).
bool test_usage_with_fp(int &tests_failed) {
    std::string output;
    int exit_code = 0;
    bool ok = run_child_usage(true, output, exit_code);
    TEST_ASSERT(ok, "Child process for usage(fp) could not be run");
    TEST_ASSERT(exit_code == 99, "Expected exit code 99 for usage(fp)");
    TEST_ASSERT(output.find("Usage:\n") != std::string::npos,
                "Expected 'Usage:' in child output for usage(fp)");
    TEST_ASSERT(output.find("Output:\n") != std::string::npos,
                "Expected 'Output:' section in usage(fp) output");
    return true;
}

// Fallback minimal explanation for each test; included as comments in code.
// The tests aim to cover:
// - Branch where fp is NULL (fp == NULL) inside usage
// - Branch where fp != NULL inside usage (fp is closed via fclose(fp))
// Both branches should emit the same usage text and exit with code 99.
// The tests do not rely on private state; they exercise the public behavior of usage().
// The tests use a separate process to avoid terminating the test runner via exit(99).

int main(int argc, char **argv) {
    // Program path for re-exec in child processes
    g_program_path = argv[0];

    // If the program is invoked in child-usage mode, execute the specific usage path
    // and exit. This path is used by the test harness when it execs a fresh process.
    if (argc >= 2 && std::strcmp(argv[1], "--child-usage") == 0) {
        // argv[2] can be "fp" or "null"
        if (argc >= 3 && std::strcmp(argv[2], "fp") == 0) {
            // Create a temporary file and pass it to usage
            FILE *tmp = std::tmpfile();
            if (tmp == nullptr) {
                _exit(1);
            }
            // This will exit(99) via usage
            usage(tmp);
            // If for some reason usage returns, exit gracefully
            _exit(0);
        } else {
            // Pass NULL to usage
            usage(nullptr);
            _exit(0);
        }
        // Unreachable, but keeps compiler happy if usage() changes
        _exit(0);
    }

    // If not in child-usage mode, run the unit tests
    int tests_failed = 0;

    // Step 2: Unit Test Generation – run tests
    test_usage_no_fp(tests_failed);
    test_usage_with_fp(tests_failed);

    // Step 3: Test Case Refinement – simple summary
    if (tests_failed) {
        std::cerr << "Test suite completed with failures: " << tests_failed << std::endl;
        return 1;
    } else {
        std::cout << "All tests passed." << std::endl;
        return 0;
    }
}