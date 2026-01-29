// Lightweight C++11 unit tests for the C function sqlite3FkOldmask
// This test harness provides minimal re-implementations of the
// dependent sqlite3 structures and helpers to exercise sqlite3FkOldmask.
// No external testing framework is used; tests are run from main().

#include <vector>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>
#include <cassert>


// Domain-specific simplifications and helpers

typedef uint32_t u32;
#define COLUMN_MASK(i) (1u << (i))
#define SQLITE_ForeignKeys 0x01

// Forward declarations to mirror the focal file's dependencies

struct FKeyCol {
    int iFrom;
};

struct FKey {
    FKey* pNextFrom;     // next foreign key that originates from this table
    FKey* pNextTo;       // next foreign key that references this table
    int nCol;
    FKeyCol* aCol;
};

struct Index {
    int nKeyCol;
    int* aiColumn;
};

struct Db {
    int flags;
};

struct Parse {
    Db* db;
};

// Table structure mirroring the usage in sqlite3FkOldmask
struct Table {
    bool isOrdinary;
    struct {
        FKey* pFKey;
    } tab;
    FKey* pReferences; // FK references (other tables referencing this table)
};

// Minimal public helpers used by sqlite3FkOldmask

static inline bool IsOrdinaryTable(Table* pTab) {
    return pTab->isOrdinary;
}

static FKey* sqlite3FkReferences(Table* pTab) {
    return pTab->pReferences;
}

// A simplified sqlite3FkLocateIndex used by sqlite3FkOldmask.
// It constructs an artificial index with nKeyCol = pFKey->nCol and
// aiColumn[i] = i for i in [0, nKeyCol)
static void sqlite3FkLocateIndex(Parse* /*pParse*/, Table* /*pParent*/, FKey* pFKey,
                                 Index** ppIdx, int** /*paiCol*/) {
    if (!ppIdx) return;
    Index* idx = nullptr;
    if (pFKey) {
        idx = new Index;
        idx->nKeyCol = pFKey->nCol > 0 ? pFKey->nCol : 0;
        if (idx->nKeyCol > 0) {
            idx->aiColumn = new int[idx->nKeyCol];
            for (int i = 0; i < idx->nKeyCol; i++) idx->aiColumn[i] = i;
        } else {
            idx->aiColumn = nullptr;
        }
    }
    *ppIdx = idx;
    // paiCol is ignored in tests (passed as 0 in sqlite3FkOldmask)
}

// The focal method under test (copied and adapted from the provided snippet).
u32 sqlite3FkOldmask(Parse *pParse, Table *pTab) {
    u32 mask = 0;
    if (pParse->db->flags & SQLITE_ForeignKeys && IsOrdinaryTable(pTab)) {
        FKey *p;
        int i;
        for (p = pTab->u.tab.pFKey; p; p = p->pNextFrom) {
            for (i = 0; i < p->nCol; i++) mask |= COLUMN_MASK(p->aCol[i].iFrom);
        }
        for (p = sqlite3FkReferences(pTab); p; p = p->pNextTo) {
            Index *pIdx = 0;
            sqlite3FkLocateIndex(pParse, pTab, p, &pIdx, 0);
            if (pIdx) {
                for (i = 0; i < pIdx->nKeyCol; i++) {
                    assert(pIdx->aiColumn[i] >= 0);
                    mask |= COLUMN_MASK(pIdx->aiColumn[i]);
                }
            }
        }
    }
    return mask;
}

// Test harness

static int test_no_fk_flags_are_off();          // Ensure 0 when FK flag is off
static int test_fk_only_no_references();        // Ensure mask from FKs only
static int test_fk_with_references_mask_includes_indexes(); // Ensure index-based mask
static int test_non_ordinary_table_returns_zero(); // Ensure non-ordinary tables skip

// Utility to run a single test and print result
static int run_test(const char* name, int (*test_func)()) {
    int ok = test_func();
    if (ok) {
        std::cout << "[OK] " << name << std::endl;
    } else {
        std::cout << "[FAILED] " << name << std::endl;
    }
    return ok;
}

// Test 1: No FK flags set -> mask should be 0
int test_no_fk_flags_are_off() {
    // Prepare parse context
    Parse pParse;
    Db db;
    db.flags = 0; // FK flag off
    pParse.db = &db;

    // Prepare an ordinary table with no FKs
    Table pTab;
    pTab.isOrdinary = true;
    pTab.tab.pFKey = nullptr;
    pTab.pReferences = nullptr;

    u32 mask = sqlite3FkOldmask(&pParse, &pTab);
    // Expect 0
    if (mask != 0) {
        std::cerr << "Expected mask 0, got " << mask << std::endl;
        return 0;
    }
    return 1;
}

// Test 2: FK defined on table, but no references -> mask includes FK columns only
int test_fk_only_no_references() {
    // Prepare parse context
    Parse pParse;
    Db db;
    db.flags = SQLITE_ForeignKeys;
    pParse.db = &db;

    // Build a single FK on the table with two columns: iFrom = 1 and iFrom = 4
    FKey* fk = new FKey;
    fk->pNextFrom = nullptr;
    fk->pNextTo = nullptr;
    fk->nCol = 2;
    fk->aCol = new FKeyCol[2];
    fk->aCol[0].iFrom = 1;
    fk->aCol[1].iFrom = 4;

    Table pTab;
    pTab.isOrdinary = true;
    pTab.tab.pFKey = fk;
    pTab.pReferences = nullptr;

    u32 mask = sqlite3FkOldmask(&pParse, &pTab);
    // Expect bits for 1 and 4: (1<<1) + (1<<4) = 2 + 16 = 18
    if (mask != (1u << 1) | (1u << 4)) {
        std::cerr << "Expected mask 18, got " << mask << std::endl;
        return 0;
    }

    // Cleanup allocated memory
    delete[] fk->aCol;
    delete fk;
    return 1;
}

// Test 3: FK + a referencing index should add index-based columns to mask
int test_fk_with_references_mask_includes_indexes() {
    // Prepare parse context
    Parse pParse;
    Db db;
    db.flags = SQLITE_ForeignKeys;
    pParse.db = &db;

    // Primary table FK
    FKey* fk = new FKey;
    fk->pNextFrom = nullptr;
    fk->pNextTo = nullptr;
    fk->nCol = 2;
    fk->aCol = new FKeyCol[2];
    fk->aCol[0].iFrom = 1;
    fk->aCol[1].iFrom = 4;

    // Reference FK on same table to simulate sqlite3FkReferences chain
    FKey* ref = new FKey;
    ref->pNextFrom = nullptr;
    ref->pNextTo = nullptr; // next reference in chain
    ref->nCol = 2; // Will trigger nKeyCol = 2 in our locator
    ref->aCol = new FKeyCol[2];
    ref->aCol[0].iFrom = 0; // not used by locator in our mock beyond nCol
    ref->aCol[1].iFrom = 0;

    // Link chains
    fk->pNextFrom = nullptr;
    // For this test, sqlite3FkReferences(pTab) will return ref
    // and ref->pNextTo = nullptr ensures single reference in loop

    // Table setup
    Table pTab;
    pTab.isOrdinary = true;
    pTab.tab.pFKey = fk;
    pTab.pReferences = ref;

    u32 mask = sqlite3FkOldmask(&pParse, &pTab);

    // Expected: FK columns (1,4) -> 2 + 16 = 18
    // Plus index columns from ref (0,1) -> (1<<0) + (1<<1) = 1 + 2 = 3
    // Total = 21
    if (mask != 21u) {
        std::cerr << "Expected mask 21, got " << mask << std::endl;
        return 0;
    }

    // Cleanup
    delete[] fk->aCol;
    delete fk;
    delete[] ref->aCol;
    delete ref;
    return 1;
}

// Test 4: Non-ordinary table should skip processing and return 0 even if FK exists
int test_non_ordinary_table_returns_zero() {
    // Prepare parse context
    Parse pParse;
    Db db;
    db.flags = SQLITE_ForeignKeys;
    pParse.db = &db;

    // Build a FK on the table
    FKey* fk = new FKey;
    fk->pNextFrom = nullptr;
    fk->pNextTo = nullptr;
    fk->nCol = 1;
    fk->aCol = new FKeyCol[1];
    fk->aCol[0].iFrom = 2;

    Table pTab;
    pTab.isOrdinary = false; // non-ordinary
    pTab.tab.pFKey = fk;
    pTab.pReferences = nullptr;

    u32 mask = sqlite3FkOldmask(&pParse, &pTab);
    if (mask != 0) {
        std::cerr << "Expected mask 0 for non-ordinary table, got " << mask << std::endl;
        return 0;
    }

    delete[] fk->aCol;
    delete fk;
    return 1;
}

// Main runner
int main() {
    int total = 0;
    int ok = 0;

    total += 1; ok += run_test("test_no_fk_flags_are_off", test_no_fk_flags_are_off);
    total += 1; ok += run_test("test_fk_only_no_references", test_fk_only_no_references);
    total += 1; ok += run_test("test_fk_with_references_mask_includes_indexes", test_fk_with_references_mask_includes_indexes);
    total += 1; ok += run_test("test_non_ordinary_table_returns_zero", test_non_ordinary_table_returns_zero);

    std::cout << "Tests: " << ok << " / " << total << " passed." << std::endl;
    return (ok == total) ? 0 : 1;
}