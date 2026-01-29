/*
Step 1: Program Understanding (high-level)
- focal function: mem_high(z_stream *strm, char *prefix)
  - identifies the opaque member of z_stream as a mem_zone* and prints
    "<prefix>: <zone->highwater> high water mark" to stderr.
  - No branching inside mem_high; behavior is deterministic given strm->opaque and highwater.

Step 2 & 3: Unit Test Generation and Refinement (C++11, no GTest)
- We assume mem_high is link-accessible (symbolized as extern "C" void mem_high(z_stream*, char*)).
- Dependency shim for test: a minimal struct mem_zone with at least a member highwater that mem_high will read.
- Test harness uses POSIX pipes to capture fprintf(stderr, ...) output from mem_high.
- Tests cover:
  1) value 0 with a non-empty prefix
  2) a typical large value with a non-empty prefix
  3) an empty prefix with a value
- Each test asserts exact stdout (stderr) text produced by mem_high.
- Non-terminating assertions are emulated via a simple PASS/FAIL report and a final summary.
- The tests are designed to be compiled together with infcover.c (containing mem_high) under C++11.

Note: This test uses POSIX APIs (pipe, dup2, read) to capture stderr. It is suitable for POSIX-like environments (Linux/macOS). If you need Windows support, a equivalent Windows-SEH/CRT capture path would be required.

Now the test code:

*/

#include <cstdio>
#include <cstring>
#include <string.h>
#include <assert.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <inftrees.h>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <inflate.h>


// Forward declaration of mem_high from the translation unit under test.
// The function is expected to be linked in; if the 'local' macro resolves to static in
// the original file, this may still be accessible depending on compilation unit visibility.
// Ensure to compile/link with infcover.c (or the appropriate object) to resolve this symbol.
extern "C" void mem_high(z_stream *strm, char *prefix);

// Minimal mem_zone structure matching the usage inside mem_high.
// The real project may have a more complex definition. We only rely on the highwater member here.
struct mem_zone {
    unsigned long highwater;
};

// Helper: capture the output written to stderr by mem_high for a given highwater and prefix.
// It uses a POSIX pipe to intercept writes to STDERR, then restores STDERR.
std::string capture_mem_high(unsigned long value, const char* prefix) {
    // Create a pipe to capture stderr output
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        return "";
    }

    // Save current stderr and redirect it to the pipe's write end
    int saved_stderr = dup(fileno(stderr));
    if (saved_stderr == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    if (dup2(pipefd[1], fileno(stderr)) == -1) {
        // Restore state on error
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stderr);
        return "";
    }
    // We no longer need the write end in this process
    close(pipefd[1]);

    // Prepare the structures expected by mem_high
    z_stream strm;
    memset(&strm, 0, sizeof(strm)); // zero-initialize to be safe
    mem_zone zone;
    zone.highwater = value;
    strm.opaque = &zone;

    // Call the focal function
    mem_high(&strm, const_cast<char*>(prefix));

    // Ensure all data is flushed to the pipe
    fflush(stderr);

    // Read all captured data from the pipe
    std::string output;
    char buffer[256];
    ssize_t n;
    // Read until EOF
    while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, static_cast<size_t>(n));
    }

    // Clean up and restore STDERR
    close(pipefd[0]);
    dup2(saved_stderr, fileno(stderr));
    close(saved_stderr);

    return output;
}

// Simple test harness: run a single test case and report PASS/FAIL with details.
bool run_case(int id, const std::string& name, unsigned long value, const char* prefix) {
    std::string output = capture_mem_high(value, prefix);
    std::string expected = std::string(prefix) + ": " +
                           std::to_string(value) + " high water mark\n";

    bool ok = (output == expected);
    std::cout << "Test " << id << " - " << name << ": " << (ok ? "PASS" : "FAIL") << std::endl;
    if (!ok) {
        std::cerr << "  Expected: [" << expected << "]" << std::endl;
        std::cerr << "  Got:      [" << output << "]" << std::endl;
    }
    return ok;
}

int main(void) {
    // Step 3: Test cases - coverage of true/false paths is not branching in mem_high itself,
    // but we verify different values and prefixes to ensure proper printing logic.
    //
    // Test 1: Zero highwater with non-empty prefix
    bool t1 = run_case(1, "Zero highwater prints with non-empty prefix", 0, "TEST0");

    // Test 2: Typical large value with non-empty prefix
    bool t2 = run_case(2, "Large highwater value prints with non-empty prefix", 123456789UL, "PFX");

    // Test 3: Non-empty value with empty prefix to verify formatting when prefix is empty
    bool t3 = run_case(3, "Non-empty value with empty prefix", 42UL, "");

    int passed = (t1 ? 1 : 0) + (t2 ? 1 : 0) + (t3 ? 1 : 0);
    int total = 3;
    std::cout << "Summary: " << passed << "/" << total << " tests passed." << std::endl;

    return (passed == total) ? 0 : 1;
}