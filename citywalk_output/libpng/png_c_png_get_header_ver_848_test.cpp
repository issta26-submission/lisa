// Test suite for the focal method: png_get_header_ver
// This test is self-contained and uses a lightweight, non-terminating assertion approach
// suitable for environments where GTest is not allowed.
// The test assumes the existence of the library's header-provided macro PNG_LIBPNG_VER_STRING
// and the symbol png_get_header_ver with C linkage.
// It compiles with C++11.

#include <iostream>
#include <pngpriv.h>
#include <cstring>


// Forward declaration of the focal function.
// We declare it with a generic void* parameter to avoid tight coupling with the exact libpng type
// since the function's implementation does not dereference the input pointer.
extern "C" const char* png_get_header_ver(void* png_ptr);

// Lightweight non-terminating assertion helpers
static int g_test_failures = 0;

#define EXPECT_STREQ(actual, expected)                                               \
    do {                                                                               \
        if ((actual) == nullptr || (expected) == nullptr ||                          \
            std::strcmp((actual), (expected)) != 0) {                                \
            std::cerr << "EXPECT_STREQ failed: actual=\"" << (actual)                 \
                      << "\", expected=\"" << (expected) << "\"\n";                   \
            ++g_test_failures;                                                       \
        }                                                                            \
    } while (0)

#define EXPECT_TRUE(cond, message)                                                    \
    do {                                                                                \
        if (!(cond)) {                                                                 \
            std::cerr << "EXPECT_TRUE failed: " << (message) << "\n";                \
            ++g_test_failures;                                                         \
        }                                                                               \
    } while (0)


// Test 1: Return value matches PNG_LIBPNG_VER_STRING when passed a nullptr.
// Rationale: png_get_header_ver should ignore its argument and return the libpng version string.
static void test_png_get_header_ver_with_nullptr() {
    // Comment: Validate that nullptr still yields the correct version string.
    const char* ver = png_get_header_ver(nullptr);
    EXPECT_STREQ(ver, PNG_LIBPNG_VER_STRING);
    // Comment: Ensure the returned string is non-empty as a basic sanity check.
    EXPECT_TRUE(ver != nullptr && std::strlen(ver) > 0, "version string should be non-empty");
}

// Test 2: Return value matches PNG_LIBPNG_VER_STRING when passed a non-null pointer.
// Rationale: Exercise the function with a typical non-null argument to ensure no special-case behavior arises.
static void test_png_get_header_ver_with_nonnull_ptr() {
    int dummy = 0;
    void* p = &dummy;
    const char* ver = png_get_header_ver(p);
    EXPECT_STREQ(ver, PNG_LIBPNG_VER_STRING);
}

// Test 3: The function should be side-effect free w.r.t. its input (no dereferencing of png_ptr).
// We test by passing an invalid pointer value (not used) and ensuring no crash and correct return.
// Note: This is a basic sanity check; the function is expected not to use the pointer.
static void test_png_get_header_ver_no_pointer_usage() {
    // Use an obviously invalid address (may be ignored by the function)
    void* invalid_ptr = reinterpret_cast<void*>(0xDEADBEEF);
    const char* ver = png_get_header_ver(invalid_ptr);
    EXPECT_STREQ(ver, PNG_LIBPNG_VER_STRING);
}


// Main test runner
int main() {
    // Execute tests
    test_png_get_header_ver_with_nullptr();
    test_png_get_header_ver_with_nonnull_ptr();
    test_png_get_header_ver_no_pointer_usage();

    if (g_test_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_test_failures << " TEST(S) FAILED\n";
        return 2; // non-zero to indicate test failures
    }
}