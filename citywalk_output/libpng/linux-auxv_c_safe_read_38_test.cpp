/*
Unit test suite for the focal method:
- safe_read(png_structp png_ptr, int fd, void *buffer_in, size_t nbytes)

Context and approach:
- The tests are implemented in C++11, without GoogleTest. A tiny, non-terminating assertion
  framework (simple boolean checks with printed results) is used to ensure all tests run
  to completion.
- We rely on the actual Linux/unistd semantics (pipe and read). We provide a minimal
  png_warning mock to satisfy the external dependency used by safe_read.
- We purposefully avoid heavy integration with external libraries. In particular, we avoid
  mocking non-virtual methods and use only public functional behavior of safe_read.
- The tests exercise:
  - Normal single-shot read
  - Partial reads followed by EOF (multiple reads until EOF)
  - Immediate EOF when no data available (writer end closed)
  - Large nbytes path to exercise the nread = INT_MAX branch
- Important: We expose a tiny C linkage shim for png_warning and a compatible forward
  declaration of png_structp to align with how safe_read is declared in the C file
  linux-auxv.c (the focal implementation). We pass NULL for png_ptr where possible to
  avoid dependencies on the actual png_structp internals.

Candidate Keywords extracted from the focal method and dependencies:
- safe_read, read, nbytes, ntotal, iread, nread, EINTR, INT_MAX, png_warning, png_structp,
  buffer, buffer_in, EOF, OS /proc read bug.

Note: This test harness assumes linux-auxv.c is compiled and linked into the test binary.
We mock png_warning to capture warnings if needed and pass a NULL png_ptr to safe_read
to avoid dependency on the actual png_structp internals.
*/

#include <vector>
#include <sys/stat.h>
#include <string>
#include <elf.h>
#include <fcntl.h>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <cstring>
#include <asm/hwcap.h>


// Forward declaration to mimic the project-provided C API for safe_read.
// We create a minimal compatibility layer for png_structp usage and a png_warning mock.

extern "C" {
   // Forward declare the private png_struct type used by the focal function.
   struct png_struct_tag;
   typedef struct png_struct_tag* png_structp;

   // Prototype of the focal function (C linkage).
   size_t safe_read(png_structp png_ptr, int fd, void *buffer_in, size_t nbytes);

   // Prototype of the png_warning function used within safe_read.
   void png_warning(png_structp png_ptr, const char* msg);
}

// Global to capture emitted warnings for potential later assertions.
static std::vector<std::string> g_warnings;

// Mock of the libpng warning function used by safe_read.
// We implement C linkage to match the actual symbol expected by linux-auxv.c.
extern "C" void png_warning(png_structp /*png_ptr*/, const char* msg)
{
    if (msg) {
        g_warnings.emplace_back(msg);
    } else {
        g_warnings.emplace_back("");
    }
}

// Helpers for the test harness: simple non-terminating assertions (do not exit on failure)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    g_tests_run++; \
    if (!(cond)) { \
        ++g_tests_failed; \
        fprintf(stderr, "TEST FAIL: %s\n", msg); \
    } else { \
        fprintf(stdout, "TEST PASS: %s\n", msg); \
    } \
} while (0)

static void reset_warnings() {
    g_warnings.clear();
}

static const std::vector<std::string>& get_warnings() { return g_warnings; }

// Helper test: read exactly what we provide in the pipe (single chunk)
static bool test_safe_read_simple_single_chunk() {
    // Prepare data to be read
    const char src[] = "ABCDEFGH"; // 8 bytes
    int fds[2];
    if (pipe(fds) != 0) {
        perror("pipe");
        return false;
    }

    // Write data to the read-end via the pipe's write end, then close write end
    ssize_t w = write(fds[1], src, sizeof(src) - 1); // 8 bytes
    if (w != (ssize_t)(sizeof(src) - 1)) {
        perror("write");
        close(fds[0]);
        close(fds[1]);
        return false;
    }
    close(fds[1]); // close writer to simulate EOF after data is read

    // Buffer to receive data
    char buf[16];
    memset(buf, 0xAA, sizeof(buf)); // fill with recognizable pattern

    // Call safe_read with exactly 8 bytes
    size_t n = safe_read(nullptr, fds[0], buf, 8);

    // Cleanup
    close(fds[0]);

    // Assertions
    bool ok = (n == 8) && (memcmp(buf, src, 8) == 0);
    return ok;
}

// Helper test: read with partial reads (expected multiple iterations)
static bool test_safe_read_partial_then_eof() {
    // Data: 5 bytes
    const char src[] = "12345";
    int fds[2];
    if (pipe(fds) != 0) {
        perror("pipe");
        return false;
    }

    // Write 5 bytes, then close writer to simulate EOF after data
    ssize_t w = write(fds[1], src, sizeof(src) - 1); // 5 bytes
    if (w != (ssize_t)(sizeof(src) - 1)) {
        perror("write");
        close(fds[0]);
        close(fds[1]);
        return false;
    }
    close(fds[1]); // close writer

    // Buffer larger than data to ensure partial read path is exercised
    char buf[16];
    memset(buf, 0, sizeof(buf));

    size_t n = safe_read(nullptr, fds[0], buf, 10);

    close(fds[0]);

    // We expect exactly 5 bytes to be read
    bool ok = (n == 5) && (memcmp(buf, src, 5) == 0);
    return ok;
}

// Helper test: immediate EOF when no data is available (writer closed)
static bool test_safe_read_immediate_eof() {
    // Create a pipe and immediately close writer
    int fds[2];
    if (pipe(fds) != 0) {
        perror("pipe");
        return false;
    }
    // Close writer to simulate EOF with no data
    close(fds[1]);

    // Pre-fill buffer to detect no change
    char buf[8];
    const char sentinel[] = "ZZZZZZZZ";
    memcpy(buf, sentinel, sizeof(buf));

    size_t n = safe_read(nullptr, fds[0], buf, 8);

    close(fds[0]);

    // Expect 0 bytes read and no modification to the buffer
    bool ok = (n == 0) && (memcmp(buf, sentinel, sizeof(sentinel)) == 0);
    return ok;
}

// Helper test: large nbytes path exercises INT_MAX boundary.
// We read a small amount of data but request far more than INT_MAX at once.
static bool test_safe_read_large_nbytes() {
    // Data: 10 bytes
    const char src[] = "0123456789";

    int fds[2];
    if (pipe(fds) != 0) {
        perror("pipe");
        return false;
    }

    // Write data and then close writer to ensure EOF after data
    ssize_t w = write(fds[1], src, sizeof(src) - 1); // 10 bytes
    if (w != (ssize_t)(sizeof(src) - 1)) {
        perror("write");
        close(fds[0]);
        close(fds[1]);
        return false;
    }
    // Close writer to trigger EOF on next read
    close(fds[1]);

    // Buffer to receive data
    char buf[16];
    memset(buf, 0, sizeof(buf));

    // Request more than INT_MAX bytes; real read will return up to available data then EOF
    size_t large_nbytes = (size_t)INT_MAX + 100; // large > INT_MAX
    size_t n = safe_read(nullptr, fds[0], buf, large_nbytes);

    close(fds[0]);

    // Expect 10 bytes read (all data present before EOF)
    bool ok = (n == 10) && (memcmp(buf, src, 10) == 0);
    return ok;
}

// Main entry: run all tests and print a summary.
// Per instructions: no GTest; use a minimal harness with non-terminating assertions.
int main() {
    printf("Starting test suite for safe_read\n");

    reset_warnings();

    bool r1 = test_safe_read_simple_single_chunk();
    EXPECT_TRUE(r1, "safe_read reads exactly 8 bytes in a single read when 8 bytes are available");

    bool r2 = test_safe_read_partial_then_eof();
    EXPECT_TRUE(r2, "safe_read handles partial reads and stops at EOF, returning 5");

    bool r3 = test_safe_read_immediate_eof();
    EXPECT_TRUE(r3, "safe_read returns 0 when EOF is encountered immediately (no data)");

    bool r4 = test_safe_read_large_nbytes();
    EXPECT_TRUE(r4, "safe_read with nbytes > INT_MAX reads available data (10 bytes) and returns 10");

    // Optional: print warnings captured (if any)
    if (!get_warnings().empty()) {
        fprintf(stdout, "Warnings captured during tests:\n");
        for (const auto& w : get_warnings()) {
            fprintf(stdout, "  %s\n", w.c_str());
        }
    }

    // Summary
    fprintf(stdout, "\nTest summary: %d run, %d failed\n", g_tests_run, g_tests_failed);
    return g_tests_failed ? EXIT_FAILURE : EXIT_SUCCESS;
}