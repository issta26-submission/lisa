// Test suite for the focal method: sqlite3Strlen30
// This test is self-contained and compiles under C++11 without GTest.
// The real SQLite code base is not required; we re-provide the focal
// function signature in order to exercise its behavior in isolation.
//
// Mapping to the requested steps:
// - Step 1: Analyzed the focal function: returns 0 for null input, otherwise
//           returns strlen(z) masked with 0x3fffffff.
// - Step 2: Generated a minimal unit test suite covering null input, empty
//           string, typical short strings, and a longer string to exercise
//           the non-null branch.
// - Step 3: Refined tests for high coverage and non-terminating assertions.
//           We implement a lightweight test harness that records failures
//           without aborting, enabling full execution and reporting.

#include <math.h>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>


// FOCAL METHOD replicated for testing purposes.
// In the original project, this would be linked from util.c.
int sqlite3Strlen30(const char *z){
  if( z==0 ) return 0;
  return 0x3fffffff & (int)strlen(z);
}

// Lightweight, non-terminating test harness
static int g_failures = 0;

// Generic check helper that does not terminate on failure.
// It reports mismatches and keeps executing remaining tests.
template <typename T>
void check_eq(const char* test_name, const T& expected, const T& actual){
  if(!(expected == actual)){
    std::cerr << "FAIL: " << test_name
              << " | expected: " << expected
              << " actual: " << actual << "\n";
    ++g_failures;
  } else {
    std::cout << "PASS: " << test_name << "\n";
  }
}

// Test 1: Null input should yield 0.
// True branch: z == 0 is handled explicitly.
void test_null_input(){
  // Domain knowledge: input is a null pointer.
  int r = sqlite3Strlen30(nullptr);
  check_eq("Null input returns 0", 0, r);
}

// Test 2: Empty string should yield 0.
void test_empty_string(){
  // True for empty C-string; strlen("") == 0
  int r = sqlite3Strlen30("");
  check_eq("Empty string length 0", 0, r);
}

// Test 3: Single character string should yield length 1.
void test_single_character(){
  int r = sqlite3Strlen30("a");
  check_eq("Single character length 1", 1, r);
}

// Test 4: Known string should yield its length (non-masked case).
void test_known_string(){
  const char* s = "abcdef"; // length 6
  int r = sqlite3Strlen30(s);
  check_eq("Known string length 6", 6, r);
}

// Test 5: Long string to exercise non-null path with a larger length.
// This ensures we correctly apply the strlen and masking within bounds.
void test_long_string(){
  std::string s(1000, 'x'); // length 1000
  int r = sqlite3Strlen30(s.c_str());
  check_eq("Long string length 1000", 1000, r);
}

// Run all tests and report total failures.
int run_all_tests(){
  test_null_input();
  test_empty_string();
  test_single_character();
  test_known_string();
  test_long_string();

  std::cout << "Total failures: " << g_failures << "\n";
  return g_failures;
}

// Entry point
int main(){
  int failures = run_all_tests();
  // Return non-zero if any test failed
  return failures;
}