/*
Unit test suite for the focal function:
png_get_bKGD(png_const_structrp png_ptr, png_inforp info_ptr, png_color_16p *background)

Notes:
- The test suite is written in plain C++11 (no Google Test, no external testing framework).
- A minimal, self-contained mock of the needed libpng-like types and constants is provided
  to compile and exercise the focal function in isolation.
- All tests use non-terminating, lightweight assertions that accumulate failures and report
  a final summary in main().
- The tests cover both true and false branches of the condition checks in png_get_bKGD:
  - Non-null pointers
  - Valid flag PNG_INFO_bKGD set in info_ptr->valid
  - Non-null background parameter
- Comments accompany each test explaining intent and what is exercised.

To run:
Compile this as a C++11 program and execute. It will print test results and a final summary.
*/

// Minimal, self-contained mock/types to allow unit-testing without external dependencies.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>


// -----------------------------------------------------------------------------
// Mocked types and constants to mirror the subset used by png_get_bKGD
// -----------------------------------------------------------------------------

// Define a subset of the libpng color structure used by png_get_bKGD.
typedef struct png_color_16 {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint16_t gray;
    uint16_t index;
} png_color_16;

// Pointers/types mirroring libpng internal typedefs
typedef png_color_16* png_color_16p;
struct png_struct_def { };      // opaque placeholder for png_ptr
struct png_info_def {
    uint32_t valid;               // bitmask of valid info chunks
    png_color_16 background;        // background color chunk (bKGD)
};

// Typedefs to match function signature in focal code
typedef struct png_struct_def* png_ptr;
typedef struct png_info_def* png_inforp;
typedef const struct png_struct_def* png_const_structrp;

// PNG_INFO_bKGD flag (bitmask); chosen value is arbitrary but consistent
#define PNG_INFO_bKGD 0x01

// -----------------------------------------------------------------------------
// Minimal helper(s) to mimic private debugging behavior in the focal code
// -----------------------------------------------------------------------------

// Stub for the internal debugging function used in the focal method.
// It is a no-op here to keep the test self-contained.
static void png_debug1(int /*level*/, const char* /*format*/, const char* /*arg*/) {
    // no-op
}

// -----------------------------------------------------------------------------
// The focal function under test (reproduced locally for testability)
// Matches the exact logic shown in the prompt.
// -----------------------------------------------------------------------------
int png_get_bKGD(png_const_structrp png_ptr, png_inforp info_ptr, png_color_16p *background)
{
{
   png_debug1(1, "in %s retrieval function", "bKGD");
   if (png_ptr != NULL && info_ptr != NULL &&
       (info_ptr->valid & PNG_INFO_bKGD) != 0 &&
       background != NULL)
   {
      *background = &(info_ptr->background);
      return PNG_INFO_bKGD;
   }
   return 0;
}
}

// -----------------------------------------------------------------------------
// Lightweight test framework (non-terminating assertions)
// -----------------------------------------------------------------------------

static int g_test_failures = 0;
static int g_tests_run = 0;

#define EXPECT_TRUE(cond) \
    do { \
        if(!(cond)) { \
            std::cerr << "EXPECT_TRUE failed: " #cond \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            ++g_test_failures; \
        } \
        ++g_tests_run; \
    } while(0)

#define EXPECT_EQ(expected, actual) \
    do { \
        if((expected) != (actual)) { \
            std::cerr << "EXPECT_EQ failed: expected " << (expected) \
                      << ", got " << (actual) \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            ++g_test_failures; \
        } \
        ++g_tests_run; \
    } while(0)

#define EXPECT_PTR_EQ(expected_ptr, actual_ptr) \
    do { \
        if((expected_ptr) != (actual_ptr)) { \
            std::cerr << "EXPECT_PTR_EQ failed: expected pointer " \
                      << expected_ptr << ", got " << actual_ptr \
                      << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            ++g_test_failures; \
        } \
        ++g_tests_run; \
    } while(0)

#define TEST_CASE(name) void test_##name()

#define RUN_TEST(name) test_##name();

// -----------------------------------------------------------------------------
// Test cases for png_get_bKGD
// -----------------------------------------------------------------------------

// Test 1: True branch - all conditions satisfied: non-null png_ptr, non-null info_ptr,
// info_ptr->valid has PNG_INFO_bKGD, and non-null background pointer.
// Expect return value PNG_INFO_bKGD and background points to info_ptr->background.
TEST_CASE(png_get_bKGD_true_branch)
{
    // Arrange
    png_ptr p = new png_struct_def();            // non-null png_ptr
    png_inforp info = new png_info_def();        // non-null info_ptr
    info->valid = PNG_INFO_bKGD;
    info->background.red = 10;
    info->background.green = 20;
    info->background.blue = 30;
    info->background.gray = 0;
    info->background.index = 0;

    png_color_16p bg_out = nullptr;

    // Act
    int result = png_get_bKGD((png_const_structrp)p, info, &bg_out);

    // Assert
    EXPECT_EQ(PNG_INFO_bKGD, result);
    EXPECT_PTR_EQ(&(info->background), bg_out);

    // Cleanup
    delete info;
    delete p;
}

// Test 2: Null png_ptr should trigger false branch (return 0)
TEST_CASE(png_get_bKGD_null_png_ptr)
{
    // Arrange
    png_ptr p = nullptr;                          // null png_ptr
    png_inforp info = new png_info_def();
    info->valid = PNG_INFO_bKGD;
    info->background.red = 1;

    png_color_16p bg_out = nullptr;

    // Act
    int result = png_get_bKGD((png_const_structrp)p, info, &bg_out);

    // Assert
    EXPECT_EQ(0, result);
    // background pointer should remain unchanged (still nullptr)
    EXPECT_PTR_EQ(nullptr, bg_out);

    // Cleanup
    delete info;
}

// Test 3: Null info_ptr should trigger false branch (return 0)
TEST_CASE(png_get_bKGD_null_info_ptr)
{
    // Arrange
    png_ptr p = new png_struct_def();
    png_inforp info = nullptr;                     // null info_ptr
    png_color_16p bg_out = nullptr;

    // Act
    int result = png_get_bKGD((png_const_structrp)p, info, &bg_out);

    // Assert
    EXPECT_EQ(0, result);
    EXPECT_PTR_EQ(nullptr, bg_out);

    // Cleanup
    delete p;
}

// Test 4: info_ptr valid flag not set for bKGD -> false branch (return 0)
TEST_CASE(png_get_bKGD_invalid_flag)
{
    // Arrange
    png_ptr p = new png_struct_def();
    png_inforp info = new png_info_def();
    info->valid = 0;                               // PNG_INFO_bKGD not set
    info->background.red = 123;

    png_color_16p bg_out = nullptr;

    // Act
    int result = png_get_bKGD((png_const_structrp)p, info, &bg_out);

    // Assert
    EXPECT_EQ(0, result);
    EXPECT_PTR_EQ(nullptr, bg_out);

    // Cleanup
    delete info;
    delete p;
}

// Test 5: background parameter is NULL -> false branch (return 0)
TEST_CASE(png_get_bKGD_null_background_ptr)
{
    // Arrange
    png_ptr p = new png_struct_def();
    png_inforp info = new png_info_def();
    info->valid = PNG_INFO_bKGD;
    info->background.red = 55;

    // Act
    int result = png_get_bKGD((png_const_structrp)p, info, nullptr);

    // Assert
    EXPECT_EQ(0, result);

    // Cleanup
    delete info;
    delete p;
}

// -----------------------------------------------------------------------------
// Main: run all tests and report a summary
// -----------------------------------------------------------------------------

int main() {
    // Run all tests
    RUN_TEST(png_get_bKGD_true_branch);
    RUN_TEST(png_get_bKGD_null_png_ptr);
    RUN_TEST(png_get_bKGD_null_info_ptr);
    RUN_TEST(png_get_bKGD_invalid_flag);
    RUN_TEST(png_get_bKGD_null_background_ptr);

    // Summary
    std::cout << "Tests run: " << g_tests_run 
              << ", Failures: " << g_test_failures << std::endl;
    // Return non-zero if any test failed
    return g_test_failures == 0 ? 0 : 1;
}