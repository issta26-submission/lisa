// Test suite for readpng2_version_info (readpng2.c) using a lightweight C++11 runner
// - No GTest is used
// - Focuses on capturing and validating the stderr output produced by readpng2_version_info
// - Uses the actual function from the C source (readpng2.c) and relies on the environment's libpng/zlib setup
// - Provides two lightweight tests with explanatory comments for maintainability

#include <unistd.h>
#include <algorithm>
#include <setjmp.h>
#include <string>
#include <zlib.h>
#include <readpng2.h>
#include <stdlib.h>
#include <cstdio>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstring>
#include <png.h>


// Forward declaration of the focal function (C linkage)
extern "C" void readpng2_version_info(void);

// Candidate Keywords derived from the focal method and class dependencies
// - libpng, PNG_LIBPNG_VER_STRING, png_libpng_ver
// - zlib, ZLIB_VERSION, zlib_version
// - readpng2_version_info (function under test)
// These keywords guide test intent: verify version-info printing to stderr and the use of
// the libpng/zlib version strings.


// Utility to capture stderr output of a function call (POSIX pipe approach)
static std::string capture_stderr(void (*func)(void)) {
    // Create pipe for capturing stderr
    int pipe_fd[2];
    if (pipe(pipe_fd) != 0) {
        return ""; // failed to create pipe
    }

    // Save current stderr
    int saved_stderr = dup(STDERR_FILENO);
    if (saved_stderr == -1) {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        return "";
    }

    // Redirect stderr to the pipe
    if (dup2(pipe_fd[1], STDERR_FILENO) == -1) {
        // cleanup
        dup2(saved_stderr, STDERR_FILENO);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        close(saved_stderr);
        return "";
    }

    // Close the write end in this process; read end is used to capture
    close(pipe_fd[1]);

    // Flush and invoke the function under test
    fflush(stderr);
    func();

    // Restore original stderr
    fflush(stderr);
    dup2(saved_stderr, STDERR_FILENO);
    close(saved_stderr);

    // Read captured data
    const size_t BUFSZ = 4096;
    char buf[BUFSZ];
    std::string captured;
    ssize_t n;
    // Read until EOF on the pipe
    while ((n = read(pipe_fd[0], buf, sizeof(buf))) > 0) {
        captured.append(buf, static_cast<size_t>(n));
    }
    close(pipe_fd[0]);
    return captured;
}

// Lightweight test framework helpers (non-terminating style)
struct TestResult {
    int failures = 0;
    void fail(const std::string &msg) {
        ++failures;
        // Print non-fatal failure message; allow subsequent tests to run
        fprintf(stderr, "TEST FAILURE: %s\n", msg.c_str());
    }
};

static bool test_output_contains_expected_format(TestResult &tr) {
    // Capture the output of readpng2_version_info
    std::string output = capture_stderr(readpng2_version_info);

    // Step 2 coverage goal: verify presence of expected substrings and two-line structure
    bool hasCompiledLine  = output.find("Compiled with libpng") != std::string::npos;
    bool hasUsingLine     = output.find("using libpng") != std::string::npos;
    bool hasZlibLine      = output.find("and with zlib") != std::string::npos;
    bool hasZlibUsingLine = output.find("using zlib") != std::string::npos;
    // We expect exactly two lines: one for libpng version, one for zlib version
    int newlineCount = std::count(output.begin(), output.end(), '\n');
    bool hasTwoLines = (newlineCount == 2);

    bool ok = hasCompiledLine && hasUsingLine && hasZlibLine && hasZlibUsingLine && hasTwoLines;
    if (!ok) {
        tr.fail("readpng2_version_info output did not contain expected format.\n"
                "Output was:\n" + output);
    }
    return ok;
}

// Additional lightweight formatting check (ensures indentation and basic structure)
static bool test_indentation_format(TestResult &tr) {
    std::string output = capture_stderr(readpng2_version_info);

    // First line should start with three spaces followed by "Compiled"
    bool firstLineOk = output.find("   Compiled with") == 0;

    // Second line should begin with "   and with zlib" after a newline
    bool secondLineIndented = output.find("\n   and with zlib") != std::string::npos;

    bool ok = firstLineOk && secondLineIndented;
    if (!ok) {
        tr.fail("readpng2_version_info indentation format not as expected.\nOutput:\n" + output);
    }
    return ok;
}

// Entry point for the test suite
int main() {
    TestResult tr;

    // Test 1: Verify the presence of expected substrings and two-line structure
    bool test1 = test_output_contains_expected_format(tr);

    // Test 2: Verify indentation/format consistency
    bool test2 = test_indentation_format(tr);

    // Aggregate results
    if (tr.failures > 0) {
        fprintf(stderr, "Total failures: %d\n", tr.failures);
        return 1;
    }

    // If both tests pass, success
    return (test1 && test2) ? 0 : 1;
}

/*
Notes for developers:
- This test assumes that the environment provides a valid libpng and zlib setup and that
  readpng2_version_info prints version information via stderr as shown in the focal method.
- The tests capture stderr output and perform substring/structure checks rather than exact
  string matching to remain robust across different libpng/zlib version values.
- No GTest is used per requirements; tests run from main() and report failures to stderr without
  terminating early to maximize coverage.
- If you want to extend tests, you can add more checks for exact presence of "PNG_LIBPNG_VER_STRING"
  or the actual content of the version strings if you have stable and known values in your build
  environment.
- Build guidance (example, adapt to your build system):
  g++ -std=c++11 -Wall -O2 -I<path_to_readpng2_headers> test_readpng2_version_info.cpp readpng2.c -o test_readpng2_version_info
  Ensure that libpng and zlib are linkable in your environment if readpng2.c references them.
*/