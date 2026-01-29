// Minimal, self-contained unit test suite for the focal function png_set_strip_alpha
// This test harness provides lightweight stubs for the necessary types and functions
// to exercise the logic of png_set_strip_alpha without relying on the full libpng build.
//
// Key goal: cover true/false branches of the predicate and idempotence behavior.
// Non-terminating assertions are used to maximize coverage without aborting on first failure.

#include <cstdint>
#include <iostream>
#include <pngpriv.h>


// Domain-specific notes implemented in this test:
// - We simulate the libpng environment with a minimal png_struct having a 'transformations' field.
// - We provide a mock for png_rtran_ok to control the control flow of png_set_strip_alpha.
// - We provide a no-op png_debug to mirror the real function's side effects minimally.
// - We implement png_set_strip_alpha as described in the focal method (including the debug call).
// - We use non-terminating assertions to collect all failures in one run.

struct png_struct {
    uint32_t transformations;
};

typedef png_struct* png_structrp;

static int mock_png_rtran_ok_return = 1; // Control return value of the mock, 0 = false, non-zero = true

// Mocked dependency: version of png_rtran_ok used by png_set_strip_alpha
extern "C" int png_rtran_ok(png_structrp png_ptr, int need_IHDR)
{
    (void)png_ptr;
    (void)need_IHDR;
    return mock_png_rtran_ok_return;
}

// Stubbed debug function to match the focal code's call
extern "C" void png_debug(int level, const char* msg)
{
    (void)level;
    (void)msg;
    // no-op in test environment
}

// Implementation of the focal method under test (copied/adapted for isolated unit testing)
extern "C" void png_set_strip_alpha(png_structrp png_ptr)
{
    {
        png_debug(1, "in png_set_strip_alpha");
        if (png_rtran_ok(png_ptr, 0) == 0)
            return;
        png_ptr->transformations |= 0x01; // PNG_STRIP_ALPHA = 0x01
    }
}

// Lightweight, non-terminating test framework
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define ASSERT_TRUE(cond, msg) \
    do { \
        ++g_total_tests; \
        if (!(cond)) { \
            std::cerr << "[TEST FAILED] " << msg << std::endl; \
            ++g_failed_tests; \
        } \
    } while (0)

#define ASSERT_EQ(expected, actual, msg) \
    do { \
        ++g_total_tests; \
        if (static_cast<uint64_t>(static_cast<uint64_t>(expected)) != static_cast<uint64_t>(actual)) { \
            std::cerr << "[TEST FAILED] " << msg \
                      << " (expected: " << expected \
                      << ", actual: " << actual << ")" << std::endl; \
            ++g_failed_tests; \
        } \
    } while (0)

// Test 1: False branch. When png_rtran_ok returns 0, png_set_strip_alpha should not modify transformations.
void test_false_branch_no_change()
{
    // Arrange
    mock_png_rtran_ok_return = 0;
    png_struct s;
    s.transformations = 0;

    // Act
    png_set_strip_alpha(&s);

    // Assert
    ASSERT_EQ(0u, s.transformations, "transformations should remain 0 when png_rtran_ok returns 0");
}

// Test 2: True branch. When png_rtran_ok returns non-zero, PNG_STRIP_ALPHA should be set.
void test_true_branch_sets_flag()
{
    // Arrange
    mock_png_rtran_ok_return = 1;
    png_struct s;
    s.transformations = 0;

    // Act
    png_set_strip_alpha(&s);

    // Assert
    ASSERT_EQ(0x01u, s.transformations, "PNG_STRIP_ALPHA flag should be set when rtran_ok returns true");
}

// Test 3: Idempotence. If the flag is already set, calling again should keep it set.
void test_idempotent_when_already_set()
{
    // Arrange
    mock_png_rtran_ok_return = 1;
    png_struct s;
    s.transformations = 0x01; // PNG_STRIP_ALPHA already set

    // Act
    png_set_strip_alpha(&s);

    // Assert (still set)
    ASSERT_EQ(0x01u, s.transformations, "PNG_STRIP_ALPHA should remain set after subsequent call when already set");
}

// Test 4: False branch with an initial non-zero transformations value. Ensure it remains unchanged.
void test_false_branch_from_nonzero_initial()
{
    // Arrange
    mock_png_rtran_ok_return = 0;
    png_struct s;
    s.transformations = 0xABCD; // some existing transformation bits

    // Act
    png_set_strip_alpha(&s);

    // Assert
    ASSERT_EQ(0xABCDu, s.transformations, "Existing transformations should remain unchanged when rtran_ok returns 0");
}

// Entry point to run all tests
int main()
{
    test_false_branch_no_change();
    test_true_branch_sets_flag();
    test_idempotent_when_already_set();
    test_false_branch_from_nonzero_initial();

    std::cout << "Tests run: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;

    // Non-zero return indicates some tests failed
    return (g_failed_tests == 0) ? 0 : 1;
}