// Test suite for gzerror function from gzlib.c
// This test is written for C++11 compilation without GoogleTest.
// It exercises different branches of gzerror by constructing internal
// gz_state_s structures and casting them to gzFile (opaque pointer).

#include <iostream>
#include <gzguts.h>
#include <zlib.h>
#include <cstring>


// Import internal state structure and public API

// Helper for simple, non-terminating test logs
static int tests_run = 0;
static int tests_passed = 0;

// Log a single test result
static void log_result(bool ok, const char* name) {
    if (ok) {
        ++tests_passed;
        std::cout << "[PASS] " << name << std::endl;
    } else {
        std::cout << "[FAIL] " << name << std::endl;
    }
    ++tests_run;
}

// Test 1: When file is NULL, gzerror should return NULL (no processing)
static void test_gzerror_null_input() {
    // Expose behavior when input file pointer is NULL
    const char* res = gzerror(nullptr, nullptr);
    log_result(res == nullptr, "gzerror(nullptr, nullptr) should return NULL");
}

// Test 2: When state->mode is neither GZ_READ nor GZ_WRITE, gzerror should return NULL
static void test_gzerror_invalid_mode() {
    // Prepare a non-valid mode state
    struct gz_state_s st;
    st.mode = 9999; // invalid mode
    st.err = 0;
    st.msg = "shouldn't be used";

    gzFile f = (gzFile)&st;
    int err_dummy = -1;
    const char* res = gzerror(f, &err_dummy);
    log_result(res == nullptr, "gzerror with invalid mode returns NULL");
}

// Test 3: Valid GZ_READ with an error number and a non-null message returns that message and error number
static void test_gzerror_valid_read_with_msg_and_errnum() {
    struct gz_state_s st;
    st.mode = GZ_READ;
    st.err = 12345;
    st.msg = "read error occurred";

    gzFile f = (gzFile)&st;
    int err_out = -1;
    const char* res = gzerror(f, &err_out);

    bool ok = (res != nullptr) &&
              (std::strcmp(res, "read error occurred") == 0) &&
              (err_out == 12345);

    log_result(ok, "gzerror with GZ_READ returns saved msg and errnum");
}

// Test 4: When err == Z_MEM_ERROR, gzerror should return "out of memory" and propagate errnum
static void test_gzerror_mem_error_case() {
    struct gz_state_s st;
    st.mode = GZ_READ;
    st.err = Z_MEM_ERROR;
    st.msg = "some message";

    gzFile f = (gzFile)&st;
    int err_out = -1;
    const char* res = gzerror(f, &err_out);

    bool ok = (res != nullptr) &&
              (std::strcmp(res, "out of memory") == 0) &&
              (err_out == Z_MEM_ERROR);

    log_result(ok, "gzerror returns 'out of memory' when Z_MEM_ERROR is set");
}

// Test 5: When err != Z_MEM_ERROR and msg == NULL, gzerror should return empty string ""
static void test_gzerror_msg_null_returns_empty() {
    struct gz_state_s st;
    st.mode = GZ_READ;
    st.err = 7;
    st.msg = NULL;

    gzFile f = (gzFile)&st;
    int err_out = -1;
    const char* res = gzerror(f, &err_out);

    bool ok = (res != nullptr) &&
              (std::strcmp(res, "") == 0) &&
              (err_out == 7);

    log_result(ok, "gzerror returns empty string when msg is NULL and err is non-mem-error");
}

int main() {
    // Run all tests
    test_gzerror_null_input();
    test_gzerror_invalid_mode();
    test_gzerror_valid_read_with_msg_and_errnum();
    test_gzerror_mem_error_case();
    test_gzerror_msg_null_returns_empty();

    // Summary
    std::cout << "\nTest results: " << tests_passed << " / " << tests_run
              << " tests passed." << std::endl;

    return (tests_run == tests_passed) ? 0 : 1;
}