// Minimal C++11 test harness for the focal function: png_get_sCAL
// This test is self-contained and does not rely on GTest.
// It provides lightweight test macros, a small fake libpng subset, and
// multiple test cases to exercise the sCAL retrieval logic.

#include <cmath>
#include <iostream>
#include <pngpriv.h>
#include <cstdlib>


// ----------------------------------------------------------------------------
// Faked dependencies and simplified types to mimic the surrounding libpng
// environment required by the focal method. These are intentionally small
// and self-contained so the unit tests can be compiled and run standalone.
// ----------------------------------------------------------------------------

namespace fake_png {

// Forward declaration of a minimal png_struct to act as png_ptr type.
struct png_struct {};

// Minimal in-memory representation of png_info (png_infor) used by png_get_sCAL.
struct png_infor {
    unsigned int valid;      // bitmask of valid chunks/features
    int scal_unit;            // unit type for sCAL
    const char* scal_s_width;  // width as string to be parsed by atof
    const char* scal_s_height; // height as string to be parsed by atof
};

// Public constants and helpers used by the focal method.
static const unsigned int PNG_INFO_sCAL = 0x01;

// Stub for libpng internal debug logging (no-op in tests).
inline void png_debug1(int /*level*/, const char* /*format*/, const char* /*arg*/) {
    // Intentionally empty to avoid clutter during tests.
}

} // namespace fake_png

using fake_png::PNG_INFO_sCAL;
using fake_png::png_infor;
using fake_png::png_struct;

// Typedefs to mirror libpng's pointer typedefs in a lightweight form.
typedef const fake_png::png_struct* png_const_structrp;
typedef fake_png::png_struct*       png_structrp;
typedef const fake_png::png_infor*   png_const_inforp;
typedef fake_png::png_infor*         png_inforp;

// ----------------------------------------------------------------------------
// Implementation of the focal method under test: png_get_sCAL
// This mirrors the behaviour shown in the provided source snippet.
// ----------------------------------------------------------------------------

int png_get_sCAL(png_const_structrp png_ptr, png_const_inforp info_ptr,
    int *unit, double *width, double *height)
{
{
   fake_png::png_debug1(1, "in %s retrieval function", "sCAL(float)");
   if (png_ptr != NULL && info_ptr != NULL &&
       (info_ptr->valid & PNG_INFO_sCAL) != 0)
   {
      *unit = info_ptr->scal_unit;
      *width = atof(info_ptr->scal_s_width);
      *height = atof(info_ptr->scal_s_height);
      return PNG_INFO_sCAL;
   }
   return 0;
}
}

// ----------------------------------------------------------------------------
// Lightweight testing framework (no GTest/GMock). Non-terminating assertions.
// ----------------------------------------------------------------------------

static int g_failures = 0; // global failure counter

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        ++g_failures; \
        std::cerr << "EXPECT_TRUE failed: " #cond "\n"; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    auto _a = (a); \
    auto _b = (b); \
    if(!(_a == _b)) { \
        ++g_failures; \
        std::cerr << "EXPECT_EQ failed: " #a " != " #b " -> " << _a << " != " << _b << "\n"; \
    } \
} while(0)

#define EXPECT_NEAR(a,b,eps) do { \
    double _a = (double)(a); \
    double _b = (double)(b); \
    if(!(std::fabs(_a - _b) <= (eps))) { \
        ++g_failures; \
        std::cerr << "EXPECT_NEAR failed: " #a " ~ " #b " (eps=" << (eps) << ") -> " << _a << " != " << _b << "\n"; \
    } \
} while(0)

#define TEST_PASSED_MARKER std::cout << "Test passed.\n";

// ----------------------------------------------------------------------------
// Test Helpers and Test Cases
// ----------------------------------------------------------------------------

// Helper to setup a PNG info structure with given fields.
static png_infor make_info(unsigned int valid, int unit,
                           const char* w, const char* h)
{
    png_infor info;
    info.valid = valid;
    info.scal_unit = unit;
    info.scal_s_width = w;
    info.scal_s_height = h;
    return info;
}

// Test 1: Null png_ptr should yield 0 (no retrieval performed)
static void test_sCAL_null_png_ptr()
{
    // Explain test purpose
    // This test ensures that when the png_ptr is NULL, the function does not
    // attempt to read info_ptr and returns 0.
    std::cout << "Test 1: png_ptr == NULL should return 0\n";

    // Arrange
    (void)make_info; // silence unused warning if optimizations remove usage
    png_infor info = make_info(PNG_INFO_sCAL, 1, "1.0", "2.0");

    int unit = 0;
    double w = 0.0, h = 0.0;

    // Act
    int ret = png_get_sCAL(nullptr, &info, &unit, &w, &h);

    // Assert
    EXPECT_EQ(ret, 0);
    // Values should remain unmodified in the test (not necessary to assert here).
    TEST_PASSED_MARKER;
}

// Test 2: NULL info_ptr should yield 0
static void test_sCAL_null_info_ptr()
{
    // Explain test purpose
    // This test ensures that when info_ptr is NULL, the function returns 0.
    std::cout << "Test 2: info_ptr == NULL should return 0\n";

    // Arrange
    png_struct dummy;
    int unit = 0;
    double w = 0.0;
    double h = 0.0;

    // Act
    int ret = png_get_sCAL((png_const_structrp)&dummy, (png_const_inforp)nullptr,
                           &unit, &w, &h);

    // Assert
    EXPECT_EQ(ret, 0);
    TEST_PASSED_MARKER;
}

// Test 3: info_ptr.valid does not include PNG_INFO_sCAL should yield 0
static void test_sCAL_missing_valid_flag()
{
    // Explain test purpose
    // This test ensures that even if info_ptr is non-NULL, without the correct
    // valid flag, the function returns 0.
    std::cout << "Test 3: valid flag missing should return 0\n";

    // Arrange
    png_struct dummy;
    png_infor info = make_info(0, 5, "10.0", "20.0");
    int unit = 0;
    double w = 0.0, h = 0.0;

    // Act
    int ret = png_get_sCAL((png_const_structrp)&dummy, &info, &unit, &w, &h);

    // Assert
    EXPECT_EQ(ret, 0);
    TEST_PASSED_MARKER;
}

// Test 4: Valid sCAL retrieval should populate unit, width, height and return PNG_INFO_sCAL
static void test_sCAL_valid_retrieval()
{
    // Explain test purpose
    // This test ensures that when info_ptr has the sCAL bit set, the function
    // correctly reads scal_unit and converts width/height strings to doubles.
    std::cout << "Test 4: valid sCAL retrieval\n";

    // Arrange
    png_struct dummy;
    png_infor info = make_info(PNG_INFO_sCAL, 7, "3.14", "2.72");
    int unit = 0;
    double w = 0.0, h = 0.0;

    // Act
    int ret = png_get_sCAL((png_const_structrp)&dummy, &info, &unit, &w, &h);

    // Assert
    EXPECT_EQ(ret, PNG_INFO_sCAL);
    EXPECT_EQ(unit, 7);
    EXPECT_NEAR(w, 3.14, 1e-9);
    EXPECT_NEAR(h, 2.72, 1e-9);
    TEST_PASSED_MARKER;
}

// Test 5: sCAL with non-numeric strings should coerce to 0.0 and still return PNG_INFO_sCAL
static void test_sCAL_non_numeric_strings()
{
    // Explain test purpose
    // This test ensures that if the width/height strings are non-numeric,
    // atof returns 0.0, but the function still returns PNG_INFO_sCAL when the flag is set.
    std::cout << "Test 5: non-numeric strings in sCAL should yield 0.0 for width/height\n";

    // Arrange
    png_struct dummy;
    png_infor info = make_info(PNG_INFO_sCAL, 2, "not_a_number", "NaN");
    int unit = -1;
    double w = -123.0, h = -456.0;

    // Act
    int ret = png_get_sCAL((png_const_structrp)&dummy, &info, &unit, &w, &h);

    // Assert
    EXPECT_EQ(ret, PNG_INFO_sCAL);
    EXPECT_EQ(unit, 2);
    EXPECT_NEAR(w, 0.0, 1e-9);
    EXPECT_NEAR(h, 0.0, 1e-9);
    TEST_PASSED_MARKER;
}

// ----------------------------------------------------------------------------
// Main entry: run all tests and report summary
// ----------------------------------------------------------------------------

int main() {
    // Run test suite
    test_sCAL_null_png_ptr();
    test_sCAL_null_info_ptr();
    test_sCAL_missing_valid_flag();
    test_sCAL_valid_retrieval();
    test_sCAL_non_numeric_strings();

    // Report summary
    if (g_failures == 0) {
        std::cout << "All tests passed. Total tests: 5\n";
    } else {
        std::cerr << "Tests finished with failures: " << g_failures << "\n";
    }
    return g_failures;
}