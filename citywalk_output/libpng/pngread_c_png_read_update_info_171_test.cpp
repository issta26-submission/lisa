// This is a C++11 test harness that provides a minimal, self-contained test
// environment for the focal function: png_read_update_info.
// It re-implements the minimal subset of the needed libpng-like API as stubs
// so the test can be compiled and run without the external libpng dependency.
// The tests focus on covering true/false branches of the predicates inside
// png_read_update_info, as requested.

// To test the transform-info path, compile with -DTEST_WITH_TRANSFORMS
// and (optionally) define the compile-time switch PNG_READ_TRANSFORMS_SUPPORTED.
// For testing without transforms, compile without -DTEST_WITH_TRANSFORMS.

// Example command lines:
// 1) With transforms: g++ -std=c++11 -DTEST_WITH_TRANSFORMS test_png_read_update_info.cpp -o test_with_transforms
// 2) Without transforms: g++ -std=c++11 test_png_read_update_info.cpp -o test_without_transforms

#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstring>


// ------------------------
// Minimal libpng-like API stubs (self-contained for testing)
// ------------------------

// Forward declarations to mimic libpng types
struct png_struct;
struct png_info;
typedef png_struct* png_structrp;
typedef png_info* png_inforp;

#define PNG_FLAG_ROW_INIT 0x01

// Global log to capture function calls (for assertions)
static std::vector<std::string> g_log;

// Simple logger
static void log_message(const std::string& s) {
    g_log.push_back(s);
}

// Macros and helpers to mimic libpng behavior in tests
#define PNG_UNUSED(x) (void)(x)

// Diagnostics stub
static void png_debug(int /*level*/, const char* /*message*/) {
    // No-op for testing
}

// api stubs
struct png_struct {
    unsigned int flags;
};

struct png_info {
    // Empty placeholder for test purposes
};

static void png_read_start_row(png_structrp /*png_ptr*/) {
    log_message("start_row");
}

// If transforms are enabled at compile-time, provide the transform hook
#ifdef PNG_READ_TRANSFORMS_SUPPORTED
static void png_read_transform_info(png_structrp /*png_ptr*/, png_inforp /*info_ptr*/) {
    log_message("transform_info");
}
#endif

static void png_app_error(png_structrp /*png_ptr*/, const char* msg) {
    log_message("app_error:");
    // Log the actual message for exact comparison in tests
    if (msg) {
        log_message(std::string(msg));
    }
}

// Utility to align with the original code's conditional branch that does nothing
#ifndef PNG_UNUSED
#define PNG_UNUSED(x) (void)(x)
#endif

// The focal function under test (re-implemented here for unit testing)
png_read_update_info(png_structrp png_ptr, png_inforp info_ptr)
{
{
   png_debug(1, "in png_read_update_info");
   if (png_ptr != NULL)
   {
      if ((png_ptr->flags & PNG_FLAG_ROW_INIT) == 0)
      {
         png_read_start_row(png_ptr);
#        ifdef PNG_READ_TRANSFORMS_SUPPORTED
            png_read_transform_info(png_ptr, info_ptr);
#        else
            PNG_UNUSED(info_ptr)
#        endif
      }
      /* New in 1.6.0 this avoids the bug of doing the initializations twice */
      else
         png_app_error(png_ptr,
             "png_read_update_info/png_start_read_image: duplicate call");
   }
}
}

// ------------------------
// Lightweight test scaffolding (no GTest; uses custom EXPECT_* macros)
// ------------------------

#define EXPECT_TRUE(cond) \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " << #cond << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    }

#define EXPECT_EQ(a, b) \
    if((a) != (b)) { \
        std::cerr << "EXPECT_EQ failed: " << #a << " (" << (a) << ") != " << #b << " (" << (b) << ")" \
                  << " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    }

static void reset_logs() {
    g_log.clear();
}

static void expect_logs_equal(const std::vector<std::string>& expected) {
    if (g_log.size() != expected.size()) {
        std::cerr << "Unexpected log size. Expected " << expected.size()
                  << " entries, got " << g_log.size() << ".\n";
        for (size_t i = 0; i < g_log.size(); ++i) {
            std::cerr << "log[" << i << "]=" << g_log[i] << "\n";
        }
        return;
    }
    for (size_t i = 0; i < expected.size(); ++i) {
        if (g_log[i] != expected[i]) {
            std::cerr << "Log mismatch at " << i << ". Expected: \"" << expected[i]
                      << "\", Got: \"" << g_log[i] << "\"\n";
            return;
        }
    }
}

// ------------------------
// Test Variants
// ------------------------

/*
Test 1: Null pointer path
- Behavior: png_read_update_info should do nothing when png_ptr is NULL.
- Expectation: No logs produced.
*/
static void test_null_ptr_no_action() {
    reset_logs();
    png_read_update_info(nullptr, nullptr);
    EXPECT_TRUE(g_log.empty());
    // Optional: print success marker
    // std::cout << "test_null_ptr_no_action passed\n";
}

/*
Test 2: Non-null pointer with ROW_INIT not set and transforms enabled
- Behavior: should call png_read_start_row and, if transforms supported, png_read_transform_info.
- Expectation: log entries ["start_row", "transform_info"]
*/
#ifdef TEST_WITH_TRANSFORMS
static void test_non_null_start_row_and_transform_with_transforms() {
    reset_logs();
    png_struct s;
    s.flags = 0; // ROW_INIT not set
    png_inforp info_ptr = nullptr;
    png_read_update_info(&s, info_ptr);
    std::vector<std::string> expected = { "start_row", "transform_info" };
    expect_logs_equal(expected);
}
#else
/*
Test 2 (alternative): Non-null pointer with ROW_INIT not set and transforms NOT enabled
- Behavior: should only call png_read_start_row (no transform_info).
- Expectation: log entries ["start_row"]
*/
static void test_non_null_start_row_without_transforms() {
    reset_logs();
    png_struct s;
    s.flags = 0; // ROW_INIT not set
    png_inforp info_ptr = nullptr;
    png_read_update_info(&s, info_ptr);
    std::vector<std::string> expected = { "start_row" };
    expect_logs_equal(expected);
}
#endif

/*
Test 3: Duplicate call path
- Behavior: when ROW_INIT flag is already set, should trigger an app_error with duplicate call message.
- Expectation: logs contain "app_error:" and the specific duplicate message.
*/
static void test_duplicate_call_logs() {
    reset_logs();
    png_struct s;
    s.flags = PNG_FLAG_ROW_INIT; // simulate that initializations already occurred
    png_inforp info_ptr = nullptr;
    png_read_update_info(&s, info_ptr);
    // Expect an app_error followed by the message text
    if (g_log.size() < 2) {
        std::cerr << "test_duplicate_call_logs: expected at least 2 log entries, got "
                  << g_log.size() << "\n";
        return;
    }
    EXPECT_EQ(std::string("app_error:"), g_log[0]);
    // The message is logged as a separate entry in this test harness
    EXPECT_TRUE(g_log[1].find("png_read_update_info/png_start_read_image: duplicate call") != std::string::npos);
}

// ------------------------
// Main driver
// ------------------------

int main() {
    // Test 1: Null pointer path (universal)
    test_null_ptr_no_action();

#ifdef TEST_WITH_TRANSFORMS
    // Test 2a: Non-null pointer with ROW_INIT not set and transforms enabled
    test_non_null_start_row_and_transform_with_transforms();
#else
    // Test 2b: Non-null pointer with ROW_INIT not set and transforms disabled
    test_non_null_start_row_without_transforms();
#endif

    // Test 3: Duplicate call path (should emit an app_error)
    test_duplicate_call_logs();

    // If any EXPECT_* failures occurred, they were printed to stderr.
    // Return code 0 indicates the tests finished; detailed results are on stdout/stderr.
    return 0;
}