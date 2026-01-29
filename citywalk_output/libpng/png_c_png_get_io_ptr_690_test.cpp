// Unit test suite for the focal method: png_get_io_ptr
// This test harness is designed to run without GTest, using a lightweight, non-terminating assertion approach.
// The tests rely on libpng public API. They verify:
// - The function returns NULL when given a NULL input.
// - The function returns the exact io_ptr previously associated with the png_struct via png_set_read_fn.
// Explanatory notes are included above each test function.

#include <cstdlib>
#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <png.h>


// Global counter for test failures (non-terminating assertions)
static int g_failures = 0;

// Lightweight non-terminating assertion macro
#define TEST_EXPECT(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "Test failed: " << (msg) << std::endl; \
        ++g_failures; \
    } \
} while (0)

// Context structure used by the read callback to simulate a real IO buffer state
struct BufState {
    unsigned char* data;
    size_t bytes_left;
};

// Read callback used by png_set_read_fn. It consumes data from BufState and fills the provided buffer.
// This function relies on png_get_io_ptr to access the user-provided IO state.
static void user_read_data(png_structp png_ptr, png_bytep out, png_size_t length)
{
    // Retrieve the user data pointer stored in the png_ptr via the IO pointer mechanism
    BufState* state = static_cast<BufState*>(png_get_io_ptr((png_const_structrp)png_ptr));
    if (state == nullptr) {
        return;
    }

    // Determine how much data we can actually copy in this call
    size_t to_read = (length < state->bytes_left) ? static_cast<size_t>(length) : state->bytes_left;
    if (to_read > 0) {
        std::memcpy(out, state->data, to_read);
        state->data += to_read;
        state->bytes_left -= to_read;
    }
}

// Test 1: Null input should yield NULL from png_get_io_ptr
// Intent: Validate the guard clause in the focal method (handles NULL gracefully).
static void test_png_get_io_ptr_null()
{
    // Act
    png_voidp result = png_get_io_ptr((png_const_structrp)nullptr);

    // Assert
    TEST_EXPECT(result == nullptr, "png_get_io_ptr(NULL) must return NULL");
}

// Test 2: Non-NULL input should return the exact io_ptr associated via png_set_read_fn
// Intent: Ensure the focal method correctly returns the IO pointer stored in the png_struct.
static void test_png_get_io_ptr_non_null()
{
    // Arrange: create a libpng read struct
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    TEST_EXPECT(png_ptr != nullptr, "png_create_read_struct should succeed");

    // Prepare a small buffer as IO state to attach to the png_ptr
    unsigned char src[] = "0123456789";
    BufState bs;
    bs.data = src;
    bs.bytes_left = sizeof(src) - 1; // exclude trailing NUL

    // Act: attach our BufState as the IO pointer using the library's API
    png_set_read_fn(png_ptr, &bs, user_read_data);

    // Retrieve the IO pointer via the focal method
    png_voidp result = png_get_io_ptr((png_const_structrp)png_ptr);

    // Assert: the returned pointer should be exactly the address of our BufState
    TEST_EXPECT(result == &bs, "png_get_io_ptr should return the same io_ptr provided to png_set_read_fn");

    // Cleanup
    png_destroy_read_struct(&png_ptr, NULL, NULL);
}

// Main test runner
int main()
{
    // Step 1: understand the focal method behavior via targeted tests
    test_png_get_io_ptr_null();
    test_png_get_io_ptr_non_null();

    // Summary
    if (g_failures == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return g_failures;
    }
}