// Unit test suite for CmppKeyword_search in c-pp.c
// Target: C++11 environment without GTest. Test harness is self-contained and invoked from main.
// Notes:
// - The test relies on the actual CmppKeyword_search implementation inside c-pp.c.
// - We declare CmppKeyword as an incomplete type to avoid requiring full knowledge of its internals.
// - We exercise both negative and positive scenarios to gain coverage of the function's behavior.
// - Explanatory comments are included for each test to describe intent and approach.

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
#include <cstdint>


// Forward declaration of the C type used by the focal function.
// We declare an opaque CmppKeyword type to enable pointer handling without needing the struct layout.
extern "C" {
  typedef struct CmppKeyword CmppKeyword;
  // Focal function under test
  const CmppKeyword * CmppKeyword_search(const char *zName);
}

// Simple test framework (non GTest, minimal, header-only)
static int g_tests_run = 0;
static int g_tests_passed = 0;

#define EXPECT_TRUE(expr, msg) do { \
    if (expr) { \
        ++g_tests_passed; \
    } else { \
        std::cerr << "Test failed: " << (msg ? msg : "EXPECT_TRUE failed") \
                  << " (line " << __LINE__ << ")" << std::endl; \
    } \
    ++g_tests_run; \
} while (0)

#define EXPECT_FALSE(expr, msg) do { \
    if (!(expr)) { \
        ++g_tests_passed; \
    } else { \
        std::cerr << "Test failed: " << (msg ? msg : "EXPECT_FALSE failed") \
                  << " (line " << __LINE__ << ")" << std::endl; \
    } \
    ++g_tests_run; \
} while (0)

#define EXPECT_NOT_NULL(ptr, msg) do { \
    if ((ptr) != nullptr) { \
        ++g_tests_passed; \
    } else { \
        std::cerr << "Test failed: " << (msg ? msg : "EXPECTED non-null pointer") \
                  << " (line " << __LINE__ << ")" << std::endl; \
    } \
    ++g_tests_run; \
} while (0)

#define EXPECT_NULL(ptr, msg) do { \
    if ((ptr) == nullptr) { \
        ++g_tests_passed; \
    } else { \
        std::cerr << "Test failed: " << (msg ? msg : "EXPECTED NULL pointer") \
                  << " (line " << __LINE__ << ")" << std::endl; \
    } \
    ++g_tests_run; \
} while (0)

// Test 1: Negative path - searching for a clearly nonexistent keyword returns NULL
// This exercises the false-branch of the predicate inside bsearch usage.
static bool test_search_nonexistent_keyword_returns_null() {
  // A deliberately unlikely keyword to exist in a predefined aKeywords array.
  const char * nonexistent = "THIS_KEYWORD_SHOULD_NOT_EXIST_12345";
  const CmppKeyword * p = CmppKeyword_search(nonexistent);

  if (p == nullptr) {
    // Expected outcome: NULL when the keyword is not present
    return true;
  } else {
    std::cerr << "CmppKeyword_search returned non-NULL for nonexistent keyword '"
              << nonexistent << "'. Pointer: " << p << std::endl;
    return false;
  }
}

// Test 2: Positive path attempt - verify that at least one likely keyword exists
// The aKeywords array probably contains common preprocessor-style keywords such as include/define/if/else/endif/etc.
// This test tries a small candidate set and expects at least one to be found.
// If none are found (unexpected), the test reports failure to ensure we catch the false negative scenario.
static bool test_search_known_keyword_returns_non_null() {
  const char * candidates[] = {
    "define",   // commonly present in keyword lists
    "if",
    "include",
    "endif",
    "pragma",
    "else",
    "stderr"
  };

  const size_t n = sizeof(candidates) / sizeof(candidates[0]);
  for (size_t i = 0; i < n; ++i) {
    const CmppKeyword * p = CmppKeyword_search(candidates[i]);
    if (p != nullptr) {
      // Found at least one known keyword; test passes for positive path
      std::cout << "Found keyword '" << candidates[i] << "' via CmppKeyword_search. Pointer: " << p << std::endl;
      return true;
    }
  }

  // If we reach here, none of the candidates were found; this is a failure for this test.
  std::cerr << "CmppKeyword_search did not find any of the common candidate keywords. "
            << "Candidates checked: define, if, include, endif, pragma, else, stderr." << std::endl;
  return false;
}

// Test 3: Consistency check - multiple identical searches for the same key return the same pointer
// This exercises potential caching or consistent comparator behavior within bsearch usage.
static bool test_search_consistency_same_key_returns_same_pointer() {
  const char * key = "include"; // attempt a reasonably common keyword
  const CmppKeyword * p1 = CmppKeyword_search(key);
  const CmppKeyword * p2 = CmppKeyword_search(key);

  if (p1 == p2) {
    // Pointer stability check passed
    return true;
  } else {
    std::cerr << "Inconsistent results for repeated search of key '" << key
              << "'. First pointer: " << p1 << ", Second pointer: " << p2 << std::endl;
    return false;
  }
}

// Test 4: Boundaries - ensure function handles a short/empty string input gracefully
// We expect either NULL or a non-NULL depending on the implementation, but the call should not crash.
static bool test_search_empty_string_behavior() {
  const char * empty = "";
  const CmppKeyword * p = CmppKeyword_search(empty);

  // Either NULL or non-NULL depending on whether empty matches an entry; both are acceptable as long as no crash occurs.
  // We treat both outcomes as acceptable, but we still exercise the path.
  if (p == nullptr) {
    // Empty string not found - acceptable branch
    return true;
  } else {
    // Empty string found (unusual, but possible if array contains an empty-key entry)
    std::cout << "CmppKeyword_search returned non-NULL for empty string. Pointer: " << p << std::endl;
    return true;
  }
}

// Driver: run all tests and report summary
int main() {
  std::cout << "Starting CmppKeyword_search unit tests (C++11)...\n";

  // Run tests and accumulate results
  bool ok;

  ok = test_search_nonexistent_keyword_returns_null();
  EXPECT_TRUE(ok, "test_search_nonexistent_keyword_returns_null should return true");

  ok = test_search_known_keyword_returns_non_null();
  EXPECT_TRUE(ok, "test_search_known_keyword_returns_non_null should return true when a candidate keyword exists");

  ok = test_search_consistency_same_key_returns_same_pointer();
  EXPECT_TRUE(ok, "test_search_consistency_same_key_returns_same_pointer should return same pointer on repeated search");

  ok = test_search_empty_string_behavior();
  EXPECT_TRUE(ok, "test_search_empty_string_behavior should handle empty string without crashing");

  std::cout << "Tests completed. Passed " << g_tests_passed << " / " << g_tests_run << " tests.\n";

  // Return non-zero if any test failed to aid integration with build/test scripts
  return (g_tests_run == g_tests_passed) ? 0 : 1;
}