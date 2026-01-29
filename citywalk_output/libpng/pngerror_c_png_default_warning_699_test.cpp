// Test suite for png_default_warning defined in pngerror.c
// This test suite targets the console IO path (PNG_CONSOLE_IO_SUPPORTED) to verify
// the exact output formatting to stderr: "libpng warning: <message>\n"
// Notes:
// - The test uses POSIX facilities (pipe/dup) to capture stderr output.
// - Tests are written to be non-terminating (they return pass/fail booleans and
//   do not call abort/exit on failure).
// - The library types are pulled from libpng headers (pngpriv.h) to match
//   the real function signature. The test passes nullptr for png_ptr since
//   the function uses PNG_UNUSED on it.

#include <unistd.h>
#include <string>
#include <functional>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstring>


// Include libpng internal header to obtain type definitions such as png_const_structrp
// and png_const_charp. Adjust include path as needed in your build environment.

// Declare the function under test with C linkage.
// We rely on the libpng typedefs for the parameter types.
extern "C" void png_default_warning(png_const_structrp png_ptr, png_const_charp warning_message);

// Helper class to capture stderr output by temporarily redirecting it to a pipe.
class StderrCapture {
public:
    StderrCapture() : old_fd_(-1), read_fd_(-1), write_fd_(-1), active_(false) {}

    // Start redirecting stderr to a pipe
    void start() {
        int fds[2];
        if (pipe(fds) != 0) {
            // If piping fails, mark as inactive to avoid undefined behavior
            active_ = false;
            return;
        }
        read_fd_ = fds[0];
        write_fd_ = fds[1];
        // Save the current stderr
        old_fd_ = dup(2);
        // Redirect stderr to the write end of the pipe
        dup2(write_fd_, 2);
        // Close the write end descriptor we no longer need in the test
        close(write_fd_);
        active_ = true;
    }

    // Stop redirecting and return captured content
    std::string stop() {
        if (!active_) return "";
        // Ensure all data is flushed
        fflush(stderr);
        // Restore original stderr
        dup2(old_fd_, 2);
        close(old_fd_);
        active_ = false;

        // Read all data from the read end of the pipe
        std::string output;
        char buffer[4096];
        ssize_t bytes = 0;
        // The descriptor might be closed if start() failed; guard
        if (read_fd_ != -1) {
            while ((bytes = static_cast<ssize_t>(read(read_fd_, buffer, sizeof(buffer)))) > 0) {
                output.append(buffer, static_cast<size_t>(bytes));
            }
            close(read_fd_);
            read_fd_ = -1;
        }
        return output;
    }

private:
    int old_fd_;
    int read_fd_;
    int write_fd_;
    bool active_;
};

// Test 1: Validate that with console IO enabled, the warning is printed in the exact format.
bool test_prints_expected_message() {
    const std::string message = "hello world";
    const std::string expected_output = "libpng warning: " + message + "\n";

    StderrCapture cap;
    cap.start();
    // The first parameter is a png_ptr; since the code uses PNG_UNUSED(png_ptr),
    // we can pass nullptr safely.
    png_default_warning(nullptr, message.c_str());
    std::string output = cap.stop();

    // Compare exact output (non-terminating assertion pattern)
    return output == expected_output;
}

// Test 2: Validate that an empty warning message prints "libpng warning: " followed by newline.
bool test_empty_message() {
    const std::string message = "";
    const std::string expected_output = "libpng warning: " + message + "\n";

    StderrCapture cap;
    cap.start();
    png_default_warning(nullptr, message.c_str());
    std::string output = cap.stop();

    return output == expected_output;
}

// Test 3: Validate multiple successive warnings produce concatenated outputs.
bool test_multiple_warnings() {
    const std::string m1 = "first message";
    const std::string m2 = "second message";
    const std::string expected_output = "libpng warning: " + m1 + "\n" +
                                        "libpng warning: " + m2 + "\n";

    StderrCapture cap;
    cap.start();
    png_default_warning(nullptr, m1.c_str());
    png_default_warning(nullptr, m2.c_str());
    std::string output = cap.stop();

    return output == expected_output;
}

// Entry point
int main() {
    int total_tests = 3;
    int passed = 0;

#if defined(PNG_CONSOLE_IO_SUPPORTED)
    // If the library was compiled with PNG_CONSOLE_IO_SUPPORTED, the tests can
    // observe console output. Execute all tests.
    if (test_prints_expected_message()) {
        ++passed;
    } else {
        std::cerr << "Test 1 failed: expected console output not observed.\n";
    }

    if (test_empty_message()) {
        ++passed;
    } else {
        std::cerr << "Test 2 failed: empty message did not produce expected output.\n";
    }

    if (test_multiple_warnings()) {
        ++passed;
    } else {
        std::cerr << "Test 3 failed: multiple warnings did not produce expected concatenated output.\n";
    }

#else
    // If PNG_CONSOLE_IO_SUPPORTED is not defined, we cannot reliably test the
    // console output branch. Inform the user but do not treat as failure.
    std::cout << "PNG_CONSOLE_IO_SUPPORTED is not defined for this build. Skipping console output tests.\n";
    // We still report total as 0 passed to avoid false failures in builds without this macro.
    total_tests = 0;
    passed = 0;
#endif

    // Summary
    if (total_tests > 0) {
        std::cout << "png_default_warning tests: " << passed << "/" << total_tests << " passed.\n";
    } else {
        std::cout << "No console-output tests executed.\n";
    }

    // Return success status if all tests that were run passed
    return (passed == total_tests) ? 0 : 1;
}