// Unit test suite for png_push_have_end in a self-contained, GTest-free setup.
// This file provides minimal PNG-related types and the function under test.
// The tests focus on the behavior of the end_fn callback invocation logic.

#include <iostream>
#include <pngpriv.h>
#include <cassert>
#include <cstddef>


// Domain knowledge mappings and candidate keywords (for reference):
// - Candidate Keywords: png_ptr, end_fn, info_ptr, png_push_have_end, function pointer callbacks
// - Key behavior: If png_ptr->end_fn != NULL, call end_fn(png_ptr, info_ptr)


// Step 1: Minimal type definitions to mirror the relevant parts of the real PNG library.
// We implement only what is necessary for testing png_push_have_end.

struct png_struct;
struct png_info;

// Typedefs to resemble the real library's naming conventions
typedef png_struct* png_structrp;
typedef png_info* png_inforp;
typedef void (*png_end_fn)(png_structrp, png_inforp);

// Minimal struct definitions with only the fields needed by the focal function.
struct png_struct {
    png_end_fn end_fn; // Callback to be invoked at end of processing
};

struct png_info {
    int dummy; // Placeholder for additional info data
};

// Forward declaration of the function under test (as in the real library)
void png_push_have_end(png_structrp png_ptr, png_inforp info_ptr);

// Step 2: Implementation of the focal function under test (as provided in source)
void png_push_have_end(png_structrp png_ptr, png_inforp info_ptr)
{
{
   if (png_ptr->end_fn != NULL)
      (*(png_ptr->end_fn))(png_ptr, info_ptr);
}
}

// Global test state to support non-terminating assertions
struct TestContext {
    int failures;
    void fail(const char* msg) {
        ++failures;
        std::cerr << "Failure: " << msg << std::endl;
    }
};

// Global test context instance
static TestContext gTest{0};

// Convenience assertion macros (non-terminating, to maximize coverage)
#define EXPECT_TRUE(cond, msg) do { if(!(cond)) gTest.fail(msg); } while(0)
#define EXPECT_EQ(a, b, msg) do { if(((a) != (b))) gTest.fail(msg); } while(0)
#define EXPECT_PTR_EQ(a, b, msg) do { if(((void*)(a)) != ((void*)(b))) gTest.fail(msg); } while(0)

// Global variables to capture end_fn invocation details
static int g_end_call_count = 0;
static png_struct* g_last_png_ptr = nullptr;
static png_inforp g_last_info_ptr = nullptr;

// Test callback to verify end_fn invocation and argument integrity
static void test_end_fn(png_structrp ptr, png_inforp info)
{
    ++g_end_call_count;
    g_last_png_ptr = ptr;
    g_last_info_ptr = info;
}

// Step 3: Test cases

// Test 1: end_fn is NULL; ensure no invocation happens and internal state remains untouched.
void test_png_push_have_end_endfn_null_not_called()
{
    // Arrange
    g_end_call_count = 0;
    g_last_png_ptr = nullptr;
    g_last_info_ptr = nullptr;

    png_struct test_ptr;
    test_ptr.end_fn = nullptr; // End function is intentionally NULL

    png_info info;
    // Act
    png_push_have_end(&test_ptr, &info);

    // Assert
    EXPECT_EQ(g_end_call_count, 0, "end_fn should not be called when end_fn == NULL");
    EXPECT_PTR_EQ(g_last_png_ptr, nullptr, "end_fn should not receive a png_ptr when end_fn == NULL");
    EXPECT_PTR_EQ(g_last_info_ptr, nullptr, "end_fn should not receive an info_ptr when end_fn == NULL");
}

// Test 2: end_fn is non-NULL; verify that it is called exactly once and receives correct arguments.
void test_png_push_have_end_endfn_called_with_correct_args()
{
    // Arrange
    g_end_call_count = 0;
    g_last_png_ptr = nullptr;
    g_last_info_ptr = nullptr;

    png_struct test_ptr;
    test_ptr.end_fn = test_end_fn; // Non-NULL callback

    png_info info;

    // Act
    png_push_have_end(&test_ptr, &info);

    // Assert
    EXPECT_EQ(g_end_call_count, 1, "end_fn should be called exactly once when non-NULL");
    EXPECT_PTR_EQ(g_last_png_ptr, &test_ptr, "end_fn should receive the correct png_ptr argument");
    EXPECT_PTR_EQ(g_last_info_ptr, &info, "end_fn should receive the correct info_ptr argument");
}

// Optional: Additional test to ensure multiple invocations behave consistently
void test_png_push_have_end_multiple_calls()
{
    // Arrange
    g_end_call_count = 0;
    g_last_png_ptr = nullptr;
    g_last_info_ptr = nullptr;

    png_struct test_ptr;
    test_ptr.end_fn = test_end_fn;

    png_info info1;
    PNG_INFO_UNUSED: (void)info1; // suppress unused warning if compiled differently
    png_inforp info_ptrs[2] = { &info1, &info1 };

    // Act: call twice
    png_push_have_end(&test_ptr, &info1);
    png_push_have_end(&test_ptr, &info1);

    // Assert
    EXPECT_EQ(g_end_call_count, 2, "end_fn should be called twice after two invocations");
    EXPECT_PTR_EQ(g_last_png_ptr, &test_ptr, "end_fn should always receive the correct png_ptr");
    EXPECT_PTR_EQ(g_last_info_ptr, &info1, "end_fn should receive the correct info_ptr for the last call");
}

// Utility macro to silence unused warnings in test 3 if needed
#define PNG_INFO_UNUSED \
    do { } while(0)

// Main entry to run tests from C++ main (no GTest)
int main()
{
    std::cout << "Running png_push_have_end unit tests (no GTest)..." << std::endl;

    // Run tests
    test_png_push_have_end_endfn_null_not_called();
    test_png_push_have_end_endfn_called_with_correct_args();
    // Optional: uncomment to run the additional test
    // test_png_push_have_end_multiple_calls();

    // Summary
    int total_tests = 2; // Update if additional tests are enabled
    std::cout << "Tests executed: " << total_tests << "\nFailures: " << gTest.failures << std::endl;

    // Return non-zero if any test failed
    return gTest.failures > 0 ? 1 : 0;
}