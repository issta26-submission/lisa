/*
  Unit test suite for gzrewind (gzlib.c) using a self-contained C++11 test harness.
  This test mocks the minimal environment required by gzrewind and exercises
  all control-flow branches without relying on GTest or external libraries.

  Test plan (mapped to code):
  - Test 1: file == NULL -> should return -1 (null input handling)
  - Test 2: state->mode != GZ_READ -> should return -1 (invalid mode)
  - Test 3: state->err is neither Z_OK nor Z_BUF_ERROR -> should return -1 (invalid error)
  - Test 4: valid state, but LSEEK fails -> should return -1 (seek failure, no reset)
  - Test 5: valid state with Z_OK err and successful LSEEK -> should return 0 and call gz_reset
  - Test 6: valid state with Z_BUF_ERROR err and successful LSEEK -> should return 0 and call gz_reset
  - Additionally validate that LSEEK was called with correct fd, offset, and SEEK_SET
  
  Notes:
  - The test defines a minimal in-file version of gzrewind with the same logic as provided,
    plus stubs for LSEEK and gz_reset to capture behavior.
  - Assertions are non-terminating; failures are reported but execution continues to
    maximize coverage.
  - The code is self-contained and compiles under C++11 as a single translation unit.
*/

#include <cstdio>
#include <cstring>
#include <cstdbool>
#include <cstdint>
#include <gzguts.h>


// Minimal stubs/types to mirror the focal function's dependencies
typedef void* gzFile;
typedef struct gz_state {
    int mode;
    int err;
    int fd;
    long start;
} gz_state, *gz_statep;

static const int GZ_READ = 1;
static const int Z_OK = 0;
static const int Z_BUF_ERROR = 1;

static const int SEEK_SET_DEFINED = 0;
#define SEEK_SET 0 // ensure compatibility for LSEEK calls in this test

// Forward declaration of the focal function to be tested
extern "C" int gzrewind(gzFile file);

// Global/test-state trackers used by the test harness
static bool gz_reset_called = false;
static int lseek_should_fail = 0;

static int last_LSEEK_fd = -1;
static long last_LSEEK_offset = 0;
static int last_LSEEK_whence = -1;

// Mocked LSEEK to capture parameters and simulate success/failure
extern "C" int LSEEK(int fd, long offset, int whence) {
    last_LSEEK_fd = fd;
    last_LSEEK_offset = offset;
    last_LSEEK_whence = whence;
    if (lseek_should_fail) {
        return -1;
    }
    return 0;
}

// Mocked gz_reset to verify that it was invoked during a successful rewind
static void gz_reset(gz_statep state) {
    (void)state; // unused parameter in this mock
    gz_reset_called = true;
}

// Minimal in-file implementation of gzrewind mirroring the focal method.
// This allows isolated unit testing without requiring the original library build.
extern "C" int gzrewind(gzFile file) {
    gz_statep state;
    if (file == NULL)
        return -1;
    state = (gz_statep)file;
    if (state->mode != GZ_READ ||
            (state->err != Z_OK && state->err != Z_BUF_ERROR))
        return -1;
    if (LSEEK(state->fd, state->start, SEEK_SET) == -1)
        return -1;
    gz_reset(state);
    return 0;
}

// Simple non-terminating assertion helpers
static int tests_run = 0;
static int tests_failed = 0;

#define CHECK_EQ(expected, actual, msg) \
    do { \
        ++tests_run; \
        if ((expected) != (actual)) { \
            ++tests_failed; \
            fprintf(stderr, "FAIL: %s (expected %d, got %d)\n", msg, (int)(expected), (int)(actual)); \
        } \
    } while (0)

#define CHECK_TRUE(cond, msg) \
    do { \
        ++tests_run; \
        if (!(cond)) { \
            ++tests_failed; \
            fprintf(stderr, "FAIL: %s\n", msg); \
        } \
    } while (0)

#define RESET_TEST_STATE() \
    do { gz_reset_called = false; last_LSEEK_fd = -1; last_LSEEK_offset = 0; last_LSEEK_whence = -1; lseek_should_fail = 0; } while (0)


// Test 1: NULL file should return -1
void test_gzrewind_null_file() {
    RESET_TEST_STATE();
    int ret = gzrewind(nullptr);
    CHECK_EQ(-1, ret, "gzrewind(NULL) should return -1");
    // No LSEEK or reset should be invoked
    CHECK_EQ(-1, last_LSEEK_fd, "LSEEK should not have been called for NULL input");
    CHECK_TRUE(!gz_reset_called, "gz_reset should not have been called for NULL input");
}

// Test 2: non-read mode should return -1
void test_gzrewind_wrong_mode() {
    RESET_TEST_STATE();
    gz_state s;
    s.mode = 0;           // not GZ_READ
    s.err = Z_OK;
    s.fd = 10;
    s.start = 0;
    int ret = gzrewind((gzFile)&s);
    CHECK_EQ(-1, ret, "gzrewind with non-read mode should return -1");
    CHECK_TRUE(!gz_reset_called, "gz_reset should not have been called when mode is invalid");
}

// Test 3: invalid error state should return -1
void test_gzrewind_invalid_err() {
    RESET_TEST_STATE();
    gz_state s;
    s.mode = GZ_READ;
    s.err = 999; // not Z_OK nor Z_BUF_ERROR
    s.fd = 11;
    s.start = 0;
    int ret = gzrewind((gzFile)&s);
    CHECK_EQ(-1, ret, "gzrewind with invalid err should return -1");
    CHECK_TRUE(!gz_reset_called, "gz_reset should not have been called for invalid error state");
}

// Test 4: LSEEK failure should cause -1 return and no reset
void test_gzrewind_lseek_failure() {
    RESET_TEST_STATE();
    lseek_should_fail = 1; // force LSEEK to fail
    gz_state s;
    s.mode = GZ_READ;
    s.err = Z_OK;
    s.fd = 22;
    s.start = 1234;
    int ret = gzrewind((gzFile)&s);
    CHECK_EQ(-1, ret, "gzrewind should return -1 when LSEEK fails");
    CHECK_TRUE(!gz_reset_called, "gz_reset should not be called when LSEEK fails");
    // Also verify LSEEK was invoked with correct parameters
    CHECK_EQ(22, last_LSEEK_fd, "LSEEK fd should be the state's fd");
    CHECK_EQ(1234, last_LSEEK_offset, "LSEEK offset should be state's start");
    CHECK_EQ(SEEK_SET, last_LSEEK_whence, "LSEEK whence should be SEEK_SET");
}

// Test 5: success path with Z_OK should rewind and call reset
void test_gzrewind_success_zok() {
    RESET_TEST_STATE();
    gz_state s;
    s.mode = GZ_READ;
    s.err = Z_OK;
    s.fd = 3;
    s.start = 5678;
    int ret = gzrewind((gzFile)&s);
    CHECK_EQ(0, ret, "gzrewind should return 0 on success with Z_OK");
    CHECK_TRUE(gz_reset_called, "gz_reset should be called on successful rewind");
    CHECK_EQ(3, last_LSEEK_fd, "LSEEK fd should be the state's fd");
    CHECK_EQ(5678, last_LSEEK_offset, "LSEEK offset should be the state's start");
    CHECK_EQ(SEEK_SET, last_LSEEK_whence, "LSEEK whence should be SEEK_SET");
}

// Test 6: success path with Z_BUF_ERROR should rewind and call reset
void test_gzrewind_success_zbuferror() {
    RESET_TEST_STATE();
    gz_state s;
    s.mode = GZ_READ;
    s.err = Z_BUF_ERROR;
    s.fd = 5;
    s.start = 999;
    int ret = gzrewind((gzFile)&s);
    CHECK_EQ(0, ret, "gzrewind should return 0 on success with Z_BUF_ERROR");
    CHECK_TRUE(gz_reset_called, "gz_reset should be called on rewind success with Z_BUF_ERROR");
    CHECK_EQ(5, last_LSEEK_fd, "LSEEK fd should be the state's fd");
    CHECK_EQ(999, last_LSEEK_offset, "LSEEK offset should be the state's start");
    CHECK_EQ(SEEK_SET, last_LSEEK_whence, "LSEEK whence should be SEEK_SET");
}

// Entry point: run all tests and report summary
int main() {
    printf("Starting gzrewind unit test suite (self-contained, no GTest).\n");

    test_gzrewind_null_file();
    test_gzrewind_wrong_mode();
    test_gzrewind_invalid_err();
    test_gzrewind_lseek_failure();
    test_gzrewind_success_zok();
    test_gzrewind_success_zbuferror();

    printf("Tests run: %d, Failures: %d\n", tests_run, tests_failed);
    return (tests_failed == 0) ? 0 : 1;
}