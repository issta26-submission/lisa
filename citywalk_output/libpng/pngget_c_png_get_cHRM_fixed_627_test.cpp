// Automated unit test suite for the focal function: png_get_cHRM_fixed
// This test suite is self-contained (no GTest) and uses a minimal harness
// to validate the behavior of png_get_cHRM_fixed as implemented in the focal code.
//
// The tests exercise true/false branches of condition predicates, null-pointer handling,
// and proper data copying from the info_ptr->cHRM block to the output parameters.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>


// Domain-specific: minimal mock infrastructure to compile and run test without libpng

// Define types and constants mirroring the focal function's expectations
typedef int png_fixed_point;           // simplified fixed point representation
typedef unsigned int png_uint_32;

// Forward declarations for opaque structures (as in the real library)
struct png_struct;
struct png_info;
typedef const png_struct* png_const_structrp;
typedef png_struct*       png_structrp;
typedef const png_info*   png_const_inforp;
typedef png_info*         png_inforp;

// Minimal PNG info/cHRM mock structure
struct cHRM_t {
    int whitex;
    int whitey;
    int redx;
    int redy;
    int greenx;
    int greeny;
    int bluex;
    int bluey;
};

struct png_info {
    unsigned int valid;
    cHRM_t cHRM;
};

// Public constants used by the function
const unsigned int PNG_INFO_cHRM = 0x01; // bit flag indicating cHRM data valid

// Minimal mock for libpng internal helper used by the focal method
static void png_debug1(int /*level*/, const char* /*format*/, const char* /*s*/)
{
    // no-op for test environment
}

// Focal method redefined (mirrors the provided code in a self-contained manner)
static png_uint_32 png_get_cHRM_fixed(png_const_structrp png_ptr, png_const_inforp info_ptr,
    png_fixed_point *whitex, png_fixed_point *whitey, png_fixed_point *redx,
    png_fixed_point *redy, png_fixed_point *greenx, png_fixed_point *greeny,
    png_fixed_point *bluex, png_fixed_point *bluey)
{
{
   png_debug1(1, "in %s retrieval function", "cHRM");
   /* PNGv3: this just returns the values store from the cHRM, if any. */
   if (png_ptr != NULL && info_ptr != NULL &&
       (info_ptr->valid & PNG_INFO_cHRM) != 0)
   {
      if (whitex != NULL) *whitex = info_ptr->cHRM.whitex;
      if (whitey != NULL) *whitey = info_ptr->cHRM.whitey;
      if (redx   != NULL) *redx   = info_ptr->cHRM.redx;
      if (redy   != NULL) *redy   = info_ptr->cHRM.redy;
      if (greenx != NULL) *greenx = info_ptr->cHRM.greenx;
      if (greeny != NULL) *greeny = info_ptr->cHRM.greeny;
      if (bluex  != NULL) *bluex  = info_ptr->cHRM.bluex;
      if (bluey  != NULL) *bluey  = info_ptr->cHRM.bluey;
      return PNG_INFO_cHRM;
   }
   return 0;
}
}

// Simple non-terminating test framework
static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << (msg) << " at line " << __LINE__ << std::endl; \
        ++g_failures; \
    } \
} while (0)

#define EXPECT_EQ(a, b, msg) do { \
    if ((a) != (b)) { \
        std::cerr << "EXPECT_EQ failed: " << (msg) << " (got " << (a) << ", expected " << (b) << ") at line " << __LINE__ << std::endl; \
        ++g_failures; \
    } \
} while (0)

int main() {
    // Test A: All pointers non-null with valid cHRM data -> expect all fields copied and non-zero return
    {
        // Prepare mock PNG structures
        struct png_struct dummy_ptr;
        struct png_info info;
        info.valid = PNG_INFO_cHRM;
        info.cHRM.whitex = 11;
        info.cHRM.whitey = 22;
        info.cHRM.redx = 33;
        info.cHRM.redy = 44;
        info.cHRM.greenx = 55;
        info.cHRM.greeny = 66;
        info.cHRM.bluex = 77;
        info.cHRM.bluey = 88;

        png_fixed_point wX = 0, wY = 0, rX = 0, rY = 0, gX = 0, gY = 0, bX = 0, bY = 0;

        png_uint_32 ret = png_get_cHRM_fixed(reinterpret_cast<png_const_structrp>(&dummy_ptr),
                                           reinterpret_cast<png_const_inforp>(&info),
                                           &wX, &wY, &rX, &rY, &gX, &gY, &bX, &bY);

        EXPECT_TRUE(ret == PNG_INFO_cHRM, "Return code should indicate PNG_INFO_cHRM");
        EXPECT_EQ(wX, 11, "whitex should be copied as 11");
        EXPECT_EQ(wY, 22, "whitey should be copied as 22");
        EXPECT_EQ(rX, 33, "redx should be copied as 33");
        EXPECT_EQ(rY, 44, "redy should be copied as 44");
        EXPECT_EQ(gX, 55, "greenx should be copied as 55");
        EXPECT_EQ(gY, 66, "greeny should be copied as 66");
        EXPECT_EQ(bX, 77, "bluex should be copied as 77");
        EXPECT_EQ(bY, 88, "bluey should be copied as 88");
    }

    // Test B: Some output pointers NULL; ensure function handles NULL targets safely
    {
        struct png_struct dummy_ptr;
        struct png_info info;
        info.valid = PNG_INFO_cHRM;
        info.cHRM.whitex = 1;
        info.cHRM.whitey = 2;
        info.cHRM.redx = 3;
        info.cHRM.redy = 4;
        info.cHRM.greenx = 5;
        info.cHRM.greeny = 6;
        info.cHRM.bluex = 7;
        info.cHRM.bluey = 8;

        png_fixed_point wY = 0, rX = 0, rY = 0, gX = 0, gY = 0, bX = 0, bY = 0;

        // whitex is NULL, so only the other fields should be assigned
        png_uint_32 ret = png_get_cHRM_fixed(reinterpret_cast<png_const_structrp>(&dummy_ptr),
                                           reinterpret_cast<png_const_inforp>(&info),
                                           /*whitex*/ nullptr, &wY, &rX, &rY, &gX, &gY, &bX, &bY);

        EXPECT_TRUE(ret == PNG_INFO_cHRM, "Return code should be PNG_INFO_cHRM even with NULL whitex");
        EXPECT_EQ(wY, 2, "whitey should be copied as 2");
        EXPECT_EQ(rX, 3, "redx should be copied as 3");
        EXPECT_EQ(rY, 4, "redy should be copied as 4");
        EXPECT_EQ(gX, 5, "greenx should be copied as 5");
        EXPECT_EQ(gY, 6, "greeny should be copied as 6");
        EXPECT_EQ(bX, 7, "bluex should be copied as 7");
        EXPECT_EQ(bY, 8, "bluey should be copied as 8");
        // whitex value remains unchecked since it's NULL
    }

    // Test C: info_ptr is NULL -> should return 0 and not crash
    {
        struct png_struct dummy_ptr;
        png_fixed_point wX = 0, wY = 0, rX = 0, rY = 0, gX = 0, gY = 0, bX = 0, bY = 0;

        png_uint_32 ret = png_get_cHRM_fixed(reinterpret_cast<png_const_structrp>(&dummy_ptr),
                                           nullptr,
                                           &wX, &wY, &rX, &rY, &gX, &gY, &bX, &bY);

        EXPECT_TRUE(ret == 0, "Return should be 0 when info_ptr is NULL");
    }

    // Test D: info_ptr valid flag not set -> should return 0
    {
        struct png_struct dummy_ptr;
        struct png_info info;
        info.valid = 0; // no cHRM data
        info.cHRM.whitex = 10;
        info.cHRM.whitey = 20;
        info.cHRM.redx = 30;
        info.cHRM.redy = 40;
        info.cHRM.greenx = 50;
        info.cHRM.greeny = 60;
        info.cHRM.bluex = 70;
        info.cHRM.bluey = 80;

        png_fixed_point wX = 0, wY = 0, rX = 0, rY = 0, gX = 0, gY = 0, bX = 0, bY = 0;

        png_uint_32 ret = png_get_cHRM_fixed(reinterpret_cast<png_const_structrp>(&dummy_ptr),
                                           reinterpret_cast<png_const_inforp>(&info),
                                           &wX, &wY, &rX, &rY, &gX, &gY, &bX, &bY);

        EXPECT_TRUE(ret == 0, "Return should be 0 when PNG_INFO_cHRM flag is not set in info_ptr.valid");
    }

    // Test E: png_ptr NULL -> should return 0
    {
        struct png_info info;
        info.valid = PNG_INFO_cHRM;
        info.cHRM.whitex = 5; info.cHRM.whitey = 6; info.cHRM.redx = 7; info.cHRM.redy = 8;
        info.cHRM.greenx = 9; info.cHRM.greeny = 10; info.cHRM.bluex = 11; info.cHRM.bluey = 12;

        png_fixed_point wX = 0, wY = 0, rX = 0, rY = 0, gX = 0, gY = 0, bX = 0, bY = 0;

        png_uint_32 ret = png_get_cHRM_fixed(nullptr,
                                           reinterpret_cast<png_const_inforp>(&info),
                                           &wX, &wY, &rX, &rY, &gX, &gY, &bX, &bY);

        EXPECT_TRUE(ret == 0, "Return should be 0 when png_ptr is NULL");
    }

    // Summary
    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
    } else {
        std::cout << g_failures << " TEST(S) FAILED" << std::endl;
    }

    return (g_failures == 0) ? 0 : 1;
}