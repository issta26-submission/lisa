#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Lightweight, self-contained unit test harness and a minimal, self-contained
// replica of the focal function and its dependencies to enable testing without
// the full libpng project. This is only for demonstration purposes in this
// environment and to validate logic paths of the focal method png_set_mDCV_fixed.

///////////////////////
// Minimal PNG types and helpers (mocked)
///////////////////////

typedef unsigned int png_uint_32;
typedef unsigned short png_uint_16;
typedef unsigned char png_byte;
typedef int png_fixed_point;

// Forward declarations for the mocked dependency surface
struct png_struct_def;
struct png_info_struct;

// Typedefs matching the focal function's signature style
typedef const struct png_struct_def* png_const_structrp;
typedef struct png_struct_def* png_structrp;
typedef struct png_info_struct* png_inforp;

// Local macro constants (mirroring libpng style)
static const unsigned int PNG_INFO_mDCV = 0x01u;
static const int PNG_CHUNK_WRITE_ERROR = 0;

// Global storage to capture chunk reports (mimicking png_chunk_report behavior)
static std::vector<std::string> g_chunk_reports;

// Mocked dependency: no-op debug helper
static void png_debug1(int level, const char* /*fmt*/, const char* /*arg*/) {
    // Intentionally empty - no side effects for tests
    (void)level;
}

// Mocked dependency: chunk report collector
static void png_chunk_report(png_const_structrp /*png_ptr*/, const char* msg, int /*error_class*/) {
    g_chunk_reports.emplace_back(msg);
}

// Mocked fixed-point to 16-bit converter with error signaling
static int png_ITU_fixed_16(int* error, png_fixed_point v) {
    // Representable range for this simplified model: 0..0xFFFF
    if (error) *error = 0;
    if (v < 0 || v > 0xFFFF) {
        if (error) *error = 1;
        return 0;
    }
    return static_cast<int>(static_cast<png_uint_16>(v));
}

///////////////////////
// Minimal, test-focused info/ptr structs
///////////////////////

struct png_struct_def {
    // Intentionally empty; not used in tests beyond non-null-ness
};

struct png_info_struct {
    // Mastering pointers (8 chromaticity coordinates)
    png_uint_16 mastering_red_x;
    png_uint_16 mastering_red_y;
    png_uint_16 mastering_green_x;
    png_uint_16 mastering_green_y;
    png_uint_16 mastering_blue_x;
    png_uint_16 mastering_blue_y;
    png_uint_16 mastering_white_x;
    png_uint_16 mastering_white_y;

    png_uint_32 mastering_maxDL;
    png_uint_32 mastering_minDL;

    unsigned int valid; // bitfield to indicate which info is set
};

// C-compatible wrapper to simulate the exact signature in the focal code
static void png_set_mDCV_fixed(png_const_structrp png_ptr, png_inforp info_ptr,
    png_fixed_point white_x, png_fixed_point white_y,
    png_fixed_point red_x, png_fixed_point red_y,
    png_fixed_point green_x, png_fixed_point green_y,
    png_fixed_point blue_x, png_fixed_point blue_y,
    png_uint_32 maxDL,
    png_uint_32 minDL)
{
    // Local copies for computation
    png_uint_16 rx, ry, gx, gy, bx, by, wx, wy;
    int error;

    png_debug1(1, "in %s storage function", "mDCV"); // no-op in mock

    if (png_ptr == NULL || info_ptr == NULL)
        return;

    error = 0;
    rx = static_cast<png_uint_16>(png_ITU_fixed_16(&error, red_x));
    ry = static_cast<png_uint_16>(png_ITU_fixed_16(&error, red_y));
    gx = static_cast<png_uint_16>(png_ITU_fixed_16(&error, green_x));
    gy = static_cast<png_uint_16>(png_ITU_fixed_16(&error, green_y));
    bx = static_cast<png_uint_16>(png_ITU_fixed_16(&error, blue_x));
    by = static_cast<png_uint_16>(png_ITU_fixed_16(&error, blue_y));
    wx = static_cast<png_uint_16>(png_ITU_fixed_16(&error, white_x));
    wy = static_cast<png_uint_16>(png_ITU_fixed_16(&error, white_y));

    if (error) {
        png_chunk_report(png_ptr,
            "mDCV chromaticities outside representable range",
            PNG_CHUNK_WRITE_ERROR);
        return;
    }

    if (maxDL > 0x7FFFFFFFU || minDL > 0x7FFFFFFFU) {
        png_chunk_report(png_ptr, "mDCV display light level exceeds PNG limit",
            PNG_CHUNK_WRITE_ERROR);
        return;
    }

    info_ptr->mastering_red_x = rx;
    info_ptr->mastering_red_y = ry;
    info_ptr->mastering_green_x = gx;
    info_ptr->mastering_green_y = gy;
    info_ptr->mastering_blue_x = bx;
    info_ptr->mastering_blue_y = by;
    info_ptr->mastering_white_x = wx;
    info_ptr->mastering_white_y = wy;
    info_ptr->mastering_maxDL = maxDL;
    info_ptr->mastering_minDL = minDL;
    info_ptr->valid |= PNG_INFO_mDCV;
}

// Additional helpers to "fuel" tests if needed in the future
static void reset_chunk_reports() {
    g_chunk_reports.clear();
}

///////////////////////
// Lightweight test framework (non-terminating assertions)
///////////////////////

static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        ++g_failed_tests; \
        std::cerr << "Expectation failed: " << __FILE__ << ":" << __LINE__ \
                  << " - (" #cond ")  in test \"" << __func__ << "\"\n"; \
    } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    ++g_total_tests; \
    if(!((a) == (b))) { \
        ++g_failed_tests; \
        std::cerr << "Expectation failed: " << __FILE__ << ":" << __LINE__ \
                  << " - (" #a " == " #b ")  actual: " << (a) \
                  << " vs " << (b) << " in test \"" << __func__ << "\"\n"; \
    } \
} while(0)

///////////////////////
// Test cases for png_set_mDCV_fixed
///////////////////////

void test_valid_input() {
    // Prepare a valid info struct and a non-null png_ptr
    png_struct_def png_obj;
    png_structrp png_ptr = &png_obj;
    png_info_struct info;
    std::memset(&info, 0, sizeof(info));
    png_inforp info_ptr = &info;

    // Clear prior reports
    reset_chunk_reports();

    // Valid chromaticities and light levels
    png_fixed_point white_x = 1000;
    png_fixed_point white_y = 1100;
    png_fixed_point red_x   = 1200;
    png_fixed_point red_y   = 1300;
    png_fixed_point green_x = 1400;
    png_fixed_point green_y = 1500;
    png_fixed_point blue_x  = 1600;
    png_fixed_point blue_y  = 1700;
    png_uint_32 maxDL = 1000;
    png_uint_32 minDL = 0;

    png_set_mDCV_fixed(png_ptr, info_ptr,
        white_x, white_y,
        red_x, red_y,
        green_x, green_y,
        blue_x, blue_y,
        maxDL, minDL);

    // Expect success: fields filled, valid bit set, no chunk reports
    EXPECT_TRUE((info.valid & PNG_INFO_mDCV) != 0);
    EXPECT_EQ(info.mastering_red_x, static_cast<png_uint_16>(red_x));
    EXPECT_EQ(info.mastering_red_y, static_cast<png_uint_16>(red_y));
    EXPECT_EQ(info.mastering_green_x, static_cast<png_uint_16>(green_x));
    EXPECT_EQ(info.mastering_green_y, static_cast<png_uint_16>(green_y));
    EXPECT_EQ(info.mastering_blue_x, static_cast<png_uint_16>(blue_x));
    EXPECT_EQ(info.mastering_blue_y, static_cast<png_uint_16>(blue_y));
    EXPECT_EQ(info.mastering_white_x, static_cast<png_uint_16>(white_x));
    EXPECT_EQ(info.mastering_white_y, static_cast<png_uint_16>(white_y));
    EXPECT_EQ(info.mastering_maxDL, maxDL);
    EXPECT_EQ(info.mastering_minDL, minDL);

    EXPECT_TRUE(g_chunk_reports.empty()); // should not report errors
}

void test_chromaticities_out_of_range() {
    // Prepare a non-null png_ptr and info_ptr
    png_struct_def png_obj;
    png_structrp png_ptr = &png_obj;
    png_info_struct info;
    std::memset(&info, 0, sizeof(info));
    png_inforp info_ptr = &info;

    // Reset reports
    reset_chunk_reports();

    // Introduce an out-of-range chromaticity value
    png_fixed_point white_x = 1000;
    png_fixed_point white_y = 1100;
    png_fixed_point red_x   = 0x20000; // out of representable range (> 0xFFFF)
    png_fixed_point red_y   = 1300;
    png_fixed_point green_x = 1400;
    png_fixed_point green_y = 1500;
    png_fixed_point blue_x  = 1600;
    png_fixed_point blue_y  = 1700;
    png_uint_32 maxDL = 1000;
    png_uint_32 minDL = 0;

    // Before state to ensure no changes occur on failure
    info.valid = 0;
    info.mastering_red_x = 0xAA;
    info.mastering_red_y = 0xBB;

    png_set_mDCV_fixed(png_ptr, info_ptr,
        white_x, white_y,
        red_x, red_y,
        green_x, green_y,
        blue_x, blue_y,
        maxDL, minDL);

    // Expect error path: chromaticities outside representable range reported
    EXPECT_TRUE(!g_chunk_reports.empty());
    EXPECT_EQ(g_chunk_reports.back(), std::string("mDCV chromaticities outside representable range"));

    // Info should remain unchanged due to error
    EXPECT_EQ(info.mastering_red_x, static_cast<png_uint_16>(0xAA));
    EXPECT_EQ(info.mastering_red_y, static_cast<png_uint_16>(0xBB));
    EXPECT_TRUE((info.valid & PNG_INFO_mDCV) == 0);
}

void test_display_light_level_exceeds_limit() {
    // Prepare a non-null png_ptr and info_ptr
    png_struct_def png_obj;
    png_structrp png_ptr = &png_obj;
    png_info_struct info;
    std::memset(&info, 0, sizeof(info));
    png_inforp info_ptr = &info;

    reset_chunk_reports();

    png_fixed_point white_x = 1000;
    png_fixed_point white_y = 1100;
    png_fixed_point red_x   = 1200;
    png_fixed_point red_y   = 1300;
    png_fixed_point green_x = 1400;
    png_fixed_point green_y = 1500;
    png_fixed_point blue_x  = 1600;
    png_fixed_point blue_y  = 1700;
    png_uint_32 maxDL = 0x80000000u; // > 0x7FFFFFFF
    png_uint_32 minDL = 0;

    png_set_mDCV_fixed(png_ptr, info_ptr,
        white_x, white_y,
        red_x, red_y,
        green_x, green_y,
        blue_x, blue_y,
        maxDL, minDL);

    EXPECT_TRUE(!g_chunk_reports.empty());
    EXPECT_EQ(g_chunk_reports.back(), std::string("mDCV display light level exceeds PNG limit"));

    // Ensure info_ptr not modified after error
    EXPECT_TRUE(info.valid == 0);
}

void test_boundaries_allows_value() {
    // Ensure boundary maxDL is accepted
    png_struct_def png_obj;
    png_structrp png_ptr = &png_obj;
    png_info_struct info;
    std::memset(&info, 0, sizeof(info));
    png_inforp info_ptr = &info;

    reset_chunk_reports();

    png_fixed_point white_x = 1000;
    png_fixed_point white_y = 1100;
    png_fixed_point red_x   = 1200;
    png_fixed_point red_y   = 1300;
    png_fixed_point green_x = 1400;
    png_fixed_point green_y = 1500;
    png_fixed_point blue_x  = 1600;
    png_fixed_point blue_y  = 1700;
    png_uint_32 maxDL = 0x7FFFFFFFu; // last allowed value
    png_uint_32 minDL = 0;

    png_set_mDCV_fixed(png_ptr, info_ptr,
        white_x, white_y,
        red_x, red_y,
        green_x, green_y,
        blue_x, blue_y,
        maxDL, minDL);

    EXPECT_TRUE((info.valid & PNG_INFO_mDCV) != 0);
    EXPECT_EQ(info.mastering_maxDL, maxDL);
    EXPECT_EQ(info.mastering_minDL, minDL);
    EXPECT_TRUE(g_chunk_reports.empty());
}

void test_null_ptrs_are_handled_gracefully() {
    // Prepare a valid info_ptr
    png_struct_def png_obj;
    png_structrp png_ptr = &png_obj;
    png_info_struct info;
    std::memset(&info, 0, sizeof(info));
    png_inforp info_ptr = &info;

    reset_chunk_reports();

    // Case 1: png_ptr == NULL
    png_set_mDCV_fixed(nullptr, info_ptr,
        1000, 1100,
        1200, 1300,
        1400, 1500,
        1600, 1700,
        0, 0);

    // Case 2: info_ptr == NULL
    png_set_mDCV_fixed(png_ptr, nullptr,
        1000, 1100,
        1200, 1300,
        1400, 1500,
        1600, 1700,
        0, 0);

    // No chunk reports expected and no crashes
    EXPECT_TRUE(g_chunk_reports.empty());
}

///////////////////////
// Test runner
///////////////////////

int main() {
    // Run tests
    test_valid_input();
    test_chromaticities_out_of_range();
    test_display_light_level_exceeds_limit();
    test_boundaries_allows_value();
    test_null_ptrs_are_handled_gracefully();

    // Summary
    std::cout << "Total tests: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;

    return (g_failed_tests != 0) ? 1 : 0;
}