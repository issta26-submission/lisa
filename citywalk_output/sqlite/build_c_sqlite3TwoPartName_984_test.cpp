// Minimal C++11 test suite for the focal C function: sqlite3TwoPartName
// This test harness is self-contained and does not rely on GTest.
// It provides a tiny non-terminating assertion mechanism and runs a few focused tests
// covering true/false branches of sqlite3TwoPartName as described in Step 2.

// Note: The test includes a compact in-file implementation of the necessary sqlite3-like
// types and helpers to exercise sqlite3TwoPartName without requiring the full SQLite source tree.

#include <vector>
#include <cstdio>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <cstdarg>
#include <iostream>
#include <cassert>


// ------------------------------
// Lightweight test framework
// ------------------------------
static std::vector<std::string> g_test_failures;   // non-terminating assertion failures
static bool g_error_called = false;                // whether sqlite3ErrorMsg was invoked
static std::string g_last_error;                    // captured error message

#define TEST_EXPECT_TRUE(cond, desc) \
    do { if(!(cond)) g_test_failures.push_back(std::string("EXPECT_TRUE failed: ") + (desc)); } while(0)

#define TEST_EXPECT_EQ_INT(actual, expected, desc) \
    do { if((actual)!=(expected)) g_test_failures.push_back(std::string("EXPECT_EQ_INT failed: ") + (desc) + " (expected " + std::to_string(expected) + ", got " + std::to_string(actual) + ")"); } while(0)

#define TEST_EXPECT_PTR_EQ(a, b, desc) \
    do { if((a) != (b)) g_test_failures.push_back(std::string("EXPECT_PTR_EQ failed: ") + (desc)); } while(0)

#define TEST_ERROR_MSG_CONTAINS(substr) \
    do { if(!g_error_called) g_test_failures.push_back("EXPECTED_ERROR_MADE but no error was set"); \
         else if(g_last_error.find(substr) == std::string::npos) g_test_failures.push_back(std::string("ERROR_NOT_CONTAINS: expected substring '") + (substr) + "' in error: '" + g_last_error + "'"); } while(0)

static void reset_error_state() {
    g_error_called = false;
    g_last_error.clear();
}

// ------------------------------
// Minimal type definitions (stand-ins for sqlite3 types used by the focal method)
// ------------------------------
typedef struct Token {
    const char *z;
    int n;
} Token;

typedef struct sqlite3InitInfo {
    int busy;
    int iDb;
} sqlite3InitInfo;

typedef struct sqlite3 {
    sqlite3InitInfo init;
    unsigned mDbFlags;
} sqlite3;

typedef struct Parse {
    sqlite3 *db;
} Parse;

// Domain constants (mocked)
#define IN_SPECIAL_PARSE 0
#define DBFLAG_Vacuum 0x01

// Forward declarations of functions used by sqlite3TwoPartName
extern "C" int sqlite3FindDb(sqlite3 *db, Token *pName);
extern "C" void sqlite3ErrorMsg(Parse *pParse, const char *zFormat, ...);

// ------------------------------
// Global placeholders for error capturing
// ------------------------------
extern "C" int sqlite3TwoPartName( Parse *pParse, Token *pName1, Token *pName2, Token **pUnqual );


// Implement a minimal sqlite3ErrorMsg to capture messages without terminating tests
extern "C" void sqlite3ErrorMsg(Parse *pParse, const char *zFormat, ...)
{
    (void)pParse;
    g_error_called = true;
    char buf[512];
    va_list ap;
    va_start(ap, zFormat);
    vsnprintf(buf, sizeof(buf), zFormat, ap);
    va_end(ap);
    g_last_error.assign(buf);
}

// Implement a simple mapping to mimic database lookup behavior
extern "C" int sqlite3FindDb(sqlite3 *db, Token *pName)
{
    if( db==0 || pName==0 || pName->z==0 ) return -1;
    const char *z = pName->z;
    if( std::strcmp(z, "unknown")==0 ) return -1;
    if( std::strcmp(z, "main")==0 ) return 0;
    if( std::strcmp(z, "temp")==0 ) return 1;
    // Default assumption: some database index (0 for simplicity)
    return 0;
}

// The focal method under test (copied/adapted from the prompt, with necessary type definitions)
extern "C" int sqlite3TwoPartName(
  Parse *pParse,      /* Parsing and code generating context */
  Token *pName1,      /* The "xxx" in the name "xxx.yyy" or "xxx" */
  Token *pName2,      /* The "yyy" in the name "xxx.yyy" */
  Token **pUnqual     /* Write the unqualified object name here */
){
{
  int iDb;                    /* Database holding the object */
  sqlite3 *db = pParse->db;
  assert( pName2!=0 );
  if( pName2->n>0 ){
    if( db->init.busy ) {
      sqlite3ErrorMsg(pParse, "corrupt database");
      return -1;
    }
    *pUnqual = pName2;
    iDb = sqlite3FindDb(db, pName1);
    if( iDb<0 ){
      sqlite3ErrorMsg(pParse, "unknown database %T", pName1);
      return -1;
    }
  }else{
    assert( db->init.iDb==0 || db->init.busy || IN_SPECIAL_PARSE
             || (db->mDbFlags & DBFLAG_Vacuum)!=0);
    iDb = db->init.iDb;
    *pUnqual = pName1;
  }
  return iDb;
}
}

// ------------------------------
// Test helpers and test cases
// ------------------------------
static Parse g_pParse;
static sqlite3 g_db;

static Token makeToken(const char* s) {
    Token t;
    t.z = s;
    t.n = (int)std::strlen(s);
    return t;
}

static Token* takeAddress(Token &t) { return &t; }

// Test 1: Two-part name with an unknown database should return -1 and set an error
static void test_twoPartName_unknownDatabase() {
    // Reset error tracking
    reset_error_state();
    
    // Setup database context
    g_db.init.busy = 0;
    g_db.init.iDb = 2;
    g_db.mDbFlags = 0;

    g_pParse.db = &g_db;

    Token pName1 = makeToken("unknown");  // will trigger iDb = -1 from sqlite3FindDb
    Token pName2 = makeToken("tbl");      // two-part name; n > 0
    Token *pUnqual = nullptr;

    int res = sqlite3TwoPartName(&g_pParse, &pName1, &pName2, &pUnqual);

    // Assertions (non-terminating)
    TEST_EXPECT_EQ_INT(res, -1, "Two-part name with unknown database should return -1");
    TEST_EXPECT_PTR_EQ(pUnqual, &pName2, "pUnqual should point to pName2 when two-part name is used");
    TEST_EXPECT_TRUE(g_error_called, "sqlite3ErrorMsg should be called for unknown database");
    TEST_ERROR_MSG_CONTAINS("unknown database");
}

// Test 2: Two-part name with a known database should return the database index and set unqual to pName2
static void test_twoPartName_knownDatabase() {
    reset_error_state();
    
    g_db.init.busy = 0;
    g_db.init.iDb = 7; // value used as return by the function for known databases in this test
    g_db.mDbFlags = 0;

    g_pParse.db = &g_db;

    Token pName1 = makeToken("main"); // known database
    Token pName2 = makeToken("tbl");  // two-part name; n > 0
    Token *pUnqual = nullptr;

    int res = sqlite3TwoPartName(&g_pParse, &pName1, &pName2, &pUnqual);

    // res should be 0 according to sqlite3FindDb mapping for "main"
    TEST_EXPECT_EQ_INT(res, 0, "Two-part name with known database 'main' should return iDb=0");
    TEST_EXPECT_PTR_EQ(pUnqual, &pName2, "pUnqual should be pName2 when two-part name is used");
    TEST_EXPECT_TRUE(!g_error_called, "No error should be raised for known database");
}

// Test 3: Two-part name with db busy should return -1 and set "corrupt database" error
static void test_twoPartName_busy() {
    reset_error_state();
    
    g_db.init.busy = 1;  // simulate busy database
    g_db.init.iDb = 0;
    g_db.mDbFlags = 0;

    g_pParse.db = &g_db;

    Token pName1 = makeToken("main");
    Token pName2 = makeToken("tbl");  // n > 0
    Token *pUnqual = nullptr;

    int res = sqlite3TwoPartName(&g_pParse, &pName1, &pName2, &pUnqual);

    TEST_EXPECT_EQ_INT(res, -1, "Two-part name when database is busy should return -1");
    TEST_EXPECT_TRUE(g_error_called, "sqlite3ErrorMsg should be called when database is busy");
    TEST_ERROR_MSG_CONTAINS("corrupt database");
}

// Test 4: One-part name path (pName2.n == 0) with iDb == 0 should set pUnqual to pName1 and return 0
static void test_onePartName_basic_zero_iDb() {
    reset_error_state();
    
    g_db.init.busy = 0;
    g_db.init.iDb = 0;
    g_db.mDbFlags = 0;

    g_pParse.db = &g_db;

    Token pName1 = makeToken("qualname");
    Token pName2 = makeToken("");   // n == 0 to trigger unqualified path
    Token *pUnqual = nullptr;

    int res = sqlite3TwoPartName(&g_pParse, &pName1, &pName2, &pUnqual);

    TEST_EXPECT_EQ_INT(res, 0, "One-part name path should return iDb value (0 in this test)");
    TEST_EXPECT_PTR_EQ(pUnqual, &pName1, "pUnqual should be set to pName1 in one-part-name path");
    TEST_EXPECT_TRUE(!g_error_called, "No error should be raised in one-part-name path when iDb==0");
}

// Test 5: One-part name path using VACUUM flag to satisfy the assertion and return iDb
static void test_onePartName_vacuumFlag_allows_nonzero_iDb() {
    reset_error_state();
    
    g_db.init.busy = 0;
    g_db.init.iDb = 5;
    g_db.mDbFlags = DBFLAG_Vacuum; // vacuum implies allowed path even if iDb != 0

    g_pParse.db = &g_db;

    Token pName1 = makeToken("qualname");
    Token pName2 = makeToken("");   // n == 0
    Token *pUnqual = nullptr;

    int res = sqlite3TwoPartName(&g_pParse, &pName1, &pName2, &pUnqual);

    TEST_EXPECT_EQ_INT(res, 5, "One-part name path with VACUUM flag should return iDb (5)");
    TEST_EXPECT_PTR_EQ(pUnqual, &pName1, "pUnqual should be set to pName1 in one-part-name path with VACUUM");
    TEST_EXPECT_TRUE(!g_error_called, "No error should be raised in this VACUUM-enabled path");
}

// ------------------------------
// Main test runner
// ------------------------------
int main() {
    // Run tests
    test_twoPartName_unknownDatabase();
    test_twoPartName_knownDatabase();
    test_twoPartName_busy();
    test_onePartName_basic_zero_iDb();
    test_onePartName_vacuumFlag_allows_nonzero_iDb();

    // Report results
    if(g_test_failures.empty()) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << g_test_failures.size() << " TEST(S) FAILED:\n";
        for(const auto &s : g_test_failures) {
            std::cout << " - " << s << "\n";
        }
        return 1;
    }
}