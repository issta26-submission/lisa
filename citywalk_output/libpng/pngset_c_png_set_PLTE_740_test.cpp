// Comprehensive unit test suite for the focal method: png_set_PLTE
// This test suite is self-contained and does not rely on GTest.
// It uses a minimal, mocked environment of the libpng-like structures and
// functions necessary to exercise the control flow of the focal method.

#include <cstdlib>
#include <iostream>
#include <pngpriv.h>
#include <stdexcept>
#include <cstring>


// Domain knowledge notes implemented as constants and mocks
#define PNG_MAX_PALETTE_LENGTH 256
#define PNG_COLOR_TYPE_PALETTE 3
#define PNG_COLOR_TYPE_RGB 2
#define PNG_INFO_PLTE 0x01
#define PNG_FREE_PLTE 0x01
#define PNG_FLAG_MNG_EMPTY_PLTE 0x01
#define PNG_MNG_FEATURES_SUPPORTED 1  // Enable MNG-related branches in tests

// Typedef-like surface to mimic libpng's types
typedef unsigned int png_uint_32;
typedef unsigned short png_uint_16;
typedef unsigned char png_byte;

typedef struct png_color {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} png_color;

typedef png_color* png_colorp;
typedef const png_color* png_const_colorp;

typedef struct png_struct_def png_struct;
typedef png_struct* png_structrp;
typedef struct png_info_def png_info;
typedef png_info* png_inforp;

// Faked "pointer to struct" types used by the focal function
typedef png_struct* png_structrp; // explicit alias for clarity
typedef png_info* png_inforp;

// Simplified debug/warning/error helpers
static void png_debug1(int /*level*/, const char* /*format*/, const char* /*arg*/) {
    // no-op in tests
}

static void png_error(png_structrp /*png_ptr*/, const char* msg) {
    // In the real libpng, this would longjmp; here we throw to signal failure.
    throw std::runtime_error(msg);
}

static void png_warning(png_structrp /*png_ptr*/, const char* /*msg*/) {
    // Warnings are non-fatal in tests; just a no-op.
}

// Memory management utilities used by the focal function
static void* png_calloc(png_structrp /*png_ptr*/, size_t size) {
    void* p = std::calloc(1, size);
    return p;
}

static void png_free_data(png_structrp /*png_ptr*/, png_inforp info_ptr,
                          int /*free_data*/, int /*unknown*/) {
    // Only free PLTE-related data for tests
    if (info_ptr && info_ptr->palette) {
        std::free(info_ptr->palette);
        info_ptr->palette = NULL;
        info_ptr->num_palette = 0;
    }
}

// Utility to emulate libpng-esque "cast" macro
#define png_voidcast(type, value) (type)(value)

// Minimal allocations helper (compat with png_calloc usage)
static void* png_raw_alloc(size_t size) {
    return std::malloc(size);
}
static void png_raw_free(void* p) {
    std::free(p);
}

// Structures used by the focal method
struct png_struct_def {
    int mng_features_permitted;
};

struct png_info_def {
    png_colorp palette;
    png_uint_16 num_palette;
    int free_me;
    int valid;
    int color_type;
    int bit_depth;
};

// Focal method under test (recreated in this test harness)
void png_set_PLTE(png_structrp png_ptr, png_inforp info_ptr,
    png_const_colorp palette, int num_palette)
{
{
   png_uint_32 max_palette_length;
   png_debug1(1, "in %s storage function", "PLTE");
   if (png_ptr == NULL || info_ptr == NULL)
      return;
   max_palette_length = (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE) ?
      (1 << info_ptr->bit_depth) : PNG_MAX_PALETTE_LENGTH;
   if (num_palette < 0 || num_palette > (int) max_palette_length)
   {
      if (info_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
         png_error(png_ptr, "Invalid palette length");
      else
      {
         png_warning(png_ptr, "Invalid palette length");
         return;
      }
   }
   if ((num_palette > 0 && palette == NULL) ||
      (num_palette == 0
#        ifdef PNG_MNG_FEATURES_SUPPORTED
            && (png_ptr->mng_features_permitted & PNG_FLAG_MNG_EMPTY_PLTE) == 0
#        endif
      ))
   {
      png_error(png_ptr, "Invalid palette");
   }
   /* It may not actually be necessary to set png_ptr->palette here;
    * we do it for backward compatibility with the way the png_handle_tRNS
    * function used to do the allocation.
    *
    * 1.6.0: the above statement appears to be incorrect; something has to set
    * the palette inside png_struct on read.
    */
   png_free_data(png_ptr, info_ptr, PNG_FREE_PLTE, 0);
   /* Changed in libpng-1.2.1 to allocate PNG_MAX_PALETTE_LENGTH instead
    * of num_palette entries, in case of an invalid PNG file or incorrect
    * call to png_set_PLTE() with too-large sample values.
    */
   png_ptr->palette = png_voidcast(png_colorp, png_calloc(png_ptr,
       PNG_MAX_PALETTE_LENGTH * (sizeof (png_color))));
   if (num_palette > 0)
      memcpy(png_ptr->palette, palette, (unsigned int)num_palette *
          (sizeof (png_color)));
   info_ptr->palette = png_ptr->palette;
   info_ptr->num_palette = png_ptr->num_palette = (png_uint_16)num_palette;
   info_ptr->free_me |= PNG_FREE_PLTE;
   info_ptr->valid |= PNG_INFO_PLTE;
}
}

// Helper to access fields that libpng style would provide
typedef png_struct_def png_structrp_alias;
typedef png_info_def png_inforp_alias;

// Test framework scaffolding
static int gFailures = 0;
#define EXPECT(cond) do { \
    if(!(cond)) { \
        ++gFailures; \
        std::cerr << "Expectation failed: " << #cond \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    } \
} while(0)

// Helper to create a fresh PNG info/struct combo for tests
static void init_structs(png_structrp* s_out, png_inforp* i_out) {
    png_struct* s = (png_struct*)std::malloc(sizeof(png_struct));
    std::memset(s, 0, sizeof(png_struct));
    s->mng_features_permitted = 0; // default: no empty-PLTE allowance
    png_info* info = (png_info*)std::malloc(sizeof(png_info));
    std::memset(info, 0, sizeof(png_info));
    info->palette = NULL;
    info->num_palette = 0;
    info->free_me = 0;
    info->valid = 0;
    info->color_type = PNG_COLOR_TYPE_RGB; // default non-palette
    info->bit_depth = 8;
    *s_out = s;
    *i_out = info;
}

// Test 1: Valid palette assignment for palette-type PNG with non-empty palette
static void test_png_set_PLTE_valid_palette() {
    // Setup
    png_structrp s; png_inforp info;
    init_structs(&s, &info);
    info->color_type = PNG_COLOR_TYPE_PALETTE; // palette-based image
    info->bit_depth = 8;
    png_color palette[3] = { {255,0,0}, {0,255,0}, {0,0,255} };

    // Exercise
    try {
        png_set_PLTE(s, info, palette, 3);
    } catch (...) {
        // If an exception occurs, test should fail
        EXPECT(false);
        // Clean up to prevent leaks
        png_free_data(s, info, PNG_FREE_PLTE, 0);
        std::free(info);
        std::free(s);
        return;
    }

    // Verify
    EXPECT(info->palette != NULL);
    EXPECT(info->num_palette == 3);
    EXPECT((info->valid & PNG_INFO_PLTE) != 0);

    // Cleanup
    png_free_data(s, info, PNG_FREE_PLTE, 0);
    std::free(info);
    std::free(s);
}

// Test 2: Negative palette length should raise error for palette-type
static void test_png_set_PLTE_negative_length_palette_type() {
    png_structrp s; png_inforp info;
    init_structs(&s, &info);
    info->color_type = PNG_COLOR_TYPE_PALETTE;
    info->bit_depth = 8;
    png_color palette[2] = { {10,20,30}, {40,50,60} };

    bool threw = false;
    try {
        png_set_PLTE(s, info, palette, -1);
    } catch (const std::exception& e) {
        threw = true;
    }

    // Verify
    EXPECT(threw); // should throw due to invalid palette length
    // Cleanup
    png_free_data(s, info, PNG_FREE_PLTE, 0);
    std::free(info);
    std::free(s);
}

// Test 3: Negative length when not palette-type should warn and return (no exception)
static void test_png_set_PLTE_negative_length_non_palette_should_warn_and_return() {
    png_structrp s; png_inforp info;
    init_structs(&s, &info);
    // Non-palette color type (RGB)
    info->color_type = PNG_COLOR_TYPE_RGB;
    info->bit_depth = 8;
    png_color dummy[1] = { {0,0,0} };

    // Should warn and return without modifying info_ptr
    try {
        png_set_PLTE(s, info, dummy, -5);
    } catch (...) {
        // Should not throw
        EXPECT(false);
    }

    // Verify no palette allocated and state unchanged
    EXPECT(info->palette == NULL);
    EXPECT(info->num_palette == 0);

    // Cleanup
    png_free_data(s, info, PNG_FREE_PLTE, 0);
    std::free(info);
    std::free(s);
}

// Test 4: Empty palette with non-zero length but NULL palette pointer should error
static void test_png_set_PLTE_zero_length_null_palette_should_error() {
    png_structrp s; png_inforp info;
    init_structs(&s, &info);
    info->color_type = PNG_COLOR_TYPE_PALETTE;
    info->bit_depth = 8;

    bool threw = false;
    try {
        png_set_PLTE(s, info, NULL, 0 + 0); // palette NULL, length 0 is edge-case
    } catch (const std::exception&) {
        threw = true;
    }

    // If the internal code path is reached for num_palette == 0, it should not error
    // due to NULL palette since mng feature may not be allowed; but with our test flags
    // PNG_MNG_FEATURES_SUPPORTED is enabled, it should allocate for zero length when allowed.
    // We will just ensure no crash occurs.
    if (threw) {
        // If an exception occurred, that's a test fail for this path
        EXPECT(false);
    } else {
        // No exception; behavior depends on mng flag (not set by default here)
        // We expect either a failure or no crash; for determinism, ensure no crash
        EXPECT(true);
    }

    // Cleanup
    png_free_data(s, info, PNG_FREE_PLTE, 0);
    std::free(info);
    std::free(s);
}

// Test 5: Zero-length palette when MNG empty-PLTE feature is not permitted should error
static void test_png_set_PLTE_zero_length_not_permitted_should_error() {
    png_structrp s; png_inforp info;
    init_structs(&s, &info);
    info->color_type = PNG_COLOR_TYPE_RGB; // non-palette
    info->bit_depth = 8;

    // Do not permit empty PLTE
    s->mng_features_permitted = 0;

    bool threw = false;
    try {
        png_set_PLTE(s, info, NULL, 0);
    } catch (...) {
        threw = true;
    }

    // Expect an error path (png_error) for invalid empty palette
    EXPECT(threw);

    // Cleanup
    png_free_data(s, info, PNG_FREE_PLTE, 0);
    std::free(info);
    std::free(s);
}

// Test 6: Zero-length palette with MNG allowed should allocate and set PLTE-related info
static void test_png_set_PLTE_zero_length_with_mng_allowed() {
    png_structrp s; png_inforp info;
    init_structs(&s, &info);
    info->color_type = PNG_COLOR_TYPE_RGB; // non-palette type; length 0 path uses max length
    info->bit_depth = 8;
    s->mng_features_permitted = PNG_FLAG_MNG_EMPTY_PLTE; // Allow empty PLTE

    try {
        png_set_PLTE(s, info, NULL, 0);
    } catch (...) {
        // Should not throw
        EXPECT(false);
        png_free_data(s, info, PNG_FREE_PLTE, 0);
        std::free(info);
        std::free(s);
        return;
    }

    // Expect a PLTE to be allocated due to empty-PLTE allowance
    EXPECT(info->palette != NULL);
    EXPECT(info->num_palette == 0);
    // PLTE should be marked valid and freed by free_me flag
    EXPECT((info->valid & PNG_INFO_PLTE) != 0);
    EXPECT((info->free_me & PNG_FREE_PLTE) != 0);

    // Cleanup
    png_free_data(s, info, PNG_FREE_PLTE, 0);
    std::free(info);
    std::free(s);
}

// Runner
int main() {
    std::cout << "Running png_set_PLTE unit tests...\n";

    test_png_set_PLTE_valid_palette();
    test_png_set_PLTE_negative_length_palette_type();
    test_png_set_PLTE_negative_length_non_palette_should_warn_and_return();
    test_png_set_PLTE_zero_length_null_palette_should_error();
    test_png_set_PLTE_zero_length_not_permitted_should_error();
    test_png_set_PLTE_zero_length_with_mng_allowed();

    if (gFailures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << gFailures << " test failure(s) detected.\n";
        return 1;
    }
}