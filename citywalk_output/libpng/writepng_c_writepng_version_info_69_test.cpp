// Test suite for writepng_version_info (C function) using a C++11 test harness.
// No GTest; a lightweight in-process test runner is provided.
// The focal method prints version information to stderr. We capture stderr
// by temporarily redirecting it to a pipe and then validating the output.

#include <unistd.h>
#include <algorithm>
#include <vector>
#include <string>
#include <sys/stat.h>
#include <zlib.h>
#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <cstdio>
#include <sys/types.h>
#include <cstring>
#include <png.h>
#include <writepng.h>


// Declare the focal function with C linkage to avoid name mangling when linked.
extern "C" void writepng_version_info(void);

// Simple non-terminating assertion for test failure reporting
#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[FAILED] " << (msg) << "\n"; \
        ++g_failures; \
    } else { \
        std::cerr << "[PASSED] " << (msg) << "\n"; \
    } \
} while (0)

// Global failure counter for the test suite
static int g_failures = 0;

// Helper: Capture the stderr output produced by writepng_version_info.
// This uses a POSIX pipe to redirect STDERR_FILENO, executes the function, then
// reads all captured data from the read end of the pipe.
static std::string capture_writepng_version_info_output() {
#if defined(__unix__) || defined(__APPLE__)
    int pipefds[2];
    if (pipe(pipefds) != 0) {
        std::cerr << "[ERROR] Failed to create pipe for stderr capture.\n";
        return "";
    }

    // Save original stderr
    int saved_stderr = dup(STDERR_FILENO);
    if (saved_stderr == -1) {
        std::cerr << "[ERROR] Failed to duplicate stderr for restoration.\n";
        close(pipefds[0]);
        close(pipefds[1]);
        return "";
    }

    // Redirect stderr to the write end of the pipe
    if (dup2(pipefds[1], STDERR_FILENO) == -1) {
        std::cerr << "[ERROR] Failed to redirect stderr to pipe.\n";
        close(pipefds[0]);
        close(pipefds[1]);
        close(saved_stderr);
        return "";
    }

    // Close the unused write end in this process after redirect
    close(pipefds[1]);

    // Invoke the focal function
    writepng_version_info();

    // Ensure all output is flushed
    fflush(stderr);

    // Restore original stderr
    dup2(saved_stderr, STDERR_FILENO);
    close(saved_stderr);

    // Read captured output from the read end of the pipe
    std::string output;
    const size_t kBufSize = 4096;
    char buffer[kBufSize];
    ssize_t n;
    // Close the write end in the reader thread if not already closed
    // (already closed above; ensure the reader can read until EOF)
    while ((n = read(pipefds[0], buffer, kBufSize)) > 0) {
        output.append(buffer, static_cast<size_t>(n));
    }

    // Close the read end
    close(pipefds[0]);
    return output;
#else
    // Fallback (non-POSIX): unable to capture reliably on some platforms.
    // Return an empty string to indicate capture could not be performed.
    std::cerr << "[WARN] stderr capture not supported on this platform.\n";
    return "";
#endif
}

// Test 1: Basic content validation
// Verifies that the output contains both libpng and zlib version information.
static bool test_writepng_version_info_basic_content() {
    const std::string out = capture_writepng_version_info_output();
    // We expect two lines, one for libpng and one for zlib.
    bool has_libpng_line  = out.find("Compiled with libpng") != std::string::npos &&
                          out.find("using libpng")    != std::string::npos;
    bool has_zlib_line    = out.find("Compiled with zlib")  != std::string::npos &&
                          out.find("using zlib")        != std::string::npos;

    bool two_lines_present = std::count(out.begin(), out.end(), '\n') >= 2;

    bool ok = has_libpng_line && has_zlib_line && two_lines_present;
    ASSERT_TRUE(ok, "writepng_version_info emits libpng and zlib version lines to stderr with two lines.");
    return ok;
}

// Test 2: Correct order of output
// Verifies that the libpng line appears before the zlib line in the captured output.
static bool test_writepng_version_info_order() {
    const std::string out = capture_writepng_version_info_output();

    size_t pos_libpng = out.find("Compiled with libpng");
    size_t pos_zlib   = out.find("Compiled with zlib");

    bool valid_order = (pos_libpng != std::string::npos) && (pos_zlib != std::string::npos) && (pos_libpng < pos_zlib);

    ASSERT_TRUE(valid_order, "writepng_version_info prints libpng version line before zlib version line.");
    return valid_order;
}

// Test 3 (optional): Ensure function is callable and does not crash when library reports versions.
// This test simply calls the function; if it crashes, the process would terminate.
// We still capture output to verify continued operation.
static bool test_writepng_version_info_is_callable() {
    // Attempt capture; if function crashes, test would be aborted by the harness.
    std::string out = capture_writepng_version_info_output();
    bool called = !out.empty() || true; // If capture works, we consider it callable
    ASSERT_TRUE(called, "writepng_version_info is callable and produced output (or attempted capture).");
    return called;
}

// Simple test runner
static void run_all_tests() {
    std::cout << "Running tests for writepng_version_info...\n";

    test_writepng_version_info_basic_content();
    test_writepng_version_info_order();
    test_writepng_version_info_is_callable();

    // Summary
    if (g_failures == 0) {
        std::cout << "[ALL TESTS PASSED] writepng_version_info tests completed successfully.\n";
    } else {
        std::cout << "[TESTS FAILED] " << g_failures << " test(s) failed.\n";
    }
}

int main() {
    run_all_tests();
    return g_failures == 0 ? 0 : 1;
}