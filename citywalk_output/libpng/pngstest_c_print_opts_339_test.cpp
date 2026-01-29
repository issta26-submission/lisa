// Unit test suite for the focal method: print_opts(png_uint_32 opts)
// This test suite is written for C++11 and does not rely on GTest.
// It uses a small in-process harness to capture stdout outputs produced by the C function.
//
// Key ideas (mapped to Candidate Keywords from Step 1):
// - Flags tested: USE_FILE, USE_STDIO, STRICT, VERBOSE, KEEP_TMPFILES, KEEP_GOING, ACCUMULATE,
//   FAST_WRITE, sRGB_16BIT, NO_RESEED, GBG_ERROR (guarded by libpng version).
// - Behavior: prints corresponding option strings when bits are set/unset; default --slow
//   is printed when FAST_WRITE is not set; --nostrict is printed when STRICT is not set.
// - Order of printing is deterministic and fixed in the focal method.
// - Test approach: construct masks that exercise true/false branches and verify output substrings.
//   Tests are designed to be robust to the presence/absence of GBG_ERROR depending on PNG version.
// - Test harness: capture stdout via a POSIX pipe, invoke print_opts, then compare captured string.
//
// Note: This test assumes a POSIX environment for stdout capture (pipe and dup2).
// It declares the png_uint_32 type as uint32_t to keep test self-contained.

#include <functional>
#include <sys/types.h>
#include <stdio.h>
#include <math.h>
#include <cstdint>
#include <cassert>
#include <vector>
#include <stddef.h>
#include <cstdlib>
#include <unistd.h>
#include <cstdio>
#include <../tools/sRGB.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <iostream>
#include <cstring>
#include <ctype.h>


extern "C" {
    // The focal function is defined in pngstest.c with signature:
    // void print_opts(png_uint_32 opts);
    // We provide a compatible prototype using a local typedef for png_uint_32.
    typedef uint32_t png_uint_32;
    void print_opts(png_uint_32 opts);
}

// Expose a slightly higher-level wrapper to keep tests readable
static std::string capture_print_opts(png_uint_32 opts) {
    // Create a pipe to capture stdout
    int pipefd[2] = {-1, -1};
    if (pipe(pipefd) != 0) {
        perror("pipe");
        return "";
    }

    // Save current stdout
    int saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1) {
        perror("dup");
        // Cleanup pipe
        close(pipefd[0]);
        close(pipefd[1]);
        return "";
    }

    // Redirect stdout to the write end of the pipe
    if (dup2(pipefd[1], STDOUT_FILENO) == -1) {
        perror("dup2");
        close(pipefd[0]);
        close(pipefd[1]);
        close(saved_stdout);
        return "";
    }

    // Close write end in the parent after redirect
    close(pipefd[1]);

    // Invoke the focal function
    print_opts(opts);

    // Ensure all output is flushed
    fflush(stdout);

    // Read from the read end of the pipe
    std::string result;
    char buffer[4096];
    ssize_t n;
    // Restore original stdout before reading to avoid deadlocks if buffers are buffered
    if (dup2(saved_stdout, STDOUT_FILENO) == -1) {
        perror("dup2 restore");
        close(saved_stdout);
        close(pipefd[0]);
        return "";
    }
    close(saved_stdout);

    // Now read the captured data
    // We need to read from pipefd[0]; since we restored stdout, data is no longer
    // being written to the pipe. To properly capture, we must read before restoring.
    // Correct approach: perform the read BEFORE restoring stdout. We'll fix this below.

    // Corrected approach: read from pipe before restoring stdout
    // --- Rewind: Reopen/redirect to capture properly
    // To keep code robust, re-do the capture correctly:

    // Since we already restored stdout, the pipe might be empty. To salvage, reopen a new capture.
    // For simplicity and correctness, re-implement the capture from scratch:

    // Rebuild correct capture path:
    // Re-create a fresh pipe and re-run with careful sequencing.

    // Close any mis-handled descriptor (pipe read end)
    // The previous attempt may have left some state inconsistent; we'll implement a fresh capture path now.

    // Clean up potential leftovers from the previous attempt (best effort)
    // Note: In production code, we'd avoid this two-pass approach; here we ensure correctness.

    // New capture path (proper single pass):
    // We'll re-run with a separate scope to avoid confusion.

    // Begin new capture block (restart):
    // Re-create a new pipe
    close(pipefd[0]); // close read end from prior attempt
    int rpipe[2];
    if (pipe(rpipe) != 0) {
        perror("pipe2");
        return "";
    }

    int new_saved_stdout = dup(STDOUT_FILENO);
    if (new_saved_stdout == -1) {
        perror("dup2_new_saved");
        close(rpipe[0]);
        close(rpipe[1]);
        return "";
    }

    if (dup2(rpipe[1], STDOUT_FILENO) == -1) {
        perror("dup2_new_redirect");
        close(rpipe[0]);
        close(rpipe[1]);
        close(new_saved_stdout);
        return "";
    }
    close(rpipe[1]);

    // Call again
    print_opts(opts);
    fflush(stdout);

    // Restore stdout
    if (dup2(new_saved_stdout, STDOUT_FILENO) == -1) {
        perror("dup2_new_restore");
        close(rpipe[0]);
        close(new_saved_stdout);
        return "";
    }
    close(new_saved_stdout);

    // Read from pipe
    char buf[4096];
    ssize_t got;
    std::string out;
    while ((got = read(rpipe[0], buf, sizeof(buf))) > 0) {
        out.append(buf, (size_t)got);
    }
    close(rpipe[0]);
    return out;
}

// Simple test assertion helper (non-terminating)
static int g_failures = 0;
#define TEST_ASSERT(cond, msg) do { if(!(cond)) { std::cerr << "ASSERT FAILED: " msg "\n"; ++g_failures; } } while(0)
#define TEST_REQUIRE(cond) TEST_ASSERT(cond, #cond)

// Test 1: Behavior with no flags set (mask = 0)
// Expect --nostrict and --slow to appear; no other option strings should appear.
static void test_print_opts_zero_flags() {
    // Domain knowledge: When all bits are zero, the code prints:
    // --nostrict (because not STRICT) and --slow (because not FAST_WRITE)
    std::string out = capture_print_opts(0u);

    // Existence checks
    TEST_REQUIRE(out.find(" --nostrict") != std::string::npos);
    TEST_REQUIRE(out.find(" --slow") != std::string::npos);

    // Non-existence checks for a few known flags (they should not appear with mask 0)
    TEST_ASSERT(out.find(" --file") == std::string::npos, "0-mask should not print --file");
    TEST_ASSERT(out.find(" --stdio") == std::string::npos, "0-mask should not print --stdio");
    TEST_ASSERT(out.find(" --verbose") == std::string::npos, "0-mask should not print --verbose");
    TEST_ASSERT(out.find(" --preserve") == std::string::npos, "0-mask should not print --preserve");
    TEST_ASSERT(out.find(" --keep-going") == std::string::npos, "0-mask should not print --keep-going");
    TEST_ASSERT(out.find(" --accumulate") == std::string::npos, "0-mask should not print --accumulate");
    TEST_ASSERT(out.find(" --sRGB-16bit") == std::string::npos, "0-mask should not print --sRGB-16bit");
    TEST_ASSERT(out.find(" --noreseed") == std::string::npos, "0-mask should not print --noreseed");
}

// Test 2: Behavior with all known flags set except STRICT is cleared and FAST_WRITE is set
// This aims to exercise many true-branch prints while ensuring --nostrict appears and --slow does not.
// Assumptions:
// - USE_FILE -> 0x01, USE_STDIO -> 0x02, STRICT -> 0x04, VERBOSE -> 0x08,
//   KEEP_TMPFILES -> 0x10, KEEP_GOING -> 0x20, ACCUMULATE -> 0x40,
//   FAST_WRITE -> 0x80, sRGB_16BIT -> 0x100, NO_RESEED -> 0x200, GBG_ERROR -> 0x400.
// These are common values used in libpng tests; if your environment uses different values,
// adjust accordingly, but keep the intended bit relationships (nostrict depends on STRICT, slow depends on FAST_WRITE).
static void test_print_opts_all_flags_mixed() {
    const uint32_t MASK_ALL = 0xFFFFFFFFu;
    uint32_t opts = MASK_ALL;
    // Clear STRICT to ensure --nostrict is printed
    opts &= ~0x04u;
    // Set FAST_WRITE to avoid printing --slow
    opts |= 0x80u;

    std::string out = capture_print_opts(opts);

    // All the flag-driven strings should appear (except --slow which should be absent)
    std::vector<const char*> expected = {
        " --file",
        " --stdio",
        " --nostrict",      // STRICT is cleared
        " --verbose",
        " --preserve",
        " --keep-going",
        " --accumulate",
        // "--slow" should NOT appear due to FAST_WRITE being set
        " --sRGB-16bit",
        " --noreseed"
    };

    for (auto s : expected) {
        TEST_ASSERT(out.find(s) != std::string::npos, "Expected substring not found: " << s);
    }

    // Ensure --slow is not printed given FAST_WRITE is set
    TEST_ASSERT(out.find(" --slow") == std::string::npos, "--slow should not appear when FAST_WRITE is set");
}

// Entry point for the tests
int main(void) {
    // Run tests
    test_print_opts_zero_flags();
    test_print_opts_all_flags_mixed();

    // Summary
    if (g_failures == 0) {
        std::cerr << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed.\n";
        return 1;
    }
}

/*
Notes for maintainers and reviewers:
- This test suite relies on a POSIX environment to capture stdout via pipe and dup2.
- The test code uses a local typedef for png_uint_32 (uint32_t) to avoid dependencies on external libpng headers.
- The exact bit values for the option flags (USE_FILE, USE_STDIO, STRICT, etc.) are assumed to align with libpng's
  conventional values used in similar tests. If your build defines different values, adjust the constants accordingly
  and ensure the test masks reflect the intended true/false branches.
- GBG_ERROR handling is guarded by the libpng version in the focal code; tests do not strictly require its presence.
- The tests use non-terminating assertions (they accumulate failures and report at the end) to maximize code coverage.
- Static and private members are not involved here since print_opts is a C-style function; tests access it via an extern "C" prototype.
- The test harness is self-contained and does not rely on GoogleTest or other frameworks per the constraints.
*/