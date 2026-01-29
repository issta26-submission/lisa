#include <sys/stat.h>
#include <fcntl.h>
#include <threaded_internal.h>
#include <iostream>
#include <string>
#include <stdexcept>
#include <time.h>
#include <sys/types.h>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>


// Step 1: Declare the focal C function.
// The function resides in the C source (threaded_testbed.c) and is exposed with C linkage.
// We need to call it from C++14/11 test code.
extern "C" void trace(const char* frm, ...);

// Step 3 / Domain knowledge notes:
// - We implement a small capture utility that redirects stderr to a pipe so we can validate
//   the exact formatted output produced by trace.
// - We provide a minimal test harness with non-terminating assertions (they print results but do not exit).
// - Tests are written in plain C++ (no GoogleTest or GMock) and are invoked from main().

class StderrCapture {
public:
    StderrCapture() : old_stderr_fd_(-1), read_fd_(-1), write_fd_(-1), writer_closed_(false) {
        int fds[2];
        if (pipe(fds) != 0) {
            throw std::runtime_error("pipe creation failed");
        }
        read_fd_ = fds[0];
        write_fd_ = fds[1];

        // Save current stderr and redirect to the pipe's write end
        old_stderr_fd_ = dup(STDERR_FILENO);
        if (old_stderr_fd_ == -1) {
            throw std::runtime_error("dup failed");
        }
        if (dup2(write_fd_, STDERR_FILENO) == -1) {
            throw std::runtime_error("dup2 failed");
        }
    }

    ~StderrCapture() {
        // Ensure we restore stdout/stderr and close FDs
        fflush(stderr);
        if (old_stderr_fd_ != -1) {
            dup2(old_stderr_fd_, STDERR_FILENO);
            close(old_stderr_fd_);
        }
        // If the test didn't explicitly close the writer, close it here to generate EOF
        if (!writer_closed_) {
            close(write_fd_);
            writer_closed_ = true;
        }
        if (read_fd_ != -1) {
            close(read_fd_);
        }
    }

    // Close the writer end to signal EOF to the reader
    void closeWriter() {
        if (!writer_closed_) {
            close(write_fd_);
            writer_closed_ = true;
        }
    }

    // Read all data from the read end of the pipe
    std::string readOutput() {
        // Read until EOF
        std::string result;
        char buffer[4096];
        ssize_t n;
        // The writer end must be closed before we can detect EOF
        while ((n = ::read(read_fd_, buffer, sizeof(buffer))) > 0) {
            result.append(buffer, buffer + n);
        }
        return result;
    }

private:
    int old_stderr_fd_;
    int read_fd_;
    int write_fd_;
    bool writer_closed_;
};

// Simple non-terminating test assertion macro.
// It prints PASS/FAIL messages but does not exit the test on failure.
#define ASSERT(cond, msg) do { \
    ++g_tests_run; \
    if (cond) { \
        ++g_tests_passed; \
        std::cerr << "PASS: " << msg << "\n"; \
    } else { \
        std::cerr << "FAIL: " << msg << "\n"; \
    } \
} while(0)

static int g_tests_run = 0;
static int g_tests_passed = 0;

// Test 1: Basic formatting with a simple string should be emitted exactly as provided.
void test_trace_basic_format() {
    // Explanation:
    // Ensure that trace prints the literal string when no format specifiers are used.
    StderrCapture cap;
    trace("Hello World");
    cap.closeWriter();
    std::string out = cap.readOutput();

    ASSERT(out == "Hello World", "trace_basic_format outputs 'Hello World'");
}

// Test 2: Numeric formatting with %d and %x should produce expected decimal and hex representations.
void test_trace_numeric_format() {
    // Explanation:
    // Validate that numeric specifiers are processed correctly.
    StderrCapture cap;
    trace("Value=%d; Hex=%x", 255, 255);
    cap.closeWriter();
    std::string out = cap.readOutput();

    // Expect lowercase hex 'ff' for %x
    ASSERT(out == "Value=255; Hex=ff", "trace_numeric_format formats %d and %x correctly");
}

// Test 3: Formatting with newline characters should preserve line breaks.
void test_trace_with_newline() {
    // Explanation:
    // Ensure newline characters in the format string are preserved in output.
    StderrCapture cap;
    trace("Line1\nLine2");
    cap.closeWriter();
    std::string out = cap.readOutput();

    ASSERT(out == "Line1\nLine2", "trace_with_newline preserves newlines");
}

// Test 4: Plain string without any format specifiers should be emitted as-is.
void test_trace_plain_string() {
    // Explanation:
    // A string that contains no format specifiers should be printed verbatim.
    StderrCapture cap;
    trace("Plain");
    cap.closeWriter();
    std::string out = cap.readOutput();

    ASSERT(out == "Plain", "trace_plain_string outputs plain string without formatting");
}

// Test 5: Consecutive calls should concatenate outputs in stream-like fashion.
void test_trace_consecutive_calls() {
    // Explanation:
    // Verify that multiple calls to trace produce concatenated output without extra separators.
    StderrCapture cap;
    trace("First %s", "call");
    trace("Second %d", 2);
    cap.closeWriter();
    std::string out = cap.readOutput();

    ASSERT(out == "First callSecond 2", "trace_consecutive_calls concatenates multiple traces");
}

// Run all tests and report summary
void run_all_tests() {
    test_trace_basic_format();
    test_trace_numeric_format();
    test_trace_with_newline();
    test_trace_plain_string();
    test_trace_consecutive_calls();
}

int main() {
    run_all_tests();

    std::cerr << "==========================\n";
    std::cerr << "Test results: " << g_tests_passed << "/" << g_tests_run << " passed\n";
    return (g_tests_run == g_tests_passed) ? 0 : 1;
}