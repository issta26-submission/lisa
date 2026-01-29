#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// This test harness provides a minimal, self-contained environment
// to exercise the focal function: png_set_alpha_mode_fixed.
// It emulates a tiny subset of the libpng internal structures and
// required helper functions so that the function can be compiled
// and executed in a plain C++11 environment without GTest.

// Minimal fixed-point type and constants (to mirror libpng usage)
typedef int32_t png_fixed_point;
static const int PNG_FP_1 = (1 << 16); // 1.0 in fixed-point (16.16)

// Alpha mode constants (subset needed by png_set_alpha_mode_fixed)
#define PNG_ALPHA_PNG        0
#define PNG_ALPHA_ASSOCIATED   1
#define PNG_ALPHA_OPTIMIZED    2
#define PNG_ALPHA_BROKEN       3

#define PNG_ENCODE_ALPHA       0x01
#define PNG_FLAG_OPTIMIZE_ALPHA 0x02
#define PNG_COMPOSE             0x04
#define PNG_BACKGROUND_EXPAND   0x08
#define PNG_BACKGROUND_GAMMA_FILE 1

// Basic color struct used for the "background" placeholder
struct png_color_16 {
    unsigned short red;
    unsigned short green;
    unsigned short blue;
    unsigned short gray;
};

// Minimal png_struct used for testing
struct png_struct {
    png_fixed_point default_gamma; // file gamma default
    png_fixed_point screen_gamma;  // screen (output) gamma
    unsigned int transformations;    // transformation flags
    unsigned int flags;              // general flags (OPTIMIZE_ALPHA, etc.)
    png_color_16 background;           // background color placeholder
    png_fixed_point background_gamma;  // background gamma
    int background_gamma_type;           // type of gamma source
};

// Typedefs to resemble libpng usage
typedef png_struct* png_structrp;

// Forward declarations of dependencies (stubs for testing)
void png_debug(int level, const char* message) { (void)level; /* no-op in test harness */ }

static int g_png_rtran_ok_value = 1;
static int g_unsupported_gamma_return = 0;
static std::string g_last_png_error;

// Stub implementations for dependencies used by the focal function
int png_rtran_ok(png_structrp png_ptr, int need_IHDR) {
    (void)png_ptr; (void)need_IHDR;
    return g_png_rtran_ok_value;
}

png_fixed_point translate_gamma_flags(png_fixed_point output_gamma, int is_screen) {
    (void)is_screen;
    // For testing, pass gamma through unchanged.
    return output_gamma;
}

int unsupported_gamma(png_structrp png_ptr, png_fixed_point gamma, int warn) {
    (void)png_ptr; (void)gamma; (void)warn;
    return g_unsupported_gamma_return;
}

png_fixed_point png_reciprocal(png_fixed_point gamma) {
    if (gamma == 0) return 0;
    return PNG_FP_1 / gamma;
}

void png_error(png_structrp png_ptr, const char* message) {
    // In the test harness, record the error but do not abort.
    (void)png_ptr;
    g_last_png_error = message ? message : "";
}

// The focal function under test (copied in full, adapted to test harness types)
void png_set_alpha_mode_fixed(png_structrp png_ptr, int mode,
    png_fixed_point output_gamma)
{
{
   png_fixed_point file_gamma;
   int compose = 0;
   png_debug(1, "in png_set_alpha_mode_fixed");
   if (png_rtran_ok(png_ptr, 0) == 0)
      return;
   output_gamma = translate_gamma_flags(output_gamma, 1/*screen*/);
   if (unsupported_gamma(png_ptr, output_gamma, 0))
      return;
   /* The default file gamma is the inverse of the output gamma; the output
    * gamma may be changed below so get the file value first.  The default_gamma
    * is set here and from the simplified API (which uses a different algorithm)
    * so don't overwrite a set value:
    */
   file_gamma = png_ptr->default_gamma;
   if (file_gamma == 0)
   {
      file_gamma = png_reciprocal(output_gamma);
      png_ptr->default_gamma = file_gamma;
   }
   /* There are really 8 possibilities here, composed of any combination
    * of:
    *
    *    premultiply the color channels
    *    do not encode non-opaque pixels
    *    encode the alpha as well as the color channels
    *
    * The differences disappear if the input/output ('screen') gamma is 1.0,
    * because then the encoding is a no-op and there is only the choice of
    * premultiplying the color channels or not.
    *
    * png_set_alpha_mode and png_set_background interact because both use
    * png_compose to do the work.  Calling both is only useful when
    * png_set_alpha_mode is used to set the default mode - PNG_ALPHA_PNG - along
    * with a default gamma value.  Otherwise PNG_COMPOSE must not be set.
    */
   switch (mode)
   {
      case PNG_ALPHA_PNG:        /* default: png standard */
         /* No compose, but it may be set by png_set_background! */
         png_ptr->transformations &= ~PNG_ENCODE_ALPHA;
         png_ptr->flags &= ~PNG_FLAG_OPTIMIZE_ALPHA;
         break;
      case PNG_ALPHA_ASSOCIATED: /* color channels premultiplied */
         compose = 1;
         png_ptr->transformations &= ~PNG_ENCODE_ALPHA;
         png_ptr->flags &= ~PNG_FLAG_OPTIMIZE_ALPHA;
         /* The output is linear: */
         output_gamma = PNG_FP_1;
         break;
      case PNG_ALPHA_OPTIMIZED:  /* associated, non-opaque pixels linear */
         compose = 1;
         png_ptr->transformations &= ~PNG_ENCODE_ALPHA;
         png_ptr->flags |= PNG_FLAG_OPTIMIZE_ALPHA;
         /* output_gamma records the encoding of opaque pixels! */
         break;
      case PNG_ALPHA_BROKEN:     /* associated, non-linear, alpha encoded */
         compose = 1;
         png_ptr->transformations |= PNG_ENCODE_ALPHA;
         png_ptr->flags &= ~PNG_FLAG_OPTIMIZE_ALPHA;
         break;
      default:
         png_error(png_ptr, "invalid alpha mode");
   }
   /* Set the screen gamma values: */
   png_ptr->screen_gamma = output_gamma;
   /* Finally, if pre-multiplying, set the background fields to achieve the
    * desired result.
    */
   if (compose != 0)
   {
      /* And obtain alpha pre-multiplication by composing on black: */
      memset(&png_ptr->background, 0, (sizeof png_ptr->background));
      png_ptr->background_gamma = file_gamma; /* just in case */
      png_ptr->background_gamma_type = PNG_BACKGROUND_GAMMA_FILE;
      png_ptr->transformations &= ~PNG_BACKGROUND_EXPAND;
      if ((png_ptr->transformations & PNG_COMPOSE) != 0)
         png_error(png_ptr,
             "conflicting calls to set alpha mode and background");
      png_ptr->transformations |= PNG_COMPOSE;
   }
}
}

// End of focal function
}

// Simple assertion helpers for non-terminating checks
static std::vector<std::string> g_failures;
#define ASSERT_TRUE(cond, msg) do { if(!(cond)) { g_failures.push_back(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": ASSERT_TRUE failed: " + (msg)); } } while(false)
#define ASSERT_EQ_INT(actual, expected, msg) do { if((actual) != (expected)) { g_failures.push_back(std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": ASSERT_EQ_INT failed: " + (msg) + " (expected " + std::to_string(expected) + ", got " + std::to_string(actual) + ")"); } } while(false)

static void reset_png_struct(png_struct* p) {
    p->default_gamma = 0;
    p->screen_gamma = 0;
    p->transformations = 0;
    p->flags = 0;
    p->background = {0,0,0,0};
    p->background_gamma = 0;
    p->background_gamma_type = 0;
    // clear last error
    g_last_png_error.clear();
}

// Test 1: Default mode PNG_ALPHA_PNG sets no composition or optimize flags, and computes reciprocal gamma
static void test_case_alpha_png_defaults() {
    // Setup
    png_struct* p = new png_struct;
    reset_png_struct(p);

    // Act
    png_set_alpha_mode_fixed(p, PNG_ALPHA_PNG, 32768); // 0.5 in fixed-point

    // Assert
    // default_gamma should be reciprocal of output_gamma (0.5)
    ASSERT_EQ_INT(p->default_gamma, PNG_FP_1 / 32768, "default_gamma should be reciprocal of output_gamma when initial default_gamma==0");
    // screen_gamma should reflect input/output gamma
    ASSERT_EQ_INT(p->screen_gamma, 32768, "screen_gamma should equal the provided output_gamma (0.5)");
    // No changes to encode alpha or optimize flags
    ASSERT_EQ_INT((p->transformations & PNG_ENCODE_ALPHA), 0, "PNG_ENCODE_ALPHA should be cleared for PNG_ALPHA_PNG");
    ASSERT_EQ_INT((p->flags & PNG_FLAG_OPTIMIZE_ALPHA), 0, "PNG_FLAG_OPTIMIZE_ALPHA should be cleared for PNG_ALPHA_PNG");
    // Ensure no composition occurred
    ASSERT_TRUE((p->transformations & PNG_COMPOSE) == 0, "PNG_COMPOSE should not be set for PNG_ALPHA_PNG");
    delete p;
}

// Test 2: PNG_ALPHA_ASSOCIATED enables premultiplication flags and sets screen gamma to 1.0
static void test_case_alpha_associated() {
    png_struct* p = new png_struct;
    reset_png_struct(p);

    png_set_alpha_mode_fixed(p, PNG_ALPHA_ASSOCIATED, 32768); // 0.5 input gamma

    // Expectations
    // default_gamma should be reciprocal of 0.5 -> 2.0
    ASSERT_EQ_INT(p->default_gamma, PNG_FP_1 / 32768, "default_gamma should be reciprocal of output_gamma when initial default_gamma==0 (ASSOCIATED)");
    // screen gamma should be 1.0 (PNG_FP_1)
    ASSERT_EQ_INT(p->screen_gamma, PNG_FP_1, "screen_gamma should be 1.0 (PNG_FP_1) for ASSOCIATED");
    // compose should be set and encoded-alpha and optimize flags cleared
    ASSERT_TRUE((p->transformations & PNG_COMPOSE) != 0, "PNG_COMPOSE should be set when ASSOCIATED");
    ASSERT_TRUE((p->transformations & PNG_ENCODE_ALPHA) == 0, "PNG_ENCODE_ALPHA should be cleared when ASSOCIATED");
    ASSERT_TRUE((p->flags & PNG_FLAG_OPTIMIZE_ALPHA) == 0, "PNG_FLAG_OPTIMIZE_ALPHA should be cleared when ASSOCIATED");
    // background gamma should be reciprocal of output_gamma (0.5) -> 2.0
    ASSERT_EQ_INT(p->background_gamma, PNG_FP_1 / PNG_FP_1, "background_gamma should be reciprocal of input? (check calculation)");
    // However, since output_gamma got set to PNG_FP_1, reciprocal of that is 1/1=1 in fixed-point context; we compute using the code's behavior:
    ASSERT_TRUE(p->background_gamma_type == PNG_BACKGROUND_GAMMA_FILE, "background_gamma_type should be PNG_BACKGROUND_GAMMA_FILE");
    delete p;
}

// Test 3: PNG_ALPHA_OPTIMIZED enables premultiply with optimize flag and preserves gamma semantics
static void test_case_alpha_optimized() {
    png_struct* p = new png_struct;
    reset_png_struct(p);

    png_set_alpha_mode_fixed(p, PNG_ALPHA_OPTIMIZED, 32768); // 0.5

    // Assertions
    ASSERT_TRUE((p->transformations & PNG_ENCODE_ALPHA) == 0, "ENCODE_ALPHA should be cleared when OPTIMIZED");
    ASSERT_TRUE((p->flags & PNG_FLAG_OPTIMIZE_ALPHA) != 0, "OPTIMIZE_ALPHA flag should be set when OPTIMIZED");
    ASSERT_TRUE((p->transformations & PNG_COMPOSE) != 0, "PNG_COMPOSE should be set when OPTIMIZED");
    // screen_gamma should reflect input gamma (unchanged in this simplified model)
    ASSERT_EQ_INT(p->screen_gamma, 32768, "screen_gamma should remain equal to input gamma for OPTIMIZED");
    // background_gamma should be set based on reciprocal of output_gamma (as file gamma)
    ASSERT_EQ_INT(p->background_gamma, PNG_FP_1 / PNG_FP_1, "background_gamma should be reciprocal of output_gamma handled in simplified test");
    delete p;
}

// Test 4: PNG_ALPHA_BROKEN enables encoding of alpha and premultiplication path
static void test_case_alpha_broken() {
    png_struct* p = new png_struct;
    reset_png_struct(p);

    png_set_alpha_mode_fixed(p, PNG_ALPHA_BROKEN, 32768); // 0.5

    // Assertions
    ASSERT_TRUE((p->transformations & PNG_ENCODE_ALPHA) != 0, "ENCODE_ALPHA should be set in BROKEN mode");
    ASSERT_TRUE((p->flags & PNG_FLAG_OPTIMIZE_ALPHA) == 0, "OPTIMIZE_ALPHA should be cleared in BROKEN mode");
    ASSERT_TRUE((p->transformations & PNG_COMPOSE) != 0, "PNG_COMPOSE should be set in BROKEN mode");
    ASSERT_EQ_INT(p->screen_gamma, 32768, "screen_gamma should stay equal to input gamma for BROKEN mode");
    delete p;
}

// Test 5: Early return when rtran_ok reports failure (no modifications should occur)
static void test_case_early_return_rtran() {
    png_struct* p = new png_struct;
    reset_png_struct(p);
    g_png_rtran_ok_value = 0; // force early return

    // Set non-default values to ensure we can detect no changes
    p->default_gamma = 1234;
    p->screen_gamma = 2345;
    p->transformations = 0xABCD;
    p->flags = 0x1234;

    png_set_alpha_mode_fixed(p, PNG_ALPHA_PNG, 32768);

    // After early return, all fields should remain unchanged
    ASSERT_EQ_INT(p->default_gamma, 1234, "default_gamma should remain unchanged after early return");
    ASSERT_EQ_INT(p->screen_gamma, 2345, "screen_gamma should remain unchanged after early return");
    ASSERT_EQ_INT(p->transformations, 0xABCD, "transformations should remain unchanged after early return");
    ASSERT_EQ_INT(p->flags, 0x1234, "flags should remain unchanged after early return");

    g_png_rtran_ok_value = 1; // reset for other tests
    delete p;
}

// Test 6: Early return due to unsupported gamma (no modifications should occur)
static void test_case_early_return_unsupported_gamma() {
    png_struct* p = new png_struct;
    reset_png_struct(p);
    g_unsupported_gamma_return = 1; // force early return on gamma check

    // Initial values
    p->default_gamma = 0;
    p->screen_gamma = 0;
    p->transformations = 0;
    p->flags = 0;

    png_set_alpha_mode_fixed(p, PNG_ALPHA_PNG, 32768);

    // Since gamma is unsupported, fields should remain as initialized
    ASSERT_EQ_INT(p->default_gamma, 0, "default_gamma should remain 0 when gamma unsupported");
    ASSERT_EQ_INT(p->screen_gamma, 0, "screen_gamma should remain 0 when gamma unsupported");
    ASSERT_EQ_INT(p->transformations, 0, "transformations should remain 0 when gamma unsupported");
    ASSERT_EQ_INT(p->flags, 0, "flags should remain 0 when gamma unsupported");

    g_unsupported_gamma_return = 0; // reset
    delete p;
}

// Runner
int main() {
    // Run all defined test cases
    test_case_alpha_png_defaults();
    test_case_alpha_associated();
    test_case_alpha_optimized();
    test_case_alpha_broken();
    test_case_early_return_rtran();
    test_case_early_return_unsupported_gamma();

    // Report results
    if (g_failures.empty()) {
        std::cout << "All tests passed (" << 6 << " tests)." << std::endl;
        return 0;
    } else {
        std::cout << g_failures.size() << " test(s) failed out of 6:" << std::endl;
        for (const auto& s : g_failures) {
            std::cout << " - " << s << std::endl;
        }
        return 1;
    }
}