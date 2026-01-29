// C++11 test harness for the focal function: testCaseStart
// This harness does not use GTest. It provides small, non-terminating
// assertions and a minimal test runner to exercise the C-style variadic
// function from the provided C source (lsmtest2.c).
//
// Focal method under test:
// void testCaseStart(int *pRc, char *zFmt, ...){
//
//   va_list ap;
//   va_start(ap, zFmt);
//   vprintf(zFmt, ap);
//   printf(" ...");
//   va_end(ap);
//   *pRc = 0;
//   fflush(stdout);
// }
//
// Notes:
// - The function prints formatted output to stdout via vprintf, then appends " ..."
// - It sets the provided rc pointer to 0
// - We capture stdout to verify the exact output and rc behavior
// - We link against the C implementation (extern "C") when compiling with the project

#include <lsmtest.h>
#include <vector>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>


// Declaration of the focal function (from lsmtest2.c)
// The function is C-style with variadic arguments; ensure proper linkage.
extern "C" void testCaseStart(int *pRc, char *zFmt, ...);

static bool starts_with(const std::string &s, const std::string &prefix) {
    return s.substr(0, prefix.size()) == prefix;
}

// Simple non-terminating assertion helper
static void assert_true(bool condition, const char *message, int &failCount) {
    if (!condition) {
        std::cerr << "ASSERT FAIL: " << message << "\n";
        ++failCount;
    }
}

// Capture stdout produced by calling testCaseStart with a specific format and arguments.
// Returns the captured output string.
static std::string captureOutputWithTestCaseStart(int *outRc, const char *fmt, ...) {
    // Setup pipe to capture stdout
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // If pipe creation fails, return empty output
        return "";
    }

    // Save current stdout
    int savedStdout = dup(fileno(stdout));
    if (savedStdout == -1) {
        // Cleanup
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Redirect stdout to pipe's write end
    fflush(stdout);
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        // Cleanup
        close(pipefd[0]);
        close(pipefd[1]);
        close(savedStdout);
        return "";
    }
    // We no longer need the duplicate write end in parent
    close(pipefd[1]);

    // Call the focal function with the provided arguments
    int rc = -1;
    va_list ap;
    va_start(ap, fmt);
    // We cannot forward the va_list directly to testCaseStart (it's variadic),
    // so we use a local approach by re-invoking with a single known argument(s)
    // based on the fmt content. To keep the wrapper generic, we use a small
    // fixed set in tests; this function is used by specific tests that pass
    // the appropriate arguments explicitly (see test wrappers below).
    // NOTE: In this harness we do not use this generic function; tests call
    // into testCaseStart directly with explicit arguments. This function is
    // kept for potential extension and to illustrate how one would capture
    // output for a variadic call.
    (void)ap;
    va_end(ap);

    // The actual invocation in tests will be done directly (see individual tests below).
    // This placeholder ensures the interface remains available if needed.

    // Restore stdout
    fflush(stdout);
    if (dup2(savedStdout, fileno(stdout)) == -1) {
        // Ignore restoration errors
    }
    close(savedStdout);

    // Read from pipe
    // Small non-blocking read with a single read; the test calls ensure output is short.
    char buffer[1024];
    ssize_t nread = read(pipefd[0], buffer, sizeof(buffer) - 1);
    if (nread < 0) nread = 0;
    buffer[nread] = '\0';
    close(pipefd[0]);

    // Return captured string
    return std::string(buffer);
}

// Test 1: Verify basic int formatting and trailing " ..."
static bool test_caseStart_IntFormat_output_matches() {
    // Redirect stdout and call testCaseStart with a simple int argument
    int rc = -1;

    // Set up capture
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        std::cerr << "Failed to create pipe for test1\n";
        return false;
    }
    int savedStdout = dup(fileno(stdout));
    if (savedStdout == -1) {
        std::cerr << "Failed to dup stdout for test1\n";
        close(pipefd[0]); close(pipefd[1]);
        return false;
    }

    fflush(stdout);
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        std::cerr << "Failed to redirect stdout for test1\n";
        close(pipefd[0]); close(pipefd[1]); close(savedStdout);
        return false;
    }
    close(pipefd[1]); // writer end no longer needed in this thread

    // Invoke focal function with known inputs
    testCaseStart(&rc, "Hello %d", 42);

    // Restore stdout
    fflush(stdout);
    if (dup2(savedStdout, fileno(stdout)) == -1) {
        std::cerr << "Failed to restore stdout for test1\n";
        // Continue to attempt to read output
    }
    close(savedStdout);

    // Read captured output
    char buffer[1024];
    ssize_t n = read(pipefd[0], buffer, sizeof(buffer) - 1);
    close(pipefd[0]);
    buffer[n] = '\0';
    std::string output(buffer);

    // Expectations:
    // - rc should be set to 0
    // - output should contain "Hello 42 ..." pattern
    bool pass = true;
    pass &= (rc == 0);
    pass &= (output.find("Hello 42") != std::string::npos);
    pass &= (output.find(" ...") != std::string::npos);

    // Explanatory assertion
    if (!pass) {
        std::cerr << "test_caseStart_IntFormat_output_matches FAILED. rc=" << rc
                  << " output=\"" << output << "\"\n";
    }
    return pass;
}

// Test 2: Verify string formatting and trailing " ..."
static bool test_caseStart_StringFormat_output_matches() {
    int rc = -1;

    int pipefd[2];
    if (pipe(pipefd) != 0) {
        std::cerr << "Failed to create pipe for test2\n";
        return false;
    }
    int savedStdout = dup(fileno(stdout));
    if (savedStdout == -1) {
        std::cerr << "Failed to dup stdout for test2\n";
        close(pipefd[0]); close(pipefd[1]);
        return false;
    }

    fflush(stdout);
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        std::cerr << "Failed to redirect stdout for test2\n";
        close(pipefd[0]); close(pipefd[1]); close(savedStdout);
        return false;
    }
    close(pipefd[1]);

    testCaseStart(&rc, "Msg: %s", "world");

    fflush(stdout);
    if (dup2(savedStdout, fileno(stdout)) == -1) {
        std::cerr << "Failed to restore stdout for test2\n";
    }
    close(savedStdout);

    char buffer[1024];
    ssize_t n = read(pipefd[0], buffer, sizeof(buffer) - 1);
    close(pipefd[0]);
    buffer[n] = '\0';
    std::string output(buffer);

    bool pass = true;
    pass &= (rc == 0);
    pass &= (output.find("Msg: world") != std::string::npos);
    pass &= (output.find(" ...") != std::string::npos);

    if (!pass) {
        std::cerr << "test_caseStart_StringFormat_output_matches FAILED. rc=" << rc
                  << " output=\"" << output << "\"\n";
    }
    return pass;
}

// Test 3: Verify plain formatting without additional arguments
static bool test_caseStart_PlainFormat_output_matches() {
    int rc = -1;

    int pipefd[2];
    if (pipe(pipefd) != 0) {
        std::cerr << "Failed to create pipe for test3\n";
        return false;
    }
    int savedStdout = dup(fileno(stdout));
    if (savedStdout == -1) {
        std::cerr << "Failed to dup stdout for test3\n";
        close(pipefd[0]); close(pipefd[1]);
        return false;
    }

    fflush(stdout);
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        std::cerr << "Failed to redirect stdout for test3\n";
        close(pipefd[0]); close(pipefd[1]); close(savedStdout);
        return false;
    }
    close(pipefd[1]);

    testCaseStart(&rc, "Plain");

    fflush(stdout);
    if (dup2(savedStdout, fileno(stdout)) == -1) {
        std::cerr << "Failed to restore stdout for test3\n";
    }
    close(savedStdout);

    char buffer[1024];
    ssize_t n = read(pipefd[0], buffer, sizeof(buffer) - 1);
    close(pipefd[0]);
    buffer[n] = '\0';
    std::string output(buffer);

    bool pass = true;
    pass &= (rc == 0);
    pass &= (output.find("Plain") != std::string::npos);
    pass &= (output.find(" ...") != std::string::npos);

    if (!pass) {
        std::cerr << "test_caseStart_PlainFormat_output_matches FAILED. rc=" << rc
                  << " output=\"" << output << "\"\n";
    }
    return pass;
}

// Test 4: Verify that the rc value is reset to 0 regardless of its initial value
static bool test_caseStart_rcResetToZero() {
    int rc = 123; // initial non-zero value

    int pipefd[2];
    if (pipe(pipefd) != 0) {
        std::cerr << "Failed to create pipe for test4\n";
        return false;
    }
    int savedStdout = dup(fileno(stdout));
    if (savedStdout == -1) {
        std::cerr << "Failed to dup stdout for test4\n";
        close(pipefd[0]); close(pipefd[1]);
        return false;
    }

    fflush(stdout);
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        std::cerr << "Failed to redirect stdout for test4\n";
        close(pipefd[0]); close(pipefd[1]); close(savedStdout);
        return false;
    }
    close(pipefd[1]);

    testCaseStart(&rc, "RcCheck %d", 1);

    fflush(stdout);
    if (dup2(savedStdout, fileno(stdout)) == -1) {
        std::cerr << "Failed to restore stdout for test4\n";
    }
    close(savedStdout);

    char buffer[1024];
    ssize_t n = read(pipefd[0], buffer, sizeof(buffer) - 1);
    close(pipefd[0]);
    buffer[n] = '\0';
    std::string output(buffer);

    bool pass = true;
    pass &= (rc == 0);
    pass &= (output.find("RcCheck 1") != std::string::npos);
    pass &= (output.find(" ...") != std::string::npos);

    if (!pass) {
        std::cerr << "test_caseStart_rcResetToZero FAILED. rc=" << rc
                  << " output=\"" << output << "\"\n";
    }
    return pass;
}

int main() {
    // Simple test runner
    std::vector<std::string> testNames;
    std::vector<bool> results;

    // Run tests
    bool t1 = test_caseStart_IntFormat_output_matches();
    testNames.push_back("test_caseStart_IntFormat_output_matches");
    results.push_back(t1);

    bool t2 = test_caseStart_StringFormat_output_matches();
    testNames.push_back("test_caseStart_StringFormat_output_matches");
    results.push_back(t2);

    bool t3 = test_caseStart_PlainFormat_output_matches();
    testNames.push_back("test_caseStart_PlainFormat_output_matches");
    results.push_back(t3);

    bool t4 = test_caseStart_rcResetToZero();
    testNames.push_back("test_caseStart_rcResetToZero");
    results.push_back(t4);

    // Summary
    int failCount = 0;
    for (size_t i = 0; i < results.size(); ++i) {
        if (!results[i]) ++failCount;
    }

    std::cout << "Test summary: " << (results.size() - failCount)
              << " passed, " << failCount << " failed.\n";

    // Optional: print detailed per-test results
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << (results[i] ? "[PASS] " : "[FAIL] ")
                  << testNames[i] << "\n";
    }

    return (failCount == 0) ? 0 : 1;
}