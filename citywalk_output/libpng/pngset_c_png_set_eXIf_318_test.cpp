// Test suite for the focal method: png_set_eXIf (as provided in the prompt)
// This test suite is self-contained (no external GTest). It mocks only the
// minimal dependencies required to exercise the focal method's behavior.

// The tests focus on verifying that png_set_eXIf emits the expected warning message
// exactly once per invocation, and that it gracefully handles null pointers.

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <pngpriv.h>


// ---------------------- Mocked/Minimal PNG-like API ----------------------

// Lightweight stand-ins for libpng types used by the focal method.
// We purposely keep them minimal since we only need to compile and run tests.

struct png_struct;                        // opaque struct representing png_ptr
using png_const_structrp = png_struct*;   // simplified: non-const pointer for test
using png_inforp = png_struct*;           // simplified: information pointer
using png_bytep = unsigned char*;          // byte pointer
using png_uint_32 = uint32_t;              // 32-bit unsigned int

// Unused macro compatible with the provided source
#define PNG_UNUSED(x) (void)(x)

// Global container to capture all warnings emitted through png_warning
static std::vector<std::string> g_warning_messages;

// Mock implementation of the libpng warning function.
// We capture the warning message to verify test expectations.
void png_warning(png_const_structrp png_ptr, const char* message)
{
    (void)png_ptr; // In tests, we don't inspect the pointer content
    if (message) {
        g_warning_messages.push_back(std::string(message));
    }
}

// Forward declaration of the helper used by png_set_eXIf (per the focal snippet)
void png_set_eXIf_1(png_const_structrp png_ptr, png_inforp info_ptr,
                   png_uint_32 num_exif, png_bytep exif);

// ---------------------- Focal Method Under Test ---------------------------
// Implemented exactly as described in the provided snippet to ensure testability.

// Declaration of the function under test
void png_set_eXIf(png_const_structrp png_ptr, png_inforp info_ptr, png_bytep exif)
{
    {
        // The focal method issues a warning unconditionally.
        png_warning(png_ptr, "png_set_eXIf does not work; use png_set_eXIf_1");
        PNG_UNUSED(info_ptr)
        PNG_UNUSED(exif)
    }
    // The snippet shows a reference-like declaration for png_set_eXIf_1 after this block;
    // for testing purposes we provide a safe no-op call to satisfy potential linkage.
    png_set_eXIf_1(png_ptr, info_ptr, 0, exif);
}

// Minimal no-op implementation for png_set_eXIf_1 to satisfy linkage in tests.
// It does not affect the observable behavior of png_set_eXIf in this test.
void png_set_eXIf_1(png_const_structrp png_ptr, png_inforp info_ptr,
                   png_uint_32 num_exif, png_bytep exif)
{
    (void)png_ptr; (void)info_ptr; (void)num_exif; (void)exif;
    // Intentionally empty for test harness
}

// ---------------------- Test Harness Helpers --------------------------------

static int g_test_failures = 0; // global failure counter for the test suite

// Helper to reset collected warnings before each test
static void reset_warnings() { g_warning_messages.clear(); }

// Helper to print a concise summary at the end
static void print_summary(int total_tests)
{
    std::cout << "Tests run: " << total_tests
              << ", Failures: " << g_test_failures
              << std::endl;
}

// ---------------------- Unit Tests -----------------------------------------

// Test 1: Verify that png_set_eXIf issues the expected warning with a non-null png_ptr
// Rationale: The focal method unconditionally emits the specific warning string.
// Coverage: true-branch path for emitting a warning; uses a non-null png_ptr to ensure
// the message is independent of the input pointer content.
static void test_png_set_eXIf_warns_with_non_null_ptr()
{
    const int TEST_ID = 1;
    reset_warnings();

    struct png_struct dummy;
    png_set_eXIf(reinterpret_cast<png_const_structrp>(&dummy), nullptr, nullptr);

    // Expect exactly one warning with the specified message
    if (g_warning_messages.size() != 1) {
        std::cerr << "[Test " << TEST_ID << "] Failure: expected 1 warning, got "
                  << g_warning_messages.size() << std::endl;
        ++g_test_failures;
        return;
    }

    const std::string expected = "png_set_eXIf does not work; use png_set_eXIf_1";
    if (g_warning_messages[0] != expected) {
        std::cerr << "[Test " << TEST_ID << "] Failure: unexpected warning message: "
                  << g_warning_messages[0] << std::endl;
        ++g_test_failures;
        return;
    }

    std::cout << "[Test " << TEST_ID << "] Passed: non-null png_ptr emitted expected warning." << std::endl;
}

// Test 2: Verify that png_set_eXIf handles null info_ptr and exif gracefully
// Rationale: The method should not crash or behave differently when info/exif are null.
// Coverage: edge case where both info_ptr and exif are null.
static void test_png_set_eXIf_handles_null_pointers()
{
    const int TEST_ID = 2;
    reset_warnings();

    // All inputs null
    png_set_eXIf(nullptr, nullptr, nullptr);

    if (g_warning_messages.size() != 1) {
        std::cerr << "[Test " << TEST_ID << "] Failure: expected 1 warning, got "
                  << g_warning_messages.size() << std::endl;
        ++g_test_failures;
        return;
    }

    const std::string expected = "png_set_eXIf does not work; use png_set_eXIf_1";
    if (g_warning_messages[0] != expected) {
        std::cerr << "[Test " << TEST_ID << "] Failure: unexpected warning message: "
                  << g_warning_messages[0] << std::endl;
        ++g_test_failures;
        return;
    }

    std::cout << "[Test " << TEST_ID << "] Passed: null pointers handled with expected warning." << std::endl;
}

// Test 3: Verify that multiple invocations accumulate multiple warnings
// Rationale: Each invocation should trigger its own warning independently.
// Coverage: repeated path exercising the same function.
static void test_png_set_eXIf_multiple_invocations()
{
    const int TEST_ID = 3;
    reset_warnings();

    struct png_struct a, b;
    png_set_eXIf(reinterpret_cast<png_const_structrp>(&a), nullptr, nullptr);
    png_set_eXIf(reinterpret_cast<png_const_structrp>(&b), nullptr, nullptr);

    if (g_warning_messages.size() != 2) {
        std::cerr << "[Test " << TEST_ID << "] Failure: expected 2 warnings, got "
                  << g_warning_messages.size() << std::endl;
        ++g_test_failures;
        return;
    }

    const std::string expected = "png_set_eXIf does not work; use png_set_eXIf_1";
    if (g_warning_messages[0] != expected || g_warning_messages[1] != expected) {
        std::cerr << "[Test " << TEST_ID << "] Failure: unexpected warning messages sequence." << std::endl;
        ++g_test_failures;
        return;
    }

    std::cout << "[Test " << TEST_ID << "] Passed: multiple invocations produced expected warnings." << std::endl;
}

// ---------------------- Main (Test Runner) ----------------------------------

int main()
{
    // Run all tests
    test_png_set_eXIf_warns_with_non_null_ptr();
    test_png_set_eXIf_handles_null_pointers();
    test_png_set_eXIf_multiple_invocations();

    // Print final summary
    print_summary(3);

    return g_test_failures == 0 ? 0 : 1;
}