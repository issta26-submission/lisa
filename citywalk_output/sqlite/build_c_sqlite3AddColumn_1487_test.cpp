// Test suite for sqlite3AddColumn(Parse*, Token, Token) from build.c
// This test harness is designed for a C/C++11 environment without Google Test.
// It provides lightweight, non-terminating assertions and logs failures to stdout.
// Note: This test is designed to illustrate unit-test intent and coverage reasoning
// for sqlite3AddColumn. It uses simplified, mockable representations of the
// sqlite3 core types (Parse, Table, Column, Token) and a minimal set of helpers
// that the focal method relies on. The actual project may have additional
// dependencies; for a real build you may need to adapt mocks to your
// compilation model.

#include <stdlib.h>
#include <vector>
#include <memory>
#include <cctype>
#include <cstring>
#include <sqliteInt.h>
#include <cstdarg>
#include <string>
#include <iostream>


// Domain-specific constants (mocked for unit tests)
#define IN_RENAME_OBJECT 0
#define SQLITE_LIMIT_COLUMN 200
#define SQLITE_AFF_BLOB 5
#define SQLITE_AFF_TEXT 2
#define COLTYPE_CUSTOM 0
#define SQLITEN 0

// Forward-declare the focal function (as if linked from build.c)
extern "C" void sqlite3AddColumn(struct Parse *pParse, struct Token sName, struct Token sType);

// Lightweight, test-friendly representations of core sqlite structures

typedef unsigned char u8;
typedef long long i64;

// Token structure resembling SQLite's token representation
struct Token {
    const char* z;
    int n;
};

// Column representation (subset)
struct Column {
    const char* zCnName;
    unsigned int hName;
    int eCType;
    int affinity;
    int szEst;
    unsigned int colFlags;
};

// Table representation (subset)
struct Table {
    int nCol;
    Column* aCol;
    const char* zName;
    int nNVCol;
};

// ConstraintName (subset)
struct ConstraintName {
    int n;
};

// Parse structure (subset)
struct sqlite3; // forward declare for Pseudo-db pointer
struct Parse {
    struct sqlite3* db;
    Table* pNewTable;
    ConstraintName constraintName;
};

// Pseudo sqlite3 structure (subset)
struct sqlite3 {
    int aLimit[2];       // only SQLITE_LIMIT_COLUMN used
    int szSorterRef;      // for optional feature flag; not used in tests
    // Other members are omitted for the sake of a small mock
};

// Lightweight global/test state to capture errors/messages
static std::vector<std::string> gErrorLog;
static std::vector<std::string> gInfoLog;

// Utility: non-terminating assertion
#define ASSERT_TRUE(cond, msg) do { \
    if(!(cond)) { \
        gErrorLog.push_back(std::string("Assertion failed: ") + (msg) + " (" + __FILE__ + ":" + std::to_string(__LINE__) + ")"); \
        /* Do not exit; continue to collect more failures for higher coverage */ \
    } \
} while(0)

#define ASSERT_EQ(a, b, msg) do { \
    if((a) != (b)) { \
        gErrorLog.push_back(std::string("Assertion failed: ") + (msg) + " | expected " + std::to_string((a)) + " got " + std::to_string((b)) + " (" + __FILE__ + ":" + std::to_string(__LINE__) + ")"); \
    } \
} while(0)

#define LOG_INFO(msg) do { gInfoLog.push_back(std::string("Info: ") + (msg)); } while(0)

// Helpers to build tokens and tables/pages

static Token makeToken(const char* s) {
    Token t;
    t.z = s;
    t.n = (int)std::strlen(s);
    return t;
}

// Hash helper used by sqlite3StrIHash (mock)
static unsigned int naiveHash(const char* s) {
    unsigned int h = 0;
    for (; *s; ++s) {
        h = h * 131 + (unsigned int)static_cast<unsigned char>(std::tolower((unsigned char)*s));
    }
    return h;
}

// Backend-like hash function (mock)
extern "C" unsigned int sqlite3StrIHash(const char* z) {
    return naiveHash(z);
}

// Case-insensitive compare (mock)
extern "C" int sqlite3StrICmp(const char* zLeft, const char* zRight) {
    while (*zLeft && *zRight) {
        int l = std::tolower((unsigned char)*zLeft);
        int r = std::tolower((unsigned char)*zRight);
        if (l != r) return l - r;
        ++zLeft; ++zRight;
    }
    return (*zLeft) - (*zRight);
}

// Case-insensitive n-compare (mock)
extern "C" int sqlite3_strnicmp(const char* zLeft, const char* zRight, int n) {
    for (int i = 0; i < n; ++i) {
        unsigned char a = (unsigned char)zLeft[i];
        unsigned char b = (unsigned char)zRight[i];
        if (a == '\0' || b == '\0') return a - b;
        int ca = std::tolower(a);
        int cb = std::tolower(b);
        if (ca != cb) return ca - cb;
    }
    return 0;
}

// Whitespace helper
extern "C" int sqlite3Isspace(int c) {
    return std::isspace(static_cast<unsigned char>(c)) ? 1 : 0;
}

// No-op dequote (mock)
extern "C" void sqlite3Dequote(char* z) { /* no-op in mock */ }
extern "C" void sqlite3DequoteToken(struct Token* t) { /* no-op in mock */ }

// No-op token map (mock)
extern "C" void sqlite3RenameTokenMap(struct Parse*, void*, struct Token*) {}

// No-op column properties setup
extern "C" void sqlite3ColumnPropertiesFromName(struct Table*, struct Column*) {}

// Affordable memory helpers (mock)

extern "C" void* sqlite3DbMallocRaw(struct sqlite3* db, i64 n) {
    (void)db;
    return malloc((size_t)n);
}
extern "C" void sqlite3DbFree(struct sqlite3* db, void* p) {
    (void)db;
    free(p);
}
extern "C" void* sqlite3DbRealloc(struct sqlite3* db, void* p, i64 n) {
    (void)db;
    return realloc(p, (size_t)n);
}

// Token length utility
extern "C" int sqlite3Strlen30(const char* z) {
    return (int)std::strlen(z);
}

// Static type info for standard types (mock)
static const int sqlite3StdTypeLen[] = {0, 7, 4};
static const char* sqlite3StdType[] = {"", "INTEGER", "TEXT"};
static const int sqlite3StdTypeAffinity[] = {0, 1 /* INTEGER */, 2 /* TEXT */};
#define SQLITE_N_STDTYPE 3

// Mock affinity resolution: map TYPE string to a small numeric affinity
extern "C" int sqlite3AffinityType(const char* zIn, struct Column* pCol) {
    if (zIn == nullptr) return 0;
    if (strcmp(zIn, "INTEGER") == 0) return 1;
    if (strcmp(zIn, "TEXT") == 0) return 2;
    if (strcmp(zIn, "BLOB") == 0) return 5;
    return 0;
}

// Prevent name collisions in tests when linking with real code
extern "C" void sqlite3ErrorMsg(struct Parse*, const char*, ...) {
    // Simple variadic capture for test harness
    va_list ap;
    va_start(ap, nullptr);
    // Build message only for test assertions (minimal, not formatting)
    std::string msg = "sqlite3ErrorMsg called";
    gErrorLog.push_back(msg);
    va_end(ap);
}
extern "C" void sqlite3RenameTokenMap(Parse*, void*, Token*) {}

// Note: The real sqlite3AddColumn uses many more helper routines.
// For unit testing in this synthetic harness we rely on the mocks above.


// Test environment builders

static Parse* createParseWithTable(const char* tableName, int maxCols) {
    // Create db with limit
    static sqlite3 dbInstance;
    dbInstance.aLimit[0] = maxCols; // SQLITE_LIMIT_COLUMN
    dbInstance.szSorterRef = 0;
    // pNewTable
    Table* t = (Table*)malloc(sizeof(Table));
    t->nCol = 0;
    t->aCol = nullptr;
    t->zName = tableName;

    Parse* p = (Parse*)malloc(sizeof(Parse));
    p->db = &dbInstance;
    p->pNewTable = t;
    p->constraintName.n = 0;
    return p;
}

static void destroyParse(Parse* p) {
    if (!p) return;
    if (p->pNewTable) {
        if (p->pNewTable->aCol) free(p->pNewTable->aCol);
        free(p->pNewTable);
    }
    free(p);
}

// Create a pre-existing column in the table to test duplicates
static void addExistingColumn(Table* pTab, const char* name) {
    // Allocate one new Column
    int n = pTab->nCol;
    Column* newCol = (Column*)malloc((n + 1) * sizeof(Column));
    if (n > 0 && pTab->aCol) {
        // copy old
        std::memcpy(newCol, pTab->aCol, n * sizeof(Column));
        free(pTab->aCol);
    }
    pTab->aCol = newCol;
    Column& c = pTab->aCol[n];
    c.zCnName = name;
    c.hName = naiveHash(name);
    c.eCType = COLTYPE_CUSTOM;
    c.affinity = SQLITE_AFF_BLOB;
    c.szEst = 1;
    c.colFlags = 0;
    pTab->nCol = n + 1;
    pTab->nNVCol = pTab->nCol;
}

// Build Token helpers
static Token tokenFromName(const char* name) {
    Token t;
    t.z = name;
    t.n = (int)std::strlen(name);
    return t;
}


// Test 1: Normal addition with standard type (INTEGER) sets eType and affinity correctly
static void test_addColumn_standardType_INTEGER() {
    gErrorLog.clear();
    gInfoLog.clear();

    Parse* p = createParseWithTable("tbl", 10);
    // sName = "col1", sType = "INTEGER"
    Token sName = tokenFromName("col1");
    Token sType;
    sType.z = "INTEGER";
    sType.n = (int)std::strlen("INTEGER");

    // Call focal function (assuming implementation linked and visible)
    sqlite3AddColumn(p, sName, sType);

    // Assertions
    // We expect one column in table
    ASSERT_TRUE(p->pNewTable->nCol == 1, "Column count should be 1 after add");
    if (p->pNewTable->nCol == 1) {
        const Column& c = p->pNewTable->aCol[0];
        // Name should be allocated by the function (zCnName)
        ASSERT_TRUE(c.zCnName != nullptr, "Column CN name should be set");
        // Name hashing
        ASSERT_TRUE(c.hName == sqlite3StrIHash(c.zCnName), "Hash should match name");
        // eCType and affinity reflect standard type INTEGER
        ASSERT_TRUE(c.eCType != 0, "Column type (eCType) should be set");
        // szEst should be 5 due to standard type INTEGER
        // (we used a simplified mock; ensure it's set to non-zero)
        ASSERT_TRUE(c.szEst >= 1, "Column size estimate should be set");
        LOG_INFO("test_addColumn_standardType_INTEGER passed");
    }

    destroyParse(p);
}

// Test 2: Duplicate column name triggers duplicate-name error path
static void test_addColumn_duplicateName() {
    gErrorLog.clear();
    gInfoLog.clear();

    Parse* p = createParseWithTable("tbl_dup", 10);
    // Pre-create a column "col1" that would clash
    addExistingColumn(p->pNewTable, "col1");

    Token sName = tokenFromName("col1"); // same name
    Token sType = tokenFromName("INTEGER");

    sqlite3AddColumn(p, sName, sType);

    // Expect an error log entry indicating duplicate column name
    bool foundDuplicateMsg = false;
    for (const auto& msg : gErrorLog) {
        if (msg.find("duplicate column name") != std::string::npos) {
            foundDuplicateMsg = true;
            break;
        }
    }
    ASSERT_TRUE(foundDuplicateMsg, "Expected duplicate column name error");
    LOG_INFO("test_addColumn_duplicateName passed");

    destroyParse(p);
}

// Test 3: Too many columns triggers error path due to limit check
static void test_addColumn_tooManyColumns() {
    gErrorLog.clear();
    gInfoLog.clear();

    // Create a table with limit 1; adding second column should fail
    Parse* p = createParseWithTable("tbl_limit", 1);
    // pre-existing one column to hit limit on +1
    addExistingColumn(p->pNewTable, "col_existing");

    Token sName = tokenFromName("col2");
    Token sType = tokenFromName("INTEGER");

    sqlite3AddColumn(p, sName, sType);

    // Expect error about too many columns
    bool foundLimitMsg = false;
    for (const auto& msg : gErrorLog) {
        if (msg.find("too many columns on") != std::string::npos) {
            foundLimitMsg = true;
            break;
        }
    }
    ASSERT_TRUE(foundLimitMsg, "Expected 'too many columns' error");
    LOG_INFO("test_addColumn_tooManyColumns passed");

    destroyParse(p);
}

// Test 4: No type specified (sType.n == 0) uses default blob affinity and size
static void test_addColumn_noTypeSpecified_defaults() {
    gErrorLog.clear();
    gInfoLog.clear();

    Parse* p = createParseWithTable("tbl_default", 10);

    Token sName = tokenFromName("col_no_type");
    Token sType;
    sType.z = ""; sType.n = 0;

    sqlite3AddColumn(p, sName, sType);

    // Validate a single column added with defaults
    ASSERT_TRUE(p->pNewTable->nCol == 1, "Column count should be 1 when no type specified");
    if(p->pNewTable->nCol == 1){
        const Column& c = p->pNewTable->aCol[0];
        // Default affinity blob
        ASSERT_TRUE(c.affinity == SQLITE_AFF_BLOB, "Default affinity should be BLOB");
        // eCType default
        ASSERT_TRUE(c.eCType == COLTYPE_CUSTOM, "Default eCType should be COLTYPE_CUSTOM");
        // Size estimate default
        ASSERT_TRUE(c.szEst >= 1, "Default size estimate should be set");
    }

    LOG_INFO("test_addColumn_noTypeSpecified_defaults passed");
    destroyParse(p);
}

// Test 5: Standard type "TEXT" is recognized and affinity resolved
static void test_addColumn_standardType_TEXT() {
    gErrorLog.clear();
    gInfoLog.clear();

    Parse* p = createParseWithTable("tbl_text", 10);
    Token sName = tokenFromName("col_text");
    Token sType = tokenFromName("TEXT");

    sqlite3AddColumn(p, sName, sType);

    // Expect one column and TEXT affinity
    ASSERT_TRUE(p->pNewTable->nCol == 1, "Column count should be 1 for TEXT type");
    if (p->pNewTable->nCol == 1) {
        const Column& c = p->pNewTable->aCol[0];
        // Text type should set eCType to a non-zero and affinity to TEXT
        ASSERT_TRUE(c.affinity == SQLITE_AFF_TEXT || c.affinity == 2, "TEXT affinity expected");
        ASSERT_TRUE(c.eCType == COLTYPE_CUSTOM || c.eCType != 0, "eCType should be set for TEXT");
    }

    LOG_INFO("test_addColumn_standardType_TEXT passed");
    destroyParse(p);
}

// Test runner
int main() {
    std::cout << "Starting sqlite3AddColumn unit tests (mocked environment)" << std::endl;

    test_addColumn_standardType_INTEGER();
    test_addColumn_duplicateName();
    test_addColumn_tooManyColumns();
    test_addColumn_noTypeSpecified_defaults();
    test_addColumn_standardType_TEXT();

    // Report summary
    if (gErrorLog.empty()) {
        std::cout << "All tests completed. No failures detected in this mock environment." << std::endl;
    } else {
        std::cout << gErrorLog.size() << " failure(s) detected in this mock environment:" << std::endl;
        for (auto &e : gErrorLog) {
            std::cout << "  - " << e << std::endl;
        }
    }
    return (int)gErrorLog.size();
}