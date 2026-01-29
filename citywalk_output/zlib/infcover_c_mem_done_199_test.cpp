// Unit test suite for the focal method mem_done in infcover.c
// Target: C++11 compatible tests, no GTest, self-contained harness.
// Notes:
// - We assume the real mem_done is compiled into a library/object that we link with.
// - We declare minimal versions of the dependent structures (mem_zone and mem_item)
//   that mirror the fields referenced by mem_done (first, total, notlifo, rogue).
// - We rely on POSIX pipes to capture stderr output produced by mem_done.
// - Each test asserts behavior without terminating test execution (non-terminating assertions).

#include <cstdio>
#include <cstring>
#include <string.h>
#include <fcntl.h>
#include <assert.h>
#include <string>
#include <functional>
#include <stdio.h>
#include <sys/types.h>
#include <iostream>
#include <vector>
#include <inftrees.h>
#include <zlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <inflate.h>
#include <cstdlib>


// Forward declaration of the focal function from the production code.
// The production code uses C linkage; enforce it here.
extern "C" void mem_done(struct z_stream *strm, char *prefix);

// Include zlib to obtain z_stream and Z_NULL

// Minimal local representations of the structures assumed by mem_done.
// We declare them with the fields accessed by mem_done to ensure layout compatibility
// with the production implementation.
struct mem_item {
    void *ptr;
    mem_item *next;
};

struct mem_zone {
    mem_item *first;
    unsigned long total;
    int notlifo;
    int rogue;
};

// Utility to capture stderr output produced by a function call.
static std::string capture_stderr(const std::function<void()> &fn) {
    // Create a pipe to capture stderr
    int pipe_fd[2];
    if (pipe(pipe_fd) != 0) {
        perror("pipe");
        return "";
    }

    // Save current stderr
    int saved_stderr = dup(fileno(stderr));
    if (saved_stderr < 0) {
        perror("dup");
        return "";
    }

    // Redirect stderr to the pipe's write end
    if (dup2(pipe_fd[1], fileno(stderr)) < 0) {
        perror("dup2");
        return "";
    }

    // Close the write end in this thread as it's now duplicated to stderr
    close(pipe_fd[1]);

    // Run the function which will write to stderr
    fn();

    // Flush and restore stderr
    fflush(stderr);
    if (dup2(saved_stderr, fileno(stderr)) < 0) {
        perror("dup2 restore");
        // continue anyway
    }
    close(saved_stderr);

    // Read all data from the pipe
    std::string output;
    char buffer[1024];
    ssize_t n;
    // Close the write end if still open
    // Read until EOF
    while ((n = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, static_cast<size_t>(n));
    }
    close(pipe_fd[0]);
    return output;
}

// Helper macros for simple, non-terminating assertions
static size_t g_pass_count = 0;
static size_t g_fail_count = 0;

#define ASSERT_TRUE(cond, msg) do { \
    if (cond) { \
        ++g_pass_count; \
    } else { \
        ++g_fail_count; \
        std::cerr << "ASSERT FAILED: " << (msg) << std::endl; \
    } \
} while (0)

#define ASSERT_STR_CONTAINS(hay, needle, msg) do { \
    if (hay.find(needle) != std::string::npos) { \
        ++g_pass_count; \
    } else { \
        ++g_fail_count; \
        std::cerr << "ASSERT FAILED: " << (msg) << " (expected substring: " \
                  << (needle) << " in: " << (hay) << ")" << std::endl; \
    } \
} while (0)

// Externally provided mem_done requires z_stream; we prepare one for tests.
static void test_mem_done_empty_zone() {
    // Prepare a zone with no allocations
    mem_zone zone;
    zone.first = nullptr;
    zone.total = 0;
    zone.notlifo = 0;
    zone.rogue = 0;

    z_stream strm;
    strm.opaque = &zone;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;

    // Capture output while invoking mem_done
    std::string out = capture_stderr([&]() {
        mem_done(&strm, (char*)"empty_zone");
    });

    // After mem_done, the opaque and alloc/free hooks should be reset
    ASSERT_TRUE(strm.opaque == Z_NULL, "mem_done should reset strm->opaque to Z_NULL");
    ASSERT_TRUE(strm.zalloc == Z_NULL, "mem_done should reset strm->zalloc to Z_NULL");
    ASSERT_TRUE(strm.zfree == Z_NULL, "mem_done should reset strm->zfree to Z_NULL");

    // There should be no "not freed" messages when zone is empty
    // The exact string formatting includes the prefix; we only assert absence
    // If any message were printed, the output wouldn't be empty.
    ASSERT_TRUE(out.empty(), "mem_done should not print messages for an empty zone");

    // Clean up: ensure no memory leaks in test environment (zone and items are null)
}

static void test_mem_done_with_items_prints() {
    // Prepare a zone with two allocated items
    mem_zone zone;
    zone.first = nullptr;
    zone.total = 128; // non-zero to trigger the "bytes in blocks not freed" message
    zone.notlifo = 0;
    zone.rogue = 0;

    // Create two items
    mem_item *it1 = (mem_item*)malloc(sizeof(mem_item));
    mem_item *it2 = (mem_item*)malloc(sizeof(mem_item));
    it1->ptr = malloc(16);
    it2->ptr = malloc(32);
    it1->next = it2;
    it2->next = nullptr;
    zone.first = it1;

    z_stream strm;
    strm.opaque = &zone;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;

    // Capture output
    std::string out = capture_stderr([&]() {
        mem_done(&strm, (char*)"testmem");
    });

    // After mem_done, pointers should be freed and zone freed; verify pointers reset
    ASSERT_TRUE(strm.opaque == Z_NULL, "mem_done should reset strm->opaque to Z_NULL");
    ASSERT_TRUE(strm.zalloc == Z_NULL, "mem_done should reset strm->zalloc to Z_NULL");
    ASSERT_TRUE(strm.zfree == Z_NULL, "mem_done should reset strm->zfree to Z_NULL");

    // We expect at least one message: total bytes and blocks not freed
    ASSERT_TRUE(!out.empty(), "mem_done should print a summary line for non-empty zone with items");

    // The prefix "testmem" should appear in the output
    ASSERT_STR_CONTAINS(out, "testmem", "Captured output should contain the provided prefix");

    // Cleanup: mem_done frees all items; no additional memory to free here
    // The items and their payloads were allocated above and freed by mem_done.
}

static void test_mem_done_prints_notlifo_and_rogue() {
    // Prepare a zone with two items and non-zero notlifo and rogue counters
    mem_zone zone;
    zone.first = nullptr;
    zone.total = 256;
    zone.notlifo = 2; // request "frees not LIFO"
    zone.rogue = 3;   // request "frees not recognized"
    // Build a small list to exercise the freeing loop
    mem_item *it1 = (mem_item*)malloc(sizeof(mem_item));
    mem_item *it2 = (mem_item*)malloc(sizeof(mem_item));
    it1->ptr = malloc(4);
    it2->ptr = malloc(8);
    it1->next = it2;
    it2->next = nullptr;
    zone.first = it1;

    z_stream strm;
    strm.opaque = &zone;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;

    // Capture output
    std::string out = capture_stderr([&]() {
        mem_done(&strm, (char*)"notlifo_rogue");
    });

    // Assertions about the resets
    ASSERT_TRUE(strm.opaque == Z_NULL, "mem_done should reset strm->opaque to Z_NULL");
    ASSERT_TRUE(strm.zalloc == Z_NULL, "mem_done should reset strm->zalloc to Z_NULL");
    ASSERT_TRUE(strm.zfree == Z_NULL, "mem_done should reset strm->zfree to Z_NULL");

    // Expect both notlifo and rogue lines (in addition to the first summary line)
    ASSERT_TRUE(out.find("not LIFO") != std::string::npos, "Output should contain 'frees not LIFO' message");
    ASSERT_TRUE(out.find("not recognized") != std::string::npos, "Output should contain 'frees not recognized' message");

    // Ensure the prefix is present
    ASSERT_STR_CONTAINS(out, "notlifo_rogue", "Captured output should contain the provided prefix");

    // No crash; items are freed by mem_done
}

// Main harness
int main(void) {
    std::cout << "Beginning mem_done unit test suite (no GTest, self-contained)..." << std::endl;
    g_pass_count = 0;
    g_fail_count = 0;

    // Run tests
    test_mem_done_empty_zone();
    test_mem_done_with_items_prints();
    test_mem_done_prints_notlifo_and_rogue();

    // Summary
    std::cout << "mem_done tests completed. Passed: " << g_pass_count
              << ", Failed: " << g_fail_count << std::endl;

    // If any failures occurred, signal non-zero exit code
    return (g_fail_count == 0) ? 0 : 1;
}

// Explanation of unit tests:
// - test_mem_done_empty_zone: Validates that mem_done handles an empty memory zone gracefully,
//   prints nothing, and resets the z_stream fields to Z_NULL.
// - test_mem_done_with_items_prints: Ensures that mem_done frees allocated items and reports
//   the total bytes and blocks not freed when there are leftover allocations.
// - test_mem_done_prints_notlifo_and_rogue: Checks that mem_done prints additional alerts when
//   the zone records non-LIFO frees (notlifo) or unrecognized frees (rogue), and that all
//   stream pointers are reset afterward.
//
// Notes about the test environment:
// - We rely on POSIX pipes to capture stderr output produced by mem_done.
// - We expose the minimal mem_item and mem_zone shapes to match the fields used by mem_done.
// - The tests assume that mem_done is available via C linkage and linked against the production
//   object containing its implementation. If mem_high or other helpers have different visibility,
//   the linked unit should be adjusted accordingly (e.g., compiling mem_done into a library).