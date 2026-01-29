// High-quality C++11 unit test suite for sqlite3SetJoinExpr (no GTest, custom lightweight framework)

/*
Notes:
- This test suite targets the focal method: sqlite3SetJoinExpr defined in select.c.
- It relies on the sqlite3 internal structures (Expr, ExprList, etc.) and helper functions
  (ExprSetProperty, ExprHasProperty, ExprUseXList, ExprSetVVAProperty) that are declared
  in sqliteInt.h (included below).
- Tests are written in C++ but call into C APIs; extern "C" is used where necessary.
- A lightweight non-terminating test framework is implemented via macros so tests can continue
  running after a failure to maximize coverage.
- Two core tests are provided:
  1) test_simple_chain_sets_iJoin: ensures iJoin is propagated along a simple right-leaning chain.
  2) test_function_expression_recurses_and_left: ensures recursion into function arguments and left subtree occurs.
- Memory management is performed to avoid leaks in the test environment.
*/

// NOTE: The environment must provide the sqliteInt.h header and the sqlite3SetJoinExpr function.
// If not available, this test will fail to compile. The tests assume standard SQLite internal types
// and macros like TK_FUNCTION, EP_OuterON, EP_InnerON, EP_TokenOnly, EP_Reduced, etc.

#include <iostream>
#include <cstdlib>
#include <sqliteInt.h>
#include <cstdint>


// Bring in the sqlite internal declarations required for the test
extern "C" {
}

// Lightweight testing framework (non-terminating assertions)
static int g_test_failures = 0;
static int g_tests_run = 0;

#define TEST_LOG(msg) std::cerr << "TEST: " << (msg) << std::endl
#define TEST_FAIL(msg) do { \
    std::cerr << "FAIL: " << (msg) << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
    ++g_test_failures; \
} while(0)
#define TEST_PASS(msg) do { \
    std::cout << "PASS: " << (msg) << std::endl; \
} while(0)
#define TEST_ASSERT(cond, msg) do { \
    ++g_tests_run; \
    if(!(cond)) { TEST_FAIL(msg); } \
    else { TEST_PASS(msg); } \
} while(0)

// Helper: create a simple leaf expression (non-function)
static Expr* makeLeafExpr() {
    Expr* p = (Expr*)calloc(1, sizeof(Expr));
    if(!p) return nullptr;
    // Ensure it's not a function and has no children
    p->op = 0;              // Not TK_FUNCTION
    p->pLeft = nullptr;
    p->pRight = nullptr;
    p->x.pList = nullptr;   // No argument list
    p->w.iJoin = -1;          // Mark as not joined yet
    return p;
}

// Helper: create a function-expression with two arguments (args may be any expressions)
static Expr* makeFunctionExprWithArgs(Expr* arg1, Expr* arg2) {
    Expr* f = (Expr*)calloc(1, sizeof(Expr));
    if(!f) return nullptr;
    f->op = TK_FUNCTION; // Represent a function expression
    f->pLeft = nullptr;
    f->pRight = nullptr;
    f->w.iJoin = -1;

    // Build an ExprList for the function arguments
    ExprList* list = (ExprList*)calloc(1, sizeof(ExprList));
    if(!list) { free(f); return nullptr; }
    list->nExpr = 2;
    list->a = (ExprList_item*)calloc(2, sizeof(ExprList_item));
    if(!list->a) { free(list); free(f); return nullptr; }

    list->a[0].pExpr = arg1;
    list->a[1].pExpr = arg2;

    f->x.pList = list;
    return f;
}

// Properly free an Expr subtree (careful to free function arg lists and sub-expressions)
static void freeExprRecursive(Expr* p) {
    if(!p) return;
    // Free function argument list if present
    if(p->op == TK_FUNCTION && p->x.pList != nullptr) {
        ExprList* li = p->x.pList;
        if(li->a){
            for(int i = 0; i < li->nExpr; ++i){
                freeExprRecursive(li->a[i].pExpr);
            }
            free(li->a);
        }
        free(li);
        p->x.pList = nullptr;
    }
    // Free children
    freeExprRecursive(p->pLeft);
    freeExprRecursive(p->pRight);
    // Free self
    free(p);
}

// Test 1: Simple right-leaning chain: root -> right -> leaf2
static void test_simple_chain_sets_iJoin() {
    // Build chain: root (leaf) -> right = leaf2
    Expr* leaf2 = makeLeafExpr();
    Expr* root  = makeLeafExpr();
    if(!leaf2 || !root) {
        TEST_FAIL("Memory allocation failed in test_simple_chain_sets_iJoin");
        return;
    }
    root->pRight = leaf2;

    // Call the focal method with a valid join flag
    sqlite3SetJoinExpr(root, 2, EP_OuterON);

    // Validate that both nodes were updated
    bool ok = (root->w.iJoin == 2) && (leaf2->w.iJoin == 2);
    TEST_ASSERT(ok, "test_simple_chain_sets_iJoin: iJoin propagated to all nodes in chain");

    // Cleanup
    freeExprRecursive(root);
}

// Test 2: Parameterized function expression with arguments and a left subtree
static void test_function_expression_recurses_and_left() {
    // Create function args (two leaves)
    Expr* arg1 = makeLeafExpr();
    Expr* arg2 = makeLeafExpr();
    if(!arg1 || !arg2) {
        TEST_FAIL("Memory allocation failed for test_function_expression_recurses_and_left args");
        return;
    }

    // Function expression with two args
    Expr* func = makeFunctionExprWithArgs(arg1, arg2);
    if(!func) {
        TEST_FAIL("Could not allocate function expression in test_function_expression_recurses_and_left");
        freeExprRecursive(arg1);
        freeExprRecursive(arg2);
        return;
    }

    // Left subtree: a separate leaf attached to the function's left
    Expr* leftLeaf = makeLeafExpr();
    if(!leftLeaf) {
        TEST_FAIL("Memory allocation failed for left subtree in test_function_expression_recurses_and_left");
        freeExprRecursive(func);
        freeExprRecursive(arg1);
        freeExprRecursive(arg2);
        return;
    }
    func->pLeft = leftLeaf;

    // Execute: should set iJoin on function, its args, and the left subtree
    sqlite3SetJoinExpr(func, 5, EP_InnerON);

    bool ok = (func->w.iJoin == 5) && (arg1->w.iJoin == 5) && (arg2->w.iJoin == 5) && (leftLeaf->w.iJoin == 5);
    TEST_ASSERT(ok, "test_function_expression_recurses_and_left: iJoin propagated to function, its args, and left subtree");

    // Cleanup
    freeExprRecursive(func);
    // freeExprRecursive will recursively free arg1, arg2, leftLeaf as they are referenced from the function's args
}

// Entry point: run all tests
int main() {
    TEST_LOG("Starting sqlite3SetJoinExpr unit tests (C++ harness, non-GTest).");

    test_simple_chain_sets_iJoin();
    test_function_expression_recurses_and_left();

    // Summary
    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_test_failures << std::endl;
    if(g_test_failures == 0) {
        TEST_LOG("All tests passed.");
        return 0;
    } else {
        TEST_LOG("Some tests failed.");
        return 1;
    }
}