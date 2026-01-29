// Lightweight, self-contained unit tests for the focal method logic
// sqlite3ExprFunctionUsable (recreated logic for testing purposes).
// This test-suite is C++11 compliant, uses only the standard library,
// and does not rely on GoogleTest. It mocks the minimal environment
// required by the focal function's control flow.
//
// Note: This is a self-contained harness intended to validate the logical
// branches of the focal method in absence of the complete SQLite internal
// environment. It does not depend on external libraries.

#include <cstdio>
#include <cstdarg>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>


// -----------------------------------------------------------------------------
// Domain-Model Mocks (minimal scaffolding to exercise the focal method)
// -----------------------------------------------------------------------------

// Public-facing constants (mocked subset)
const unsigned int SQLITE_FUNC_DIRECT = 0x01;
const unsigned int SQLITE_FUNC_UNSAFE = 0x02;
const int EP_FromDDL = 1;

// Trusted schema flag bit (mock)
const unsigned int SQLITE_TrustedSchema = 0x01;

// IN_RENAME_OBJECT macro (mock as always false for test harness)
#define IN_RENAME_OBJECT 0

// Forward-declared types (mock)
struct Db {
    unsigned int flags; // e.g., SQLITE_TrustedSchema bit
};

struct Parse {
    Db *db;
};

struct Expr {
    unsigned int flags; // property bitfields (e.g., EP_FromDDL)
};

struct FuncDef {
    unsigned int funcFlags; // SQLITE_FUNC_DIRECT | SQLITE_FUNC_UNSAFE
};

// Simple predicate used by focal method (mock of internal API)
static int ExprHasProperty(const Expr *pExpr, int mask) {
    if (!pExpr) return 0;
    return (pExpr->flags & mask) != 0;
}

// Global state to observe whether an error was reported by sqlite3ErrorMsg
static bool g_error_reported = false;
static std::string g_error_message;

// Mock of sqlite3ErrorMsg: records that an error was reported (no formatting required for test)
extern "C" void sqlite3ErrorMsg(Parse *pParse, const char *zFormat, ...) {
    (void)pParse; // unused in test harness beyond signaling error
    g_error_reported = true;
    // We can capture a deterministic message for assertions if needed
    g_error_message = "unsafe use reported";
}

// -----------------------------------------------------------------------------
// Focal method under test (recreated locally for isolated unit testing)
// Note: This mirrors the logic shown in the provided snippet.
// In a real environment, this function would be linked from expr.c of SQLite.
// -----------------------------------------------------------------------------
extern "C" void sqlite3ExprFunctionUsable(
    Parse *pParse,         /* Parsing and code generating context */
    const Expr *pExpr,     /* The function invocation */
    const FuncDef *pDef      /* The function being invoked */
) {
    // Assertions (mirrored behavior for test)
    // In test harness we ensure no rename-flag is set.
    if (IN_RENAME_OBJECT) {
        std::abort();
    }
    // Must have DIRECT or UNSAFE flag set for function to be considered usable
    if ( (pDef->funcFlags & (SQLITE_FUNC_DIRECT|SQLITE_FUNC_UNSAFE)) == 0 ) {
        // In real code this would assert; for test harness we simply return
        return;
    }

    // If function is FromDDL, additional restrictions apply
    if( ExprHasProperty(pExpr, EP_FromDDL) ){
        if( (pDef->funcFlags & SQLITE_FUNC_DIRECT) != 0
         || (pParse->db->flags & SQLITE_TrustedSchema) == 0
        ){
            sqlite3ErrorMsg((Parse*)pParse, "unsafe use of %#T()", pExpr);
        }
    }
}

// -----------------------------------------------------------------------------
// Lightweight test framework (non-terminating assertions)
// -----------------------------------------------------------------------------

static int g_test_failures = 0;

#define EXPECT_TRUE(cond) \
    do { \
        if(!(cond)) { \
            std::cerr << "[EXPECT_FAIL] " << __FUNCTION__ \
                      << " (line " << __LINE__ << "): expected true — " \
                      << #cond << std::endl; \
            ++g_test_failures; \
        } \
    } while(0)

#define EXPECT_FALSE(cond) EXPECT_TRUE(!(cond))

#define EXPECT_EQ(a, b) \
    do { \
        if(!((a) == (b))) { \
            std::cerr << "[EXPECT_FAIL] " << __FUNCTION__ \
                      << " (line " << __LINE__ << "): " \
                      << #a << " != " << #b << " (" << (a) << " != " << (b) << ")" \
                      << std::endl; \
            ++g_test_failures; \
        } \
    } while(0)

#define TESTCASE_DEADLOOP() \
    do { if(false) { (void)sqlite3ExprFunctionUsable; } } while(0)

// -----------------------------------------------------------------------------
// Test Cases
// -----------------------------------------------------------------------------

// Helper to reset global state between tests
static void reset_test_state() {
    g_error_reported = false;
    g_error_message.clear();
}

// Case A: EP_FromDDL is false. Expect no error reported regardless of DIRECT/UNSAFE flags.
static void test_case_A_no_fromddl_no_error() {
    reset_test_state();

    Db db{0};            // no SPECIAL flags, not trusted
    Parse pParse{ &db };
    Expr pExpr{ 0 };     // EP_FromDDL not set
    FuncDef pDef{ SQLITE_FUNC_DIRECT }; // has DIRECT flag

    sqlite3ExprFunctionUsable(&pParse, &pExpr, &pDef);

    EXPECT_FALSE(g_error_reported);
}

// Case B: EP_FromDDL true and DIRECT flag set. Expect error reported.
static void test_case_B_fromddl_direct_triggers_error() {
    reset_test_state();

    Db db{0};
    Parse pParse{ &db };
    Expr pExpr{ static_cast<unsigned int>(EP_FromDDL) }; // EP_FromDDL set

    FuncDef pDef{ SQLITE_FUNC_DIRECT | SQLITE_FUNC_UNSAFE }; // has DIRECT (>= triggers)
    sqlite3ExprFunctionUsable(&pParse, &pExpr, &pDef);

    EXPECT_TRUE(g_error_reported);
    // Optionally verify that the error message was produced
    EXPECT_FALSE(g_error_message.empty());
}

// Case C: EP_FromDDL true, no DIRECT but UNSAFE flag present, not trusted schema. Expect error.
static void test_case_C_fromddl_not_direct_unsafe_not_trusted_error() {
    reset_test_state();

    Db db{0}; // not trusted (no SQLITE_TrustedSchema bit)
    Parse pParse{ &db };
    Expr pExpr{ static_cast<unsigned int>(EP_FromDDL) }; // EP_FromDDL set

    FuncDef pDef{ SQLITE_FUNC_UNSAFE }; // UNSAFE only, no DIRECT
    sqlite3ExprFunctionUsable(&pParse, &pExpr, &pDef);

    EXPECT_TRUE(g_error_reported);
}

// Case D: EP_FromDDL true, no DIRECT but UNSAFE flag present, trusted schema is on. Expect no error.
static void test_case_D_fromddl_not_direct_unsafe_trusted_no_error() {
    reset_test_state();

    Db db{0};
    // Mark the schema as trusted
    db.flags |= SQLITE_TrustedSchema;

    Parse pParse{ &db };
    Expr pExpr{ static_cast<unsigned int>(EP_FromDDL) }; // EP_FromDDL set

    FuncDef pDef{ SQLITE_FUNC_UNSAFE }; // UNSAFE only, no DIRECT
    sqlite3ExprFunctionUsable(&pParse, &pExpr, &pDef);

    EXPECT_FALSE(g_error_reported);
}

// Case E: Additional branch coverage - conflicting flags scenario
static void test_case_E_both_direct_and_unsafe_with_fromddl() {
    reset_test_state();

    Db db{0};
    Parse pParse{ &db };
    Expr pExpr{ static_cast<unsigned int>(EP_FromDDL) }; // EP_FromDDL set

    // Both DIRECT and UNSAFE set; should trigger due to DIRECT
    FuncDef pDef{ SQLITE_FUNC_DIRECT | SQLITE_FUNC_UNSAFE };
    sqlite3ExprFunctionUsable(&pParse, &pExpr, &pDef);

    EXPECT_TRUE(g_error_reported);
}

// -----------------------------------------------------------------------------
// Main: Run all tests
// -----------------------------------------------------------------------------

int main() {
    // Run tests
    test_case_A_no_fromddl_no_error();
    test_case_B_fromddl_direct_triggers_error();
    test_case_C_fromddl_not_direct_unsafe_not_trusted_error();
    test_case_D_fromddl_not_direct_unsafe_trusted_no_error();
    test_case_E_both_direct_and_unsafe_with_fromddl();

    if(g_test_failures == 0) {
        std::cout << "All tests passed.\n";
        return 0;
    } else {
        std::cerr << g_test_failures << " test(s) failed.\n";
        return 1;
    }
}