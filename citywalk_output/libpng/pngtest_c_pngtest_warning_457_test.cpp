// Test suite for the focal method: pngtest_warning in pngtest.c
// This test suite follows the provided instructions to build a comprehensive
// set of unit tests without using GTest. It targets the behavior of
// pngtest_warning, which prints a libpng warning message and uses the error_ptr
// stored in the png_ptr to optionally override the "name" shown in the message.
//
// Assumptions and approach:
// - We rely on libpng being available (header <png.h> and library linkage).
// - The pngtest_warning function accesses the error payload via png_get_error_ptr
//   and expects it to point to a structure with a member named 'file_name'.
// - We simulate three scenarios by supplying different error_ptr contents:
//   1) error_ptr not set (NULL) => name defaults to "UNKNOWN (ERROR!)".
//   2) error_ptr->file_name set to a specific string => that string is used as the
//      name in the warning output.
//   3) error_ptr present but file_name == NULL => fall back to "UNKNOWN (ERROR!)".
// - Output is captured by redirecting stdout (the macro STDERR is defined to stdout in
//   the original file, so the test observes stdout).
//
// Notes:
// - This test uses libpng's API: png_create_read_struct, png_set_error_ptr, and
//   png_get_error_ptr. It also uses a small, local struct pngtest_error_parameters
//   to populate the file_name used in the warning output. The layout is chosen so
//   that test->file_name maps to the actual code path in pngtest_warning.
// - Tests are implemented in C++11, using standard library I/O for assertions.
// - Non-terminating assertions are used: each test reports PASS/FAIL but does not exit.

#include <unistd.h>
#include <vector>
#include <string.h>
#include <time.h>
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <png.h>



// Declaration of the focal function (from pngtest.c).
// We use extern "C" to prevent name mangling when calling from C++.
extern "C" void pngtest_warning(png_structp png_ptr, png_const_charp message);

// Helper: Define a local structure compatible with the expected access in pngtest_warning
// The function casts png_get_error_ptr(png_ptr) to this type and then accesses file_name.
typedef struct pngtest_error_parameters {
    const char* file_name;
} pngtest_error_parameters;

// Utility: capture stdout while executing a test function
// This redirects stdout to a pipe, runs the function, then reads the captured data.
class StdoutCapture {
public:
    StdoutCapture() : pipe_fd{-1, -1}, stdout_dup(-1) {
        // Create a pipe for capturing stdout
        if (pipe(pipe_fd) != 0) {
            throw std::runtime_error("Pipe creation failed");
        }
            // Duplicate the current stdout
        stdout_dup = dup(fileno(stdout));
        if (stdout_dup == -1) {
            throw std::runtime_error("dup for stdout failed");
        }
        // Redirect stdout to the write end of the pipe
        if (dup2(pipe_fd[1], fileno(stdout)) == -1) {
            throw std::runtime_error("dup2 redirection failed");
        }
        // Close the write end in this process (it's dup'ed now)
        close(pipe_fd[1]);
    }

    ~StdoutCapture() {
        // Flush and restore stdout
        fflush(stdout);
        if (stdout_dup != -1) {
            dup2(stdout_dup, fileno(stdout));
            close(stdout_dup);
        }
        // Close read end
        if (pipe_fd[0] != -1) {
            close(pipe_fd[0]);
        }
    }

    std::string getOutput() {
        // Read captured data from the read end of the pipe
        std::string output;
        char buffer[4096];
        ssize_t n;
        // Ensure we flush before reading
        fflush(stdout);
        // Move to the read end (it's the pipe's read descriptor)
        // We already replaced stdout with the pipe's write end; so we read from pipe_fd[0]
        int fd = pipe_fd[0];
        // Read until EOF
        while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
            output.append(buffer, buffer + n);
        }
        return output;
    }

private:
    int pipe_fd[2];
    int stdout_dup;
};

// Helper: run a single test scenario and return the captured stdout
// The test function will set up the necessary libpng structures and call
// pngtest_warning with a message.
static std::string run_test_void(void (*test_func)()) {
    StdoutCapture capt;
    test_func();
    // Destructor of StdoutCapture will restore stdout
    // Retrieve the captured data
    return capt.getOutput();
}

// Test Scenario 1: No error_ptr set (test == NULL) -> name should be "UNKNOWN (ERROR!)"
static void test_case_no_error_ptr() {
    // Create a libpng read struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        std::cerr << "Failed to create png_structp in test_case_no_error_ptr\n";
        return;
    }

    // Do NOT set any error_ptr
    // Call the focal function
    pngtest_warning(png_ptr, "test message 1");

    // Note: We intentionally do not destroy the struct to keep the example short
    // (Memory leak benign for test harness scope)
    (void)png_ptr;
}

// Test Scenario 2: error_ptr->file_name non-NULL -> name should be the file_name
static void test_case_error_ptr_with_filename() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        std::cerr << "Failed to create png_structp in test_case_error_ptr_with_filename\n";
        return;
    }

    // Populate an error parameter structure with a non-NULL file_name
    static pngtest_error_parameters params;
    params.file_name = "myfile.png";

    // Set the error_ptr for the png_ptr
    png_set_error_ptr(png_ptr, (void*)&params);

    // Call the focal function
    pngtest_warning(png_ptr, "hello world");

    // Cleanup (best effort)
    // Note: libpng's destroy is not strictly necessary for this focused test
    // (omitted here for brevity in the harness)
    (void)png_ptr;
}

// Test Scenario 3: error_ptr present but file_name NULL -> name should be "UNKNOWN (ERROR!)"
static void test_case_error_ptr_filename_null() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        std::cerr << "Failed to create png_structp in test_case_error_ptr_filename_null\n";
        return;
    }

    static pngtest_error_parameters params;
    params.file_name = NULL; // explicit NULL

    png_set_error_ptr(png_ptr, (void*)&params);
    pngtest_warning(png_ptr, "another message");

    (void)png_ptr;
}

// Helper: Run all tests and accumulate pass/fail
static void run_all_tests() {
    std::vector<std::string> test_names = {
        "test_case_no_error_ptr",
        "test_case_error_ptr_with_filename",
        "test_case_error_ptr_filename_null"
    };
    int total = test_names.size();
    int passed = 0;

    // Test 1
    {
        std::string output = run_test_void([](){
            test_case_no_error_ptr();
        });
        // Verify that output contains the expected default name and message
        bool ok = (output.find("\nUNKNOWN (ERROR!)") != std::string::npos) &&
                  (output.find("libpng warning: test message 1") != std::string::npos);
        if (ok) {
            std::cout << "[PASS] test_case_no_error_ptr\n";
            ++passed;
        } else {
            std::cout << "[FAIL] test_case_no_error_ptr\n";
            std::cout << "Output: " << output << "\n";
        }
    }

    // Test 2
    {
        std::string output = run_test_void([](){
            test_case_error_ptr_with_filename();
        });
        bool ok = (output.find("\nmyfile.png: libpng warning: hello world") != std::string::npos);
        if (ok) {
            std::cout << "[PASS] test_case_error_ptr_with_filename\n";
            ++passed;
        } else {
            std::cout << "[FAIL] test_case_error_ptr_with_filename\n";
            std::cout << "Output: " << output << "\n";
        }
    }

    // Test 3
    {
        std::string output = run_test_void([](){
            test_case_error_ptr_filename_null();
        });
        bool ok = (output.find("\nUNKNOWN (ERROR!)") != std::string::npos) &&
                  (output.find("libpng warning:") != std::string::npos);
        if (ok) {
            std::cout << "[PASS] test_case_error_ptr_filename_null\n";
            ++passed;
        } else {
            std::cout << "[FAIL] test_case_error_ptr_filename_null\n";
            std::cout << "Output: " << output << "\n";
        }
    }

    std::cout << "Tests passed: " << passed << " / " << total << "\n";
}

int main() {
    // Run the test suite
    run_all_tests();
    return 0;
}