// Self-contained C++11 unit tests for the focal function sqlite3CheckObjectName
// This file implements a small mock environment to exercise the core logic
// of sqlite3CheckObjectName as described in the provided focal method.
// The tests are non-terminating: they accumulate failures and report at the end.
// Note: This is a self-contained harness; it mimics the necessary parts of SQLite
// API used by sqlite3CheckObjectName, to enable isolated testing without GTest.

#include <vector>
#include <cstdio>
#include <cstdarg>
#include <string>
#include <sqliteInt.h>
#include <cstring>
#include <cctype>
#include <iostream>


// Domain constants mimicking sqlite return codes
static const int SQLITE_OK = 0;
static const int SQLITE_ERROR = 1;

// Forward declarations and minimal type definitions to support sqlite3CheckObjectName
struct InitInfo {
    int busy;
    const char *azInit[3];
    int imposterTable;
};

struct sqlite3 {
    InitInfo init;
};

struct Parse {
    sqlite3 *db;
    int nested;
};

// Global-ish configuration and test state to drive mocked helper behavior
struct Config {
    int bExtraSchemaChecks;
};

static Config sqlite3Config = {1}; // default: extra schema checks enabled

// Test-scoped flags to drive behaviors used inside sqlite3CheckObjectName
static int g_forceWritableSchema = 0;          // if non-zero, sqlite3WritableSchema(db) returns true
static int g_shadowReadOnly = 0;                 // sqlite3ReadOnlyShadowTables(db)
static int g_shadowNameExists = 0;               // sqlite3ShadowTableName(db, zName) returns non-null when true
static const char *g_shadowNameUsed = nullptr;    // not strictly required, but kept for readability

static std::string g_lastErrorMessage; // captures last sqlite3ErrorMsg content

static int sqlite3WritableSchema(sqlite3 *db) {
    // Mockable: return true to simulate writable_schema enabled
    (void)db;
    return g_forceWritableSchema;
}

static int sqlite3StrNICmp(const char *zLeft, const char *zRight, int n){
    // Case-insensitive prefix comparison up to n characters
    if (n <= 0) return 0;
    for(int i = 0; i < n; ++i){
        unsigned char c1 = zLeft[i] ? static_cast<unsigned char>(std::tolower(static_cast<unsigned char>(zLeft[i]))) : 0;
        unsigned char c2 = zRight[i] ? static_cast<unsigned char>(std::tolower(static_cast<unsigned char>(zRight[i]))) : 0;
        if (c1 != c2) return c1 - c2;
        if (zLeft[i] == '\0' || zRight[i] == '\0') {
            // if one string ends, compare lengths
            return (zLeft[i] == zRight[i]) ? 0 : (zLeft[i] ? 1 : -1);
        }
    }
    return 0;
}

static int sqlite3_stricmp(const char *zLeft, const char *zRight){
    // Case-insensitive strcmp
    while(*zLeft && *zRight){
        int c1 = std::tolower(static_cast<unsigned char>(*zLeft));
        int c2 = std::tolower(static_cast<unsigned char>(*zRight));
        if (c1 != c2) return c1 - c2;
        ++zLeft; ++zRight;
    }
    return static_cast<unsigned char>(*zLeft) - static_cast<unsigned char>(*zRight);
}

static int sqlite3ReadOnlyShadowTables(sqlite3 *db){
    (void)db;
    return g_shadowReadOnly;
}
static const char* sqlite3ShadowTableName(sqlite3 *db, const char *zName){
    // If a shadow table name exists, pretend sqlite returns non-null for it
    (void)db;
    if (g_shadowNameExists) return zName;
    return nullptr;
}

static void sqlite3ErrorMsg(Parse *pParse, const char *zFormat, ...) {
    // Simple formatted message capture (non-terminating in tests)
    char buffer[512];
    va_list ap;
    va_start(ap, zFormat);
    vsnprintf(buffer, sizeof(buffer), zFormat, ap);
    va_end(ap);
    g_lastErrorMessage = buffer;
    (void)pParse;
}

// The focal method under test (reproduced here for self-contained testing)
int sqlite3CheckObjectName(
    Parse *pParse,            /* Parsing context */
    const char *zName,        /* Name of the object to check */
    const char *zType,        /* Type of this object */
    const char *zTblName      /* Parent table name for triggers and indexes */
){
    sqlite3 *db = pParse->db;
    if( sqlite3WritableSchema(db)
     || db->init.imposterTable
     || !sqlite3Config.bExtraSchemaChecks
    ){
        /* Skip these error checks for writable_schema=ON */
        return SQLITE_OK;
    }
    if( db->init.busy ){
        if( sqlite3_stricmp(zType, db->init.azInit[0])
         || sqlite3_stricmp(zName, db->init.azInit[1])
         || sqlite3_stricmp(zTblName, db->init.azInit[2])
        ){
            sqlite3ErrorMsg(pParse, ""); /* corruptSchema() will supply the error */
            return SQLITE_ERROR;
        }
    }else{
        if( (pParse->nested==0 && 0==sqlite3StrNICmp(zName, "sqlite_", 7))
             || (sqlite3ReadOnlyShadowTables(db) && sqlite3ShadowTableName(db, zName))
        ){
            sqlite3ErrorMsg(pParse, "object name reserved for internal use: %s",
                            zName);
            return SQLITE_ERROR;
        }
    }
    return SQLITE_OK;
}

// Helper facility to reset error capture between tests
static void reset_error_state(){
    g_lastErrorMessage.clear();
    g_shadowReadOnly = 0;
    g_shadowNameExists = 0;
    g_shadowNameUsed = nullptr;
    g_forceWritableSchema = 0;
}

// Simple test harness state
static int g_total_tests = 0;
static int g_failed_tests = 0;

static void log_failure(const char* test_name, const char* message){
    std::cerr << "TEST FAILURE [" << test_name << "]: " << message << "\n";
    ++g_failed_tests;
}

// Test 1: Early return due to writable_schema enabled
void test_writable_schema_skips_checks(){
    const char* test_name = "test_writable_schema_skips_checks";
    ++g_total_tests;

    reset_error_state();
    g_forceWritableSchema = 1;                     // simulate writable_schema ON
    sqlite3Config.bExtraSchemaChecks = 1;           // extra checks enabled (irrelevant due to early return)

    Parse p; sqlite3 db; p.db = &db; p.nested = 0;
    // Initialize a minimal db state
    db.init.busy = 0; db.init.imposterTable = 0;
    db.init.azInit[0] = "TYPE"; db.init.azInit[1] = "NAME"; db.init.azInit[2] = "TABLE";

    int rc = sqlite3CheckObjectName(&p, "NAME", "TYPE", "TABLE");
    if (rc != SQLITE_OK){
        log_failure(test_name, "Expected SQLITE_OK when writable_schema is ON");
    } else {
        // success
    }
}

// Test 2: Early return due to imposterTable flag
void test_imposter_table_skips_checks(){
    const char* test_name = "test_imposter_table_skips_checks";
    ++g_total_tests;

    reset_error_state();
    g_forceWritableSchema = 0;
    // imposterTable set to non-zero to skip checks
    sqlite3 db; Parse p; p.db = &db; p.nested = 0;
    db.init.busy = 0; db.init.imposterTable = 1;
    db.init.azInit[0] = "TYPE"; db.init.azInit[1] = "NAME"; db.init.azInit[2] = "TABLE";

    int rc = sqlite3CheckObjectName(&p, "NAME", "TYPE", "TABLE");
    if (rc != SQLITE_OK){
        log_failure(test_name, "Expected SQLITE_OK when imposterTable is set");
    }
}

// Test 3: Busy mode with mismatch triggers error
void test_busy_mismatch_triggers_error(){
    const char* test_name = "test_busy_mismatch_triggers_error";
    ++g_total_tests;

    reset_error_state();
    g_forceWritableSchema = 0; // not writable
    sqlite3 db; Parse p; p.db = &db; p.nested = 0;
    db.init.busy = 1;
    db.init.imposterTable = 0;
    db.init.azInit[0] = "TYPE"; db.init.azInit[1] = "NAME"; db.init.azInit[2] = "TABLE";

    // zType mismatches azInit[0]
    int rc = sqlite3CheckObjectName(&p, "NAME", "DIFF", "TABLE");
    if (rc != SQLITE_ERROR){
        log_failure(test_name, "Expected SQLITE_ERROR on busy mismatch");
    }
    // And ensure that an error message was produced (empty format in this code path)
    if (g_lastErrorMessage.empty()){
        log_failure(test_name, "Expected an error message to be produced (empty format path)");
    }
}

// Test 4: Busy mode with all fields matching passes
void test_busy_all_match_passes(){
    const char* test_name = "test_busy_all_match_passes";
    ++g_total_tests;

    reset_error_state();
    g_forceWritableSchema = 0;
    sqlite3 db; Parse p; p.db = &db; p.nested = 0;
    db.init.busy = 1;
    db.init.azInit[0] = "TYPE"; db.init.azInit[1] = "NAME"; db.init.azInit[2] = "TABLE";

    int rc = sqlite3CheckObjectName(&p, "NAME", "TYPE", "TABLE");
    if (rc != SQLITE_OK){
        log_failure(test_name, "Expected SQLITE_OK when busy and all fields match azInit");
    }
}

// Test 5: Not-busy, reserved name with sqlite_ prefix triggers error
void test_not_busy_reserved_name_triggers_error(){
    const char* test_name = "test_not_busy_reserved_name_triggers_error";
    ++g_total_tests;

    reset_error_state();
    g_forceWritableSchema = 0;
    sqlite3 db; Parse p; p.db = &db; p.nested = 0;
    db.init.busy = 0; db.init.imposterTable = 0;
    db.init.azInit[0] = "TYPE"; db.init.azInit[1] = "NAME"; db.init.azInit[2] = "TABLE";

    int rc = sqlite3CheckObjectName(&p, "sqlite_master", "TABLE", "NAME");
    if (rc != SQLITE_OK){
        log_failure(test_name, "Expected SQLITE_ERROR for internal use reserved name");
    }
    // Expect an explanatory error message
    if (g_lastErrorMessage.find("internal use") == std::string::npos){
        log_failure(test_name, "Expected error message to mention internal use");
    }
}

// Test 6: Not-busy, shadow table reserved name triggers error
void test_not_busy_shadow_table_triggers_error(){
    const char* test_name = "test_not_busy_shadow_table_triggers_error";
    ++g_total_tests;

    reset_error_state();
    g_forceWritableSchema = 0;
    g_shadowReadOnly = 1;        // simulate read-only shadow tables
    g_shadowNameExists = 1;      // shadow table name exists
    g_shadowNameUsed = "shadow_tbl";

    sqlite3 db; Parse p; p.db = &db; p.nested = 0;
    db.init.busy = 0; db.init.imposterTable = 0;
    db.init.azInit[0] = "TYPE"; db.init.azInit[1] = "NAME"; db.init.azInit[2] = "TABLE";

    int rc = sqlite3CheckObjectName(&p, "shadow_tbl", "TABLE", "NAME");
    if (rc != SQLITE_OK){
        // Expected to error due to internal/shadow name
        // Nothing else to assert here
    } else {
        log_failure(test_name, "Expected SQLITE_ERROR for shadow/internal use name");
    }
}

// Test 7: Not-busy, no issues
void test_not_busy_no_issues(){
    const char* test_name = "test_not_busy_no_issues";
    ++g_total_tests;

    reset_error_state();
    g_forceWritableSchema = 0;
    sqlite3 db; Parse p; p.db = &db; p.nested = 0;
    db.init.busy = 0; db.init.imposterTable = 0;
    db.init.azInit[0] = "TYPE"; db.init.azInit[1] = "NAME"; db.init.azInit[2] = "TABLE";

    int rc = sqlite3CheckObjectName(&p, "my_table", "TABLE", "my_table_parent"); // non-reserved
    if (rc != SQLITE_OK){
        log_failure(test_name, "Expected SQLITE_OK for non-reserved, non-shadow object name");
    }
}

// Main driver: run all tests and report summary
int main(){
    // Run test suite
    test_writable_schema_skips_checks();
    test_imposter_table_skips_checks();
    test_busy_mismatch_triggers_error();
    test_busy_all_match_passes();
    test_not_busy_reserved_name_triggers_error();
    test_not_busy_shadow_table_triggers_error();
    test_not_busy_no_issues();

    // Summary
    std::cout << "\nTest Summary: " << g_total_tests << " tests executed." << std::endl;
    if (g_failed_tests == 0) {
        std::cout << "All tests PASSED." << std::endl;
        return 0;
    } else {
        std::cout << g_failed_tests << " tests FAILED." << std::endl;
        return 1;
    }
}