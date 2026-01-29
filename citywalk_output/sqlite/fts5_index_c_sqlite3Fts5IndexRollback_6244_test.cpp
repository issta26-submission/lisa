// Test suite for sqlite3Fts5IndexRollback
// This test focuses on verifying that the function:
//   int sqlite3Fts5IndexRollback(Fts5Index *p)
// correctly invokes its dependencies in the expected order and
// returns SQLITE_OK. We mock the dependent C functions using weak
// symbols to avoid linkage conflicts with the actual implementation
// in fts5_index.c.

#include <vector>
#include <fts5Int.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <cassert>


// Domain knowledge notes implemented as comments above tests.
// - We target the focal function sqlite3Fts5IndexRollback in fts5_index.c.
// - Dependencies exercised by the focal function are:
//     sqlite3Fts5IndexCloseReader, fts5IndexDiscardData, fts5StructureInvalidate
// - We record the sequence of calls to these dependencies to validate order.
// - We use weak symbol overrides so that, if the real definitions exist in
//   the linked library, they will take precedence; otherwise, our test
//   stubs will be used.

extern "C" {

// Forward declare the minimal type used by the function under test.
// We do not rely on the internal layout of Fts5Index here.
typedef struct Fts5Index Fts5Index;

// Declaration of the function under test (provided by the production code).
int sqlite3Fts5IndexRollback(Fts5Index *p);

// Dependency functions (we provide weak overrides so that the linker
// will use our stubs if the real ones are not present, or override them).
// If the production code provides strong definitions, these weak aliases
// will be ignored in favor of the production implementations.

#ifdef __GNUC__
#define WEAK_ATTR __attribute__((weak))
#else
#define WEAK_ATTR
#endif

// Record of calls made by the rollback function
extern std::vector<std::string> g_calls;

// Weak override for sqlite3Fts5IndexCloseReader
void WEAK_ATTR sqlite3Fts5IndexCloseReader(Fts5Index *p);

// Weak override for fts5IndexDiscardData
void WEAK_ATTR fts5IndexDiscardData(Fts5Index *p);

// Weak override for fts5StructureInvalidate
void WEAK_ATTR fts5StructureInvalidate(Fts5Index *p);

} // extern "C"


// Global call log
std::vector<std::string> g_calls;

// Weak override implementations (collect call names in g_calls)
void WEAK_ATTR sqlite3Fts5IndexCloseReader(Fts5Index *p) {
    (void)p; // suppress unused warning if not used
    g_calls.push_back("CloseReader");
}
void WEAK_ATTR fts5IndexDiscardData(Fts5Index *p) {
    (void)p;
    g_calls.push_back("DiscardData");
}
void WEAK_ATTR fts5StructureInvalidate(Fts5Index *p) {
    (void)p;
    g_calls.push_back("StructureInvalidate");
}

// Simple non-terminating test helpers
static int g_total = 0;
static int g_failed = 0;

static void test_pass(const std::string &name) {
    std::cout << "[PASS] " << name << "\n";
}
static void test_fail(const std::string &name) {
    std::cout << "[FAIL] " << name << "\n";
    ++g_failed;
}
static void expect_eq(const std::string &desc, int a, int b) {
    ++g_total;
    if (a == b) {
        test_pass(desc);
    } else {
        test_fail(desc);
        std::cout << "  Expected: " << b << ", Actual: " << a << "\n";
    }
}
static void expect_seq(const std::string &desc,
                       const std::vector<std::string> &actual,
                       const std::vector<std::string> &expected) {
    ++g_total;
    if (actual == expected) {
        test_pass(desc);
    } else {
        test_fail(desc);
        std::cout << "  Sequence mismatch. Expected: [";
        for (size_t i = 0; i < expected.size(); ++i) {
            if (i) std::cout << ", ";
            std::cout << expected[i];
        }
        std::cout << "], Actual: [";
        for (size_t i = 0; i < actual.size(); ++i) {
            if (i) std::cout << ", ";
            std::cout << actual[i];
        }
        std::cout << "]\n";
    }
}

// Test 1: Normal rollback should call dependencies in order and return SQLITE_OK
// This verifies:
//   1) The function returns SQLITE_OK (0).
//   2) The three dependency calls are made exactly once.
//   3) The order of calls is CloseReader -> DiscardData -> StructureInvalidate.
static void test_rollback_calls_in_order() {
    g_calls.clear();
    Fts5Index dummy; // opaque in test; layout not relied upon

    int rc = sqlite3Fts5IndexRollback(&dummy);

    // SQLITE_OK is usually 0 in sqlite3; we rely on the conventional value.
    expect_eq("Return code is SQLITE_OK", rc, 0);
    std::vector<std::string> expected = {"CloseReader", "DiscardData", "StructureInvalidate"};
    expect_seq("Call order: CloseReader -> DiscardData -> StructureInvalidate",
               g_calls, expected);
}

// Test 2: Rollback with null pointer should still execute and return SQLITE_OK
// This validates how the function behaves when given a null p. The test ensures
// the same dependency sequence is produced, demonstrating that the function
// doesn't perform undefined behavior based solely on a null input in this path.
static void test_rollback_with_null_pointer() {
    g_calls.clear();
    Fts5Index *p-nullptr = nullptr; // forcing a null pointer scenario

    int rc = sqlite3Fts5IndexRollback(p-nullptr);

    expect_eq("Return code with null pointer is SQLITE_OK", rc, 0);
    std::vector<std::string> expected = {"CloseReader", "DiscardData", "StructureInvalidate"};
    expect_seq("Call order with null pointer", g_calls, expected);
}

// Entry point: run tests
int main() {
    // Run tests to validate the focal method behavior.
    test_rollback_calls_in_order();
    test_rollback_with_null_pointer();

    // Summary
    std::cout << "Summary: " << (g_total - g_failed) << " passed, "
              << g_failed << " failed, out of " << g_total << " tests.\n";

    return g_failed ? 1 : 0;
}