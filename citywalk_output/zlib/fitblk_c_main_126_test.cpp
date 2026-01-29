/*
  C++11 test harness for the focal method main() in fitblk.c

  Purpose and approach:
  - Treat the focal method as a black-box executable (fitblk).
  - Validate control-flow and input validation branches by invoking the binary with various argument configurations
    and with different stdin contents.
  - Use a small, self-contained test framework (no Google Test) and a POSIX-based process runner to capture
    stdout/stderr and exit codes.
  - Tests are designed to be executable against a pre-built fitblk binary in the same directory (./fitblk).

  Candidate Keywords (Step 1/2):
  - zlib, deflate, inflate, deflateInit, inflateInit, partcompress, recompress
  - EXCESS, MARGIN (size margins for buffering)
  - stdin, stdout, stderr, ferror, fwrite
  - quit (error reporting helper)
  - unsigned, size, ret, argv processing
  - memory management: malloc/free
  - error paths: Z_ERRNO, Z_MEM_ERROR, Z_STREAM_END, Z_OK
  - input validation: argc check, numeric argument, minimum size 8
  - compression round-trips and final block emission

  Dependencies (Step 2):
  - The focal class dependency is essentially the C zlib-based main routine; the test suite interacts with the
    executable binary rather than internal static/private helpers.
  - No GTest allowed. We implement direct main() tests by spawning ./fitblk as a subprocess and
    validating its behavior.

  Domain knowledge (Step 3):
  - Use only standard library and POSIX facilities for process control and I/O redirection.
  - Use non-terminating assertions (i.e., test continues on failures to collect multiple results).
  - Do not mock private/internal details of the C file; test external observable behavior.
  - If a function is static in the focal file, it is not accessible from tests; we exercise via the binary.

  How to run:
  - Build fitblk.c into an executable named fitblk (e.g., gcc -o fitblk fitblk.c -lz)
  - Compile this test harness (e.g., g++ -std=c++11 -O2 -Wall -Wextra -pthread fitblk_test.cpp -o fitblk_test)
  - Run ./fitblk_test
  - The harness will spawn ./fitblk with various args/stdin combinations and report test outcomes.

  Notes:
  - This test harness assumes the fitblk binary is in the same directory as the test executable.
  - Some outputs from fitblk are binary data; the tests focus on exit codes, presence of expected textual
    diagnostic messages on stderr, and non-empty stdout where applicable.
*/

#include <cstring>
#include <fcntl.h>
#include <cstdlib>
#include <cerrno>
#include <assert.h>
#include <string>
#include <sys/types.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <zlib.h>
#include <random>
#include <unistd.h>
#include <stdlib.h>
#include <sstream>
#include <sys/wait.h>



// Candidate Keywords -----------------------------------------------------------
// See analysis comments above for rationale and mapping to the focal method's behavior.

// RunResult stores the outcome of a single fitblk execution
struct RunResult {
    int exit_code;              // exit status (0 for success)
    std::string stdout_str;     // captured stdout
    std::string stderr_str;     // captured stderr
};

// Helper: read all data from a file descriptor into string
static std::string read_all(int fd) {
    std::string data;
    char buf[4096];
    ssize_t n;
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        data.append(buf, static_cast<size_t>(n));
    }
    return data;
}

// Execute the fitblk binary with given args and stdin, capture stdout/stderr and exit code
static RunResult run_fitblk(const std::vector<std::string>& args, const std::string& stdin_data) {
    RunResult result;
    result.exit_code = -1;
    result.stdout_str.clear();
    result.stderr_str.clear();

    int stdout_pipe[2];
    int stderr_pipe[2];
    int stdin_pipe[2];

    if (pipe(stdout_pipe) != 0) { perror("pipe stdout"); return result; }
    if (pipe(stderr_pipe) != 0) { perror("pipe stderr"); return result; }
    if (pipe(stdin_pipe) != 0) { perror("pipe stdin"); return result; }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return result;
    }

    if (pid == 0) {
        // Child
        // Redirect stdin, stdout, stderr
        dup2(stdin_pipe[0], STDIN_FILENO);
        dup2(stdout_pipe[1], STDOUT_FILENO);
        dup2(stderr_pipe[1], STDERR_FILENO);

        // Close unused ends
        close(stdin_pipe[1]);
        close(stdout_pipe[0]);
        close(stderr_pipe[0]);
        // Also close potential extra fds
        // Prepare argv for execv
        std::vector<char*> cargs;
        cargs.reserve(args.size() + 2);
        // First argument is program name
        cargs.push_back(const_cast<char*>("fitblk"));
        for (const auto& s : args) {
            cargs.push_back(const_cast<char*>(s.c_str()));
        }
        cargs.push_back(nullptr);

        // Execute
        execv("./fitblk", cargs.data());
        // If execv returns, it's an error
        _exit(127);
    }

    // Parent
    // Close ends not used by parent
    close(stdin_pipe[0]);
    close(stdout_pipe[1]);
    close(stderr_pipe[1]);

    // Write stdin data
    if (!stdin_data.empty()) {
        size_t total = 0;
        while (total < stdin_data.size()) {
            ssize_t w = write(stdin_pipe[1], stdin_data.data() + total, stdin_data.size() - total);
            if (w <= 0) break;
            total += static_cast<size_t>(w);
        }
    }
    // Close stdin to signal EOF
    close(stdin_pipe[1]);

    // Read outputs
    result.stdout_str = read_all(stdout_pipe[0]);
    result.stderr_str = read_all(stderr_pipe[0]);
    close(stdout_pipe[0]);
    close(stderr_pipe[0]);

    // Wait for child
    int status = 0;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) {
        result.exit_code = WEXITSTATUS(status);
    } else if (WIFSIGNALED(status)) {
        // Terminated by signal
        result.exit_code = 128 + WTERMSIG(status);
    } else {
        result.exit_code = -1;
    }

    return result;
}

// Helper: produce some binary-like data
static std::string make_binary_data(size_t len, unsigned int seed = 12345) {
    std::string s;
    s.resize(len);
    std::mt19937 rng(seed);
    std::uniform_int_distribution<unsigned char> dist(0, 255);
    for (size_t i = 0; i < len; ++i) {
        s[i] = static_cast<char>(dist(rng));
    }
    return s;
}

// Simple test framework
static int tests_run = 0;
static int tests_failed = 0;

#define ASSERT_TRUE(cond, msg) \
    do { \
        tests_run++; \
        if (!(cond)) { \
            std::cerr << "Test failed: " << (msg) << " (line " << __LINE__ << ")\n"; \
            ++tests_failed; \
        } \
    } while(0)

#define ASSERT_CONTAINS(hay, needle, msg) \
    do { \
        tests_run++; \
        if ((hay).find((needle)) == std::string::npos) { \
            std::cerr << "Test failed: " << (msg) << " - missing: \"" << (needle) << "\"\n"; \
            ++tests_failed; \
        } \
    } while(0)

#define ASSERT_EQ(a, b, msg) \
    do { \
        tests_run++; \
        if (!((a) == (b))) { \
            std::cerr << "Test failed: " << (msg) << " - expected: " << (b) << " got: " << (a) << "\n"; \
            ++tests_failed; \
        } \
    } while(0)

// Test Cases -------------------------------------------------------------------

/*
  Test 1: No argument provided
  - Expect non-zero exit code
  - Expect stderr to contain the error message about needing one argument
*/
static bool test_no_argument() {
    std::vector<std::string> args; // no extra arguments
    RunResult r = run_fitblk(args, "");
    bool ok = (r.exit_code != 0);
    ASSERT_TRUE(ok, "No-argument should exit with non-zero code");
    ASSERT_CONTAINS(r.stderr_str, "need one argument: size of output block", "No-argument error message missing");
    return ok;
}

/*
  Test 2: Non-numeric argument
  - argv = {"abc"}
  - Expect non-zero exit code
  - Expect stderr to mention "argument must be a number"
*/
static bool test_non_numeric_argument() {
    std::vector<std::string> args = {"abc"};
    RunResult r = run_fitblk(args, "");
    bool ok = (r.exit_code != 0);
    ASSERT_TRUE(ok, "Non-numeric argument should exit non-zero");
    ASSERT_CONTAINS(r.stderr_str, "argument must be a number", "Non-numeric argument error missing");
    return ok;
}

/*
  Test 3: Argument too small (less than 8)
  - argv = {"4"}
  - Expect non-zero exit code
  - Expect stderr to contain "need positive size of 8 or greater"
*/
static bool test_small_argument() {
    std::vector<std::string> args = {"4"};
    RunResult r = run_fitblk(args, "");
    bool ok = (r.exit_code != 0);
    ASSERT_TRUE(ok, "Too small argument should exit non-zero");
    ASSERT_CONTAINS(r.stderr_str, "need positive size of 8 or greater", "Small argument error missing");
    return ok;
}

/*
  Test 4: Empty input with valid size
  - argv = {"8"}
  - stdin = empty
  - Expect exit code 0
  - Expect stdout to be empty (no data written)
  - Expect stderr to indicate that 0 bytes were unused out of 8 (all input)
*/
static bool test_empty_input_no_data() {
    std::vector<std::string> args = {"8"};
    RunResult r = run_fitblk(args, "");
    bool ok = (r.exit_code == 0);
    ASSERT_TRUE(ok, "Empty input should exit with code 0");
    // stdout should be 0-length
    ASSERT_EQ(r.stdout_str.size(), 0u, "Expected no stdout data when input is empty");
    // stderr should mention 0 bytes unused
    ASSERT_CONTAINS(r.stderr_str, "0 bytes unused out of 8 requested (all input)", "Empty input stderr message missing");
    return ok;
}

/*
  Test 5: Non-empty input with data
  - argv = {"1024"}
  - stdin = some binary-like data (to avoid predictable compression)
  - Expect exit code 0
  - Expect stdout to be non-empty (block of data emitted)
*/
static bool test_nonempty_input() {
    std::vector<std::string> args = {"1024"};
    std::string input = make_binary_data(4096, 54321); // 4 KB of pseudo-random data
    RunResult r = run_fitblk(args, input);
    bool ok = (r.exit_code == 0);
    if (!ok) {
        std::cerr << "Test nonempty input failed: exit_code=" << r.exit_code << "\n";
    }
    // Expect stdout to contain some data (binary output)
    if (r.stdout_str.empty()) {
        // It's possible in some cases all data is not written, but this is unlikely for nontrivial input.
        std::cerr << "Warning: stdout is empty for non-empty input. This may indicate an edge-case in compression path.\n";
        ok = false;
    } else {
        ok = true;
    }
    ASSERT_TRUE(ok, "Non-empty input should produce some stdout data");
    // We also ensure stderr reports summary text
    ASSERT_TRUE(r.stderr_str.find("bytes unused out of 1024") != std::string::npos,
                "Non-empty input stderr should mention bytes unused out of 1024");
    return ok;
}

// Main: run all tests
int main() {
    // Seed time for deterministic test runs if needed; not strictly required here
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    std::cout << "fitblk_test: starting test suite\n";

    bool t1 = test_no_argument();
    bool t2 = test_non_numeric_argument();
    bool t3 = test_small_argument();
    bool t4 = test_empty_input_no_data();
    bool t5 = test_nonempty_input();

    int total = 5;
    int passed = (t1 ? 1 : 0) + (t2 ? 1 : 0) + (t3 ? 1 : 0) + (t4 ? 1 : 0) + (t5 ? 1 : 0);

    std::cout << "fitblk_test: " << passed << " / " << total << " tests passed\n";

    if (tests_failed > 0) {
        std::cerr << "fitblk_test: " << tests_failed << " test(s) failed\n";
    } else {
        std::cout << "fitblk_test: all tests passed\n";
    }

    return (tests_failed == 0) ? 0 : 1;
}