// Test Suite for the focal method: png_set_text_compression_mem_level
// This test is self-contained and does not depend on GTest. It provides a minimal
// fake environment to exercise the behavior of the function as described in the
// extracted FOCAL_METHOD snippet.
// 
// Step 1 (Program Understanding) - Candidate Keywords (captured in comments):
// - png_struct (png_structrp, pointer to struct with zlib_text_mem_level)
// - zlib_text_mem_level (int) - target field set by the function
// - png_ptr (png_structrp) - input pointer, may be NULL
// - mem_level (int) - value assigned to zlib_text_mem_level
// - png_debug(int, const char*) - diagnostic hook invoked unconditionally
// 
// Step 2 (Unit Test Generation) - Approach:
// - Validate NULL pointer path (ensures early return after a diagnostic call).
// - Validate non-NULL path updates zlib_text_mem_level and calls png_debug once.
// - Validate multiple invocations update the field and accumulate diagnostic calls.
// 
// Step 3 (Domain Knowledge) - Practical notes for C++11 tests:
// - Use simple, non-terminating assertions (custom EXPECT_TRUE macro) to allow
//   multiple tests to run and report failures.
// - Do not rely on private members or private test hooks; use the public-like view
//   exposed via the focal function (zlib_text_mem_level).
// - Keep tests self-contained and compilable with C++11.

#include <iostream>
#include <pngpriv.h>


// A minimal reproduction of the dependent types used by the focal method.
// This is intentionally simple to avoid pulling in the full libpng codebase.
typedef struct png_struct png_struct;
typedef png_struct* png_structrp;

// Minimal definition of the PNG struct that the focal method operates on.
struct png_struct {
    int zlib_text_mem_level;
};

// Forward declaration of the diagnostic hook used by the focal method.
// In the real library this would print or log diagnostic messages.
static int g_debug_call_count = 0;
void png_debug(int /*level*/, const char* /*message*/) {
    // Increment diagnostic call counter to verify that the function invokes it.
    ++g_debug_call_count;
    (void)message; // suppress unused if optimized away
}

// Focal method under test (reproduced to be self-contained for unit tests).
// Signature mirrors the extracted C-like form adapted to C++.
void png_set_text_compression_mem_level(png_structrp png_ptr, int mem_level) {
    // Diagnostic hook as per the original implementation
    png_debug(1, "in png_set_text_compression_mem_level");
    // Guard against NULL input as in the focal method
    if (png_ptr == nullptr)
        return;
    // Core behavior: store the memory level into the png_struct
    png_ptr->zlib_text_mem_level = mem_level;
}

// Simple non-terminating test harness (no GTest) with explanatory comments.
// It uses a lightweight EXPECT_TRUE macro to log failures but continue execution.

static int g_failures = 0;

#define EXPECT_TRUE(cond, msg) \
    do { \
        if (!(cond)) { \
            std::cerr << "Test Failure: " << msg \
                      << " (Line " << __LINE__ << ")" << std::endl; \
            ++g_failures; \
        } \
    } while (0)

// Test 1: Null pointer path
// - Expect: png_debug is called once and function returns without dereferencing the pointer.
void test_null_pointer_behavior() {
    // Reset diagnostic counter
    g_debug_call_count = 0;
    // Call with NULL pointer
    png_set_text_compression_mem_level(nullptr, 42);
    // We expect the diagnostic hook to have been invoked exactly once,
    // despite the pointer being NULL (as shown by the focal method).
    EXPECT_TRUE(g_debug_call_count == 1,
        "png_debug should be called exactly once when png_ptr is NULL");
}

// Test 2: Non-null pointer path updates field
// - Expect: png_debug is called once and zlib_text_mem_level is updated to mem_level.
void test_non_null_pointer_sets_field() {
    png_struct s;
    s.zlib_text_mem_level = -1; // initial sentinel value
    g_debug_call_count = 0;

    int mem_level = 7;
    png_set_text_compression_mem_level(&s, mem_level);

    EXPECT_TRUE(g_debug_call_count == 1,
        "png_debug should be called exactly once for non-null pointer");
    EXPECT_TRUE(s.zlib_text_mem_level == mem_level,
        "zlib_text_mem_level should be updated to mem_level on non-null pointer");
}

// Test 3: Multiple invocations accumulate state and diagnostics
// - Expect: successive calls update the field and produce multiple diagnostic calls.
void test_multiple_invocations() {
    png_struct s2;
    s2.zlib_text_mem_level = -999; // reset to a distinct value
    g_debug_call_count = 0;

    // First call sets to 0
    png_set_text_compression_mem_level(&s2, 0);
    // Second call sets to 9
    png_set_text_compression_mem_level(&s2, 9);

    EXPECT_TRUE(s2.zlib_text_mem_level == 9,
        "zlib_text_mem_level should reflect the last mem_level value (9)");
    EXPECT_TRUE(g_debug_call_count == 2,
        "png_debug should be called on every invocation (2 calls total)");
}

// Helper to run all tests and report overall status
int main() {
    // Run Step 2/3 tests with explanatory comments per unit test
    std::cout << "Starting unit tests for png_set_text_compression_mem_level (self-contained)\n";

    test_null_pointer_behavior();
    test_non_null_pointer_sets_field();
    test_multiple_invocations();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << g_failures << " TEST(S) FAILED" << std::endl;
        return g_failures;
    }
}