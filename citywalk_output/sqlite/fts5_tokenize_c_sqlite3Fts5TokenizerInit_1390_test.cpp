#include <vector>
#include <fts5Int.h>
#include <sqlite3.h>
#include <string>
#include <iostream>


extern "C" {
}

// Global state used by test mocks to simulate behavior and record interactions
static std::vector<std::string> g_calls; // Records tokenizer names passed to xCreateTokenizer
static int g_fail_after = -1;            // If >=0, fail on the N-th call (0-based)
static int g_call_count = 0;             // Counts how many times xCreateTokenizer has been invoked

static int g_assert_failures = 0;        // Non-terminating assertion failures counter

// Mock implementation of xCreateTokenizer used by tests.
// It records the tokenizer name and optionally simulates an error on a specific call.
extern "C" int mock_xCreateTokenizer(fts5_api *pApi,
                                   const char *zName,
                                   void *pCtx,
                                   fts5_tokenizer *ppOut,
                                   int nArg)
{
    // Record the tokenizer name for verification
    if (zName) g_calls.emplace_back(zName);
    else g_calls.emplace_back("");

    // Optional failure simulation: fail on a specific invocation
    if (g_fail_after >= 0 && g_call_count == g_fail_after) {
        ++g_call_count;
        return SQLITE_ERROR; // Simulated failure
    }

    ++g_call_count;
    return SQLITE_OK; // Success
}

// Lightweight non-terminating assertion macro.
// It reports failures but does not abort the test run.
#define ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "ASSERTION FAILED: " << msg << std::endl; \
        ++g_assert_failures; \
    } \
} while(0)

// Helper to reset per-test state
static void reset_test_state() {
    g_calls.clear();
    g_call_count = 0;
    // Do not reset g_fail_after here; tests set it explicitly before each run
}

// Test 1: Normal path where all builtins are successfully registered.
// Verifies that four calls are made with the expected tokenizer names in order.
static void test_normal_path() {
    reset_test_state();
    g_fail_after = -1; // Do not simulate failure

    // Prepare a fake API object with the mock xCreateTokenizer
    fts5_api api = {};
    api.xCreateTokenizer = mock_xCreateTokenizer;

    // Call the focal function
    int rc = sqlite3Fts5TokenizerInit(&api);

    // True path expectations
    ASSERT(rc == SQLITE_OK, "Normal path: rc should be SQLITE_OK");
    ASSERT(g_calls.size() == 4, "Normal path: expected 4 calls to xCreateTokenizer");

    // Verify the exact order and names
    const char* expected[] = { "unicode61", "ascii", "porter", "trigram" };
    for (size_t i = 0; i < 4; ++i) {
        ASSERT(i < g_calls.size() && g_calls[i] == expected[i],
               "Normal path: tokenizer name mismatch at position " + std::to_string(i));
    }
}

// Test 2: Simulate a failure on the second tokenizer registration (i.e., on i==1).
// Verifies that the function stops early and returns an error, with exactly two calls observed.
static void test_fail_on_second() {
    reset_test_state();
    g_fail_after = 1; // Fail on the second call (0-based index)

    fts5_api api = {};
    api.xCreateTokenizer = mock_xCreateTokenizer;

    int rc = sqlite3Fts5TokenizerInit(&api);

    // Expect an error and two attempts before failure
    ASSERT(rc != SQLITE_OK, "Fail-on-second: rc should indicate an error");
    ASSERT(g_calls.size() == 2, "Fail-on-second: expected 2 calls before failure");

    // Verify the first two tokenizer names were attempted
    const char* expected[] = { "unicode61", "ascii" };
    for (size_t i = 0; i < 2; ++i) {
        ASSERT(g_calls[i] == expected[i],
               "Fail-on-second: tokenizer name mismatch at position " + std::to_string(i));
    }
}

// Test 3: Simulate a failure on the first tokenizer registration (i==0).
// Verifies that the function stops immediately and returns an error, with exactly one call observed.
static void test_fail_on_first() {
    reset_test_state();
    g_fail_after = 0; // Fail on the first call

    fts5_api api = {};
    api.xCreateTokenizer = mock_xCreateTokenizer;

    int rc = sqlite3Fts5TokenizerInit(&api);

    // Expect an error on the very first registration
    ASSERT(rc != SQLITE_OK, "Fail-on-first: rc should indicate an error");
    ASSERT(g_calls.size() == 1, "Fail-on-first: expected only 1 call before failure");

    if (!g_calls.empty()) {
        ASSERT(g_calls[0] == "unicode61",
               "Fail-on-first: first tokenizer name should be unicode61");
    }
}

// Entry point that runs all tests using the provided main function pattern.
// This satisfies the requirement to call test methods from main without gtest.
int main() {
    // Run tests
    test_normal_path();
    test_fail_on_second();
    test_fail_on_first();

    if (g_assert_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_assert_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}