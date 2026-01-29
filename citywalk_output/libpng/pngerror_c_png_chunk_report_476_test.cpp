// Minimal C++-based unit test harness for the focal method: png_chunk_report
// This test suite uses hand-written assertions (no external test framework).
// It relies on a mocked, minimal subset of libpng types and functions to drive
// the behavior of png_chunk_report for both read and write modes.
//
// Important: This test is self-contained in terms of the test harness and mocks.
// It is designed to be linked with the actual pngerror.c (the focal file) in a
// project that compiles C code with C linkage. The mocks provide the necessary
// behavior to verify which internal branch of png_chunk_report is exercised.

#include <iostream>
#include <pngpriv.h>
#include <cstring>


// ---------------------------------------------------------------------------
// Mocked libpng-like environment (only what's needed by the focal method)
// ---------------------------------------------------------------------------

// Forward declare the types used by png_chunk_report to minimize coupling.
struct png_struct {
    unsigned int mode;
};

// Typedefs mirroring the libpng style used in the focal method
typedef struct png_struct* png_structrp;       // non-const pointer
typedef const struct png_struct* png_const_structrp; // const pointer

typedef const char* png_const_charp;

// Bit flag used by png_chunk_report to determine read vs write mode
#define PNG_IS_READ_STRUCT 0x01

// Section-wide feature macros (as used by the focal method's #if guards)
#define PNG_READ_SUPPORTED
#define PNG_WRITE_SUPPORTED

// Error thresholds used by the focal method
#define PNG_CHUNK_ERROR 100
#define PNG_CHUNK_WRITE_ERROR 200

// When PNG_WARNINGS_SUPPORTED is not defined, the code uses PNG_UNUSED(message).
// For the test, we enable warnings support to exercise the warning path cleanly.
// If the library you test against defines PNG_WARNINGS_SUPPORTED, this section
// is effectively ignored in png_chunk_report (we still define PNG_UNUSED for safety).
#define PNG_WARNINGS_SUPPORTED

// Ensure unused parameters don't warn with a portable approach
#ifndef PNG_UNUSED
#define PNG_UNUSED(x) (void)(x)
#endif

// Prototypes of the focal function under test (C linkage)
extern "C" void png_chunk_report(png_const_structrp png_ptr, png_const_charp message, int error);

// Prototypes for the mocked callback hooks invoked by png_chunk_report
extern "C" void png_chunk_warning(png_const_structrp png_ptr, png_const_charp warning_message);
extern "C" void png_chunk_benign_error(png_const_structrp png_ptr, png_const_charp error_message);
extern "C" void png_app_warning(png_const_structrp png_ptr, png_const_charp error_message);
extern "C" void png_app_error(png_const_structrp png_ptr, png_const_charp error_message);

// Global state to observe which callback was invoked and with what message
enum class CallKind {
    NONE = -1,
    CHUNK_WARNING,
    CHUNK_BENIGN_ERROR,
    APP_WARNING,
    APP_ERROR
};

static CallKind last_call = CallKind::NONE;
static const char* last_message = nullptr;
static int last_ptr_identity = 0; // optional, helps differentiate instances if needed

// A lightweight mock PNG struct to be passed as png_ptr
static struct png_struct test_png_struct;

// Helpers to reset the observer state for each test
static void reset_observer() {
    last_call = CallKind::NONE;
    last_message = nullptr;
    last_ptr_identity = 0;
}

// Mock implementations of the callbacks
extern "C" void png_chunk_warning(png_const_structrp png_ptr, png_const_charp warning_message)
{
    (void)png_ptr; // Silence unused parameter warning in some compilers
    last_call = CallKind::CHUNK_WARNING;
    last_message = warning_message;
}

extern "C" void png_chunk_benign_error(png_const_structrp png_ptr, png_const_charp error_message)
{
    (void)png_ptr;
    last_call = CallKind::CHUNK_BENIGN_ERROR;
    last_message = error_message;
}

extern "C" void png_app_warning(png_const_structrp png_ptr, png_const_charp warning_message)
{
    (void)png_ptr;
    last_call = CallKind::APP_WARNING;
    last_message = warning_message;
}

extern "C" void png_app_error(png_const_structrp png_ptr, png_const_charp error_message)
{
    (void)png_ptr;
    last_call = CallKind::APP_ERROR;
    last_message = error_message;
}

// ---------------------------------------------------------------------------
// Lightweight test harness (plain C++11, no GTest)
// ---------------------------------------------------------------------------

// Helper to perform a simple, non-terminating assertion (records failures)
static int g_failures = 0;
static void expect(bool condition, const char* test_desc)
{
    if (!condition) {
        ++g_failures;
        std::cerr << "FAIL: " << test_desc << "\n";
    }
}

// Public interface for a single test: Read branch - chunk warning
static void test_png_chunk_report_read_branch_warning()
{
    // This tests the read-path when error < PNG_CHUNK_ERROR => chunk_warning is invoked
    reset_observer();

    test_png_struct.mode = PNG_IS_READ_STRUCT; // simulate "read" mode

    const char* msg = "read-small-error";
    int error = 50; // < PNG_CHUNK_ERROR (100)

    // Call the focal method (C linkage). The test harness passes a pointer to our mock struct.
    png_chunk_report(reinterpret_cast<png_const_structrp>(&test_png_struct), msg, error);

    // Assertions: we should have invoked chunk_warning with the exact same message
    expect(last_call == CallKind::CHUNK_WARNING, "png_chunk_report should call chunk_warning in read path with small error");
    expect(last_message == msg, "png_chunk_report should pass through the message to chunk_warning");
}

// Public interface for a single test: Read branch - chunk benign error
static void test_png_chunk_report_read_branch_benign()
{
    // Test read-path when error >= PNG_CHUNK_ERROR => chunk_benign_error is invoked
    reset_observer();

    test_png_struct.mode = PNG_IS_READ_STRUCT; // simulate "read" mode

    const char* msg = "read-large-error";
    int error = 150; // >= PNG_CHUNK_ERROR (100)

    png_chunk_report(reinterpret_cast<png_const_structrp>(&test_png_struct), msg, error);

    // Assertions
    expect(last_call == CallKind::CHUNK_BENIGN_ERROR, "png_chunk_report should call chunk_benign_error in read path with large error");
    expect(last_message == msg, "png_chunk_report should pass through the message to chunk_benign_error");
}

// Public interface for a single test: Write branch - app warning
static void test_png_chunk_report_write_branch_warning()
{
    // Test write-path when not in read mode and error < PNG_CHUNK_WRITE_ERROR => app_warning
    reset_observer();

    test_png_struct.mode = 0; // not read-struct; implies write path when both READ and WRITE are defined

    const char* msg = "write-small-error";
    int error = 50; // < PNG_CHUNK_WRITE_ERROR (200)

    png_chunk_report(reinterpret_cast<png_const_structrp>(&test_png_struct), msg, error);

    // Assertions
    expect(last_call == CallKind::APP_WARNING, "png_chunk_report should call app_warning in write path with small error");
    expect(last_message == msg, "png_chunk_report should pass through the message to app_warning");
}

// Public interface for a single test: Write branch - app error
static void test_png_chunk_report_write_branch_error()
{
    // Test write-path when not in read mode and error >= PNG_CHUNK_WRITE_ERROR => app_error
    reset_observer();

    test_png_struct.mode = 0; // not read-struct

    const char* msg = "write-large-error";
    int error = 250; // >= PNG_CHUNK_WRITE_ERROR (200)

    png_chunk_report(reinterpret_cast<png_const_structrp>(&test_png_struct), msg, error);

    // Assertions
    expect(last_call == CallKind::APP_ERROR, "png_chunk_report should call app_error in write path with large error");
    expect(last_message == msg, "png_chunk_report should pass through the message to app_error");
}

// Helper to run all tests in sequence
static void run_all_tests()
{
    std::cout << "Running png_chunk_report unit tests (mocked environment)..." << std::endl;

    test_png_chunk_report_read_branch_warning();
    test_png_chunk_report_read_branch_benign();
    test_png_chunk_report_write_branch_warning();
    test_png_chunk_report_write_branch_error();

    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
    }
}

// Main entry: run tests from a single executable
int main()
{
    run_all_tests();
    return (g_failures == 0) ? 0 : 1;
}

// Note for maintainers:
// - This test harness mocks the essential parts of libpng used by png_chunk_report.
// - It exercises both the read path and the write path, covering true/false branches
//   of each conditional predicate.
// - It uses simple, non-terminating EXPECT-like assertions (implemented as expect()).
// - The test is designed to be compiled together with the real pngerror.c in a C
//   environment; ensure the build system links this file with the C sources so that
//   the actual png_chunk_report implementation is exercised with the mocked callbacks.