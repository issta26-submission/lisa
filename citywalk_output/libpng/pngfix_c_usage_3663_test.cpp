// Test suite for the focal method: usage(const char *prog) from pngfix.c
// This test suite is written in C++11 (no GoogleTest) and uses a small
// dynamic capturing harness to verify stdout output produced by the C function.
// It aims to exercise basic non-crashing behavior and verify presence of
// expected usage sections in the output.
//
// Notes:
// - The tests assume the pngfix.c (and its dependencies) can be compiled together
//   with this test file in the same build unit or linked appropriately.
// - We capture stdout by temporarily redirecting file descriptor 1 to a pipe.
// - We do not modify the production code; we only call usage(prog) with different
//   arguments and verify the produced textual output.
//
// Candidate Keywords (Step 1):
// - usage: the focal function under test
// - PROGRAM_NAME: defined as "pngfix" inside pngfix.c
// - DESCRIPTION, OPTIONS, EXIT CODES, MESSAGES: sections expected in usage output
// - stdout/stderr: I/O streams used by the usage output
// - libpng/pngfix dependencies: implicit in the surrounding code, but not directly tested here
//
// Step 2/3: Unit Test Generation and Refinement are implemented together below
// as a lightweight, non-GTest test harness focusing on output content coverage.

#include <functional>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>
#include <vector>
#include <limits.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <stdlib.h>
#include <errno.h>
#include <setjmp.h>
#include <string.h>
#include <string>
#include <zlib.h>
#include <iostream>
#include <cstring>
#include <ctype.h>


// Declaration of the focal function from pngfix.c
extern "C" void usage(const char *prog);

// Helper: capture stdout output produced by calling a function that uses stdout.
// We redirect stdout to a pipe, invoke the function, then read the produced data.
static std::string capture_usage_output(const char* prog) {
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // Pipe creation failed; return empty output to indicate failure gracefully.
        return "";
    }

    // Save current stdout
    int saved_stdout = dup(fileno(stdout));
    if (saved_stdout == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Redirect stdout to the write end of the pipe
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        // Cleanup
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdout);
        return "";
    }
    // Close the now-duplicated write end in this process
    close(pipefd[1]);

    // Call the focal function which writes to stdout
    usage(prog);
    fflush(stdout);

    // Read the pipe contents
    std::string output;
    char buffer[4096];
    ssize_t r;
    // The reader should be independent of the writer; keep reading until EOF
    while ((r = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, static_cast<size_t>(r));
    }

    // Cleanup: restore stdout and close fds
    close(pipefd[0]);
    dup2(saved_stdout, fileno(stdout));
    close(saved_stdout);

    return output;
}

// Test 1: Basic presence of key sections in usage output
// Expect that usage output contains:
 // - "OPTIONS"
 // - "EXIT CODES"
 // - "DESCRIPTION"
static bool test_usage_contains_key_sections() {
    std::string out = capture_usage_output("pngfix");
    if (out.empty()) {
        std::cerr << "[Test1] Failed: usage output is empty.\n";
        return false;
    }

    if (out.find("OPTIONS") == std::string::npos) {
        std::cerr << "[Test1] Failed: usage output missing 'OPTIONS' section.\n";
        return false;
    }
    if (out.find("EXIT CODES") == std::string::npos) {
        std::cerr << "[Test1] Failed: usage output missing 'EXIT CODES' section.\n";
        return false;
    }
    if (out.find("DESCRIPTION") == std::string::npos) {
        std::cerr << "[Test1] Failed: usage output missing 'DESCRIPTION' section.\n";
        return false;
    }

    // If we reach here, the expected sections are present.
    return true;
}

// Test 2: Consistency across different program names
// The usage output should be produced for different 'prog' values; content
// should still include the main sections (no crash, no empty output).
static bool test_usage_with_different_prog_names() {
    std::string out1 = capture_usage_output("pngfix");
    std::string out2 = capture_usage_output("anotherprog");

    if (out1.empty() || out2.empty()) {
        std::cerr << "[Test2] Failed: one or both usage outputs are empty.\n";
        return false;
    }

    // Ensure both outputs contain the essential sections
    if (out1.find("OPTIONS") == std::string::npos ||
        out2.find("OPTIONS") == std::string::npos) {
        std::cerr << "[Test2] Failed: missing 'OPTIONS' in one of the outputs.\n";
        return false;
    }

    // Basic sanity: outputs should not be completely different in a way that breaks
    // the structure. We at least expect a common substring such as "DESCRIPTION".
    if (out1.find("DESCRIPTION") == std::string::npos ||
        out2.find("DESCRIPTION") == std::string::npos) {
        std::cerr << "[Test2] Failed: missing 'DESCRIPTION' in one of the outputs.\n";
        return false;
    }

    return true;
}

// Test 3: Non-crashing behavior with multiple invocations
// Call usage multiple times in succession to ensure no global state persists
// that would crash or corrupt subsequent outputs.
static bool test_usage_multiple_invocations() {
    const char* prog_names[] = { "pngfix", "pngfix_cli", "testprog" };
    for (const char* name : prog_names) {
        std::string out = capture_usage_output(name);
        if (out.empty()) {
            std::cerr << "[Test3] Failed: usage output empty for prog=" << name << "\n";
            return false;
        }
        if (out.find("OPTIONS") == std::string::npos) {
            std::cerr << "[Test3] Failed: missing 'OPTIONS' after prog=" << name << "\n";
            return false;
        }
    }
    return true;
}

// Entry point: Run tests and report aggregate result
int main() {
    // Collect results
    int failures = 0;

    // Run tests with explanatory comments
    if (!test_usage_contains_key_sections()) {
        std::cerr << "Test 1 failed: usage() output did not contain required sections.\n";
        ++failures;
    } else {
        std::cout << "Test 1 passed: usage() output contains required sections.\n";
    }

    if (!test_usage_with_different_prog_names()) {
        std::cerr << "Test 2 failed: usage() output inconsistent across prog values.\n";
        ++failures;
    } else {
        std::cout << "Test 2 passed: usage() output remains valid for different prog values.\n";
    }

    if (!test_usage_multiple_invocations()) {
        std::cerr << "Test 3 failed: usage() did not handle multiple invocations correctly.\n";
        ++failures;
    } else {
        std::cout << "Test 3 passed: usage() handles multiple invocations without crashes.\n";
    }

    if (failures) {
        std::cerr << "Unit tests completed with " << failures << " failure(s).\n";
        return 1;
    }

    std::cout << "All unit tests passed.\n";
    return 0;
}