// Unit test suite for cre2_quote_meta (cre2.cpp) without GTest.
// This test harness is C++11 compliant and uses a lightweight, non-terminating
// assertion mechanism to maximize code execution coverage.

#include <cre2.h>
#include <cstring>
#include <re2/re2.h>
#include <cstdlib>
#include <string>
#include <sstream>
#include <re2/stringpiece.h>
#include <vector>
#include <iostream>
#include <cstdio>
#include <re2/set.h>


// Forward declaration of the focal function.
// The actual implementation is in cre2.cpp, but we declare it here to link properly.
// The cre2_string_t type is defined in cre2.h (part of the project under test).
#ifdef __cplusplus
extern "C" {
#endif
// If cre2.h provides a prototype, it will be harmonized by the compiler/linker.
// We provide an explicit prototype to guarantee linkage in this test standalone file.
typedef struct {
  char *data;
  int   length;
} cre2_string_t;

int cre2_quote_meta (cre2_string_t * quoted, cre2_string_t * original);
#ifdef __cplusplus
}
#endif

// Include the original headers for RE2 usage to compute expected values.


// Lightweight test harness (non-terminating assertions)
struct TestContext {
  int failures = 0;
  int tests_run = 0;

  void fail(const std::string& msg) {
    ++failures;
    std::cerr << "[TEST_FAIL] " << msg << std::endl;
  }

  template <typename T>
  void expect_eq(const T& a, const T& b, const std::string& msg) {
    if (!(a == b)) {
      std::ostringstream oss;
      oss << "Expected equality failed. " << msg
          << " | actual: " << a << " != expected: " << b;
      fail(oss.str());
    }
  }

  void expect_true(bool cond, const std::string& msg) {
    if (!cond) {
      fail("Expectation failed: " + msg);
    }
  }
};

// Helper to run a single test and increment test counter
#define RUN_TEST(ctx, test_func) do { \
    ++(ctx).tests_run; \
    test_func((ctx)); \
} while(0)

// Test 1: Basic quote without special regex characters
// Original: "abc" -> Quoted should be "abc" (no escaping needed)
void test_cre2_quote_meta_basic(TestContext& ctx) {
  // Prepare original string
  char orig_buf[] = "abc";
  cre2_string_t original;
  original.data = orig_buf;
  original.length = 3;

  // Prepare container for quoted result
  cre2_string_t quoted;
  quoted.data = nullptr;
  quoted.length = 0;

  // Call focal function
  int ret = cre2_quote_meta(&quoted, &original);

  // Verify return code
  ctx.expect_true(ret == 0, "cre2_quote_meta should return 0 on success for basic input");

  if (ret == 0) {
    // Expected result using RE2's QuoteMeta
    re2::StringPiece O(orig_buf, 3);
    std::string expected = RE2::QuoteMeta(O);

    // Validate length
    ctx.expect_eq<int>(quoted.length, (int)expected.length(),
      "Quoted length should match expected length for basic input");

    // Validate content
    std::string got(quoted.data, quoted.length);
    ctx.expect_eq<std::string>(got, expected,
      "Quoted content should match RE2::QuoteMeta output for basic input");

    // Clean up
    free(quoted.data);
  }
}

// Test 2: Quote meta characters that require escaping
// Original: "a(b)c" -> Quoted should be "a\\(b\\)c" (parentheses escaped)
void test_cre2_quote_meta_escape_parens(TestContext& ctx) {
  char orig_buf[] = "a(b)c";
  cre2_string_t original;
  original.data = orig_buf;
  original.length = 5;

  cre2_string_t quoted;
  quoted.data = nullptr;
  quoted.length = 0;

  int ret = cre2_quote_meta(&quoted, &original);

  ctx.expect_true(ret == 0, "cre2_quote_meta should return 0 when escaping parentheses");

  if (ret == 0) {
    re2::StringPiece O(orig_buf, 5);
    std::string expected = RE2::QuoteMeta(O); // Should produce "a\\(b\\)c"

    ctx.expect_eq<int>(quoted.length, (int)expected.length(),
      "Quoted length should equal expected length for escaped input");

    std::string got(quoted.data, quoted.length);
    ctx.expect_eq<std::string>(got, expected,
      "Quoted content should escape regex meta characters correctly (parentheses)");

    free(quoted.data);
  }
}

// Test 3: Quote meta for empty string
// Original: "" -> Quoted should be "" (empty)
void test_cre2_quote_meta_empty(TestContext& ctx) {
  char orig_buf[] = "";
  cre2_string_t original;
  original.data = orig_buf;
  original.length = 0;

  cre2_string_t quoted;
  quoted.data = nullptr;
  quoted.length = 0;

  int ret = cre2_quote_meta(&quoted, &original);

  ctx.expect_true(ret == 0, "cre2_quote_meta should return 0 for empty input");

  if (ret == 0) {
    std::string got(quoted.data, quoted.length);
    ctx.expect_eq<std::string>(got, std::string(""),
      "Quoted content should be empty for empty input");

    free(quoted.data);
  }
}

// Main entry point to run all tests
int main() {
  TestContext ctx;

  // Run tests
  RUN_TEST(ctx, test_cre2_quote_meta_basic);
  RUN_TEST(ctx, test_cre2_quote_meta_escape_parens);
  RUN_TEST(ctx, test_cre2_quote_meta_empty);

  // Summary
  std::cout << "Tests run: " << ctx.tests_run
            << " | Failures: " << ctx.failures << std::endl;

  if (ctx.failures == 0) {
    std::cout << "[ALL TESTS PASSED]" << std::endl;
    return 0;
  } else {
    std::cout << "[TESTS FAILED]" << std::endl;
    return 1;
  }
}