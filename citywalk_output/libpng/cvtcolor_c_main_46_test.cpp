#include <algorithm>
#include <sRGB.h>
#include <vector>
#include <sstream>
#include <string.h>
#include <fenv.h>
#include <string>
#include <sys/stat.h>
#include <errno.h>
#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <cstring>
#include <math.h>


// This test suite drives the focal C program cvtcolor (cv tcolor) by executing it as a
// separate process and capturing its stdout/stderr for assertions.
// It avoids any GTest dependency and uses only the C++11 standard library + POSIX APIs.
// The tests aim to exercise various command-line argument paths and capture both
// normal outputs and error messages (on stderr) to maximize coverage.

/*
Assumptions:
- The target executable cvtcolor is available in the current directory (./cvtcolor)
  or in PATH as cvtcolor. The tests will attempt ./cvtcolor first, then cvtcolor.
- The focal code relies on external (stubbed) helpers in the provided snippet.
  We treat its behavior as deterministic given the same command-line inputs.
- This test suite focuses on basic control flow coverage and I/O behavior, not
  on the numeric correctness of the image color conversions (since core helper
  functions are stubs in the provided focal file).
*/

// Simple helper to check if a file is executable
static bool is_executable(const std::string& path) {
    struct stat sb;
    if (stat(path.c_str(), &sb) != 0) return false;
    if (!S_ISREG(sb.st_mode)) return false;
    if ((sb.st_mode & (S_IEXEC)) == 0) return false;
    return true;
}

// Run the cvtcolor program with given args (excluding argv[0]), capture stdout/stderr and exit code.
// Returns true on successful spawn/communication; false otherwise.
static bool runProgram(const std::vector<std::string>& args,
                       std::string& stdout_out,
                       std::string& stderr_out,
                       int& exit_code)
{
    // Determine program path: prefer ./cvtcolor, then fallback to cvtcolor (PATH)
    std::string program_path = "./cvtcolor";
    if (!is_executable(program_path)) {
        program_path = "cvtcolor";
        // If still not executable, fail early
        if (!is_executable(program_path)) {
            std::cerr << "Test harness: cvtcolor executable not found." << std::endl;
            return false;
        }
    }

    int stdout_pipe[2];
    int stderr_pipe[2];
    if (pipe(stdout_pipe) != 0) return false;
    if (pipe(stderr_pipe) != 0) return false;

    pid_t pid = fork();
    if (pid < 0) {
        return false;
    }
    if (pid == 0) {
        // Child process
        // Redirect stdout and stderr
        dup2(stdout_pipe[1], STDOUT_FILENO);
        dup2(stderr_pipe[1], STDERR_FILENO);
        // Close unused fds
        close(stdout_pipe[0]);
        close(stdout_pipe[1]);
        close(stderr_pipe[0]);
        close(stderr_pipe[1]);

        // Build argv for execv
        std::vector<char*> argv_exec;
        argv_exec.push_back(const_cast<char*>(program_path.c_str()));
        for (const auto& s : args) argv_exec.push_back(const_cast<char*>(s.c_str()));
        argv_exec.push_back(nullptr);

        execv(program_path.c_str(), argv_exec.data());
        // If execv fails
        _exit(127);
    }

    // Parent process
    // Close write ends
    close(stdout_pipe[1]);
    close(stderr_pipe[1]);

    // Read stdout
    char buffer[4096];
    std::stringstream out_ss;
    ssize_t n;
    while ((n = read(stdout_pipe[0], buffer, sizeof(buffer))) > 0) {
        out_ss.write(buffer, n);
    }
    // Read stderr
    std::stringstream err_ss;
    while ((n = read(stderr_pipe[0], buffer, sizeof(buffer))) > 0) {
        err_ss.write(buffer, n);
    }
    close(stdout_pipe[0]);
    close(stderr_pipe[0]);

    // Wait for child
    int status = 0;
    if (waitpid(pid, &status, 0) < 0) return false;
    if (WIFEXITED(status)) exit_code = WEXITSTATUS(status);
    else exit_code = -1;

    stdout_out = out_ss.str();
    stderr_out = err_ss.str();

    return true;
}

// Small assertion helper
#define ASSERT_TRUE(cond, msg) do { if(!(cond)) { \
    std::cerr << "ASSERTION FAILED: " << (msg) << "\n" \
              << "  File: " << __FILE__ << ", Line: " << __LINE__ << "\n"; \
    return false; } } while(0)

#define ASSERT_CONTAINS(hay, needle, msg) do { \
    if ((hay).find((needle)) == std::string::npos) { \
        std::cerr << "ASSERTION FAILED: " << (msg) \
                  << " (substring not found: '" << (needle) << "')\n" \
                  << "  In: \"" << (hay) << "\"\n"; \
        return false; } } while(0)

#define ASSERT_EQUALS(actual, expected, msg) do { \
    if ((actual) != (expected)) { \
        std::cerr << "ASSERTION FAILED: " << (msg) \
                  << " (expected: \"" << (expected) \
                  << "\", actual: \"" << (actual) << "\")\n"; \
        return false; } } while(0)

// Test 1: No command-line arguments
// - Purpose: Ensure program handles the absence of arguments gracefully.
// - Expected: returns 0 and prints a single newline (i.e., stdout == "\n").
static bool test_no_args() {
    // No additional args after program name
    std::vector<std::string> args;
    std::string stdout_str, stderr_str;
    int exit_code = -1;

    if (!runProgram(args, stdout_str, stderr_str, exit_code)) {
        std::cerr << "Test 1: Failed to run cvtcolor with no args.\n";
        return false;
    }

    // Validate exit code and basic stdout structure
    ASSERT_EQUALS(exit_code, 0, "Test 1: exit code should be 0 for no args");
    // stdout should be just a newline when no channels are provided
    ASSERT_EQUALS(stdout_str, "\n", "Test 1: stdout should be a single newline when no args are provided");
    // No specific stderr content is required for this test
    (void)stderr_str;
    return true;
}

// Test 2: Gray conversion with 3 numeric channels
// - Purpose: Exercise -gray pathway and enough channels to perform a conversion.
// - Expected: exit 0 and stdout contains a line of numbers (format: leading space numbers).
static bool test_gray_three_channels() {
    // Command: -gray 0 0 0
    std::vector<std::string> args = {"-gray", "0", "0", "0"};
    std::string stdout_str, stderr_str;
    int exit_code = -1;
    if (!runProgram(args, stdout_str, stderr_str, exit_code)) {
        std::cerr << "Test 2: Failed to run cvtcolor with -gray 0 0 0.\n";
        return false;
    }

    ASSERT_EQUALS(exit_code, 0, "Test 2: exit code should be 0 for -gray with 3 channels");
    // The output should be a line of numbers; we expect at least a newline and spaces
    ASSERT_TRUE(!stdout_str.empty(), "Test 2: stdout should not be empty");
    ASSERT_TRUE(stdout_str[0] == ' ', "Test 2: stdout should start with a space (format: ' nnn')");

    // Ensure newline terminates the output
    if (stdout_str.find('\n') == std::string::npos) {
        std::cerr << "Test 2: stdout should end with a newline.\n";
        return false;
    }
    // stderr should typically be empty for this success path
    (void)stderr_str;
    return true;
}

// Test 3: Linear mode with 2 channels
// - Purpose: Exercise the linear path where channels is even and includes a division by 255.
// - Expected: exit 0 and stdout contains a line of numbers.
static bool test_linear_two_channels() {
    // Command: -linear 0 0
    std::vector<std::string> args = {"-linear", "0", "0"};
    std::string stdout_str, stderr_str;
    int exit_code = -1;
    if (!runProgram(args, stdout_str, stderr_str, exit_code)) {
        std::cerr << "Test 3: Failed to run cvtcolor with -linear 0 0.\n";
        return false;
    }

    ASSERT_EQUALS(exit_code, 0, "Test 3: exit code should be 0 for -linear with 2 channels");
    ASSERT_TRUE(!stdout_str.empty(), "Test 3: stdout should not be empty");
    ASSERT_TRUE(stdout_str[0] == ' ', "Test 3: stdout should start with a space");
    if (stdout_str.find('\n') == std::string::npos) {
        std::cerr << "Test 3: stdout should end with a newline.\n";
        return false;
    }
    (void)stderr_str;
    return true;
}

// Test 4: Color mode with too many channels
// - Purpose: Exercise error path for -color when more than 2 channels are provided.
// - Expected: exit 0 and stderr contains "too many channels" and mentions -color.
static bool test_color_too_many_channels() {
    // Command: -color 1 2 3
    std::vector<std::string> args = {"-color", "1", "2", "3"};
    std::string stdout_str, stderr_str;
    int exit_code = -1;
    if (!runProgram(args, stdout_str, stderr_str, exit_code)) {
        std::cerr << "Test 4: Failed to run cvtcolor with -color 1 2 3.\n";
        return false;
    }

    ASSERT_EQUALS(exit_code, 0, "Test 4: exit code should be 0 even on error path");
    // Check that stderr contains the expected error message
    ASSERT_CONTAINS(stderr_str, "too many channels", "Test 4: stderr should mention too many channels");
    ASSERT_CONTAINS(stderr_str, "-color", "Test 4: stderr should mention -color since the error occurred in -color path");
    return true;
}

// Test 5: sRGB option flag with a single channel
// - Purpose: Ensure -sRGB flag does not force linear conversion and still prints a value.
// - Expected: exit 0 and stdout contains a line (starting with a space).
static bool test_srgb_option_single_channel() {
    // Command: -sRGB 128
    std::vector<std::string> args = {"-sRGB", "128"};
    std::string stdout_str, stderr_str;
    int exit_code = -1;
    if (!runProgram(args, stdout_str, stderr_str, exit_code)) {
        std::cerr << "Test 5: Failed to run cvtcolor with -sRGB 128.\n";
        return false;
    }

    ASSERT_EQUALS(exit_code, 0, "Test 5: exit code should be 0 for -sRGB with 1 channel");
    ASSERT_TRUE(!stdout_str.empty(), "Test 5: stdout should not be empty");
    ASSERT_TRUE(stdout_str[0] == ' ', "Test 5: stdout should start with a space");
    if (stdout_str.find('\n') == std::string::npos) {
        std::cerr << "Test 5: stdout should end with a newline.\n";
        return false;
    }
    (void)stderr_str;
    return true;
}

// Entry point for the test suite
int main() {
    int passed = 0;
    int total = 0;

    std::cout << "Starting cvtcolor focal method test suite (C++11, no GTest)...\n";

    // Run Test 1
    ++total;
    if (test_no_args()) {
        std::cout << "[OK] Test 1: No arguments handling\n";
        ++passed;
    } else {
        std::cout << "[FAIL] Test 1: No arguments handling\n";
    }

    // Run Test 2
    ++total;
    if (test_gray_three_channels()) {
        std::cout << "[OK] Test 2: -gray with 3 channels\n";
        ++passed;
    } else {
        std::cout << "[FAIL] Test 2: -gray with 3 channels\n";
    }

    // Run Test 3
    ++total;
    if (test_linear_two_channels()) {
        std::cout << "[OK] Test 3: -linear with 2 channels\n";
        ++passed;
    } else {
        std::cout << "[FAIL] Test 3: -linear with 2 channels\n";
    }

    // Run Test 4
    ++total;
    if (test_color_too_many_channels()) {
        std::cout << "[OK] Test 4: -color with too many channels\n";
        ++passed;
    } else {
        std::cout << "[FAIL] Test 4: -color with too many channels\n";
    }

    // Run Test 5
    ++total;
    if (test_srgb_option_single_channel()) {
        std::cout << "[OK] Test 5: -sRGB with 1 channel\n";
        ++passed;
    } else {
        std::cout << "[FAIL] Test 5: -sRGB with 1 channel\n";
    }

    std::cout << "Test results: " << passed << " / " << total << " tests passed.\n";

    return (passed == total) ? 0 : 1;
}