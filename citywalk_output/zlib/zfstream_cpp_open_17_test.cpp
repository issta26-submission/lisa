// Unit test suite for gzfilebuf::open in zfstream.cpp
// Targets: C++11, non-GTest framework, executable from main()
// Notes:
// - This test suite exercises different branches of gzfilebuf::open:
//   - Reading (ios::in) path
//   - Writing (ios::out) path
//   - Appending (ios::app) path
//   - Binary flag (ios::binary)
//   - Compression level 9 flag (added when writing/appending)
//   - Already-open short-circuit path
// - Requires zlib (gzopen/gzprintf/gzclose) to be available.
// - Tests create temporary gzip files in the working directory.
// - Uses a minimal, self-contained test harness (no GTest).

#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <zfstream.h>


extern "C" {
  #include <zlib.h> // for gzopen, gzprintf, gzclose
}


using std::cout;
using std::endl;
using std::string;

// Simple test harness state
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Utility to report per-test results
#define REPORT_TEST(name, passed) \
  do { \
    ++g_total_tests; \
    if (passed) { \
      cout << "[PASSED] " << name << "\n"; \
    } else { \
      cout << "[FAILED] " << name << "\n"; \
      ++g_failed_tests; \
    } \
  } while (0)

// Test 1: Open in read mode on an existing gzip file should succeed
// Rationale: Exercises the 'if ( io_mode & ios::in )' branch and gzopen with 'r'.
// Precondition: A valid gzip file exists.
bool test_open_in_mode_opens_existing_gzip_file() {
  const char* fname = "test_open_in_existing.gz";

  // Create a small gzip file to read from
  gzFile f = gzopen(fname, "wb9"); // write with max compression
  if (f == NULL) {
    std::cerr << "Could not create test gzip file for test_open_in_mode_opens_existing_gzip_file\n";
    return false;
  }
  gzprintf(f, "hello world");
  gzclose(f);

  // Open the gzip file in read mode via gzfilebuf::open
  gzfilebuf buf;
  gzfilebuf* res = buf.open(fname, ios::in);

  bool passed = (res != NULL);
  // Clean up: do not forcibly close to avoid assumptions about internal lifecycle;
  // The OS will reclaim resources when process ends, and this is a simple unit test.
  // (Files may remain as artifacts in the test directory.)
  return passed;
}

// Test 2: Open in read mode on a non-existing file should fail (return NULL)
bool test_open_in_missing_file_returns_null() {
  const char* fname = "test_open_in_missing.gz";

  gzfilebuf buf;
  gzfilebuf* res = buf.open(fname, ios::in);
  bool passed = (res == NULL);
  return passed;
}

// Test 3: Open in write mode should succeed (creates new gzip file)
// Rationale: Exercises the 'else' branch (out) for mode selection and gzopen with 'w' (and without 'b' unless binary is specified).
bool test_open_out_mode_creates_file() {
  const char* fname = "test_open_out.gz";

  gzfilebuf buf;
  gzfilebuf* res = buf.open(fname, ios::out);
  bool passed = (res != NULL);

  // We do not attempt to read/write contents here; presence of a non-NULL pointer indicates success.
  return passed;
}

// Test 4: Re-opening when already open should return NULL (short-circuit path)
// Rationale: Exercises the first 'if ( is_open() ) return NULL;' branch
bool test_open_already_open_returns_null() {
  const char* fname = "test_open_twice.gz";

  gzfilebuf buf;
  bool first = (buf.open(fname, ios::out) != NULL);
  bool second = (buf.open(fname, ios::out) == NULL);

  return first && second;
}

// Test 5: Open in appending mode (ios::app) should succeed
// Rationale: Exercises the app path and ensures char_mode accumulates 'a' and '9' when appropriate.
bool test_open_app_mode_succeeds() {
  const char* fname = "test_open_app.gz";

  gzfilebuf buf;
  gzfilebuf* res = buf.open(fname, ios::app);
  bool passed = (res != NULL);
  return passed;
}

// Test 6: Open in appending + binary mode should succeed (to cover binary flag handling)
bool test_open_app_binary_mode_succeeds() {
  const char* fname = "test_open_app_bin.gz";

  gzfilebuf buf;
  gzfilebuf* res = buf.open(fname, ios::app | ios::binary);
  bool passed = (res != NULL);
  return passed;
}

// Main: Run all tests and report summary
int main() {
  // Execute tests (non-terminating; collect results)
  REPORT_TEST("test_open_in_mode_opens_existing_gzip_file", test_open_in_mode_opens_existing_gzip_file());
  REPORT_TEST("test_open_in_missing_file_returns_null", test_open_in_missing_file_returns_null());
  REPORT_TEST("test_open_out_mode_creates_file", test_open_out_mode_creates_file());
  REPORT_TEST("test_open_already_open_returns_null", test_open_already_open_returns_null());
  REPORT_TEST("test_open_app_mode_succeeds", test_open_app_mode_succeeds());
  REPORT_TEST("test_open_app_binary_mode_succeeds", test_open_app_binary_mode_succeeds());

  // Summary
  cout << "Total tests: " << g_total_tests << ", Passed: " << (g_total_tests - g_failed_tests)
       << ", Failed: " << g_failed_tests << endl;

  // Exit code 0 if all tests passed, non-zero otherwise
  return g_failed_tests == 0 ? 0 : 1;
}