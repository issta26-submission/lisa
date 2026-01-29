// Test suite for the focal method: image_transform_reset_count
// This test targets the behavior of the reset routine on the global
// image_transform linked list used by pngvalid.c.
// The tests assume the public API and structures defined in libpng/pngvalid.h
// are available. We exercise normal paths and the abort path when there are
// more than 32 transforms in the list.
//
// Notes:
// - We rely on the actual image_transform struct layout provided by <png.h>.
// - We manipulate the global list via image_transform_first and image_transform_end.
// - For the overflow case (N = 33), we run the test in a forked child process
//   to capture abort() behavior without terminating the test runner.

#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <float.h>
#include <cstdlib>
#include <stdlib.h>
#include <cstdio>
#include <sys/wait.h>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <math.h>


// We will use the actual library-provided types. The header <png.h> is expected
// to declare: typedef struct image_transform image_transform; extern image_transform *image_transform_first;
// extern image_transform image_transform_end; and the function
// void image_transform_reset_count(void);
// We declare them here again to ensure linkage with C symbols.
extern "C" {
    void image_transform_reset_count(void);
    extern image_transform *image_transform_first;
    extern image_transform image_transform_end;
}

// Very lightweight test harness (non-terminating assertions)
static int g_test_failures = 0;
#define TEST_EXPECT_INT_EQ(actual, expected, desc) do { \
    if ((actual) != (expected)) { \
        fprintf(stderr, "TEST FAIL: %s | expected %d, got %d\n", (desc), (int)(expected), (int)(actual)); \
        ++g_test_failures; \
    } \
} while (0)

#define TEST_EXPECT_PTR_EQ(actual, expected, desc) do { \
    if ((actual) != (expected)) { \
        fprintf(stderr, "TEST FAIL: %s | expected pointer %p, got %p\n", (desc), (void*)(expected), (void*)(actual)); \
        ++g_test_failures; \
    } \
} while (0)

static void report_summary(const char* name) {
    if (g_test_failures == 0) {
        printf("[PASS] %s\n", name);
    } else {
        printf("[FAIL] %s -- %d failure(s)\n", name, g_test_failures);
        g_test_failures = 0;
    }
}

// Helper to run a test with a specific number of manually constructed transforms.
// We directly allocate an array of image_transform nodes, link them via the 'list'
// field, and set 'local_use' and 'next' to non-zero to verify they are cleared.
// The last node's 'list' points to the sentinel &image_transform_end.
static void run_construct_and_test_chain(size_t n) {
    // Allocate n transforms
    image_transform *nodes = static_cast<image_transform*>(malloc(n * sizeof(image_transform)));
    if (!nodes && n > 0) {
        fprintf(stderr, "Allocation failed for %zu transforms\n", n);
        g_test_failures++;
        return;
    }

    // Build the chain: nodes[i].list -> nodes[i+1] ... ; last -> &image_transform_end
    for (size_t i = 0; i < n; ++i) {
        if (i + 1 < n) {
            nodes[i].list = &nodes[i + 1];
        } else {
            nodes[i].list = &image_transform_end;
        }
        // Pre-fill with non-zero values to ensure the reset actually changes them
        nodes[i].local_use = 5 + (int)i;
        nodes[i].next = reinterpret_cast<image_transform*>(0xDEADBEEF);
    }

    // Point the public head to the first transform
    image_transform_first = (n > 0) ? &nodes[0] : &image_transform_end;

    // Call the function under test
    image_transform_reset_count();

    // Validate: every transform in the list should have local_use = 0 and next = NULL
    for (size_t i = 0; i < n; ++i) {
        TEST_EXPECT_INT_EQ(nodes[i].local_use, 0, "local_use should be reset to 0");
        TEST_EXPECT_INT_EQ(reinterpret_cast<int>(nodes[i].next), 0, "next should be cleared to null");
    }

    free(nodes);
}

// Test 1: Zero transforms (edge case)
static void test_zero_transforms() {
    // Point to the sentinel immediately (empty list)
    image_transform_first = &image_transform_end;

    // Expect no abort and no changes (should exit cleanly)
    image_transform_reset_count();

    // If we reached here, zero transforms case passed (no crash)
    printf("[INFO] test_zero_transforms completed (no transforms).\n");
    report_summary("test_zero_transforms");
}

// Test 2: A small number of transforms (e.g., 5, within allowed range)
static void test_small_chain() {
    run_construct_and_test_chain(5);
    report_summary("test_small_chain (5 transforms)");
}

// Test 3: Maximum allowed transforms (32 transforms) should not abort and should reset
static void test_max_allowed_chain() {
    run_construct_and_test_chain(32);
    report_summary("test_max_allowed_chain (32 transforms)");
}

// Test 4: Overflow case (33 transforms) should abort.
// We run this test in a separate forked process to safely detect abort without killing the test suite.
static void test_overflow_abort() {
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork() failed\n");
        g_test_failures++;
        report_summary("test_overflow_abort");
        return;
    }

    if (pid == 0) {
        // Child process: construct 33 transforms and call the reset function.
        image_transform *nodes = static_cast<image_transform*>(malloc(33 * sizeof(image_transform)));
        if (!nodes) {
            _exit(1);
        }
        for (size_t i = 0; i < 33; ++i) {
            if (i + 1 < 33) nodes[i].list = &nodes[i + 1];
            else nodes[i].list = &image_transform_end;
            nodes[i].local_use = 9 + (int)i;
            nodes[i].next = reinterpret_cast<image_transform*>(0xBADC0DE);
        }
        image_transform_first = &nodes[0];

        // This should abort (count > 32)
        image_transform_reset_count();

        // If it returns, exit with success (we expect abort, so this path is unexpected)
        _exit(0);
    } else {
        // Parent: wait for child
        int status = 0;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGABRT) {
            printf("[PASS] test_overflow_abort: child aborted as expected (SIGABRT).\n");
            report_summary("test_overflow_abort");
        } else if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            fprintf(stderr, "[FAIL] test_overflow_abort: child exited without abort (unexpected).\n");
            g_test_failures++;
            report_summary("test_overflow_abort");
        } else {
            fprintf(stderr, "[FAIL] test_overflow_abort: child terminated abnormally.\n");
            g_test_failures++;
            report_summary("test_overflow_abort");
        }
    }
}

// Entry point
int main(void) {
    // Initialize a clean test environment message
    printf("Running tests for image_transform_reset_count (pngvalid.c focal method)\n");

    // Run tests
    test_zero_transforms();
    test_small_chain();
    test_max_allowed_chain();
    test_overflow_abort();

    // Final summary
    if (g_test_failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("SOME TESTS FAILED: %d failure(s) observed\n", g_test_failures);
        return 1;
    }
}