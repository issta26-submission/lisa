/*
Unit Test Suite for the focal method:

png_set_write_fn(png_structrp png_ptr, png_voidp io_ptr,
    png_rw_ptr write_data_fn, png_flush_ptr output_flush_fn)

This test suite is crafted as a standalone C++11 program (no GTest) to validate
the core logic of png_set_write_fn as described in the provided snippet.

Assumptions and simplifications (matching the Unit Test instructions):
- We implement a minimal subset of the relevant png_struct and related types to mirror the
  behavior of the focal function without requiring the full libpng dependency.
- Macros are defined to exercise the true-branch code guarded by PNG_STDIO_SUPPORTED,
  PNG_WRITE_FLUSH_SUPPORTED, and PNG_READ_SUPPORTED as in real code.
- We exercise true/false branches for:
  - write_data_fn != NULL vs NULL
  - output_flush_fn != NULL vs NULL
  - read_data_fn != NULL vs NULL (triggering a warning)
  - NULL png_ptr input case (early return)
- We provide lightweight "mock" implementations for defaults (png_default_write_data, png_default_flush)
  and a small png_warning collector to verify warnings were issued.

Candidate Keywords (Step 1 reproduction in test):
- png_set_write_fn, png_ptr fields (io_ptr, write_data_fn, output_flush_fn, read_data_fn)
- PNG_STDIO_SUPPORTED, PNG_WRITE_FLUSH_SUPPORTED, PNG_READ_SUPPORTED
- png_default_write_data, png_default_flush
- png_warning, validation of function pointers, NULL handling, and warnings

Notes:
- This test is designed to be self-contained and compilable as C++11 code.
- The test uses a non-terminating assertion style: it collects pass/fail counts and prints details.
- The tests are runnable from main() as required by the instruction set (no GTest).

All code is provided below. Compile with a C++11 compiler, e.g., g++ -std=c++11 test_png_set_write_fn.cpp -o test_png.

*/

// Include standard IO for logging and printing test results
#include <cstdint>
#include <iostream>
#include <pngpriv.h>
#include <cstddef>


// -----------------------------------------------------------------------------
// Minimal type definitions to mirror the simplified libpng-like API used in tests
// -----------------------------------------------------------------------------

// forward declaration of the simplified png_struct
struct png_struct;
typedef png_struct* png_structrp;

// pointer types used by the focal function
typedef void (*png_rw_ptr)(png_structrp, unsigned char*, size_t);
typedef void (*png_flush_ptr)(png_structrp);
typedef void* png_voidp;

// a minimal substitute for the libpng pixel data types
typedef unsigned char* png_bytep;
typedef const unsigned char* png_const_bytep;

// Simplified static utilities/macros
#ifndef PNG_UNUSED
#define PNG_UNUSED(x) (void)(x)
#endif

// The "default" write and flush operations (no-ops for our tests)
void png_default_write_data(png_structrp /*png_ptr*/, png_bytep /*data*/, size_t /*length*/) { /* no-op */ }
void png_default_flush(png_structrp /*png_ptr*/) { /* no-op */ }

// Forward declaration of a mock warning function so the focal code can call it
static int g_warning_count = 0;
static const char* g_last_warning = nullptr;
void png_warning(png_structrp /*png_ptr*/, const char* /*message*/) {
    ++g_warning_count;
    g_last_warning = message;
}

// The focal structure used by png_set_write_fn
struct png_struct {
    void* io_ptr;
    png_rw_ptr write_data_fn;
    png_flush_ptr output_flush_fn;
    void* read_data_fn; // keep as pointer type for NULL-check semantics
};

// The focal function under test (recreated in test file to be self-contained)
#define PNG_STDIO_SUPPORTED 1
#define PNG_WRITE_FLUSH_SUPPORTED 1
#define PNG_READ_SUPPORTED 1
#ifndef PNG_UNUSED
#define PNG_UNUSED(x) (void)(x)
#endif

// The actual implementation under test (as per the provided snippet)
void png_set_write_fn(png_structrp png_ptr, png_voidp io_ptr,
    png_rw_ptr write_data_fn, png_flush_ptr output_flush_fn)
{
    {
       if (png_ptr == NULL)
          return;
       png_ptr->io_ptr = io_ptr;
#ifdef PNG_STDIO_SUPPORTED
       if (write_data_fn != NULL)
          png_ptr->write_data_fn = write_data_fn;
       else
          png_ptr->write_data_fn = png_default_write_data;
#else
       png_ptr->write_data_fn = write_data_fn;
#endif
#ifdef PNG_WRITE_FLUSH_SUPPORTED
#  ifdef PNG_STDIO_SUPPORTED
       if (output_flush_fn != NULL)
          png_ptr->output_flush_fn = output_flush_fn;
       else
          png_ptr->output_flush_fn = png_default_flush;
#  else
       png_ptr->output_flush_fn = output_flush_fn;
#  endif
#else
       PNG_UNUSED(output_flush_fn)
#endif /* WRITE_FLUSH */
#ifdef PNG_READ_SUPPORTED
       /* It is an error to read while writing a png file */
       if (png_ptr->read_data_fn != NULL)
       {
          png_ptr->read_data_fn = NULL;
          png_warning(png_ptr,
              "Can't set both read_data_fn and write_data_fn in the"
              " same structure");
       }
#endif
    }
}

// -----------------------------------------------------------------------------
// Test helpers and test cases
// -----------------------------------------------------------------------------

static void reset_test_state() {
    g_warning_count = 0;
    g_last_warning = nullptr;
}

// Helper to report test results
static int test_passed = 0;
static int test_failed = 0;

static void report_result(const std::string& name, bool ok) {
    if (ok) {
        ++test_passed;
        std::cout << "[PASS] " << name << "\n";
    } else {
        ++test_failed;
        std::cout << "[FAIL] " << name << "\n";
    }
}

// Test 1: png_ptr == NULL should be a no-op
static void test_ptr_null_noop() {
    reset_test_state();

    png_set_write_fn(nullptr, (void*)0xDEADBEEF, (png_rw_ptr)0x1, (png_flush_ptr)0x2);

    // Expect no crash and no writes to internal state (since function returns early)
    // There's nothing to check directly, but at least ensure no static crashes occur.
    report_result("test_ptr_null_noop", true); // if we reached here, it's a pass
}

// Test 2: write_data_fn != NULL should be assigned directly
static void test_write_fn_non_null_assigned() {
    reset_test_state();

    png_struct ptr = {};
    ptr.io_ptr = nullptr;
    ptr.write_data_fn = nullptr;
    ptr.output_flush_fn = nullptr;
    ptr.read_data_fn = nullptr;

    // Define a unique test function to assign
    auto test_write = [](png_structrp p, png_bytep data, size_t len) { /* do nothing in test */ };
    auto test_flush = [](png_structrp p) { /* do nothing in test */ };

    png_set_write_fn(&ptr, (void*)0xCAFEBABE, (png_rw_ptr)test_write, (png_flush_ptr)test_flush);

    bool ok = (ptr.io_ptr == (void*)0xCAFEBABE) &&
              (ptr.write_data_fn == (png_rw_ptr)test_write) &&
              (ptr.output_flush_fn == (png_flush_ptr)test_flush) &&
              (ptr.read_data_fn == nullptr);

    report_result("test_write_fn_non_null_assigned", ok);
}

// Test 3: write_data_fn == NULL should use png_default_write_data
static void test_write_fn_null_uses_default() {
    reset_test_state();

    png_struct ptr = {};
    ptr.io_ptr = nullptr;
    ptr.write_data_fn = nullptr;
    ptr.output_flush_fn = nullptr;
    ptr.read_data_fn = nullptr;

    png_set_write_fn(&ptr, (void*)0xABCD, nullptr, (png_flush_ptr)0); // NULL write function, NULL flush intentionally

    bool ok = (ptr.write_data_fn == png_default_write_data);

    report_result("test_write_fn_null_uses_default", ok);
}

// Test 4: output_flush_fn NULL should use png_default_flush when STDIO is supported
static void test_output_flush_null_uses_default() {
    reset_test_state();

    png_struct ptr = {};
    ptr.io_ptr = nullptr;
    ptr.write_data_fn = nullptr;
    ptr.output_flush_fn = nullptr;
    ptr.read_data_fn = nullptr;

    // Provide a non-null write function to ensure only flush gets defaulted
    auto test_write = [](png_structrp p, png_bytep data, size_t len) { /* do nothing */ };

    png_set_write_fn(&ptr, (void*)0x1234, (png_rw_ptr)test_write, nullptr);

    bool ok = (ptr.write_data_fn == (png_rw_ptr)test_write) &&
              (ptr.output_flush_fn == png_default_flush);

    report_result("test_output_flush_null_uses_default", ok);
}

// Test 5: read_data_fn non-NULL should trigger a warning and set read_data_fn to NULL
static void test_read_data_conflict_warns() {
    reset_test_state();

    png_struct ptr = {};
    ptr.io_ptr = nullptr;
    ptr.write_data_fn = nullptr;
    ptr.output_flush_fn = nullptr;
    ptr.read_data_fn = (void*)0xFEEDFACE; // non-NULL triggers warning path

    // Provide non-NULL write_data_fn and non-NULL output_flush_fn to avoid affecting other branches
    auto test_write = [](png_structrp p, png_bytep d, size_t l) { };
    auto test_flush = [](png_structrp p) { };

    // Reset warning counter
    g_warning_count = 0;

    png_set_write_fn(&ptr, (void*)0x9999, (png_rw_ptr)test_write, (png_flush_ptr)test_flush);

    bool ok = (ptr.read_data_fn == nullptr) && (g_warning_count >= 1);

    report_result("test_read_data_conflict_warns", ok);
}

// Run all test cases
static void run_all_tests() {
    std::cout << "Running tests for png_set_write_fn (simplified test harness)\n";
    test_ptr_null_noop();
    test_write_fn_non_null_assigned();
    test_write_fn_null_uses_default();
    test_output_flush_null_uses_default();
    test_read_data_conflict_warns();

    int total = test_passed + test_failed;
    std::cout << "\nTest results: " << test_passed << " passed / " << total << " total, "
              << test_failed << " failed.\n";
}

// Entry point
int main() {
    run_all_tests();
    // Return non-zero if any test failed
    if (test_failed != 0) {
        return 1;
    }
    return 0;
}