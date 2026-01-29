// Minimal, self-contained unit-test for the focal method: png_set_read_user_transform_fn
// This test is designed to compile as C++11 code without GoogleTest.
// It uses a lightweight, runtime assertion approach and conditionally exercises
// the true/false branches of the compile-time guard PNG_READ_USER_TRANSFORM_SUPPORTED.
// To exercise the true branch, compile with -DPNG_READ_USER_TRANSFORM_SUPPORTED.
// To exercise the false branch, compile without that definition.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>


// Candidate Keywords (for reference in understanding dependencies):
// - png_struct, png_structrp, png_user_transform_ptr
// - PNG_USER_TRANSFORM flag
// - png_debug and PNG_READ_USER_TRANSFORM_SUPPORTED macro
// - read_user_transform_fn pointer assignment

// Minimal stand-ins to mimic the needed parts of the focal code environment.
struct png_struct;
typedef png_struct* png_structrp;
typedef void (*png_user_transform_ptr)(void*);

// Lightweight mock of the png_struct used by the focal function
struct png_struct {
    int transformations;
    png_user_transform_ptr read_user_transform_fn;
};

// A no-op debug function to mimic the original's behavior without producing I/O during tests
void png_debug(int level, const char* /*message*/) {
    // Intentionally empty to avoid I/O in unit tests
    (void)level;
}

// Symbol required by the focal function
const int PNG_USER_TRANSFORM = 0x01;

// Forward declaration of the focal function under test
png_set_read_user_transform_fn(png_structrp png_ptr, png_user_transform_ptr read_user_transform_fn);

// Implementation of the focal function under test
png_set_read_user_transform_fn(png_structrp png_ptr,
    png_user_transform_ptr read_user_transform_fn)
{
{
   png_debug(1, "in png_set_read_user_transform_fn");
#ifdef PNG_READ_USER_TRANSFORM_SUPPORTED
   png_ptr->transformations |= PNG_USER_TRANSFORM;
   png_ptr->read_user_transform_fn = read_user_transform_fn;
#endif
}
}

// Lightweight test harness
static int test_failures = 0;
#define TEST_FAIL(msg) do { std::cerr << "Test failure: " << (msg) << "\n"; ++test_failures; } while(0)
#define ASSERT_TRUE(cond, msg) do { if(!(cond)) { TEST_FAIL(msg); } } while(0)
#define ASSERT_EQ(a, b, msg) do { if((a) != (b)) { TEST_FAIL(msg); } } while(0)

// A sample user transform function to use in tests
void my_transform(void* data) {
    // Simple no-op transform placeholder
    (void)data;
}

// Run test for the "true" branch (when PNG_READ_USER_TRANSFORM_SUPPORTED is defined)
static void test_true_branch_impl() {
    // Initialize a fake png_ptr
    png_struct png;
    png_structrp png_ptr = &png;
    png_ptr->transformations = 0;
    png_ptr->read_user_transform_fn = nullptr;

    // Call the focal function with a non-null function pointer
    png_set_read_user_transform_fn(png_ptr, my_transform);

    // Expect the true-branch side effects to occur
    ASSERT_TRUE((png_ptr->transformations & PNG_USER_TRANSFORM) != 0,
                "PNG_USER_TRANSFORM flag should be set when supported");
    ASSERT_TRUE(png_ptr->read_user_transform_fn == my_transform,
                "read_user_transform_fn should be updated to the provided function");
}

// Run test for the "false" branch (when PNG_READ_USER_TRANSFORM_SUPPORTED is NOT defined)
static void test_false_branch_impl() {
    // Initialize a fake png_ptr
    png_struct png;
    png_structrp png_ptr = &png;
    png_ptr->transformations = 0;
    png_ptr->read_user_transform_fn = nullptr;

    // Call the focal function with a non-null function pointer
    png_set_read_user_transform_fn(png_ptr, my_transform);

    // Expect no changes, since the feature is not compiled in
    ASSERT_TRUE((png_ptr->transformations & PNG_USER_TRANSFORM) == 0,
                "PNG_USER_TRANSFORM flag should not be set when not supported");
    ASSERT_TRUE(png_ptr->read_user_transform_fn == nullptr,
                "read_user_transform_fn should remain unchanged when not supported");
}

// Entry point
int main() {
#if defined(PNG_READ_USER_TRANSFORM_SUPPORTED)
    // True-branch test: compile-time define is enabled
    test_true_branch_impl();
#else
    // False-branch test: compile-time define is NOT enabled
    test_false_branch_impl();
#endif

    if (test_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << test_failures << " TEST(S) FAILED\n";
        return test_failures;
    }
}