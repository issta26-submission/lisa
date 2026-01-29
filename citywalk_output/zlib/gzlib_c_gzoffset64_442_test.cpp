// Automated unit tests for gzoffset64 in gzlib.c
// This test suite is written for a C++11 environment without Google Test.
// It uses a small in-process test harness with non-terminating assertions.
// The tests rely on gzoffset64's internal behavior by crafting a fake internal
// gz_state structure and casting it to gzFile (opaque pointer) as expected by the API.
// Note: This approach hinges on the assumption that the internal layout of gz_statep
// in the tested library places 'mode', 'strm.avail_in', and 'fd' at the expected offsets.
// If the library's private layout differs, these tests may need adjustment.

#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <iostream>
#include <gzguts.h>
#include <zlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <cstdlib>


extern "C" {
}

// --------------------------------------------------------------------------------
// Fake internal state layout (to be cast to gz_statep for gzoffset64).
// We intentionally mimic only the fields used by gzoffset64:
//   - mode
//   - strm.avail_in
//   - fd
// This is a test stub and must be kept consistent with the library's expectations
// for memory layout (see analysis notes above).
// --------------------------------------------------------------------------------

struct FakeStrm {
    unsigned int avail_in;
};

struct FakeState {
    int mode;
    FakeStrm strm;
    int fd;
};

// Global fake state instance used across tests. The tests will modify fields as needed.
static FakeState g_fake_state;

// Helper to obtain a pointer of type gz_statep from the fake state.
// We cast the address of the fake state to gz_statep, which gzoffset64 will then
// interpret as its internal state structure.
static inline gz_statep get_fake_state_ptr() {
    return (gz_statep)&g_fake_state;
}

// --------------------------------------------------------------------------------
// Minimal test framework (non-terminating assertions)
// --------------------------------------------------------------------------------

static int tests_run = 0;
static int tests_failed = 0;

#define ASSERT_EQ(expected, actual, test_desc) do { \
    ++tests_run; \
    if ((expected) != (actual)) { \
        ++tests_failed; \
        std::cerr << "TEST FAIL: " << (test_desc) << "\n" \
                  << "  Expected: " << (expected) << "\n" \
                  << "  Actual  : " << (actual) << "\n"; \
    } \
} while (0)

#define ASSERT_TRUE(cond, test_desc) do { \
    ++tests_run; \
    if (!(cond)) { \
        ++tests_failed; \
        std::cerr << "TEST FAIL: " << (test_desc) << "\n" \
                  << "  Condition failed: " << #cond << "\n"; \
    } \
} while (0)

// --------------------------------------------------------------------------------
// Test cases for gzoffset64
// --------------------------------------------------------------------------------

/*
 Test 1: NULL file input should return -1
 - Reason: Function explicitly checks for NULL file and returns -1.
*/
static bool test_gzoffset64_null_input() {
    // Explanatory comment:
    // Verify that passing a NULL gzFile yields an error code (-1)
    z_off64_t res = gzoffset64(nullptr);
    ASSERT_EQ((z_off64_t)-1, res, "gzoffset64 should return -1 for NULL file");
    return (tests_run > 0 && tests_failed == 0); // success if no failures so far
}

/*
 Test 2: Invalid mode in internal state should return -1
 - Reason: Function checks state->mode against GZ_READ and GZ_WRITE.
*/
static bool test_gzoffset64_invalid_mode() {
    // Prepare an invalid mode (not GZ_READ nor GZ_WRITE)
    g_fake_state.mode = -1;            // invalid mode
    g_fake_state.strm.avail_in = 0;
    g_fake_state.fd = 0;
    z_off64_t res = gzoffset64((gzFile)&g_fake_state);
    ASSERT_EQ((z_off64_t)-1, res, "gzoffset64 should return -1 for invalid mode");
    return (tests_run > 0 && tests_failed == 0);
}

/*
 Test 3: Valid GZ_READ mode with zero current offset and non-zero avail_in
 - Reason: Should return current offset minus avail_in (buffered input not yet consumed).
*/
static bool test_gzoffset64_read_subtract_buffered() {
    // Create a temporary file and obtain a stable fd
    char tmpl[] = "/tmp/gzoffset64_testXXXXXX";
    int fd = mkstemp(tmpl);
    if (fd == -1) {
        std::cerr << "Failed to create temp file: " << strerror(errno) << "\n";
        return false;
    }

    // Ensure the file offset is at 0
    if (lseek(fd, 0, SEEK_SET) == (off_t)-1) {
        std::cerr << "lseek failed: " << strerror(errno) << "\n";
        close(fd);
        unlink(tmpl);
        return false;
    }

    g_fake_state.fd = fd;
    g_fake_state.mode = GZ_READ;
    g_fake_state.strm.avail_in = 5; // pretend there is 5 bytes buffered for reading

    z_off64_t res = gzoffset64((gzFile)&g_fake_state);

    // Expected: current offset (0) minus avail_in (5) = -5
    ASSERT_EQ((z_off64_t)-5, res, "gzoffset64 in GZ_READ should subtract avail_in from current offset");

    // Cleanup
    close(fd);
    unlink(tmpl);
    return (tests_run > 0 && tests_failed == 0);
}

/*
 Test 4: Valid GZ_WRITE mode should not subtract avail_in
 - Reason: Only GZ_READ mode triggers subtracting avail_in.
*/
static bool test_gzoffset64_write_no_subtract() {
    // Reuse a temporary file to obtain a stable fd and set offset to a known value
    char tmpl[] = "/tmp/gzoffset64_testXXXXXXb";
    int fd = mkstemp(tmpl);
    if (fd == -1) {
        std::cerr << "Failed to create temp file (test 4): " << strerror(errno) << "\n";
        return false;
    }

    // Move offset to 60
    if (lseek(fd, 60, SEEK_SET) == (off_t)-1) {
        std::cerr << "lseek to 60 failed: " << strerror(errno) << "\n";
        close(fd);
        unlink(tmpl);
        return false;
    }

    g_fake_state.fd = fd;
    g_fake_state.mode = GZ_WRITE;
    g_fake_state.strm.avail_in = 999; // not used in GZ_WRITE

    z_off64_t res = gzoffset64((gzFile)&g_fake_state);

    // Expected: current offset is 60, no subtraction for GZ_WRITE
    ASSERT_EQ((z_off64_t)60, res, "gzoffset64 in GZ_WRITE should not subtract avail_in");

    // Cleanup
    close(fd);
    unlink(tmpl);
    return (tests_run > 0 && tests_failed == 0);
}

/*
 Test 5: Invalid file descriptor (-1) should result in -1
 - Reason: LSEEK on -1 should fail, returning -1, which propagates.
*/
static bool test_gzoffset64_invalid_fd() {
    g_fake_state.fd = -1;
    g_fake_state.mode = GZ_READ;
    g_fake_state.strm.avail_in = 0;
    z_off64_t res = gzoffset64((gzFile)&g_fake_state);
    ASSERT_EQ((z_off64_t)-1, res, "gzoffset64 should return -1 for invalid fd");
    return (tests_run > 0 && tests_failed == 0);
}

// --------------------------------------------------------------------------------
// Main harness
// --------------------------------------------------------------------------------

int main() {
    std::cout << "Starting gzoffset64 unit tests (C++11, no GTest)\n";

    // Run tests. Each test prints its own explanatory comments within the test body.

    bool ok;

    // Test 1: NULL input
    ok = test_gzoffset64_null_input();

    // Test 2: Invalid mode
    ok = test_gzoffset64_invalid_mode();

    // Test 3: GZ_READ with buffer subtraction
    ok = test_gzoffset64_read_subtract_buffered();

    // Test 4: GZ_WRITE should not subtract avail_in
    ok = test_gzoffset64_write_no_subtract();

    // Test 5: Invalid fd results in -1
    ok = test_gzoffset64_invalid_fd();

    // Summary
    std::cout << "Tests run: " << tests_run << ", Failures: " << tests_failed << "\n";

    // Return non-zero if any test failed
    return (tests_failed == 0) ? 0 : 1;
}