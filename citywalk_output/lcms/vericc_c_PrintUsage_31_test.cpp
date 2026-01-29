// Test suite for PrintUsage() from vericc.c
// - No Google Test; a minimal in-process test harness is implemented.
// - The tests capture stderr output produced by PrintUsage and verify the exact
//   string content and the returned value.
// - This test suite is designed to be compiled alongside vericc.c (C file) in a
//   C++11 environment.
// - Run: g++ -std=c++11 tests.cpp vericc.c -o vericc_tests
// - Ensure a POSIX-like environment (pipe, dup2, read). Windows compatibility is not guaranteed.

#include <cstring>
#include <sys/socket.h>
#include <cstdio>
#include <math.h>
#include <iostream>
#include <string>
#include <sys/types.h>
#include <lcms2.h>
#include <unistd.h>
#include <string.h>


extern "C" int PrintUsage(void); // Function under test defined in vericc.c

// Helper: capture the output written to stderr by PrintUsage.
// It temporarily redirects STDERR_FILENO to a pipe, calls PrintUsage, then reads
// everything from the pipe and restores stderr.
int capturePrintUsage(std::string& capturedOutput) {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return -1;
    }

    // Save current stderr
    int savedStderr = dup(STDERR_FILENO);
    if (savedStderr == -1) {
        perror("dup");
        return -1;
    }

    // Redirect stderr to the write end of the pipe
    if (dup2(pipefd[1], STDERR_FILENO) == -1) {
        perror("dup2");
        return -1;
    }
    close(pipefd[1]); // Close the original write end, it's now dup'ed to stderr

    // Call the function under test
    int ret = PrintUsage();

    // Read all data from the read end of the pipe
    std::string output;
    char buffer[512];
    ssize_t n;
    while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, static_cast<size_t>(n));
    }

    // Cleanup: restore stderr and close file descriptors
    dup2(savedStderr, STDERR_FILENO);
    close(savedStderr);
    close(pipefd[0]);

    capturedOutput = output;
    return ret;
}

// Test 1: Verify that PrintUsage outputs the exact expected string to stderr
// and returns 0.
bool Test_PrintUsage_BasicOutput() {
    // Expected output exactly as in vericc.c
    const char* expected = "Sets profile version\n\nUsage: vericc --r<version> iccprofile.icc\n";

    std::string captured;
    int ret = capturePrintUsage(captured);

    if (ret != 0) {
        std::cerr << "[Test_PrintUsage_BasicOutput] FAILED: Return value expected 0 but got " << ret << "\n";
        return false;
    }

    if (captured != expected) {
        std::cerr << "[Test_PrintUsage_BasicOutput] FAILED: Output mismatch.\n";
        std::cerr << "  Expected: [" << expected << "]\n";
        std::cerr << "  Actual  : [" << captured << "]\n";
        return false;
    }

    std::cout << "[Test_PrintUsage_BasicOutput] PASSED\n";
    return true;
}

// Test 2: Verify that repeated calls to PrintUsage produce identical output
// and identical return values (i.e., repeatability of the function).
bool Test_PrintUsage_Repeatable() {
    const char* expected = "Sets profile version\n\nUsage: vericc --r<version> iccprofile.icc\n";

    std::string out1;
    int ret1 = capturePrintUsage(out1);

    std::string out2;
    int ret2 = capturePrintUsage(out2);

    if (ret1 != 0 || ret2 != 0) {
        std::cerr << "[Test_PrintUsage_Repeatable] FAILED: Return values expected 0, got "
                  << ret1 << " and " << ret2 << "\n";
        return false;
    }

    if (out1 != expected || out2 != expected) {
        std::cerr << "[Test_PrintUsage_Repeatable] FAILED: Output mismatch on repeatable calls.\n";
        std::cerr << "  First output : [" << out1 << "]\n";
        std::cerr << "  Second output: [" << out2 << "]\n";
        return false;
    }

    // Extra assertion: both outputs must be identical to each other
    if (out1 != out2) {
        std::cerr << "[Test_PrintUsage_Repeatable] FAILED: Outputs differ between calls.\n";
        return false;
    }

    std::cout << "[Test_PrintUsage_Repeatable] PASSED\n";
    return true;
}

// Entry point for the test suite
int main() {
    int totalFailed = 0;

    if (!Test_PrintUsage_BasicOutput()) {
        std::cout << "[Test] Test_PrintUsage_BasicOutput: FAILED\n";
        ++totalFailed;
    }

    if (!Test_PrintUsage_Repeatable()) {
        std::cout << "[Test] Test_PrintUsage_Repeatable: FAILED\n";
        ++totalFailed;
    }

    if (totalFailed == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << totalFailed << " TEST(S) FAILED\n";
        return totalFailed;
    }
}