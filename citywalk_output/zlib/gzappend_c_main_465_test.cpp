// gzappend_unit_tests.cpp
// Lightweight C++11 test suite for the focal main() function in gzappend.c
// Approach: invoke the gzappend executable as a separate process using popen,
// capture its output, and verify exit statuses. This avoids modifying the
// original C source and avoids linking two mains together. Tests are designed
// to exercise the argument parsing paths and early exits (no-argument case and
// error cases for invalid/insufficient arguments).

#include <cstdio>
#include <string.h>
#include <fcntl.h>
#include <cassert>
#include <string>
#include <functional>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <zlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>


// Helper: run the gzappend executable with given args and capture stdout/stderr.
// Returns true on success (process launched and finished), false on failure to launch.
static bool runGzappendWithArgs(const std::string& args, int& exitCode, std::string& output) {
    // Build command line. Redirect stdout and stderr to capture both.
    std::string cmd = "./gzappend " + args + " 2>&1";

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) {
        std::cerr << "Failed to launch gzappend with args: " << args << std::endl;
        return false;
    }

    char buffer[256];
    output.clear();
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output += buffer;
    }

    int status = pclose(pipe);
    // On Unix-like systems, WIFEXITED/WEXITSTATUS can interpret the status.
    if (WIFEXITED(status)) {
        exitCode = WEXITSTATUS(status);
    } else {
        // If the process didn't exit normally, mark as abnormal.
        exitCode = -1;
    }

    return true;
}

// Simple assertion helper with message
static void ASSERT(bool condition, const std::string& msg) {
    if (!condition) {
        std::cerr << "Assertion failed: " << msg << std::endl;
    }
    assert(condition);
}

// Test 1: No arguments provided -> should print usage and exit with code 0.
// Verifies that the program handles the "no arguments" branch correctly.
static void test_no_arguments() {
    int exitCode = -1;
    std::string output;
    bool ok = runGzappendWithArgs("", exitCode, output);
    ASSERT(ok, "Failed to launch gzappend for test_no_arguments");
    ASSERT(exitCode == 0, "Expected exit code 0 for no-argument case");
    // Check for presence of usage/help strings in output.
    ASSERT(output.find("gzappend 1.2") != std::string::npos &&
           output.find("usage: gzappend") != std::string::npos,
           "Expected usage/help text to be present for no-argument case");
}

// Test 2: Invalid compression level provided (e.g., "-x") should trigger bye() path
// and exit with a non-zero status.
static void test_invalid_compression_level() {
    int exitCode = -1;
    std::string output;
    bool ok = runGzappendWithArgs("-x", exitCode, output);
    ASSERT(ok, "Failed to launch gzappend for test_invalid_compression_level");
    // Expect non-zero exit status due to invalid compression level handling.
    ASSERT(exitCode != 0, "Expected non-zero exit code for invalid compression level");
    // Optional: verify that the error message mentions "invalid compression level".
    // Some implementations of bye() print the message; keep assertion lenient.
    // If available, ensure the output contains the substring "invalid compression level".
    // (Commented out to avoid brittle tests on different builds)
    // ASSERT(output.find("invalid compression level") != std::string::npos, "Expected invalid compression level message");
}

// Test 3: Compression level option provided but no gzip file name after options
// (e.g., "-5" with no subsequent arguments) should trigger bye() path and exit non-zero.
static void test_no_gzip_file_after_options() {
    int exitCode = -1;
    std::string output;
    bool ok = runGzappendWithArgs("-5", exitCode, output);
    ASSERT(ok, "Failed to launch gzappend for test_no_gzip_file_after_options");
    // Expect non-zero exit status due to missing gzip filename after options.
    ASSERT(exitCode != 0, "Expected non-zero exit code for missing gzip filename after options");
}

// Entry point: run all tests and report results.
int main() {
    std::vector<std::pair<std::string, std::function<void()>>> tests = {
        {"NoArguments", test_no_arguments},
        {"InvalidCompressionLevel", test_invalid_compression_level},
        {"NoGzipFileAfterOptions", test_no_gzip_file_after_options}
    };

    std::cout << "Running gzappend unit tests (no GTest)..." << std::endl;
    int passed = 0;
    int total = static_cast<int>(tests.size());

    for (const auto& t : tests) {
        std::cout << "[ RUN      ] " << t.first << std::endl;
        try {
            t.second();
            std::cout << "[       OK ] " << t.first << std::endl;
            ++passed;
        } catch (...) {
            std::cerr << "[  FAILED  ] " << t.first << " (exception)" << std::endl;
        }
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}

/*
Notes for how to build and run the tests:

1) Compile the target program gzappend.c into an executable named gzappend.
   Example (Linux with zlib available):
     gcc -std=c11 -O2 -Wall -D_GNU_SOURCE -lz -o gzappend gzappend.c

2) Compile this test harness into an executable, linking with C++11 standard library.
   Example:
     g++ -std=c++11 -O2 -Wall -o gzappend_unit_tests gzappend_unit_tests.cpp

3) Ensure the gzappend executable is in the same directory as the test harness, or adjust
   the path in runGzappendWithArgs() accordingly.

4) Run the tests:
     ./gzappend_unit_tests

5) The tests use a lightweight approach (system/popens) to invoke the focal main()
   function in gzappend.c as a separate process, which ensures we test actual
   command-line behavior without modifying the original codebase.

Caveats:
- These tests rely on POSIX features (popen, pclose, WIFEXITED, WEXITSTATUS). They may not be
  portable to non-POSIX environments (Windows). If Windows support is required, a
  separate Windows-compatible test strategy would be needed (e.g., CreateProcess and pipes).
- The tests focus on argument parsing and early exit branches. Deep functional testing of
  gzappend's gzip manipulation requires actual gzip file scenarios and may be environment-dependent.
*/