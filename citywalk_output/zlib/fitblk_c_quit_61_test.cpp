/*
Step 1 - Candidate Keywords (from the focal method)
- quit: the focal function under test
- fprintf, stderr: used for output before abort
- "fitblk abort: %s\n": the exact abort message format
- why: parameter to printf
- exit: aborts program with status 1
- char*: argument type for quit
- zlib, partcompress, recompress, main: class dependencies listed but not exercised directly here
Notes:
- The focal function quit(char* why) unconditionally prints an abort message and exits with status 1.
- In tests we override exit to prevent actual termination and to capture the exit status.
- We capture stderr to validate the exact abort message formatting.
- This test harness uses no GTest; tests are invoked from main().
*/

#include <cstring>
#include <fcntl.h>
#include <setjmp.h>
#include <assert.h>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>


// Forward declaration of the focal function under test.
// We assume external linkage for quit(char*).
extern "C" void quit(char* why);

// DOMAIN KNOWLEDGE GUIDANCE:
//
// - Use non-terminating assertions (do not call std::abort or exit on failure).
// - Tests are driven from main() as GTest is not allowed.
// - Redirect and capture STDERR to verify the exact output format.
// - Override exit with a C linkage function to prevent process termination.
// - Ensure compatibility with C++11 standard library only.


// Global state used by the overridden exit() to communicate with tests.
static jmp_buf test_env;          // environment used for non-local goto (longjmp)
static int captured_exit_status;    // status passed to exit()

// Override the C library exit() to prevent the test process from terminating.
// We provide C linkage to ensure it matches the standard library symbol resolution.
extern "C" void exit(int status)
{
    captured_exit_status = status;
    longjmp(test_env, 1); // jump back to the test harness after quit() calls exit(1)
}


// Helper function: Run a single quit() test case with the given reason.
// Captures stderr output and exit status. Returns true if the observed
// behavior matches the expected message and exit status.
static bool runQuitTest(const std::string& reason, std::string& capturedOutput)
{
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // Unable to create pipe; consider test as failed.
        return false;
    }

    // Redirect stderr to the write end of the pipe.
    int savedStderr = dup(STDERR_FILENO);
    if (dup2(pipefd[1], STDERR_FILENO) == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return false;
    }
    close(pipefd[1]); // write end duplicated to STDERR now; close original

    capturedOutput.clear();
    captured_exit_status = -1;

    // Execute quit() in a context where exit() is overridden to longjmp back.
    if (setjmp(test_env) == 0) {
        // Call the focal function with the provided reason.
        quit((char*)reason.c_str());

        // If quit() returns (which it shouldn't under normal behavior), restore streams.
        dup2(savedStderr, STDERR_FILENO);
        close(savedStderr);
        // Read any output that might have been produced (if the function behaved unexpectedly).
        // We still return false to indicate failure.
        char placeholder[1];
        ssize_t n = 0;
        while ((n = read(STDERR_FILENO, placeholder, sizeof(placeholder))) > 0) {
            // discard
        }
        close(pipefd[0]);
        return false;
    } else {
        // Returned here via longjmp from our exit() override after quit() called exit(1)
        // Restore stderr to its original state
        dup2(savedStderr, STDERR_FILENO);
        close(savedStderr);

        // Read captured stderr content from the pipe
        const size_t BUFSIZE = 512;
        char buf[BUFSIZE];
        ssize_t r;
        while ((r = read(pipefd[0], buf, BUFSIZE)) > 0) {
            capturedOutput.append(buf, (size_t)r);
        }
        close(pipefd[0]);

        // Expected pattern: "fitblk abort: <reason>\n"
        std::string expected = std::string("fitblk abort: ") + reason + "\n";

        // Check that the captured output matches exactly and the exit status is 1
        bool outputMatches = (capturedOutput == expected);
        bool statusMatches = (captured_exit_status == 1);

        return outputMatches && statusMatches;
    }
}


// Test 1: Basic reason string
// Verifies the exact abort message formatting and exit status when a non-empty reason is provided.
static bool test_quit_basic_reason()
{
    std::string output;
    bool result = runQuitTest("test reason", output);
    if (!result) {
        std::cerr << "Test 1 FAILED: Expected exact message \"fitblk abort: test reason\\n\" and exit status 1.\n";
        std::cerr << "Captured output: " << output << "\n";
        std::cerr << "Captured exit status: " << captured_exit_status << "\n";
    } else {
        std::cout << "Test 1 PASSED: Correct abort message and exit status.\n";
    }
    return result;
}


// Test 2: Empty reason string
// Verifies the exact abort message formatting when the reason is an empty string.
static bool test_quit_empty_reason()
{
    std::string output;
    bool result = runQuitTest("", output);
    if (!result) {
        std::cerr << "Test 2 FAILED: Expected exact message \"fitblk abort: \\n\" and exit status 1.\n";
        std::cerr << "Captured output: " << output << "\n";
        std::cerr << "Captured exit status: " << captured_exit_status << "\n";
    } else {
        std::cout << "Test 2 PASSED: Correct abort message for empty reason and exit status.\n";
    }
    return result;
}


// Step 3 - Test Runner (main)
// Calls all tests from main() to comply with non-GTest environments.
// Aggregates results and returns non-zero on any failure.
int main()
{
    // Run tests in sequence
    bool t1 = test_quit_basic_reason();
    bool t2 = test_quit_empty_reason();

    bool allPassed = t1 && t2;
    if (allPassed) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cerr << "Some tests FAILED.\n";
        return 1;
    }
}