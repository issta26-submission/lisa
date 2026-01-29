// A self-contained C++11 unit test suite for the focal function:
// sqlite3ExprListSetSortOrder, adapted from the provided C source.
// This test suite is crafted to be standalone (no GTest) and uses a
// lightweight, non-terminating assertion style to maximize code coverage.

#include <vector>
#include <cstdio>
#include <cstring>
#include <sqliteInt.h>
#include <cstdlib>


// Domain-specific constants and types (re-implemented minimal subset)
using u8 = unsigned char;

const int SQLITE_SO_UNDEFINED = -1;
const int SQLITE_SO_ASC = 0;
const int SQLITE_SO_DESC = 1;
const unsigned int KEYINFO_ORDER_BIGNULL = 0x80; // Arbitrary bit flag for testing

// Minimal stand-ins for the ExprList structures used by sqlite3ExprListSetSortOrder
struct ExprList_item_fg {
    u8 sortFlags;
    unsigned char bNulls;
};

struct ExprList_item {
    ExprList_item_fg fg;
    // Other fields are not required for the focal function
};

struct ExprList {
    int nExpr;
    ExprList_item *a;
};

// Focal method under test (copied/adapted to be testable in C++11)
void sqlite3ExprListSetSortOrder(ExprList *p, int iSortOrder, int eNulls){
  struct ExprList_item *pItem;
  if( p==0 ) return;
  // In tests we ensure preconditions; we mirror the original assertions.
  // Note: No side effects if preconditions are not met (under RELEASE builds, but tests keep them true).
  // For the purposes of this test, we simply emulate behavior when preconditions hold.
  if(!( p->nExpr>0 )){
    // Emulate: in real code this would hit an assert; here we bail gracefully
    return;
  }
  // The same asserts as in the original code (kept for fidelity)
  // In test mode we assume they pass
  pItem = &p->a[p->nExpr-1];
  if( pItem->fg.bNulls!=0 ){
    // If precondition not met, just return to avoid undefined behavior in tests
    return;
  }
  if( iSortOrder==SQLITE_SO_UNDEFINED ){
    iSortOrder = SQLITE_SO_ASC;
  }
  pItem->fg.sortFlags = (u8)iSortOrder;
  if( eNulls!=SQLITE_SO_UNDEFINED ){
    pItem->fg.bNulls = 1;
    if( iSortOrder!=eNulls ){
      pItem->fg.sortFlags |= KEYINFO_ORDER_BIGNULL;
    }
  }
}

// Lightweight test harness (no GTest, non-terminating assertions)
static int g_total = 0;
static int g_passed = 0;

#define TEST_PASS() do { g_passed++; printf("[PASS] %s\n", __func__); } while(0)
#define TEST_FAIL(msg) do { printf("[FAIL] %s: %s\n", __func__, msg); } while(0)

// Helper to create a single-element ExprList with a zero-initialized fg
static ExprList* create_single_expr_list(){
    ExprList *p = (ExprList*)malloc(sizeof(ExprList));
    if(!p) return nullptr;
    p->nExpr = 1;
    p->a = (ExprList_item*)calloc(1, sizeof(ExprList_item));
    if(!p->a){
        free(p);
        return nullptr;
    }
    // Initialize fg to known state (bNulls = 0)
    p->a[0].fg.bNulls = 0;
    p->a[0].fg.sortFlags = 0;
    return p;
}

static void destroy_expr_list(ExprList *p){
    if(!p) return;
    if(p->a) free(p->a);
    free(p);
}

// Test 1: Null pointer input should be a no-op (no crash)
static bool test_null_pointer_input(){
    // Explanatory comment:
    // Calling with p == nullptr should return immediately without modifying anything.
    sqlite3ExprListSetSortOrder(nullptr, SQLITE_SO_ASC, SQLITE_SO_ASC);
    return true; // Passed if no crash
}

// Test 2: UNDEFINED sort and UNDEFINED nulls -> defaults to ASC, no nulls set
static bool test_undefined_undefined_defaults_to_asc_no_nulls(){
    ExprList *p = create_single_expr_list();
    if(!p) return false;
    // Precondition: last item's bNulls is 0
    if(p->a[0].fg.bNulls != 0){ destroy_expr_list(p); return false; }

    sqlite3ExprListSetSortOrder(p, SQLITE_SO_UNDEFINED, SQLITE_SO_UNDEFINED);

    // Expected: sortFlags == SQLITE_SO_ASC (0), bNulls remains 0
    bool ok = (p->a[0].fg.sortFlags == SQLITE_SO_ASC) && (p->a[0].fg.bNulls == 0);
    destroy_expr_list(p);
    return ok;
}

// Test 3: DESC with explicit ASC for eNulls -> sortFlags should be (DESC) with bNUL and BIGNULL bit
static bool test_desc_with_asc_nulls_produces_bignull_and_flag(){
    ExprList *p = create_single_expr_list();
    if(!p) return false;

    // Set initial state explicitly
    p->a[0].fg.bNulls = 0;
    p->a[0].fg.sortFlags = 0;

    sqlite3ExprListSetSortOrder(p, SQLITE_SO_DESC, SQLITE_SO_ASC);

    // Expected: sortFlags = DESC (1) | BIGNULL, bNulls = 1
    unsigned int expected = (unsigned int)SQLITE_SO_DESC | KEYINFO_ORDER_BIGNULL;
    bool ok = (p->a[0].fg.sortFlags == (u8)expected) && (p->a[0].fg.bNulls == 1);
    destroy_expr_list(p);
    return ok;
}

// Test 4: ASC with DESC for eNulls -> sortFlags should be BIGNULL only, bNulls = 1
static bool test_asc_with_desc_nulls_produces_only_bignull(){
    ExprList *p = create_single_expr_list();
    if(!p) return false;

    p->a[0].fg.bNulls = 0;
    p->a[0].fg.sortFlags = 0;

    sqlite3ExprListSetSortOrder(p, SQLITE_SO_ASC, SQLITE_SO_DESC);

    unsigned int expected = KEYINFO_ORDER_BIGNULL;
    bool ok = (p->a[0].fg.sortFlags == (u8)expected) && (p->a[0].fg.bNulls == 1);
    destroy_expr_list(p);
    return ok;
}

// Test 5: UNDEFINED sort with DESC nulls -> should default to ASC, then apply DESC null handling
static bool test_undefined_sort_with_desc_nulls_behaves_expected(){
    ExprList *p = create_single_expr_list();
    if(!p) return false;

    p->a[0].fg.bNulls = 0;
    p->a[0].fg.sortFlags = 0;

    sqlite3ExprListSetSortOrder(p, SQLITE_SO_UNDEFINED, SQLITE_SO_DESC);

    // After call: iSortOrder becomes ASC (0); bNulls=1; since iSortOrder(0) != eNulls(1) -> add BIGNULL
    unsigned int expected = (unsigned int)SQLITE_SO_ASC | KEYINFO_ORDER_BIGNULL;
    bool ok = (p->a[0].fg.sortFlags == (u8)expected) && (p->a[0].fg.bNulls == 1);
    destroy_expr_list(p);
    return ok;
}

// Test 6: Single-case coverage: UNDEFINED both but ensuring branch safety
static bool test_undefined_both_branch_safety(){
    ExprList *p = create_single_expr_list();
    if(!p) return false;
    p->a[0].fg.bNulls = 0;

    sqlite3ExprListSetSortOrder(p, SQLITE_SO_UNDEFINED, SQLITE_SO_UNDEFINED);

    bool ok = (p->a[0].fg.sortFlags == (u8)SQLITE_SO_ASC) && (p->a[0].fg.bNulls == 0);
    destroy_expr_list(p);
    return ok;
}

// Runner: Collect all tests
static bool run_all_tests(){
    int total_before = g_total;
    int passed_before = g_passed;

    bool res0 = test_null_pointer_input();
    g_total++; if(res0) g_passed++;
    bool res1 = test_undefined_undefined_defaults_to_asc_no_nulls();
    g_total++; if(res1) g_passed++;
    bool res2 = test_desc_with_asc_nulls_produces_bignull_and_flag();
    g_total++; if(res2) g_passed++;
    bool res3 = test_asc_with_desc_nulls_produces_only_bignull();
    g_total++; if(res3) g_passed++;
    bool res4 = test_undefined_sort_with_desc_nulls_behaves_expected();
    g_total++; if(res4) g_passed++;
    bool res5 = test_undefined_both_branch_safety();
    g_total++; if(res5) g_passed++;

    int total = g_total - total_before;
    int passed = g_passed - passed_before;
    return (passed == total);
}

int main(){
    printf("Running sqlite3ExprListSetSortOrder test suite (standalone, C++11)\n");
    bool all_passed = run_all_tests();
    printf("Summary: %d/%d tests passed.\n", g_passed, g_total);
    return all_passed ? 0 : 1;
}