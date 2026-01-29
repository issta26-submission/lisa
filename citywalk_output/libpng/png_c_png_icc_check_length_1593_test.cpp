/*
   Unit test suite for the focal method: png_icc_check_length
   Location: png.c (as provided by the prompt)
   Testing approach (self-contained):

   - We mock the dependencies used by png_icc_check_length:
     * icc_check_length(png_ptr, name, profile_length) -> bool
     * png_chunk_max(png_ptr) -> uint32
     * png_icc_profile_error(png_ptr, name, value, reason) -> int

   - We re-create the minimal signatures/types required to call
     png_icc_check_length in a C++11 test harness.

   - We exercise all relevant branches:
     1) icc_check_length returns false -> png_icc_check_length returns 0
     2) icc_check_length returns true and profile_length <= chunk_max -> returns 1
     3) icc_check_length returns true and profile_length > chunk_max -> calls
        png_icc_profile_error and returns its value

   Notes:
   - This test is self-contained (no external test framework is required).
   - Non-terminating assertions are used (they log failures but continue).
   - The tests do not rely on private data; only the required API is mocked.
   - Static members or file-scope details in the original file are simulated via
     internal static-like control in this harness.

   Important: This code is intended as a demonstration/test harness and can be
   adapted to link with the real png.c in a full build environment.
*/

#include <cstdint>
#include <string>
#include <iostream>
#include <pngpriv.h>
#include <cstddef>


// Domain knowledge compliance: use only C++ standard library for tests.

// Typedefs matching the signatures used by png.c (simplified for unit tests)
typedef uint32_t png_uint_32;
typedef struct png_struct png_struct;
typedef const png_struct* png_structrp;
typedef const png_struct* png_const_structrp;
typedef const char* png_const_charp;

// Forward declare to mimic the external API used by png_icc_check_length
extern "C" int icc_check_length(png_const_structrp png_ptr, png_const_charp name,
                              png_uint_32 profile_length);
extern "C" png_uint_32 png_chunk_max(png_structrp png_ptr);
extern "C" int png_icc_profile_error(png_const_structrp png_ptr, png_const_charp name,
                                   size_t value, png_const_charp reason);

// ---------------------------------------------------------------------------
// Test Harness Globals (mock behavior control)
static bool icc_check_length_should_return = true; // controls icc_check_length
static uint32_t mock_png_chunk_max_value = 1024;   // default max length
static int png_icc_profile_error_return = -1;      // return value of error handler
static int png_icc_profile_error_called = 0;       // count of error handler invocations

// Mock implementations to control behavior in tests
extern "C" int icc_check_length(png_const_structrp /*png_ptr*/, png_const_charp /*name*/,
                              png_uint_32 profile_length)
{
    // The test can set icc_check_length_should_return to drive true/false
    return icc_check_length_should_return ? 1 : 0;
}

extern "C" png_uint_32 png_chunk_max(png_structrp /*png_ptr*/)
{
    // Return the test-controlled maximum allowed chunk length
    return mock_png_chunk_max_value;
}

extern "C" int png_icc_profile_error(png_const_structrp /*png_ptr*/, png_const_charp /*name*/,
                                   size_t /*value*/, png_const_charp /*reason*/)
{
    // Track calls and return a test-controlled value
    ++png_icc_profile_error_called;
    return png_icc_profile_error_return;
}

// ---------------------------------------------------------------------------
// Original function under test (reproduced here for the self-contained harness)
//
// This mirrors the logic exactly as in the prompt.
// It is defined here to enable unit testing without relying on the actual png.c linkage.
extern "C" int png_icc_check_length(png_const_structrp png_ptr, png_const_charp name,
                                   png_uint_32 profile_length)
{
    if (!icc_check_length(png_ptr, name, profile_length))
        return 0;
    /* This needs to be here because the 'normal' check is in
     * png_decompress_chunk, yet this happens after the attempt to
     * png_malloc_base the required data.  We only need this on read; on write
     * the caller supplies the profile buffer so libpng doesn't allocate it.  See
     * the call to icc_check_length below (the write case).
     */
    if (profile_length > png_chunk_max(png_ptr))
        return png_icc_profile_error(png_ptr, name, profile_length,
                                   "profile too long");
    return 1;
}

// ---------------------------------------------------------------------------
// Minimal fake structs to satisfy types (no functional data needed for tests)
struct png_struct {
    // Placeholder to resemble real-world usage (not used by tests directly)
    int dummy;
};

// ---------------------------------------------------------------------------
// Simple test framework (non-GTest, non-GMock)
static int g_failures = 0;

#define EXPECT_TRUE(cond) do {                                         \
    if (!(cond)) {                                                       \
        std::cerr << "EXPECT_TRUE failed: " #cond " at " << __FILE__     \
                  << ":" << __LINE__ << "\n";                          \
        ++g_failures;                                                   \
    }                                                                    \
} while(0)

#define EXPECT_FALSE(cond) do {                                        \
    if (!!(cond)) {                                                      \
        std::cerr << "EXPECT_FALSE failed: " #cond " at " << __FILE__    \
                  << ":" << __LINE__ << "\n";                          \
        ++g_failures;                                                   \
    }                                                                    \
} while(0)

#define EXPECT_EQ(a,b) do {                                             \
    if (!((a)==(b))) {                                                  \
        std::cerr << "EXPECT_EQ failed: " #a " == " #b " (got "            \
                  << (a) << " vs " << (b) << ") at " << __FILE__        \
                  << ":" << __LINE__ << "\n";                         \
        ++g_failures;                                                  \
    }                                                                   \
} while(0)

#define LOG_MSG(msg) do { std::cout << "[LOG] " << msg << std::endl; } while(0)

// ---------------------------------------------------------------------------
// Test Case 1: icc_check_length returns false -> png_icc_check_length should return 0
void test_case_icc_check_length_false()
{
    LOG_MSG("Test Case 1: icc_check_length returns false -> expect 0");
    // Arrange
    icc_check_length_should_return = false;
    // Use a reasonable profile_length; actual value doesn't matter because icc_check_length
    // will return false immediately.
    png_uint_32 profile_length = 100;
    // fake png_ptr
    png_struct fake_ptr;
    // Ensure chunk_max is not relevant in this branch
    mock_png_chunk_max_value = 500;

    // Act
    int result = png_icc_check_length(reinterpret_cast<png_const_structrp>(&fake_ptr),
                                      "TestProfile", profile_length);

    // Assert
    EXPECT_EQ(result, 0);
    // Cleanup (not strictly necessary here)
    icc_check_length_should_return = true;
}

// ---------------------------------------------------------------------------
// Test Case 2: icc_check_length returns true and profile_length <= chunk_max -> expect 1
void test_case_icc_check_length_within_limit()
{
    LOG_MSG("Test Case 2: icc_check_length true and profile_length within chunk_max -> expect 1");
    // Arrange
    icc_check_length_should_return = true;
    mock_png_chunk_max_value = 1024;
    png_uint_32 profile_length = 512; // <= 1024
    png_struct fake_ptr;

    // Act
    int result = png_icc_check_length(reinterpret_cast<png_const_structrp>(&fake_ptr),
                                      "TestProfile", profile_length);

    // Assert
    EXPECT_EQ(result, 1);
    // Ensure no error path was taken
    EXPECT_FALSE(png_icc_profile_error_called != 0);
}

// ---------------------------------------------------------------------------
// Test Case 3: icc_check_length returns true and profile_length > chunk_max -> expect
//               the return value of png_icc_profile_error
void test_case_profile_too_long_triggers_error_path()
{
    LOG_MSG("Test Case 3: profile_length > chunk_max -> expect png_icc_profile_error return value");
    // Arrange
    icc_check_length_should_return = true;
    mock_png_chunk_max_value = 256;
    png_uint_32 profile_length = 512; // > 256
    png_icc_profile_error_return = 42; // sentinel value for error handler
    png_icc_profile_error_called = 0;
    png_struct fake_ptr;

    // Act
    int result = png_icc_check_length(reinterpret_cast<png_const_structrp>(&fake_ptr),
                                      "TestProfile", profile_length);

    // Assert
    EXPECT_EQ(result, 42);
    EXPECT_EQ(png_icc_profile_error_called, 1);
    // Cleanup
    icc_check_length_should_return = true;
}

// ---------------------------------------------------------------------------
// Main: run all tests
int main()
{
    std::cout << "Starting tests for png_icc_check_length (unit test harness)\n";

    test_case_icc_check_length_false();
    test_case_icc_check_length_within_limit();
    test_case_profile_too_long_triggers_error_path();

    if (g_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed.\n";
        return 1;
    }
}

/*
   Explanatory notes per test:

   - Test Case 1 validates the short-circuit: if icc_check_length returns false,
     png_icc_check_length must return 0 regardless of profile_length. We simulate
     this by forcing icc_check_length to return false and verify the function
     short-circuits.

   - Test Case 2 validates the "profile_length within allowed range" branch:
     when icc_check_length is true and profile_length <= png_chunk_max(png_ptr),
     the function should return 1 (success). We set the mock chunk_max to a value
     that accommodates the test length and assert the return value is 1.

   - Test Case 3 validates the "profile_length too long" branch:
     when icc_check_length is true but profile_length > png_chunk_max(png_ptr),
     the function calls png_icc_profile_error. We verify that the function returns
     the error code supplied by png_icc_profile_error and that the error handler
     was invoked exactly once.

   Notes for integration with real project:
   - The actual project would link against the real png.c and its dependencies.
   - To run these tests against real code, ensure the following:
     * icc_check_length, png_chunk_max, and png_icc_profile_error are extern
       (non-static) symbols provided by the library.
     * Provide real implementations or suitable mocks that can be overridden
       for testing (e.g., via a test-specific compilation unit or linker options).
     * The test harness should be compiled with C++11 support and linked with
       the library that contains png_icc_check_length.
*/