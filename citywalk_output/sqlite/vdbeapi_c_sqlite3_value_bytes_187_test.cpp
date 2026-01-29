// Unit test suite for sqlite3_value_bytes (wrapper around sqlite3ValueBytes(..., SQLITE_UTF8))
// This test targets the focal method from the provided code and its direct dependency
// sqlite3ValueBytes. We mock sqlite3ValueBytes to validate that sqlite3_value_bytes
// forwards the correct parameters (pointer and encoding) and returns the same value.
//
// Notes based on domain knowledge:
// - We test with non-null and null pointers to ensure call-through behavior.
// - We verify that the encoding parameter equals SQLITE_UTF8.
// - We implement a lightweight test harness without GoogleTest; tests are non-terminating
//   and report failures to stdout/stderr for coverage robustness.
//
// Candidate Keywords (Step 1/2):
// - sqlite3_value_bytes
// - sqlite3ValueBytes
// - SQLITE_UTF8
// - sqlite3_value
// - forwarding/call-through behavior
// - null pointer handling
// - non-terminating test assertions

#include <iostream>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <opcodes.h>


// Forward declare the minimal type used by the focal function pointer
struct sqlite3_value;

// Encoding constant (must align with the production code's SQLITE_UTF8 value)
#ifndef SQLITE_UTF8
#define SQLITE_UTF8 1
#endif

// Global state captured by our mock of sqlite3ValueBytes to inspect test behavior
sqlite3_value* g_lastValue = nullptr;
int g_lastEnc = -1;

// Mock implementation of sqlite3ValueBytes used by sqlite3_value_bytes
// This function intercepts the call from the focal method and records inputs.
// It must be C linkage to match the production symbol.
extern "C" int sqlite3ValueBytes(sqlite3_value* pVal, int enc) {
    g_lastValue = pVal;
    g_lastEnc = enc;
    // Return a deterministic value so tests can verify the wrapper propagates it
    return 0x1234;
}

// Prototype of the focal method under test (produced by the project under test)
extern "C" int sqlite3_value_bytes(struct sqlite3_value *pVal);

// Lightweight test harness (non-terminating assertions)
static int g_totalChecks = 0;
static int g_failedChecks = 0;
#define CHECK(cond, msg) do { \
    ++g_totalChecks; \
    if(!(cond)) { \
        ++g_failedChecks; \
        std::cerr << "CHECK FAILED: " << (msg) << std::endl; \
    } \
} while(0)

// Test 1: Basic non-null input ensures forwarding of pVal and correct encoding
void test_basic_non_null_input() {
    sqlite3_value val;
    g_lastValue = nullptr;
    g_lastEnc = -1;

    int ret = sqlite3_value_bytes(&val);

    CHECK(ret == 0x1234, "Return value should be the mock value 0x1234 for non-null input");
    CHECK(g_lastValue == &val, "sqlite3ValueBytes should receive the original pointer from wrapper");
    CHECK(g_lastEnc == SQLITE_UTF8, "Encoding passed to sqlite3ValueBytes should be SQLITE_UTF8");
}

// Test 2: Null pointer input checks that wrapper forwards NULL as pVal
void test_null_pointer_input() {
    g_lastValue = nullptr;
    g_lastEnc = -1;

    int ret = sqlite3_value_bytes(nullptr);

    CHECK(ret == 0x1234, "Return value should be the mock value 0x1234 for null input (established by mock)");
    CHECK(g_lastValue == nullptr, "Wrapper should forward NULL pointer unchanged to sqlite3ValueBytes");
    CHECK(g_lastEnc == SQLITE_UTF8, "Encoding should still be SQLITE_UTF8 for null input");
}

// Test 3: Multiple consecutive calls ensure wrapper consistency and state isolation
void test_multiple_consecutive_calls() {
    sqlite3_value a, b;
    g_lastValue = nullptr;
    g_lastEnc = -1;

    int ret1 = sqlite3_value_bytes(&a);
    CHECK(ret1 == 0x1234, "First call returns mock value 0x1234");
    CHECK(g_lastValue == &a, "First call should pass &a as pVal");
    CHECK(g_lastEnc == SQLITE_UTF8, "First call encoding remains SQLITE_UTF8");

    int ret2 = sqlite3_value_bytes(&b);
    CHECK(ret2 == 0x1234, "Second call returns mock value 0x1234");
    CHECK(g_lastValue == &b, "Second call should pass &b as pVal");
    CHECK(g_lastEnc == SQLITE_UTF8, "Second call encoding remains SQLITE_UTF8");
}

// Helper to summarize test results
void report_summary() {
    std::cout << "Test checks performed: " << g_totalChecks
              << ", Failures: " << g_failedChecks << std::endl;
    if(g_failedChecks > 0){
        std::cout << "Some tests failed. See stderr for details." << std::endl;
    } else {
        std::cout << "All tests passed." << std::endl;
    }
}

// Main entry point to run tests (non-GTest, self-contained)
int main() {
    // Run tests
    test_basic_non_null_input();
    test_null_pointer_input();
    test_multiple_consecutive_calls();

    // Report results
    report_summary();

    // Return non-zero if any test failed to aid integration in CI scripts
    return (g_failedChecks > 0) ? 1 : 0;
}