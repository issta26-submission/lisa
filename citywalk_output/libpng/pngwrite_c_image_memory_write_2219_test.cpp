// Lightweight C++11 unit tests for the focal function image_memory_write
// This test suite avoids GTest and uses a small, non-terminating assertion mechanism.
// The tests mock only the necessary libpng-like structures and functions to exercise
// the conditional branches inside image_memory_write.

#include <cstdint>
#include <string>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>
#include <cstddef>


// Minimal, self-contained representations to mirror the dependencies of image_memory_write

// Typedefs mimicking libpng types used by the focal method
typedef unsigned char* png_bytep;
typedef size_t png_alloc_size_t;

// Forward declaration of the focal function to be tested
void image_memory_write(struct png_struct* png_ptr, png_bytep data, size_t size);

// Backdoor structures to simulate the libpng write context
typedef struct png_struct {
    void* io_ptr; // backdoor pointer to the display/control block
} png_struct;

// The control block that holds the memory buffer used by image_memory_write
typedef struct png_image_write_control {
    png_alloc_size_t output_bytes; // how many bytes have been written so far
    png_alloc_size_t memory_bytes; // total available memory size
    unsigned char* memory;          // memory buffer to write into
} png_image_write_control;

// Simple macro to emulate the libpng internal cast used in the focal method
#define png_voidcast(type, value) ((type)(value))

// Global error flag to capture calls to png_error without terminating the test
static bool g_png_error_called = false;
static std::string g_last_png_error;

// Minimal mock of the libpng error handler used by the focal method
void png_error(png_struct* /*png_ptr*/, const char* /*msg*/) {
    g_png_error_called = true;
    // Store a readable message for assertion
    g_last_png_error = (msg != nullptr) ? msg : "";
}

// A tiny non-terminating assertion framework (like EXPECT_*).
// Each test can call these; they accumulate failures but do not abort tests.
static int g_fail_count = 0;

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        printf("EXPECT_TRUE failed at %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        ++g_fail_count; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    if(!((a) == (b))) { \
        printf("EXPECT_EQ failed at %s:%d: %s ==? %s (actual: %ld vs expected: %ld)\n", __FILE__, __LINE__, #a, #b, static_cast<long>(a), static_cast<long>(b)); \
        ++g_fail_count; \
    } \
} while(0)

#define EXPECT_STR_EQ(a, b) do { \
    if(((a) == nullptr && (b) == nullptr) || (std::strcmp((a), (b)) == 0)) { \
        /* pass */ \
    } else { \
        printf("EXPECT_STR_EQ failed at %s:%d: %s != %s\n", __FILE__, __LINE__, (a), (b)); \
        ++g_fail_count; \
    } \
} while(0)

// Implementation of the focal function (reproduced for test isolation)
void image_memory_write(png_struct* png_ptr, png_bytep data, size_t size) {
    png_image_write_control *display = png_voidcast(png_image_write_control*,
        png_ptr->io_ptr);
    png_alloc_size_t ob = display->output_bytes;

    // Check for overflow; this should never happen:
    if (size <= ((png_alloc_size_t)-1) - ob) {
        // I don't think libpng ever does this, but just in case:
        if (size > 0) {
            if (display->memory_bytes >= ob + size) // writing
                memcpy(display->memory + ob, data, size);
            // Always update the size
            display->output_bytes = ob + size;
        }
    } else {
        png_error(png_ptr, "png_image_write_to_memory: PNG too big");
    }
}

// Helper to reset test environment before each test
static void reset_error_state() {
    g_png_error_called = false;
    g_last_png_error.clear();
}

// Test 1: True branch where there is enough memory and size > 0
// Expect: bytes are copied into memory, and output_bytes is updated to size
static void test_image_memory_write_basic_copy() {
    // Set up memory buffer and the display/control structure
    unsigned char mem[100];
    memset(mem, 0xAA, sizeof(mem)); // recognizable initial pattern

    png_image_write_control display;
    display.output_bytes = 0;
    display.memory_bytes = 100;
    display.memory = mem;

    png_struct png;
    png.io_ptr = &display; // backdoor: png_ptr->io_ptr points to display

    unsigned char input[10] = {1,2,3,4,5,6,7,8,9,10};

    reset_error_state();
    image_memory_write(&png, input, 10);

    // Assertions
    EXPECT_EQ(display.output_bytes, 10);
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(static_cast<int>(mem[i]), i + 1);
    }
    // No error should have been raised
    EXPECT_TRUE(!g_png_error_called);
}

// Test 2: False branch for memory availability: not enough memory to copy
// Expect: still updates output_bytes to ob+size and does not copy if memory is too small
static void test_image_memory_write_insufficient_memory() {
    unsigned char mem[100];
    memset(mem, 0xFF, sizeof(mem)); // distinct initial pattern

    png_image_write_control display;
    display.output_bytes = 0;
    display.memory_bytes = 9; // less than ob+size (0+10)
    display.memory = mem;

    png_struct png;
    png.io_ptr = &display;

    unsigned char input[10] = {11,22,33,44,55,66,77,88,99,100};

    reset_error_state();
    image_memory_write(&png, input, 10);

    // Assertions
    EXPECT_EQ(display.output_bytes, 10);
    // Memory should remain unchanged due to insufficient memory
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(static_cast<int>(mem[i]), 0xFF);
    }
    EXPECT_TRUE(!g_png_error_called);
}

// Test 3: size == 0: should not touch memory or update output_bytes
static void test_image_memory_write_zero_size() {
    unsigned char mem[50];
    memset(mem, 0x11, sizeof(mem));

    png_image_write_control display;
    display.output_bytes = 5;
    display.memory_bytes = 50;
    display.memory = mem;

    png_struct png;
    png.io_ptr = &display;

    unsigned char input[0] = {}; // zero-length input

    reset_error_state();
    image_memory_write(&png, input, 0);

    // Assertions
    EXPECT_EQ(display.output_bytes, 5); // unchanged
    // Memory should be untouched
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(static_cast<int>(mem[i]), 0x11);
    }
    EXPECT_TRUE(!g_png_error_called);
}

// Test 4: Overflow case triggers png_error
// Set ob to the maximum value so the overflow condition is triggered
static void test_image_memory_write_overflow_error() {
    unsigned char mem[20];
    memset(mem, 0x00, sizeof(mem));

    png_image_write_control display;
    display.output_bytes = static_cast<png_alloc_size_t>(-1); // max value
    display.memory_bytes = 20;
    display.memory = mem;

    png_struct png;
    png.io_ptr = &display;

    unsigned char input[5] = {9,9,9,9,9};

    reset_error_state();
    image_memory_write(&png, input, 5);

    // Assertions
    EXPECT_TRUE(g_png_error_called);
    // The exact text should match the error string
    EXPECT_STR_EQ(g_last_png_error.c_str(), "png_image_write_to_memory: PNG too big");
}

// Entry point for running tests
int main() {
    printf("Running image_memory_write unit tests...\n");

    // Run tests
    test_image_memory_write_basic_copy();
    test_image_memory_write_insufficient_memory();
    test_image_memory_write_zero_size();
    test_image_memory_write_overflow_error();

    // Summary
    if (g_fail_count == 0) {
        printf("All tests passed.\n");
        return 0;
    } else {
        printf("Total failures: %d\n", g_fail_count);
        return 1;
    }
}