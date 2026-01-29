// Self-contained unit test suite for the focal method equivalent of png_get_pHYs_dpi.
// This test suite is designed for C++11, uses only the C++ standard library and
// a minimal in-file reproduction of the focal function (to avoid external dependencies).
// The tests cover true/false branches, pointer-null handling, conversion logic,
// and edge cases around unit_type == 1 (conversion path).

#include <string>
#include <iostream>
#include <pngpriv.h>


// Namespace to isolate the test-friendly reimplementation of the focal function.
namespace pngdpi_test {

typedef unsigned int png_uint_32;

// Minimal PNG pHYs metadata structure used by the focal function.
// This mirrors the subset of fields accessed by png_get_pHYs_dpi.
struct PNGInfo {
    unsigned int valid;
    unsigned int x_pixels_per_unit;
    unsigned int y_pixels_per_unit;
    unsigned int phys_unit_type;
};

// PNG_INFO_pHYs bit flag (simplified for unit testing).
static const unsigned int PNG_INFO_pHYs = 0x01;

// Local reimplementation of the focal method (as a drop-in for testing).
static png_uint_32 png_get_pHYs_dpi_local(void* /*png_ptr*/,
                                        const PNGInfo* info_ptr,
                                        png_uint_32* res_x,
                                        png_uint_32* res_y,
                                        int* unit_type)
{
    png_uint_32 retval = 0;
    // true branches: only proceed when png_ptr non-null, info_ptr non-null, and
    // info_ptr indicates pHYs data is present.
    if (png_ptr != NULL && info_ptr != NULL && (info_ptr->valid & PNG_INFO_pHYs) != 0)
    {
        if (res_x != NULL)
        {
            *res_x = info_ptr->x_pixels_per_unit;
            retval |= PNG_INFO_pHYs;
        }
        if (res_y != NULL)
        {
            *res_y = info_ptr->y_pixels_per_unit;
            retval |= PNG_INFO_pHYs;
        }
        if (unit_type != NULL)
        {
            *unit_type = (int)info_ptr->phys_unit_type;
            retval |= PNG_INFO_pHYs;
            if (*unit_type == 1)
            {
                if (res_x != NULL) *res_x = (png_uint_32)(*res_x * 0.0254 + 0.50);
                if (res_y != NULL) *res_y = (png_uint_32)(*res_y * 0.0254 + 0.50);
            }
        }
    }
    return retval;
}

} // namespace pngdpi_test

// Lightweight test harness (no GTest/GMock), using only standard library.
namespace test_harness {

using namespace std;
using namespace pngdpi_test;

// Simple test result logger
static void log_result(const string& test_name, bool pass, const string& detail = "") {
    if (pass) {
        cout << "[PASSED] " << test_name << "\n";
    } else {
        cout << "[FAILED] " << test_name;
        if (!detail.empty()) cout << " -- " << detail;
        cout << "\n";
    }
}

// Test 1: Null png_ptr should yield no writes and zero return value.
static bool test_null_png_ptr() {
    PNGInfo info;
    info.valid = PNG_INFO_pHYs;
    info.x_pixels_per_unit = 1000;
    info.y_pixels_per_unit = 2000;
    info.phys_unit_type = 0;

    png_uint_32 rx = 7, ry = 9;
    int ut = 123; // ensure we can detect any spurious modification

    png_uint_32 ret = png_get_pHYs_dpi_local(nullptr, &info, &rx, &ry, &ut);

    bool ok = (ret == 0) && (rx == 7) && (ry == 9) && (ut == 123);
    if (!ok) log_result("test_null_png_ptr", ok,
        "Expected (ret=0, rx=7, ry=9, ut=123) got (ret=" + to_string(ret) +
        ", rx=" + to_string(rx) + ", ry=" + to_string(ry) + ", ut=" + to_string(ut) + ")");
    return ok;
}

// Test 2: Null info_ptr should yield no writes and zero return value.
static bool test_null_info_ptr() {
    png_uint_32 rx = 5, ry = 6;
    int ut = -1;

    png_uint_32 ret = png_get_pHYs_dpi_local((void*)0x1, nullptr, &rx, &ry, &ut);

    bool ok = (ret == 0) && (rx == 5) && (ry == 6) && (ut == -1);
    if (!ok) log_result("test_null_info_ptr", ok,
        "Expected (ret=0, rx=5, ry=6, ut=-1) got (ret=" + to_string(ret) +
        ", rx=" + to_string(rx) + ", ry=" + to_string(ry) + ", ut=" + to_string(ut) + ")");
    return ok;
}

// Test 3: info_ptr valid bit not set should produce no writes and zero return.
static bool test_invalid_pHYs_bit() {
    PNGInfo info;
    info.valid = 0; // bit not set
    info.x_pixels_per_unit = 123;
    info.y_pixels_per_unit = 456;
    info.phys_unit_type = 0;

    png_uint_32 rx = 1, ry = 2;
    int ut = 0;

    png_uint_32 ret = png_get_pHYs_dpi_local((void*)0x1, &info, &rx, &ry, &ut);

    bool ok = (ret == 0) && (rx == 1) && (ry == 2) && (ut == 0);
    if (!ok) log_result("test_invalid_pHYs_bit", ok,
        "Expected (ret=0, rx=1, ry=2, ut=0) got (ret=" + to_string(ret) +
        ", rx=" + to_string(rx) + ", ry=" + to_string(ry) + ", ut=" + to_string(ut) + ")");
    return ok;
}

// Test 4: Basic assignment when unit_type is NULL; ensures ret flag is set and values copied.
static bool test_basic_assignment_unittype_null() {
    PNGInfo info;
    info.valid = PNG_INFO_pHYs;
    info.x_pixels_per_unit = 111;
    info.y_pixels_per_unit = 222;
    info.phys_unit_type = 0;

    png_uint_32 rx = 0, ry = 0;
    int ut = 0;

    png_uint_32 ret = png_get_pHYs_dpi_local((void*)0x1, &info, &rx, &ry, nullptr);

    bool ok = (ret == PNG_INFO_pHYs) && (rx == 111) && (ry == 222) && (ut == 0);
    // ut remains unchanged as unit_type is NULL
    if (!ok) log_result("test_basic_assignment_unittype_null", ok,
        "Expected (ret=PNG_INFO_pHYs, rx=111, ry=222) with ut unchanged; got ret=" +
        to_string(ret) + ", rx=" + to_string(rx) + ", ry=" + to_string(ry) +
        ", ut=" + to_string(ut));
    return ok;
}

// Test 5: unit_type == 1 with full conversion on both res_x and res_y.
static bool test_unit_type_one_conversion_both() {
    PNGInfo info;
    info.valid = PNG_INFO_pHYs;
    info.x_pixels_per_unit = 1000;
    info.y_pixels_per_unit = 2000;
    info.phys_unit_type = 1; // triggers conversion

    png_uint_32 rx = 0, ry = 0;
    int ut = -1;

    png_uint_32 ret = png_get_pHYs_dpi_local((void*)0x1, &info, &rx, &ry, &ut);

    // 1000 * 0.0254 + 0.50 = 25.4 + 0.5 = 25.9 -> 25
    // 2000 * 0.0254 + 0.50 = 50.8 + 0.5 = 51.3 -> 51
    bool ok = (ret == PNG_INFO_pHYs) && (rx == 25) && (ry == 51) && (ut == 1);
    if (!ok) log_result("test_unit_type_one_conversion_both", ok,
        "Expected (ret=PNG_INFO_pHYs, rx=25, ry=51, ut=1); got ret=" + to_string(ret) +
        ", rx=" + to_string(rx) + ", ry=" + to_string(ry) + ", ut=" + to_string(ut));
    return ok;
}

// Test 6: unit_type == 1 but res_x is NULL; ensure conversion still occurs for res_y only.
static bool test_unit_type_one_conversion_resy_only() {
    PNGInfo info;
    info.valid = PNG_INFO_pHYs;
    info.x_pixels_per_unit = 1000; // would convert if non-NULL
    info.y_pixels_per_unit = 2000;
    info.phys_unit_type = 1;

    png_uint_32 rx_unused = 999; // not used
    png_uint_32 ry = 0;
    int ut = -1;

    png_uint_32 ret = png_get_pHYs_dpi_local((void*)0x1, &info, nullptr, &ry, &ut);

    bool ok = (ret == PNG_INFO_pHYs) && (rx_unused == 999) && (ry == 51) && (ut == 1);
    if (!ok) log_result("test_unit_type_one_conversion_resy_only", ok,
        "Expected (ret=PNG_INFO_pHYs, ry=51, ut=1); got ret=" + to_string(ret) +
        ", ry=" + to_string(ry) + ", ut=" + to_string(ut) + ", rx_unused=" + to_string(rx_unused));
    return ok;
}

// Test 7: unit_type != 1 (e.g., 2) should copy values without conversion.
static bool test_unit_type_not_one_no_conversion() {
    PNGInfo info;
    info.valid = PNG_INFO_pHYs;
    info.x_pixels_per_unit = 123;
    info.y_pixels_per_unit = 456;
    info.phys_unit_type = 2; // no conversion

    png_uint_32 rx = 0, ry = 0;
    int ut = -5;

    png_uint_32 ret = png_get_pHYs_dpi_local((void*)0x1, &info, &rx, &ry, &ut);

    bool ok = (ret == PNG_INFO_pHYs) && (rx == 123) && (ry == 456) && (ut == 2);
    if (!ok) log_result("test_unit_type_not_one_no_conversion", ok,
        "Expected (ret=PNG_INFO_pHYs, rx=123, ry=456, ut=2); got ret=" + to_string(ret) +
        ", rx=" + to_string(rx) + ", ry=" + to_string(ry) + ", ut=" + to_string(ut));
    return ok;
}

} // namespace test_harness

int main() {
    using namespace test_harness;

    int total = 0;
    int passed = 0;

    // Run tests
    total++; if (test_harness::test_null_png_ptr()) passed++; // Test 1
    total++; if (test_harness::test_null_info_ptr()) passed++;
    total++; if (test_harness::test_invalid_pHYs_bit()) passed++;
    total++; if (test_harness::test_basic_assignment_unittype_null()) passed++;
    total++; if (test_harness::test_unit_type_one_conversion_both()) passed++;
    total++; if (test_harness::test_unit_type_one_conversion_resy_only()) passed++;
    total++; if (test_harness::test_unit_type_not_one_no_conversion()) passed++;

    std::cout << "\nSummary: " << passed << " / " << total << " tests passed.\n";

    return (passed == total) ? 0 : 1;
}