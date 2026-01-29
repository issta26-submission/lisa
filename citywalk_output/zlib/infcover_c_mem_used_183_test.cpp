/*
Unit test suite for the focal method mem_used located in infcover.c
- The tests are written in C++11 (no GTest) and call the C function
  extern "C" void mem_used(z_stream *strm, char *prefix);
- mem_used prints to stderr in the format: "<prefix>: <total> allocated\n"
- To exercise mem_used, tests build a minimal z_stream-like structure
  (using the zlib z_stream) and a mem_zone with a 'total' member that mem_used
  reads via strm->opaque.
- Tests capture stderr output by redirecting the file descriptor, invoke mem_used,
  then restore stdout/stderr and validate the captured content.
- Each test includes comments explaining its purpose and expected behavior.
*/

#include <cstdio>
#include <cstring>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <string>
#include <sys/stat.h>
#include <stdio.h>
#include <sys/types.h>
#include <inftrees.h>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <inflate.h>


// Declare the focal method from the target file.
// It is assumed to have C linkage in the linked object/module.
extern "C" void mem_used(z_stream *strm, char *prefix);

// Helper function to capture stderr output produced by mem_used.
// It redirects stderr to a pipe, invokes mem_used, then reads and returns
// the captured string.
static std::string capture_mem_used_output(z_stream *strm, const char *prefix) {
    // Prepare a buffer for a copy of the prefix (mem_used expects a non-const char*)
    char prefix_buf[256];
    std::strncpy(prefix_buf, prefix, sizeof(prefix_buf) - 1);
    prefix_buf[sizeof(prefix_buf) - 1] = '\0';

    // Setup pipe to capture stderr
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        return ""; // failed to create pipe
    }

    // Save current stderr
    int saved_stderr = dup(STDERR_FILENO);
    if (saved_stderr == -1) {
        // cleanup
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Redirect stderr to the write end of the pipe
    fflush(stderr);
    if (dup2(pipefd[1], STDERR_FILENO) == -1) {
        // cleanup
        dup2(saved_stderr, STDERR_FILENO);
        close(saved_stderr);
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Call the function under test
    mem_used(strm, prefix_buf);

    // Flush and restore stderr
    fflush(stderr);
    dup2(saved_stderr, STDERR_FILENO);
    close(saved_stderr);
    close(pipefd[1]); // close write end to signal EOF to reader

    // Read all data written to the pipe
    std::string output;
    char buf[1024];
    ssize_t n;
    while ((n = read(pipefd[0], buf, sizeof(buf))) > 0) {
        output.append(buf, static_cast<size_t>(n));
    }

    close(pipefd[0]);
    return output;
}

// Test 1: Basic correctness for a typical total value with a simple prefix
// - Verifies that the output contains the exact prefix and the correct total value.
// - Ensures the format is "<prefix>: <total> allocated\n".
static bool test_mem_used_basic_prefix_and_value() {
    // Arrange
    struct mem_zone { unsigned long total; };
    mem_zone zone;
    zone.total = 12345;

    z_stream strm;
    // Zero initialize to be safe; only opaque is used by mem_used
    std::memset(&strm, 0, sizeof(strm));
    strm.opaque = &zone; // mem_used will read zone->total

    const char *prefix = "PFX_TEST";

    // Act
    std::string captured = capture_mem_used_output(&strm, prefix);

    // Build expected output
    std::string expected = std::string(prefix) + ": " + std::to_string(zone.total) + " allocated\n";

    // Assert
    if (captured != expected) {
        // For debugging: print what was captured
        fprintf(stderr, "Test 1 failed. Expected: \"%s\" Got: \"%s\"\n",
                expected.c_str(), captured.c_str());
        return false;
    }
    return true;
}

// Test 2: Zero total value with a short prefix
// - Verifies formatting and that 0 is printed correctly.
static bool test_mem_used_zero_total() {
    struct mem_zone { unsigned long total; };
    mem_zone zone;
    zone.total = 0;

    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));
    strm.opaque = &zone;

    const char *prefix = "ZERO";

    std::string captured = capture_mem_used_output(&strm, prefix);
    std::string expected = std::string(prefix) + ": " + std::to_string(zone.total) + " allocated\n";

    if (captured != expected) {
        fprintf(stderr, "Test 2 failed. Expected: \"%s\" Got: \"%s\"\n",
                expected.c_str(), captured.c_str());
        return false;
    }
    return true;
}

// Test 3: Long and spaced prefix to ensure proper handling of prefix formatting
// - Verifies that prefixes containing spaces are preserved correctly.
static bool test_mem_used_long_and_spaced_prefix() {
    struct mem_zone { unsigned long total; };
    mem_zone zone;
    zone.total = 987654;

    z_stream strm;
    std::memset(&strm, 0, sizeof(strm));
    strm.opaque = &zone;

    const char *prefix = "THIS IS A LONG PREFIX";

    std::string captured = capture_mem_used_output(&strm, prefix);
    std::string expected = std::string(prefix) + ": " + std::to_string(zone.total) + " allocated\n";

    if (captured != expected) {
        fprintf(stderr, "Test 3 failed. Expected: \"%s\" Got: \"%s\"\n",
                expected.c_str(), captured.c_str());
        return false;
    }
    return true;
}

// Test 4: Multiple sequential calls to mem_used with different totals
// - Ensures that successive invocations do not contaminate each other's output.
// - Useful for coverage of potential state carried by the surrounding file scope.
static bool test_mem_used_multiple_calls_independent_outputs() {
    // First call
    struct mem_zone { unsigned long total; };
    mem_zone zone1;
    zone1.total = 55;

    z_stream strm1;
    std::memset(&strm1, 0, sizeof(strm1));
    strm1.opaque = &zone1;
    const char *prefix1 = "FIRST";

    std::string captured1 = capture_mem_used_output(&strm1, prefix1);
    std::string expected1 = std::string(prefix1) + ": " + std::to_string(zone1.total) + " allocated\n";

    if (captured1 != expected1) {
        fprintf(stderr, "Test 4 (part 1) failed. Expected: \"%s\" Got: \"%s\"\n",
                expected1.c_str(), captured1.c_str());
        return false;
    }

    // Second call
    struct mem_zone zone2;
    zone2.total = 9999;

    z_stream strm2;
    std::memset(&strm2, 0, sizeof(strm2));
    strm2.opaque = &zone2;
    const char *prefix2 = "SECOND";

    std::string captured2 = capture_mem_used_output(&strm2, prefix2);
    std::string expected2 = std::string(prefix2) + ": " + std::to_string(zone2.total) + " allocated\n";

    if (captured2 != expected2) {
        fprintf(stderr, "Test 4 (part 2) failed. Expected: \"%s\" Got: \"%s\"\n",
                expected2.c_str(), captured2.c_str());
        return false;
    }

    return true;
}

// Helper to run all tests and report summary
int main(void) {
    int passed = 0;
    int total = 0;

    fprintf(stderr, "Starting mem_used unit tests...\n");

    if (test_mem_used_basic_prefix_and_value()) {
        ++passed;
        fprintf(stderr, "Test 1: PASS\n");
    } else {
        fprintf(stderr, "Test 1: FAIL\n");
    }
    ++total;

    if (test_mem_used_zero_total()) {
        ++passed;
        fprintf(stderr, "Test 2: PASS\n");
    } else {
        fprintf(stderr, "Test 2: FAIL\n");
    }
    ++total;

    if (test_mem_used_long_and_spaced_prefix()) {
        ++passed;
        fprintf(stderr, "Test 3: PASS\n");
    } else {
        fprintf(stderr, "Test 3: FAIL\n");
    }
    ++total;

    if (test_mem_used_multiple_calls_independent_outputs()) {
        ++passed;
        fprintf(stderr, "Test 4: PASS\n");
    } else {
        fprintf(stderr, "Test 4: FAIL\n");
    }
    ++total;

    fprintf(stderr, "mem_used tests: %d / %d passed.\n", passed, total);
    return (passed == total) ? 0 : 1;
}