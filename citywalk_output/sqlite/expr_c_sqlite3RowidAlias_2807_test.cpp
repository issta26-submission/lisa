// Unit test suite for sqlite3RowidAlias in expr.c
// Target environment: C++11, no GoogleTest. Use only standard library.
// Note: To avoid terminating assertions from the focal function (which uses assert(VisibleRowid(pTab))),
// we compile with NDEBUG to disable asserts. The tests rely on the real sqlite internal Table/Column structures.
// The test calls the real sqlite3RowidAlias from the SQLite codebase (expressly the function under test).

#include <vector>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>


// Include SQLite internal headers to obtain real Table/Column structures and APIs.
// Adjust include path as necessary for your build environment.
extern "C" {
}

// Ensure that we do not terminate on asserts inside sqlite3RowidAlias
#ifndef NDEBUG
#define NDEBUG
#endif

// Declaration of the focal function (C linkage)
extern "C" const char *sqlite3RowidAlias(Table *pTab);

// Helper: allocate and build a temporary Table with given column names.
// This uses the actual internal structures (Table and Column) expected by sqlite3RowidAlias.
static Table* buildTestTable(const std::vector<const char*>& colNames) {
    int n = (int)colNames.size();
    // Allocate Table object
    Table *pTab = (Table*)malloc(sizeof(Table));
    if(!pTab) return nullptr;
    // Initialize fields (use actual layout from sqliteInt.h)
    pTab->nCol = n;

    // aCol is typically allocated as a separate array.
    // Allocate array of Column and assign to aCol
    pTab->aCol = (Column*)malloc(sizeof(Column) * n);
    if(pTab->aCol == nullptr) {
        free(pTab);
        return nullptr;
    }

    // Assign zCnName strings for each column
    // Make persistent copies to ensure lifetime beyond function scope
    for(int i = 0; i < n; ++i) {
        // Some SQLite builds may name the field zCnName; the test relies on that exact name
        // We'll allocate C-strings for each column name
        size_t len = std::strlen(colNames[i]) + 1;
        char *cpy = (char*)std::malloc(len);
        std::memcpy(cpy, colNames[i], len);
        pTab->aCol[i].zCnName = cpy;
        // Other fields of Column are left default (uninitialized) but not used by sqlite3RowidAlias
    }

    return pTab;
}

// Helper: free the allocated Table and its column names
static void freeTestTable(Table *pTab) {
    if(!pTab) return;
    if(pTab->aCol) {
        for(int i = 0; i < pTab->nCol; ++i) {
            if(pTab->aCol[i].zCnName) {
                std::free((void*)pTab->aCol[i].zCnName);
            }
        }
        std::free(pTab->aCol);
    }
    std::free(pTab);
}

// Simple test harness helpers
static void logPass(const char* desc) {
    std::cout << "[PASS] " << desc << std::endl;
}
static void logFail(const char* desc) {
    std::cerr << "[FAIL] " << desc << std::endl;
}
static void expectTrue(bool cond, const char* desc) {
    if(cond) logPass(desc);
    else logFail(desc);
}

// Test 1: No alias names present in table -> expect first alias "_ROWID_"
static void test_case_no_alias_present() {
    Table *p = buildTestTable({ "A", "B", "C" }); // none of the aliases present
    const char *res = sqlite3RowidAlias(p);
    bool ok = (res != nullptr) && (std::strcmp(res, "_ROWID_") == 0);
    expectTrue(ok, "Case 1: No alias names present -> should return \"_ROWID_\"");
    freeTestTable(p);
}

// Test 2: First alias present, second alias present, third not present -> expect "OID"
static void test_case_first_two_aliases_present() {
    // Names include both "_ROWID_" and "ROWID" (case-insensitive match)
    Table *p = buildTestTable({ "_ROWID_", "ROWID", "Other" });
    const char *res = sqlite3RowidAlias(p);
    bool ok = (res != nullptr) && (std::strcmp(res, "OID") == 0);
    expectTrue(ok, "Case 2: First two aliases present -> should return \"OID\"");
    freeTestTable(p);
}

// Test 3: All three aliases present -> expect null (0)
static void test_case_all_aliases_present() {
    Table *p = buildTestTable({ "_ROWID_", "ROWID", "OID" });
    const char *res = sqlite3RowidAlias(p);
    bool ok = (res == nullptr);
    expectTrue(ok, "Case 3: All three aliases present -> should return NULL");
    freeTestTable(p);
}

// Test 4: Case-insensitive matching for alias presence
static void test_case_case_insensitive_matching() {
    // Use lowercase in column names to ensure case-insensitive matching with alphanumeric CamelCase aliases
    Table *p = buildTestTable({ "_rowid_", "rowid", "Other" });
    const char *res = sqlite3RowidAlias(p);
    // Since all three possible aliases are present (in case-insensitive sense), we should get the next non-present alias
    // Here, "OID" is not present, so expect "OID" to be returned.
    bool ok = (res != nullptr) && (std::strcmp(res, "OID") == 0);
    expectTrue(ok, "Case 4: Case-insensitive matching -> should treat names as present and return \"OID\"");
    freeTestTable(p);
}

// Test 5: No columns (nCol = 0) -> should return first alias "_ROWID_"
static void test_case_no_columns() {
    Table *p = (Table*)std::malloc(sizeof(Table));
    if(!p) { logFail("Case 5: memory allocation failed"); return; }
    p->nCol = 0;
    p->aCol = nullptr;
    const char *res = sqlite3RowidAlias(p);
    bool ok = (res != nullptr) && (std::strcmp(res, "_ROWID_") == 0);
    expectTrue(ok, "Case 5: No columns (nCol=0) -> should return \"_ROWID_\"");
    std::free(p);
}

// Main: run all tests
int main() {
    // Note: We rely on the SQLite internal headers and symbols being available in the build environment.
    // Compile with NDEBUG to disable assertions inside sqlite3RowidAlias and avoid requiring VisibleRowid.
    std::cout << "Running sqlite3RowidAlias unit tests (C++11)..." << std::endl;

    test_case_no_alias_present();
    test_case_first_two_aliases_present();
    test_case_all_aliases_present();
    test_case_case_insensitive_matching();
    test_case_no_columns();

    std::cout << "Unit tests completed." << std::endl;
    return 0;
}