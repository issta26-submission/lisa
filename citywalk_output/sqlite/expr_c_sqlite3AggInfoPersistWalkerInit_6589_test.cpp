// Unit tests for sqlite3AggInfoPersistWalkerInit
// Target: Validate the focal function sets up a Walker correctly.
// Notes:
// - This test suite assumes the project provides sqliteInt.h (or equivalent)
//   which defines Walker, Parse and the related API.
// - No Google Test or other frameworks are used. A lightweight, non-terminating
//   assertion framework is implemented inline.
// - Tests are designed to be C++11 compatible and rely only on the standard library.

#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Domain knowledge: include project dependencies.
// It is expected that this header (or an equivalent one in the project) provides
// the declarations for Walker, Parse, and sqlite3AggInfoPersistWalkerInit.

// Lightweight test harness (non-terminating assertions)
static int g_test_count = 0;
static int g_failure_count = 0;

static void log_failure(const char* test_name, const char* message) {
    std::cerr << "[FAIL] " << test_name << ": " << message << std::endl;
    ++g_failure_count;
}

#define TEST_ASSERT(cond, message) do { \
    ++g_test_count; \
    if(!(cond)) log_failure(__FUNCTION__, message); \
} while(0)

#define TEST_RUN(test_func) do { test_func(); } while(0)

// ----------------------------------------------------------------------------------
// Test 1: Basic initialization semantics
// - Ensure that after sqlite3AggInfoPersistWalkerInit is invoked, the Walker's
//   pParse field points to the supplied Parse*, and the two callback pointers are non-null.
// - This covers the main side-effects of the function which are explicit in the source.
// ----------------------------------------------------------------------------------
static void test_sqlite3AggInfoPersistWalkerInit_sets_fields() {
    // Prepare a dummy Parse object (zero-initialized to simulate a valid Parse *)
    Parse pParseDummy;
    std::memset(&pParseDummy, 0, sizeof(pParseDummy));

    // Prepare a Walker object with non-zero sentinels to verify the function overwrites it.
    Walker w;
    // Fill with non-zero values to ensure memset in the function has an effect.
    std::memset(&w, 0xAA, sizeof(w));

    // Call the focal function
    sqlite3AggInfoPersistWalkerInit(&w, &pParseDummy);

    // Assertions
    TEST_ASSERT(w.pParse == &pParseDummy, "Walker.pParse should be set to the provided Parse*");
    TEST_ASSERT(w.xExprCallback != NULL, "Walker.xExprCallback should be non-null after init");
    TEST_ASSERT(w.xSelectCallback != NULL, "Walker.xSelectCallback should be non-null after init");

    // Optional: ensure these two callbacks are not accidentally equal (addresses are unlikely to be identical before init)
    // Note: This is a soft check since the exact function pointers are implementation-defined.
    TEST_ASSERT(w.xExprCallback != w.xSelectCallback, "Walker callbacks should point to distinct functions");
}

// ----------------------------------------------------------------------------------
// Test 2: Initialization with NULL Parse pointer
// - Ensure that even when pParse is NULL, the function still sets the callback pointers
//   and stores NULL in pParse as expected (to reflect the input).
// ----------------------------------------------------------------------------------
static void test_sqlite3AggInfoPersistWalkerInit_null_parse() {
    // Prepare a Walker with a known pattern to verify it gets cleared.
    Walker w;
    std::memset(&w, 0x55, sizeof(w));

    // Call the focal function with NULL parse
    sqlite3AggInfoPersistWalkerInit(&w, NULL);

    // Assertions
    TEST_ASSERT(w.pParse == NULL, "Walker.pParse should be NULL when NULL is passed as Parse*");
    TEST_ASSERT(w.xExprCallback != NULL, "Walker.xExprCallback should be non-null even if Parse* is NULL");
    TEST_ASSERT(w.xSelectCallback != NULL, "Walker.xSelectCallback should be non-null even if Parse* is NULL");
}

// ----------------------------------------------------------------------------------
// Main: Run all tests and report summary
// ----------------------------------------------------------------------------------
int main() {
    // Run tests
    TEST_RUN(test_sqlite3AggInfoPersistWalkerInit_sets_fields);
    TEST_RUN(test_sqlite3AggInfoPersistWalkerInit_null_parse);

    // Summary
    std::cout << "Tests run: " << g_test_count << ", Failures: " << g_failure_count << std::endl;
    if (g_failure_count == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED" << std::endl;
        return 1;
    }
}