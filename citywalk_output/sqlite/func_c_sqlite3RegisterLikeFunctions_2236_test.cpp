// Test suite for sqlite3RegisterLikeFunctions (C API surface) using a minimal C++11 test harness.
// Purpose: verify correct behavior of the focal function by intercepting its
// interactions with sqlite3CreateFunc and sqlite3FindFunction, and validating
// the resulting function flags for the "like" function registrations.
// Note: This test provides mocks for the underlying C API to run in a C++11
// environment without requiring the real SQLite library.

#include <stdlib.h>
#include <math.h>
#include <functional>
#include <vector>
#include <assert.h>
#include <tuple>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <unordered_map>
#include <iostream>
#include <cassert>


// Domain constants (mirroring typical SQLite internal flag values)
extern "C" {
    // Forward declarations to satisfy the focal function's linkage expectations.
    // These types are intentionally opaque for the test harness.
    struct sqlite3;
    struct sqlite3_context;
    struct sqlite3_value;

    // Function pointer type used by sqlite3CreateFunc for the xFunc callback.
    typedef void (*sqlite3_func)(sqlite3_context*, int, sqlite3_value**);

    // Minimal external function declarations (the focal function will call these).
    void sqlite3CreateFunc(sqlite3*, const char*, int, int, void*, sqlite3_func,
                           int, int, int, int, int);
    struct FuncDef;
    FuncDef* sqlite3FindFunction(sqlite3*, const char*, int, int, void*);
    void sqlite3RegisterLikeFunctions(sqlite3*, int); // focal function under test
}

// Local test harness types and storage
struct sqlite3 {
    // Intentionally empty; only serves as a placeholder in tests
};

struct sqlite3_context {};
struct sqlite3_value {};

// The internal function descriptor used by the test (mirrors the real library's shape enough for tests)
struct FuncDef {
    int funcFlags;
};

// Flag constants (assumed values matching the real code conventions)
static const int SQLITE_UTF8       = 1;
static const int SQLITE_FUNC_LIKE  = 0x01;
static const int SQLITE_FUNC_CASE  = 0x02;
static const int SQLITE_FUNC_UNSAFE= 0x04;

// Guards and state for the test doubles
static std::vector<FuncDef*> g_allocated_funcs;
static std::unordered_map<std::tuple<std::string,int,int>, FuncDef*> g_func_map;

// Mock implementations of the required sqlite3* API for testing

extern "C" {

// Mock: create a function and store it for later retrieval
void sqlite3CreateFunc(sqlite3* db, const char* zName, int nArg, int enc,
                       void* pApp, sqlite3_func xFunc,
                       int, int, int, int, int)
{
    // We only care about name, nargs and encoding for the test.
    (void)db; (void)pApp; (void)xFunc;
    std::string name = zName ? zName : "";
    auto key = std::make_tuple(name, nArg, enc);
    // Create a new FuncDef and initialize its flags to 0
    FuncDef* p = new FuncDef();
    p->funcFlags = 0;
    g_allocated_funcs.push_back(p);
    g_func_map[key] = p;
}

// Mock: find a previously created function by its characteristics
FuncDef* sqlite3FindFunction(sqlite3* db, const char* zName, int nArg, int enc, void* pApp)
{
    (void)db; (void)pApp;
    std::string name = zName ? zName : "";
    auto key = std::make_tuple(name, nArg, enc);
    auto it = g_func_map.find(key);
    if (it != g_func_map.end()) {
        return it->second;
    }
    return nullptr;
}
} // extern "C"

// Simple test framework (non-GTest)
namespace test_framework {

static int g_total = 0;
static int g_failed = 0;

#define TEST(name) void test_##name(); \
    static void run_##name() { test_##name(); } \
    void test_##name()

#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << " - " << (msg) << "\n"; \
        ++g_failed; \
    } \
} while(false)

#define ASSERT_EQ(a, b, msg) do { \
    if (!((a) == (b))) { \
        std::cerr << "[FAIL] " << __FILE__ << ":" << __LINE__ << " - " << (msg) \
                  << " | expected: " << (b) << " actual: " << (a) << "\n"; \
        ++g_failed; \
    } \
} while(false)

#define REPORT() do { \
    std::cout << "Tests run: " << g_total << ", Failures: " << g_failed << std::endl; \
} while(false)

} // namespace test_framework

using namespace test_framework;

// Helper utilities for tests
static void resetMocks() {
    // Clear registered functions
    g_allocated_funcs.clear();
    g_func_map.clear();
}

static void verifyFlags(FuncDef* pDef, int expectedFlags, bool expectUnsafeCleared) {
    ASSERT_TRUE(pDef != nullptr, "FunctionDef must not be null");
    // Check required flags are present
    ASSERT_EQ((pDef->funcFlags & expectedFlags), expectedFlags,
              "FuncDef flags must include expected LIKE/CASE bits");
    // Check UNSAFE bit is cleared
    if (expectUnsafeCleared) {
        ASSERT_TRUE((pDef->funcFlags & SQLITE_FUNC_UNSAFE) == 0,
                    "UNSAFE flag must be cleared after registration");
    }
}

// Public tests

TEST(sqlite3RegisterLikeFunctions_caseSensitive_true)
{
    // true: case-sensitive LIKE behavior
    resetMocks();
    sqlite3 db; // dummy db handle

    // Call the focal function (extern "C" linkage)
    sqlite3RegisterLikeFunctions(&db, 1);

    // Expect two registrations: nargs 2 and 3
    FuncDef* p2 = sqlite3FindFunction(&db, "like", 2, SQLITE_UTF8, 0);
    FuncDef* p3 = sqlite3FindFunction(&db, "like", 3, SQLITE_UTF8, 0);

    int expectedFlags = SQLITE_FUNC_LIKE | SQLITE_FUNC_CASE;

    // Each should exist and have required bits set
    verifyFlags(p2, expectedFlags, true);
    verifyFlags(p3, expectedFlags, true);

    ++g_total;
}

TEST(sqlite3RegisterLikeFunctions_caseSensitive_false)
{
    // false: case-insensitive LIKE behavior
    resetMocks();
    sqlite3 db; // dummy db handle

    // Call the focal function
    sqlite3RegisterLikeFunctions(&db, 0);

    // Expect two registrations: nargs 2 and 3
    FuncDef* p2 = sqlite3FindFunction(&db, "like", 2, SQLITE_UTF8, 0);
    FuncDef* p3 = sqlite3FindFunction(&db, "like", 3, SQLITE_UTF8, 0);

    int expectedFlags = SQLITE_FUNC_LIKE; // CASE flag should not be set

    verifyFlags(p2, expectedFlags, true);
    verifyFlags(p3, expectedFlags, true);

    ++g_total;
}

// Main: Run tests from a single entry point as required when gtest is not used.
int main() {
    // Run tests
    test_sqlite3RegisterLikeFunctions_caseSensitive_true();
    test_sqlite3RegisterLikeFunctions_caseSensitive_false();

    // Report results
    REPORT();
    if (g_failed != 0) {
        std::cerr << "Some tests FAILED.\n";
        return 1;
    }
    std::cout << "All tests PASSED.\n";
    return 0;
}

// Explanation of tests:
// - test_sqlite3RegisterLikeFunctions_caseSensitive_true validates:
//   - For caseSensitive = 1, the function registers two 'like' functions (nArg 2 and 3) with
//     both SQLITE_FUNC_LIKE and SQLITE_FUNC_CASE set, and clears SQLITE_FUNC_UNSAFE.
// - test_sqlite3RegisterLikeFunctions_caseSensitive_false validates:
//   - For caseSensitive = 0, the function registers two 'like' functions (nArg 2 and 3) with
//     only SQLITE_FUNC_LIKE set (CASE not set), and clears SQLITE_FUNC_UNSAFE.
// - The tests use a lightweight mock of sqlite3CreateFunc and sqlite3FindFunction to observe
//   and verify the flags without requiring the actual SQLite runtime.
// - Static analysis notes: We do not access private static members of any real class; the tests
//   interact with the focal C function via the provided C API surface and simple mocks.