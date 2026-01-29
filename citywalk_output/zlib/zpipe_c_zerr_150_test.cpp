// Test suite for the focal C function zerr(int) from zpipe.c
// This test is written in C++11, uses no GTest, and captures stderr output
// to validate the behavior of zerr for various ret codes.
// The tests assume zpipe.c provides a non-static C function:
//     extern "C" void zerr(int);
// The test harness links against that object file (and zlib) during build.

#include <cstdio>
#include <cstring>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <iterator>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <iostream>
#include <zlib.h>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <cstdlib>


// Declare the focal function with C linkage to prevent name mangling
extern "C" void zerr(int);

// Helper to produce a readable representation of a string (for test diffs)
static std::string repr(const std::string& s) {
    std::string r = "\"";
    for (char c : s) {
        if (c == '\n') r += "\\n";
        else if (c == '\r') r += "\\r";
        else if (c == '\"') r += "\\\"";
        else r += c;
    }
    r += "\"";
    return r;
}

// Capture the exact stderr output produced by zerr(ret).
// It temporarily redirects STDERR to a temporary file, invokes zerr, then
// restores STDERR and reads back the contents.
static std::string captureZerrOutput(int ret) {
    using std::string;

    // Backup original STDERR file descriptor
    int STDERR_fd = dup(fileno(stderr));
    if (STDERR_fd == -1) {
        return "";
    }

    // Create a temporary file to capture output
    char tmpTemplate[] = "zerr_outputXXXXXX";
    int tmpFD = mkstemp(tmpTemplate);
    if (tmpFD == -1) {
        close(STDERR_fd);
        return "";
    }

    // Redirect STDERR to the temporary file
    if (dup2(tmpFD, fileno(stderr)) == -1) {
        // Cleanup
        close(tmpFD);
        close(STDERR_fd);
        return "";
    }
    // Close the extra descriptor as it's no longer needed
    close(tmpFD);

    // Invoke the focal function
    zerr(ret);

    // Flush and restore STDERR state
    fflush(stderr);
    if (dup2(STDERR_fd, fileno(stderr)) == -1) {
        // If restore fails, best effort cleanup
    }
    close(STDERR_fd);

    // Read captured output
    std::ifstream ifs(tmpTemplate);
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        std::istreambuf_iterator<char>());
    ifs.close();

    // Remove the temporary file
    remove(tmpTemplate);

    return content;
}

// Lightweight test harness
static void runTest(int ret, const std::string& expected, const char* testName) {
    std::string out = captureZerrOutput(ret);
    if (out == expected) {
        std::cout << "[PASS] " << testName << "\n";
    } else {
        std::cout << "[FAIL] " << testName << "\n";
        std::cout << "  Expected: " << repr(expected) << "\n";
        std::cout << "  Got:      " << repr(out) << "\n";
    }
}

// Entry point: run a focused set of tests covering zerr behavior.
// We target true/false coverage where feasible. Some branches involving
// ferror(stdin)/ferror(stdout) are environment-dependent to reliably test,
// so we validate the deterministic message outputs for each Z_* case.
int main() {
    // Ensure zlib constants are available
    // We rely on Z_ERRNO, Z_STREAM_ERROR, Z_DATA_ERROR, Z_MEM_ERROR, Z_VERSION_ERROR

    // Test 1: ret = Z_ERRNO, with no simulated stream errors.
    // Expected output: "zpipe: " (prefix only, since neither stdin nor stdout is in error).
    // This exercises the initial prefix and the absence of additional messages.
    runTest(Z_ERRNO, "zpipe: ", "zerr Z_ERRNO with no ferror flags");

    // Test 2: ret = Z_STREAM_ERROR
    // Expected output: "zpipe: invalid compression level\n"
    runTest(Z_STREAM_ERROR, "zpipe: invalid compression level\n", "zerr Z_STREAM_ERROR");

    // Test 3: ret = Z_DATA_ERROR
    // Expected output: "zpipe: invalid or incomplete deflate data\n"
    runTest(Z_DATA_ERROR, "zpipe: invalid or incomplete deflate data\n", "zerr Z_DATA_ERROR");

    // Test 4: ret = Z_MEM_ERROR
    // Expected output: "zpipe: out of memory\n"
    runTest(Z_MEM_ERROR, "zpipe: out of memory\n", "zerr Z_MEM_ERROR");

    // Test 5: ret = Z_VERSION_ERROR
    // Expected output: "zpipe: zlib version mismatch!\n"
    runTest(Z_VERSION_ERROR, "zpipe: zlib version mismatch!\n", "zerr Z_VERSION_ERROR");

    return 0;
}