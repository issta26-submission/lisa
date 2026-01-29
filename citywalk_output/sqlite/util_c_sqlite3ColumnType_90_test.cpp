// A self-contained C++11 unit test suite for the sqlite3ColumnType function.
// The tests are designed to be self-contained (no GTest) and focus on
// the core logic of sqlite3ColumnType as defined in the provided focal method.
// The test mocks necessary dependencies (Column structure, COLFLAG_HASTYPE,
// sqlite3StdType array, SQLITE_N_STDTYPE) to exercise all branches.

#include <math.h>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cassert>


// Domain-specific (mocked) dependencies to support sqlite3ColumnType.
// These definitions are crafted to mirror the minimal surface required by the focal method.

// Candidate Keywords drawn from the focal method: Column, colFlags, COLFLAG_HASTYPE, zCnName, eCType, sqlite3StdType, SQLITE_N_STDTYPE

// Mock of COLFLAG_HASTYPE flag
#define COLFLAG_HASTYPE 0x01

// Mock of standard types; maps 1..SQLITE_N_STDTYPE to string representations.
static const int SQLITE_N_STDTYPE = 5;
static const char* sqlite3StdType[SQLITE_N_STDTYPE] = {
    "INTEGER",  // 1
    "TEXT",     // 2
    "BLOB",     // 3
    "REAL",     // 4
    "NUMERIC"   // 5
};

// Minimal mock of the Column structure used by sqlite3ColumnType.
// In the real code, this would be a more complex struct; here we only keep the fields
// required by the focal method.
struct Column {
    unsigned int colFlags;
    const char* zCnName; // The concatenated CN name + a second string following a null terminator
    int eCType;            // Integer code for a standard type (1..SQLITE_N_STDTYPE)
};

// Focal method under test.
// Kept faithful to the provided implementation (adapted to C++ for unit testing).
char *sqlite3ColumnType(Column *pCol, char *zDflt){
    if( pCol->colFlags & COLFLAG_HASTYPE ){
        // Return pointer to the string immediately after the first NUL terminator in zCnName.
        return (char*)(pCol->zCnName) + strlen(pCol->zCnName) + 1;
    }else if( pCol->eCType ){
        // Ensure the eCType is within the defined range (mirroring the original assert).
        assert( pCol->eCType <= SQLITE_N_STDTYPE );
        return (char*)sqlite3StdType[pCol->eCType-1];
    }else{
        // Fallback to the provided default string.
        return zDflt;
    }
}

// Simple non-terminating test assertion macro.
// Unlike assert, this reports failures but does not abort test execution.
#define EXPECT_TRUE(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "[TEST FAILED] " << msg << " (at " << __FILE__ << ":" << __LINE__ << ")\n"; \
            ++g_failed; \
        } else { \
            ++g_passed; \
        } \
    } while(0)

static int g_passed = 0;
static int g_failed = 0;

// Test 1: HASTYPE branch is taken and returns the string after the first null terminator.
// Setup: zCnName contains "colName\0TYPE_NAME", colFlags has COLFLAG_HASTYPE.
void test_hastype_branch_returns_following_string() {
    // Prepare a buffer with two strings: "colName" and "TYPE_NAME".
    char zbuf[64];
    std::strcpy(zbuf, "colName");
    // Place the second string after the first string's terminator.
    int len = std::strlen(zbuf);                 // length of "colName" = 7
    std::strcpy(zbuf + len + 1, "TYPE_NAME");     // writes "TYPE_NAME\0" after the first string

    Column c;
    c.colFlags = COLFLAG_HASTYPE;
    c.zCnName = zbuf;
    c.eCType = 0;

    char* result = sqlite3ColumnType(&c, "DEFAULT");
    // Expect the returned pointer to point to "TYPE_NAME"
    EXPECT_TRUE(result != nullptr, "HASTYPE: result should not be null");
    EXPECT_TRUE(std::strcmp(result, "TYPE_NAME") == 0,
                "HASTYPE: should return the string following the first NUL terminator");
}

// Test 2: eCType path is taken when HASTYPE is not set and eCType is non-zero.
// Setup: no colFlags, eCType = 3 should map to sqlite3StdType[2] = "BLOB".
void test_ctype_branch_returns_stdtype() {
    Column c;
    c.colFlags = 0;
    c.zCnName = nullptr;
    c.eCType = 3; // maps to sqlite3StdType[2] -> "BLOB"

    char* result = sqlite3ColumnType(&c, "DEFAULT");
    EXPECT_TRUE(result != nullptr, "CTYPE: result should not be null");
    EXPECT_TRUE(std::strcmp(result, "BLOB") == 0,
                "CTYPE: eCType=3 should map to sqlite3StdType[2] => 'BLOB'");
}

// Test 3: Fallback to default (zDflt) when neither HASTYPE nor eCType is set.
// Setup: colFlags = 0, eCType = 0; zDflt should be returned as-is (pointer equality).
void test_default_branch_returns_zDflt_pointer() {
    Column c;
    c.colFlags = 0;
    c.zCnName = nullptr;
    c.eCType = 0;

    const char* zDefault = "DEFAULT_STRING";
    char* result = sqlite3ColumnType(&c, (char*)zDefault);
    // Ensure the pointer returned is exactly the same as zDflt (address equality)
    EXPECT_TRUE(result == zDefault, "DEFAULT: should return the provided zDflt pointer (address equality)");
}

// Test 4: HASTYPE takes precedence over eCType.
// Ensure that even if eCType is non-zero, the HASTYPE path is chosen.
// Setup: zCnName contains "alpha\0beta", colFlags has COLFLAG_HASTYPE; eCType non-zero.
void test_hastype_precedence_over_ctype() {
    char zbuf[64];
    std::strcpy(zbuf, "alpha");
    int len = std::strlen(zbuf); // 5
    std::strcpy(zbuf + len + 1, "beta"); // second string at position after null

    Column c;
    c.colFlags = COLFLAG_HASTYPE;
    c.zCnName = zbuf;
    c.eCType = 5; // would map to "NUMERIC" if reached, but HASTYPE should win

    char* result = sqlite3ColumnType(&c, "DEFAULT");
    EXPECT_TRUE(result != nullptr, "HASTYPE precedence: result should not be null");
    EXPECT_TRUE(std::strcmp(result, "beta") == 0,
                "HASTYPE precedence: should return the string after the first null terminator ('beta')");
}

// Test 5: Boundary condition for eCType mapping (eCType = SQLITE_N_STDTYPE).
// This should map to sqlite3StdType[SQLITE_N_STDTYPE-1] -> "NUMERIC".
void test_ctype_boundary_map_to_last_type() {
    Column c;
    c.colFlags = 0;
    c.zCnName = nullptr;
    c.eCType = SQLITE_N_STDTYPE; // 5

    char* result = sqlite3ColumnType(&c, "DEFAULT");
    EXPECT_TRUE(result != nullptr, "CTYPE_BOUNDARY: result should not be null");
    EXPECT_TRUE(std::strcmp(result, "NUMERIC") == 0,
                "CTYPE_BOUNDARY: eCType=SQLITE_N_STDTYPE should map to last std type 'NUMERIC'");
}

// Run all tests and report summary.
int main() {
    // Run tests with explanatory comments for each unit test.
    test_hastype_branch_returns_following_string();
    test_ctype_branch_returns_stdtype();
    test_default_branch_returns_zDflt_pointer();
    test_hastype_precedence_over_ctype();
    test_ctype_boundary_map_to_last_type();

    // Summary
    std::cout << "Tests run: " << (g_passed + g_failed) << "\n";
    std::cout << "Passed: " << g_passed << "\n";
    std::cout << "Failed: " << g_failed << "\n";

    // Return non-zero if any test failed
    return (g_failed == 0) ? 0 : 1;
}