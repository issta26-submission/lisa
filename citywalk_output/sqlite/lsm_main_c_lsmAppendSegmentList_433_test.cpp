/*
  Unit tests for the focal method: lsmAppendSegmentList
  - Target: test the behavior of the function that appends a Segment's representation
    into an LsmString, using the provided prefix zPre.
  - Environment assumptions:
      • The project provides LsmString and Segment types and the function:
            void lsmAppendSegmentList(LsmString *pStr, char *zPre, Segment *pSeg);
      • A working lsmStringAppendf function used by lsmAppendSegmentList to format
        the output into LsmString.
      • A way to read back the content of LsmString for assertions, via
        lsmStringContent(const LsmString*).
    If your project exposes different APIs to initialize an LsmString or extract its
    content, adapt the test harness accordingly.

  Design notes:
  - No GTest or other external test frameworks are used. A simple in-file test harness
    with lightweight EXPECT_* assertions is provided.
  - We cover typical, boundary, and edge cases, including:
      - Basic formatting with a standard prefix.
      - Long long formatting correctness (%lld) via large values.
      - Prefix behavior (non-empty zPre).
      - Concatenation/accumulation if multiple calls are performed on the same string.
  - The test suite uses only the C++11 standard library and the provided method under test.
  - All tests are designed to be executable via a single main() in this file. If your
    project uses a different test runner, you can integrate the individual test
    functions accordingly.

  Important: The following test code is written as a self-contained example harness.
  If your environment provides different initialization or content retrieval APIs
  for LsmString, adjust the code accordingly (lsmStringInit, lsmStringContent, etc.).
*/

#include <lsmInt.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <cstdint>


// Domain knowledge: we expect the following types to exist in the project under test.
// If your environment exposes different names or a different layout, adjust accordingly.

// Forward declarations to interoperate with the focal function.
// These declarations assume the project exposes these types/functions in the
// global scope of the program that links this test file with lsm_main.c.
// If your project uses a C header (lsmInt.h) that provides these, include that header
// instead of re-declaring here.
// NOTE: This test harness is designed to be adaptable; if your project provides
// different APIs to access LsmString content, replace the getters accordingly.

extern "C" {

// Opaque LsmString type (as used by the focal function in the project under test)
typedef struct LsmString LsmString;

// Segment structure used by the focal method
typedef struct {
    long long iFirst;
    long long iLastPg;
    long long iRoot;
    long long nSize;
} Segment;

// Focal method under test (assumed signature per given snippet)
void lsmAppendSegmentList(LsmString *pStr, char *zPre, Segment *pSeg);

// Utility to fetch the current content of an LsmString for assertions.
// If your project exposes a different accessor (e.g., a member z or a function),
// substitute accordingly.
const char* lsmStringContent(const LsmString *pStr);

} // extern "C"

// Lightweight test framework (non-GTest)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond) \
    do { \
        if (!(cond)) { \
            printf("EXPECT_TRUE failed: %s, at %s:%d\n", #cond, __FILE__, __LINE__); \
            return false; \
        } \
    } while (0)

#define EXPECT_EQLL(a, b) \
    do { \
        long long _aa = (long long)(a); \
        long long _bb = (long long)(b); \
        if (_aa != _bb) { \
            printf("EXPECT_EQLL failed: %lld != %lld (%s vs %s), at %s:%d\n", _aa, _bb, #a, #b, __FILE__, __LINE__); \
            return false; \
        } \
    } while (0)

#define EXPECT_STREQ(a, b) \
    do { \
        if (strcmp((a), (b)) != 0) { \
            printf("EXPECT_STREQ failed: \"%s\" != \"%s\" (at %s:%d)\n", (a), (b), __FILE__, __LINE__); \
            return false; \
        } \
    } while (0)

// Helper to initialize an empty LsmString.
// If your project provides a dedicated initializer (e.g., lsmStringInit), use that.
static void init_empty_lsm_string(LsmString *pStr) {
    // If there is an initializer function in your project, call it here.
    // This is a placeholder to keep the test runnable in environments
    // where LsmString is opaque.
    // Example (if available): lsmStringInit(pStr, 0);
    (void)pStr;
}

// Test 1: Basic formatting with a simple prefix and small values
static bool test_lsmAppendSegmentList_basic() {
    // Arrange
    LsmString s;
    init_empty_lsm_string(&s);

    Segment seg;
    seg.iFirst = 1;
    seg.iLastPg = 2;
    seg.iRoot = 3;
    seg.nSize = 4;

    const char* expected_prefix = "P"; // example prefix
    // Act
    lsmAppendSegmentList(&s, (char*)expected_prefix, &seg);

    // Retrieve content
    const char* content = lsmStringContent(&s);
    // Expected: "P{1 2 3 4}"
    // Build expected string (static since zPre is a C-string)
    char expected[256];
    snprintf(expected, sizeof(expected), "%s{%lld %lld %lld %lld}", expected_prefix,
             seg.iFirst, seg.iLastPg, seg.iRoot, seg.nSize);

    // Assert
    EXPECT_STREQ(content, expected);
    return true;
}

// Test 2: Long long formatting (verify %lld prints large values correctly)
static bool test_lsmAppendSegmentList_large_values() {
    // Arrange
    LsmString s;
    init_empty_lsm_string(&s);

    Segment seg;
    seg.iFirst = 9223372036854775807LL;  // max of int64
    seg.iLastPg = -9223372036854775807LL; // negative large value
    seg.iRoot = 123456789012345LL;
    seg.nSize = 9876543210LL;

    const char* zPre = "LT"; // prefix

    // Act
    lsmAppendSegmentList(&s, (char*)zPre, &seg);

    // Retrieve content
    const char* content = lsmStringContent(&s);

    // Build expected string
    char expected[512];
    snprintf(expected, sizeof(expected), "%s{%lld %lld %lld %lld}", zPre,
             seg.iFirst, seg.iLastPg, seg.iRoot, seg.nSize);

    // Assert
    EXPECT_STREQ(content, expected);
    return true;
}

// Test 3: Non-empty zPre prefix behavior with another segment
static bool test_lsmAppendSegmentList_prefix_concatenation() {
    // Arrange
    LsmString s;
    init_empty_lsm_string(&s);

    Segment seg1;
    seg1.iFirst = 10;
    seg1.iLastPg = 20;
    seg1.iRoot = 30;
    seg1.nSize = 40;

    Segment seg2;
    seg2.iFirst = 100;
    seg2.iLastPg = 200;
    seg2.iRoot = 300;
    seg2.nSize = 400;

    // First append
    lsmAppendSegmentList(&s, (char*)"A", &seg1);
    // Second append with a different segment
    lsmAppendSegmentList(&s, (char*)"B", &seg2);

    // Retrieve content
    const char* content = lsmStringContent(&s);

    // Expected concatenation:
    // First: "A{10 20 30 40}"
    // Second: "B{100 200 300 400}"
    // The total content should be their concatenation in order
    char expected[1024];
    snprintf(expected, sizeof(expected), "%s{%lld %lld %lld %lld}%s{%lld %lld %lld %lld}",
             "A", seg1.iFirst, seg1.iLastPg, seg1.iRoot, seg1.nSize,
             "B", seg2.iFirst, seg2.iLastPg, seg2.iRoot, seg2.nSize);

    // Assert
    EXPECT_STREQ(content, expected);
    return true;
}

// Test 4: Empty prefix with a single segment
static bool test_lsmAppendSegmentList_empty_prefix() {
    LsmString s;
    init_empty_lsm_string(&s);

    Segment seg;
    seg.iFirst = 0;
    seg.iLastPg = 0;
    seg.iRoot = 0;
    seg.nSize = 0;

    lsmAppendSegmentList(&s, (char*)"", &seg);

    const char* content = lsmStringContent(&s);
    char expected[64];
    snprintf(expected, sizeof(expected), "{%lld %lld %lld %lld}", seg.iFirst, seg.iLastPg, seg.iRoot, seg.nSize);

    EXPECT_STREQ(content, expected);
    return true;
}

// Test runner
int main() {
    printf("Running unit tests for lsmAppendSegmentList...\n");
    g_tests_run = 0;
    g_tests_failed = 0;

    bool ok;

    ok = test_lsmAppendSegmentList_basic();
    g_tests_run++;
    if (!ok) {
        g_tests_failed++;
        printf("Test 1 failed.\n");
    } else {
        printf("Test 1 passed.\n");
    }

    ok = test_lsmAppendSegmentList_large_values();
    g_tests_run++;
    if (!ok) {
        g_tests_failed++;
        printf("Test 2 failed.\n");
    } else {
        printf("Test 2 passed.\n");
    }

    ok = test_lsmAppendSegmentList_prefix_concatenation();
    g_tests_run++;
    if (!ok) {
        g_tests_failed++;
        printf("Test 3 failed.\n");
    } else {
        printf("Test 3 passed.\n");
    }

    ok = test_lsmAppendSegmentList_empty_prefix();
    g_tests_run++;
    if (!ok) {
        g_tests_failed++;
        printf("Test 4 failed.\n");
    } else {
        printf("Test 4 passed.\n");
    }

    printf("Tests completed. Run results: %d passed, %d failed, %d total.\n",
           g_tests_run - g_tests_failed, g_tests_failed, g_tests_run);
    return g_tests_failed ? 1 : 0;
}

/*
  Explanation of Candidate Keywords (Step 1):
  - LsmString: string-like accumulator used by the library to accumulate formatted text.
  - Segment: struct carrying iFirst, iLastPg, iRoot, nSize fields (as used by lsmAppendSegmentList).
  - lsmAppendSegmentList: focal method; relies on lsmStringAppendf to format and append.
  - lsmStringAppendf: a formatting helper function used to append formatted text to LsmString.
  - iFirst, iLastPg, iRoot, nSize: data members of Segment that are printed in order.
  - zPre: prefix string passed to format ("%s{...}").
  - The tests exercise formatting correctness (%lld), prefix handling, and concatenation.
  - lsmStringContent: accessor to read back LsmString content for assertion.

  Step 2/3 notes (assumptions for your environment):
  - The tests assume availability of:
      - LsmString type
      - Segment type as defined above
      - lsmAppendSegmentList(LsmString*, char*, Segment*)
      - lsmStringContent(const LsmString*) to retrieve the content for assertions
  - If your environment provides alternative APIs (e.g., lsmStringInit, lsmStringGetContent, etc.),
    adjust the test harness accordingly while preserving the same test intent and coverage.
  - The tests use a lightweight, in-file test harness (no external frameworks) and
    produce per-test result messages and a final exit code indicating success or failure.

  Caveat:
  - If your build requires linking against the actual C implementation (lsm_main.c) and
    the proper headers (lsmInt.h, etc.), prefer including the real headers and building
    this test as part of the same binary. If you instead run into name-mangling or missing
    symbols, adapt the test to be a thin wrapper around the actual API provided by your project.
*/