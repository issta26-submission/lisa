// Test suite for the focal method: DebugMemPrintTotals
// This test suite is C++11 based and avoids Google Test.
// It relies on the external C file testcms2.c (and testcms2.h) which defines
// the focal function and the global memory statistics used by it.
//
// Key assumptions (derived from the provided code snippet):
// - The focal function DebugMemPrintTotals(void) prints:
//   [Memory statistics]
//   Allocated = <TotalMemory> MaxAlloc = <MaxAllocated> Single block hit = <SingleHit>
// - TotalMemory, MaxAllocated, SingleHit are global unsigned integers defined in testcms2.c
// - We link against testcms2.c and testcms2.h in the test build.
//
// The tests capture stdout while invoking DebugMemPrintTotals and verify the
// printed content.
//
// Note: This test uses POSIX APIs (dup/pipe/dup2/read) to capture stdout.
// It may require a POSIX-like environment (e.g., Linux/macOS) to compile and run.

#include <cstring>
#include <cstdio>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <testcms2.h>
#include <sys/types.h>
#include <unistd.h>


// Include the focal class dependencies header (as provided)
extern "C" {
}

// Expose the global memory statistics used by DebugMemPrintTotals
// These are assumed to be defined in testcms2.c as globals.
extern "C" unsigned int TotalMemory;
extern "C" unsigned int MaxAllocated;
extern "C" unsigned int SingleHit;

// Expose the focal function
extern "C" void DebugMemPrintTotals(void);

// Wrap the focal function in a C-linkage wrapper to obtain a plain function pointer
extern "C" void DebugMemPrintTotals_Wrapper(void) {
    DebugMemPrintTotals();
}

// A small wrapper function to obtain a plain void (*)() compatible call
static void DebugMemPrintTotals_Call() {
    DebugMemPrintTotals();
}

// Utility: capture stdout produced by calling a void function with no args.
// Uses POSIX pipe/dup2 to redirect stdout temporarily.
static std::string capture_stdout(void (*func)()) {
    // Save original stdout
    int stdout_fd = dup(fileno(stdout));
    if (stdout_fd == -1) {
        return "";
    }

    // Create a pipe to read output
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // Restore stdout before returning
        close(stdout_fd);
        return "";
    }

    // Redirect stdout to pipe's write end
    if (dup2(pipefd[1], fileno(stdout)) == -1) {
        close(pipefd[0]);
        close(pipefd[1]);
        close(stdout_fd);
        return "";
    }

    // Close unused write end in this process (stdout now points to pipe)
    close(pipefd[1]);

    // Execute the function that writes to stdout
    func();

    // Flush and restore stdout
    fflush(stdout);
    if (dup2(stdout_fd, fileno(stdout)) == -1) {
        // Cleanup and return
        close(pipefd[0]);
        close(stdout_fd);
        return "";
    }

    // Read the captured output from the pipe
    std::string output;
    char buffer[4096];
    ssize_t n;
    // The read should occur from the pipe's read end
    while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, static_cast<size_t>(n));
    }

    // Cleanup
    close(pipefd[0]);
    close(stdout_fd);

    return output;
}

// Forward declarations of tests
static int test_DebugMemPrintTotals_basic();
static int test_DebugMemPrintTotals_zero();

// Main test runner
int main(int argc, char* argv[]) {
    // Test 1: Basic values
    int failures = 0;

    // Run test 1
    int t1 = test_DebugMemPrintTotals_basic();
    if (t1 != 0) {
        std::fprintf(stderr, "test_DebugMemPrintTotals_basic: FAILED (code=%d)\n", t1);
        ++failures;
    } else {
        std::printf("test_DebugMemPrintTotals_basic: PASSED\n");
    }

    // Run test 2
    int t2 = test_DebugMemPrintTotals_zero();
    if (t2 != 0) {
        std::fprintf(stderr, "test_DebugMemPrintTotals_zero: FAILED (code=%d)\n", t2);
        ++failures;
    } else {
        std::printf("test_DebugMemPrintTotals_zero: PASSED\n");
    }

    if (failures == 0) {
        std::printf("ALL TESTS PASSED\n");
    } else {
        std::printf("%d test(s) FAILED\n", failures);
    }

    return failures;
}

// Test 1: Verify that DebugMemPrintTotals prints the header and the expected values
// under normal non-zero memory statistics.
static int test_DebugMemPrintTotals_basic() {
    // Setup: assign representative non-zero values
    TotalMemory  = 123u;
    MaxAllocated = 456u;
    SingleHit    = 7u;

    // Capture stdout while invoking the focal function
    std::string output = capture_stdout(DebugMemPrintTotals_Call);

    // Explanatory comment:
// We expect at least the header line and the exact values line to be present.
// The function prints exactly two lines:
// [Memory statistics]
// Allocated = <TotalMemory> MaxAlloc = <MaxAllocated> Single block hit = <SingleHit>
    if (output.find("[Memory statistics]") == std::string::npos) {
        std::fprintf(stderr, "Expected header '[Memory statistics]' not found in output.\n");
        return 1;
    }

    char expectedLine[128];
    std::snprintf(expectedLine, sizeof(expectedLine),
                  "Allocated = %u MaxAlloc = %u Single block hit = %u",
                  TotalMemory, MaxAllocated, SingleHit);

    if (output.find(expectedLine) == std::string::npos) {
        std::fprintf(stderr, "Expected values line not found. Looking for: %s\n", expectedLine);
        std::fprintf(stderr, "Actual output:\n%s\n", output.c_str());
        return 2;
    }

    // If we reach here, the test passes
    return 0;
}

// Test 2: Verify that DebugMemPrintTotals handles zero values correctly.
static int test_DebugMemPrintTotals_zero() {
    // Setup: zero statistics
    TotalMemory  = 0u;
    MaxAllocated = 0u;
    SingleHit    = 0u;

    // Capture stdout
    std::string output = capture_stdout(DebugMemPrintTotals_Call);

    // Explanatory comment:
// Ensure both header and zero-values line are formatted as expected.
    if (output.find("[Memory statistics]") == std::string::npos) {
        std::fprintf(stderr, "Zero-values test: header missing.\n");
        return 1;
    }

    char expectedLine[128];
    std::snprintf(expectedLine, sizeof(expectedLine),
                  "Allocated = %u MaxAlloc = %u Single block hit = %u",
                  TotalMemory, MaxAllocated, SingleHit);

    if (output.find(expectedLine) == std::string::npos) {
        std::fprintf(stderr, "Zero-values test: values line mismatch. Expected: %s\n", expectedLine);
        std::fprintf(stderr, "Actual output:\n%s\n", output.c_str());
        return 2;
    }

    return 0;
}