// Test suite for the focal method Dot() found in testcms2.c
// This file provides a small, self-contained test harness (no GTest) using only the C++ standard library.
// It uses a POSIX pipe-based approach to capture stdout output produced by the C function Dot().
// The tests verify that Dot() prints a single dot '.' per invocation and that multiple invocations produce the correct sequence "....".
// The Dot() function is declared with C linkage to ensure proper linkage when compiled together with testcms2.c.

#include <cstring>
#include <cstdio>
#include <fcntl.h>
#include <vector>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <unistd.h>


extern "C" void Dot(void); // Declaration of the focal method (from testcms2.c)


// Helper: capture the output produced by calling Dot() 'n' times.
// Uses POSIX pipe to redirect stdout temporarily.
static std::string capture_dot_output(int n) {
    // Create a pipe to capture stdout
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // If pipe creation fails, return empty string to indicate failure
        return "";
    }

    // Save current stdout
    int saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1) {
        // Cleanup and return
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Redirect stdout to the write end of the pipe
    if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
        // Cleanup
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdout);
        return "";
    }
    // Close the now-duplicated write end in this process; it's handled by stdout now
    close(pipefd[1]);

    // Call Dot() n times
    for (int i = 0; i < n; ++i) {
        Dot();
    }
    // Ensure all output is flushed
    fflush(stdout);

    // Restore original stdout
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    // Read captured output from the read end
    std::string result;
    char buffer[256];
    ssize_t r;
    // The read end might have data ready immediately; loop until EOF
    while ((r = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        result.append(buffer, static_cast<size_t>(r));
    }

    // Cleanup
    close(pipefd[0]);

    return result;
}

// Simple test utilities
static bool assert_equal(std::string actual, const std::string& expected, const char* test_desc) {
    if (actual == expected) {
        std::cout << "[PASS] " << test_desc << "\n";
        return true;
    } else {
        std::cerr << "[FAIL] " << test_desc << "\n";
        std::cerr << "  Expected: \"" << expected << "\"\n";
        std::cerr << "  Actual  : \"" << actual << "\"\n";
        return false;
    }
}

// Candidate keywords for Step 1 (documented for traceability)
// - Dot: focal method under test
// - stdout / fprintf: IO mechanisms used by Dot
// - fflush: ensures output is flushed
// - testcms2.c: source providing Dot and related test scaffolding dependencies
// Note: The static helper functions in testcms2.c are not accessible from here due to file scope (static),
// but Dot() is non-static and accessible via C linkage.

static bool testDotSingle() {
    // true contract: a single invocation prints exactly "."
    std::string output = capture_dot_output(1);
    return assert_equal(output, ".", "Dot() prints a single '.' on a single invocation");
}

static bool testDotTriple() {
    // true contract: three sequential invocations print "..."
    std::string output = capture_dot_output(3);
    return assert_equal(output, "...", "Dot() prints three sequential '.' characters on three invocations");
}

static bool testDotMultipleInvocationsAccumulation() {
    // Another check: interleaved separate captures should each reflect the exact counts
    std::string a = capture_dot_output(2);
    bool okA = assert_equal(a, "..", "Dot() across 2 invocations yields '..'");

    std::string b = capture_dot_output(1);
    bool okB = assert_equal(b, ".", "Dot() across 1 invocation yields '.'");

    return okA && okB;
}

int main() {
    // Step 2/3: Execute the suite and report a concise summary
    std::cout << "Running Dot() unit tests (no GTest, C linkage):\n";

    int total = 0;
    int passed = 0;

    auto runTest = [&](bool (*fn)(), const char* name) {
        ++total;
        bool result = fn();
        if (result) ++passed;
        else {
            // In case of failure, print a concise diagnostic line
            std::cerr << "[DIAG] Test failed: " << name << "\n";
        }
        return result;
    };

    runTest([]{ return testDotSingle(); }, "Dot.SingleInvocation");
    runTest([]{ return testDotTriple(); }, "Dot.TripleInvocation");
    runTest([]{ return testDotMultipleInvocationsAccumulation(); }, "Dot.MultipleInvocationsAccumulation");

    std::cout << "Tests run: " << total << ", Passed: " << passed << ", Failed: " << (total - passed) << "\n";

    return (passed == total) ? 0 : 1;
}