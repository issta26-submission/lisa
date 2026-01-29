// Minimal C++11 test suite for the focal method: png_get_pixels_per_meter
// This test reproduces the essential logic of the function and provides
// unit tests to cover true/false branches of the key predicates.
// The test is self-contained (no GTest) and uses a lightweight harness with
// non-terminating EXPECT_* assertions.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>


// Domain-specific mock environment to emulate libpng's minimal API used by the focal method.
// We define the necessary constants and types locally to keep the test self-contained.

// Enable the focal feature (simulate PNG_pHYs_SUPPORTED being defined)
#define PNG_pHYs_SUPPORTED 1

// Minimal constants to drive the test scenarios
#define PNG_INFO_pHYs         0x01
#define PNG_RESOLUTION_METER   1

// No-op debug function to mimic libpng's internal logging (kept for compatibility)
static void png_debug(int level, const char* msg) { (void)level; (void)msg; }

// Macro to suppress unused parameter warnings (used in the non-supported branch)
#define PNG_UNUSED(x) (void)(x)

//------------------------------------------------------------------------------
// Focal method under test (copied/embedded here for a self-contained test harness)
//
// Original intent:
//   If pHYs chunk is present (valid & PNG_INFO_pHYs) with unit type = meter and
//   x_pixels_per_unit == y_pixels_per_unit, return that value; otherwise return 0.
//------------------------------------------------------------------------------

struct png_struct { }; // opaque in tests
struct png_info_struct {
    unsigned int valid;
    unsigned int phys_unit_type;
    unsigned int x_pixels_per_unit;
    unsigned int y_pixels_per_unit;
};

// Typedefs to resemble libpng's style
typedef struct png_struct* png_ptr;
typedef const struct png_struct* png_const_structrp;

typedef struct png_info_struct* png_inforp;
typedef const struct png_info_struct* png_const_inforp;

// The focal function as per the provided code snippet
unsigned int png_get_pixels_per_meter(png_const_structrp png_ptr, png_const_inforp info_ptr)
{
#ifdef PNG_pHYs_SUPPORTED
   png_debug(1, "in png_get_pixels_per_meter");
   if (png_ptr != NULL && info_ptr != NULL &&
       (info_ptr->valid & PNG_INFO_pHYs) != 0)
   {
      if (info_ptr->phys_unit_type == PNG_RESOLUTION_METER &&
          info_ptr->x_pixels_per_unit == info_ptr->y_pixels_per_unit)
         return info_ptr->x_pixels_per_unit;
   }
#else
   PNG_UNUSED(png_ptr)
   PNG_UNUSED(info_ptr)
#endif
   return 0;
}

//------------------------------------------------------------------------------
// Lightweight test framework (non-terminating assertions)
//------------------------------------------------------------------------------

// Global failure counter
static int g_test_failures = 0;

// Basic expectations (non-terminating)
#define EXPECT_TRUE(cond) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " #cond " at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_test_failures; \
    } \
} while (0)

#define EXPECT_EQ(a, b) do { \
    if ((a) != (b)) { \
        std::cerr << "EXPECT_EQ failed: " #a " == " #b " is false at " \
                  << __FILE__ << ":" << __LINE__ << "\n"; \
        std::cerr << "  Actual: " << static_cast<unsigned long>(a) \
                  << "  Expected: " << static_cast<unsigned long>(b) << "\n"; \
        ++g_test_failures; \
    } \
} while (0)

// Global test fixtures (to simplify test setup)
static png_struct g_png_struct;
static png_info_struct g_png_info;

//------------------------------------------------------------------------------
// Test cases
// 1) True path: info_ptr has pHYs flag, meter unit, and equal x/y pixels per unit.
// 2) False path: x_pixels_per_unit != y_pixels_per_unit -> should return 0.
// 3) False path: phys_unit_type != PNG_RESOLUTION_METER -> should return 0.
// 4) False path: info_ptr valid flag not including PNG_INFO_pHYs -> should return 0.
// 5) False path: png_ptr is NULL -> should return 0.
// 6) False path: info_ptr is NULL -> should return 0.
//------------------------------------------------------------------------------

static void test_true_path_returns_pixels()
{
    // Setup: valid bit set, unit type meter, and equal x/y pixels per unit
    g_png_info.valid = PNG_INFO_pHYs;
    g_png_info.phys_unit_type = PNG_RESOLUTION_METER;
    g_png_info.x_pixels_per_unit = 300;
    g_png_info.y_pixels_per_unit = 300;

    unsigned int result = png_get_pixels_per_meter(reinterpret_cast<png_const_structrp>(&g_png_struct),
                                                   reinterpret_cast<png_const_inforp>(&g_png_info));
    // Expect to obtain the exact pixel-per-meter value
    EXPECT_EQ(result, 300u);
}

static void test_false_path_x_not_equal()
{
    // Setup: same as true path but x and y differ
    g_png_info.valid = PNG_INFO_pHYs;
    g_png_info.phys_unit_type = PNG_RESOLUTION_METER;
    g_png_info.x_pixels_per_unit = 300;
    g_png_info.y_pixels_per_unit = 250;

    unsigned int result = png_get_pixels_per_meter(reinterpret_cast<png_const_structrp>(&g_png_struct),
                                                   reinterpret_cast<png_const_inforp>(&g_png_info));
    // Expect 0 due to x != y
    EXPECT_EQ(result, 0u);
}

static void test_false_path_phys_unit_not_meter()
{
    // x/y equal, but unit type is not meter
    g_png_info.valid = PNG_INFO_pHYs;
    g_png_info.phys_unit_type = 0; // not PNG_RESOLUTION_METER
    g_png_info.x_pixels_per_unit = 200;
    g_png_info.y_pixels_per_unit = 200;

    unsigned int result = png_get_pixels_per_meter(reinterpret_cast<png_const_structrp>(&g_png_struct),
                                                   reinterpret_cast<png_const_inforp>(&g_png_info));
    // Expect 0 due to unit type mismatch
    EXPECT_EQ(result, 0u);
}

static void test_false_path_invalid_info_flag()
{
    // valid does not include PNG_INFO_pHYs
    g_png_info.valid = 0; // clear flag
    g_png_info.phys_unit_type = PNG_RESOLUTION_METER;
    g_png_info.x_pixels_per_unit = 256;
    g_png_info.y_pixels_per_unit = 256;

    unsigned int result = png_get_pixels_per_meter(reinterpret_cast<png_const_structrp>(&g_png_struct),
                                                   reinterpret_cast<png_const_inforp>(&g_png_info));
    // Expect 0 because the pHYs flag is not set
    EXPECT_EQ(result, 0u);
}

static void test_false_path_null_png_ptr()
{
    // Non-null info_ptr but null png_ptr
    g_png_info.valid = PNG_INFO_pHYs;
    g_png_info.phys_unit_type = PNG_RESOLUTION_METER;
    g_png_info.x_pixels_per_unit = 400;
    g_png_info.y_pixels_per_unit = 400;

    unsigned int result = png_get_pixels_per_meter(nullptr,
                                                   reinterpret_cast<png_const_inforp>(&g_png_info));
    // Expect 0 because png_ptr is NULL
    EXPECT_EQ(result, 0u);
}

static void test_false_path_null_info_ptr()
{
    // Non-null png_ptr but null info_ptr
    unsigned int result = png_get_pixels_per_meter(reinterpret_cast<png_const_structrp>(&g_png_struct),
                                                   nullptr);
    // Expect 0 because info_ptr is NULL
    EXPECT_EQ(result, 0u);
}

// Runner to execute all tests
static void run_all_tests()
{
    test_true_path_returns_pixels();
    test_false_path_x_not_equal();
    test_false_path_phys_unit_not_meter();
    test_false_path_invalid_info_flag();
    test_false_path_null_png_ptr();
    test_false_path_null_info_ptr();
}

//------------------------------------------------------------------------------
// Main entry
int main()
{
    // Run tests and report summary
    run_all_tests();

    if (g_test_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << g_test_failures << " test(s) failed.\n";
        return g_test_failures;
    }
}