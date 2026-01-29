/*
Step 1: Program Understanding and Candidate Keywords
- Focal method: png_get_image_width(png_const_structrp png_ptr, png_const_inforp info_ptr)
- Core dependencies (as visible in the focal snippet and surrounding placeholders):
  - png_ptr: a non-null check is performed; represents a pointer to a PNG "struct" (opaque in this simplified test)
  - info_ptr: a non-null pointer to an information block that contains image metadata
  - info_ptr->width: the width value to be returned when inputs are valid
- Key behavior to test:
  - When both png_ptr and info_ptr are non-null, return info_ptr->width
  - If either png_ptr or info_ptr is NULL, return 0
- Candidate Keywords for tests: png_ptr, png_const_structrp, info_ptr, png_const_inforp, width, NULL checks, non-null vs null branches, return value, type safety

Step 2: Unit Test Generation (targeting png_get_image_width)
- We implement a minimal in-file reproduction of the function under test with compatible types suitable for a C++11 test harness (no external GTest).
- Tests cover:
  - Both pointers non-null -> returns the stored width
  - png_ptr is NULL -> returns 0
  - info_ptr is NULL -> returns 0
  - Both NULL -> returns 0
  - Edge cases: width = 0, width = max unsigned value
- Static analysis constraints:
  - No private member access; no GMock; only standard library and provided method logic
  - Use non-terminating assertions (custom EXPECT_* macros) to maximize code execution coverage
  - Tests are invoked from main (no GTest)

Step 3: Test Case Refinement
- Provide clear, explicit test cases with comments describing intent and expected outcomes
- Ensure tests compile under C++11 and are executable as a standalone translation unit
- Use a small, self-contained test harness with a lightweight assertion mechanism

Code (self-contained unit tests for png_get_image_width)
*/

// Self-contained test harness and focal function under test
#include <cstdint>
#include <iostream>
#include <pngpriv.h>


// -------------------- FOCAL METHOD REPRODUCTION --------------------
// Types compatible with the focal snippet
// We simulate the minimal necessary structures/types to compile and test.

struct png_info_struct {
  unsigned int width;
};

typedef const void* png_const_structrp;     // lightweight opaque const struct pointer
typedef const png_info_struct* png_const_inforp; // pointer to const info struct (width field used)

// Function under test (reproduced here to allow standalone unit tests)
unsigned int png_get_image_width(png_const_structrp png_ptr, png_const_inforp info_ptr)
{
  if (png_ptr != nullptr && info_ptr != nullptr)
    return info_ptr->width;
  return 0;
}

// -------------------- TEST HARNESS --------------------

// Simple non-terminating assertion macro
// Prints a message on failure but does not abort the test run.
static int g_failed; // global failure counter used by EXPECT_EQ

#define EXPECT_EQ(actual, expected) do {                         \
    auto a = (actual);                                           \
    auto b = (expected);                                         \
    if (!(a == b)) {                                             \
        std::cerr << "EXPECT_EQ failed: " #actual " (" << a       \
                  << ") != " #expected " (" << b                 \
                  << ") at " << __FILE__ << ":" << __LINE__ << "\n"; \
        ++g_failed;                                              \
    }                                                            \
} while(0)

// Test 1: Both png_ptr and info_ptr are non-null; expect width to be returned
// Intent: Validate the true branch of the conditional
int test_png_get_image_width_both_non_null() {
  g_failed = 0;

  png_info_struct info;
  info.width = 12345;

  void* non_null_ptr = reinterpret_cast<void*>(0x1); // non-null sentinel

  png_const_structrp png_ptr = non_null_ptr;
  png_const_inforp info_ptr = &info;

  unsigned int w = png_get_image_width(png_ptr, info_ptr);
  EXPECT_EQ(w, 12345);

  return g_failed == 0 ? 0 : 1;
}

// Test 2: png_ptr is NULL; expect 0
// Intent: False branch when png_ptr is NULL
int test_png_get_image_width_png_ptr_null() {
  g_failed = 0;

  png_info_struct info;
  info.width = 255;

  png_const_structrp png_ptr = nullptr;
  png_const_inforp info_ptr = &info;

  unsigned int w = png_get_image_width(png_ptr, info_ptr);
  EXPECT_EQ(w, 0u);

  return g_failed == 0 ? 0 : 1;
}

// Test 3: info_ptr is NULL; expect 0
// Intent: False branch when info_ptr is NULL
int test_png_get_image_width_info_ptr_null() {
  g_failed = 0;

  void* non_null_ptr = reinterpret_cast<void*>(0x2);
  png_const_structrp png_ptr = non_null_ptr;
  png_const_inforp info_ptr = nullptr;

  unsigned int w = png_get_image_width(png_ptr, info_ptr);
  EXPECT_EQ(w, 0u);

  return g_failed == 0 ? 0 : 1;
}

// Test 4: Both png_ptr and info_ptr are NULL; expect 0
// Intent: Full false path when both inputs are NULL
int test_png_get_image_width_both_null() {
  g_failed = 0;

  png_const_structrp png_ptr = nullptr;
  png_const_inforp info_ptr = nullptr;

  unsigned int w = png_get_image_width(png_ptr, info_ptr);
  EXPECT_EQ(w, 0u);

  return g_failed == 0 ? 0 : 1;
}

// Test 5: Edge case width = 0; ensure 0 is returned when inputs are valid
int test_png_get_image_width_zero_width() {
  g_failed = 0;

  png_info_struct info;
  info.width = 0;

  void* non_null_ptr = reinterpret_cast<void*>(0x3);
  png_const_structrp png_ptr = non_null_ptr;
  png_const_inforp info_ptr = &info;

  unsigned int w = png_get_image_width(png_ptr, info_ptr);
  EXPECT_EQ(w, 0u);

  return g_failed == 0 ? 0 : 1;
}

// Test 6: Edge case width = max unsigned value
int test_png_get_image_width_max_width() {
  g_failed = 0;

  png_info_struct info;
  info.width = std::numeric_limits<unsigned int>::max();

  void* non_null_ptr = reinterpret_cast<void*>(0x4);
  png_const_structrp png_ptr = non_null_ptr;
  png_const_inforp info_ptr = &info;

  unsigned int w = png_get_image_width(png_ptr, info_ptr);
  EXPECT_EQ(w, std::numeric_limits<unsigned int>::max());

  return g_failed == 0 ? 0 : 1;
}

// -------------------- MAIN -------------------------------------------------

int main() {
  int total_failures = 0;

  std::cout << "Running png_get_image_width unit tests (C++11-only, no GTest)...\n";

  total_failures += test_png_get_image_width_both_non_null();
  total_failures += test_png_get_image_width_png_ptr_null();
  total_failures += test_png_get_image_width_info_ptr_null();
  total_failures += test_png_get_image_width_both_null();
  total_failures += test_png_get_image_width_zero_width();
  total_failures += test_png_get_image_width_max_width();

  if (total_failures == 0) {
    std::cout << "All tests passed.\n";
  } else {
    std::cerr << "Total test failures: " << total_failures << "\n";
  }

  return total_failures;
}