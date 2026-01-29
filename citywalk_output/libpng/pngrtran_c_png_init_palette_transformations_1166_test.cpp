// Minimal C++11 unit tests for png_init_palette_transformations
// This test suite provides a lightweight harness (no GTest) and a
// minimal stub of the necessary PNG structures/macros to exercise
// the focal function's logic.
// The tests cover true/false branches of condition predicates and
// specific code paths (palette-backed background handling and alpha paths).

#include <cstdint>
#include <cstdlib>
#include <pngpriv.h>
#include <cstdio>


// Domain-specific macro configuration (simulate libpng features)
#define PNG_ENCODE_ALPHA          0x01
#define PNG_COMPOSE                 0x02
#define PNG_BACKGROUND_EXPAND       0x04
#define PNG_EXPAND                  0x08
#define PNG_EXPAND_tRNS             0x10
#define PNG_INVERT_ALPHA            0x20
#define PNG_READ_EXPAND_SUPPORTED   1
#define PNG_READ_BACKGROUND_SUPPORTED 1
#define PNG_READ_INVERT_ALPHA_SUPPORTED 1
#define PNG_FLAG_OPTIMIZE_ALPHA       0x100

typedef unsigned char png_byte;

// Lightweight color struct used for the test
typedef struct {
    uint16_t red;
    uint16_t green;
    uint16_t blue;
} png_color;

// Forward declaration of the focal struct
typedef struct png_struct png_struct;
typedef png_struct* png_structrp;

// Minimal png_struct matching the fields used by the focal method
struct png_struct {
    int num_trans;
    png_byte* trans_alpha;
    png_color* palette;
    struct {
        uint16_t red;
        uint16_t green;
        uint16_t blue;
        int index;
    } background;
    unsigned int transformations;
    unsigned int flags;
};

// Forward declaration of the focal function (as implemented below)
extern "C" void png_init_palette_transformations(png_structrp png_ptr);

// Implementation of the focal method (simplified to shared test harness)
extern "C" void png_init_palette_transformations(png_structrp png_ptr)
{
    int input_has_alpha = 0;
    int input_has_transparency = 0;
    if (png_ptr->num_trans > 0)
    {
        int i;
        for (i = 0; i < png_ptr->num_trans; ++i)
        {
            if (png_ptr->trans_alpha[i] == 255)
                continue;
            else if (png_ptr->trans_alpha[i] == 0)
                input_has_transparency = 1;
            else
            {
                input_has_transparency = 1;
                input_has_alpha = 1;
                break;
            }
        }
    }
    if (input_has_alpha == 0)
    {
        png_ptr->transformations &= ~PNG_ENCODE_ALPHA;
        png_ptr->flags &= ~PNG_FLAG_OPTIMIZE_ALPHA;
        if (input_has_transparency == 0)
            png_ptr->transformations &= ~(PNG_COMPOSE | PNG_BACKGROUND_EXPAND);
    }
#if defined(PNG_READ_EXPAND_SUPPORTED) && defined(PNG_READ_BACKGROUND_SUPPORTED)
    if ((png_ptr->transformations & PNG_BACKGROUND_EXPAND) != 0 &&
        (png_ptr->transformations & PNG_EXPAND) != 0)
    {
        {
            png_ptr->background.red =
                png_ptr->palette[png_ptr->background.index].red;
            png_ptr->background.green =
                png_ptr->palette[png_ptr->background.index].green;
            png_ptr->background.blue =
                png_ptr->palette[png_ptr->background.index].blue;
#ifdef PNG_READ_INVERT_ALPHA_SUPPORTED
            if ((png_ptr->transformations & PNG_INVERT_ALPHA) != 0)
            {
                if ((png_ptr->transformations & PNG_EXPAND_tRNS) == 0)
                {
                    int i, istop = png_ptr->num_trans;
                    for (i = 0; i < istop; i++)
                        png_ptr->trans_alpha[i] =
                            (png_byte)(255 - png_ptr->trans_alpha[i]);
                }
            }
#endif /* READ_INVERT_ALPHA */
        }
    } /* background expand and (therefore) no alpha association. */
#endif /* READ_EXPAND && READ_BACKGROUND */
}

// Lightweight test harness
static int g_failures = 0;

static void report_failure(const char* test_name, const char* detail, const char* file, int line) {
    std::fprintf(stderr, "TEST FAIL: %s -- %s (%s:%d)\n", test_name, detail, file, line);
    ++g_failures;
}

// Helpers to compare values and report
#define CHECK_EQ_UINT(expected, actual, test_name, detail) \
    do { if (static_cast<uint64_t>(expected) != static_cast<uint64_t>(actual)) { report_failure(test_name, detail, __FILE__, __LINE__); } } while(0)

#define CHECK_EQ_INT(expected, actual, test_name, detail) \
    do { if ((int)(expected) != (int)(actual)) { report_failure(test_name, detail, __FILE__, __LINE__); } } while(0)

// Test Case 1: No palette entries (num_trans = 0) -> must strip encode alpha, clear optimize flag,
// and if no transparency, also clear compose/background_expand.
static void test_case_no_alpha_no_transparency() {
    // Setup: no per-pixel alpha; ensure initial flags are set to be cleared
    png_struct s;
    s.num_trans = 0;
    s.trans_alpha = nullptr;
    s.palette = nullptr;
    s.background.red = s.background.green = s.background.blue = 0;
    s.background.index = 0;
    s.transformations = PNG_ENCODE_ALPHA | PNG_COMPOSE | PNG_BACKGROUND_EXPAND;
    s.flags = PNG_FLAG_OPTIMIZE_ALPHA;

    png_init_palette_transformations(&s);

    // Expect ENCODE_ALPHA cleared
    CHECK_EQ_UINT(0, s.transformations & PNG_ENCODE_ALPHA, "case1:ENCODE_ALPHA cleared", "ENCODE_ALPHA should be cleared when no alpha present");
    // Expect OPTIMIZE_ALPHA cleared
    CHECK_EQ_UINT(0, s.flags & PNG_FLAG_OPTIMIZE_ALPHA, "case1:OPTIMIZE_ALPHA cleared", "OPTIMIZE_ALPHA should be cleared when no alpha present");
    // Since input_has_transparency == 0, COMPOSE and BACKGROUND_EXPAND should be cleared
    CHECK_EQ_UINT(0, s.transformations & (PNG_COMPOSE | PNG_BACKGROUND_EXPAND), "case1:COMPOSE/BACKGROUND_EXPAND cleared", "COMPOSE and BACKGROUND_EXPAND should be cleared when no transparency and no alpha");
}

// Test Case 2: num_trans > 0 with all entries opaque (255) -> input_has_alpha == 0, input_has_transparency == 0
static void test_case_all_entries_opaque() {
    png_struct s;
    s.num_trans = 1;
    png_byte t = 255;
    s.trans_alpha = &t;
    s.palette = nullptr;
    s.background.red = s.background.green = s.background.blue = 0;
    s.background.index = 0;
    s.transformations = PNG_ENCODE_ALPHA | PNG_COMPOSE | PNG_BACKGROUND_EXPAND;
    s.flags = PNG_FLAG_OPTIMIZE_ALPHA;

    png_init_palette_transformations(&s);

    // ENCODE_ALPHA and OPTIMIZE_ALPHA should be cleared
    CHECK_EQ_UINT(0, s.transformations & PNG_ENCODE_ALPHA, "case2:ENCODE_ALPHA cleared", "ENCODE_ALPHA should be cleared when alpha entries exist but opaque");
    CHECK_EQ_UINT(0, s.flags & PNG_FLAG_OPTIMIZE_ALPHA, "case2:OPTIMIZE_ALPHA cleared", "OPTIMIZE_ALPHA should be cleared when alpha entries exist but opaque");
    // Since input_has_transparency == 0, COMPOSE/BACKGROUND_EXPAND should be cleared
    CHECK_EQ_UINT(0, s.transformations & (PNG_COMPOSE | PNG_BACKGROUND_EXPAND), "case2:COMPOSE/BACKGROUND_EXPAND cleared", "COMPOSE/BACKGROUND_EXPAND should be cleared when no transparency");
}

// Test Case 3: num_trans > 0 with at least one zero (0) entry -> input_has_transparency == 1, input_has_alpha == 0
static void test_case_transparency_present_but_no_per_pixel_alpha() {
    png_struct s;
    s.num_trans = 1;
    png_byte t = 0;
    s.trans_alpha = &t;
    s.palette = nullptr;
    s.background.red = s.background.green = s.background.blue = 0;
    s.background.index = 0;
    s.transformations = PNG_ENCODE_ALPHA | PNG_COMPOSE | PNG_BACKGROUND_EXPAND;
    s.flags = PNG_FLAG_OPTIMIZE_ALPHA;

    png_init_palette_transformations(&s);

    // ENCODE_ALPHA and OPTIMIZE_ALPHA should be cleared
    CHECK_EQ_UINT(0, s.transformations & PNG_ENCODE_ALPHA, "case3:ENCODE_ALPHA cleared", "ENCODE_ALPHA should be cleared when there is some transparency value (0) but no per-pixel alpha");
    CHECK_EQ_UINT(0, s.flags & PNG_FLAG_OPTIMIZE_ALPHA, "case3:OPTIMIZE_ALPHA cleared", "OPTIMIZE_ALPHA should be cleared in this scenario");
    // COMPOSE and BACKGROUND_EXPAND should remain set
    CHECK_EQ_UINT(1, s.transformations & PNG_COMPOSE, "case3:COMPOSE remains set", "COMPOSE should remain set when transparency present but no per-pixel alpha");
    CHECK_EQ_UINT(1, s.transformations & PNG_BACKGROUND_EXPAND, "case3:BACKGROUND_EXPAND remains set", "BACKGROUND_EXPAND should remain set when transparency present but no per-pixel alpha");
}

// Test Case 4: Background expansion path with alpha present, palette lookup
static void test_case_background_expand_and_palette_assignment() {
    png_struct s;
    s.num_trans = 2;
    png_byte tA[2] = { 100, 200 };
    s.trans_alpha = tA;
    s.palette = new png_color[2];
    // Palette: [0] arbitrary, [1] target color
    s.palette[0].red = 1; s.palette[0].green = 2; s.palette[0].blue = 3;
    s.palette[1].red = 10; s.palette[1].green = 20; s.palette[1].blue = 30;
    s.background.red = 0; s.background.green = 0; s.background.blue = 0;
    s.background.index = 1; // choose palette[1]
    s.transformations = PNG_BACKGROUND_EXPAND | PNG_EXPAND; // trigger block
    s.flags = 0;

    png_init_palette_transformations(&s);

    // Background should be set to palette[1]
    CHECK_EQ_UINT(10, s.background.red, "case4:background.red", "Background red should come from palette[1].red");
    CHECK_EQ_UINT(20, s.background.green, "case4:background.green", "Background green should come from palette[1].green");
    CHECK_EQ_UINT(30, s.background.blue, "case4:background.blue", "Background blue should come from palette[1].blue");
    // trans_alpha should be unchanged
    CHECK_EQ_UINT(100, s.trans_alpha[0], "case4:trans_alpha[0] unchanged", "First trans alpha should remain 100");
    CHECK_EQ_UINT(200, s.trans_alpha[1], "case4:trans_alpha[1] unchanged", "Second trans alpha should remain 200");

    delete[] s.palette;
}

// Test Case 5: Invert alpha path without tRNS expansion (should invert trans_alpha)
static void test_case_invert_alpha_path_no_trns() {
    png_struct s;
    s.num_trans = 2;
    png_byte tA[2] = { 100, 200 };
    s.trans_alpha = tA;
    s.palette = new png_color[2];
    s.palette[0].red = 1; s.palette[0].green = 2; s.palette[0].blue = 3;
    s.palette[1].red = 11; s.palette[1].green = 22; s.palette[1].blue = 33;
    s.background.red = 0; s.background.green = 0; s.background.blue = 0;
    s.background.index = 1;
    // Trigger both expand conditions and invert alpha in effect
    s.transformations = PNG_BACKGROUND_EXPAND | PNG_EXPAND | PNG_INVERT_ALPHA;
    s.flags = 0;

    // No tRNS expansion
    // Ensure TRANSFORMATIONS do not set PNG_EXPAND_tRNS
    // OP: Invert path will run
    png_init_palette_transformations(&s);

    // trans_alpha should be inverted
    CHECK_EQ_UINT(155, s.trans_alpha[0], "case5:trans_alpha[0] inverted", "First trans alpha should be inverted to 155");
    CHECK_EQ_UINT(55, s.trans_alpha[1], "case5:trans_alpha[1] inverted", "Second trans alpha should be inverted to 55");
    // Background should come from palette[1]
    CHECK_EQ_UINT(11, s.background.red, "case5:background.red", "Background red from palette[1].red");
    CHECK_EQ_UINT(22, s.background.green, "case5:background.green", "Background green from palette[1].green");
    CHECK_EQ_UINT(33, s.background.blue, "case5:background.blue", "Background blue from palette[1].blue");

    delete[] s.palette;
}

// Test Case 6: Invert alpha path but with tRNS present (should NOT invert)
static void test_case_invert_alpha_path_with_trns_present() {
    png_struct s;
    s.num_trans = 2;
    png_byte tA[2] = { 100, 200 };
    s.trans_alpha = tA;
    s.palette = new png_color[2];
    s.palette[0].red = 1; s.palette[0].green = 2; s.palette[0].blue = 3;
    s.palette[1].red = 7; s.palette[1].green = 14; s.palette[1].blue = 21;
    s.background.red = 0; s.background.green = 0; s.background.blue = 0;
    s.background.index = 1;
    // Trigger both expand and invert alpha, but tRNS expansion is active
    s.transformations = PNG_BACKGROUND_EXPAND | PNG_EXPAND | PNG_INVERT_ALPHA | PNG_EXPAND_tRNS;
    s.flags = 0;

    png_init_palette_transformations(&s);

    // trans_alpha should NOT be inverted because PNG_EXPAND_tRNS is set
    CHECK_EQ_UINT(100, s.trans_alpha[0], "case6:trans_alpha[0] unchanged", "First trans alpha should remain 100 when tRNS is present");
    CHECK_EQ_UINT(200, s.trans_alpha[1], "case6:trans_alpha[1] unchanged", "Second trans alpha should remain 200 when tRNS is present");
    // Background color should still be palette[1]
    CHECK_EQ_UINT(7, s.background.red, "case6:background.red", "Background red from palette[1].red");
    CHECK_EQ_UINT(14, s.background.green, "case6:background.green", "Background green from palette[1].green");
    CHECK_EQ_UINT(21, s.background.blue, "case6:background.blue", "Background blue from palette[1].blue");

    delete[] s.palette;
}

// Runner and summary
int main() {
    // Run all test cases
    test_case_no_alpha_no_transparency();
    test_case_all_entries_opaque();
    test_case_transparency_present_but_no_per_pixel_alpha();
    test_case_background_expand_and_palette_assignment();
    test_case_invert_alpha_path_no_trns();
    test_case_invert_alpha_path_with_trns_present();

    if (g_failures != 0) {
        std::fprintf(stderr, "Total failures: %d\n", g_failures);
        return EXIT_FAILURE;
    }
    std::printf("All tests passed.\n");
    return EXIT_SUCCESS;
}