/*
  C++11 test suite for the focal C program's main function (untgz.c)

  Approach:
  - We treat the target program as an external executable named "untgz".
  - Each test spawns a separate process to execute "./untgz" with a specific
    set of command-line arguments to exercise different branches of main().
  - We capture stdout/stderr and the process exit code to validate behavior.
  - This avoids modifying the original C source and avoids private/internal
    state assumptions.
  - No Google Test; a simple, self-contained test harness with descriptive
    inline comments for each test case.

  Notes:
  - The tests rely on POSIX facilities (fork/exec) and are intended for
    environments where untgz can be executed as a separate binary.
  - If your CI uses a different path for the untgz binary, adjust UNTGZ_PATH.
  - The tests check exit codes and, where feasible, basic presence of output.
    Specific string assertions are avoided due to potential variation in
    help messages, but exit codes give deterministic behavior on known code paths.
*/

#include <cstring>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <vector>
#include <zlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>



// Path to the target executable under test.
// Adjust if your build system places the binary elsewhere.
static const char* UNTGZ_PATH = "./untgz";

// Helper to run the untgz binary with given args and capture outputs.
// Returns 0 on success to spawn and wait, and sets exit_code accordingly.
// Outputs are captured by reference.
int runUntgz(const std::vector<std::string>& args, std::string& stdout_out, std::string& stderr_out, int& exit_code)
{
    // Build argv for execv: argv[0] should be the program name as invoked.
    std::vector<char*> argv_vec;
    argv_vec.reserve(args.size() + 1);
    for (const auto& s : args) {
        // We cast away constness safely because execv expects modifiable array,
        // but we immediately pass to execv; memory lifetime is the vector.
        argv_vec.push_back(const_cast<char*>(s.c_str()));
    }
    argv_vec.push_back(nullptr);

    int stdout_pipe[2];
    int stderr_pipe[2];
    if (pipe(stdout_pipe) != 0) return -1;
    if (pipe(stderr_pipe) != 0) {
        close(stdout_pipe[0]); close(stdout_pipe[1]);
        return -1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        // Fork failed
        close(stdout_pipe[0]); close(stdout_pipe[1]);
        close(stderr_pipe[0]); close(stderr_pipe[1]);
        return -1;
    }

    if (pid == 0) {
        // Child process
        // Redirect stdout
        dup2(stdout_pipe[1], STDOUT_FILENO);
        // Redirect stderr
        dup2(stderr_pipe[1], STDERR_FILENO);
        // Close unused ends
        close(stdout_pipe[0]); close(stdout_pipe[1]);
        close(stderr_pipe[0]); close(stderr_pipe[1]);

        // Execute the untgz binary
        execv(UNTGZ_PATH, argv_vec.data());

        // If exec fails
        _exit(127);
    }

    // Parent process: close writing ends, read outputs
    close(stdout_pipe[1]);
    close(stderr_pipe[1]);

    // Read stdout
    constexpr size_t BUF_SIZE = 4096;
    char buf[BUF_SIZE];
    stdout_out.clear();
    stderr_out.clear();

    // Non-blocking read loop until child exits
    int status = 0;
    while (true) {
        // Read stdout
        ssize_t n = read(stdout_pipe[0], buf, BUF_SIZE);
        if (n > 0) stdout_out.append(buf, buf + n);
        // Read stderr
        ssize_t m = read(stderr_pipe[0], buf, BUF_SIZE);
        if (m > 0) stderr_out.append(buf, buf + m);

        pid_t result = waitpid(pid, &status, WNOHANG);
        if (result == pid) break;
        // If both pipes are closed and no child yet, continue looping
        if (n <= 0 && m <= 0 && result == 0) {
            // Sleep briefly to avoid busy wait
            usleep(1000);
            continue;
        }
    }

    // Drain any remaining data after child termination
    ssize_t n;
    while ((n = read(stdout_pipe[0], buf, BUF_SIZE)) > 0) stdout_out.append(buf, buf + n);
    while ((n = read(stderr_pipe[0], buf, BUF_SIZE)) > 0) stderr_out.append(buf, buf + n);

    close(stdout_pipe[0]);
    close(stderr_pipe[0]);

    if (WIFEXITED(status)) {
        exit_code = WEXITSTATUS(status);
        return 0;
    } else {
        // Abnormal termination
        exit_code = -1;
        return 0;
    }
}

// Utility: simple assertion helper
static void assert_true(bool cond, const std::string& msg)
{
    if (!cond) {
        std::cerr << "ASSERTION FAILED: " << msg << std::endl;
    }
}

// Test 1: Help flag (-h) should produce help output and exit cleanly (exit code 0 in this environment).
// Rationale: Cover the path where the program handles a help request.
void test_help_flag()
{
    std::cout << "Test 1: Help flag (-h) invocation" << std::endl;
    std::vector<std::string> args = { "./untgz", "-h" };
    std::string out, err;
    int exit_code;
    int rc = runUntgz(args, out, err, exit_code);
    // If rc != 0, we still consider testable; we primarily check exit_code for expected behavior.
    // Best-effort assertion: expect a successful invocation (exit code 0) in typical help scenarios.
    if (rc == 0) {
        assert_true(exit_code == 0, "Help invocation should exit with code 0");
    } else {
        // If process spawn failed, that's a test harness infrastructure issue.
        std::cerr << "Warning: Failed to spawn untgz for help test." << std::endl;
    }
    // Optional: ensure some help-related content is produced
    // Note: exact text depends on the help() implementation.
    if (!out.empty() || !err.empty()) {
        std::cout << "Help output captured (stdout+stderr)." << std::endl;
    }
}

// Test 2: No arguments provided (argc == 1) should trigger help path in the focal method.
// Rationale: Exercise the early code path that handles argc == 1.
void test_no_args_triggers_help()
{
    std::cout << "Test 2: No-argument invocation to trigger help path" << std::endl;
    std::vector<std::string> args = { "./untgz" };
    std::string out, err;
    int exit_code;
    int rc = runUntgz(args, out, err, exit_code);

    if (rc == 0) {
        // If help() exits with 0 in this build, assert accordingly.
        assert_true(exit_code == 0, "No-arg invocation should exit with code 0 if help returns 0");
    } else {
        std::cerr << "Warning: Failed to spawn untgz for no-args test." << std::endl;
    }
}

// Test 3: Unknown option (e.g., -x) should result in an error path and non-zero exit code.
// Rationale: Ensure default error handling and exit status when an unsupported option is provided.
void test_unknown_option()
{
    std::cout << "Test 3: Unknown option (-x) should trigger error path" << std::endl;
    std::vector<std::string> args = { "./untgz", "-x" };
    std::string out, err;
    int exit_code;
    int rc = runUntgz(args, out, err, exit_code);
    // We expect a non-zero exit code, typically 1 for error path.
    if (rc == 0) {
        assert_true(exit_code != 0, "Unknown option should exit with non-zero code");
    } else {
        std::cerr << "Warning: Failed to spawn untgz for unknown option test." << std::endl;
    }
    // Optional: examine stderr for "Unknown option" string if deterministic in your build.
}

// Test 4: List mode with a non-existent file should attempt to gzopen and fail gracefully (exit code 1).
// Rationale: Cover the TGZ_LIST path where the TGZ file cannot be opened.
void test_list_with_missing_file()
{
    std::cout << "Test 4: List mode with non-existent TGZ file" << std::endl;
    std::vector<std::string> args = { "./untgz", "-l", "nonexistent_file.tgz" };
    std::string out, err;
    int exit_code;
    int rc = runUntgz(args, out, err, exit_code);
    if (rc == 0) {
        // If the program prints a helpful message and exits, verify non-zero exit code assumption.
        // Most builds would return 1 when gzopen fails.
        assert_true(exit_code == 1, "Missing TGZ file should lead to exit code 1 in list mode");
    } else {
        std::cerr << "Warning: Failed to spawn untgz for list-missing-file test." << std::endl;
    }
}

// Test 5: Extract mode with a non-existent TGZ should fail gzopen and exit with code 1.
// Rationale: Exercise the extract branch (TGZ_EXTRACT) and ensure graceful failure.
void test_extract_with_missing_file()
{
    std::cout << "Test 5: Extract mode with non-existent TGZ file" << std::endl;
    std::vector<std::string> args = { "./untgz", "-x", "nonexistent_file.tgz" };
    std::string out, err;
    int exit_code;
    int rc = runUntgz(args, out, err, exit_code);
    if (rc == 0) {
        assert_true(exit_code == 1, "Missing TGZ file in extract mode should yield exit code 1");
    } else {
        std::cerr << "Warning: Failed to spawn untgz for extract-missing-file test." << std::endl;
    }
}

int main()
{
    std::cout << "Starting untgz focal method test suite (C++11 harness)" << std::endl;

    test_help_flag();
    test_no_args_triggers_help();
    test_unknown_option();
    test_list_with_missing_file();
    test_extract_with_missing_file();

    std::cout << "Untgz focal method test suite completed." << std::endl;
    // Return 0 to indicate the harness completed (individual tests log their results).
    return 0;
}