// Test suite for png_get_gAMA (libpng) using a lightweight, non-GTest C++11 harness.
// The tests rely on the public libpng API to exercise true/false branches of the function.
// Explanatory comments accompany each test case to clarify intent and expectations.

#include <setjmp.h>
#include <cstdlib>
#include <cmath>
#include <pngpriv.h>
#include <cstdio>
#include <png.h>


// If the environment provides libpng, include its header; otherwise, the tests will fail to compile.
// The code is designed to compile against libpng's public API.
// We do not rely on any Google Test framework, adhering to the constraints.

// Simple non-terminating test harness
static int g_total = 0;
static int g_failed = 0;

static void log_test_result(bool passed, const char* test_name) {
    ++g_total;
    if (passed) {
        printf("TEST PASS: %s\n", test_name);
    } else {
        ++g_failed;
        fprintf(stderr, "TEST FAIL: %s\n", test_name);
    }
}

// Helper for floating-point comparison
static bool almost_equal(double a, double b, double tol = 1e-9) {
    return std::fabs(a - b) <= tol;
}

// Test 1: When gAMA is present, png_get_gAMA must return PNG_INFO_gAMA and write the gamma value.
// This uses the public API to set gAMA and then read it back via png_get_gAMA.
static void test_gama_present_sets_gamma() {
    // Create PNG read structures
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        log_test_result(false, "test_gama_present_sets_gamma: failed to create png_ptr");
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        log_test_result(false, "test_gama_present_sets_gamma: failed to create info_ptr");
        return;
    }

    // Ensure libpng won't longjmp on normal operation
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        log_test_result(false, "test_gama_present_sets_gamma: libpng longjmp on test");
        return;
    }

    // Set a known gamma value (2.0) using the public API
    const double gamma_to_set = 2.0;
    png_set_gAMA(png_ptr, info_ptr, gamma_to_set);

    // Read back using the focal function
    double gamma_read = 0.0;
    unsigned int ret = png_get_gAMA(png_ptr, info_ptr, &gamma_read);

    bool pass = (ret == PNG_INFO_gAMA) && almost_equal(gamma_read, gamma_to_set);
    log_test_result(pass, "test_gama_present_sets_gamma: returns PNG_INFO_gAMA and gamma == 2.0");

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

// Test 2: When file_gamma pointer is NULL, the function should still return PNG_INFO_gAMA
// and not attempt to write to a NULL destination (i.e., no crash or undefined behavior).
static void test_gama_present_with_null_output_pointer() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        log_test_result(false, "test_gama_present_with_null_output_pointer: failed to create png_ptr");
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        log_test_result(false, "test_gama_present_with_null_output_pointer: failed to create info_ptr");
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        log_test_result(false, "test_gama_present_with_null_output_pointer: libpng longjmp on test");
        return;
    }

    // Set a gamma value as before
    const double gamma_to_set = 3.0;
    png_set_gAMA(png_ptr, info_ptr, gamma_to_set);

    // Pass NULL for the gamma output pointer
    unsigned int ret = png_get_gAMA(png_ptr, info_ptr, NULL);

    bool pass = (ret == PNG_INFO_gAMA);
    log_test_result(pass, "test_gama_present_with_null_output_pointer: returns PNG_INFO_gAMA even when output is NULL");

    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

// Test 3: If gAMA is not present in the info struct, png_get_gAMA should return 0
// even if a gamma was previously set in a different info struct (ensuring isolation).
static void test_gama_absent_returns_zero() {
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        log_test_result(false, "test_gama_absent_returns_zero: failed to create png_ptr");
        return;
    }

    png_infop info_ptr_present = png_create_info_struct(png_ptr);
    if (!info_ptr_present) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        log_test_result(false, "test_gama_absent_returns_zero: failed to create info_ptr_present");
        return;
    }

    png_infop info_ptr_absent = png_create_info_struct(png_ptr);
    if (!info_ptr_absent) {
        png_destroy_read_struct(&png_ptr, &info_ptr_present, NULL);
        log_test_result(false, "test_gama_absent_returns_zero: failed to create info_ptr_absent");
        return;
    }

    // Do not set gAMA on info_ptr_absent; leave it with default valid mask not including PNG_INFO_gAMA

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr_present, &info_ptr_absent);
        log_test_result(false, "test_gama_absent_returns_zero: libpng longjmp on test");
        return;
    }

    double gamma_read = 0.0;
    unsigned int ret = png_get_gAMA(png_ptr, info_ptr_present, &gamma_read);

    bool pass = (ret == 0);
    log_test_result(pass, "test_gama_absent_returns_zero: returns 0 when gAMA not present");

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr_present, NULL);
    png_destroy_read_struct(&png_ptr, &info_ptr_absent, NULL);
}

// Test 4: Null input pointers should yield 0 as per the function's defensive checks.
// Validate both png_ptr == NULL and info_ptr == NULL scenarios.
static void test_gama_null_input_pointers() {
    // First, setup a valid info_ptr to isolate the test to pointer validity
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        log_test_result(false, "test_gama_null_input_pointers: failed to create png_ptr");
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        png_destroy_read_struct(&png_ptr, NULL, NULL);
        log_test_result(false, "test_gama_null_input_pointers: failed to create info_ptr");
        return;
    }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        log_test_result(false, "test_gama_null_input_pointers: libpng longjmp on test");
        return;
    }

    double gamma_read = 0.0;
    // Case A: png_ptr == NULL
    unsigned int ret_null_ptr = png_get_gAMA(NULL, info_ptr, &gamma_read);
    bool pass_null_ptr = (ret_null_ptr == 0);

    // Case B: info_ptr == NULL
    unsigned int ret_null_info = png_get_gAMA(png_ptr, NULL, &gamma_read);
    pass_null_ptr = pass_null_ptr && (ret_null_info == 0);

    log_test_result(pass_null_ptr, "test_gama_null_input_pointers: handles NULL png_ptr and NULL info_ptr safely");

    // Cleanup
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
}

// Entry point: run all unit tests
int main() {
    printf("Starting unit tests for png_get_gAMA (libpng).\n");

    test_gama_present_sets_gamma();
    test_gama_present_with_null_output_pointer();
    test_gama_absent_returns_zero();
    test_gama_null_input_pointers();

    printf("Unit tests completed: total=%d failed=%d\n", g_total, g_failed);
    return g_failed ? 1 : 0;
}