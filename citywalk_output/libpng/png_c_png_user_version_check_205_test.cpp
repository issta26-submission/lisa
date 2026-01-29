// C++11 test suite for the focal method: png_user_version_check
// Approach:
// - Include the png.c implementation in C linkage to test the real logic
// - Provide minimal mock scaffolding (types, macros, and helper functions)
// - Exercise NULL and non-NULL inputs, matching and mismatching version strings
// - Validate return values and the presence/absence of library-mismatch flag
// - Capture warning messages when enabled via PNG_WARNINGS_SUPPORTED
//
// Note: This test suite is designed to be built in a project where png.c is accessible.
// It uses a lightweight custom test harness without any external testing framework.

#include <string>
#include <iostream>
#include <pngpriv.h>
#include <png.c>
#include <cstring>


// Domain-driven constants and helpers (mocked for testing png_user_version_check)
#define PNG_FLAG_LIBRARY_MISMATCH 0x01
#define PNG_LIBPNG_VER_STRING "1.6.37"
#define PNG_WARNINGS_SUPPORTED 1
#define PNG_UNUSED(x) (void)(x)

// Mock minimal png_struct used by the focal function
typedef struct png_struct {
    unsigned int flags;
} png_struct;
typedef png_struct* png_structrp;
typedef const char* png_const_charp;

// Forward declarations matching expected signatures inside the test environment
extern "C" {
    // The actual png_user_version_check will be pulled in by including png.c
    int png_user_version_check(png_structrp png_ptr, png_const_charp user_png_ver);

    // Mocked helpers expected by png_user_version_check (provided in test harness)
    size_t png_safecat(char *dest, size_t size, size_t pos, const char *src);
    void png_warning(png_structrp png_ptr, const char *message);
}

// Global to capture warnings for verification
static std::string last_warning_message;

// Mock implementation of png_warning to capture emitted messages
extern "C" void png_warning(png_structrp png_ptr, const char* message)
{
    if (message != nullptr) {
        last_warning_message.assign(message);
    } else {
        last_warning_message.clear();
    }
}

// Simple safe-cat helper used by the focal function when warnings are active
extern "C" size_t png_safecat(char *dest, size_t size, size_t pos, const char *src)
{
    if (src == nullptr || dest == nullptr || size == 0) {
        return pos;
    }
    size_t i = 0;
    while (pos < size - 1 && src[i] != '\0') {
        dest[pos++] = src[i++];
    }
    if (pos < size) {
        dest[pos] = '\0';
    }
    return pos;
}

// Tests will compile the real png_user_version_check via png.c included in C linkage
extern "C" {
}

// Simple non-terminating assertion helper
#define ASSERT_TRUE(cond, msg) \
    do { if (!(cond)) { std::cout << "[ FAIL ] " << (msg) << "\n"; failures++; } else { /* test passes silently to maximize coverage */ } } while (0)
#define ASSERT_EQ(a, b, msg) \
    do { if ((a) != (b)) { std::cout << "[ FAIL ] " << (msg) << " (expected " << (a) << " == " << (b) << ")\n"; failures++; } } while (0)

// Global test counters
static int tests_run = 0;
static int failures = 0;

// Helper to reset warning capture before each test
void reset_warning_capture() {
    last_warning_message.clear();
}

// Test 1: NULL user_png_ver should cause a library mismatch flag and return 0
// This validates the else-branch of the initial if (user_png_ver != NULL) check.
bool test_null_user_png_ver_behaviour()
{
    // Prepare a pseudo-png_ptr with zeroed flags
    png_struct instance;
    instance.flags = 0;
    png_structrp ptr = &instance;

    reset_warning_capture();

    int ret = png_user_version_check(ptr, NULL);

    // Expect mismatch due to NULL input and return value 0
    ASSERT_TRUE(ret == 0, "png_user_version_check should return 0 when user_png_ver is NULL");
    // Flags should indicate a library mismatch
    ASSERT_TRUE((ptr->flags & PNG_FLAG_LIBRARY_MISMATCH) != 0, "Library mismatch flag should be set for NULL user_png_ver");
    // A warning is emitted when mismtach occurs (if enabled)
    ASSERT_TRUE(!last_warning_message.empty(), "Warning message should be emitted when library mismatch occurs");
    return ret == 0;
}

// Test 2: Matching version string should return success (1) and no mismatch
bool test_matching_version_string_behaviour()
{
    png_struct instance;
    instance.flags = 0;
    png_structrp ptr = &instance;

    reset_warning_capture();

    // Use a version string that exactly matches PNG_LIBPNG_VER_STRING up to two dots
    const char* user_ver = PNG_LIBPNG_VER_STRING;
    int ret = png_user_version_check(ptr, user_ver);

    ASSERT_TRUE(ret == 1, "png_user_version_check should return 1 for matching version string");
    ASSERT_TRUE((ptr->flags & PNG_FLAG_LIBRARY_MISMATCH) == 0, "Library mismatch flag should remain unset for matching version");
    // No warning should be emitted on a correct version
    if (!last_warning_message.empty()) {
        std::cout << "[ WARN ] Unexpected warning for matching version: " << last_warning_message << "\n";
        failures++;
    }
    return ret == 1;
}

// Test 3: Mismatching version string should return 0 and set mismatch flag
bool test_mismatching_version_string_behaviour()
{
    png_struct instance;
    instance.flags = 0;
    png_structrp ptr = &instance;

    reset_warning_capture();

    // Create a version string with an obvious mismatch (first differing character after '1')
    const char* user_ver = "2.0.0";
    int ret = png_user_version_check(ptr, user_ver);

    ASSERT_TRUE(ret == 0, "png_user_version_check should return 0 for mismatching version string");
    ASSERT_TRUE((ptr->flags & PNG_FLAG_LIBRARY_MISMATCH) != 0, "Library mismatch flag should be set on mismatch");
    // Warning should be emitted when mismatch occurs
    ASSERT_TRUE(!last_warning_message.empty(), "Warning message should be emitted on mismatch");
    return ret == 0;
}

// Test 4: Ensure no crash and proper behavior when user_png_ver is shorter than library version
bool test_shorter_version_string_behaviour()
{
    png_struct instance;
    instance.flags = 0;
    png_structrp ptr = &instance;

    reset_warning_capture();

    // Shorter version string (first two components still match)
    const char* user_ver = "1.6";
    int ret = png_user_version_check(ptr, user_ver);

    // Should consider compatible as per implementation (no mismatch flag set)
    ASSERT_TRUE(ret == 1, "png_user_version_check should return 1 when user version is a prefix (shorter) but matches up to available characters");
    ASSERT_TRUE((ptr->flags & PNG_FLAG_LIBRARY_MISMATCH) == 0, "Library mismatch flag should remain unset for prefix match");
    return ret == 1;
}

// Main test runner
int main() {
    std::cout << "Starting png_user_version_check unit tests (C++ harness)\n";

    // Run tests
    tests_run = 0;
    failures = 0;

    tests_run++;
    if (test_null_user_png_ver_behaviour()) {
        // pass
    } else {
        // failure handled inside test
    }

    tests_run++;
    if (test_matching_version_string_behaviour()) {
        // pass
    }

    tests_run++;
    if (test_mismatching_version_string_behaviour()) {
        // pass
    }

    tests_run++;
    if (test_shorter_version_string_behaviour()) {
        // pass
    }

    // Summary
    std::cout << "Tests run: " << tests_run << ", Failures: " << failures << "\n";
    if (failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cout << "Some tests failed.\n";
        return 1;
    }
}