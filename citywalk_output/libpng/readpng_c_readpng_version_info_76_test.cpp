/*
Unit test suite for the focal method readpng_version_info located in readpng.c

Overview:
- This test suite is written in C++11 (no GTest) and relies on linking with the existing C implementation
  (readpng.c/readpng.h) which prints version information to stderr via fprintf.
- The tests capture stderr output produced by readpng_version_info and perform assertions on the
  content to validate correct behavior across typical environments.
- Tests are executed from main() to satisfy the constraint of not using a dedicated test framework.
- The tests avoid modifying private/internal state and rely on the public behavior exposed by readpng_version_info.

Key candidate keywords representing the focal function's core components:
- PNG_LIBPNG_VER_STRING, png_libpng_ver
- ZLIB_VERSION, zlib_version
- readpng_version_info
- fprintf, stderr
- libpng, zlib
*/

#include <unistd.h>
#include <vector>
#include <string>
#include <zlib.h>
#include <sys/types.h>
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <sys/wait.h>
#include <readpng.h>
#include <stdio.h>
#include <cstring>
#include <png.h>


extern "C" {
}

// Helper: capture stderr output while invoking a C function with signature void(void)
static std::string capture_stderr(void (*func)(void)) {
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // Fallback: return empty output on failure to create pipe
        return "";
    }

    int saved_stderr = dup(2);
    if (saved_stderr == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Redirect stderr to the write end of the pipe
    dup2(pipefd[1], 2);
    close(pipefd[1]); // the duplicated fd is now the write end

    // Execute the function that writes to stderr
    func();

    // Flush to ensure all data is written
    fflush(stderr);

    // Read the data from the pipe
    const size_t BUFSZ = 4096;
    char buffer[BUFSZ];
    std::string output;
    ssize_t n;
    while ((n = read(pipefd[0], buffer, BUFSZ)) > 0) {
        output.append(buffer, static_cast<size_t>(n));
    }

    // Restore original stderr
    dup2(saved_stderr, 2);
    close(saved_stderr);
    close(pipefd[0]);

    return output;
}

// Helper: split captured output into lines
static void split_lines(const std::string& s, std::vector<std::string>& lines) {
    std::string current;
    for (char c : s) {
        if (c == '\n') {
            lines.push_back(current);
            current.clear();
        } else {
            current.push_back(c);
        }
    }
    if (!current.empty()) lines.push_back(current);
}

/*
Candidate Test 1: Basic output structure validation
- Capture stderr when readpng_version_info() is invoked.
- Validate that there are at least two lines.
- Validate that the first line contains "Compiled with libpng" and "using libpng".
- Validate that the second line contains "Compiled with zlib" and "using zlib".
- This test does not rely on exact version strings (which are environment-dependent) but asserts correct formatting and presence of both components.
*/
static bool test_readpng_version_info_basic_output() {
    // Capture stderr output from the focal function
    std::string output = capture_stderr(readpng_version_info);

    // Split into lines for analysis
    std::vector<std::string> lines;
    split_lines(output, lines);

    if (lines.size() < 2) {
        std::cerr << "[TEST FAIL] Expected at least 2 lines in output, got " << lines.size() << "\n";
        return false;
    }

    // Validate first line: libpng information
    if (lines[0].find("Compiled with libpng") == std::string::npos) {
        std::cerr << "[TEST FAIL] First line does not contain 'Compiled with libpng'\n";
        return false;
    }
    if (lines[0].find("using libpng") == std::string::npos) {
        std::cerr << "[TEST FAIL] First line does not contain 'using libpng'\n";
        return false;
    }

    // Validate second line: zlib information
    if (lines[1].find("Compiled with zlib") == std::string::npos) {
        std::cerr << "[TEST FAIL] Second line does not contain 'Compiled with zlib'\n";
        return false;
    }
    if (lines[1].find("using zlib") == std::string::npos) {
        std::cerr << "[TEST FAIL] Second line does not contain 'using zlib'\n";
        return false;
    }

    // Basic sanity check on line lengths to ensure content exists
    if (lines[0].length() < 20 || lines[1].length() < 20) {
        std::cerr << "[TEST FAIL] Output lines appear unexpectedly short\n";
        return false;
    }

    return true;
}

/*
Candidate Test 2: Repeated invocation stability
- Call readpng_version_info() twice via the capture_stderr wrapper.
- Ensure that outputs are produced for both invocations and that no crash occurs.
*/
static bool test_readpng_version_info_multiple_calls_no_crash() {
    std::string first = capture_stderr(readpng_version_info);
    std::string second = capture_stderr(readpng_version_info);

    if (first.empty()) {
        std::cerr << "[TEST FAIL] First invocation produced empty output\n";
        return false;
    }
    if (second.empty()) {
        std::cerr << "[TEST FAIL] Second invocation produced empty output\n";
        return false;
    }

    // Basic consistency: both invocations should produce similar structure
    // Check that both contain the expected phrases
    if (first.find("Compiled with libpng") == std::string::npos ||
        first.find("using libpng") == std::string::npos) {
        std::cerr << "[TEST FAIL] First invocation output malformed\n";
        return false;
    }
    if (second.find("Compiled with zlib") == std::string::npos ||
        second.find("using zlib") == std::string::npos) {
        std::cerr << "[TEST FAIL] Second invocation output malformed\n";
        return false;
    }

    return true;
}

/*
Domain Knowledge considerations supported:
- Use of C linkage for C functions when testing from C++ (extern "C" via header inclusion).
- Avoid private details; test through public API only.
- Use only standard library and provided method; no GMock or GTest.
- Non-terminating assertions: tests return bool but do not exit on failure; main aggregates results.
*/

// Simple test harness
int main() {
    int failures = 0;

    // Test 1: Basic output structure
    if (test_readpng_version_info_basic_output()) {
        std::cout << "[TEST PASSED] test_readpng_version_info_basic_output\n";
    } else {
        std::cerr << "[TEST FAILED] test_readpng_version_info_basic_output\n";
        ++failures;
    }

    // Test 2: Repeated invocations stability
    if (test_readpng_version_info_multiple_calls_no_crash()) {
        std::cout << "[TEST PASSED] test_readpng_version_info_multiple_calls_no_crash\n";
    } else {
        std::cerr << "[TEST FAILED] test_readpng_version_info_multiple_calls_no_crash\n";
        ++failures;
    }

    if (failures == 0) {
        std::cout << "All tests passed.\n";
    } else {
        std::cerr << failures << " test(s) failed.\n";
    }

    return failures;
}