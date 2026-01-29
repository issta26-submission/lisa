// C++11 test harness for the focal method do_png2pnm (png2pnm.c) using libpng.
// This test suite creates small PNGs in memory/disk, invokes do_png2pnm with
// various configurations (raw vs ASCII, with/without alpha) and validates
// the generated PNM outputs. It does not rely on GTest; it uses a lightweight
// internal EXPECT-like macros to maximize code coverage while avoiding
// terminating on assertion failures.

// Note: The tested project is expected to be compiled with libpng available.
// The do_png2pnm symbol is assumed to have C linkage, hence the extern "C" usage.

#include <vector>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include <fcntl.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <png.h>


extern "C" {
}

// Basic BOOL alias used by the focal code and PNG utilities
typedef unsigned char BOOL;
#define TRUE ((BOOL)1)
#define FALSE ((BOOL)0)

// Forward declaration of the focal method under test (C linkage)
extern "C" BOOL do_png2pnm (png_struct *png_ptr, png_info *info_ptr,
                 FILE *pnm_file, FILE *alpha_file,
                 BOOL raw, BOOL alpha);

// Lightweight test framework (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << __FUNCTION__ << ": " << (msg) << "\n"; \
        ++g_tests_failed; \
    } \
} while (0)

#define EXPECT_EQ_STR(expected, actual, msg) do { \
    ++g_tests_run; \
    if ((actual) != (expected)) { \
        std::cerr << "[FAIL] " << __FUNCTION__ << ": " << (msg) \
                  << " | Expected: \"" << (expected) << "\""; \
        std::cerr << " | Actual: \"" << (actual) << "\"\n"; \
        ++g_tests_failed; \
    } \
} while (0)

#define EXPECT_EQ_BUF(expected, actual, len, msg) do { \
    ++g_tests_run; \
    if ((actual) == nullptr || (expected) == nullptr) { \
        std::cerr << "[FAIL] " << __FUNCTION__ << ": " << (msg) \
                  << " | Null pointer detected.\n"; \
        ++g_tests_failed; \
    } else if (std::strncmp((const char*)(actual), (const char*)(expected), (len)) != 0) { \
        std::cerr << "[FAIL] " << __FUNCTION__ << ": " << (msg) \
                  << " | Buffer mismatch (len=" << (len) << ").\n"; \
        ++g_tests_failed; \
    } \
} while (0)

static void remove_if_exists(const std::string &path) {
    struct stat st;
    if (stat(path.c_str(), &st) == 0) {
        remove(path.c_str());
    }
}

// Helpers to create simple 1x1 PNGs with libpng (gray and rgba) for the tests.
// These functions are tiny writers tailored for our unit tests.

static void write_png_gray1x1(const char* path, unsigned char value) {
    FILE* f = fopen(path, "wb");
    if (!f) return;
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(f);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return;
    }
    png_init_io(png_ptr, f);
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_GRAY,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_byte row[1];
    row[0] = value;
    png_bytep rows[1];
    rows[0] = row;
    png_set_rows(png_ptr, info_ptr, rows);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(f);
}

static void write_png_rgb1x1(const char* path, unsigned char r, unsigned char g, unsigned char b) {
    FILE* f = fopen(path, "wb");
    if (!f) return;
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(f);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return;
    }
    png_init_io(png_ptr, f);
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_byte row[3];
    row[0] = r; row[1] = g; row[2] = b;
    png_bytep rows[1];
    rows[0] = row;
    png_set_rows(png_ptr, info_ptr, rows);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(f);
}

static void write_png_rgba1x1(const char* path, unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    FILE* f = fopen(path, "wb");
    if (!f) return;
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(f);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return;
    }
    png_init_io(png_ptr, f);
    png_set_IHDR(png_ptr, info_ptr, 1, 1, 8, PNG_COLOR_TYPE_RGBA,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
    png_byte row[4];
    row[0] = r; row[1] = g; row[2] = b; row[3] = a;
    png_bytep rows[1];
    rows[0] = row;
    png_set_rows(png_ptr, info_ptr, rows);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(f);
}

// Helper to read an entire binary file into a string
static std::string read_file_to_string(const std::string &path) {
    std::ifstream ifs(path, std::ios::binary);
    if (!ifs) return "";
    std::string contents((std::istreambuf_iterator<char>(ifs)),
                         (std::istreambuf_iterator<char>()));
    return contents;
}

// Test 1: Gray 1x1 PNG, raw (P5) output, no alpha
static void test_gray1x1_raw_output() {
    const std::string png_path = "test_gray1x1.png";
    const std::string pnm_path = "test_gray1x1.pnm";
    const std::string alpha_path = "test_gray1x1_alpha.pnm"; // unused in this test
    // Prepare input PNG with a single gray pixel value 128
    write_png_gray1x1(png_path.c_str(), 128);

    // Prepare output files
    FILE* pnm_file = fopen(pnm_path.c_str(), "wb");
    FILE* alpha_file = fopen(alpha_path.c_str(), "wb");

    // Reopen libpng structures for reading from disk
    FILE* png_in = fopen(png_path.c_str(), "rb");
    if (!png_in) {
        EXPECT_TRUE(false, "Failed to open generated gray PNG for reading");
        return;
    }

    // Initialize png_ptr / info_ptr for reading
    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(png_in);
        fclose(pnm_file);
        fclose(alpha_file);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        EXPECT_TRUE(false, "libpng read initialization failed");
        return;
    }
    png_init_io(png_ptr, png_in);

    // Call the focal function (raw = TRUE, alpha = FALSE)
    BOOL res = do_png2pnm(png_ptr, info_ptr, pnm_file, alpha_file, TRUE, FALSE);

    // Cleanup
    fclose(png_in);
    fclose(pnm_file);
    fclose(alpha_file);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    // Validate
    std::string actual = read_file_to_string(pnm_path);
    // Expected: P5 header + "1 1" + "255" then 1 byte 128
    std::string expected = "P5\n1 1\n255\n";
    expected.push_back(static_cast<char>(128));

    EXPECT_EQ_STR(expected, actual, "Gray 1x1 raw output mismatch");
    // Cleanup
    remove_if_exists(png_path);
    remove_if_exists(pnm_path);
    remove_if_exists(alpha_path);
}

// Test 2: Gray 1x1 PNG, ASCII (P2) output, no alpha
static void test_gray1x1_ascii_output() {
    const std::string png_path = "test_gray1x1_ascii.png";
    const std::string pnm_path = "test_gray1x1_ascii.pnm";
    const std::string alpha_path = "test_gray1x1_ascii_alpha.pnm";
    write_png_gray1x1(png_path.c_str(), 128);

    FILE* pnm_file = fopen(pnm_path.c_str(), "wb");
    FILE* alpha_file = fopen(alpha_path.c_str(), "wb");

    FILE* png_in = fopen(png_path.c_str(), "rb");
    if (!png_in) {
        EXPECT_TRUE(false, "Failed to open generated gray PNG for reading (ASCII test)");
        return;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(png_in);
        fclose(pnm_file);
        fclose(alpha_file);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        EXPECT_TRUE(false, "libpng read initialization failed (ASCII test)");
        return;
    }
    png_init_io(png_ptr, png_in);

    BOOL res = do_png2pnm(png_ptr, info_ptr, pnm_file, alpha_file, FALSE, FALSE);

    fclose(png_in);
    fclose(pnm_file);
    fclose(alpha_file);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    std::string actual = read_file_to_string(pnm_path);
    std::string expected = "P2\n1 1\n255\n128 \n";

    EXPECT_EQ_STR(expected, actual, "Gray 1x1 ASCII output mismatch");
    remove_if_exists(png_path);
    remove_if_exists(pnm_path);
    remove_if_exists(alpha_path);
}

// Test 3: RGB 1x1 PNG, raw (P6) output
static void test_rgb1x1_raw_output() {
    const std::string png_path = "test_rgb1x1.png";
    const std::string pnm_path = "test_rgb1x1.pnm";
    const std::string alpha_path = "test_rgb1x1_alpha.pnm";
    write_png_rgb1x1(png_path.c_str(), 10, 20, 30);

    FILE* pnm_file = fopen(pnm_path.c_str(), "wb");
    FILE* alpha_file = fopen(alpha_path.c_str(), "wb");

    FILE* png_in = fopen(png_path.c_str(), "rb");
    if (!png_in) {
        EXPECT_TRUE(false, "Failed to open generated RGB PNG for reading");
        return;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(png_in);
        fclose(pnm_file);
        fclose(alpha_file);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        EXPECT_TRUE(false, "libpng read initialization failed (RGB test)");
        return;
    }
    png_init_io(png_ptr, png_in);

    BOOL res = do_png2pnm(png_ptr, info_ptr, pnm_file, alpha_file, TRUE, FALSE);

    fclose(png_in);
    fclose(pnm_file);
    fclose(alpha_file);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    std::string actual = read_file_to_string(pnm_path);
    std::string expected = "P6\n1 1\n255\n";
    expected.push_back(static_cast<char>(10)); // newline after header; not part of pixel data but header ends with 10
    // The actual binary header is "P6\n1 1\n255\n" - already included
    // Append expected data (RGB)
    expected = "P6\n1 1\n255\n";
    expected.push_back(static_cast<char>(10)); // ensure compile: but we'll override with exact header+bytes
    // For exact comparison, we construct precisely:
    expected = "P6\n1 1\n255\n";
    expected.push_back(static_cast<char>(10)); // Unused in final; but we will override below
    // Simpler: build exact expected with bytes
    expected = "P6\n1 1\n255\n";
    expected.push_back(static_cast<char>(10)); // this will create an extra newline; we must not add this
    // Correction: We'll construct exact expected properly:
    expected = "P6\n1 1\n255\n";
    // Then append RGB data 10,20,30
    expected.append(1, static_cast<char>(10));
    expected.append(1, static_cast<char>(20));
    expected.append(1, static_cast<char>(30));
    // However, the above approach is wrong: PNM raw data immediately follows header and is 3 bytes, not separated by newline.
    // We'll instead rebuild correctly:
    expected = "P6\n1 1\n255\n";
    expected.push_back(static_cast<char>(10)); // This line is incorrect. We need no extra newline before data for binary format.
    // Realistic approach: we'll construct expected directly as header bytes + data bytes.
    std::string exact_expected_header = "P6\n1 1\n255\n";
    std::string exact_expected = exact_expected_header;
    exact_expected.push_back(static_cast<char>(10)); // This would be extra; but for 1x1 RGB, data are 3 bytes: 10,20,30
    // We'll instead set actual verification differently: compare header portion and pixel data separately.
    // We'll fix the test logic below and not rely on the ad-hoc string assembly here.

    // Instead, perform robust checks:
    // 1) The header is exactly "P6\n1 1\n255\n" (first 9 chars for "P6\n1 1\n255\n" length=9 including trailing newline)
    // 2) The next 3 bytes are 10,20,30.

    // Re-read actual file content to inspect header and data explicitly
    std::string actual_full = actual;
    // To perform a precise check, ensure at least header is present
    const std::string header = "P6\n1 1\n255\n";
    EXPECT_TRUE(actual_full.substr(0, header.size()) == header, "RGB test header mismatch");
    // Pixel data
    const unsigned char exp_r = 10, exp_g = 20, exp_b = 30;
    if (actual_full.size() >= header.size() + 3) {
        unsigned char got_r = static_cast<unsigned char>(actual_full[header.size()]);
        unsigned char got_g = static_cast<unsigned char>(actual_full[header.size() + 1]);
        unsigned char got_b = static_cast<unsigned char>(actual_full[header.size() + 2]);
        if (got_r != exp_r || got_g != exp_g || got_b != exp_b) {
            std::cerr << "[FAIL] " << __FUNCTION__ << ": RGB pixel data mismatch\n";
            ++g_tests_failed;
        } else {
            EXPECT_TRUE(true, "RGB pixel data matches");
        }
    } else {
        std::cerr << "[FAIL] " << __FUNCTION__ << ": RGB output data too short\n";
        ++g_tests_failed;
    }

    // Cleanup
    remove_if_exists(png_path);
    remove_if_exists(pnm_path);
    remove_if_exists(alpha_path);
}

// Test 4: RGBA 1x1 PNG, raw (P6) output with alpha file populated
static void test_rgba1x1_raw_with_alpha() {
    const std::string png_path = "test_rgba1x1.png";
    const std::string pnm_path = "test_rgba1x1.pnm";
    const std::string alpha_path = "test_rgba1x1.alpha.pnm";
    write_png_rgba1x1(png_path.c_str(), 1, 2, 3, 4);

    FILE* pnm_file = fopen(pnm_path.c_str(), "wb");
    FILE* alpha_file = fopen(alpha_path.c_str(), "wb");

    FILE* png_in = fopen(png_path.c_str(), "rb");
    if (!png_in) {
        EXPECT_TRUE(false, "Failed to open generated RGBA PNG for reading");
        return;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(png_in);
        fclose(pnm_file);
        fclose(alpha_file);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        EXPECT_TRUE(false, "libpng read initialization failed (RGBA test)");
        return;
    }
    png_init_io(png_ptr, png_in);

    BOOL res = do_png2pnm(png_ptr, info_ptr, pnm_file, alpha_file, TRUE, TRUE);

    fclose(png_in);
    fclose(pnm_file);
    fclose(alpha_file);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    // Validate pnm_file (RGB) header and data
    std::string pnm_full = read_file_to_string(pnm_path);
    std::string header = "P6\n1 1\n255\n";
    if (pnm_full.size() < header.size() + 3) {
        std::cerr << "[FAIL] RGBA test: insufficient pnm data\n";
        ++g_tests_failed;
    } else {
        EXPECT_TRUE(pnm_full.substr(0, header.size()) == header, "RGBA test header mismatch for pnm_file");
        unsigned char r = static_cast<unsigned char>(pnm_full[header.size()]);
        unsigned char g = static_cast<unsigned char>(pnm_full[header.size() + 1]);
        unsigned char b = static_cast<unsigned char>(pnm_full[header.size() + 2]);
        if (r != 1 || g != 2 || b != 3) {
            std::cerr << "[FAIL] RGBA test: pnm pixel data mismatch: got "
                      << int(r) << "," << int(g) << "," << int(b)
                      << " expected 1,2,3\n";
            ++g_tests_failed;
        } else {
            EXPECT_TRUE(true, "RGBA pnm_file pixel data matches");
        }
    }
    // Validate alpha_file
    std::string alpha_full = read_file_to_string(alpha_path);
    std::string alpha_header = "P5\n1 1\n255\n";
    if (alpha_full.size() < alpha_header.size() + 1) {
        std::cerr << "[FAIL] RGBA test: insufficient alpha data\n";
        ++g_tests_failed;
    } else {
        EXPECT_TRUE(alpha_full.substr(0, alpha_header.size()) == alpha_header, "RGBA test: alpha header mismatch");
        unsigned char a = static_cast<unsigned char>(alpha_full[alpha_header.size()]);
        if (a != 4) {
            std::cerr << "[FAIL] RGBA test: alpha data mismatch: got " << int(a) << " expected 4\n";
            ++g_tests_failed;
        } else {
            EXPECT_TRUE(true, "RGBA alpha data matches");
        }
    }

    // Cleanup
    remove_if_exists(png_path);
    remove_if_exists(pnm_path);
    remove_if_exists(alpha_path);
}

// Test 5: alpha requested but PNG has no alpha; expect FALSE
static void test_alpha_requested_but_no_alpha() {
    const std::string png_path = "test_gray1x1_noalpha.png";
    write_png_gray1x1(png_path.c_str(), 200);

    FILE* pnm_file = fopen("tmp_should_not_write.pnm", "wb");
    FILE* alpha_file = fopen("tmp_should_not_write.alpha", "wb");

    FILE* png_in = fopen(png_path.c_str(), "rb");
    if (!png_in) {
        EXPECT_TRUE(false, "Failed to open PNG for reading (alpha mismatch test)");
        return;
    }

    png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(png_in);
        fclose(pnm_file);
        fclose(alpha_file);
        png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
        EXPECT_TRUE(false, "libpng read initialization failed (alpha mismatch test)");
        return;
    }
    png_init_io(png_ptr, png_in);

    BOOL res = do_png2pnm(png_ptr, info_ptr, pnm_file, alpha_file, TRUE, TRUE);

    fclose(png_in);
    fclose(pnm_file);
    fclose(alpha_file);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    // Expect FALSE
    EXPECT_TRUE(res == FALSE, "Alpha requested with no alpha present should fail");

    // Cleanup
    remove_if_exists(png_path);
    remove_if_exists("tmp_should_not_write.pnm");
    remove_if_exists("tmp_should_not_write.alpha");
}

int main() {
    std::cout << "Starting do_png2pnm unit tests (C++11, no GTest)..." << std::endl;

    test_gray1x1_raw_output();
    test_gray1x1_ascii_output();
    test_rgb1x1_raw_output();
    test_rgba1x1_raw_with_alpha();
    test_alpha_requested_but_no_alpha();

    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << std::endl;
    if (g_tests_failed == 0) {
        std::cout << "All tests passed." << std::endl;
        return 0;
    } else {
        std::cout << "Some tests failed." << std::endl;
        return 1;
    }
}