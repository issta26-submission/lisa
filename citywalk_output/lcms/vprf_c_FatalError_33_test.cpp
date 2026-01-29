// Unit test suite for the focal method FatalError in vprf.c
// This test harness uses process isolation (fork) to safely test
// a function that terminates the process via exit(1).
// Each test runs in a separate child process with stderr redirected
// to capture the exact output produced by FatalError.
//
// Assumptions:
// - FatalError is defined in vprf.c and linked into the test binary.
// - InitUtils(const char* PName) sets the ProgramName used by FatalError.
// - The tests are compiled with C++11 support.
//
// Important: This test harness does not use GTest; it uses a custom
// lightweight test harness with explicit assertions and reporting.

#include <cstring>
#include <cstdio>
#include <iostream>
#include <string>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstdlib>
#include <unistd.h>
#include <utils.h>


// Declarations of the focal function and its dependency as provided in the codebase.
extern "C" void InitUtils(const char* PName);
extern "C void FatalError(const char* frm, ...);

// Test case 1:
// - Sets ProgramName to "TestProg1".
// - Calls FatalError with a simple message "test".
// - Expected output: [TestProg1 fatal error]: test\n and exit status 1.
static void test_case_1() {
    InitUtils("TestProg1");
    FatalError("test");
}

// Test case 2:
// - Sets ProgramName to "TestProg2".
// - Calls FatalError with an integer value using format "Value=%d".
// - Expected output: [TestProg2 fatal error]: Value=42\n and exit status 1.
static void test_case_2() {
    InitUtils("TestProg2");
    FatalError("Value=%d", 42);
}

// Test case 3:
// - Sets ProgramName to "TestProg3".
// - Calls FatalError with a string and a number using format "Msg: %s, %d".
// - Expected output: [TestProg3 fatal error]: Msg: alpha, 3\n and exit status 1.
static void test_case_3() {
    InitUtils("TestProg3");
    FatalError("Msg: %s, %d", "alpha", 3);
}

// Structure to capture the results of a single test run (in a child process).
struct ChildResult {
    int id;                     // Test id (1..3)
    std::string output;         // Captured stderr output
    int exitStatus;               // Exit status reported by the child
    bool exitedNormally;          // Whether the child exited normally (WIFEXITED)
};

// Run a specific test function in a separate child process and capture stderr.
static ChildResult runChild(int id, void (*testFunc)()) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        std::exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        std::exit(1);
    }

    if (pid == 0) {
        // Child process
        // Redirect stderr to the pipe's write end
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);

        // Run the test case function which will call FatalError
        testFunc();

        // If FatalError unexpectedly returns, exit cleanly.
        _exit(0);
    } else {
        // Parent process
        close(pipefd[1]); // Close write end; read from the read end
        char buffer[4096];
        std::string output;
        ssize_t n;
        while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
            output.append(buffer, buffer + n);
        }
        close(pipefd[0]);

        int status = 0;
        waitpid(pid, &status, 0);

        ChildResult cr;
        cr.id = id;
        cr.output = output;
        cr.exitedNormally = WIFEXITED(status);
        cr.exitStatus = cr.exitedNormally ? WEXITSTATUS(status) : -1;
        return cr;
    }
}

// Simple helper to print a concise test result with optional diagnostic details.
static void printResult(const ChildResult& r, const std::string& expected, bool forceShowDiff = false) {
    bool pass = r.exitedNormally && (r.exitStatus == 1) && (r.output == expected);
    std::cout << "Test " << r.id << ": " << (pass ? "PASS" : "FAIL") << std::endl;
    if (!pass || forceShowDiff) {
        std::cout << "  Output  = \"" << r.output << "\"" << std::endl;
        std::cout << "  Expected= \"" << expected << "\"" << std::endl;
        if (r.exitedNormally) {
            std::cout << "  ExitStatus=" << r.exitStatus << " (exited normally)" << std::endl;
        } else {
            std::cout << "  ExitStatus=" << r.exitStatus << " (did not exit normally)" << std::endl;
        }
    }
}

int main() {
    // Run each test in isolation (child process) and capture their stderr output
    // and exit status to validate the FatalError behavior.

    // Test 1
    ChildResult r1 = runChild(1, test_case_1);
    // Expected output for test 1
    std::string exp1 = "[TestProg1 fatal error]: test\n";

    // Test 2
    ChildResult r2 = runChild(2, test_case_2);
    // Expected output for test 2
    std::string exp2 = "[TestProg2 fatal error]: Value=42\n";

    // Test 3
    ChildResult r3 = runChild(3, test_case_3);
    // Expected output for test 3
    std::string exp3 = "[TestProg3 fatal error]: Msg: alpha, 3\n";

    // Print results
    // Explanatory comments accompany each assertion as per the test plan.
    printResult(r1, exp1);
    printResult(r2, exp2);
    printResult(r3, exp3);

    return 0;
}