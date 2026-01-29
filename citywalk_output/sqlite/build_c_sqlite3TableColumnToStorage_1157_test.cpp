// C++11 test suite for sqlite3TableColumnToStorage
// Note: This test harness provides minimal stubs for the dependent types
// and implements the focal function directly for testing purposes.
// The tests cover true/false branches of the condition predicates and
// the mapping logic of virtual vs non-virtual columns.

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>


// -----------------------------------------------------------------------------
// Minimal type definitions to mirror the dependencies used by the focal method
// -----------------------------------------------------------------------------

using i16 = short; // Focal method uses i16

// Flags definitions (simplified for testing)
static const unsigned int TF_HasVirtual = 0x01;
static const unsigned int COLFLAG_VIRTUAL = 0x01;

// Structure stubs matching the expectations in sqlite3TableColumnToStorage
struct Column {
    unsigned int colFlags;
};

struct Table {
    int nCol;           // number of columns
    unsigned int tabFlags; // table flags (e.g., TF_HasVirtual)
    Column *aCol;        // array of columns
    int nNVCol;          // number of non-virtual stored columns
};

// -----------------------------------------------------------------------------
// FOCAL METHOD UNDER TEST
// -----------------------------------------------------------------------------
// i16 sqlite3TableColumnToStorage(Table *pTab, i16 iCol)
i16 sqlite3TableColumnToStorage(Table *pTab, i16 iCol) {
    int i;
    i16 n;
    // Precondition assertion from the original code
    // In tests we ensure iCol < pTab->nCol
    // (No runtime assert is used to keep tests non-terminating.)

    // If the table has no virtuals or iCol is negative, return as-is
    if( (pTab->tabFlags & TF_HasVirtual)==0 || iCol<0 ) return iCol;

    // Count non-virtual columns before iCol
    for(i=0, n=0; i<iCol; i++){
        if( (pTab->aCol[i].colFlags & COLFLAG_VIRTUAL)==0 ) n++;
    }

    // If the iCol-th column itself is virtual, map to storage index
    if( pTab->aCol[i].colFlags & COLFLAG_VIRTUAL ){
        /* iCol is a virtual column itself */
        return pTab->nNVCol + i - n;
    }else{
        /* iCol is a normal or stored column */
        return n;
    }
}

// -----------------------------------------------------------------------------
// Lightweight test framework (non-terminating assertions)
// -----------------------------------------------------------------------------

static int g_failures = 0;

#define TEST_ASSERT_EQ(expected, actual, msg)                                  \
    do {                                                                        \
        if ((expected) != (actual)) {                                         \
            std::cerr << "TEST FAIL: " << (msg)                                 \
                      << " | expected: " << (expected)                           \
                      << ", actual: " << (actual) << std::endl;              \
            ++g_failures;                                                     \
        } else {                                                              \
            std::cout << ".";                                               \
        }                                                                     \
    } while(false)

// Helper to create a Table with given parameters
static Table make_table(int nCol, unsigned int tabFlags, int nNVCol, Column* cols) {
    Table t;
    t.nCol = nCol;
    t.tabFlags = tabFlags;
    t.nNVCol = nNVCol;
    t.aCol = cols;
    return t;
}

// -----------------------------------------------------------------------------
// Test 1: No virtual columns; any iCol in range should be returned as-is
// - Rationale: If TF_HasVirtual is not set, the early return triggers.
// -----------------------------------------------------------------------------
static void test_no_virtuals_returns_input_index() {
    // Prepare a table with no virtual columns
    const int N = 3;
    Column cols[N] = { {0}, {0}, {0} };
    Table t = make_table(N, 0 /* no TF_HasVirtual */, 0, cols);

    // iCol values to test
    for(int i = 0; i < N; ++i) {
        i16 result = sqlite3TableColumnToStorage(&t, (i16)i);
        TEST_ASSERT_EQ((i16)i, result, "No virtuals: iCol should be returned unchanged");
    }

    // Also test negative iCol
    i16 neg = sqlite3TableColumnToStorage(&t, (i16)-1);
    TEST_ASSERT_EQ((i16)-1, neg, "No virtuals: negative iCol should be returned unchanged");
}

// -----------------------------------------------------------------------------
// Test 2: With virtual columns; iCol itself is virtual
// - Scenario: First two columns non-virtual, third is virtual.
//   nNVCol = 2; iCol = 2 (virtual). Expect storage index = nNVCol + i - n = 2 + 2 - 2 = 2
// -----------------------------------------------------------------------------
static void test_virtual_column_mapping_when_target_is_virtual() {
    // Setup: [N, N, V, N, N] with iCol=2 virtual
    const int N = 5;
    Column cols[N] = { {0}, {0}, {COLFLAG_VIRTUAL}, {0}, {0} };
    Table t = make_table(N, TF_HasVirtual, 2, cols); // nNVCol = 2

    i16 iCol = 2;
    i16 result = sqlite3TableColumnToStorage(&t, iCol);
    TEST_ASSERT_EQ((i16)2, result, "Virtual iCol at index 2 maps to storage index 2");
}

// -----------------------------------------------------------------------------
// Test 3: With virtual columns; iCol refers to a non-virtual column
// - Scenario: iCol=1, first two columns non-virtual; iCol itself non-virtual.
//   n before loop = 1; since aCol[1] is non-virtual, result should be 1.
// -----------------------------------------------------------------------------
static void test_non_virtual_column_before_mapping() {
    // Setup: [N, N, V, ...] with iCol=1 non-virtual
    const int N = 4;
    Column cols[N] = { {0}, {0}, {COLFLAG_VIRTUAL}, {0} };
    Table t = make_table(N, TF_HasVirtual, 1, cols); // nNVCol = 1

    i16 iCol = 1;
    i16 result = sqlite3TableColumnToStorage(&t, iCol);
    TEST_ASSERT_EQ((i16)1, result, "iCol=1 refers to non-virtual column; expect n (1)");
}

// -----------------------------------------------------------------------------
// Test 4: With virtual columns; iCol=0 and first column is virtual
// - Scenario: First column virtual; nNVCol=7; iCol=0 should map to nNVCol (7)
// -----------------------------------------------------------------------------
static void test_first_column_virtual_maps_to_nnvcol() {
    // Setup: [V, N, N, N] with iCol=0 virtual
    const int N = 4;
    Column cols[N] = { {COLFLAG_VIRTUAL}, {0}, {0}, {0} };
    Table t = make_table(N, TF_HasVirtual, 7, cols); // nNVCol = 7

    i16 iCol = 0;
    i16 result = sqlite3TableColumnToStorage(&t, iCol);
    TEST_ASSERT_EQ((i16)7, result, "First column virtual should map to nNVCol (7)");
}

// -----------------------------------------------------------------------------
// Main entry: run all tests and report summary
// -----------------------------------------------------------------------------
int main() {
    std::cout << "Running sqlite3TableColumnToStorage tests...\n";

    test_no_virtuals_returns_input_index();
    test_virtual_column_mapping_when_target_is_virtual();
    test_non_virtual_column_before_mapping();
    test_first_column_virtual_maps_to_nnvcol();

    if (g_failures == 0) {
        std::cout << "\nALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << "\n" << g_failures << " TEST(S) FAILED\n";
        return 1;
    }
}