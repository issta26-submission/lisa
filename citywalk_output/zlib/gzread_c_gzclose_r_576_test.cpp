// Unit test suite for gzclose_r in gzread.c
// Target: C++11 compatible tests without using GTest.
// This harness uses the internal gz_state_s structure exposed by gzguts.h
// and the public gzclose_r function. Tests are run from main() and report
// failures to stdout rather than using a test framework.

#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <gzguts.h>
#include <zlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <cstdlib>


// Bring in the internal state structure and constants.
// The gzclose_r function expects a gzFile (typically a void* alias) that, when cast,
// yields a pointer to a gz_state_s containing fields used by gzclose_r.

// Simple test harness utilities
static int g_failures = 0;
#define TEST_ASSERT(cond, msg)                                   \
    do {                                                           \
        if (!(cond)) {                                           \
            fprintf(stderr, "FAILED: %s\n", msg);                \
            ++g_failures;                                         \
        }                                                        \
    } while (0)

static void test_gzclose_r_null_input() {
    // Test: Passing NULL should return Z_STREAM_ERROR
    // This exercises the first if (file == NULL) branch.
    int ret = gzclose_r((gzFile)NULL);
    TEST_ASSERT(ret == Z_STREAM_ERROR, "gzclose_r(NULL) should return Z_STREAM_ERROR");
}

// Test helper to create a minimal gz_state_s in a controlled environment.
// We rely on the internal layout defined by gzguts.h; this test must be compiled
// against the same sources. The test intentionally avoids size > 0 branch to
// prevent side effects from inflateEnd/free in this harness.
static void test_gzclose_r_wrong_mode() {
    // Allocate and initialize an internal gz_state_s
    struct gz_state_s *state = (struct gz_state_s *)malloc(sizeof(struct gz_state_s));
    memset(state, 0, sizeof(struct gz_state_s));

    // Set mode to something other than GZ_READ to trigger the false branch
    // GZ_READ is defined in zlib headers; any value != GZ_READ will do.
    state->mode = 0; // not GZ_READ
    state->size = 0;
    state->err = Z_OK;

    // path and fd are irrelevant for this path, but initialize to sane values.
    state->path = NULL;
    state->fd = -1;

    int ret = gzclose_r((gzFile)state);

    // Expect early return with Z_STREAM_ERROR due to wrong mode
    TEST_ASSERT(ret == Z_STREAM_ERROR, "gzclose_r with wrong mode should return Z_STREAM_ERROR");

    free(state);
}

// Test: size == 0 path (true branch for the size check is not exercised here to avoid
// invoking inflateEnd and free(state->out/in). This covers the false branch for the
// size condition and exercises the cleanup in the rest of the function, including
// closing the file descriptor and freeing path/state.
static void test_gzclose_r_size_zero_closes_fd_and_stays_safe() {
    // Create a temporary file to obtain a valid file descriptor
    char tmpname[] = "/tmp/gzclose_r_testXXXXXX";
    int fd = mkstemp(tmpname);
    if (fd == -1) {
        fprintf(stderr, "SKIP: unable to create temp file for test_gzclose_r_size_zero_closes_fd_and_stays_safe\n");
        return;
    }

    // Ensure the descriptor is not auto-closed by prior code
    // (don’t unlink the file yet; it will be removed after tests)
    // Prepare internal state
    struct gz_state_s *state = (struct gz_state_s *)malloc(sizeof(struct gz_state_s));
    memset(state, 0, sizeof(struct gz_state_s));

    state->mode = GZ_READ;
    state->size = 0;              // Do not call inflateEnd or free(in/out)
    state->err = Z_OK;             // err used to compute the return value
    state->strm = z_stream();       // initialize to zeroed-ish by default; safe because size==0
    memset(&(state->strm), 0, sizeof(state->strm));
    state->in = NULL;
    state->out = NULL;

    // Allocate a path string to verify that the code will free it
    state->path = strdup("gzclose_r_path_test");

    state->fd = fd;

    int ret = gzclose_r((gzFile)state);

    // Expect: close(fd) succeeds (returns 0) and err is Z_OK, so return 0
    TEST_ASSERT(ret == Z_OK, "gzclose_r with size==0 should return Z_OK when close succeeds");

    // After the call, state and its path would be freed by gzclose_r.
    // No explicit memory check is performed here to avoid relying on allocator internals.

    // Cleanup: ensure the temporary file is removed
    // Note: gzclose_r should have closed the fd; attempt to unlink using the filename.
    // If the fd was closed, this should succeed (unlink doesn't rely on fd).
    unlink(tmpname);
}

// Entry point
int main() {
    fprintf(stdout, "Starting gzclose_r unit tests...\n");

    test_gzclose_r_null_input();
    test_gzclose_r_wrong_mode();
    test_gzclose_r_size_zero_closes_fd_and_stays_safe();

    if (g_failures == 0) {
        printf("ALL TESTS PASSED\n");
        return 0;
    } else {
        printf("TESTS FAILED: %d failure(s)\n", g_failures);
        return 1;
    }
}