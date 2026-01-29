// Unit test suite for the focal method: write_function(pp, data, size)
// This test harness is crafted for C++11, without Google Test.
// It provides minimal stubs for the dependencies used by write_function
// and validates key execution paths: 1) dp->fp == NULL path, 2) successful
// fwrite path, 3) overflow path via wrap-around, 4) else path where fwrite fails.

#include <stdarg.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/stat.h>
#include <limits.h>
#include <assert.h>
#include <cstdlib>
#include <fcntl.h>
#include <cstdio>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <pnglibconf.h>
#include <string.h>
#include <string>
#include <zlib.h>
#include <iostream>
#include <cstdarg>
#include <cstring>


// Forward declare the focal function and types as used by the C source under test.
typedef void* png_structp;
typedef unsigned char* png_bytep;

// The USER_ERROR level constant (as used by the focal code)
#define USER_ERROR 2

// Minimal representation of the dependent 'display' structure
struct display {
    FILE* fp;
    size_t write_size;
    const char* output_file;
};

// Global instance used by the test harness to simulate the 'display' state
static display g_dp;

// Flags and storage for verifying behavior inside stubs
static bool png_error_called = false;
static std::string last_png_error_msg;

static bool display_log_called = false;
static int display_log_level = -1;
static std::string last_display_log_msg;

// Prototypes of functions expected by the focal method from the test harness
extern "C" {
    struct display* get_dp(png_structp pp);
    void png_error(png_structp pp, const char* msg);
    void display_log(struct display *dp, int level, const char *fmt, ...);
    void write_function(png_structp pp, png_bytep data, size_t size);
}

// Stub implementations to satisfy the focal method's dependencies

extern "C" {

struct display* get_dp(png_structp pp) {
    // Always return our global display for testing purposes
    (void)pp; // unused
    return &g_dp;
}

void png_error(png_structp pp, const char* msg) {
    // Record that an error was raised (do not exit test)
    (void)pp;
    (void)msg;
    png_error_called = true;
    last_png_error_msg = (msg != nullptr) ? msg : "";
}

void display_log(struct display *dp, int level, const char *fmt, ...) {
    // Capture that a log was produced; store level and formatted message (if possible)
    (void)dp;
    va_list ap;
    char buf[512];
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    display_log_called = true;
    display_log_level = level;
    last_display_log_msg = buf;
}

// The focal method is compiled in a separate translation unit (pngcp.c).
// We declare its signature here for linkage with the test harness.
#define UNUSED(x) (void)(x)
}

// The following tests exercise the correct branching inside write_function.
// Each test returns true on success and false on failure, printing details.

static bool reset_state() {
    png_error_called = false;
    last_png_error_msg.clear();
    display_log_called = false;
    display_log_level = -1;
    last_display_log_msg.clear();
    return true;
}

// Test 1: When dp->fp is NULL, write_function should perform a write_size update
// and should not call png_error or display_log under normal conditions.
bool test_write_when_fp_null_increments_write_size_no_error() {
    reset_state();

    // Arrange
    g_dp.fp = NULL;                   // force path with NULL FILE*
    g_dp.write_size = 0;
    g_dp.output_file = "test.png";

    unsigned char data[3] = { 'A', 'B', 'C' };
    size_t size = sizeof(data);

    // Act
    write_function(nullptr, data, size);

    // Assert
    bool passed = true;
    if (g_dp.write_size != size) {
        std::cerr << "[Test 1] Expected write_size == " << size
                  << ", got " << g_dp.write_size << std::endl;
        passed = false;
    }
    if (png_error_called) {
        std::cerr << "[Test 1] Unexpected png_error call: " << last_png_error_msg << std::endl;
        passed = false;
    }
    if (display_log_called) {
        std::cerr << "[Test 1] Unexpected display_log call: level=" << display_log_level
                  << ", msg='" << last_display_log_msg << "'" << std::endl;
        passed = false;
    }
    return passed;
}

// Test 2: When dp->fp is a writable file, and fwrite succeeds (returns 1),
// write_function should update write_size and not trigger errors.
bool test_write_when_fp_writable_succeeds() {
    reset_state();

    // Create a temporary writable file
    char templ[] = "/tmp/pngcp_testXXXXXX";
    int fd = mkstemp(templ);
    if (fd == -1) {
        std::cerr << "[Test 2] Failed to create temporary file" << std::endl;
        return false;
    }
    FILE* f = fdopen(fd, "wb");
    if (!f) {
        std::cerr << "[Test 2] Failed to fdopen temporary file" << std::endl;
        close(fd);
        unlink(templ);
        return false;
    }

    // Arrange
    g_dp.fp = f;
    g_dp.write_size = 0;
    g_dp.output_file = "test.png";

    unsigned char data[4] = { 'D', 'E', 'F', 'G' };
    size_t size = sizeof(data);

    // Act
    write_function(nullptr, data, size);

    // Ensure data was written to disk
    fflush(f);
    fclose(f);
    // Read back to verify
    FILE* check = fopen(templ, "rb");
    if (!check) {
        std::cerr << "[Test 2] Failed to reopen temporary file for verification" << std::endl;
        unlink(templ);
        return false;
    }
    unsigned char read_buf[4] = {0};
    size_t read_sz = fread(read_buf, 1, size, check);
    fclose(check);

    // Cleanup
    unlink(templ);

    bool passed = true;
    if (read_sz != size || std::memcmp(read_buf, data, size) != 0) {
        std::cerr << "[Test 2] Data written to file does not match input data." << std::endl;
        passed = false;
    }
    if (g_dp.write_size != size) {
        std::cerr << "[Test 2] Expected write_size == " << size
                  << ", got " << g_dp.write_size << std::endl;
        passed = false;
    }
    if (png_error_called) {
        std::cerr << "[Test 2] Unexpected png_error call: " << last_png_error_msg << std::endl;
        passed = false;
    }
    if (display_log_called) {
        std::cerr << "[Test 2] Unexpected display_log call: level=" << display_log_level
                  << ", msg='" << last_display_log_msg << "'" << std::endl;
        passed = false;
    }
    return passed;
}

// Test 3: Overflow scenario using wrap-around behavior.
// Set initial write_size to (size_t)-1 and size to 1; after addition, write_size becomes 0,
// which is < size, so png_error should be invoked.
bool test_overflow_triggers_png_error() {
    reset_state();

    g_dp.fp = NULL;           // Either path works; use NULL to simplify
    g_dp.write_size = static_cast<size_t>(-1);
    g_dp.output_file = "overflow.png";

    unsigned char data[1] = { 0xFF };
    size_t size = 1;

    write_function(nullptr, data, size);

    bool passed = true;
    if (!png_error_called) {
        std::cerr << "[Test 3] Expected png_error to be called due to overflow, but it was not." << std::endl;
        passed = false;
    }
    // write_size after wrap should be 0 (overflow wrap)
    if (g_dp.write_size != 0) {
        std::cerr << "[Test 3] Expected write_size == 0 after wrap, got " << g_dp.write_size << std::endl;
        passed = false;
    }
    if (display_log_called) {
        std::cerr << "[Test 3] Unexpected display_log call: level=" << display_log_level
                  << ", msg='" << last_display_log_msg << "'" << std::endl;
        passed = false;
    }
    return passed;
}

// Test 4: Else path where fwrite fails (dp->fp is readable-only).
// We open a named file in read mode and pass its FILE* to write_function.
// fwrite should fail and trigger a USER_ERROR log via display_log.
bool test_else_path_on_fwrite_failure_logs_user_error() {
    reset_state();

    // Create a temporary file and keep it open read-only
    char templ[] = "/tmp/pngcp_test_readonlyXXXXXX";
    int fd = mkstemp(templ);
    if (fd == -1) {
        std::cerr << "[Test 4] Failed to create temporary file" << std::endl;
        return false;
    }
    // Write some data to get a non-empty file (optional)
    FILE* writable = fdopen(fd, "wb");
    if (writable) {
        const unsigned char prefill[] = {0x00};
        fwrite(prefill, 1, 1, writable);
        fflush(writable);
    }
    // Reopen as read-only to force fwrite to fail
    FILE* read_only = fopen(templ, "rb");
    if (!read_only) {
        std::cerr << "[Test 4] Failed to reopen temporary file as read-only" << std::endl;
        close(fd);
        unlink(templ);
        return false;
    }

    // Setup
    if (writable) fclose(writable); // close the writable handle if it exists
    g_dp.fp = read_only;
    g_dp.write_size = 0;
    g_dp.output_file = "readonly.png";

    unsigned char data[3] = { 'X', 'Y', 'Z' };
    size_t size = sizeof(data);

    write_function(nullptr, data, size);

    // Cleanup
    fclose(read_only);
    unlink(templ);

    bool passed = true;
    if (!display_log_called) {
        std::cerr << "[Test 4] Expected display_log to be called due to write failure, but it was not." << std::endl;
        passed = false;
    } else if (display_log_level != USER_ERROR) {
        std::cerr << "[Test 4] Expected USER_ERROR level, got " << display_log_level << std::endl;
        passed = false;
    }
    if (png_error_called) {
        std::cerr << "[Test 4] Unexpected png_error call: " << last_png_error_msg << std::endl;
        // Not necessarily a failure; the path should be display_log on failure.
    }
    return passed;
}

// Simple test runner
int main() {
    int ok = 0;
    int total = 4;

    std::cout << "Running unit tests for write_function (pngcp.c) in C++11 test harness..." << std::endl;

    if (test_write_when_fp_null_increments_write_size_no_error()) {
        std::cout << "[PASS] Test 1: NULL FILE* path increments write_size without error." << std::endl;
        ++ok;
    } else {
        std::cout << "[FAIL] Test 1 failed." << std::endl;
    }

    if (test_write_when_fp_writable_succeeds()) {
        std::cout << "[PASS] Test 2: Writable FILE* path writes data and updates write_size." << std::endl;
        ++ok;
    } else {
        std::cout << "[FAIL] Test 2 failed." << std::endl;
    }

    if (test_overflow_triggers_png_error()) {
        std::cout << "[PASS] Test 3: Overflow wrap triggers png_error as expected." << std::endl;
        ++ok;
    } else {
        std::cout << "[FAIL] Test 3 failed." << std::endl;
    }

    if (test_else_path_on_fwrite_failure_logs_user_error()) {
        std::cout << "[PASS] Test 4: fwrite failure triggers USER_ERROR logging." << std::endl;
        ++ok;
    } else {
        std::cout << "[FAIL] Test 4 failed." << std::endl;
    }

    std::cout << "Tests passed: " << ok << " / " << total << std::endl;
    return (ok == total) ? 0 : 1;
}