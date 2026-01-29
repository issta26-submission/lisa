// Test suite for the focal function: makepng_warning
// Notes:
// - This test assumes the environment provides libpng and the standard png.h header.
// - The tests are written in C++11 without GoogleTest. A small, lightweight test harness is used.
// - The tests cover true/false branches of the internal decision about the error name.
// - Output is captured by redirecting stderr to a temporary file for verification.

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <string>
#include <fstream>
#include <zlib.h>
#include <errno.h>
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <functional>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <png.h>
#include <ctype.h>


// Include libpng headers to access png_structp, png_const_charp, etc.

// Declaration of the focal function (defined in makepng.c)
extern "C" void makepng_warning(png_structp png_ptr, png_const_charp message);

/*
  Helper: capture the stderr output produced by a function call.
  Implementation uses freopen to redirect stderr to a temporary file,
  then restores stderr and reads the file content.
*/
static std::string capture_stderr_output(const std::function<void()> &fn) {
    // Create a temporary file for capturing stderr
    const char *tmpname = "makepng_warning_output.txt";

    // Save the current stderr file descriptor
    int saved_stderr_fd = dup(fileno(stderr));
    if (saved_stderr_fd == -1) {
        // If dup fails, fall back to empty string (test cannot proceed)
        return "";
    }

    // Redirect stderr to a file
    if (freopen(tmpname, "w", stderr) == NULL) {
        // If redirection fails, restore and return empty
        dup2(saved_stderr_fd, fileno(stderr));
        close(saved_stderr_fd);
        return "";
    }

    // Execute the test action which should write to stderr
    fn();

    // Flush and ensure data is written
    fflush(stderr);

    // Restore stderr to the original descriptor
    dup2(saved_stderr_fd, fileno(stderr));
    // Close the saved descriptor; the original fd is now restored
    close(saved_stderr_fd);

    // Read the captured content
    std::ifstream in(tmpname);
    std::string output((std::istreambuf_iterator<char>(in)),
                       std::istreambuf_iterator<char>());
    in.close();

    return output;
}

/*
  Test 1: ep (error_ptr) is non-NULL and *ep is non-NULL.
  We set the error_ptr to point to a non-null C-string name and verify the printed
  line uses that name.
*/
static bool test_makepng_warning_named_error_ptr() {
    // Create a png_ptr using libpng API
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        std::cerr << "Test 1: Failed to create png_ptr\n";
        return false;
    }

    // Prepare a non-null error_ptr: &name_ptr where name_ptr points to a string
    const char *name = "custom_name_test";
    const char **name_ptr = &name;
    // Set the error_ptr via libpng API
    png_set_error_fn(png_ptr, (png_voidp)name_ptr, NULL, NULL);

    // Capture the warning output
    auto captured = capture_stderr_output([&]() {
        makepng_warning(png_ptr, "test message 1");
    });

    // Expected output: "custom_name_test: warning: test message 1\n"
    const std::string expected = std::string(name) + std::string(": warning: ") + "test message 1" + "\n";

    // Cleanup
    png_destroy_read_struct(&png_ptr, NULL, NULL);

    // Compare
    if (captured == expected) {
        return true;
    } else {
        std::cerr << "Test 1 failed. Expected: \"" << expected
                  << "\" Got: \"" << captured << "\"\n";
        return false;
    }
}

/*
  Test 2: ep is non-NULL but *ep is NULL.
  We set the error_ptr to point to a NULL name and verify the fallback "makepng" is used.
*/
static bool test_makepng_warning_null_name_ptr() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        std::cerr << "Test 2: Failed to create png_ptr\n";
        return false;
    }

    const char *null_name = NULL;
    const char **name_ptr = &null_name;
    png_set_error_fn(png_ptr, (png_voidp)name_ptr, NULL, NULL);

    auto captured = capture_stderr_output([&]() {
        makepng_warning(png_ptr, "test message 2");
    });

    const std::string expected = "makepng: warning: test message 2\n";

    png_destroy_read_struct(&png_ptr, NULL, NULL);

    if (captured == expected) {
        return true;
    } else {
        std::cerr << "Test 2 failed. Expected: \"" << expected
                  << "\" Got: \"" << captured << "\"\n";
        return false;
    }
}

/*
  Test 3: ep is NULL.
  We pass NULL as the error_ptr and verify the default "makepng" name is used.
*/
static bool test_makepng_warning_ep_null() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        std::cerr << "Test 3: Failed to create png_ptr\n";
        return false;
    }

    // Do not set any error_ptr (pass NULL)
    png_set_error_fn(png_ptr, NULL, NULL, NULL);

    auto captured = capture_stderr_output([&]() {
        makepng_warning(png_ptr, "test message 3");
    });

    const std::string expected = "makepng: warning: test message 3\n";

    png_destroy_read_struct(&png_ptr, NULL, NULL);

    if (captured == expected) {
        return true;
    } else {
        std::cerr << "Test 3 failed. Expected: \"" << expected
                  << "\" Got: \"" << captured << "\"\n";
        return false;
    }
}

/*
  Main test runner:
  - Invokes all test cases.
  - Reports per-test results and a final summary.
  - Mirrors a lightweight testing harness suitable for environments without GoogleTest.
*/
int main() {
    int total = 3;
    int passed = 0;

    std::cout << "Running test_makepng_warning test suite...\n";

    if (test_makepng_warning_named_error_ptr()) {
        std::cout << "[PASS] test_makepng_warning_named_error_ptr\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_makepng_warning_named_error_ptr\n";
    }

    if (test_makepng_warning_null_name_ptr()) {
        std::cout << "[PASS] test_makepng_warning_null_name_ptr\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_makepng_warning_null_name_ptr\n";
    }

    if (test_makepng_warning_ep_null()) {
        std::cout << "[PASS] test_makepng_warning_ep_null\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_makepng_warning_ep_null\n";
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";

    return (passed == total) ? 0 : 1;
}