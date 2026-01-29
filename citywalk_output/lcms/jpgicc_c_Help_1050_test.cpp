/*
  C++11 test suite for the focal method Help in jpgicc.c

  Approach:
  - The Help function prints usage information to stderr and then calls exit(0).
  - To test it without terminating the test runner, we fork a child process.
  - In the child, we redirect stderr to a pipe, invoke Help(level), which will write to the pipe and terminate.
  - The parent collects the emitted text from the pipe and inspects it for expected substrings.
  - We provide lightweight test scaffolding (no GTest) and execute test cases from main(), printing results.

  Notes:
  - We declare the C function Help with C linkage: extern "C" void Help(int level);
  - Tests verify key substrings to ensure correct output is produced, and that exit status is 0.
  - This test suite is designed to be compiled together with jpgicc.c under a C++11 compiler.
*/

#include <iccjpeg.h>
#include <fcntl.h>
#include <jpeglib.h>
#include <vector>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <utils.h>


extern "C" void Help(int level); // Focal method under test (C linkage)

/*
  Utility to run Help(level) in a separate process and capture stderr output.
  Returns true if the child exited with status 0.
  The captured output is stored in 'out'.
*/
bool RunHelpInChild(int level, std::string& out, int& exitStatus) {
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        std::cerr << "Failed to create pipe for inter-process communication.\n";
        return false;
    }

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Fork failed.\n";
        return false;
    }

    if (pid == 0) {
        // Child process
        // Redirect stderr to pipe
        dup2(pipefd[1], STDERR_FILENO);
        // Close unused ends
        close(pipefd[0]);
        close(pipefd[1]);

        // Execute the focal method
        // It is expected to terminate via exit(0)
        Help(level);

        // If Help returns (which it shouldn't), exit with non-zero to signal failure
        _exit(1);
    } else {
        // Parent process
        // Close write end; only read from the pipe
        close(pipefd[1]);

        // Read output from the child
        char buffer[4096];
        std::string collected;
        ssize_t n;
        while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            collected.append(buffer, buffer + n);
        }
        close(pipefd[0]);

        // Wait for child to finish
        int status = 0;
        if (waitpid(pid, &status, 0) == -1) {
            std::cerr << "Failed to wait for child process.\n";
            return false;
        }

        exitStatus = status;
        out = collected;
        // A successful exit is indicated by WIFEXITED and WEXITSTATUS == 0
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            return true;
        } else {
            return false;
        }
    }
}

// Simple test framework helpers
struct TestCase {
    const char* name;
    bool (*func)();
};

// Test 1: Validate that Help prints the usage line and some expected pieces of information
bool Test_Help_PrintsUsageAndKeyLines() {
    std::string output;
    int exitStatus = -1;
    bool ok = RunHelpInChild(0, output, exitStatus);
    if (!ok) {
        std::cerr << "[Test_Help_PrintsUsageAndKeyLines] Child did not exit with status 0.\n";
        std::cerr << "Captured output (partial):\n" << output << "\n";
        return false;
    }

    // Check critical substrings without requiring exact formatting
    // 1) The usage line
    if (output.find("usage: jpgicc [flags] input.jpg output.jpg") == std::string::npos) {
        std::cerr << "[Test_Help_PrintsUsageAndKeyLines] Missing usage line in output.\n";
        return false;
    }

    // 2) A representative flag line (verbose flag)
    if (output.find("-v - Verbose") == std::string::npos) {
        std::cerr << "[Test_Help_PrintsUsageAndKeyLines] Missing verbose flag description in output.\n";
        return false;
    }

    // 3) A hint that the program references Little CMS (as per the help text)
    if (output.find("Little CMS") == std::string::npos) {
        std::cerr << "[Test_Help_PrintsUsageAndKeyLines] Missing Little CMS text in output.\n";
        return false;
    }

    // 4) A hint that examples are present (optional but good to verify)
    if (output.find("Examples:") == std::string::npos && output.find("Examples:\n") == std::string::npos) {
        // Some variants may wrap differently, but it's still useful to require at least presence of "Examples"
        std::cerr << "[Test_Help_PrintsUsageAndKeyLines] Missing Examples section in output.\n";
        return false;
    }

    return true;
}

// Test 2: Validate that the help output includes the input/output profile flags
bool Test_Help_IncludesInputOutputFlags() {
    std::string output;
    int exitStatus = -1;
    bool ok = RunHelpInChild(0, output, exitStatus);
    if (!ok) {
        std::cerr << "[Test_Help_IncludesInputOutputFlags] Child did not exit with status 0.\n";
        std::cerr << "Captured output:\n" << output << "\n";
        return false;
    }

    // Check for input and output profile flags lines
    if (output.find("-i<profile> - Input profile (defaults to sRGB)") == std::string::npos) {
        std::cerr << "[Test_Help_IncludesInputOutputFlags] Missing -i<profile> description.\n";
        return false;
    }
    if (output.find("-o<profile> - Output profile (defaults to sRGB)") == std::string::npos) {
        std::cerr << "[Test_Help_IncludesInputOutputFlags] Missing -o<profile> description.\n";
        return false;
    }

    return true;
}

// Aggregator of tests
TestCase g_tests[] = {
    { "Test_Help_PrintsUsageAndKeyLines", &Test_Help_PrintsUsageAndKeyLines },
    { "Test_Help_IncludesInputOutputFlags", &Test_Help_IncludesInputOutputFlags },
    { nullptr, nullptr }
};

// Main function to run tests from the test suite
int main() {
    int failures = 0;

    // Execute each test case
    for (TestCase* t = g_tests; t->name != nullptr; ++t) {
        std::cout << "Running " << t->name << "..." << std::flush;
        bool result = t->func();
        if (result) {
            std::cout << " PASSED\n";
        } else {
            std::cout << " FAILED\n";
            ++failures;
        }
    }

    if (failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << failures << " test(s) failed.\n";
        return 1;
    }
}