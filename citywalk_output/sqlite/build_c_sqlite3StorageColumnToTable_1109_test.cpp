// Test suite for the focal method: sqlite3StorageColumnToTable
// This file contains a minimal, self-contained reproduction of the
// dependent types and the function under test, followed by unit tests.
// The tests are written without GTest, using a lightweight non-terminating
// assertion style (EXPECT_* macros) and a main() to run all tests.
// Notes:
// - This test recreates only the necessary parts of the surrounding code to
//   exercise the focal function's logic (TF_HasVirtual and COLFLAG_VIRTUAL
//   handling on Table->aCol[] elements).
// - Static details from the real project are not relied upon; all definitions
//   used here are local to the test binary.
//
// Step 1 (Program Understanding): Candidate Keywords representing core dependencies
// - TF_HasVirtual: flag bit checked in Table->tabFlags to enable virtual-column logic
// - COLFLAG_VIRTUAL: flag bit checked in each Table->aCol[i].colFlags to count virtual columns
// - aCol[i].colFlags: per-column flags, used to detect virtual columns
// - tabFlags: Table's flag field, used to guard the virtual-column loop
// - iCol: input/output storage column index being transformed
//
// Step 2: Unit Test Generation: Coverage plan
// - Case A: No virtual support (TF_HasVirtual not set) => result should be the same as input iCol
// - Case B: Virtual support enabled, but no virtual columns in range [0, iCol] => result unchanged
// - Case C: Some virtual columns in range, causing iCol to increment accordingly
// - Case D: All first N columns virtual with iCol = 0 => final iCol equals number of virtuals found
// - Case E: Mixed virtuals interspersed to exercise dynamic loop expansion
//
// Step 3: Test Case Refinement
// - Use deterministic, small Table/aCol arrays to avoid out-of-bounds
// - Verify both content (final iCol) and behavior when entering/leaving the loop
// - Provide explanatory comments for each test

#include <iostream>
#include <sqliteInt.h>
#include <cstdint>


// Minimal reproductions of the dependencies used by the focal method

using i16 = int16_t;

#define TF_HasVirtual 0x01
#define COLFLAG_VIRTUAL 0x01

struct Column {
    unsigned int colFlags; // per-column flags
};

struct Table {
    unsigned int tabFlags; // table-level flags
    Column aCol[16];        // aCol array with enough space for tests
};

// Focal method under test (reproduced here for self-contained testing)
i16 sqlite3StorageColumnToTable(Table *pTab, i16 iCol) {
  if( pTab->tabFlags & TF_HasVirtual ){
    int i;
    for(i=0; i<=iCol; i++){
      if( pTab->aCol[i].colFlags & COLFLAG_VIRTUAL ) iCol++;
    }
  }
  return iCol;
}

// Lightweight test harness (non-terminating assertions)
static int testCount = 0;
static int failCount = 0;

#define EXPECT_EQ(actual, expected) \
  do { \
    ++testCount; \
    if ((actual) != (expected)) { \
      std::cerr << "Test " << testCount << " FAILED: " \
                << #actual << " (" << (actual) << ") != " \
                << #expected << " (" << (expected) << ")"; \
      std::cerr << std::endl; \
      ++failCount; \
    } \
  } while(0)

#define EXPECT_TRUE(cond) \
  do { \
    ++testCount; \
    if (!(cond)) { \
      std::cerr << "Test " << testCount << " FAILED: Condition is false: " \
                << #cond << std::endl; \
      ++failCount; \
    } \
  } while(0)

void resetCounters() {
    testCount = 0;
    failCount = 0;
}

// Helper to initialize a Table with all non-virtual columns
void initTableNonVirtual(Table &t) {
    t.tabFlags = 0;
    for (int i = 0; i < 16; ++i) {
        t.aCol[i].colFlags = 0;
    }
}

// Helper to initialize a Table with specific virtual flags on given indices
void setVirtualAt(Table &t, int idx) {
    if (idx >= 0 && idx < 16) {
        t.aCol[idx].colFlags |= COLFLAG_VIRTUAL;
    }
}

// Main tests
int main() {
    // Test A: No virtual support (TF_HasVirtual not set) => iCol unchanged
    {
        Table t;
        t.tabFlags = 0; // no virtual support
        for (int i = 0; i < 16; ++i) t.aCol[i].colFlags = 0;
        i16 in = 5;
        i16 out = sqlite3StorageColumnToTable(&t, in);
        EXPECT_EQ(out, in); // should be unchanged
    }

    // Test B: Virtual support enabled, but no virtual columns in range [0..iCol]
    {
        Table t;
        t.tabFlags = TF_HasVirtual;
        // first few columns non-virtual
        for (int i = 0; i < 16; ++i) t.aCol[i].colFlags = 0;
        i16 in = 3;
        i16 out = sqlite3StorageColumnToTable(&t, in);
        EXPECT_EQ(out, in); // unchanged since no virtuals
    }

    // Test C: Some virtual columns within [0..iCol], cause iCol to increment
    {
        Table t;
        t.tabFlags = TF_HasVirtual;
        // reset all, then set virtual on 0 and 2
        for (int i = 0; i < 16; ++i) t.aCol[i].colFlags = 0;
        setVirtualAt(t, 0);
        setVirtualAt(t, 2);
        i16 in = 3;
        i16 out = sqlite3StorageColumnToTable(&t, in);
        // Simulation: iCol starts 3
        // i=0 -> virtual -> iCol becomes 4
        // i=1 -> non-virtual
        // i=2 -> virtual -> iCol becomes 5
        // i=3 -> non-virtual
        // i=4 -> 4 <= 5 true; i=4 non-virtual
        // i=5 -> 5 <= 5 true; i=5 non-virtual
        // i=6 -> 6 <= 5 false; end
        // Final iCol = 5
        EXPECT_EQ(out, 5);
    }

    // Test D: All first N columns virtual with iCol starting at 0
    {
        Table t;
        t.tabFlags = TF_HasVirtual;
        for (int i = 0; i < 16; ++i) t.aCol[i].colFlags = 0;
        // make 0,1,2 virtual
        setVirtualAt(t, 0);
        setVirtualAt(t, 1);
        setVirtualAt(t, 2);
        i16 in = 0;
        i16 out = sqlite3StorageColumnToTable(&t, in);
        // i=0 -> v -> iCol=1
        // i=1 -> v -> iCol=2
        // i=2 -> v -> iCol=3
        // i=3 -> 3 <= 3 true but aCol[3] non-virtual, end after
        // Final iCol = 3
        EXPECT_EQ(out, 3);
    }

    // Test E: Mixed virtuals to exercise dynamic expansion in the loop
    {
        Table t;
        t.tabFlags = TF_HasVirtual;
        for (int i = 0; i < 16; ++i) t.aCol[i].colFlags = 0;
        // virtuals at 1 and 3, and 5
        setVirtualAt(t, 1);
        setVirtualAt(t, 3);
        setVirtualAt(t, 5);
        i16 in = 1;
        i16 out = sqlite3StorageColumnToTable(&t, in);
        // Step through:
        // i=0: non-virtual
        // i=1: virtual -> iCol=2
        // i=2: non-virtual
        // i=3: virtual -> iCol=3
        // i=4: non-virtual
        // i=5: virtual -> iCol=4
        // i=6: non-virtual
        // i=7: 7 <= 4? false -> end
        // Final iCol = 4
        EXPECT_EQ(out, 4);
    }

    // Summary
    if (failCount == 0) {
        std::cout << "ALL TESTS PASSED (" << testCount << " tests)." << std::endl;
        return 0;
    } else {
        std::cerr << failCount << " of " << testCount << " tests FAILED." << std::endl;
        return 1;
    }
}