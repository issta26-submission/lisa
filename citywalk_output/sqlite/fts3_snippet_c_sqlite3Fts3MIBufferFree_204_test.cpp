// Test suite for sqlite3Fts3MIBufferFree
// Target: validate behavior of freeing/memory handling logic for MatchinfoBuffer
// Note: This test avoids GTest and uses a lightweight main-based test harness.
// The production function sqlite3Fts3MIBufferFree is defined in fts3_snippet.c
// We mock sqlite3_free to observe internal behavior without terminating the test suite.

#include <fts3Int.h>
#include <string.h>
#include <assert.h>
#include <cstddef>
#include <iostream>


// 1) Domain-knowledge-aligned minimal type to match production usage.
// We declare a compatible struct layout for MatchinfoBuffer used by sqlite3Fts3MIBufferFree.
struct MatchinfoBuffer {
    int aRef[3];
};

// 2) Expose production function and sqlite3_free with C linkage to avoid name mangling.
extern "C" void sqlite3Fts3MIBufferFree(MatchinfoBuffer *p);
extern "C" void sqlite3_free(void* p); // production prototype (mocked in tests)

// 3) Test harness state to observe side effects without terminating the program.
static int g_free_call_count = 0;
static MatchinfoBuffer* g_last_freed_ptr = nullptr;

// Mock implementation of sqlite3_free used by the produced function.
// We intentionally avoid deleting to preserve test visibility of buffer state post-call.
extern "C" void sqlite3_free(void* p) {
    ++g_free_call_count;
    g_last_freed_ptr = static_cast<MatchinfoBuffer*>(p);
    // Do not delete to allow verification of p->aRef effects in tests.
}

// Utility to reset test state between tests
static void reset_test_state() {
    g_free_call_count = 0;
    g_last_freed_ptr = nullptr;
}

// 4) Individual test cases (non-terminating assertions are avoided; tests return bool).
//     - Each test includes explanatory comments describing the scenario and expected outcome.

bool test_FreeWhenAllZeroAfterSetToZero() {
    // Scenario: p != NULL, aRef = {1,0,0}; After call, aRef[0] becomes 0 and buffer should be freed
    reset_test_state();
    MatchinfoBuffer* p = new MatchinfoBuffer();
    p->aRef[0] = 1;
    p->aRef[1] = 0;
    p->aRef[2] = 0;

    sqlite3Fts3MIBufferFree(p);

    // Expect: sqlite3_free called exactly once and aRef[0] set to 0 (before or while freeing)
    bool pass = (g_free_call_count == 1) && (p->aRef[0] == 0);

    // Clean up test memory (we don't rely on real sqlite3_free to delete for visibility)
    delete p;
    return pass;
}

bool test_NoFreeWhenNotAllZeroAfterSetToZero() {
    // Scenario: p != NULL, aRef = {1,1,0}; After call, aRef[0] becomes 0 but not all refs zero => no free
    reset_test_state();
    MatchinfoBuffer* p = new MatchinfoBuffer();
    p->aRef[0] = 1;
    p->aRef[1] = 1;
    p->aRef[2] = 0;

    sqlite3Fts3MIBufferFree(p);

    // Expect: sqlite3_free not called and aRef[0] is 0 (since code sets it to 0)
    bool pass = (g_free_call_count == 0) && (p->aRef[0] == 0);

    delete p;
    return pass;
}

bool test_NullPointerDoesNothing() {
    // Scenario: p == NULL; should not invoke anything (no crash, no frees)
    reset_test_state();

    sqlite3Fts3MIBufferFree(NULL);

    // Expect: no free calls and no side effects
    bool pass = (g_free_call_count == 0) && (g_last_freed_ptr == nullptr);
    return pass;
}

// 5) Entry point
int main() {
    int total = 0;
    int passed = 0;

    std::cout << "Running sqlite3Fts3MIBufferFree unit tests (non-GTest C++11):\n";

    // Test 1
    total++;
    if (test_FreeWhenAllZeroAfterSetToZero()) {
        std::cout << "[PASS] test_FreeWhenAllZeroAfterSetToZero\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_FreeWhenAllZeroAfterSetToZero\n";
    }

    // Test 2
    total++;
    if (test_NoFreeWhenNotAllZeroAfterSetToZero()) {
        std::cout << "[PASS] test_NoFreeWhenNotAllZeroAfterSetToZero\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_NoFreeWhenNotAllZeroAfterSetToZero\n";
    }

    // Test 3
    total++;
    if (test_NullPointerDoesNothing()) {
        std::cout << "[PASS] test_NullPointerDoesNothing\n";
        ++passed;
    } else {
        std::cout << "[FAIL] test_NullPointerDoesNothing\n";
    }

    std::cout << "Summary: " << passed << " / " << total << " tests passed.\n";

    // Return non-zero on failure to assist automated tools
    return (passed == total) ? 0 : 1;
}