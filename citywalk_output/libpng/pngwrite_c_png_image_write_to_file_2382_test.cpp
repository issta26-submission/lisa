// C++11 compliant unit test for png_image_write_to_file (libpng) without using GTest.
// The tests are designed as a lightweight, non-terminating assertion framework.
// Each test includes explanatory comments describing its purpose and how it maps to
// the Candidate Keywords / focal method behavior described in Step 1.
//
// Notes:
// - This test links against libpng (provide -lpng when compiling).
// - Tests are executed from main() without a test framework.
// - The tests exercise various branches of png_image_write_to_file, focusing on argument validation
//   and a successful write path for a minimal 1x1 RGB image.

#include <unistd.h>
#include <cerrno>
#include <sys/stat.h>
#include <cstdlib>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>
#include <png.h>


// Lightweight test harness: non-terminating expectations
static int tests_run = 0;
static int tests_failed = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        ++tests_run; \
        if (!(cond)) { \
            fprintf(stderr, "TEST FAILURE: %s (line %d)\n", msg, __LINE__); \
            ++tests_failed; \
        } \
    } while (0)

#define EXPECT_NOT_NULL(ptr, msg) EXPECT_TRUE((ptr) != NULL, msg)

static bool path_exists(const char* path) {
    return access(path, F_OK) == 0;
}

// Test 1: image == NULL should result in a successful no-op (return 0)
static void test_png_write_null_image_returns_zero() {
    // Candidate Keywords: image NULL, early exit, return 0
    const char* file_name = "/tmp/pngwrite_null_image_test.png";
    const void* buffer = NULL;
    int result = png_image_write_to_file(nullptr, file_name, 0, buffer, 0, nullptr);
    EXPECT_TRUE(result == 0, "png_image_write_to_file with NULL image should return 0");
}

// Test 2: image version mismatch should trigger an error path (non-zero return)
static void test_png_write_wrong_version_returns_error() {
    // Prepare a malformed image (wrong version)
    struct png_image image;
    std::memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION - 1; // incorrect version
    image.width = 1;
    image.height = 1;
    image.format = PNG_FORMAT_RGB;
    image.flags = 0;

    // Create a temporary path via mkstemp, ensuring a unique path
    char path_template[] = "/tmp/pngwrite_wrongver_testXXXXXX.png";
    int fd = mkstemp(path_template);
    if (fd != -1) close(fd);
    // Create minimal buffer for a 1x1 RGB pixel
    unsigned char pixel[3] = { 255, 0, 0 };
    int result = png_image_write_to_file(reinterpret_cast<png_imagep>(&image),
                                         path_template,
                                         0,
                                         pixel,
                                         3,
                                         nullptr);
    EXPECT_TRUE(result != 0, "png_image_write_to_file with wrong version should return non-zero error");

    // Cleanup if the file somehow exists
    if (path_exists(path_template)) {
        unlink(path_template);
    }
}

// Test 3: NULL file_name should trigger invalid argument error path
static void test_png_write_null_filename_returns_error() {
    struct png_image image;
    std::memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.width = 1;
    image.height = 1;
    image.format = PNG_FORMAT_RGB;
    image.flags = 0;

    unsigned char buffer[3] = { 0x12, 0x34, 0x56 };
    int result = png_image_write_to_file(reinterpret_cast<png_imagep>(&image),
                                         nullptr, // invalid: NULL filename
                                         0,
                                         buffer,
                                         3,
                                         nullptr);
    EXPECT_TRUE(result != 0, "png_image_write_to_file with NULL file_name should return non-zero error");
}

// Test 4: NULL buffer should trigger invalid argument error path
static void test_png_write_null_buffer_returns_error() {
    struct png_image image;
    std::memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.width = 1;
    image.height = 1;
    image.format = PNG_FORMAT_RGB;
    image.flags = 0;

    // Valid temporary filename
    char path_template[] = "/tmp/pngwrite_nullbuffer_testXXXXXX.png";
    int fd = mkstemp(path_template);
    if (fd != -1) close(fd);

    int result = png_image_write_to_file(reinterpret_cast<png_imagep>(&image),
                                         path_template,
                                         0,
                                         nullptr, // NULL buffer
                                         3,
                                         nullptr);
    EXPECT_TRUE(result != 0, "png_image_write_to_file with NULL buffer should return non-zero error");

    if (path_exists(path_template)) unlink(path_template);
}

// Test 5: fopen failure path by attempting to open a directory (should fail)
static void test_png_write_fopen_failure_returns_error() {
    // IMAGE valid, but file_name points to a directory (should fail to fopen)
    struct png_image image;
    std::memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.width = 1;
    image.height = 1;
    image.format = PNG_FORMAT_RGB;
    image.flags = 0;

    unsigned char buffer[3] = { 0xAA, 0xBB, 0xCC };
    const char* dir_path = "/tmp"; // a directory, opens as error
    int result = png_image_write_to_file(reinterpret_cast<png_imagep>(&image),
                                         dir_path,
                                         0,
                                         buffer,
                                         3,
                                         nullptr);
    EXPECT_TRUE(result != 0, "png_image_write_to_file with directory as file_name should return non-zero error");
}

// Test 6: Success path with a minimal valid 1x1 RGB image.
// This exercises the actual encoding path and the cleanup (fclose + remove) behavior.
static void test_png_write_success_path() {
    // Create a minimal valid 1x1 RGB image (the libpng encoder will attempt to encode)
    struct png_image image;
    std::memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;
    image.width = 1;
    image.height = 1;
    image.format = PNG_FORMAT_RGB;
    image.flags = 0;

    unsigned char buffer[3] = { 0xFF, 0x00, 0x00 }; // red pixel
    int result;

    // Use a temporary unique path
    char path_template[] = "/tmp/pngwrite_success_testXXXXXX.png";
    int fd = mkstemp(path_template);
    if (fd != -1) close(fd);

    result = png_image_write_to_file(reinterpret_cast<png_imagep>(&image),
                                     path_template,
                                     0, // do not force 8-bit conversion (keep 8-bit as is)
                                     buffer,
                                     3,  // row_stride (bytes per row)
                                     nullptr);

    EXPECT_TRUE(result == 0, "png_image_write_to_file should succeed for a minimal valid 1x1 RGB image");

    // After a successful write, the code should remove the file (cleanup). The file should no longer exist.
    if (path_exists(path_template)) {
        // If still exists, attempt to unlink and report if necessary
        unlink(path_template);
        EXPECT_TRUE(false, "Expected file to be removed after successful write, but it still exists");
    }
}

// Simple runner
static void run_all_tests() {
    test_png_write_null_image_returns_zero();
    test_png_write_wrong_version_returns_error();
    test_png_write_null_filename_returns_error();
    test_png_write_null_buffer_returns_error();
    test_png_write_fopen_failure_returns_error();
    test_png_write_success_path();
}

int main() {
    run_all_tests();

    // Summary
    fprintf(stderr, "\nTest Summary: total=%d, failed=%d\n", tests_run, tests_failed);
    if (tests_failed == 0) {
        fprintf(stderr, "ALL TESTS PASSED\n");
    } else {
        fprintf(stderr, "SOME TESTS FAILED\n");
    }
    return (tests_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}