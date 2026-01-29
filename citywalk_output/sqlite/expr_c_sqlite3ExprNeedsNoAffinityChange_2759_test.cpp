// C++11 test suite for sqlite3ExprNeedsNoAffinityChange
// This test is self-contained and uses the project's internal sqliteInt.h
// to align with the focal method's dependencies (Expr structure, TK_*,
// SQLITE_AFF_* constants). It does not rely on GTest; a small custom test
// harness is implemented in main() to exercise true/false branches.

#include <iostream>
#include <sqliteInt.h>
#include <vector>
#include <string>


// Include the project's internal definitions (Expr struct, token constants, etc.)
// Wrap in extern "C" to ensure C linkage for C headers.
extern "C" {
}

// If the exact prototype differs in the project headers, rely on the declaration
// from sqliteInt.h. We declareExtern to be safe for the C linkage.
extern "C" int sqlite3ExprNeedsNoAffinityChange(const Expr *p, char aff);

// Lightweight test harness
static int g_total = 0;
static int g_passed = 0;

static void report(bool ok, const std::string &name) {
    ++g_total;
    if (ok) {
        ++g_passed;
        std::cout << "[PASS] " << name << "\n";
    } else {
        std::cout << "[FAIL] " << name << "\n";
    }
}

// Helper to allocate and initialize a minimal Expr node
static Expr* mkExpr(int op, Expr* left, int iTable = 0, int iColumn = -1) {
    // Allocate a new Expr object provided by the project's internal type.
    // We only initialize the fields that sqlite3ExprNeedsNoAffinityChange reads.
    Expr* e = new Expr();
    // The internal layout is library-defined; we assume these fields exist
    // and are used by the focal method.
    e->op = op;
    e->pLeft = left;
    e->pRight = nullptr;
    e->op2 = 0;
    e->iTable = iTable;
    e->iColumn = iColumn;
    return e;
}

// Convenience macros for common constants expected from sqliteInt.h
// The project provides TK_* and SQLITE_AFF_* constants; we rely on those.
static int THE(TK) { return 0; } // dummy to avoid unused macro warnings if any

int main() {
    // Ensure we have the necessary symbol available
    // (If the environment links sqlite3 correctly, this should resolve.)

    // Test 1: Affinity BLOB short-circuits to true
    // aff == SQLITE_AFF_BLOB should return 1 irrespective of the expression
    {
        Expr* p = mkExpr(TK_INTEGER, nullptr); // any expression
        int aff = SQLITE_AFF_BLOB;
        int res = sqlite3ExprNeedsNoAffinityChange(p, (char)aff);
        report(res == 1, "AFF_BLOB short-circuit returns 1");
        delete p;
    }

    // Test 2: Unary MINUS chain reduces string affinity when there is a unary minus
    // p := UMINUS(String)  -> unaryMinus = 1, op becomes TK_STRING, expect false
    {
        Expr* inner = mkExpr(TK_STRING, nullptr);
        Expr* p = mkExpr(TK_UMINUS, inner); // pLeft = inner
        int aff = SQLITE_AFF_TEXT;
        int res = sqlite3ExprNeedsNoAffinityChange(p, (char)aff);
        report(res == 0, "STRING with unary MINUS yields false for TEXT affinity");
        delete inner;
        delete p;
    }

    // Test 3: String without unary minus yields true for TEXT affinity
    // p := TK_STRING
    {
        Expr* p = mkExpr(TK_STRING, nullptr);
        int aff = SQLITE_AFF_TEXT;
        int res = sqlite3ExprNeedsNoAffinityChange(p, (char)aff);
        report(res == 1, "STRING -> TEXT affinity without unary minus returns true");
        delete p;
    }

    // Test 4: String with unary plus (UPLUS) should still yield TEXT affinity
    // p := UPLUS(String)
    {
        Expr* child = mkExpr(TK_STRING, nullptr);
        Expr* p = mkExpr(TK_UPLUS, child);
        int aff = SQLITE_AFF_TEXT;
        int res = sqlite3ExprNeedsNoAffinityChange(p, (char)aff);
        report(res == 1, "STRING with unary PLUS yields true for TEXT affinity");
        delete child;
        delete p;
    }

    // Test 5: TK_BLOB without minus should be true (non-unary path)
    // p := BLOB, aff = NUMERIC
    {
        Expr* p = mkExpr(TK_BLOB, nullptr);
        int aff = SQLITE_AFF_NUMERIC;
        int res = sqlite3ExprNeedsNoAffinityChange(p, (char)aff);
        report(res == 1, "TK_BLOB without unary yields true when aff >= NUMERIC");
        delete p;
    }

    // Test 6: TK_COLUMN with iTable>=0 and iColumn<0 should be true for NUMERIC+
    // p := COLUMN with iTable>=0, iColumn<0
    {
        Expr* p = mkExpr(TK_COLUMN, nullptr, 0, -1); // iTable=0, iColumn=-1
        int aff = SQLITE_AFF_NUMERIC;
        int res = sqlite3ExprNeedsNoAffinityChange(p, (char)aff);
        report(res == 1, "TK_COLUMN with iColumn<0 and NUMERIC affinity returns true");
        delete p;
    }

    // Test 6b: TK_COLUMN with iColumn>=0 should be false (negated by iColumn check)
    // p := COLUMN with iTable>=0, iColumn>=0
    {
        Expr* p = mkExpr(TK_COLUMN, nullptr, 0, 0); // iTable=0, iColumn=0
        int aff = SQLITE_AFF_NUMERIC;
        int res = sqlite3ExprNeedsNoAffinityChange(p, (char)aff);
        report(res == 0, "TK_COLUMN with iColumn>=0 returns false for NUMERIC affinity");
        delete p;
    }

    // Test 7: Default/unrecognized operator yields false
    // p := an unknown op value
    {
        Expr* p = mkExpr(9999, nullptr);
        int aff = SQLITE_AFF_NUMERIC;
        int res = sqlite3ExprNeedsNoAffinityChange(p, (char)aff);
        report(res == 0, "Unknown operator yields false");
        delete p;
    }

    // Test 8: TK_REGISTER path should forward to inner op (INTEGER)
    // p := REGISTER -> op2 = TK_INTEGER
    {
        Expr* inner = mkExpr(TK_INTEGER, nullptr);
        Expr* p = mkExpr(TK_REGISTER, inner);
        int aff = SQLITE_AFF_NUMERIC;
        int res = sqlite3ExprNeedsNoAffinityChange(p, (char)aff);
        report(res == 1, "TK_REGISTER with inner TK_INTEGER yields true for NUMERIC affinity");
        delete inner;
        delete p;
    }

    // Summary
    std::cout << "Test results: " << g_passed << " / " << g_total << " passed.\n";

    // Exit code: 0 for success, 1 for failure
    return (g_passed == g_total) ? 0 : 1;
}