#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>


// Minimal stand-in types and macros to simulate libpng structures for unit testing.
// This allows testing of png_image_memory_read without requiring the real libpng library.

typedef unsigned char png_byte;
typedef unsigned char const* png_const_bytep;
typedef void* png_voidp;
typedef struct png_struct_def png_struct;
typedef png_struct* png_structp;
typedef void (*png_error_ptr)(png_structp, const char*);

#define png_voidcast(type, ptr) ((type)(ptr))

// Forward declarations to mirror the real code's expectations.
struct png_control_def;
typedef struct png_control_def* png_controlp;

struct png_image_def;
typedef struct png_image_def* png_imagep;

// Real code references within png_image_memory_read
struct png_image_def {
    png_controlp opaque;
};

struct png_control_def {
    png_const_bytep memory;
    size_t size;
};

// The png_struct_def used by png_image_memory_read
struct png_struct_def {
    png_voidp io_ptr;
};

// Global test state to capture error messages from png_error
static std::string g_last_error_message;
static bool g_error_seen = false;

// Mocked error handler to intercept errors from the function under test
extern "C" void png_error(png_structp png_ptr, const char* message)
{
    (void)png_ptr; // Suppress unused parameter warnings in test harness
    g_last_error_message = (message != nullptr) ? message : "";
    g_error_seen = true;
}

// The focal function under test (reproduced to enable isolated unit testing without
// requiring the actual libpng sources). It mirrors the behavior described in the prompt.
void png_image_memory_read(png_structp png_ptr, png_byte* out, size_t need)
{
{
   if (png_ptr != NULL)
   {
      png_imagep image = png_voidcast(png_imagep, png_ptr->io_ptr);
      if (image != NULL)
      {
         png_controlp cp = image->opaque;
         if (cp != NULL)
         {
            png_const_bytep memory = cp->memory;
            size_t size = cp->size;
            if (memory != NULL && size >= need)
            {
               memcpy(out, memory, need);
               cp->memory = memory + need;
               cp->size = size - need;
               return;
            }
            png_error(png_ptr, "read beyond end of data");
         }
      }
      png_error(png_ptr, "invalid memory read");
   }
}
}

// Helper utilities to build test objects

// Simple helper to create a png_ptr-like object pointing to an image and its control
struct TestImageHolder {
    png_imagep image;
    png_controlp control;
    TestImageHolder() : image(nullptr), control(nullptr) {}
};

// Utility to reset error state between tests
static void reset_error_state() {
    g_last_error_message.clear();
    g_error_seen = false;
}

// Test 1: Successful memory read when memory exists and size is sufficient
bool test_memory_read_success() {
    // Prepare memory source
    unsigned char memory[] = { 'A', 'B', 'C', 'D', 'E', 'F' };
    size_t mem_size = sizeof(memory);

    // Build structures
    TestImageHolder holder;
    holder.control = new png_control_def;
    holder.control->memory = memory;
    holder.control->size = mem_size;

    holder.image = new png_image_def;
    holder.image->opaque = holder.control;

    // Create png_ptr-like object
    png_struct_ptr png_ptr = new png_struct_def;
    png_ptr->io_ptr = holder.image; // io_ptr points to image

    unsigned char out[4];

    reset_error_state();
    png_image_memory_read(png_ptr, out, 4);

    // Assertions
    bool passed = true;
    // Expect no error
    if (g_error_seen) {
        std::cerr << "Test 1 Failure: Unexpected error: " << g_last_error_message << std::endl;
        passed = false;
    }
    // Expect output to be first 4 bytes: 'A','B','C','D'
    unsigned char expected[4] = { 'A','B','C','D' };
    if (std::memcmp(out, expected, 4) != 0) {
        std::cerr << "Test 1 Failure: Output data mismatch." << std::endl;
        passed = false;
    }
    // Expect cp->memory advanced by need
    if (holder.control->memory != memory + 4 || holder.control->size != mem_size - 4) {
        std::cerr << "Test 1 Failure: Memory pointer/size not advanced correctly." << std::endl;
        passed = false;
    }

    // Cleanup
    delete holder.image;
    delete holder.control;
    delete png_ptr;
    return passed;
}

// Test 2: Read beyond end of data triggers error and does not modify memory/size
bool test_memory_read_beyond_end() {
    unsigned char memory[] = { '1', '2', '3' };
    size_t mem_size = sizeof(memory);

    TestImageHolder holder;
    holder.control = new png_control_def;
    holder.control->memory = memory;
    holder.control->size = mem_size;

    holder.image = new png_image_def;
    holder.image->opaque = holder.control;

    png_structp png_ptr = new png_struct_def;
    png_ptr->io_ptr = holder.image;

    unsigned char out[5] = {0};

    reset_error_state();
    png_image_memory_read(png_ptr, out, 5);

    bool passed = true;
    // Expect error
    if (!g_error_seen) {
        std::cerr << "Test 2 Failure: Expected error was not reported." << std::endl;
        passed = false;
    } else {
        if (g_last_error_message != "read beyond end of data") {
            std::cerr << "Test 2 Failure: Unexpected error message: " << g_last_error_message << std::endl;
            passed = false;
        }
    }
    // Memory and size should be unchanged
    if (holder.control->memory != memory || holder.control->size != mem_size) {
        std::cerr << "Test 2 Failure: Memory pointer/size should not have changed." << std::endl;
        passed = false;
    }

    delete holder.image;
    delete holder.control;
    delete png_ptr;
    return passed;
}

// Test 3: Invalid memory read path when cp == NULL
bool test_invalid_memory_read_cp_null() {
    unsigned char memory[] = { 'X','Y','Z' };
    size_t mem_size = sizeof(memory);

    TestImageHolder holder;
    holder.control = new png_control_def;
    holder.control->memory = memory;
    holder.control->size = mem_size;

    holder.image = new png_image_def;
    holder.image->opaque = nullptr; // cp == NULL

    png_structp png_ptr = new png_struct_def;
    png_ptr->io_ptr = holder.image;

    unsigned char out[4] = {0};

    reset_error_state();
    png_image_memory_read(png_ptr, out, 3);

    bool passed = true;
    if (!g_error_seen) {
        std::cerr << "Test 3 Failure: Expected error for invalid memory read was not reported." << std::endl;
        passed = false;
    } else {
        if (g_last_error_message != "invalid memory read") {
            std::cerr << "Test 3 Failure: Unexpected error message: " << g_last_error_message << std::endl;
            passed = false;
        }
    }
    // Cleanup
    delete holder.image;
    delete holder.control;
    delete png_ptr;
    return passed;
}

// Test 4: image pointer (io_ptr) is NULL -> should still lead to error "invalid memory read"
bool test_invalid_memory_read_image_null() {
    unsigned char memory[] = { 'A','B' };
    size_t mem_size = sizeof(memory);

    TestImageHolder holder;
    holder.control = new png_control_def;
    holder.control->memory = memory;
    holder.control->size = mem_size;
    // image allocated but io_ptr will be NULL
    holder.image = new png_image_def;
    holder.image->opaque = holder.control;

    png_structp png_ptr = new png_struct_def;
    png_ptr->io_ptr = nullptr; // image will be NULL after cast

    unsigned char out[2] = {0};

    reset_error_state();
    png_image_memory_read(png_ptr, out, 2);

    bool passed = true;
    if (!g_error_seen) {
        std::cerr << "Test 4 Failure: Expected error for invalid memory read (image NULL) was not reported." << std::endl;
        passed = false;
    } else {
        if (g_last_error_message != "invalid memory read") {
            std::cerr << "Test 4 Failure: Unexpected error message: " << g_last_error_message << std::endl;
            passed = false;
        }
    }
    delete holder.image;
    delete holder.control;
    delete png_ptr;
    return passed;
}

// Test 5: png_ptr is NULL -> no error should be reported
bool test_null_png_ptr_no_error() {
    unsigned char memory[] = { 'Z','9' };
    size_t mem_size = sizeof(memory);

    TestImageHolder holder;
    holder.control = new png_control_def;
    holder.control->memory = memory;
    holder.control->size = mem_size;

    holder.image = new png_image_def;
    holder.image->opaque = holder.control;

    unsigned char out[2] = {0};

    // Intentionally set png_ptr to NULL
    png_structp png_ptr = nullptr;

    reset_error_state();
    png_image_memory_read(png_ptr, out, 2);

    bool passed = true;
    // No error should be reported when png_ptr is NULL
    if (g_error_seen) {
        std::cerr << "Test 5 Failure: Unexpected error reported when png_ptr is NULL." << std::endl;
        passed = false;
    }

    delete holder.image;
    delete holder.control;
    return passed;
}

// Entry point to run all tests
int main() {
    int total = 0;
    int passed = 0;

    auto run = [&](bool result, const char* name){
        total++;
        if (result) {
            passed++;
            std::cout << "[PASS] " << name << std::endl;
        } else {
            std::cout << "[FAIL] " << name << std::endl;
        }
    };

    // Execute tests
    run(test_memory_read_success(), "test_memory_read_success");
    run(test_memory_read_beyond_end(), "test_memory_read_beyond_end");
    run(test_invalid_memory_read_cp_null(), "test_invalid_memory_read_cp_null");
    run(test_invalid_memory_read_image_null(), "test_invalid_memory_read_image_null");
    run(test_null_png_ptr_no_error(), "test_null_png_ptr_no_error");

    std::cout << "Summary: " << passed << "/" << total << " tests passed." << std::endl;
    if (passed != total) {
        std::cerr << "Some tests failed. See above for details." << std::endl;
        return 1;
    }
    return 0;
}