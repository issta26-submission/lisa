/*
Unit test suite for the focal method: main in expert.c
Targeted environment: C++11, POSIX (Linux/macOS). No GTest allowed.
Approach:
- Use a lightweight, self-contained test harness that spawns the external
  expert binary, captures its stdout/stderr, and inspects its exit code.
- Provide a small internal test framework (no abortive asserts) that reports
  pass/fail per test case.
- Cover representative execution paths by invoking the binary with various
  command line arguments (e.g., no args, verbose mode, file/sql options).
- NOTE: This test suite assumes the expert executable is built as ./expert
  in the test working directory prior to running tests.
*/

/* Step 1: Candidate Keywords (highlights from the focal method)
   - argc, argv, zDb, zErr, rc, i, iVerbose, db, p
   - usage, sqlite3_open, SQLITE_OK, sqlite3_errmsg
   - sqlite3_expert_new, sqlite3_expert_analyze, sqlite3_expert_count
   - sqlite3_expert_report, EXPERt_REPORT_CANDIDATES, EXPERT_REPORT_SQL
     EXPERT_REPORT_INDEXES, EXPERT_REPORT_PLAN
   - readSqlFromFile, sqlite3_expert_sql, sqlite3_expert_config
   - option_requires_argument, option_integer_arg, zArg parsing
   - sqlite3_strnicmp, EXPERT_CONFIG_SAMPLE
   - CLEANUP: sqlite3_expert_destroy, sqlite3_free
   - IO: fprintf, stdout, stderr
*/

/* Step 2: Unit Test Generation (external-process based tests)
   We implement 3 tests to exercise critical branches of main in expert.c:
   - Test 1: No arguments provided -> exercise argc < 2 branch
   - Test 2: Minimal verbose operation with a dummy DB path -> exercise -verbose path
   - Test 3: Basic options path with -file and -sql (control-flow coverage)
   Each test runs the external expert executable and asserts expected exit behavior
   or presence of output (non-empty stdout/stderr).
*/

#include <stdlib.h>
#include <functional>
#include <string.h>
#include <vector>
#include <sys/types.h>
#include <cerrno>
#include <sys/wait.h>
#include <sqlite3.h>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sqlite3expert.h>


// Lightweight test helpers (non-terminating assertions)
static bool expect_bool(bool cond, const std::string &desc) {
    if (!cond) {
        std::cerr << "Expectation FAILED: " << desc << std::endl;
        return false;
    }
    return true;
}

// Run the external expert executable with given args.
// It captures stdout, stderr and exit code.
// Returns true if the process was launched and waited successfully.
static bool run_expert_with(const std::vector<std::string> &args,
                            int &exit_code,
                            std::string &stdout_out,
                            std::string &stderr_out)
{
    // Build argv for execvp: first arg is the path to the executable
    const char *exe_path = "./expert";
    std::vector<char*> argv_exec;
    argv_exec.push_back(const_cast<char*>(exe_path));
    for (const auto &a : args) {
        argv_exec.push_back(strdup(a.c_str()));
    }
    argv_exec.push_back(nullptr);

    // Pipes to capture stdout and stderr from the child
    int stdout_pipe[2];
    int stderr_pipe[2];
    if (pipe(stdout_pipe) != 0) return false;
    if (pipe(stderr_pipe) != 0) return false;

    pid_t pid = fork();
    if (pid < 0) {
        // Fork failed
        return false;
    } else if (pid == 0) {
        // Child process
        // Redirect stdout and stderr
        dup2(stdout_pipe[1], STDOUT_FILENO);
        dup2(stderr_pipe[1], STDERR_FILENO);
        // Close unused pipe ends
        close(stdout_pipe[0]);
        close(stdout_pipe[1]);
        close(stderr_pipe[0]);
        close(stderr_pipe[1]);

        // Execute external binary
        execvp(exe_path, argv_exec.data());

        // If exec fails
        _exit(127);
    } else {
        // Parent process
        // Close write ends
        close(stdout_pipe[1]);
        close(stderr_pipe[1]);

        // Read stdout
        stdout_out.clear();
        char buf[4096];
        ssize_t n;
        while ((n = read(stdout_pipe[0], buf, sizeof(buf))) > 0) {
            stdout_out.append(buf, static_cast<size_t>(n));
        }
        // Read stderr
        stderr_out.clear();
        while ((n = read(stderr_pipe[0], buf, sizeof(buf))) > 0) {
            stderr_out.append(buf, static_cast<size_t>(n));
        }
        close(stdout_pipe[0]);
        close(stderr_pipe[0]);

        // Wait for child
        int status = 0;
        if (waitpid(pid, &status, 0) == -1) {
            return false;
        }
        if (WIFEXITED(status)) {
            exit_code = WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) {
            exit_code = 128 + WTERMSIG(status);
        } else {
            exit_code = -1;
        }
        // Clean up duplicated argv strings
        for (auto p : argv_exec) {
            if (p) free(p);
        }
        return true;
    }
}

// Test 1: No arguments provided to expert.
// Expecting the program to hit argc < 2 branch and exit with a non-zero status (typically -2).
// This test validates that the early usage path is exercised.
static bool test_no_args() {
    std::cout << "Running test_no_args: no arguments provided" << std::endl;
    int exit_code = -1;
    std::string stdout_str, stderr_str;
    bool ok = run_expert_with({}, exit_code, stdout_str, stderr_str);
    if (!ok) {
        std::cerr << "test_no_args: Failed to spawn expert." << std::endl;
        return false;
    }
    // In POSIX, exit(-2) becomes 254 when captured via WEXITSTATUS
    bool exit_code_ok = (exit_code == 254);
    if (!exit_code_ok) {
        std::cerr << "test_no_args: Expected exit code 254, got " << exit_code << std::endl;
    }
    // Also ensure stdout/stderr exist (non-empty is better for coverage)
    bool output_present = (!stdout_str.empty() || !stderr_str.empty());
    if (!output_present) {
        std::cerr << "test_no_args: No output captured (stdout/stderr are empty)." << std::endl;
    }
    return exit_code_ok && output_present;
}

// Test 2: Minimal verbose operation with a dummy DB path.
// This exercises the -verbose parsing path and subsequent reporting (if the DB can be opened).
// We expect a successful run (exit code 0) under typical conditions.
static bool test_minimal_verbose() {
    std::cout << "Running test_minimal_verbose: minimal args with -verbose" << std::endl;
    int exit_code = -1;
    std::string stdout_str, stderr_str;
    // Use a dummy database name; sqlite3_open will create a new empty database if allowed.
    // The exact behavior depends on sqlite3_expert availability; we allow exit code 0 as success here.
    bool ok_spawn = run_expert_with({"test_minimal.db", "-verbose", "0"}, exit_code, stdout_str, stderr_str);
    if (!ok_spawn) {
        std::cerr << "test_minimal_verbose: Failed to spawn expert." << std::endl;
        return false;
    }
    // Accept both 0 (success) or 1 (generic error) but prefer 0 for coverage
    bool exit_ok = (exit_code == 0);
    if (!exit_ok) {
        std::cerr << "test_minimal_verbose: Expected exit code 0, got " << exit_code << std::endl;
    }
    // Ensure some output was produced (candidates or queries)
    bool output_present = !stdout_str.empty();
    if (!output_present) {
        std::cerr << "test_minimal_verbose: Expected stdout content, got empty." << std::endl;
    }
    return exit_ok && output_present;
}

// Test 3: Basic option path with -file and -sql to cover option handling paths.
// This test exercises the readSqlFromFile and sqlite3_expert_sql branches.
// Depending on environment and sqlite3_expert implementation, exit may be 0 or non-zero.
// We primarily check that the program runs and produces some output (or at least not crash).
static bool test_file_and_sql_options() {
    std::cout << "Running test_file_and_sql_options: -file and -sql options" << std::endl;
    int exit_code = -1;
    std::string stdout_str, stderr_str;
    // Provide a dummy db path and arguments
    bool ok_spawn = run_expert_with({"test_db.db", "-file", "dummy.sql", "-sql", "SELECT 1;"},
                                     exit_code, stdout_str, stderr_str);
    if (!ok_spawn) {
        std::cerr << "test_file_and_sql_options: Failed to spawn expert." << std::endl;
        return false;
    }
    // Accept 0 as success (best case). If not, still report the actual exit code for coverage.
    bool exit_ok = (exit_code == 0);
    if (!exit_ok) {
        std::cerr << "test_file_and_sql_options: Expected exit code 0, got " << exit_code << std::endl;
    }
    bool output_present = !stdout_str.empty() || !stderr_str.empty();
    if (!output_present) {
        std::cerr << "test_file_and_sql_options: Expected some output to be produced." << std::endl;
    }
    return exit_ok && output_present;
}

// Step 3: Test runner main
int main() {
    // Basic test harness that runs the above tests and reports results.
    typedef bool (*TestFn)();
    struct TestCase {
        const char* name;
        TestFn fn;
    };
    TestCase tests[] = {
        {"test_no_args", test_no_args},
        {"test_minimal_verbose", test_minimal_verbose},
        {"test_file_and_sql_options", test_file_and_sql_options},
    };

    int total = sizeof(tests)/sizeof(tests[0]);
    int passed = 0;

    std::cout << "Expert main unit test suite (C++11) starting. Binary under test: ./expert" << std::endl;

    for (int i = 0; i < total; ++i) {
        std::cout << "[RUN] " << tests[i].name << std::endl;
        bool res = tests[i].fn();
        if (res) {
            std::cout << "[PASS] " << tests[i].name << std::endl;
            ++passed;
        } else {
            std::cout << "[FAIL] " << tests[i].name << std::endl;
        }
    }

    std::cout << "Tests passed: " << passed << " / " << total << std::endl;
    return (passed == total) ? 0 : 1;
}