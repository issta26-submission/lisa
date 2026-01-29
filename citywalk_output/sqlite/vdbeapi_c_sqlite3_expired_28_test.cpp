// Lightweight C++11 test suite for sqlite3_expired (no Google Test)
// This test exercises the focal function by supplying NULL and crafted Vdbe objects
// to ensure both branches (pStmt == NULL and p->expired true/false) are covered.

#include <opcodes.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Forward declarations and dependent includes from the focal class dependencies.
// We rely on the actual project headers to provide concrete definitions for Vdbe and
// sqlite3_stmt as used by sqlite3_expired. The test focuses on behavior, not on
// internal implementation details.
extern "C" {
  // Forward-declare the minimal sqlite3_stmt type for the API surface.
  struct sqlite3_stmt;
  typedef struct sqlite3_stmt sqlite3_stmt;

  // The focal function under test.
  int sqlite3_expired(sqlite3_stmt *pStmt);

  // Include the actual definitions used by sqlite3_expired (must be available in the project).
  // These headers provide the Vdbe type and the field 'expired' accessed by sqlite3_expired.
  // The path should be adjusted to the project structure when integrating into the build.
  #include "vdbeInt.h"     // defines Vdbe
  #include "sqliteInt.h"   // core SQLite internal types (including sqlite3_stmt if redefined)
  #include "opcodes.h"     // opcode definitions (required by other parts, may be harmless for test)
}

// The test suite uses a small, non-terminating assertion framework.
// It aggregates failures and reports a non-zero exit code if any test fails.

static int g_test_failures = 0;

// Non-terminating assertions
#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed: " #cond " at " __FILE__ ":" << __LINE__ << "\n"; \
        ++g_test_failures; \
    } \
} while(0)

#define EXPECT_EQ(a,b) do { \
    if( (a) != (b) ) { \
        std::cerr << "EXPECT_EQ failed: " #a " == " #b " is " << (a) << " vs " << (b) \
                  << " at " << __FILE__ ":" << __LINE__ << "\n"; \
        ++g_test_failures; \
    } \
} while(0)

// Helper to create a Vdbe-compatible statement wrapper for sqlite3_expired.
// Since sqlite3_expired casts the input to Vdbe*, we only need to provide a memory
// region large enough for Vdbe and set the 'expired' flag accordingly.
static sqlite3_stmt* make_stmt_with_expired(int expired_flag) {
    // Allocate memory for Vdbe (the function will cast it to Vdbe* via sqlite3_stmt*).
    // We use malloc to avoid C++ object ctor side-effects and to align with C style usage.
    Vdbe *p = (Vdbe*)std::malloc(sizeof(Vdbe));
    if(!p) return nullptr;
    std::memset(p, 0, sizeof(Vdbe));
    // Set the expired flag if the field exists in the Vdbe struct
    // The field is named 'expired' in sqlite's Vdbe struct as per the focal code.
    p->expired = (expired_flag != 0) ? 1 : 0;
    return (sqlite3_stmt*)p;
}

// Test 1: sqlite3_expired should return true when pStmt is NULL
static void test_sqlite3_expired_null_stmt() {
    // Case: NULL pointer
    int rc = sqlite3_expired(nullptr);
    EXPECT_TRUE(rc != 0); // true branch
}

// Test 2: sqlite3_expired should return false when pStmt is non-NULL and p->expired == 0
static void test_sqlite3_expired_not_expired() {
    sqlite3_stmt *stmt = make_stmt_with_expired(0); // expired = false
    if(stmt == nullptr) {
        std::cerr << "Failed to allocate Vdbe for not-expired test\n";
        ++g_test_failures;
        return;
    }
    int rc = sqlite3_expired(stmt);
    EXPECT_EQ(rc, 0); // false branch
    std::free(stmt);
}

// Test 3: sqlite3_expired should return true when pStmt is non-NULL and p->expired == 1
static void test_sqlite3_expired_is_expired() {
    sqlite3_stmt *stmt = make_stmt_with_expired(1); // expired = true
    if(stmt == nullptr) {
        std::cerr << "Failed to allocate Vdbe for expired test\n";
        ++g_test_failures;
        return;
    }
    int rc = sqlite3_expired(stmt);
    EXPECT_TRUE(rc != 0); // true branch
    std::free(stmt);
}

// Runner that executes all tests and reports summary
static int run_all_tests() {
    test_sqlite3_expired_null_stmt();
    test_sqlite3_expired_not_expired();
    test_sqlite3_expired_is_expired();
    return g_test_failures;
}

// Main entry point for the test executable
int main() {
    int failures = run_all_tests();
    if(failures == 0) {
        std::cout << "ALL TESTS PASSED: sqlite3_expired behavior verified.\n";
        return 0;
    } else {
        std::cerr << "TESTS FAILED: " << failures << " failure(s) detected.\n";
        return 1;
    }
}