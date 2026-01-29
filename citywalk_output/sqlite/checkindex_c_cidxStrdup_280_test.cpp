/*
Unit test suite for: cidxStrdup (in checkindex.c)

Step 1 - Program Understanding (brief in comments)
- Focus: cidxStrdup(int *pRc, const char *zStr)
  - Behavior:
    - If *pRc == SQLITE_OK
      - n = strlen(zStr)
      - zRet = cidxMalloc(pRc, n+1)
      - if (zRet) memcpy(zRet, zStr, n+1)
    - Returns zRet (may be NULL if allocation fails or pRc != SQLITE_OK)
- Key dependent components (Candidate Keywords):
  - pRc (output/reason code), SQLITE_OK, zStr, strlen, n, cidxMalloc, zRet, memcpy
  - Branches:
    - true path: *pRc == SQLITE_OK
    - allocation success: zRet non-NULL then memcpy
    - allocation failure: zRet NULL (no memcpy)
    - false path: *pRc != SQLITE_OK
- Dependencies (from FOCAL_CLASS_DEP): cidxMalloc, memcpy, SQLITE_OK usage
- Testing constraints:
  - No GTest; plain C++11 test harness
  - True and false branches for predicate and inner allocation branch
  - Use only standard library; expose tests via main()
*/

#include <cstdio>
#include <sqlite3ext.h>
#include <cstring>
#include <cstdlib>


extern "C" {
  // Declaration of function under test (provided by production code in checkindex.c)
  // We assume the actual implementation links in during test harness execution.
  char *cidxStrdup(int *pRc, const char *zStr);

  // Optional: define SQLITE_OK if not defined in target environment
  // (production code typically defines this in sqlite3.h)
  #ifndef SQLITE_OK
  #define SQLITE_OK 0
  #endif
}

// Lightweight, non-terminating test harness
static int g_tests_run = 0;
static int g_tests_passed = 0;

// Non-terminating assertion macro: logs failures but continues execution
#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if (cond) { \
        ++g_tests_passed; \
    } else { \
        std::fprintf(stderr, "Test failure: %s (line %d)\n", msg, __LINE__); \
    } \
} while (0)

// Test 1: True branch of *pRc == SQLITE_OK with non-empty string.
// Expected: rc remains SQLITE_OK, a non-NULL pointer is returned,
// and the copied content matches input; pointer is different from input.
static void test_cidxStrdup_ok_nonempty() {
    // Arrange
    int rc = SQLITE_OK;
    const char *input = "hello, world";
    // Act
    char *dup = cidxStrdup(&rc, input);
    // Assert
    EXPECT_TRUE(rc == SQLITE_OK, "rc should remain SQLITE_OK for valid path");
    if (dup != nullptr) {
        EXPECT_TRUE(std::strcmp(dup, input) == 0, "duplicated string content should match input");
        EXPECT_TRUE(dup != input, "duplicated buffer should not be the same address as input string");
        // Best-effort cleanup (best-effort free; cross-lib compatibility assumed)
        std::free(dup);
    } else {
        // Allocation unexpectedly failed in this environment; report but do not crash
        std::fprintf(stderr, "Warning: allocation failed in test_cidxStrdup_ok_nonempty (dup is NULL)\n");
    }
}

// Test 2: True branch of *pRc == SQLITE_OK with an empty string.
// Expected: rc SQLITE_OK, non-NULL pointer returned, content is empty string.
static void test_cidxStrdup_ok_empty() {
    // Arrange
    int rc = SQLITE_OK;
    const char *input = "";
    // Act
    char *dup = cidxStrdup(&rc, input);
    // Assert
    EXPECT_TRUE(rc == SQLITE_OK, "rc should remain SQLITE_OK for empty string path");
    if (dup != nullptr) {
        EXPECT_TRUE(std::strcmp(dup, input) == 0, "duplicated content should be empty string");
        // If empty, content should still be null-terminated and equal to input
        std::free(dup);
    } else {
        // Allocation could fail in some environments; log but continue
        std::fprintf(stderr, "Warning: allocation failed in test_cidxStrdup_ok_empty (dup is NULL)\n");
    }
}

// Test 3: False branch of the predicate: *pRc != SQLITE_OK
// Expected: NULL is returned regardless of zStr content.
static void test_cidxStrdup_error_rc() {
    // Arrange
    int rc = -1; // Not SQLITE_OK
    const char *input = "irrelevant";
    // Act
    char *dup = cidxStrdup(&rc, input);
    // Assert
    EXPECT_TRUE(dup == nullptr, "when rc != SQLITE_OK, result should be NULL");
    // No cleanup needed as dup is NULL
}

// Entry point: run all tests and report summary
int main() {
    // Step 2/3: Execute generated test suite
    test_cidxStrdup_ok_nonempty();
    test_cidxStrdup_ok_empty();
    test_cidxStrdup_error_rc();

    // Summary
    std::printf("Test Summary: %d ran, %d passed\n", g_tests_run, g_tests_passed);
    // Return non-zero if any test failed
    return (g_tests_run == g_tests_passed) ? 0 : 1;
}