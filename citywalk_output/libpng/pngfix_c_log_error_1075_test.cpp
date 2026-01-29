// Test suite for the focal method: log_error(struct file *file, int code, const char *what)
// This test is written in C++11 (no Google Test) and targets the C function
// log_error defined in pngfix.c. We create a minimal fake layout for
// struct file / struct global to drive the behavior declared in log_error.
// The actual implementation uses file->global->errors to decide whether to call emit_error.
// We observe emitted messages by redirecting stderr to a file and inspecting its contents.

#include <unistd.h>
#include <setjmp.h>
#include <sstream>
#include <string.h>
#include <string>
#include <fstream>
#include <limits.h>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <iostream>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <ctype.h>


// Forward declare the C function to be tested with C linkage.
extern "C" {
struct file; // incomplete type; actual layout is inside the C source.
void log_error(struct file *file, int code, const char *what);
}

// Minimal fake layout mirroring the intended access pattern in log_error.
// These are intentionally separate from the real C types to avoid coupling with internal headers.
struct fake_global {
    int errors;
};

struct fake_file {
    fake_global *global;
};

// Utility: redirect stderr to a file to capture emitted error messages.
// On POSIX systems (the test environment is typically Linux), we use dup2 to swap
// the file descriptor of stderr to a writable file descriptor pointing to a file.
static int redirect_stderr_to_file(const char* path) {
    int saved_fd = dup(fileno(stderr));      // Save current stderr
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open redirect_stderr_to_file");
        return -1;
    }
    // Replace stderr with our file descriptor
    if (dup2(fd, fileno(stderr)) != fileno(stderr)) {
        perror("dup2 redirect_stderr_to_file");
        close(fd);
        return -1;
    }
    close(fd);
    return saved_fd; // caller must restore using this fd
}

// Utility: restore stderr from saved_fd
static void restore_stderr(int saved_fd) {
    fflush(stderr);
    dup2(saved_fd, fileno(stderr));
    close(saved_fd);
}

// Utility: read entire contents of a file into a string
static std::string read_file_to_string(const char* path) {
    std::ifstream in(path, std::ios::in);
    if (!in) return "";
    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;
#define EXPECT(cond, msg) do { \
    if (cond) { ++g_tests_passed; } \
    else { ++g_tests_failed; std::cerr << "[TEST FAIL] " << msg << "\n"; } \
    ++g_tests_run; \
} while(0)

// Helper to run both tests
static void run_all_tests();

// Test 1: When global.errors is non-zero, log_error should emit an error message containing the provided 'what' text.
// We capture stderr output and verify that the message contains the expected substring.
static void test_log_error_emits_when_errors_true() {
    // Arrange
    static fake_global g;
    g.errors = 1; // true

    static fake_file f;
    f.global = &g;

    const char* what = "some_error_message_for_test";

    // Redirect stderr to a file to capture emitted output
    const char* tmp_path = "log_err_true_output.txt";
    int saved_fd = redirect_stderr_to_file(tmp_path);
    if (saved_fd < 0) {
        std::cerr << "Failed to redirect stderr for test_log_error_emits_when_errors_true\n";
        ++g_tests_run; ++g_tests_failed;
        return;
    }

    // Act
    log_error(reinterpret_cast<struct file*>(&f), 123, what);

    // Restore stderr
    restore_stderr(saved_fd);

    // Assert
    std::string captured = read_file_to_string(tmp_path);
    // Clean up
    remove(tmp_path);

    bool contains_text = (captured.find(what) != std::string::npos);
    EXPECT(contains_text, "log_error should emit the provided 'what' text when errors is true");
}

// Test 2: When global.errors is zero, log_error should not emit anything (emit_error should not be called).
static void test_log_error_no_emit_when_errors_false() {
    // Arrange
    static fake_global g;
    g.errors = 0; // false

    static fake_file f;
    f.global = &g;

    const char* what = "should_not_appear";

    // Redirect stderr to a file to capture potential output
    const char* tmp_path = "log_err_false_output.txt";
    int saved_fd = redirect_stderr_to_file(tmp_path);
    if (saved_fd < 0) {
        std::cerr << "Failed to redirect stderr for test_log_error_no_emit_when_errors_false\n";
        ++g_tests_run; ++g_tests_failed;
        return;
    }

    // Act
    log_error(reinterpret_cast<struct file*>(&f), 404, what);

    // Restore stderr
    restore_stderr(saved_fd);

    // Assert
    std::string captured = read_file_to_string(tmp_path);
    // Clean up
    remove(tmp_path);

    bool emitted_something = !captured.empty();
    EXPECT(!emitted_something, "log_error should not emit anything when errors == 0");
}

// Run tests
static void run_all_tests() {
    test_log_error_emits_when_errors_true();
    test_log_error_no_emit_when_errors_false();

    std::cout << "Tests run: " << g_tests_run << "\n";
    std::cout << "Passed: " << g_tests_passed << "\n";
    std::cout << "Failed: " << g_tests_failed << "\n";
}

// Main entry for the test executable
int main() {
    run_all_tests();
    // Return non-zero if any test failed
    return (g_tests_failed == 0) ? 0 : 1;
}