// Unit test suite for the focal method: sqlite3ColumnIndex
// Target: verify behavior of sqlite3ColumnIndex(Table *pTab, const char *zCol)
// Note: This test suite uses a lightweight, non-terminating assertion approach
// and calls test functions from main() as gtest is not allowed.
// The tests rely on internal sqlite structures (Table, Column) and sqlite3StrIHash,
// sqlite3StrICmp which are provided by the project headers (sqliteInt.h).

#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Import necessary internal dependencies

// Ensure C linkage for the focal function when linked with C++ code
extern "C" int sqlite3ColumnIndex(Table *pTab, const char *zCol);

// Lightweight non-terminating assertion helpers
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_EQ(actual, expected, msg) do { \
    ++g_total_tests; \
    if((actual) != (expected)) { \
        std::cerr << "[FAIL] " << msg \
                  << " | Expected: " << (expected) \
                  << " | Actual: " << (actual) << "\n"; \
        ++g_failed_tests; \
    } \
} while(0)

#define EXPECT_TRUE(cond, msg) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        std::cerr << "[FAIL] " << msg << "\n"; \
        ++g_failed_tests; \
    } \
} while(0)

// Helper to create a simple table with nCol columns
static Table createTestTable(int nCol) {
    Table t;
    t.nCol = nCol;
    t.aCol = new Column[nCol];
    return t;
}

// Helper to free a test table's memory
static void destroyTestTable(Table &t) {
    if(t.aCol) delete[] t.aCol;
    t.aCol = nullptr;
    t.nCol = 0;
}

/*
  Test 1: Match found at first column
  - zCol = "colA"
  - First column has hName == hash("colA") and zCnName == "colA"
  - Expect sqlite3ColumnIndex returns 0
*/
static void test_sqlite3ColumnIndex_MatchFirst() {
    // Prepare table with 3 columns
    Table t = createTestTable(3);

    const char *zCol = "colA";
    unsigned char h = sqlite3StrIHash(zCol);

    // Column 0 matches
    t.aCol[0].hName = h;
    t.aCol[0].zCnName = "colA";

    // Other columns do not match
    t.aCol[1].hName = 0x01; t.aCol[1].zCnName = "colB";
    t.aCol[2].hName = 0x02; t.aCol[2].zCnName = "colC";

    int idx = sqlite3ColumnIndex(&t, zCol);
    EXPECT_EQ(idx, 0, "Match should be found at index 0 for first column");

    destroyTestTable(t);
}

/*
  Test 2: Match found at second column when first column hash matches but name differs
  - zCol = "colX"
  - Column 0: hName matches but zCnName != zCol
  - Column 1: hName matches and zCnName == zCol
  - Expect sqlite3ColumnIndex returns 1
*/
static void test_sqlite3ColumnIndex_MatchSecondAfterHashCollision() {
    Table t = createTestTable(2);

    const char *zCol = "colX";
    unsigned char h = sqlite3StrIHash(zCol);

    // First column has same hash but different name
    t.aCol[0].hName = h;
    t.aCol[0].zCnName = "colY";

    // Second column matches exactly
    t.aCol[1].hName = h;
    t.aCol[1].zCnName = "colX";

    int idx = sqlite3ColumnIndex(&t, zCol);
    EXPECT_EQ(idx, 1, "Match should be found at index 1 when first column hashes but mismatches");

    destroyTestTable(t);
}

/*
  Test 3: No match exists
  - zCol = "not_present"
  - All columns have non-matching hashes or names
  - Expect -1
  - Additionally exercises the branch where the first condition fails (hName != h)
*/
static void test_sqlite3ColumnIndex_NoMatch() {
    Table t = createTestTable(2);

    const char *zCol = "not_present";
    unsigned char h = sqlite3StrIHash(zCol);

    // No column matches
    t.aCol[0].hName = h ^ 0xFF; t.aCol[0].zCnName = "other1";
    t.aCol[1].hName = h ^ 0x0F; t.aCol[1].zCnName = "other2";

    int idx = sqlite3ColumnIndex(&t, zCol);
    EXPECT_EQ(idx, -1, "No matching column should return -1");

    destroyTestTable(t);
}

/*
  Test 4: Duplicate matching columns; should return the first matching index (0)
  - zCol = "dupCol"
  - Both column 0 and 1 have identical hash and zCnName
  - Expect 0
*/
static void test_sqlite3ColumnIndex_DuplicateMatchesFirst() {
    Table t = createTestTable(2);

    const char *zCol = "dupCol";
    unsigned char h = sqlite3StrIHash(zCol);

    t.aCol[0].hName = h; t.aCol[0].zCnName = "dupCol";
    t.aCol[1].hName = h; t.aCol[1].zCnName = "dupCol";

    int idx = sqlite3ColumnIndex(&t, zCol);
    EXPECT_EQ(idx, 0, "When multiple columns match, should return the first index (0)");

    destroyTestTable(t);
}

int main() {
    // Execute all tests
    test_sqlite3ColumnIndex_MatchFirst();
    test_sqlite3ColumnIndex_MatchSecondAfterHashCollision();
    test_sqlite3ColumnIndex_NoMatch();
    test_sqlite3ColumnIndex_DuplicateMatchesFirst();

    // Basic summary
    std::cout << "Total tests: " << g_total_tests
              << ", Failures: " << g_failed_tests << "\n";

    return (g_failed_tests == 0) ? 0 : 1;
}