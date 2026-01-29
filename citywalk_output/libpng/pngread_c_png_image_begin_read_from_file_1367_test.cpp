// C++11 test harness for png_image_begin_read_from_file
// This test suite exercises the focal function from libpng's PNG image API.
// It avoids GTest and uses a lightweight assertion framework with non-terminating checks.

#include <vector>
#include <memory>
#include <string>
#include <cstdlib>
#include <pngpriv.h>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <cstring>
#include <png.h>


#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

// Lightweight assertion helpers (non-terminating)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define LOG_TEST_START(name) \
    std::cout << "[TEST] " << (name) << " ..." << std::endl

#define EXPECT_TRUE(cond) \
    do { \
        ++g_total_tests; \
        if (!(cond)) { \
            ++g_failed_tests; \
            std::cout << "  [FAIL] Expect " #cond " at " __FILE__ ":" << __LINE__ << std::endl; \
        } \
    } while(0)

#define EXPECT_FALSE(cond) \
    do { \
        ++g_total_tests; \
        if (cond) { \
            ++g_failed_tests; \
            std::cout << "  [FAIL] Expect " #cond " to be false at " __FILE__ ":" << __LINE__ << std::endl; \
        } \
    } while(0)

#define EXPECT_EQ(a,b) \
    do { \
        ++g_total_tests; \
        if (!((a) == (b))) { \
            ++g_failed_tests; \
            std::cout << "  [FAIL] Expect " #a " == " #b " at " __FILE__ ":" << __LINE__ \
                      << " (got: " << (a) << " vs " << (b) << ")" << std::endl; \
        } \
    } while(0)

#define EXPECT_NEQ(a,b) \
    do { \
        ++g_total_tests; \
        if ((a) == (b)) { \
            ++g_failed_tests; \
            std::cout << "  [FAIL] Expect " #a " != " #b " at " __FILE__ ":" << __LINE__ \
                      << " (got: " << (a) << ")" << std::endl; \
        } \
    } while(0)


// Helper: write a minimal valid 1x1 RGBA PNG to disk using libpng (to obtain a true header)
static bool write_minimal_png(const char* path) {
    FILE* fp = fopen(path, "wb");
    if (!fp) return false;

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) { fclose(fp); return false; }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) { png_destroy_write_struct(&png_ptr, NULL); fclose(fp); return false; }

    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(fp);
        return false;
    }

    png_init_io(png_ptr, fp);

    const int width = 1;
    const int height = 1;
    // RGBA
    png_set_IHDR(png_ptr, info_ptr, width, height, 8,
                 PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png_ptr, info_ptr);

    png_bytep row = (png_bytep)malloc(width * 4);
    // Pixel: red, green, blue, alpha
    row[0] = 255; // R
    row[1] = 0;   // G
    row[2] = 0;   // B
    row[3] = 255; // A
    png_write_row(png_ptr, row);

    free(row);
    png_write_end(png_ptr, NULL);

    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
    return true;
}

// Test 1: True path - valid image, existing file, header read succeeds
static bool test_png_image_begin_read_from_file_valid()
{
    // Create a temporary minimal PNG file
    char tmpname[L_tmpnam];
    std::tmpnam(tmpname);
    std::string path = std::string(tmpname) + ".png";

    if (!write_minimal_png(path.c_str())) {
        std::cout << "  [WARN] Could not create minimal PNG for test_valid. Skipping test." << std::endl;
        return false; // skip as couldn't set up
    }

    png_image image;
    std::memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    int ret = png_image_begin_read_from_file(&image, path.c_str());

    // Cleanup: do not rely on library to close the file here; memory cleanup handled by library when appropriate.
    // Note: We are not calling any additional libpng end/read routines as the test focuses on the entry path.

    // Expect a non-zero return (success path)
    bool passed = (ret != 0);
    if (!passed) {
        std::cout << "  [DETAIL] Expected non-zero return for valid file, got: " << ret << std::endl;
    }
    // Remove created file (best effort)
    std::remove(path.c_str());
    return passed;
}

// Test 2: Path false - file_name is NULL should yield an error (non-zero)
static bool test_png_image_begin_read_from_file_null_filename()
{
    // Prepare a real temporary PNG so image is valid
    char tmpname[L_tmpnam];
    std::tmpnam(tmpname);
    std::string path = std::string(tmpname) + ".png";

    if (!write_minimal_png(path.c_str())) {
        std::cout << "  [WARN] Could not create minimal PNG for test_null_filename. Skipping test." << std::endl;
        return false; // skip
    }

    png_image image;
    std::memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    int ret = png_image_begin_read_from_file(&image, /*nullptr*/ NULL);
    bool passed = (ret != 0);

    // Cleanup
    std::remove(path.c_str());
    // Expect non-zero due to invalid argument
    if (!passed) {
        std::cout << "  [DETAIL] Expected non-zero when file_name is NULL, got: " << ret << std::endl;
    }
    return passed;
}

// Test 3: image is NULL should yield 0 (no operation)
static bool test_png_image_begin_read_from_file_image_null()
{
    // Create a real temporary PNG file to ensure there is a file
    char tmpname[L_tmpnam];
    std::tmpnam(tmpname);
    std::string path = std::string(tmpname) + ".png";

    if (!write_minimal_png(path.c_str())) {
        std::cout << "  [WARN] Could not create minimal PNG for test_null_image. Skipping test." << std::endl;
        return false;
    }

    // Pass NULL image
    int ret = png_image_begin_read_from_file(NULL, path.c_str());
    bool passed = (ret == 0);

    std::remove(path.c_str());
    if (!passed) {
        std::cout << "  [DETAIL] Expected 0 when image is NULL, got: " << ret << std::endl;
    }
    return passed;
}

// Test 4: image version incorrect should yield error (non-zero)
static bool test_png_image_begin_read_from_file_wrong_version()
{
    // Create a real temporary PNG file
    char tmpname[L_tmpnam];
    std::tmpnam(tmpname);
    std::string path = std::string(tmpname) + ".png";

    if (!write_minimal_png(path.c_str())) {
        std::cout << "  [WARN] Could not create minimal PNG for test_wrong_version. Skipping test." << std::endl;
        return false;
    }

    png_image image;
    std::memset(&image, 0, sizeof(image));
    image.version = 0; // incorrect version

    int ret = png_image_begin_read_from_file(&image, path.c_str());
    bool passed = (ret != 0);

    std::remove(path.c_str());
    if (!passed) {
        std::cout << "  [DETAIL] Expected non-zero for incorrect PNG_IMAGE_VERSION, got: " << ret << std::endl;
    }
    return passed;
}

// Test 5: non-existent file should yield error (non-zero)
static bool test_png_image_begin_read_from_file_nonexistent()
{
    // Use a valid image for the test, but point to a path that does not exist
    char tmpname[L_tmpnam];
    std::tmpnam(tmpname);
    std::string path = std::string(tmpname) + "_does_not_exist.png";

    // Ensure file does not exist
    std::remove(path.c_str());

    png_image image;
    std::memset(&image, 0, sizeof(image));
    image.version = PNG_IMAGE_VERSION;

    int ret = png_image_begin_read_from_file(&image, path.c_str());
    bool passed = (ret != 0);

    if (!passed) {
        std::cout << "  [DETAIL] Expected non-zero when file does not exist, got: " << ret << std::endl;
    }
    return passed;
}

// Runner
int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    LOG_TEST_START("png_image_begin_read_from_file - Valid path");
    EXPECT_TRUE(test_png_image_begin_read_from_file_valid());

    LOG_TEST_START("png_image_begin_read_from_file - NULL filename");
    EXPECT_TRUE(test_png_image_begin_read_from_file_null_filename());

    LOG_TEST_START("png_image_begin_read_from_file - NULL image");
    EXPECT_TRUE(test_png_image_begin_read_from_file_image_null());

    LOG_TEST_START("png_image_begin_read_from_file - Wrong version");
    EXPECT_TRUE(test_png_image_begin_read_from_file_wrong_version());

    LOG_TEST_START("png_image_begin_read_from_file - Non-existent file");
    EXPECT_TRUE(test_png_image_begin_read_from_file_nonexistent());

    // Summary
    std::cout << "====================================" << std::endl;
    std::cout << "Total tests: " << g_total_tests
              << "  Passed: " << (g_total_tests - g_failed_tests)
              << "  Failed: " << g_failed_tests << std::endl;
    if (g_failed_tests > 0) {
        std::cout << "Some tests failed. Review the logs above for details." << std::endl;
    } else {
        std::cout << "All tests passed." << std::endl;
    }

    return g_failed_tests ? 1 : 0;
}