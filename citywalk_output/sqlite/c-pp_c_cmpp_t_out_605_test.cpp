// This test suite targets the focal function cmpp_t_out located in c-pp.c.
// It exercises the behavior of the function with respect to the skip predicates
// and the fwrite outcome, without relying on Google Test. It uses a minimal
// in-process test harness and overrides to validate function behavior.
//
// Notes:
// - We provide C-like stubs for CT_skip, CT_skipLevel and fatal to control
//   behavior during tests.
// - We override fwrite to intercept writes to the test FILE* g.out.pFile.
// - We expose a dummy g variable with the expected layout to satisfy
//   references in cmpp_t_out (g.out.pFile).
// - This test is written to be compiled with a C++11 compiler as requested.

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <assert.h>
#include <cstdio>
#include <sqlite3.h>
#include <stdio.h>
#include <cstdarg>
#include <stdarg.h>
#include <iostream>
#include <cstdlib>


// Forward declaration of the focal function from the C file.
// The function uses C linkage, so declare with extern "C".
extern "C" void cmpp_t_out(void * t, void const * z, unsigned int n);

// Lightweight type alias to match the focal usage in c-pp.c.
// We declare CmppTokenizer as an opaque type for test purposes.
typedef struct CmppTokenizer CmppTokenizer;

// ---------------------------------------------------------------------------
// Minimal global layout that cmpp_t_out expects: a global 'g' with an
// 'out' member containing a FILE* named pFile.
// We define an anonymous struct with C linkage to satisfy external reference.
// The layout must be compatible with g.out.pFile access in cmpp_t_out.
// ---------------------------------------------------------------------------

extern "C" struct {
  struct { FILE * pFile; } out;
} g = { { nullptr } };  // initially no file open

// ---------------------------------------------------------------------------
// Test harness state: control the behavior of dependent predicates and mocks.
// ---------------------------------------------------------------------------

// Predicates controlled by tests
static int test_CT_skip_value = 0;         // 0 -> not skipped; 1 -> skip
static int test_CT_skipLevel_value = 0;

// Flags for test verification
static int fatal_called = 0;
static int fwrite_calls = 0;

// Fallback to simulate fwrite failure when desired
static int fwrite_should_fail = 0;

// Capture data written through our fwrite override (to validate content once)
static std::vector<char> write_buf;

// ---------------------------------------------------------------------------
// Test harness: provide required symbols expected by cmpp_t_out and the C file.
// - CT_skip and CT_skipLevel: controlled by test state above.
// - fatal: mark when called (non-terminating assertion style).
// - g_debug: no-op to avoid noise in test output.
// - fwrite: intercept writes to g.out.pFile; optionally simulate failure; record data.
// ---------------------------------------------------------------------------

extern "C" {

// Provide the tokenizer type that cmpp_t_out will pass through.
// We don't actually use it in the tests beyond passing a pointer.
int CT_skip(CmppTokenizer * /*t*/) {
  return test_CT_skip_value;
}
int CT_skipLevel(CmppTokenizer * /*t*/) {
  return test_CT_skipLevel_value;
}

// fatal should not terminate the test process. Record that it was called.
void fatal(char const *zFmt, ...) {
  // We don't attempt to format the message since it's only used for test signaling.
  (void)zFmt;
  fatal_called = 1;
}

// No-op debug function (the real code may route through a macro; we satisfy the symbol).
void g_debug(int /*level*/, ...) {
  // Intentionally empty for tests.
}

// Override of fwrite to intercept writes to the test file.
// If the stream equals g.out.pFile, record the data; otherwise, emulate 0 writes.
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
  // Only intercept writes to the test-managed file
  if (stream == g.out.pFile) {
    fwrite_calls++;
    if (fwrite_should_fail) {
      return 0; // simulate failure
    }
    const unsigned char* data = static_cast<const unsigned char*>(ptr);
    write_buf.insert(write_buf.end(), data, data + size * nmemb);
    return nmemb;
  }
  // For any other stream, do not pretend to write
  return 0;
}

} // extern "C"

// ---------------------------------------------------------------------------
// Helper utilities for test cases
// ---------------------------------------------------------------------------

static void reset_test_state() {
  test_CT_skip_value = 0;
  test_CT_skipLevel_value = 0;
  fatal_called = 0;
  fwrite_calls = 0;
  fwrite_should_fail = 0;
  write_buf.clear();
}

// Create a temporary file for g.out.pFile; returns FILE* or nullptr on failure
static FILE* open_test_file() {
  // tmpfile provides an anonymous temporary file; it's suitable for tests.
  FILE* f = tmpfile();
  if (!f) {
    std::cerr << "Failed to create temporary file for test.\n";
  }
  return f;
}

// Compare helper for test assertions (non-terminating)
#define EXPECT_EQ(actual, expected, msg) \
  do { if ((actual) != (expected)) { std::cerr << "FAIL: " << msg \
    << " | expected: " << (expected) << ", actual: " << (actual) << "\n"; \
    failures++; } else { std::cout << "PASS: " << msg << "\n"; } } while(0)

static int failures = 0;

// ---------------------------------------------------------------------------
// Test cases
// ---------------------------------------------------------------------------

// Test 1: When CT_skip returns true, cmpp_t_out should not attempt to write any data.
static void test_cmpp_t_out_skip_predicate_true() {
  reset_test_state();
  test_CT_skip_value = 1; // skip = true

  FILE* f = open_test_file();
  g.out.pFile = f;

  // Call with some data; length is 4
  char data[4] = {'A','B','C','D'};
  cmpp_t_out(nullptr, data, 4);

  // Assertions
  EXPECT_EQ(fwrite_calls, 0, "cmpp_t_out should not call fwrite when CT_skip is true");
  EXPECT_EQ(fatal_called, 0, "cmpp_t_out should not call fatal when CT_skip is true");

  if (f) fclose(f);
}

// Test 2: When CT_skip returns false and fwrite succeeds, data should be written and no fatal called.
static void test_cmpp_t_out_write_succeeds() {
  reset_test_state();
  test_CT_skip_value = 0;        // skip = false
  test_CT_skipLevel_value = 0;

  FILE* f = open_test_file();
  g.out.pFile = f;

  // Data to write
  const char data[] = "DATA";
  cmpp_t_out(nullptr, data, sizeof(data) - 1);

  // Assertions
  EXPECT_EQ(fatal_called, 0, "cmpp_t_out should not call fatal when fwrite succeeds");
  EXPECT_EQ(fwrite_calls, 1, "cmpp_t_out should call fwrite exactly once when not skipping and writing data");
  // Optional: verify data captured matches input
  if (write_buf.size() != sizeof(data) - 1) {
    std::cerr << "FAIL: cmpp_t_out write buffer size mismatch; expected " 
              << (sizeof(data) - 1) << ", got " << write_buf.size() << "\n";
  } else {
    bool equal = (std::memcmp(write_buf.data(), data, write_buf.size()) == 0);
    if (!equal) {
      std::cerr << "FAIL: cmpp_t_out write buffer content mismatch\n";
      failures++;
    } else {
      std::cout << "PASS: cmpp_t_out write buffer content matches input\n";
    }
  }

  if (f) fclose(f);
}

// Test 3: When CT_skip returns false but fwrite reports failure, cmpp_t_out should call fatal.
static void test_cmpp_t_out_write_failure_triggers_fatal() {
  reset_test_state();
  test_CT_skip_value = 0;        // skip = false
  fwrite_should_fail = 1;        // simulate fwrite failure

  FILE* f = open_test_file();
  g.out.pFile = f;

  const char data[] = "ERR";
  cmpp_t_out(nullptr, data, sizeof(data) - 1);

  // Assertions
  EXPECT_EQ(fatal_called, 1, "cmpp_t_out should call fatal when fwrite fails");
  // Even on failure, function should not crash; ensure no extra unwinding
  if (f) fclose(f);
}

// ---------------------------------------------------------------------------
// Main function: run tests in sequence (as per domain guidance)
int main(int argc, char const * argv[]) {
  (void)argc; (void)argv;

  std::cout << "Running cmpp_t_out unit tests (C++11) ...\n";

  test_cmpp_t_out_skip_predicate_true();
  test_cmpp_t_out_write_succeeds();
  test_cmpp_t_out_write_failure_triggers_fatal();

  if (failures > 0) {
    std::cout << "Some tests FAILED: " << failures << " failure(s).\n";
    return 1;
  } else {
    std::cout << "All tests PASSED.\n";
    return 0;
  }
}

// End of test suite