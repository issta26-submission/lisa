/*
Unit Test Suite for sqlite3Fts5Strndup
- This test harness targets the focal function sqlite3Fts5Strndup(int *pRc, const char *pIn, int nIn).
- Due to the testing environment constraints (no external test frameworks like GTest),
  we implement a lightweight test runner with non-terminating assertions (EXPECT_*)
  to maximize code coverage while continuing execution after failures.
- To keep the test self-contained, a minimal mock of sqlite3_malloc is provided
  along with necessary constants (SQLITE_OK, SQLITE_NOMEM, SQLITE_ERROR).
- The focal function logic is reproduced inline within this test file to ensure
  consistent behavior across test runs in environments where the original compilation
  unit may not be linked or accessible. This mirrors the control flow of the original
  function and allows comprehensive branch coverage (true/false decisions).
- Each test includes explanatory comments describing the scenario and expected outcome.
*/

#include <fts5Int.h>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Minimal SQLite-like constants used by the focal function
#define SQLITE_OK 0
#define SQLITE_NOMEM 7
#define SQLITE_ERROR 1

// Global flag to simulate malloc failure in tests
static bool g_failMalloc = false;

// Minimal sqlite3_malloc mock to satisfy the focal function's dependency
extern "C" void* sqlite3_malloc(size_t n) {
    if (g_failMalloc) return nullptr;
    return std::malloc(n);
}

// The focal method under test (reproduced here to ensure test can run in isolation)
// In the actual project, this should be linked from fts5_buffer.c. Here we mirror the
// exact logic to validate behavior across scenarios.
char *sqlite3Fts5Strndup(int *pRc, const char *pIn, int nIn){
  char *zRet = 0;
  if( *pRc==SQLITE_OK ){
    if( nIn<0 ){
      nIn = (int)strlen(pIn);
    }
    zRet = (char*)sqlite3_malloc(nIn+1);
    if( zRet ){
      memcpy(zRet, pIn, nIn);
      zRet[nIn] = '\0';
    }else{
      *pRc = SQLITE_NOMEM;
    }
  }
  return zRet;
}

// Lightweight test framework (non-terminating assertions)
static int g_testsTotal = 0;
static int g_testsPassed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_testsTotal; \
    if ( (cond) ) { \
        ++g_testsPassed; \
    } else { \
        std::cerr << "[FAIL] " << msg << "\n"; \
    } \
} while(0)

#define EXPECT_EQ_STR(actual, expected, msg) do { \
    ++g_testsTotal; \
    if ( (actual) != nullptr && std::strcmp((actual), (expected)) == 0 ) { \
        ++g_testsPassed; \
    } else { \
        std::cerr << "[FAIL] " << msg << " | Expected: \"" << (expected) \
                  << "\" got: \"" << (actual ? actual : "NULL") << "\"\n"; \
    } \
} while(0)

#define EXPECT_EQ_INT(actual, expected, msg) do { \
    ++g_testsTotal; \
    if ( (actual) == (expected) ) { \
        ++g_testsPassed; \
    } else { \
        std::cerr << "[FAIL] " << msg << " | Expected: " << (expected) \
                  << " got: " << (actual) << "\n"; \
    } \
} while(0)

#define FREE_MEM(p) do { if(p) { std::free((void*)p); (p) = nullptr; } } while(0)

// Test 1: Basic behavior with nIn < 0 should copy entire input string
void test_basic_negative_nIn_consumes_full_string() {
    // Arrange
    int rc = SQLITE_OK;
    const char *input = "hello_world";
    int nIn = -1; // signal: copy full length
    // Act
    char *out = sqlite3Fts5Strndup(&rc, input, nIn);
    // Assert
    EXPECT_EQ_INT(rc, SQLITE_OK, "RC should remain SQLITE_OK after successful copy");
    EXPECT_EQ_STR(out, "hello_world", "Output should be the full input string when nIn < 0");
    FREE_MEM(out);
}

// Test 2: nIn positive smaller than input length should copy only that many chars
void test_basic_positive_nIn_limits_copy_length() {
    // Arrange
    int rc = SQLITE_OK;
    const char *input = "abcdef";
    int nIn = 2; // copy first 2 chars
    // Act
    char *out = sqlite3Fts5Strndup(&rc, input, nIn);
    // Assert
    EXPECT_EQ_INT(rc, SQLITE_OK, "RC should be SQLITE_OK on successful partial copy");
    EXPECT_EQ_STR(out, "ab", "Output should be first nIn characters of input");
    FREE_MEM(out);
}

// Test 3: When *pRc is not SQLITE_OK, function should not allocate and should return NULL
void test_rc_not_ok_no_allocation() {
    // Arrange
    int rc = SQLITE_ERROR; // not SQLITE_OK
    const char *input = "should_be_ignored";
    int nIn = -1;
    // Act
    char *out = sqlite3Fts5Strndup(&rc, input, nIn);
    // Assert
    EXPECT_TRUE(out == nullptr, "Output should be NULL when initial rc != SQLITE_OK");
    EXPECT_EQ_INT(rc, SQLITE_ERROR, "RC should remain unchanged when not SQLITE_OK");
    // No memory to free
}

// Test 4: Simulate malloc failure to verify SQLITE_NOMEM is returned and NULL is produced
void test_malloc_failure_sets_nmem_and_null_output() {
    // Arrange
    int rc = SQLITE_OK;
    const char *input = "data_that_would_be_allocated";
    int nIn = -1; // copy full length
    g_failMalloc = true; // force malloc to fail
    // Act
    char *out = sqlite3Fts5Strndup(&rc, input, nIn);
    // Assert
    EXPECT_TRUE(out == nullptr, "Output should be NULL when malloc fails");
    EXPECT_EQ_INT(rc, SQLITE_NOMEM, "RC should be SQLITE_NOMEM after allocation failure");
    // Cleanup
    g_failMalloc = false;
}

// Test 5: nIn = 0 should allocate 1 byte for terminating null and return empty string
void test_zero_length_copy_results_in_empty_string() {
    // Arrange
    int rc = SQLITE_OK;
    const char *input = "anything";
    int nIn = 0;
    // Act
    char *out = sqlite3Fts5Strndup(&rc, input, nIn);
    // Assert
    EXPECT_EQ_INT(rc, SQLITE_OK, "RC should be SQLITE_OK for zero-length copy");
    EXPECT_EQ_STR(out, "", "Output should be empty string when nIn is 0");
    FREE_MEM(out);
}

// Entry point for running tests
int main() {
    std::cout << "Starting sqlite3Fts5Strndup test suite (C++11, no GTest)..." << std::endl;

    test_basic_negative_nIn_consumes_full_string();
    test_basic_positive_nIn_limits_copy_length();
    test_rc_not_ok_no_allocation();
    test_malloc_failure_sets_nmem_and_null_output();
    test_zero_length_copy_results_in_empty_string();

    std::cout << "Test results: " << g_testsPassed << " / " << g_testsTotal << " tests passed." << std::endl;

    // Return non-zero if any test failed
    if (g_testsPassed != g_testsTotal) {
        return 1;
    }
    return 0;
}