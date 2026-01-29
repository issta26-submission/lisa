// Test suite for PutCGATSValues in transicc.c
// This file provides a pure-C++11 test harness (no GTest) with lightweight
// assertions and simple mock implementations for the dependencies used by
// PutCGATSValues. It invokes PutCGATSValues with various inputs and validates
// the produced CGATS field writes via SetCGATSfld and the IT8 data via cmsIT8SetData.
//
// Note: This test assumes the project provides the real transicc.c and its
// headers. We supply C-style externs/mocks to intercept internal calls and
// verify behavior without requiring the full CGATS/IIT8 infrastructure.

#include <cstdint>
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <cmath>
#include <utils.h>


// Typedefs compatible with the focal code
typedef double cmsFloat64Number;
typedef int cmsInt32Number;
typedef bool cmsBool;

// Externs to link with the focal production code (from transicc.c)
extern "C" void PutCGATSValues(const cmsFloat64Number Float[]);
extern "C" void SetCGATSfld(const char* Col, cmsFloat64Number Val);
extern "C" void cmsIT8SetData(void* hIT8out, int patch, const char* field, cmsFloat64Number val);

// Globals used by the focal method (provided by transicc.c / library)
extern "C" void* hIT8out;
extern "C" int CGATSPatch;
extern "C" int OutputColorSpace;
extern "C" int InputColorSpace;

// We'll provide a lightweight mock for cmsChannelsOfColorSpace used by the focal code
extern "C" int cmsChannelsOfColorSpace(int space);

// ---------------------------------------------------------------------------
// Mock storage to capture SetCGATSfld writes and cmsIT8SetData calls
// ---------------------------------------------------------------------------

struct CGATS_FieldWrite {
    std::string Col;
    cmsFloat64Number Val;
};

static std::vector<CGATS_FieldWrite> g_SetCGATSfldCalls;

static struct IT8Write {
    int patch;
    std::string field;
    cmsFloat64Number value;
} static_it8_write;

static std::vector<struct IT8Write> g_IT8SetDataCalls;

// Storage for the fields written by SetCGATSfld
static std::map<std::string, cmsFloat64Number> g_CGATSFields;

// Reset mocks before each test
static void reset_mocks() {
    g_SetCGATSfldCalls.clear();
    g_IT8SetDataCalls.clear();
    g_CGATSFields.clear();
    static_it8_write.patch = 0;
    static_it8_write.field.clear();
    static_it8_write.value = 0.0;
}

// Minimal wrappers matching the production code expectations
extern "C" void SetCGATSfld(const char* Col, cmsFloat64Number Val) {
    if (Col == nullptr) return;
    g_SetCGATSfldCalls.push_back({Col, Val});
    g_CGATSFields[Col] = Val;
}

extern "C" void cmsIT8SetData(void* h, int patch, const char* field, cmsFloat64Number val) {
    (void)h; (void)patch;
    struct IT8Write w;
    w.patch = patch;
    if (field) w.field = field;
    else w.field = "";
    w.value = val;
    g_IT8SetDataCalls.push_back(w);
    static_it8_write = w;
}

// The easiest is to map common signature constants to numeric values.
// We assume the environment uses standard Little CMS signatures.
// Values chosen to align with typical ASCII-quadruple signatures.
enum {
    cmsSigXYZData  = 0x58595A20, // 'XYZ '
    cmsSigLabData   = 0x4C616220, // 'Lab '
    cmsSigRgbData   = 0x52474220, // 'RGB '
    cmsSigGrayData  = 0x47524159, // 'GRAY'
    cmsSigCmykData  = 0x434D594B, // 'CMYK'
    cmsSigCmyData   = 0x434D5920, // 'CMY '
    cmsSig1colorData= 0x31534C52, // '1CLR'
    cmsSig2colorData= 0x32534C52, // '2CLR'
    cmsSig3colorData= 0x33534C52, // '3CLR'
    // We only need 1..3 for tests; others map to default
};

// Implement cmsChannelsOfColorSpace in a minimal, test-friendly way
extern "C" int cmsChannelsOfColorSpace(int space) {
    switch (space) {
        case cmsSigXYZData:   // XYZ
        case cmsSigLabData:    // Lab
        case cmsSigRgbData:    // RGB
            return 3;
        case cmsSigGrayData:   // Gray
            return 1;
        case cmsSigCmykData:   // CMYK
            return 4;
        case cmsSigCmyData:    // CMY
            return 3;
        case cmsSig1colorData:  // 1CLR
        case cmsSig2colorData:  // 2CLR
        case cmsSig3colorData:  // 3CLR
            // For test purposes, treat color group as 3 channels
            return 3;
        default:
            // Unknown space -> assume 2 channels for default case
            return 2;
    }
}

// Helper to access CGATS fields in tests
static cmsFloat64Number cgats_fld_value(const std::string& key) {
    auto it = g_CGATSFields.find(key);
    if (it == g_CGATSFields.end()) return NAN;
    return it->second;
}

// A simple, non-terminating assertion macro
#define ASSERT_TRUE(cond, msg) \
    do { if (!(cond)) { std::cerr << "ASSERT FAILED: " << (msg) << "\n"; g_test_failed = true; } } while (0)
#define ASSERT_EQ(a, b, msg) \
    do { if (!((a) == (b))) { std::cerr << "ASSERT FAILED: " << (msg) << " | " #a " != " #b "\n"; g_test_failed = true; } } while (0)

static bool g_test_failed = false;

// ---------------------------------------------------------------------------
// Test cases
// Each test prints a short header, runs PutCGATSValues with controlled inputs,
// and asserts that the produced CGATS fields and IT8 data are as expected.
// ---------------------------------------------------------------------------

// Utility to print test header
static void print_header(const std::string& name) {
    std::cout << "\n==== Test: " << name << " ====" << std::endl;
}

// Test 1: XYZ encoding path
static void test_PutCGATSValues_XYZEncoding() {
    reset_mocks();
    print_header("XYZData encoding");

    cmsFloat64Number Float[4] = {0.1, 0.2, 0.3, 0.0};
    OutputColorSpace = cmsSigXYZData;

    PutCGATSValues(Float);

    // Verify the SAMPLE_ID is recorded via cmsIT8SetData
    ASSERT_TRUE(!g_IT8SetDataCalls.empty(), "SAMPLE_ID should be set via cmsIT8SetData");
    if (!g_IT8SetDataCalls.empty()) {
        const auto& first = g_IT8SetDataCalls.front();
        ASSERT_EQ(first.field, "SAMPLE_ID", "First IT8 set should be 'SAMPLE_ID'");
        ASSERT_EQ(static_cast<int>(first.patch), CGATSPatch, "Patch should be CGATSPatch");
        ASSERT_EQ(first.value, static_cast<cmsFloat64Number>(CGATSPatch), "SAMPLE_ID value should equal CGATSPatch");
    }

    // Verify XYZ fields written
    ASSERT_TRUE(g_CGATSFields.find("XYZ_X") != g_CGATSFields.end(), "XYZ_X should be written");
    ASSERT_TRUE(g_CGATSFields.find("XYZ_Y") != g_CGATSFields.end(), "XYZ_Y should be written");
    ASSERT_TRUE(g_CGATSFields.find("XYZ_Z") != g_CGATSFields.end(), "XYZ_Z should be written");

    ASSERT_EQ(cgats_fld_value("XYZ_X"), 0.1 * 100.0, "XYZ_X encoding");
    ASSERT_EQ(cgats_fld_value("XYZ_Y"), 0.2 * 100.0, "XYZ_Y encoding");
    ASSERT_EQ(cgats_fld_value("XYZ_Z"), 0.3 * 100.0, "XYZ_Z encoding");
}

// Test 2: Lab encoding path
static void test_PutCGATSValues_LabEncoding() {
    reset_mocks();
    print_header("LabData encoding");

    cmsFloat64Number Float[3] = {0.4, -0.5, 0.6};
    OutputColorSpace = cmsSigLabData;

    PutCGATSValues(Float);

    ASSERT_TRUE(g_CGATSFields.find("LAB_L") != g_CGATSFields.end(), "LAB_L written");
    ASSERT_TRUE(g_CGATSFields.find("LAB_A") != g_CGATSFields.end(), "LAB_A written");
    ASSERT_TRUE(g_CGATSFields.find("LAB_B") != g_CGATSFields.end(), "LAB_B written");

    ASSERT_EQ(cgats_fld_value("LAB_L"), 0.4, "LAB_L value");
    ASSERT_EQ(cgats_fld_value("LAB_A"), -0.5, "LAB_A value");
    ASSERT_EQ(cgats_fld_value("LAB_B"), 0.6, "LAB_B value");
}

// Test 3: RGB encoding path
static void test_PutCGATSValues_RGBEncoding() {
    reset_mocks();
    print_header("RGBData encoding");

    cmsFloat64Number Float[3] = {0.25, 0.5, 0.75};
    OutputColorSpace = cmsSigRgbData;

    PutCGATSValues(Float);

    ASSERT_TRUE(g_CGATSFields.find("RGB_R") != g_CGATSFields.end(), "RGB_R written");
    ASSERT_TRUE(g_CGATSFields.find("RGB_G") != g_CGATSFields.end(), "RGB_G written");
    ASSERT_TRUE(g_CGATSFields.find("RGB_B") != g_CGATSFields.end(), "RGB_B written");

    ASSERT_EQ(cgats_fld_value("RGB_R"), 0.25 * 255.0, "RGB_R value");
    ASSERT_EQ(cgats_fld_value("RGB_G"), 0.5 * 255.0, "RGB_G value");
    ASSERT_EQ(cgats_fld_value("RGB_B"), 0.75 * 255.0, "RGB_B value");
}

// Test 4: Gray encoding path
static void test_PutCGATSValues_GrayEncoding() {
    reset_mocks();
    print_header("GrayData encoding");

    cmsFloat64Number Float[1] = {0.7};
    OutputColorSpace = cmsSigGrayData;

    PutCGATSValues(Float);

    ASSERT_TRUE(g_CGATSFields.find("GRAY") != g_CGATSFields.end(), "GRAY written");
    ASSERT_EQ(cgats_fld_value("GRAY"), 0.7 * 255.0, "GRAY value");
}

// Test 5: CMYK encoding path
static void test_PutCGATSValues_CMYKEncoding() {
    reset_mocks();
    print_header("CMYKData encoding");

    cmsFloat64Number Float[4] = {0.1, 0.2, 0.3, 0.4};
    OutputColorSpace = cmsSigCmykData;

    PutCGATSValues(Float);

    ASSERT_TRUE(g_CGATSFields.find("CMYK_C") != g_CGATSFields.end(), "CMYK_C written");
    ASSERT_TRUE(g_CGATSFields.find("CMYK_M") != g_CGATSFields.end(), "CMYK_M written");
    ASSERT_TRUE(g_CGATSFields.find("CMYK_Y") != g_CGATSFields.end(), "CMYK_Y written");
    ASSERT_TRUE(g_CGATSFields.find("CMYK_K") != g_CGATSFields.end(), "CMYK_K written");

    ASSERT_EQ(cgats_fld_value("CMYK_C"), 0.1, "CMYK_C value");
    ASSERT_EQ(cgats_fld_value("CMYK_M"), 0.2, "CMYK_M value");
    ASSERT_EQ(cgats_fld_value("CMYK_Y"), 0.3, "CMYK_Y value");
    ASSERT_EQ(cgats_fld_value("CMYK_K"), 0.4, "CMYK_K value");
}

// Test 6: CMY encoding path (no K)
static void test_PutCGATSValues_CMYEncoding() {
    reset_mocks();
    print_header("CMYData encoding");

    cmsFloat64Number Float[3] = {0.05, 0.15, 0.25};
    OutputColorSpace = cmsSigCmyData;

    PutCGATSValues(Float);

    ASSERT_TRUE(g_CGATSFields.find("CMY_C") != g_CGATSFields.end(), "CMY_C written");
    ASSERT_TRUE(g_CGATSFields.find("CMY_M") != g_CGATSFields.end(), "CMY_M written");
    ASSERT_TRUE(g_CGATSFields.find("CMY_Y") != g_CGATSFields.end(), "CMY_Y written");

    ASSERT_EQ(cgats_fld_value("CMY_C"), 0.05, "CMY_C value");
    ASSERT_EQ(cgats_fld_value("CMY_M"), 0.15, "CMY_M value");
    ASSERT_EQ(cgats_fld_value("CMY_Y"), 0.25, "CMY_Y value");
}

// Test 7: 1colorData (e.g., 3 channels) branch
static void test_PutCGATSValues_1ColorBranch() {
    reset_mocks();
    print_header("1ColorData branch (3 channels)");

    cmsFloat64Number Float[3] = {0.9, 0.8, 0.7};
    OutputColorSpace = cmsSig1colorData;
    InputColorSpace = cmsSigRgbData; // n = 3

    PutCGATSValues(Float);

    // Expect fields: "3CLR_1", "3CLR_2", "3CLR_3" with value  * 100
    ASSERT_TRUE(g_CGATSFields.find("3CLR_1") != g_CGATSFields.end(), "3CLR_1 present");
    ASSERT_TRUE(g_CGATSFields.find("3CLR_2") != g_CGATSFields.end(), "3CLR_2 present");
    ASSERT_TRUE(g_CGATSFields.find("3CLR_3") != g_CGATSFields.end(), "3CLR_3 present");

    ASSERT_EQ(cgats_fld_value("3CLR_1"), 0.9 * 100.0, "3CLR_1 value");
    ASSERT_EQ(cgats_fld_value("3CLR_2"), 0.8 * 100.0, "3CLR_2 value");
    ASSERT_EQ(cgats_fld_value("3CLR_3"), 0.7 * 100.0, "3CLR_3 value");
}

// Test 8: Default branch (unknown color space -> 2 channels)
static void test_PutCGATSValues_DefaultBranch() {
    reset_mocks();
    print_header("Default branch (2 channels)");

    cmsFloat64Number Float[2] = {0.11, 0.22};
    OutputColorSpace = 99999; // Unknown, triggers default
    InputColorSpace = 0xDEADBEEF; // Unknown -> default 2 channels

    PutCGATSValues(Float);

    // Expect fields: "CHAN_1" and "CHAN_2"
    ASSERT_TRUE(g_CGATSFields.find("CHAN_1") != g_CGATSFields.end(), "CHAN_1 present");
    ASSERT_TRUE(g_CGATSFields.find("CHAN_2") != g_CGATSFields.end(), "CHAN_2 present");
    ASSERT_EQ(cgats_fld_value("CHAN_1"), 0.11, "CHAN_1 value");
    ASSERT_EQ(cgats_fld_value("CHAN_2"), 0.22, "CHAN_2 value");
}

// ---------------------------------------------------------------------------
// Test runner
// ---------------------------------------------------------------------------

static void run_all_tests() {
    test_PutCGATSValues_XYZEncoding();
    test_PutCGATSValues_LabEncoding();
    test_PutCGATSValues_RGBEncoding();
    test_PutCGATSValues_GrayEncoding();
    test_PutCGATSValues_CMYKEncoding();
    test_PutCGATSValues_CMYEncoding();
    test_PutCGATSValues_1ColorBranch();
    test_PutCGATSValues_DefaultBranch();
}

// Entry point
int main() {
    g_test_failed = false;
    run_all_tests();

    if (g_test_failed) {
        std::cout << "\nSome tests FAILED.\n";
        return 1;
    } else {
        std::cout << "\nAll tests PASSED.\n";
        return 0;
    }
}