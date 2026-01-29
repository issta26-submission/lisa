/*
Unit test suite for cre2_strings_to_ranges (C interface) using a lightweight
test harness (no GoogleTest). The tests exercise the core logic of
cre2_strings_to_ranges, which computes the numeric range offsets (start, past)
for each matched string given a base text and an array of cre2_string_t entries.

Assumptions about types (as defined in cre2.h):
- cre2_string_t has at least:
  - const char *data
  - int length
- cre2_range_t has at least:
  - int start
  - int past

The test suite covers:
- Basic non-overlapping matches
- Zero-length match
- Zero iterations (nmatch == 0)
- Negative/out-of-bounds data pointer scenarios
- Multiple mixed ranges

Note: The tests use a simple non-terminating assertion macro to allow all
tests to run and report a summary of failures.
*/

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Include the CRE2 style declarations

// Ensure the function is visible with C linkage
extern "C" void cre2_strings_to_ranges (const char * text, cre2_range_t * ranges, cre2_string_t * strings, int nmatch);

static int g_failures = 0;
#define TEST_ASSERT(cond, msg) do { \
  if(!(cond)) { \
    std::cerr << "Test assertion failed: " << (msg) \
              << " [" << __FILE__ << ":" << __LINE__ << "]" << std::endl; \
    ++g_failures; \
  } \
} while(0)

/*
Test 1: Basic ranges calculation with two matches at known positions.
- text: "abcdef"
- match 0: data = text + 0, length = 2 ("ab")
- match 1: data = text + 3, length = 3 ("def")
Expected:
- ranges[0].start == 0
- ranges[0].past  == 2
- ranges[1].start == 3
- ranges[1].past  == 6
*/
bool test_basic_ranges() {
  int local_failures = 0; // track per-test failures via global macro
  const char * text = "abcdef";
  const int nmatch = 2;

  cre2_range_t ranges[2];
  cre2_string_t strings[2];

  // Initialize input strings to point inside the text
  strings[0].data = text;       // points to 'a'
  strings[0].length = 2;          // "ab"

  strings[1].data = text + 3;     // points to 'd'
  strings[1].length = 3;            // "def"

  // Run the function under test
  cre2_strings_to_ranges(text, ranges, strings, nmatch);

  TEST_ASSERT(ranges[0].start == 0, "ranges[0].start should be 0");
  TEST_ASSERT(ranges[0].past  == 2, "ranges[0].past should be 2");
  TEST_ASSERT(ranges[1].start == 3, "ranges[1].start should be 3");
  TEST_ASSERT(ranges[1].past  == 6, "ranges[1].past should be 6");

  // Return true if no new failures occurred in this test
  return (g_failures - local_failures) == 0;
}

/*
Test 2: Zero-length match handling.
- text: "hello"
- match 0: data = text + 1, length = 0
Expected:
- ranges[0].start == 1
- ranges[0].past  == 1
*/
bool test_zero_length() {
  int local_before = g_failures;

  const char * text = "hello";
  int nmatch = 1;
  cre2_range_t ranges[1];
  cre2_string_t strings[1];

  strings[0].data = text + 1; // points to 'e'
  strings[0].length = 0;       // zero-length

  cre2_strings_to_ranges(text, ranges, strings, nmatch);

  TEST_ASSERT(ranges[0].start == 1, "Zero-length: start should be 1");
  TEST_ASSERT(ranges[0].past  == 1, "Zero-length: past should be 1");

  return (g_failures == local_before);
}

/*
Test 3: No iterations (nmatch = 0) should not modify ranges or crash.
- text: arbitrary
- ranges initial values set to sentinel
- strings irrelevant
Expected: after call, ranges remain unchanged.
*/
bool test_nmatch_zero() {
  int local_before = g_failures;

  const char * text = "xyz";
  cre2_range_t ranges[1] = { { -9999, -9999 } };
  cre2_string_t strings[1] = { { text, 3 } };

  cre2_strings_to_ranges(text, ranges, strings, 0);

  TEST_ASSERT(ranges[0].start == -9999, "nmatch==0: ranges[0].start unchanged");
  TEST_ASSERT(ranges[0].past  == -9999, "nmatch==0: ranges[0].past unchanged");

  return (g_failures == local_before);
}

/*
Test 4: Negative data pointer in strings (data points before text).
- text: "abcdef"
- match 0: data = text - 2, length = 4
Expected:
- ranges[0].start == -2
- ranges[0].past  == 2
*/
bool test_negative_data_pointer() {
  int local_before = g_failures;

  const char * text = "abcdef";
  const int nmatch = 1;
  cre2_range_t ranges[1];
  cre2_string_t strings[1];

  strings[0].data = text - 2; // deliberately before text
  strings[0].length = 4;       // would span to 'cdef' from -2

  cre2_strings_to_ranges(text, ranges, strings, nmatch);

  TEST_ASSERT(ranges[0].start == -2, "Negative data: start should be -2");
  TEST_ASSERT(ranges[0].past  == 2,  "Negative data: past should be 2");

  return (g_failures == local_before);
}

/*
Test 5: Multiple matches with mixed positions to exercise broader behavior.
- text: "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
- match 0: data = text + 0, length = 5 ("01234") -> start 0, past 5
- match 1: data = text + 10, length = 3 ("678") -> start 10, past 13
- match 2: data = text + 25, length = 6 ("QRSTUVWXYZ") -> start 25, past 31
*/
bool test_multiple_matches_mixed() {
  int local_before = g_failures;

  const char * text = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int nmatch = 3;
  cre2_range_t ranges[3];
  cre2_string_t strings[3];

  strings[0].data = text + 0;
  strings[0].length = 5; // "01234"

  strings[1].data = text + 10;
  strings[1].length = 3; // "678"

  strings[2].data = text + 25;
  strings[2].length = 6; // "QRSTUVWXYZ"

  cre2_strings_to_ranges(text, ranges, strings, nmatch);

  TEST_ASSERT(ranges[0].start == 0,  "First range start should be 0");
  TEST_ASSERT(ranges[0].past  == 5,  "First range past should be 5");

  TEST_ASSERT(ranges[1].start == 10, TEST_ASSERT(ranges[1].start == 10, "Second range start should be 10"));
  TEST_ASSERT(ranges[1].past  == 13, TEST_ASSERT(ranges[1].past  == 13, "Second range past should be 13"));

  TEST_ASSERT(ranges[2].start == 25, TEST_ASSERT(ranges[2].start == 25, "Third range start should be 25"));
  TEST_ASSERT(ranges[2].past  == 31, TEST_ASSERT(ranges[2].past  == 31, "Third range past should be 31"));

  return (g_failures == local_before);
}

int main() {
  std::cout << "Running cre2_strings_to_ranges unit tests (C++11, no gtest) ..." << std::endl;

  // Run tests; each test reports its own results via TEST_ASSERT
  bool ok = true;

  ok &= test_basic_ranges();
  ok &= test_zero_length();
  ok &= test_nmatch_zero();
  ok &= test_negative_data_pointer();
  ok &= test_multiple_matches_mixed();

  if (ok && g_failures == 0) {
    std::cout << "All tests passed." << std::endl;
    return 0;
  } else {
    std::cerr << "Tests completed with " << g_failures << " failure(s)." << std::endl;
    return 1;
  }
}