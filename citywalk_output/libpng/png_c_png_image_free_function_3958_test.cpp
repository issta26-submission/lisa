// A self-contained C++11 unit test suite for the focal function
// png_image_free_function(png_voidp argument) with minimal, test-oriented stubs.
// This file is designed to compile standalone without GTest and to maximize
// code coverage across the function's branches.

#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <pngpriv.h>
#include <cstdio>


// Domain-oriented guidelines adhered to within the test suite:
// - Use only standard library (plus the provided function stubs).
// - Implement non-terminating checks (accumulate failures instead of aborting).
// - Access static-like members via direct class/struct names where applicable.
// - Provide explanatory comments for each test case.

// Define minimal, test-oriented stubs to mimic required libpng structures
// and functions used by the focal function. The goal is to exercise logic
// branches without pulling in the real libpng code.

#define PNG_STDIO_SUPPORTED 1
#define PNG_SIMPLIFIED_WRITE_SUPPORTED 1
#define PNG_SIMPLIFIED_READ_SUPPORTED 1

// Forward declarations of test stubs
struct png_struct;
struct png_image;
struct png_control;
struct png_info;
typedef void* png_voidp;

// Utility macro to mimic the original code's casting
#define png_voidcast(type, x) ((type)(x))

// Minimal libpng-like structures for testing
struct png_struct {
    void* io_ptr; // simulate IO pointer (FILE*)
};

struct png_info {
    // empty placeholder for test purposes
};

struct png_control {
    png_struct* png_ptr;
    png_info* info_ptr;
    int owned_file;
    int for_write;
};

// Public-ish types consistent with focal code usage
typedef struct png_image* png_imagep;

// The focal function expects an opaque pointer to a png_image, which internally
// contains a pointer to the associated control structure.
struct png_image {
    png_control* opaque;
};

// Global test state to verify that the focal function interacts with the
// stubs as expected.
static bool free_called_with_cp = false;
static png_control* free_cp_argument = nullptr;

static bool write_destroy_called = false;
static png_struct* last_destroyed_write_png_ptr = nullptr;
static png_info* last_destroyed_write_info_ptr = nullptr;

static bool read_destroy_called = false;
static png_struct* last_destroyed_read_png_ptr = nullptr;
static png_info* last_destroyed_read_info_ptr = nullptr;

// Helper: reset all test state between tests
static void reset_test_state() {
    free_called_with_cp = false;
    free_cp_argument = nullptr;
    write_destroy_called = false;
    last_destroyed_write_png_ptr = nullptr;
    last_destroyed_write_info_ptr = nullptr;
    read_destroy_called = false;
    last_destroyed_read_png_ptr = nullptr;
    last_destroyed_read_info_ptr = nullptr;
}

// Stubbed functions to simulate libpng behavior

static void png_free(png_struct* /*png_ptr*/, png_control* cp) {
    // Record that freeing was requested and which control block was passed
    free_called_with_cp = true;
    free_cp_argument = cp;
    // In a real scenario this would deallocate memory; for test, just note it was called
}

// Simulated write-structure destruction
static void png_destroy_write_struct(png_struct** png_ptr_ptr, png_info** info_ptr_ptr) {
    write_destroy_called = true;
    if (png_ptr_ptr && *png_ptr_ptr) {
        last_destroyed_write_png_ptr = *png_ptr_ptr;
    }
    if (info_ptr_ptr && *info_ptr_ptr) {
        last_destroyed_write_info_ptr = *info_ptr_ptr;
    }
}

// Simulated read-structure destruction
static void png_destroy_read_struct(png_struct** png_ptr_ptr, png_info** info_ptr_ptr, void** end_ptr_ptr) {
    read_destroy_called = true;
    if (png_ptr_ptr && *png_ptr_ptr) {
        last_destroyed_read_png_ptr = *png_ptr_ptr;
    }
    if (info_ptr_ptr && *info_ptr_ptr) {
        last_destroyed_read_info_ptr = *info_ptr_ptr;
    }
}

// The focal function under test, copied/adapted for the test environment
int png_image_free_function(png_voidp argument) {
{
   png_imagep image = png_voidcast(png_imagep, argument);
   png_controlp cp = image->opaque;
   png_control c;
   /* Double check that we have a png_ptr - it should be impossible to get here
    * without one.
    */
   if (cp->png_ptr == NULL)
      return 0;
#  ifdef PNG_STDIO_SUPPORTED
      if (cp->owned_file != 0)
      {
         FILE *fp = png_voidcast(FILE *, cp->png_ptr->io_ptr);
         cp->owned_file = 0;
         /* Ignore errors here. */
         if (fp != NULL)
         {
            cp->png_ptr->io_ptr = NULL;
            (void)fclose(fp);
         }
      }
#  endif
   /* Copy the control structure so that the original, allocated, version can be
    * safely freed.  Notice that a png_error here stops the remainder of the
    * cleanup, but this is probably fine because that would indicate bad memory
    * problems anyway.
    */
   c = *cp;
   image->opaque = &c;
   png_free(c.png_ptr, cp);
   /* Then the structures, calling the correct API. */
   if (c.for_write != 0)
   {
#     ifdef PNG_SIMPLIFIED_WRITE_SUPPORTED
         png_destroy_write_struct(&c.png_ptr, &c.info_ptr);
#     else
         // In test environment, this path won't be exercised due to macro
         // configuration, but keep structure for completeness.
#     endif
   }
   else
   {
#     ifdef PNG_SIMPLIFIED_READ_SUPPORTED
         png_destroy_read_struct(&c.png_ptr, &c.info_ptr, NULL);
#     else
#     endif
   }
   /* Success. */
   return 1;
}
}

// Expose a function that the tests can call (static linkage is fine for this test file)
static int call_png_image_free(png_image* img) {
    return png_image_free_function(reinterpret_cast<png_voidp>(img));
}

// Test helpers to create test images and controls

// Create a new control structure with given properties
static png_control* make_control(png_struct* ptr, int for_write, int owned_file) {
    png_control* cp = new png_control();
    cp->png_ptr = ptr;
    cp->info_ptr = new png_info();
    cp->owned_file = owned_file;
    cp->for_write = for_write;
    return cp;
}

// Create a simple, temporary file and return its FILE*; caller must close via fclose
static FILE* create_temp_file() {
    // Use a small temporary file
    char tmpname[] = "libpng_testXXXXXX";
    int fd = mkstemp(tmpname);
    if (fd == -1) return nullptr;
    // Unlink to avoid leaving files behind; file will be deleted on close
    unlink(tmpname);
    FILE* f = fdopen(fd, "w+");
    return f;
}

// Simple test case framework

static int tests_failed = 0;
#define CHECK(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "[FAIL] " << msg << "\n"; \
            ++tests_failed; \
        } else { \
            std::cout << "[PASS] " << msg << "\n"; \
        } \
    } while (0)

static void test_png_image_free_function_png_ptr_null() {
    reset_test_state();

    // Prepare a png_struct with NULL png_ptr to exercise the first branch
    png_struct dummy_ptr;
    dummy_ptr.io_ptr = nullptr;

    png_control* cp = new png_control();
    cp->png_ptr = nullptr;          // triggers early return
    cp->info_ptr = new png_info();
    cp->owned_file = 0;
    cp->for_write = 0;

    png_image img;
    img.opaque = cp;

    int result = call_png_image_free(&img);

    CHECK(result == 0, "png_image_free_function returns 0 when cp->png_ptr is NULL");
    // No frees or destroys should have occurred
    CHECK(!free_called_with_cp, "png_free should not be called when cp->png_ptr is NULL");

    delete cp->info_ptr;
    delete cp;
}

static void test_png_image_free_function_owned_file_closed_and_zeroed() {
    reset_test_state();

    // Set up a png_ptr with a valid io_ptr pointing to a real FILE*
    FILE* tmp = create_temp_file();
    if (!tmp) {
        std::cerr << "[WARN] Unable to create temp file for test; skipping test.\n";
        return;
    }

    png_struct* pngptr = new png_struct();
    pngptr->io_ptr = (void*)tmp; // store FILE* in io_ptr

    png_control* cp = new png_control();
    cp->png_ptr = pngptr;
    cp->info_ptr = new png_info();
    cp->owned_file = 1; // trigger the fclose path
    cp->for_write = 0;

    png_image img;
    img.opaque = cp;

    int result = call_png_image_free(&img);

    // Expect success and owned_file cleared; io_ptr set to NULL after fclose
    CHECK(result == 1, "png_image_free_function returns 1 after owned file path");
    CHECK(cp->owned_file == 0, "cp->owned_file should be reset to 0 after closing file");
    CHECK(pngptr->io_ptr == NULL, "cp->png_ptr->io_ptr should be NULL after fclose");

    // Also verify that png_free was invoked with the given cp
    CHECK(free_called_with_cp, "png_free should be called when freeing control block");
    CHECK(free_cp_argument == cp, "png_free should be called with the original control pointer");

    // Cleanup
    delete cp->info_ptr;
    delete cp;
    delete pngptr;
    // tmp FILE should be closed by fclose in the function; attempt to use it now would be invalid
}

static void test_png_image_free_function_write_path_invokes_destroy_write() {
    reset_test_state();

    // Prepare a non-null png_ptr
    png_struct* pngptr = new png_struct();
    pngptr->io_ptr = nullptr;

    png_control* cp = new png_control();
    cp->png_ptr = pngptr;
    cp->info_ptr = new png_info();
    cp->owned_file = 0;
    cp->for_write = 1; // trigger write path

    png_image img;
    img.opaque = cp;

    int result = call_png_image_free(&img);

    CHECK(result == 1, "png_image_free_function returns 1 for write path");
    CHECK(write_destroy_called, "png_destroy_write_struct should be called for write path");
    CHECK(last_destroyed_write_png_ptr == pngptr, "png_destroy_write_struct should be called with original png_ptr");
    CHECK(last_destroyed_write_info_ptr != nullptr, "png_destroy_write_struct should receive info_ptr");

    // Cleanup
    delete cp->info_ptr;
    delete cp;
    delete pngptr;
}

static void test_png_image_free_function_read_path_invokes_destroy_read() {
    reset_test_state();

    // Prepare a non-null png_ptr
    png_struct* pngptr = new png_struct();
    pngptr->io_ptr = nullptr;

    png_control* cp = new png_control();
    cp->png_ptr = pngptr;
    cp->info_ptr = new png_info();
    cp->owned_file = 0;
    cp->for_write = 0; // trigger read path

    png_image img;
    img.opaque = cp;

    int result = call_png_image_free(&img);

    CHECK(result == 1, "png_image_free_function returns 1 for read path");
    CHECK(read_destroy_called, "png_destroy_read_struct should be called for read path");
    CHECK(last_destroyed_read_png_ptr == pngptr, "png_destroy_read_struct should be called with original png_ptr");
    CHECK(last_destroyed_read_info_ptr != nullptr, "png_destroy_read_struct should receive info_ptr");

    // Cleanup
    delete cp->info_ptr;
    delete cp;
    delete pngptr;
}

// Entry point for running tests
int main() {
    std::cout << "Running png_image_free_function unit tests...\n";

    test_png_image_free_function_png_ptr_null();
    test_png_image_free_function_owned_file_closed_and_zeroed();
    test_png_image_free_function_write_path_invokes_destroy_write();
    test_png_image_free_function_read_path_invokes_destroy_read();

    if (tests_failed > 0) {
        std::cerr << "Total failures: " << tests_failed << "\n";
        return 1;
    }

    std::cout << "All tests completed successfully.\n";
    return 0;
}