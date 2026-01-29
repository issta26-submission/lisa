// High-quality C++11 unit test suite for the focal method: unsupported_gamma
// This test suite is designed to be self-contained (no GTest) and uses a
// lightweight, non-terminating assertion framework to maximize code coverage.
// The tests reimplement the minimal pieces of the dependent PNG structures and
// helper functions to allow compiling and testing in isolation.

#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// -----------------------------------------------------------------------------
// Minimal stubs to emulate the libpng environment used by unsupported_gamma
// These are defined solely for tests and do not require the real libpng.
// -----------------------------------------------------------------------------

// Forward declare opaque pointer type used by the focal method
struct png_struct;
typedef png_struct* png_structrp;
typedef int png_fixed_point;

// Gamma bounds used by the focal method.
// These mimic the behavior of the real constants used in the project.
static const png_fixed_point PNG_LIB_GAMMA_MIN = 1;
static const png_fixed_point PNG_LIB_GAMMA_MAX = 100000;

// Simple struct to track warnings/errors and messages for test assertions
struct png_struct {
    int warnings;
    int errors;
    const char* last_warning_message;
    const char* last_error_message;
};

// Mock implementations of the error/warning handlers referenced by unsupported_gamma
void png_app_warning(png_structrp png_ptr, const char* msg)
{
    if (png_ptr) {
        ++png_ptr->warnings;
        png_ptr->last_warning_message = msg;
    }
}
void png_app_error(png_structrp png_ptr, const char* msg)
{
    if (png_ptr) {
        ++png_ptr->errors;
        png_ptr->last_error_message = msg;
    }
}

// -----------------------------------------------------------------------------
// Focal method under test: unsupported_gamma
// This implementation mirrors the provided source, including the exact
// control-flow and message text used by the tests.
// -----------------------------------------------------------------------------
int unsupported_gamma(png_structrp png_ptr, png_fixed_point gamma, int warn)
{
{
   /* Validate a gamma value to ensure it is in a reasonable range.  The value
    * is expected to be 1 or greater, but this range test allows for some
    * viewing correction values.  The intent is to weed out the API users
    * who might use the inverse of the gamma value accidentally!
    *
    * 1.6.47: apply the test in png_set_gamma as well but only warn and return
    * false if it fires.
    *
    * TODO: 1.8: make this an app_error in png_set_gamma as well.
    */
   if (gamma < PNG_LIB_GAMMA_MIN || gamma > PNG_LIB_GAMMA_MAX)
   {
#     define msg "gamma out of supported range"
      if (warn)
         png_app_warning(png_ptr, msg);
      else
         png_app_error(png_ptr, msg);
      return 1;
#     undef msg
   }
   return 0;
}
}

// -----------------------------------------------------------------------------
// Lightweight test harness (non-terminating assertions)
// This harness provides EXPECT_ macros that report failures but continue
// execution, to maximize coverage during testing.
// -----------------------------------------------------------------------------

static int g_test_failures = 0;

#define EXPECT_TRUE(cond, description) \
    do { \
        if(!(cond)) { \
            std::cerr << "EXPECT_TRUE FAILED: " << (description) \
                      << " (in " << __FILE__ << ":" << __LINE__ << ")\n"; \
            ++g_test_failures; \
        } \
    } while(false)

#define EXPECT_EQ_INT(a, b, description) \
    do { \
        if((a) != (b)) { \
            std::cerr << "EXPECT_EQ INT FAILED: " << (a) << " != " << (b) \
                      << " (" << (description) << ") in " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++g_test_failures; \
        } \
    } while(false)

#define EXPECT_STR_EQ(a, b, description) \
    do { \
        if(std::strcmp((a), (b)) != 0) { \
            std::cerr << "EXPECT_STR_EQ FAILED: \"" << (a) << "\" != \"" << (b) \
                      << "\" (" << (description) << ") in " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++g_test_failures; \
        } \
    } while(false)

#define EXPECT_PTR_NULL(p, description) \
    do { \
        if((p) != nullptr) { \
            std::cerr << "EXPECT_PTR_NULL FAILED: Pointer is not null (" \
                      << (description) << ") in " << __FILE__ << ":" << __LINE__ << "\n"; \
            ++g_test_failures; \
        } \
    } while(false)

// -----------------------------------------------------------------------------
// Test cases for unsupported_gamma
// Each test is annotated with comments describing its purpose and expected path.
// -----------------------------------------------------------------------------

// Test: gamma exactly at the lower bound, within range
// Expect: return 0, no warnings or errors
void test_gamma_within_range_at_min()
{
    png_struct s = {0, 0, nullptr, nullptr};
    png_structrp p = &s;

    int ret = unsupported_gamma(p, PNG_LIB_GAMMA_MIN, 1);

    EXPECT_EQ_INT(ret, 0, "gamma at min should be within range and return 0");
    EXPECT_TRUE(s.warnings == 0, "no warnings expected for in-range gamma");
    EXPECT_TRUE(s.errors == 0, "no errors expected for in-range gamma");
    EXPECT_PTR_NULL(s.last_warning_message, "no warning message should be set");
    EXPECT_PTR_NULL(s.last_error_message, "no error message should be set");
}

// Test: gamma just below the minimum with warn enabled
// Expect: return 1, one warning issued, no error
void test_gamma_below_min_warn()
{
    png_struct s = {0, 0, nullptr, nullptr};
    png_structrp p = &s;

    int ret = unsupported_gamma(p, PNG_LIB_GAMMA_MIN - 1, 1);

    EXPECT_EQ_INT(ret, 1, "gamma below min with warn should return 1");
    EXPECT_TRUE(s.warnings == 1, "one warning should be recorded");
    EXPECT_TRUE(s.errors == 0, "no errors should be recorded");
    EXPECT_STR_EQ(s.last_warning_message, "gamma out of supported range",
                  "warning message should indicate gamma range issue");
}

// Test: gamma just below the minimum with warn disabled
// Expect: return 1, one error issued, no warning
void test_gamma_below_min_error()
{
    png_struct s = {0, 0, nullptr, nullptr};
    png_structrp p = &s;

    int ret = unsupported_gamma(p, PNG_LIB_GAMMA_MIN - 1, 0);

    EXPECT_EQ_INT(ret, 1, "gamma below min without warn should return 1");
    EXPECT_TRUE(s.warnings == 0, "no warnings should be recorded");
    EXPECT_TRUE(s.errors == 1, "one error should be recorded");
    EXPECT_STR_EQ(s.last_error_message, "gamma out of supported range",
                  "error message should indicate gamma range issue");
}

// Test: gamma just above the maximum with warn enabled
// Expect: return 1, one warning issued, no error
void test_gamma_above_max_warn()
{
    png_struct s = {0, 0, nullptr, nullptr};
    png_structrp p = &s;

    int ret = unsupported_gamma(p, PNG_LIB_GAMMA_MAX + 1, 1);

    EXPECT_EQ_INT(ret, 1, "gamma above max with warn should return 1");
    EXPECT_TRUE(s.warnings == 1, "one warning should be recorded");
    EXPECT_TRUE(s.errors == 0, "no errors should be recorded");
    EXPECT_STR_EQ(s.last_warning_message, "gamma out of supported range",
                  "warning message should indicate gamma range issue");
}

// Test: gamma just above the maximum with warn disabled
// Expect: return 1, one error issued, no warning
void test_gamma_above_max_error()
{
    png_struct s = {0, 0, nullptr, nullptr};
    png_structrp p = &s;

    int ret = unsupported_gamma(p, PNG_LIB_GAMMA_MAX + 1, 0);

    EXPECT_EQ_INT(ret, 1, "gamma above max without warn should return 1");
    EXPECT_TRUE(s.warnings == 0, "no warnings should be recorded");
    EXPECT_TRUE(s.errors == 1, "one error should be recorded");
    EXPECT_STR_EQ(s.last_error_message, "gamma out of supported range",
                  "error message should indicate gamma range issue");
}

// -----------------------------------------------------------------------------
// Main test runner
// -----------------------------------------------------------------------------

int main()
{
    // Run the test cases
    test_gamma_within_range_at_min();
    test_gamma_below_min_warn();
    test_gamma_below_min_error();
    test_gamma_above_max_warn();
    test_gamma_above_max_error();

    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_test_failures << " test(s) FAILED\n";
        return 1;
    }
}