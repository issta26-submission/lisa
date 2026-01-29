// Unit test suite for the focal method: emit_error(struct file *file, int code, const char *what)
// This test harness is crafted for a C/C++11 project that includes the actual pngfix.c (emitting emit_error).
// The tests use a minimal opaque "struct file" layout focusing on the fields accessed by emit_error.
// The tests do not rely on Google Test or any external framework; instead, a light inline harness is used.
// Note: This test assumes a POSIX environment for stdout/stderr capture via pipes.

#include <setjmp.h>
#include <string.h>
#include <string>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <iostream>
#include <stdlib.h>
#include <functional>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Forward declaration of the focal function (C linkage)
extern "C" void emit_error(struct file *file, int code, const char *what);

// Minimal local replica of the struct file used by emit_error.
// We define only the fields accessed by emit_error to avoid depending on libpng's internal layout.
// If the real pngfix.c uses a different struct, ensure these fields exist and are laid out compatibly.
struct file {
    const char *file_name;  // name of the file being reported
    int read_errno;           // errno for read-related errors
    int write_errno;          // errno for write-related errors
};

// ---------------------------------------------------------------------------
// Test harness utilities: capture stderr output when calling a function.
// ---------------------------------------------------------------------------

// Capture the stderr output produced by calling the provided function.
std::string capture_stderr_output(const std::function<void()> &func) {
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        // Fallback: return empty if pipe creation fails
        return "";
    }

    // Save current stderr
    int old_stderr = dup(STDERR_FILENO);
    // Redirect stderr to the write end of the pipe
    dup2(pipefd[1], STDERR_FILENO);
    // Close the original write end as it's now duplicated
    close(pipefd[1]);

    // Execute the function that will write to stderr
    func();

    // Ensure all streams are flushed
    fflush(stderr);

    // Restore original stderr
    dup2(old_stderr, STDERR_FILENO);
    close(old_stderr);

    // Read the captured output
    std::string output;
    char buffer[4096];
    ssize_t n;
    // After redirecting, read from the read end of the pipe
    // Close the write end first if not already closed
    close(pipefd[1]);
    while ((n = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
        output.append(buffer, static_cast<size_t>(n));
    }
    close(pipefd[0]);
    return output;
}

// Helper to run a single test with descriptive name and result
#define RUN_TEST(name) TestCase(name, [&](){ name##_impl(); })

struct TestCase {
    std::string name;
    std::function<void()> run;
    TestCase(const std::string &n, std::function<void()> f) : name(n), run(f) {}
};

// Simple assertion macro: non-terminating, prints pass/fail with description
#define ASSERT(cond, desc) \
    do { \
        if (cond) { \
            std::cout << "[OK] " << desc << std::endl; \
        } else { \
            std::cout << "[FAIL] " << desc << std::endl; \
        } \
    } while (0)


// ---------------------------------------------------------------------------
// Candidate macro values (match the focal code's switch cases).
// These values are chosen to align with common libpng/libz definitions.
// If the real project uses different numeric values, adjust accordingly.
// ---------------------------------------------------------------------------

#define LIBPNG_WARNING_CODE 0
#define LIBPNG_ERROR_CODE   1
#define ZLIB_ERROR_CODE     2
#define INVALID_ERROR_CODE  3
#define READ_ERROR_CODE     4
#define WRITE_ERROR_CODE    5
#define UNEXPECTED_ERROR_CODE 6

// ---------------------------------------------------------------------------
// Test Implementations
// Each test constructs a minimal 'struct file' and calls emit_error, capturing
// stderr to validate the emitted message.
// ---------------------------------------------------------------------------

// Test 1: LIBPNG_WARNING_CODE with no errno set -> no strerror appended
void emit_error_warning_no_errno_impl() {
    file f;
    f.file_name = "warning.png";
    f.read_errno = 0;
    f.write_errno = 0;

    auto out = capture_stderr_output([&](){
        emit_error(&f, LIBPNG_WARNING_CODE, "test-warning");
    });

    // Expect exact prefix with no errno appended
    ASSERT(out.find("warning.png: libpng warning: test-warning") != std::string::npos,
           "LIBPNG_WARNING_code should print 'libpng warning' with message");
    // No errno suffix expected when errno is 0
    ASSERT(out.find("[") == std::string::npos,
           "No errno detail should be printed when errno == 0");
}

// Test 2: LIBPNG_ERROR_CODE with no errno set
void emit_error_error_no_errno_impl() {
    file f;
    f.file_name = "error.png";
    f.read_errno = 0;
    f.write_errno = 0;

    auto out = capture_stderr_output([&](){
        emit_error(&f, LIBPNG_ERROR_CODE, "boom");
    });

    ASSERT(out.find("error.png: libpng error: boom") != std::string::npos,
           "LIBPNG_ERROR_code should print 'libpng error' with message");
    ASSERT(out.find("[") == std::string::npos,
           "No errno detail should be printed when errno == 0");
}

// Test 3: READ_ERROR_CODE with non-zero read_errno
void emit_error_read_error_with_errno_impl() {
    file f;
    f.file_name = "read.png";
    f.read_errno = EIO;   // set a non-zero errno
    f.write_errno = 0;

    auto out = capture_stderr_output([&](){
        emit_error(&f, READ_ERROR_CODE, "read failed");
    });

    // Expect "read failure:" plus the message, with [errno] appended
    ASSERT(out.find("read.png: read failure: read failed") != std::string::npos,
           "READ_ERROR_CODE should print 'read failure' with message");
    ASSERT(out.find("[") != std::string::npos && out.find("]") != std::string::npos,
           "READ_ERROR_CODE should append strerror(err) in brackets when errno != 0");
}

// Test 4: UNEXPECTED_ERROR_CODE with non-zero read_errno
void emit_error_unexpected_with_read_errno_impl() {
    file f;
    f.file_name = "unexpected.png";
    f.read_errno = EINVAL;
    f.write_errno = 0;

    auto out = capture_stderr_output([&](){
        emit_error(&f, UNEXPECTED_ERROR_CODE, "unexpected");
    });

    // Expect "unexpected error:" plus the message and strerror(err)
    // The exact strerror string is platform-dependent; we verify presence of generic parts.
    ASSERT(out.find("unexpected.png: unexpected error: unexpected") != std::string::npos,
           "UNEXPECTED_ERROR_CODE should map to 'unexpected error:' and include the message");
    // Should include strerror output in brackets since errno != 0
    ASSERT(out.find("[") != std::string::npos && out.find("]") != std::string::npos,
           "UNEXPECTED_ERROR_CODE with non-zero errno should include strerror(err)");
}

// Test 5: UNEXPECTED_ERROR_CODE with zero read_errno but non-zero write_errno
void emit_error_unexpected_zero_read_errno_with_write_impl() {
    file f;
    f.file_name = "unexpected2.png";
    f.read_errno = 0;
    f.write_errno = EACCES;

    auto out = capture_stderr_output([&](){
        emit_error(&f, UNEXPECTED_ERROR_CODE, "unexpected");
    });

    // Since read_errno == 0, the code should use write_errno for strerror
    ASSERT(out.find("unexpected2.png: unexpected error: unexpected") != std::string::npos,
           "UNEXPECTED_ERROR_CODE should fallback to write errno when read_errno == 0");
    ASSERT(out.find("[") != std::string::npos && out.find("]") != std::string::npos,
           "UNEXPECTED_ERROR_CODE with write errno should include strerror(err)");
}

// Test 6: WRITE_ERROR_CODE with non-zero write_errno
void emit_error_write_error_with_errno_impl() {
    file f;
    f.file_name = "write.png";
    f.read_errno = 0;
    f.write_errno = EPIPE;

    auto out = capture_stderr_output([&](){
        emit_error(&f, WRITE_ERROR_CODE, "write failed");
    });

    // Should print "write error" and include strerror(err)
    ASSERT(out.find("write.png: write error write failed") != std::string::npos,
           "WRITE_ERROR_CODE should print 'write error' with message");
    ASSERT(out.find("[") != std::string::npos && out.find("]") != std::string::npos,
           "WRITE_ERROR_CODE should include strerror(err) in brackets when errno != 0");
}

// Test 7: INVALID_ERROR_CODE maps to internal invalid message
void emit_error_invalid_error_code_impl() {
    file f;
    f.file_name = "misc.png";
    f.read_errno = 0;
    f.write_errno = 0;

    auto out = capture_stderr_output([&](){
        emit_error(&f, INVALID_ERROR_CODE, "badcode");
    });

    // Should map to "invalid" (with no trailing colon per the code)
    // The exact formatting becomes "<file>: invalid badcode\n" (verify containing prefix and message)
    ASSERT(out.find("misc.png: invalid badcode") != std::string::npos,
           "INVALID_ERROR_CODE should map to 'invalid' prefix without internal formatting");
}

// ---------------------------------------------------------------------------
// Main entry: run all tests and report summary
// ---------------------------------------------------------------------------

int main() {
    std::cout << "Starting emit_error unit test suite (no GTest).\n";

    // Run tests
    emit_error_warning_no_errno_impl();            // LIBPNG_WARNING_CODE, errno == 0
    emit_error_error_no_errno_impl();              // LIBPNG_ERROR_CODE, errno == 0
    emit_error_read_error_with_errno_impl();       // READ_ERROR_CODE, errno != 0
    emit_error_unexpected_with_read_errno_impl();  // UNEXPECTED_ERROR_CODE, errno != 0 (read)
    emit_error_unexpected_zero_read_errno_with_write_impl(); // UNEXPECTED_ERROR_CODE, errno 0 then use write_errno
    emit_error_write_error_with_errno_impl();      // WRITE_ERROR_CODE, errno != 0
    emit_error_invalid_error_code_impl();          // INVALID_ERROR_CODE case

    std::cout << "emit_error unit tests completed." << std::endl;
    return 0;
}