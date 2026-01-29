// Minimal C++11 test harness for the focal method: png_set_text_compression_level
// This test suite is self-contained and does not rely on external test frameworks.
// It exercises true/false branches of the NULL check and validates that the
// internal zlib_text_level field is updated as expected when a non-NULL pointer
// is provided. It also verifies that passing a NULL pointer does not invoke the
// debug function (no crash, and no side effects).

#include <climits>
#include <iostream>
#include <pngpriv.h>


// Lightweight stand-in for the real png_struct used by libpng.
// We only model the member accessed by the focal method.
struct png_struct {
  int zlib_text_level;
};

// Typedef mirroring the library's pointer type used by the focal function.
typedef struct png_struct* png_structrp;

// Global counter to observe calls to the internal debug facility.
static int g_debug_calls = 0;

// Minimal stub for the library's debugging function used inside the focal method.
// We treat each invocation as a signal that the path was taken.
static void png_debug(int /*level*/, const char* /*msg*/) {
  ++g_debug_calls;
}

// Focal method under test (replicated locally for the self-contained test).
// Behavior:
// - Logs a debug message (via png_debug).
// - If png_ptr is NULL, returns immediately (no dereference).
// - Otherwise sets the zlib_text_level field to the provided level.
void png_set_text_compression_level(png_structrp png_ptr, int level)
{
  {
     png_debug(1, "in png_set_text_compression_level");
     if (png_ptr == NULL)
        return;
     png_ptr->zlib_text_level = level;
  }
}

/*
Test 1: Non-NULL pointer updates the zlib_text_level and triggers the debug call.
- Purpose: exercise the true branch of the NULL check and validate state mutation.
*/
bool test_set_value_non_null_updates_field()
{
  g_debug_calls = 0;
  png_struct s;
  s.zlib_text_level = -1;            // initial state different from expected
  png_set_text_compression_level(&s, 7); // apply test value
  bool ok = (g_debug_calls == 1) && (s.zlib_text_level == 7);
  return ok;
}

/*
Test 2: Verifies that setting level to zero works and still calls the debug path.
- Purpose: ensure edge value (0) is handled correctly and still exercises the true branch.
*/
bool test_zero_level_sets_to_zero()
{
  g_debug_calls = 0;
  png_struct s;
  s.zlib_text_level = 42;
  png_set_text_compression_level(&s, 0);
  bool ok = (g_debug_calls == 1) && (s.zlib_text_level == 0);
  return ok;
}

/*
Test 3: Verifies handling of the maximum int value.
- Purpose: ensure large positive values are accepted and correctly assigned.
*/
bool test_max_int_sets_to_int_max()
{
  g_debug_calls = 0;
  png_struct s;
  s.zlib_text_level = -5;
  png_set_text_compression_level(&s, INT_MAX);
  bool ok = (g_debug_calls == 1) && (s.zlib_text_level == INT_MAX);
  return ok;
}

/*
Test 4: NULL pointer handling.
- Purpose: exercise the NULL-check branch to ensure no debug call occurs and no crash.
*/
bool test_null_pointer_no_debug_calls()
{
  g_debug_calls = 0;
  png_set_text_compression_level(nullptr, 12345);
  bool ok = (g_debug_calls == 0);
  return ok;
}

int main()
{
  int total = 0, passed = 0;

  auto run = [&](bool test_result, const char* test_name){
    ++total;
    if (test_result) {
      ++passed;
      std::cout << "PASS: " << test_name << std::endl;
    } else {
      std::cout << "FAIL: " << test_name << std::endl;
    }
  };

  run(test_set_value_non_null_updates_field(), "test_set_value_non_null_updates_field");
  run(test_zero_level_sets_to_zero(), "test_zero_level_sets_to_zero");
  run(test_max_int_sets_to_int_max(), "test_max_int_sets_to_int_max");
  run(test_null_pointer_no_debug_calls(), "test_null_pointer_no_debug_calls");

  std::cout << "Summary: " << passed << " / " << total << " tests passed." << std::endl;
  return (passed == total) ? 0 : 1;
}