// Unit test suite for the focal method: png_image_error
// This tests the behavior of logging an error into a png_image object,
// flagging the error, and freeing the image as per the original function.
//
// Notes:
// - This test suite is self-contained and mocks the minimal dependencies
//   used by png_image_error (png_safecat, png_image_free, and the image struct).
// - No gtest/gmock is used; a lightweight test harness with non-terminating
//   assertions is provided to maximize execution and coverage.
// - The code is written for C++11 compatibility.

#include <iostream>
#include <pngpriv.h>
#include <cstring>
#include <cstddef>


// Domain Knowledge adaptation:
// - We simulate the PNG-related types and constants.
// - We only implement the subset required by the focal method under test.

////////////////////////////////////////////////////////////////////////////////
// Candidate Keywords (core components of the focal method and its deps)
////////////////////////////////////////////////////////////////////////////////
// PNG_IMAGE_ERROR: bit flag set when an image-level error occurs
// png_imagep: pointer to a png_image structure
// png_const_charp: const char* type for error messages
// png_safecat: safe string concatenation routine used to log errors
// png_image_free: frees a png_image object
// png_image_error: the focal function to test

////////////////////////////////////////////////////////////////////////////////
// Minimal Mocked Types and Globals (to support the focal method in tests)
////////////////////////////////////////////////////////////////////////////////

// Expose a small, self-contained API surface that mimics the necessary parts
#define PNG_IMAGE_ERROR 0x01

// Forward declaration to mimic the real API
struct png_image;
typedef struct png_image* png_imagep;
typedef const char* png_const_charp;

// Global to observe deallocation call during tests
static png_imagep last_freed_image = nullptr;

// Minimal png_image structure used by the focal method
struct png_image {
    // Reasonable buffer size to test both normal and boundary behavior
    char message[128];
    unsigned int warning_or_error;
};

// Mocked dependencies (to be resolved by the test suite)
extern "C" {

// Forward declare the functions that the focal method uses
int png_image_error(png_imagep image, png_const_charp error_message);
void png_image_free(png_imagep image);
void png_safecat(char *dest, size_t size, size_t pos, const char* src);

// Implementations aligned with the focal method's expectations

// Safe concatenation: write at position 'pos' into 'dest' with total capacity 'size'.
// This is a simplistic emulation of libpng's png_safecat behavior.
void png_safecat(char *dest, size_t size, size_t pos, const char* src) {
    if (dest == nullptr || size == 0) {
        return;
    }
    if (pos >= size) {
        return;
    }
    size_t space = size - pos;
    // Leave room for null-terminator
    size_t max_copy = (space > 0) ? space - 1 : 0;

    size_t i = 0;
    while (i < max_copy && src != nullptr && src[i] != '\0') {
        dest[pos + i] = src[i];
        ++i;
    }
    // Null-terminate
    dest[pos + i] = '\0';
}

// Emulate freeing of the image: we do not actually delete memory here to enable
// post-call inspection of the message field, but record that a free occurred.
void png_image_free(png_imagep image) {
    // Mark that this image was freed (test can inspect)
    last_freed_image = image;
    // Do not delete to allow inspection of contents after "free"
    // In real code, this would deallocate resources.
}

// The focal method under test (reproduced here to enable isolated unit tests).
int png_image_error(png_imagep image, png_const_charp error_message)
{
{
   /* Utility to log an error. */
   png_safecat(image->message, (sizeof image->message), 0, error_message);
   image->warning_or_error |= PNG_IMAGE_ERROR;
   png_image_free(image);
   return 0;
}
}

// End extern "C" block
} // extern "C"

////////////////////////////////////////////////////////////////////////////////
// Lightweight Test Harness (non-terminating assertions)
////////////////////////////////////////////////////////////////////////////////

static int test_failures = 0;
#define ASSERT_TRUE(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "ASSERT_TRUE failed: " << (msg) << " in " << __FUNCTION__ << "\n"; \
            ++test_failures; \
        } \
    } while(0)

#define ASSERT_EQ(a, b, msg) \
    do { \
        if(!((a) == (b))) { \
            std::cerr << "ASSERT_EQ failed: " #a " != " #b " (" << (a) << " != " << (b) \
                      << ") in " << __FUNCTION__ << " - " << (msg) << "\n"; \
            ++test_failures; \
        } \
    } while(0)


// Utility to create a fresh image object
static png_imagep create_image() {
    png_imagep img = new png_image();
    std::memset(img->message, 0, sizeof(img->message));
    img->warning_or_error = 0;
    return img;
}

// Utility to reset last_freed pointer
static void reset_last_freed() {
    last_freed_image = nullptr;
}

// Test 1: Basic logging of a simple error message
// - Verifies that the error flag is set
// - Verifies that the error message is logged into image->message
// - Verifies that the image is "freed" by the mocked png_image_free
static void test_basic_logging() {
    // EXPECT: Typical error message is logged and state updated
    reset_last_freed();
    png_imagep img = create_image();

    const char* err = "example error occurred";

    int ret = png_image_error(img, err);

    // Return value should be 0
    ASSERT_EQ(ret, 0, "png_image_error should return 0");

    // PNG_IMAGE_ERROR flag should be set
    ASSERT_TRUE((img->warning_or_error & PNG_IMAGE_ERROR) != 0, "PNG_IMAGE_ERROR flag should be set");

    // The message should contain the exact error string
    ASSERT_TRUE(std::strcmp(img->message, err) == 0, "image message should equal the error_message");

    // png_image_free should have been invoked with the same image pointer
    ASSERT_TRUE(last_freed_image == img, "png_image_free should be called with the image");

    // Cleanup
    delete img;
}

// Test 2: Empty error message handling
// - Ensures that an empty string is logged without crashing
// - Verifies that the message field ends up as an empty string
static void test_empty_message() {
    reset_last_freed();
    png_imagep img = create_image();

    const char* err = "";

    int ret = png_image_error(img, err);

    ASSERT_EQ(ret, 0, "png_image_error should return 0 with empty message");
    ASSERT_TRUE((img->warning_or_error & PNG_IMAGE_ERROR) != 0, "PNG_IMAGE_ERROR flag should be set for empty message");
    ASSERT_TRUE(std::strcmp(img->message, "") == 0, "image message should be empty after logging empty error");
    ASSERT_TRUE(last_freed_image == img, "png_image_free should be called with the image for empty message case");

    delete img;
}

// Test 3: Long error message truncation behavior
// - Verifies that very long messages are truncated to fit the destination buffer
static void test_long_message_truncation() {
    reset_last_freed();
    png_imagep img = create_image();

    // Create a long message (> 127 chars) to force truncation
    const size_t LONG_LEN = 200;
    std::string longMsg;
    longMsg.reserve(LONG_LEN);
    for (size_t i = 0; i < LONG_LEN; ++i) {
        longMsg.push_back('A' + (i % 26)); // cyclic letters
    }
    longMsg.push_back('\0');

    int ret = png_image_error(img, longMsg.c_str());

    ASSERT_EQ(ret, 0, "png_image_error should return 0 for long message");
    ASSERT_TRUE((img->warning_or_error & PNG_IMAGE_ERROR) != 0, "PNG_IMAGE_ERROR flag should be set for long message");

    // Since the destination size is 128 with 0-based pos, we copy at most 127 chars
    // And ensure the string is null-terminated.
    size_t expected_len = 127;
    // The actual string length should be at most 127
    size_t actual_len = std::strlen(img->message);
    ASSERT_TRUE(actual_len <= expected_len, "image message should be truncated to at most 127 chars");
    // The beginning should reflect the long message (truncated at the end)
    ASSERT_TRUE( std::strncmp(img->message, longMsg.c_str(), actual_len) == 0, "image message prefix should match long message up to truncation");

    ASSERT_TRUE(last_freed_image == img, "png_image_free should be called with the image after long message");

    delete img;
}

// Run all tests
int main() {
    std::cout << "Running tests for png_image_error focal method (mocked environment)\n";

    test_basic_logging();
    test_empty_message();
    test_long_message_truncation();

    if (test_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << "Tests completed with " << test_failures << " failure(s).\n";
        return 1;
    }
}