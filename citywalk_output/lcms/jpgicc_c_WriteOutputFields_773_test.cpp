/*
Unit test suite for WriteOutputFields in jpgicc.c

Notes:
- The tests are written in C++11, but target the C function WriteOutputFields.
- We rely on the existing libjpeg interface and the iccjpeg.h definitions that the
  focal file uses (e.g., PT_GRAY, PT_RGB, PT_YCbCr, PT_CMYK, PT_Lab, JCS_* constants).
- External C symbols are declared with C linkage to match the C implementation.
- A minimal custom test harness is used (no GoogleTest); tests are executed from main.
- The tests aim to cover true/false branches of predicates and key state mutations
  performed by WriteOutputFields, including:
  - Correct mapping of OutputColorSpace to in_color_space, jpeg_color_space, and components
  - Special behaviors for PT_CMYK (write_JFIF_header)
  - Behavior when jpegQuality >= 100 (force jpeg_space to in_space)
  - Subsampling disablement when jpegQuality >= 70
- Assertions are non-terminating (test continues after failures) and results are summarized.
*/

#include <iccjpeg.h>
#include <cstdio>
#include <jpeglib.h>
#include <iostream>
#include <cstdlib>
#include <utils.h>



extern "C" {
    // Function under test
    void WriteOutputFields(int OutputColorSpace);

    // Global state used by the function under test
    extern jpeg_compress_struct Compressor;
    extern int jpegQuality;
}

/* Simple non-terminating test harness */
static int g_pass = 0;
static int g_fail = 0;

#define TEST_EXPECT_EQ(actual, expected, desc) do { \
    if ((actual) != (expected)) { \
        std::cerr << "FAIL: " << (desc) << " | Expected: " << (expected) \
                  << ", Got: " << (actual) << std::endl; \
        ++g_fail; \
    } else { \
        ++g_pass; \
    } \
} while(0)

#define TEST_EXPECT_TRUE(cond, desc) do { \
    if (!(cond)) { \
        std::cerr << "FAIL: " << (desc) << std::endl; \
        ++g_fail; \
    } else { \
        ++g_pass; \
    } \
} while(0)

/*
Test 1: PT_GRAY
- Expect in_color_space and jpeg_color_space to be JCS_GRAYSCALE
- Expect components and num_components to be 1
*/
static void test_PT_GRAY() {
    std::cout << "Running test_PT_GRAY..." << std::endl;
    jpegQuality = 50; // normal quality
    // Call the function under test
    WriteOutputFields(PT_GRAY);

    TEST_EXPECT_EQ(Compressor.in_color_space, JCS_GRAYSCALE, "PT_GRAY: in_color_space == JCS_GRAYSCALE");
    TEST_EXPECT_EQ(Compressor.jpeg_color_space, JCS_GRAYSCALE, "PT_GRAY: jpeg_color_space == JCS_GRAYSCALE");
    TEST_EXPECT_EQ(Compressor.input_components, 1, "PT_GRAY: input_components == 1");
    TEST_EXPECT_EQ(Compressor.num_components, 1, "PT_GRAY: num_components == 1");
    std::cout << "test_PT_GRAY completed.\n" << std::endl;
}

/*
Test 2: PT_RGB
- Expect in_color_space = JCS_RGB, jpeg_color_space = JCS_YCbCr, components = 3
*/
static void test_PT_RGB() {
    std::cout << "Running test_PT_RGB..." << std::endl;
    jpegQuality = 60; // standard quality
    WriteOutputFields(PT_RGB);

    TEST_EXPECT_EQ(Compressor.in_color_space, JCS_RGB, "PT_RGB: in_color_space == JCS_RGB");
    TEST_EXPECT_EQ(Compressor.jpeg_color_space, JCS_YCbCr, "PT_RGB: jpeg_color_space == JCS_YCbCr");
    TEST_EXPECT_EQ(Compressor.input_components, 3, "PT_RGB: input_components == 3");
    TEST_EXPECT_EQ(Compressor.num_components, 3, "PT_RGB: num_components == 3");
    std::cout << "test_PT_RGB completed.\n" << std::endl;
}

/*
Test 3: PT_YCbCr
- Expect in_color_space = jpeg_space = JCS_YCbCr, components = 3
*/
static void test_PT_YCbCr() {
    std::cout << "Running test_PT_YCbCr..." << std::endl;
    jpegQuality = 55;
    WriteOutputFields(PT_YCbCr);

    TEST_EXPECT_EQ(Compressor.in_color_space, JCS_YCbCr, "PT_YCbCr: in_color_space == JCS_YCbCr");
    TEST_EXPECT_EQ(Compressor.jpeg_color_space, JCS_YCbCr, "PT_YCbCr: jpeg_color_space == JCS_YCbCr");
    TEST_EXPECT_EQ(Compressor.input_components, 3, "PT_YCbCr: input_components == 3");
    TEST_EXPECT_EQ(Compressor.num_components, 3, "PT_YCbCr: num_components == 3");
    std::cout << "test_PT_YCbCr completed.\n" << std::endl;
}

/*
Test 4: PT_CMYK
- Expect in_color_space = JCS_CMYK, jpeg_color_space = JCS_YCCK, components = 4
- Expect write_JFIF_header to be 1
*/
static void test_PT_CMYK() {
    std::cout << "Running test_PT_CMYK..." << std::endl;
    jpegQuality = 64;
    WriteOutputFields(PT_CMYK);

    TEST_EXPECT_EQ(Compressor.in_color_space, JCS_CMYK, "PT_CMYK: in_color_space == JCS_CMYK");
    TEST_EXPECT_EQ(Compressor.jpeg_color_space, JCS_YCCK, "PT_CMYK: jpeg_color_space == JCS_YCCK");
    TEST_EXPECT_EQ(Compressor.input_components, 4, "PT_CMYK: input_components == 4");
    TEST_EXPECT_EQ(Compressor.num_components, 4, "PT_CMYK: num_components == 4");
    TEST_EXPECT_TRUE(Compressor.write_JFIF_header == 1, "PT_CMYK: write_JFIF_header == 1");
    std::cout << "test_PT_CMYK completed.\n" << std::endl;
}

/*
Test 5: PT_Lab
- Expect in_color_space = jpeg_space = JCS_YCbCr (Fake to don't touch)
- components = 3
*/
static void test_PT_Lab() {
    std::cout << "Running test_PT_Lab..." << std::endl;
    jpegQuality = 45;
    WriteOutputFields(PT_Lab);

    TEST_EXPECT_EQ(Compressor.in_color_space, JCS_YCbCr, "PT_Lab: in_color_space == JCS_YCbCr");
    TEST_EXPECT_EQ(Compressor.jpeg_color_space, JCS_YCbCr, "PT_Lab: jpeg_color_space == JCS_YCbCr");
    TEST_EXPECT_EQ(Compressor.input_components, 3, "PT_Lab: input_components == 3");
    TEST_EXPECT_EQ(Compressor.num_components, 3, "PT_Lab: num_components == 3");
    std::cout << "test_PT_Lab completed.\n" << std::endl;
}

/*
Test 6: jpegQuality >= 100 forces jpeg_space to in_space
- Use PT_RGB; expect jpeg_color_space to be JCS_RGB after the call
*/
static void test_jpegQuality100_overrides_space() {
    std::cout << "Running test_jpegQuality100_overrides_space..." << std::endl;
    jpegQuality = 100; // lossless-ish parameter
    WriteOutputFields(PT_RGB);

    // Expect the jpeg_space to be equal to in_space (JCS_RGB)
    TEST_EXPECT_EQ(Compressor.in_color_space, JCS_RGB, "jpegQuality100: in_color_space remains JCS_RGB");
    TEST_EXPECT_EQ(Compressor.jpeg_color_space, JCS_RGB, "jpegQuality100: jpeg_color_space overridden to JCS_RGB");
    std::cout << "test_jpegQuality100_overrides_space completed.\n" << std::endl;
}

/*
Test 7: Subsampling disabled when jpegQuality >= 70
- Use PT_RGB to have 3 components; ensure h_samp_factor and v_samp_factor are set to 1 for all components
*/
static void test_subsampling_disabled_at_high_quality() {
    std::cout << "Running test_subsampling_disabled_at_high_quality..." << std::endl;
    jpegQuality = 75;
    WriteOutputFields(PT_RGB);

    // After  jpeg_set_defaults and possible modification, ensure subsampling factors are 1 for each component
    for (int i = 0; i < Compressor.num_components; ++i) {
        std::stringstream idx;
        idx << "component_" << i;
        TEST_EXPECT_EQ(Compressor.comp_info[i].h_samp_factor, 1, "subsampling: h_samp_factor[" + idx.str() + "]");
        TEST_EXPECT_EQ(Compressor.comp_info[i].v_samp_factor, 1, "subsampling: v_samp_factor[" + idx.str() + "]");
    }
    std::cout << "test_subsampling_disabled_at_high_quality completed.\n" << std::endl;
}

int main() {
    std::cout << "===== Begin WriteOutputFields unit tests (C++ harness) =====" << std::endl;

    // Run tests in a defined order
    test_PT_GRAY();
    test_PT_RGB();
    test_PT_YCbCr();
    test_PT_CMYK();
    test_PT_Lab();
    test_jpegQuality100_overrides_space();
    test_subsampling_disabled_at_high_quality();

    int total = g_pass + g_fail;
    std::cout << "Tests passed: " << g_pass << " / " << total << std::endl;
    if (g_fail > 0) {
        std::cout << "Tests failed: " << g_fail << std::endl;
        return 1;
    }
    return 0;
}