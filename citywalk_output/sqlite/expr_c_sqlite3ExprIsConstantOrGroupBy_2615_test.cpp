// Unit test suite for sqlite3ExprIsConstantOrGroupBy
// This test suite provides a minimal in-repo reimplementation of the
// required SQLite-like structures (Parse, Expr, ExprList, Walker)
// and a lightweight walker to drive sqlite3ExprIsConstantOrGroupBy.
// The goal is to exercise true/false branches of the focal method
// without requiring the full SQLite project.
//
// Notes about design choices:
// - We implement sqlite3ExprIsConstantOrGroupBy as provided in the prompt.
// - We supply a simplified sqlite3WalkExpr and a non-static exprNodeIsConstantOrGroupBy
//   so the focal method can be exercised in a standalone manner.
// - Expressions are built with a tiny, explicit AST (CONSTANT, COLUMN, ADD).
// - Group-by membership is checked by pointer equality against the ExprList passed
//   to the focal method.
// - Tests use non-terminating assertions (they log failures but continue).
// - All code is self-contained in this translation unit and compiles with C++11.

#include <vector>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>


// Domain knowledge: we implement only the minimal API/types needed by the focal method.
// This avoids pulling in the entire SQLite codebase while still exercising the logic.

extern "C" {

// Forward declarations to satisfy the focal method's signature.
struct Parse;
struct Expr;
struct ExprList;

// Lightweight test-specific assertion logger (non-terminating).
#define TEST_LOG(msg) do { std::cerr << "[TEST] " << msg << std::endl; } while(0)
#define CHECK(cond, msg) do { if(!(cond)) { TEST_LOG(std::string("CHECK FAILED: ") + (msg)); } } while(0)

// Minimal expression node types.
// We mirror the intent of SQLite's expression categories sufficiently for tests.
enum ExprTag {
    CONSTANT_EXPR = 0,
    COLUMN_EXPR   = 1,
    ADD_EXPR      = 2
};

// Simple Parse type (empty for test purposes).
struct Parse {};

// Simple Expr node (very small subset for testing).
struct Expr {
    int type;          // ExprTag
    int value;         // for constants
    std::string name;  // for column names (optional in tests)
    Expr *pLeft;
    Expr *pRight;

    // Constructors for convenience
    Expr(int t) : type(t), value(0), pLeft(nullptr), pRight(nullptr) {}
    Expr(int t, int v) : type(t), value(v), pLeft(nullptr), pRight(nullptr) {}
    Expr(int t, const std::string &n) : type(t), value(0), name(n), pLeft(nullptr), pRight(nullptr) {}
    Expr(int t, Expr *l, Expr *r) : type(t), value(0), pLeft(l), pRight(r) {}
};

// A simple expression list (ExprList) to represent GROUP BY lists.
struct ExprList {
    std::vector<Expr*> a;
};

// Forward declarations of the focal function and helpers
int sqlite3ExprIsConstantOrGroupBy(Parse *pParse, Expr *p, ExprList *pGroupBy);

// Walker structure used by the focal method.
struct Walker {
    int eCode;
    int (*xExprCallback)(Walker*, Expr*);
    int (*xSelectCallback)(Walker*, void*);
    struct { ExprList *pGroupBy; } u;  // simplified union-like member
    Parse *pParse;
};

// Lightweight traversal function: walks an expression tree and invokes callbacks.
// This is a minimal replica sufficient for unit-testing the focal method.
void sqlite3WalkExpr(Walker *w, Expr *p);

// Callback used by the walker to decide if a node is constant or grouped.
static int exprNodeIsConstantOrGroupBy(Walker *pWalker, Expr *pExpr);

// The focal method under test (as provided in the prompt).
int sqlite3ExprIsConstantOrGroupBy(Parse *pParse, Expr *p, ExprList *pGroupBy){
  Walker w;
  w.eCode = 1;
  w.xExprCallback = exprNodeIsConstantOrGroupBy;
  w.xSelectCallback = 0;
  w.u.pGroupBy = pGroupBy;
  w.pParse = pParse;
  sqlite3WalkExpr(&w, p);
  return w.eCode;
}

} // extern "C"

// Implementation details (C++ side, but inside extern "C" we used C-like API).

static int exprNodeIsConstantOrGroupBy(Walker *pWalker, Expr *pExpr) {
    // If the expression is CONSTANT, it's acceptable (do not flip eCode).
    if (pExpr->type == CONSTANT_EXPR) {
        return 0;
    }
    // If the expression is present in the GROUP BY list, it's acceptable.
    if (pWalker->u.pGroupBy) {
        for (Expr *e : pWalker->u.pGroupBy->a) {
            if (e == pExpr) {
                return 0;
            }
        }
    }
    // Otherwise, this node is neither a constant nor part of GROUP BY;
    // signal the caller by flipping eCode to 0.
    pWalker->eCode = 0;
    return 0;
}

void sqlite3WalkExpr(Walker *w, Expr *p) {
    if (p == nullptr) return;

    if (w->xExprCallback) {
        int rc = w->xExprCallback(w, p);
        // In this minimal implementation, returning non-zero would skip children;
        // here, we treat non-zero strictly as a signal to stop, but our callback always returns 0.
        (void)rc;
        // If rc != 0, we could return; but our tests rely on traversing all children.
    }

    // Traverse children for binary-like expressions.
    switch (p->type) {
        case ADD_EXPR:
            if (p->pLeft) sqlite3WalkExpr(w, p->pLeft);
            if (p->pRight) sqlite3WalkExpr(w, p->pRight);
            break;
        // CONSTANT and COLUMN have no children in this simplified model.
        default:
            break;
    }
}

// Helper constructors for tests (in the same translation unit to simplify memory mgmt).
Expr* Const(int value) { return new Expr(CONSTANT_EXPR, value); }
Expr* Col(const std::string &name) { return new Expr(COLUMN_EXPR, name); }
Expr* Add(Expr* l, Expr* r) { return new Expr(ADD_EXPR, l, r); }

ExprList* GroupBy(std::initializer_list<Expr*> elems) {
    ExprList* g = new ExprList();
    for (Expr* e : elems) g->a.push_back(e);
    return g;
}

// Test harness
static void test_constant_expression_is_constant_true_case() {
    // TRUE case: the expression is a constant
    Parse p;
    Expr* c = Const(42);
    ExprList* group = nullptr; // no GROUP BY
    int res = sqlite3ExprIsConstantOrGroupBy(&p, c, group);
    CHECK(res == 1, "Constant expression should be treated as constant (return 1).");
    // Cleanup
    delete c;
    if (group) delete group;
    TEST_LOG("test_constant_expression_is_constant_true_case completed");
}

static void test_group_by_expression_true_case() {
    // TRUE case: expression is in GROUP BY
    Parse p;
    Expr* c = Col("X");     // non-constant expression
    ExprList* group = GroupBy({ c });
    int res = sqlite3ExprIsConstantOrGroupBy(&p, c, group);
    CHECK(res == 1, "Expression present in GROUP BY should return 1.");
    delete c;
    delete group;
    TEST_LOG("test_group_by_expression_true_case completed");
}

static void test_expression_not_constant_not_group_by_false_case() {
    // FALSE case: expression is not a constant and not in GROUP BY
    Parse p;
    Expr* c = Col("Y");      // not grouped
    ExprList* group = GroupBy({ Col("X") }); // different column
    int res = sqlite3ExprIsConstantOrGroupBy(&p, c, group);
    CHECK(res == 0, "Non-constant, non-grouped expression should return 0.");
    delete c;
    delete group; // contains Col("X") allocated here
    TEST_LOG("test_expression_not_constant_not_group_by_false_case completed");
}

static void test_nested_expression_group_by_true_case() {
    // TRUE case with nested expression: ADD(constant, column in GROUP BY)
    Parse p;
    Expr* c = Const(3);
    Expr* x = Col("X");
    Expr* add = Add(c, x);
    ExprList* group = GroupBy({ x });

    int res = sqlite3ExprIsConstantOrGroupBy(&p, add, group);
    CHECK(res == 1, "Nested expression with GROUP BY member should return 1.");

    // Cleanup
    delete c;
    delete x;
    delete add;
    delete group;
    TEST_LOG("test_nested_expression_group_by_true_case completed");
}

static void test_nested_expression_not_group_by_false_case() {
    // FALSE case with nested expression and no GROUP BY
    Parse p;
    Expr* c = Const(7);
    Expr* y = Col("Y");
    Expr* add = Add(c, y);

    int res = sqlite3ExprIsConstantOrGroupBy(&p, add, nullptr);
    CHECK(res == 0, "Nested non-constant, non-grouped expression should return 0.");

    delete c;
    delete y;
    delete add;
    TEST_LOG("test_nested_expression_not_group_by_false_case completed");
}

int main() {
    // Run tests
    test_constant_expression_is_constant_true_case();
    test_group_by_expression_true_case();
    test_expression_not_constant_not_group_by_false_case();
    test_nested_expression_group_by_true_case();
    test_nested_expression_not_group_by_false_case();

    // Summary
    TEST_LOG("ALL_TESTS_COMPLETED");
    // No abrupt exit; tests already logged failures but continue.

    return 0;
}