// Minimal C++11 unit test harness for sqlite3_column_int64
// This test isolates the focal method by providing lightweight mockups
// of its dependent functions (columnMem, sqlite3_value_int64, columnMallocFailure).
// The test is self-contained (no GTest) and uses simple assertions that do not terminate on failure.

#include <opcodes.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <limits>
#include <iostream>
#include <cstdint>


// Typedefs matching the SQLite style
typedef long long sqlite_int64;

// Forward declaration of sqlite3_stmt to mimic the real type in tests
struct sqlite3_stmt;

// Lightweight sqlite3_value structure used by the mocks
struct sqlite3_value {
    sqlite_int64 v; // stored value for the fake value
};

// A minimal, fake implementation of sqlite3_value_int64 used by the focal method
// This simulates extracting a 64-bit integer from a sqlite3_value object.
static sqlite_int64 sqlite3_value_int64(sqlite3_value *pVal) {
    return pVal ? pVal->v : 0;
}

// The actual focal method under test (re-implemented in this test harness)
static sqlite_int64 sqlite3_column_int64(sqlite3_stmt *pStmt, int i);

// Lightweight test statement structure used to drive the mocks
struct TestStmt {
    sqlite3_value values[5]; // fake value slots
    int lastAccessedIndex;   // records the last index passed to columnMem
    int mallocFailureCount;  // records how many times columnMallocFailure was invoked
};

// Forward declarations of the mocks (these substitute the real static dependencies)
static sqlite3_value* columnMem(sqlite3_stmt *pStmt, int i);
static void columnMallocFailure(sqlite3_stmt *pStmt);

// Implementations of mocks and the focal method for this test harness

// The focal method logic (re-implemented)
static sqlite_int64 sqlite3_column_int64(sqlite3_stmt *pStmt, int i){
    sqlite_int64 val = sqlite3_value_int64( columnMem(pStmt,i) );
    columnMallocFailure(pStmt);
    return val;
}

// Mock: simulate retrieving the i-th value from the statement
static sqlite3_value* columnMem(sqlite3_stmt *pStmt, int i){
    TestStmt* s = (TestStmt*)pStmt;
    s->lastAccessedIndex = i;
    return &s->values[i];
}

// Mock: simulate potential malloc failure handling (but here we just count calls)
static void columnMallocFailure(sqlite3_stmt *pStmt){
    TestStmt* s = (TestStmt*)pStmt;
    s->mallocFailureCount++;
}

// Lightweight test assertion framework (non-terminating)
static int g_failCount = 0;
#define ASSERT_EQ(expected, actual, msg) do { \
    auto _e = (expected); auto _a = (actual); \
    if (!(_e == _a)) { \
        std::cerr << "FAIL: " << (msg) << " | expected: " << _e << " actual: " << _a << "\n"; \
        ++g_failCount; \
    } \
} while(0)

#define ASSERT_TRUE(cond, msg) do { \
    if (!(cond)) { \
        std::cerr << "FAIL: " << (msg) << "\n"; \
        ++g_failCount; \
    } \
} while(0)

// Test 1: Basic positive path - ensure correct value is returned and mocks are invoked
// - columnMem should be called with index 3
// - sqlite3_value_int64 should return the value stored in values[3]
// - columnMallocFailure should be invoked exactly once
void test_sqlite3_column_int64_basic() {
    TestStmt stmt;
    // initialize all slots to zero
    for (int k = 0; k < 5; ++k) {
        stmt.values[k].v = 0;
    }
    stmt.lastAccessedIndex = -1;
    stmt.mallocFailureCount = 0;

    // Set a known value at index 3
    stmt.values[3].v = 12345;

    sqlite_int64 result = sqlite3_column_int64((sqlite3_stmt*)&stmt, 3);

    ASSERT_EQ(3, stmt.lastAccessedIndex, "columnMem should be called with i=3");
    ASSERT_EQ(1, stmt.mallocFailureCount, "columnMallocFailure should be invoked exactly once");
    ASSERT_EQ(12345, result, "sqlite3_column_int64 should return the value stored in columnMem");
}

// Test 2: Second independent path - ensure another index is handled correctly
// - columnMem should be called with index 0
// - sqlite3_value_int64 should return the value stored in values[0]
// - columnMallocFailure should be invoked exactly once
void test_sqlite3_column_int64_second_case() {
    TestStmt stmt;
    for (int k = 0; k < 5; ++k) {
        stmt.values[k].v = 0;
    }
    stmt.lastAccessedIndex = -1;
    stmt.mallocFailureCount = 0;

    // Set a different known value at index 0
    stmt.values[0].v = -999;

    sqlite_int64 result = sqlite3_column_int64((sqlite3_stmt*)&stmt, 0);

    ASSERT_EQ(0, stmt.lastAccessedIndex, "columnMem should be called with i=0");
    ASSERT_EQ(1, stmt.mallocFailureCount, "columnMallocFailure should be invoked exactly once in second test");
    ASSERT_EQ(-999, result, "sqlite3_column_int64 should return the value stored in columnMem for index 0");
}

int main() {
    std::cout << "Starting sqlite3_column_int64 unit tests (mocked environment)\n";

    test_sqlite3_column_int64_basic();
    test_sqlite3_column_int64_second_case();

    if (g_failCount == 0) {
        std::cout << "All tests passed\n";
        return 0;
    } else {
        std::cerr << g_failCount << " test(s) failed\n";
        return 1;
    }
}