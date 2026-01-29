/*
Unit test suite for the focal method: testPrintFUsage

Context:
- The focal function prints a usage line to stderr using a global g.argv[0] and g.argv[1], followed by a variadic formatted string and a newline.
- We test multiple scenarios to ensure correct formatting, integration with g.argv, and proper newline handling.
- No GTest; a small in-process test runner is implemented with manual expectations and a simple PASS/FAIL reporting.
- Tests redirect stderr to capture output for verification.

Notes:
- We rely on the provided lsmtest.h which defines the global g and the C function:
  extern "C" void testPrintFUsage(const char *zFormat, ...);
- The test suite uses POSIX pipes to capture stderr output safely within a single process.
- We explicitly set g.argv[0] and g.argv[1] to known values ("prog" and "arg") for deterministic output.
*/

#include <string.h>
#include <lsmtest.h>
#include <sys/types.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>



extern "C" void testPrintFUsage(const char *zFormat, ...); // C variadic function under test

// Forward declarations of small wrappers that call the focal function with specific arguments.
// Each wrapper also sets g.argv to deterministic test values.
static void t1_call(void); // Hello (no format specifiers)
static void t2_call(void); // "%s %d" with "A", 7
static void t3_call(void); // "Value=%d" with 42
static void t4_call(void); // "Mixed: %s and %d" with "X", 99

// Capture the output of a given wrapper by redirecting stderr to a pipe.
static std::string captureWithWrapper(void (*wrapper)()) {
    // Prepare capture
    int savedStderr = dup(fileno(stderr));
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        perror("pipe");
        return "";
    }

    // Redirect stderr to pipe's write end
    dup2(pipefd[1], fileno(stderr));
    close(pipefd[1]); // write end is now duplicated to stderr

    // Call the wrapper (which will invoke testPrintFUsage)
    wrapper();

    // Restore stderr
    fflush(stderr);
    dup2(savedStderr, fileno(stderr));
    close(savedStderr);

    // Read captured output from pipe's read end
    std::string output;
    char buf[1024];
    ssize_t n;
    while ((n = read(pipefd[0], buf, sizeof(buf))) > 0) {
        output.append(buf, static_cast<size_t>(n));
    }
    close(pipefd[0]);
    return output;
}

// Helper: Set g.argv to deterministic values for tests
static void setGArgvForTests() {
    // The actual types depend on lsmtest.h; assume argv is modifiable as char*
    if (g.argv[0] != nullptr) g.argv[0] = (char*)"prog";
    if (g.argv[1] != nullptr) g.argv[1] = (char*)"arg";
}

// Simple non-terminating assertion to report PASS/FAIL while continuing execution.
static bool expectEquals(const std::string& expected, const std::string& actual, const char* testName) {
    bool ok = (expected == actual);
    if (ok) {
        std::cout << "[PASS] " << testName << std::endl;
    } else {
        std::cout << "[FAIL] " << testName << std::endl
                  << "  Expected: \"" << expected << "\"\n"
                  << "  Actual  : \"" << actual << "\"\n";
    }
    return ok;
}

// Implementations of test wrappers

static void t1_call(void) {
    // Case 1: No variadic arguments, simple string
    setGArgvForTests();
    testPrintFUsage("Hello");
}

static void t2_call(void) {
    // Case 2: One string and one int
    setGArgvForTests();
    testPrintFUsage("%s %d", "A", 7);
}

static void t3_call(void) {
    // Case 3: Integer only
    setGArgvForTests();
    testPrintFUsage("Value=%d", 42);
}

static void t4_call(void) {
    // Case 4: String and int with a more complex format
    setGArgvForTests();
    testPrintFUsage("Mixed: %s and %d", "X", 99);
}

// Test runners for each case
static std::string runCase1() { return captureWithWrapper(t1_call); }
static std::string runCase2() { return captureWithWrapper(t2_call); }
static std::string runCase3() { return captureWithWrapper(t3_call); }
static std::string runCase4() { return captureWithWrapper(t4_call); }

// Main test entry
int main() {
    // Expected outputs based on the focal implementation:
    // Output format: "Usage: %s %s " + formatted_args + "\n"
    // g.argv[0] = "prog", g.argv[1] = "arg"
    const std::string expected1 = "Usage: prog arg Hello\n";
    const std::string expected2 = "Usage: prog arg A 7\n";
    const std::string expected3 = "Usage: prog arg Value=42\n";
    const std::string expected4 = "Usage: prog arg Mixed: X and 99\n";

    // Run tests
    bool p1 = expectEquals(expected1, runCase1(), "testPrintFUsage Case 1 (no format specifiers)");
    bool p2 = expectEquals(expected2, runCase2(), "testPrintFUsage Case 2 (string and int)");
    bool p3 = expectEquals(expected3, runCase3(), "testPrintFUsage Case 3 (int only)");
    bool p4 = expectEquals(expected4, runCase4(), "testPrintFUsage Case 4 (string and int with complex format)");

    // Return non-zero if any test failed
    int failures = 0;
    if (!p1) ++failures;
    if (!p2) ++failures;
    if (!p3) ++failures;
    if (!p4) ++failures;

    return (failures == 0) ? 0 : 1;
}

/*
Explanatory notes for the tests:
- Test 1: Verifies basic print without variadic args. Ensures the fixed prefix and newline are present.
- Test 2: Verifies handling of a format with a string and an integer, ensuring correct spacing between formatted arguments.
- Test 3: Verifies handling of a format that only uses an integer, ensuring proper formatting without additional strings.
- Test 4: Verifies a more complex format with both a string and an integer to confirm correct integration of multiple specifiers.
- All tests capture stderr output to ensure deterministic verification without interfering with stdout.
*/