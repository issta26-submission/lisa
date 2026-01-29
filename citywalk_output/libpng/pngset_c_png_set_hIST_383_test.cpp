// Minimal unit test suite for the focal method png_set_hIST
// This test harness provides lightweight mocks for a subset of the libpng API
// and validates the behavior of png_set_hIST under several scenarios.
// The tests are written in C++11 and do not rely on GTest. They use a small
// custom test framework with non-terminating assertions to maximize coverage.

#include <vector>
#include <string>
#include <cstdlib>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// ------------------------------------------------------------
// Minimal fake libpng environment to compile and run tests
// ------------------------------------------------------------

// Public API surface (enough to compile and run png_set_hIST)
typedef struct PngStruct* png_const_structrp; // opaque in tests
typedef struct PngInfo* png_inforp;
typedef const unsigned short* png_const_uint_16p;
typedef unsigned short* png_uint_16p;

#define PNG_MAX_PALETTE_LENGTH 256
#define PNG_FREE_HIST 0x01
#define PNG_INFO_hIST 0x02

// Global hooks to simulate libpng behavior in tests
static bool g_memory_fail = false;
static std::vector<std::string> g_warnings;

// Utility: reset global test state
static void test_reset_state() {
    g_memory_fail = false;
    g_warnings.clear();
}

// Utility: record a warning (mimics libpng warning path)
static void png_warning(png_const_structrp /*png_ptr*/, const char* msg) {
    g_warnings.push_back(std::string(msg));
}

// Utility: fake memory allocator with optional failure
static void* png_malloc_warn(png_const_structrp /*png_ptr*/, size_t size) {
    if (g_memory_fail) {
        return nullptr;
    }
    return std::malloc(size);
}

// Utility: free data (only hist is managed in tests)
typedef struct PngInfo {
    int num_palette;
    unsigned int free_me;
    unsigned int valid;
    png_uint_16p hist;
} PngInfo;

static void png_free_data(png_const_structrp /*png_ptr*/, png_inforp info_ptr,
                          int free_mode, int /*flag*/) {
    if (info_ptr != nullptr && info_ptr->hist != nullptr) {
        if (free_mode == PNG_FREE_HIST) {
            std::free(info_ptr->hist);
            info_ptr->hist = nullptr;
        }
    }
}

// Utility: no-op debug (kept for compatibility with original API)
static void png_debug1(int /*level*/, const char* /*format*/, const char* /*arg*/) {
    // do nothing in test harness
}

// Helper macro to cast void* to desired pointer type
#define png_voidcast(type, expr) ((type)(expr))

// Forward declaration of focal method (as provided in the prompt)
static void png_set_hIST(png_const_structrp png_ptr, png_inforp info_ptr,
                        png_const_uint_16p hist);

// Minimal dummy png_ptr type to satisfy function signatures
typedef struct PngStruct {
    // Intentionally empty; the real library would hold many fields.
} PngStruct;

// Implementation of the focal method with the same logic as provided
static void png_set_hIST(png_const_structrp png_ptr, png_inforp info_ptr,
                        png_const_uint_16p hist)
{
{
   int i;
   png_debug1(1, "in %s storage function", "hIST");
   if (png_ptr == NULL || info_ptr == NULL)
      return;
   if (info_ptr->num_palette == 0 || info_ptr->num_palette
       > PNG_MAX_PALETTE_LENGTH)
   {
      png_warning(png_ptr,
          "Invalid palette size, hIST allocation skipped");
      return;
   }
   png_free_data(png_ptr, info_ptr, PNG_FREE_HIST, 0);
   /* Changed from info->num_palette to PNG_MAX_PALETTE_LENGTH in
    * version 1.2.1
    */
   info_ptr->hist = png_voidcast(png_uint_16p, png_malloc_warn(png_ptr,
       PNG_MAX_PALETTE_LENGTH * (sizeof (png_uint_16))));
   if (info_ptr->hist == NULL)
   {
      png_warning(png_ptr, "Insufficient memory for hIST chunk data");
      return;
   }
   for (i = 0; i < info_ptr->num_palette; i++)
      info_ptr->hist[i] = hist[i];
   info_ptr->free_me |= PNG_FREE_HIST;
   info_ptr->valid |= PNG_INFO_hIST;
}
}

// ------------------------------------------------------------
// Test framework (lightweight, non-terminating assertions)
// ------------------------------------------------------------

#define TEST_OK(cond, msg) do { if (!(cond)) { \
    std::cerr << "TEST FAILED: " << (msg) << "\n"; \
} } while(false)

#define ASSERT_TRUE(cond, msg) TEST_OK((cond), (msg))

// Test helper: create a fresh PngInfo
static PngInfo* make_info() {
    PngInfo* p = (PngInfo*)std::calloc(1, sizeof(PngInfo));
    p->num_palette = 0;
    p->free_me = 0;
    p->valid = 0;
    p->hist = nullptr;
    return p;
}

// Test 1: Null pointers should cause no crash and no changes
static void test_null_pointers() {
    test_reset_state();
    // Call with null png_ptr
    PNG_FREE_HIST; // dummy to avoid unused macro warnings
    png_const_uint_16p dummy_hist = nullptr;
    png_set_hIST(nullptr, nullptr, dummy_hist);

    // Expect no warnings
    ASSERT_TRUE(g_warnings.empty(), "No warnings expected when both pointers are null");
}

// Test 2: info_ptr->num_palette == 0 triggers warning and returns
static void test_zero_palette_triggers_warning() {
    test_reset_state();
    PngStruct dummy_ptr;
    PngInfo* info = make_info();

    info->num_palette = 0; // invalid
    unsigned short hist_input[1] = { 123 };
    png_set_hIST(reinterpret_cast<png_const_structrp>(&dummy_ptr),
                info,
                hist_input);

    ASSERT_TRUE(!info->hist, "hist should remain NULL when palette size invalid");
    ASSERT_TRUE(!((info->free_me) & PNG_FREE_HIST), "PNG_FREE_HIST should not be set when palette invalid");
    ASSERT_TRUE(((info->valid) & PNG_INFO_hIST) == 0, "PNG_INFO_hIST should not be set when palette invalid");
    ASSERT_TRUE(!g_warnings.empty(), "A warning should be emitted for invalid palette size");
}

// Test 3: info_ptr->num_palette > PNG_MAX_PALETTE_LENGTH triggers warning
static void test_palette_exceeds_max_triggers_warning() {
    test_reset_state();
    PngStruct dummy_ptr;
    PngInfo* info = make_info();

    info->num_palette = PNG_MAX_PALETTE_LENGTH + 1; // invalid
    unsigned short hist_input[1] = { 1 };
    png_set_hIST(reinterpret_cast<png_const_structrp>(&dummy_ptr),
                info,
                hist_input);

    ASSERT_TRUE(!info->hist, "hist should be NULL when palette size exceeds max");
    ASSERT_TRUE(((info->valid) & PNG_INFO_hIST) == 0, "PNG_INFO_hIST should not be set when palette invalid");
    ASSERT_TRUE(!g_warnings.empty(), "A warning should be emitted for palette too large");
}

// Test 4: Allocation failure leads to warning and no hist allocated
static void test_allocation_failure_handling() {
    test_reset_state();
    PngStruct dummy_ptr;
    PngInfo* info = make_info();

    info->num_palette = 2; // valid size
    unsigned short hist_input[2] = { 5, 7 };

    // Simulate allocation failure
    g_memory_fail = true;

    png_set_hIST(reinterpret_cast<png_const_structrp>(&dummy_ptr),
                info,
                hist_input);

    // hist should remain NULL
    ASSERT_TRUE(info->hist == nullptr, "hist should be NULL when allocation fails");
    ASSERT_TRUE(g_warnings.size() >= 1, "Warning should be emitted on allocation failure");
}

// Test 5: Successful allocation copies data and sets flags
static void test_successful_hIST_copy_and_flags() {
    test_reset_state();
    PngStruct dummy_ptr;
    PngInfo* info = make_info();

    info->num_palette = 3; // valid
    unsigned short hist_input[3] = { 10, 20, 30 };

    png_set_hIST(reinterpret_cast<png_const_structrp>(&dummy_ptr),
                info,
                hist_input);

    // Validate allocation happened
    ASSERT_TRUE(info->hist != nullptr, "hist should be allocated on success");
    // Validate copied data
    ASSERT_TRUE(info->hist[0] == 10 && info->hist[1] == 20 && info->hist[2] == 30,
                "hist data should be copied for first three entries");
    // Validate flags
    ASSERT_TRUE((info->free_me & PNG_FREE_HIST) != 0, "PNG_FREE_HIST flag should be set after successful hIST");
    ASSERT_TRUE((info->valid & PNG_INFO_hIST) != 0, "PNG_INFO_hIST flag should be set after successful hIST");
    // No warnings expected
    ASSERT_TRUE(g_warnings.empty(), "No warnings expected on successful allocation");
}

// Run all tests
static void run_all_tests() {
    test_null_pointers();
    test_zero_palette_triggers_warning();
    test_palette_exceeds_max_triggers_warning();
    test_allocation_failure_handling();
    test_successful_hIST_copy_and_flags();

    // Summary
    std::cout << "Tests executed. Warnings captured: " << g_warnings.size() << "\n";
}

// Entry point
int main() {
    run_all_tests();

    // Optional: print detailed warnings if any
    if (!g_warnings.empty()) {
        std::cout << "Warnings:\n";
        for (const auto& w : g_warnings) {
            std::cout << "  - " << w << "\n";
        }
    }
    return 0;
}