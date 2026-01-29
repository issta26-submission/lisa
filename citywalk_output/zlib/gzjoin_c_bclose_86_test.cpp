// C++11 test suite for the focal method bclose(bin *in) from gzjoin.c
// Notes:
// - This test uses a lightweight, self-contained harness (no Google Test).
// - To observe internal behavior (free/close calls), we provide wrappers for free() and close()
//   that intercept real libc calls via dlsym(RTLD_NEXT).
// - We declare an approximate bin structure compatible with the focal function's usage:
//     int fd; void *buf;  (Only the fields accessed by bclose are used.)

#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <cerrno>
#include <sys/stat.h>
#include <dlfcn.h>
#include <stdio.h>
#include <sys/types.h>
#include <vector>
#include <zlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>


// Forward declaration of the focal type used by bclose.
// We assume the minimal layout used by bclose: { int fd; void *buf; }
struct bin {
    int fd;
    void *buf;
};

// Declare the focal function. We assume it is accessible (extern "C").
// If gzjoin.c defines bin as a typedef, this declaration should remain compatible
// because 'struct bin' corresponds to the same layout.
extern "C" void bclose(struct bin* in);

// Global trackers to verify interactions inside bclose.
static std::vector<void*> freed_ptrs; // pointers passed to free()
static int close_call_count = 0;       // number of times close() was invoked
static int last_closed_fd = -1;        // last fd passed to close()

// Real libc function pointers resolved dynamically to avoid recursion/cycles.
static int (*real_close)(int) = nullptr;
static void (*real_free)(void*) = nullptr;

// Wrapper for free() to observe which pointers are freed by bclose.
extern "C" void free(void* p) {
    if (!real_free) {
        real_free = (void (*)(void*))dlsym(RTLD_NEXT, "free");
        if (!real_free) {
            fprintf(stderr, "Error: failed to locate real free() symbol\n");
            _exit(1);
        }
    }
    // Track the freed pointer for assertions in tests.
    freed_ptrs.push_back(p);
    // Call the real free to actually release memory.
    real_free(p);
}

// Wrapper for close() to observe which file descriptors are closed.
extern "C" int close(int fd) {
    if (!real_close) {
        real_close = (int (*)(int))dlsym(RTLD_NEXT, "close");
        if (!real_close) {
            fprintf(stderr, "Error: failed to locate real close() symbol\n");
            _exit(1);
        }
    }
    int ret = real_close(fd);
    close_call_count++;
    last_closed_fd = fd;
    return ret;
}

// Utility to reset test trackers between tests.
static void reset_trackers() {
    freed_ptrs.clear();
    close_call_count = 0;
    last_closed_fd = -1;
}

// Test Case 1: Null pointer input should be a no-op (no frees or closes).
static bool test_null_pointer() {
    reset_trackers();
    bclose(nullptr); // should safely do nothing
    bool ok = freed_ptrs.empty() && close_call_count == 0;
    if (!ok) {
        printf("Test 1 FAILED: expected no frees/closes on NULL input, got frees=%zu, closes=%d\n",
               freed_ptrs.size(), close_call_count);
    } else {
        printf("Test 1 PASSED\n");
    }
    return ok;
}

// Test Case 2: in != NULL, with in->fd == -1 and in->buf == NULL
// Expect: free(in) is called exactly once; no close() calls or free(in->buf).
static bool test_in_not_null_but_no_resources() {
    reset_trackers();
    struct bin* in = (struct bin*)malloc(sizeof(struct bin));
    if (!in) {
        printf("Test 2 FAILED: malloc failed\n");
        return false;
    }
    in->fd = -1;
    in->buf = NULL;

    bclose(in);

    // Expect one free() call for 'in' pointer, no frees for in->buf
    bool ok = (freed_ptrs.size() == 1) && (freed_ptrs[0] == in) && (close_call_count == 0);
    if (!ok) {
        printf("Test 2 FAILED: expected 1 free(in) and 0 closes; frees=%zu, close_calls=%d\n",
               freed_ptrs.size(), close_call_count);
    } else {
        printf("Test 2 PASSED\n");
    }

    // In a real scenario, the memory is freed by bclose; don't touch 'in' further.
    // Clean up a dangling allocation was performed inside bclose; guard against leaks:
    // nothing to free here.

    return ok;
}

// Test Case 3: in != NULL with in->fd set to a valid (open) fd.
// Expect: close(fd) is called for that descriptor (we verify by attempting a write after close).
static bool test_close_called_for_fd() {
    reset_trackers();

    // Create a pipe to have a valid write end fd we can attempt to operate on after close.
    int fds[2];
    if (pipe(fds) != 0) {
        printf("Test 3 FAILED: pipe() failed with errno=%d\n", errno);
        return false;
    }

    struct bin* in = (struct bin*)malloc(sizeof(struct bin));
    if (!in) {
        printf("Test 3 FAILED: malloc failed\n");
        // Cleanup pipe
        close(fds[0]);
        close(fds[1]);
        return false;
    }
    in->fd = fds[1]; // write end
    in->buf = NULL;

    // Call bclose, which should close the write end (fds[1]).
    bclose(in);

    // Now, attempt to write to the write end. Since bclose should have closed it,
    // write() should fail with EBADF.
    const char ch = 'x';
    ssize_t w = write(fds[1], &ch, 1);
    int err = errno;
    bool write_failed_with_EBADF = (w == -1 && err == EBADF);

    // Cleanup the still-open read end to avoid leaks.
    // The write end should be closed already; close whatever is left.
    close(fds[0]); // read end
    // Note: the write end (fds[1]) may already be closed by bclose.
    // If not closed, close() will clean up here.

    // Validate: close should have been invoked on the exact fd we set.
    bool ok = (close_call_count == 1) && (last_closed_fd == fds[1]) && write_failed_with_EBADF;
    if (!ok) {
        printf("Test 3 FAILED: expected 1 close on fd %d and EBADF on write after close; "
               "close_calls=%d, last_closed_fd=%d, write_errno=%d\n",
               fds[1], close_call_count, last_closed_fd, errno);
    } else {
        printf("Test 3 PASSED\n");
    }

    // Free test memory (in case bclose did not free in this scenario)
    // The test allocated in, which should have been freed by bclose if it followed the contract.
    // If for some reason the memory wasn't freed due to static linkage, guard against leaks here:
    // No direct access to in after bclose; ignore.

    return ok;
}

// Test Case 4: in != NULL with in->buf != NULL
// Expect: free(in->buf) and free(in) are called (two frees observed).
static bool test_free_buf_and_in() {
    reset_trackers();

    struct bin* in = (struct bin*)malloc(sizeof(struct bin));
    if (!in) {
        printf("Test 4 FAILED: malloc failed\n");
        return false;
    }
    in->fd = -1;
    in->buf = malloc(64);
    if (!(in->buf)) {
        free(in);
        printf("Test 4 FAILED: malloc for buf failed\n");
        return false;
    }

    bclose(in);

    // Expect two frees: free(in->buf) and free(in)
    bool ok = (freed_ptrs.size() == 2) &&
              ((freed_ptrs[0] == in->buf) || (freed_ptrs[1] == in->buf)) &&
              ((freed_ptrs[0] == in)     || (freed_ptrs[1] == in));

    if (!ok) {
        printf("Test 4 FAILED: expected two frees (buf and in); frees=%zu\n", freed_ptrs.size());
    } else {
        printf("Test 4 PASSED\n");
    }

    // No further cleanup required; memory was freed by bclose.
    return ok;
}

// Main runner to execute all tests in sequence and report summary.
int main() {
    printf("Starting unit tests for bclose(bin* in) with custom wrappers...\n");

    int total = 0;
    int passed = 0;

    struct {
        const char* name;
        bool (*fn)();
    } tests[] = {
        {"Null pointer input", test_null_pointer},
        {"Non-NULL input with no resources", test_in_not_null_but_no_resources},
        {"Close called for valid fd", test_close_called_for_fd},
        {"Free both in and buf", test_free_buf_and_in},
    };

    const int n_tests = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < n_tests; ++i) {
        printf("Running test %d: %s\n", i+1, tests[i].name);
        bool result = tests[i].fn();
        total++;
        if (result) {
            passed++;
        } else {
            // Detailed per-test failure message already printed inside test.
        }
    }

    printf("\nTest summary: %d/%d tests passed.\n", passed, total);
    return (passed == total) ? 0 : 1;
}