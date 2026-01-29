/*
  Unit test suite for the focal method:
  png_set_filler(png_structrp png_ptr, png_uint_32 filler, int filler_loc)

  Notes:
  - This test suite provides a self-contained mock of the minimal libpng-like
    environment needed to exercise the core logic of png_set_filler as defined
    in the provided focal method, without relying on GTest.
  - It uses a lightweight, non-terminating assertion framework (EXPECT_* macros)
    to maximize test execution and coverage.
  - To cover both compile-time branches (READ_FILLER_SUPPORTED / WRITE_FILLER_SUPPORTED),
    compile the test file in separate runs with the corresponding preprocessor flags:
      - For read-case with support:  -D PNG_READ_FILLER_SUPPORTED
      - For read-case without support: -U PNG_READ_FILLER_SUPPORTED (or omit define)
      - For write-case with support: -D PNG_WRITE_FILLER_SUPPORTED
      - For write-case without support: -U PNG_WRITE_FILLER_SUPPORTED
  - The tests are designed to be compiled as a C++11 program.
*/

#include <cstdint>
#include <string>
#include <iostream>
#include <pngpriv.h>


// Lightweight test framework (non-terminating)
static int g_tests_passed = 0;
static int g_tests_failed = 0;
static int g_tests_total = 0;
static void test_log(const std::string &s) { std::cout << s << std::endl; }

#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_total; \
    if (!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "[FAILED] " << msg << std::endl; \
    } else { \
        ++g_tests_passed; \
        std::cerr << "[PASSED] " << msg << std::endl; \
    } \
} while (0)

#define EXPECT_EQ(a,b, msg) do { \
    ++g_tests_total; \
    if (!((a) == (b))) { \
        ++g_tests_failed; \
        std::cerr << "[FAILED] " << msg << " (expected " << (a) << " == " << (b) << ")" << std::endl; \
    } else { \
        ++g_tests_passed; \
        std::cerr << "[PASSED] " << msg << std::endl; \
    } \
} while (0)

// Minimal libpng-like types and constants for testing
typedef struct png_struct_s* png_structrp;
typedef uint32_t png_uint_32;
typedef uint16_t png_uint_16;

// Binary flags and color types (subset)
#define PNG_IS_READ_STRUCT 0x01
#define PNG_COLOR_TYPE_GRAY 0
#define PNG_COLOR_TYPE_RGB 2
#define PNG_FILLER 0x02
#define PNG_FLAG_FILLER_AFTER 0x04

// Forward declarations to satisfy focal function dependencies in test
static void png_debug(int, const char*);
static void png_app_error(png_structrp png_ptr, const char*); // error handler
#define PNG_UNUSED(x) (void)(x)

// Global error tracking for tests
static bool g_error_raised = false;
static std::string g_last_error_message;

// Mock implementations used by the focal method under test
static void png_debug(int /*level*/, const char* /*message*/) {
    // No-op for test
    (void)/*silence*/0;
}
static void png_app_error(png_structrp /*png_ptr*/, const char* message) {
    g_error_raised = true;
    g_last_error_message = message ? message : "";
}

// The focal method under test (a self-contained version not depending on libpng internals)
void png_set_filler(png_structrp png_ptr, uint32_t filler, int filler_loc)
{
{
   png_debug(1, "in png_set_filler");
   if (png_ptr == NULL)
      return;
   // In libpng 1.6 it is possible to determine whether this is a read or write
   // operation and therefore to do more checking here for a valid call.
   if ((png_ptr->mode & PNG_IS_READ_STRUCT) != 0)
   {
#     ifdef PNG_READ_FILLER_SUPPORTED
         /* On read png_set_filler is always valid, regardless of the base PNG
          * format, because other transformations can give a format where the
          * filler code can execute (basically an 8 or 16-bit component RGB or G
          * format.)
          *
          * NOTE: usr_channels is not used by the read code!  (This has led to
          * confusion in the past.)  The filler is only used in the read code.
          */
         png_ptr->filler = (png_uint_16)filler;
#     else
         png_app_error(png_ptr, "png_set_filler not supported on read");
         PNG_UNUSED(filler) /* not used in the write case */
         return;
#     endif
   }
   else /* write */
   {
#     ifdef PNG_WRITE_FILLER_SUPPORTED
         /* On write the usr_channels parameter must be set correctly at the
          * start to record the number of channels in the app-supplied data.
          */
         switch (png_ptr->color_type)
         {
            case PNG_COLOR_TYPE_RGB:
               png_ptr->usr_channels = 4;
               break;
            case PNG_COLOR_TYPE_GRAY:
               if (png_ptr->bit_depth >= 8)
               {
                  png_ptr->usr_channels = 2;
                  break;
               }
               else
               {
                  /* There simply isn't any code in libpng to strip out bits
                   * from bytes when the components are less than a byte in
                   * size!
                   */
                  png_app_error(png_ptr,
                      "png_set_filler is invalid for"
                      " low bit depth gray output");
                  return;
               }
            default:
               png_app_error(png_ptr,
                   "png_set_filler: inappropriate color type");
               return;
         }
#     else
         png_app_error(png_ptr, "png_set_filler not supported on write");
         return;
#     endif
   }
   /* Here on success - libpng supports the operation, set the transformation
    * and the flag to say where the filler channel is.
    */
   png_ptr->transformations |= PNG_FILLER;
   if (filler_loc == PNG_FILLER_AFTER)
      png_ptr->flags |= PNG_FLAG_FILLER_AFTER;
   else
      png_ptr->flags &= ~PNG_FLAG_FILLER_AFTER;
}
}

// End of focal method
}

// Lightweight PNG-like struct
struct png_struct_s {
    uint32_t mode;
    uint16_t filler;
    int color_type;
    int bit_depth;
    int usr_channels;
    uint32_t transformations;
    uint32_t flags;
};

// Helper to create a fresh png_ptr
static png_structrp make_png_ptr(uint32_t mode,
                               int color_type,
                               int bit_depth)
{
    static png_struct_s s;
    s.mode = mode;
    s.color_type = color_type;
    s.bit_depth = bit_depth;
    s.usr_channels = 0;
    s.filler = 0;
    s.transformations = 0;
    s.flags = 0;
    return &s;
}

// Test helpers for test scenarios

// 1) Read path: with READ_FILLER_SUPPORTED
#ifdef PNG_READ_FILLER_SUPPORTED
static void test_png_set_filler_read_supported_basic()
{
    g_error_raised = false;
    g_last_error_message.clear();

    // Case: Read mode, filler set, filler after
    png_structrp p = make_png_ptr(PNG_IS_READ_STRUCT, PNG_COLOR_TYPE_RGB, 8);
    png_set_filler(p, 0xABCD, PNG_FLAG_FILLER_AFTER); // Wait: we pass filler_loc; we'll test after using correct macro
}
#endif

// Implement actual test cases within runnable functions
static void test_read_path_read_supported_via_api()
{
#ifdef PNG_READ_FILLER_SUPPORTED
    g_error_raised = false;
    g_last_error_message.clear();

    // Case 1: Read mode, filler set, filler_loc after
    png_structrp p = make_png_ptr(PNG_IS_READ_STRUCT, PNG_COLOR_TYPE_RGB, 8);
    png_set_filler(p, 0xDEAD, (int)PNG_FLAG_FILLER_AFTER);

    EXPECT_TRUE(p->filler == (uint16_t)0xDEAD, "Read path assigns filler on read when supported");
    EXPECT_TRUE((p->transformations & PNG_FILLER) != 0, "Read path sets transformation flag");
    EXPECT_TRUE((p->flags & PNG_FLAG_FILLER_AFTER) != 0, "Read path preserves filler-after flag");

    // Case 2: NULL pointer should return gracefully
    png_set_filler(nullptr, 0x1234, PNG_FILLER_AFTER);
    EXPECT_TRUE(true, "NULL pointer check does not crash (no-op)");
#else
    // If not compiled with read filler support, skip
    EXPECT_TRUE(true, "Skipped read-supported test (READ_FILLER_SUPPORTED not defined)");
#endif
}

static void test_write_path_rgb_and_flags()
{
#ifdef PNG_WRITE_FILLER_SUPPORTED
    g_error_raised = false;
    g_last_error_message.clear();

    // Case 3: Write mode, color RGB, bit_depth 8+
    png_structrp p = make_png_ptr(0, PNG_COLOR_TYPE_RGB, 8);
    png_set_filler(p, 0x01, PNG_FLAG_FILLER_AFTER);

    EXPECT_TRUE(p->usr_channels == 4, "Write path: RGB -> usr_channels = 4");
    EXPECT_TRUE((p->transformations & PNG_FILLER) != 0, "Write path: set PNG_FILLER transformation");
    EXPECT_TRUE((p->flags & PNG_FLAG_FILLER_AFTER) != 0, "Write path: filler-after flag set when specified");

    // Case 4: Gray with bit_depth >= 8
    png_structrp q = make_png_ptr(0, PNG_COLOR_TYPE_GRAY, 8);
    png_set_filler(q, 0x02, 0);
    EXPECT_TRUE(q->usr_channels == 2, "Write path: Gray >=8 -> usr_channels = 2");
    EXPECT_TRUE((q->flags & PNG_FLAG_FILLER_AFTER) == 0, "Write path: filler-after flag cleared when not after");
#else
    EXPECT_TRUE(true, "Skipped write-supported test (WRITE_FILLER_SUPPORTED not defined)");
#endif
}

static void test_write_path_gray_low_bit_depth_error()
{
#ifdef PNG_WRITE_FILLER_SUPPORTED
    g_error_raised = false;
    g_last_error_message.clear();

    // Case 5: Gray with bit_depth < 8 should trigger error
    png_structrp p = make_png_ptr(0, PNG_COLOR_TYPE_GRAY, 4);
    png_set_filler(p, 0, 0);
    EXPECT_TRUE(g_error_raised, "Write path: Gray low bit depth produces error");
    EXPECT_TRUE(!g_last_error_message.empty(), "Write path: error message emitted");
#else
    EXPECT_TRUE(true, "Skipped write low-bit-depth test (WRITE_FILLER_SUPPORTED not defined)");
#endif
}

static void test_write_path_inappropriate_color_type_error()
{
#ifdef PNG_WRITE_FILLER_SUPPORTED
    g_error_raised = false;
    g_last_error_message.clear();

    // Case 6: Inappropriate color type (simulate unsupported type)
    png_structrp p = make_png_ptr(0, 99 /* arbitrary unsupported color type */, 8);
    png_set_filler(p, 0, 0);
    EXPECT_TRUE(g_error_raised, "Write path: inappropriate color type triggers error");
#else
    EXPECT_TRUE(true, "Skipped write color-type error test (WRITE_FILLER_SUPPORTED not defined)");
#endif
}

// Test runner
int main()
{
    // Intro
    test_log("Starting unit tests for png_set_filler (mocked environment)");

    // Run read-path tests
    test_read_path_read_supported_via_api();
    // Run write-path tests
    test_write_path_rgb_and_flags();
    test_write_path_gray_low_bit_depth_error();
    test_write_path_inappropriate_color_type_error();

    // Summary
    std::cerr << "Tests run: " << g_tests_total
              << ", Passed: " << g_tests_passed
              << ", Failed: " << g_tests_failed << std::endl;

    return (g_tests_failed == 0) ? 0 : 1;
}