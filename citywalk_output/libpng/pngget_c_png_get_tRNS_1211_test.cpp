// Unit test suite for png_get_tRNS (simplified mock of libpng environment)
// This test suite is implemented in C++11 without GoogleTest.
// It exercises both branches of the focal function and verifies expected outputs.

#include <cstdint>
#include <cassert>
#include <pngpriv.h>
#include <cstdio>
#include <cstring>


// ---------------- Mocked / Minimal PNG types and constants -----------------

typedef unsigned char png_bytep;
typedef unsigned char png_byte;
typedef uint32_t      png_uint_32;

#define PNG_INFO_tRNS          0x01
#define PNG_COLOR_TYPE_PALETTE  3

// Forward-declare a minimal opaque png_struct to satisfy the function signature.
struct png_struct;
typedef const struct png_struct* png_const_structrp;

// Minimal info struct with only fields used by png_get_tRNS.
struct png_color_16 {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
    uint16_t gray;
    unsigned char index;
};

typedef png_color_16* png_color_16p;
struct png_info {
    uint32_t valid;
    uint32_t color_type;
    png_bytep trans_alpha; // pointer to alpha data for tRNS
    png_color_16 trans_color;
    uint32_t num_trans;
};

// Typedefs matching the function signature
typedef struct png_info* png_inforp;
typedef const struct png_info* png_const_inforp;

// The actual function under test (copied/adapted from the provided snippet)
static void png_debug1(int, const char*, const char*) { /* no-op for tests */ }

png_uint_32 png_get_tRNS(png_const_structrp png_ptr,
                         png_inforp info_ptr,
                         png_bytep *trans_alpha,
                         int *num_trans,
                         png_color_16p *trans_color)
{
    png_uint_32 retval = 0;
    png_debug1(1, "in %s retrieval function", "tRNS");
    if (png_ptr != NULL && info_ptr != NULL &&
        (info_ptr->valid & PNG_INFO_tRNS) != 0)
    {
        if (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
        {
            if (trans_alpha != NULL)
            {
                *trans_alpha = info_ptr->trans_alpha;
                retval |= PNG_INFO_tRNS;
            }
            if (trans_color != NULL)
                *trans_color = &(info_ptr->trans_color);
        }
        else /* if (info_ptr->color_type != PNG_COLOR_TYPE_PALETTE) */
        {
            if (trans_color != NULL)
            {
                *trans_color = &(info_ptr->trans_color);
                retval |= PNG_INFO_tRNS;
            }
            if (trans_alpha != NULL)
                *trans_alpha = NULL;
        }
        if (num_trans != NULL)
        {
            *num_trans = info_ptr->num_trans;
            retval |= PNG_INFO_tRNS;
        }
    }
    return retval;
}

// -------------------------- Unit Tests (no gtest) ---------------------------

static bool test_palette_path_complete_output() {
    // Purpose: Validate that in palette color type, the function fills trans_alpha,
    // trans_color, and num_trans and returns PNG_INFO_tRNS when valid bit is set.

    // Prepare fake png_ptr (non-null to satisfy first condition)
    struct png_struct dummy;
    png_const_structrp png_ptr = &dummy;

    // Prepare info_ptr with tRNS valid flag and palette color type
    struct png_info info;
    info.valid = PNG_INFO_tRNS;
    info.color_type = PNG_COLOR_TYPE_PALETTE;
    unsigned char alpha_arr[4] = { 0x11, 0x22, 0x33, 0x44 };
    info.trans_alpha = alpha_arr;
    info.trans_color.red = 5;
    info.trans_color.green = 6;
    info.trans_color.blue = 7;
    info.trans_color.gray = 0;
    info.trans_color.index = 9;
    info.num_trans = 13;

    // Outputs
    png_bytep trans_alpha_out = nullptr;
    png_color_16p trans_color_out = nullptr;
    int num_trans_out = -1;

    png_uint_32 ret = png_get_tRNS(png_ptr, &info, &trans_alpha_out, &num_trans_out, &trans_color_out);

    bool ok = true;
    if (ret != PNG_INFO_tRNS) {
        printf("FAIL palette_path_complete_output: retval mismatch. expected %u, got %u\n",
               (unsigned)PNG_INFO_tRNS, (unsigned)ret);
        ok = false;
    }
    if (trans_alpha_out != alpha_arr) {
        printf("FAIL palette_path_complete_output: trans_alpha_out pointer mismatch.\n");
        ok = false;
    }
    if (trans_color_out != &info.trans_color) {
        printf("FAIL palette_path_complete_output: trans_color_out pointer mismatch.\n");
        ok = false;
    }
    if (num_trans_out != (int)info.num_trans) {
        printf("FAIL palette_path_complete_output: num_trans_out mismatch. expected %d, got %d\n",
               (int)info.num_trans, num_trans_out);
        ok = false;
    }

    return ok;
}

static bool test_non_palette_path_output_only_color_and_num() {
    // Purpose: Validate that in non-palette color types, the function fills only trans_color
    // (and num_trans) and sets trans_alpha to NULL.

    // Prepare fake png_ptr
    struct png_struct dummy;
    png_const_structrp png_ptr = &dummy;

    // info_ptr non-palette
    struct png_info info;
    info.valid = PNG_INFO_tRNS;
    info.color_type = 0 /* not PALETTE, e.g., grayscale */;
    unsigned char alpha_arr[2] = { 0xAA, 0xBB };
    info.trans_alpha = alpha_arr;
    info.trans_color.red = 1;
    info.trans_color.green = 2;
    info.trans_color.blue = 3;
    info.trans_color.gray = 4;
    info.trans_color.index = 0;
    info.num_trans = 5;

    // Outputs
    png_bytep trans_alpha_out = reinterpret_cast<png_bytep>(0x1234);
    png_color_16p trans_color_out = nullptr;
    int num_trans_out = -99;

    png_uint_32 ret = png_get_tRNS(png_ptr, &info, &trans_alpha_out, &num_trans_out, &trans_color_out);

    bool ok = true;
    if (ret != PNG_INFO_tRNS) {
        printf("FAIL non_palette_path_output_only_color_and_num: retval mismatch. expected %u, got %u\n",
               (unsigned)PNG_INFO_tRNS, (unsigned)ret);
        ok = false;
    }
    if (trans_color_out != &info.trans_color) {
        printf("FAIL non_palette_path_output_only_color_and_num: trans_color_out pointer mismatch.\n");
        ok = false;
    }
    if (trans_alpha_out != nullptr) {
        printf("FAIL non_palette_path_output_only_color_and_num: trans_alpha_out should be NULL, got non-NULL.\n");
        ok = false;
    }
    if (num_trans_out != (int)info.num_trans) {
        printf("FAIL non_palette_path_output_only_color_and_num: num_trans_out mismatch. expected %d, got %d\n",
               (int)info.num_trans, num_trans_out);
        ok = false;
    }

    return ok;
}

static bool test_invalid_info_ptr_does_not_modify_outputs() {
    // Purpose: Ensure that when info_ptr is NULL or invalid, the function returns 0
    // and does not modify output parameters.

    // Prepare fake png_ptr
    struct png_struct dummy;
    png_const_structrp png_ptr = &dummy;

    // Case A: info_ptr is NULL
    {
        png_bytep trans_alpha_out;
        png_color_16p trans_color_out = nullptr;
        int num_trans_out = 42;
        png_uint_32 ret = png_get_tRNS(png_ptr, nullptr, &trans_alpha_out, &num_trans_out, &trans_color_out);
        if (ret != 0) {
            printf("FAIL invalid_info_ptr_caseA: expected ret 0, got %u\n", (unsigned)ret);
            return false;
        }
        // No outputs to check since info_ptr was NULL
    }

    // Case B: info_ptr not valid (valid bit not set)
    {
        struct png_info info;
        info.valid = 0;
        info.color_type = PNG_COLOR_TYPE_PALETTE;
        info.trans_alpha = nullptr;
        info.trans_color.red = info.trans_color.green = info.trans_color.blue = 0;
        info.trans_color.gray = 0;
        info.trans_color.index = 0;
        info.num_trans = 0;

        png_bytep trans_alpha_out = nullptr;
        png_color_16p trans_color_out = nullptr;
        int num_trans_out = -7;

        png_uint_32 ret = png_get_tRNS(png_ptr, &info, &trans_alpha_out, &num_trans_out, &trans_color_out);
        if (ret != 0) {
            printf("FAIL invalid_info_ptr_caseB: expected ret 0, got %u\n", (unsigned)ret);
            return false;
        }
        // Ensure no modifications occurred
        if (trans_alpha_out != nullptr) {
            printf("FAIL invalid_info_ptr_caseB: trans_alpha_out unexpectedly modified.\n");
            return false;
        }
        if (trans_color_out != nullptr) {
            printf("FAIL invalid_info_ptr_caseB: trans_color_out unexpectedly modified.\n");
            return false;
        }
        if (num_trans_out != -7) {
            printf("FAIL invalid_info_ptr_caseB: num_trans_out unexpectedly modified.\n");
            return false;
        }
    }

    return true;
}

static bool test_null_trans_alpha_parameter_skips_write() {
    // Purpose: When trans_alpha parameter is NULL, ensure function does not attempt to write it
    // and still returns tRNS when applicable.

    // png_ptr
    struct png_struct dummy;
    png_const_structrp png_ptr = &dummy;

    // info_ptr with palette
    struct png_info info;
    info.valid = PNG_INFO_tRNS;
    info.color_type = PNG_COLOR_TYPE_PALETTE;
    unsigned char alpha_arr[4] = { 9, 9, 9, 9 };
    info.trans_alpha = alpha_arr;
    info.trans_color.red = 10;
    info.trans_color.green = 20;
    info.trans_color.blue = 30;
    info.trans_color.gray = 40;
    info.trans_color.index = 1;
    info.num_trans = 2;

    png_bytep trans_alpha_out = nullptr;
    png_color_16p trans_color_out = nullptr;
    int num_trans_out = 0;

    // Pass trans_alpha as NULL (i.e., do not attempt to retrieve alpha)
    png_uint_32 ret = png_get_tRNS(png_ptr, &info, NULL, &num_trans_out, &trans_color_out);

    bool ok = true;
    if (ret != PNG_INFO_tRNS) {
        printf("FAIL null_trans_alpha_parameter_skips_write: retval mismatch. expected %u, got %u\n",
               (unsigned)PNG_INFO_tRNS, (unsigned)ret);
        ok = false;
    }
    if (trans_color_out != &info.trans_color) {
        printf("FAIL null_trans_alpha_parameter_skips_write: trans_color_out pointer mismatch.\n");
        ok = false;
    }
    // Since trans_alpha was NULL, there should be no attempt to modify it; nothing to check for NULL here.
    if (num_trans_out != info.num_trans) {
        printf("FAIL null_trans_alpha_parameter_skips_write: num_trans_out mismatch. expected %d, got %d\n",
               (int)info.num_trans, num_trans_out);
        ok = false;
    }

    return ok;
}

// --------------------------------- Main Runner ---------------------------------

int main() {
    int total = 0;
    int passed = 0;

    auto run = [&](const char* name, bool (*test)()) {
        total++;
        bool ok = test();
        if (ok) {
            passed++;
            printf("[PASS] %s\n", name);
        } else {
            printf("[FAIL] %s\n", name);
        }
    };

    run("test_palette_path_complete_output", test_palette_path_complete_output);
    run("test_non_palette_path_output_only_color_and_num", test_non_palette_path_output_only_color_and_num);
    run("test_invalid_info_ptr_does_not_modify_outputs", test_invalid_info_ptr_does_not_modify_outputs);
    run("test_null_trans_alpha_parameter_skips_write", test_null_trans_alpha_parameter_skips_write);

    printf("Summary: %d / %d tests passed.\n", passed, total);
    return (passed == total) ? 0 : 1;
}