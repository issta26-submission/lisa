// Comprehensive unit tests for the focal method: sqlite3BtreeSecureDelete
// This test suite is written for C++11, without Google Test.
// It uses a lightweight in-file implementation of the focal function and its minimal
// dependencies to enable isolated testing of behavior, including edge cases.

#include <btreeInt.h>
#include <iostream>


// Domain-specific constants chosen to satisfy internal asserts:
#define BTS_SECURE_DELETE 1
#define BTS_OVERWRITE 2
#define BTS_FAST_SECURE (BTS_OVERWRITE | BTS_SECURE_DELETE) // 3

// Minimal reproductions of the structures used by sqlite3BtreeSecureDelete

struct BtShared {
    int btsFlags; // bitfield flags used by sqlite3BtreeSecureDelete
};

struct Btree {
    BtShared *pBt; // pointer to shared btree state
};

// Stubs for sqlite3BtreeEnter and sqlite3BtreeLeave (no-ops for unit test)
static void sqlite3BtreeEnter(Btree *p){
    (void)p; // no-op in unit test
}
static void sqlite3BtreeLeave(Btree *p){
    (void)p; // no-op in unit test
}

// The focal method under test (reproduced in test harness for isolation)
int sqlite3BtreeSecureDelete(Btree *p, int newFlag){
  int b;
  if( p==0 ) return 0;
  sqlite3BtreeEnter(p);
  // Mirror the assertion constraints from the original source
  assert( BTS_OVERWRITE==BTS_SECURE_DELETE*2 );
  assert( BTS_FAST_SECURE==(BTS_OVERWRITE|BTS_SECURE_DELETE) );
  if( newFlag>=0 ){
    p->pBt->btsFlags &= ~BTS_FAST_SECURE;
    p->pBt->btsFlags |= BTS_SECURE_DELETE*newFlag;
  }
  b = (p->pBt->btsFlags & BTS_FAST_SECURE)/BTS_SECURE_DELETE;
  sqlite3BtreeLeave(p);
  return b;
}

// Lightweight test framework (non-terminating assertions)
static int g_failures = 0;
#define TEST_ASSERT(cond, msg) \
    do { \
        if(!(cond)) { \
            std::cerr << "ASSERTION FAILED: " << (msg) \
                      << " (" << __FILE__ << ":" << __LINE__ << ")" << std::endl; \
            ++g_failures; \
        } \
    } while(0)

#define TEST_EQ(actual, expected, msg) \
    TEST_ASSERT((actual) == (expected), (std::string("EXPECT_EQ: ") + (msg) + " | actual=" + std::to_string((actual)) + " expected=" + std::to_string((expected))).c_str())

// Helper to construct a Btree with given initial flags
static Btree* makeBtreeWithFlags(int flags){
    BtShared* pBt = new BtShared();
    pBt->btsFlags = flags;
    Btree* p = new Btree();
    p->pBt = pBt;
    return p;
}

// Cleanup helper
static void destroyBtree(Btree* p){
    if(p){
        delete p->pBt;
        delete p;
    }
}

// Test 1: Null pointer should return 0 (no operation performed)
static void test_null_pointer_returns_zero(){
    int res = sqlite3BtreeSecureDelete(nullptr, -1);
    TEST_EQ(res, 0, "sqlite3BtreeSecureDelete(nullptr, -1) should return 0");
}

// Test 2: newFlag < 0 with initial flags = 0 should yield 0 (no fast-secure bit set)
static void test_negative_flag_initial_zero(){
    Btree* p = makeBtreeWithFlags(0);
    int res = sqlite3BtreeSecureDelete(p, -1);
    TEST_EQ(res, 0, "Initial flags 0, newFlag -1 should yield 0");
    // Cleanup
    destroyBtree(p);
}

// Test 3: initial flags = 3 (BTS_FAST_SECURE set), newFlag = -1 should return 3
static void test_initial_three_newFlag_negative_one_returns_three(){
    Btree* p = makeBtreeWithFlags(BTS_FAST_SECURE); // 3
    int res = sqlite3BtreeSecureDelete(p, -1);
    TEST_EQ(res, 3, "Initial 3 with newFlag -1 should return 3");
    destroyBtree(p);
}

// Test 4: initial flags = 3, newFlag = 0 should clear fast-secure and set zero multiplier
// Expect resulting return value 1 due to remaining BTS_SECURE_DELETE bit
static void test_initial_three_newFlag_zero_returns_one_and_flags_after(){
    Btree* p = makeBtreeWithFlags(BTS_FAST_SECURE); // 3
    int res = sqlite3BtreeSecureDelete(p, 0);
    TEST_EQ(res, 1, "Initial 3 with newFlag 0 should return 1");
    // After operation, btsFlags should be BTS_SECURE_DELETE (1)
    TEST_EQ(p->pBt->btsFlags, BTS_SECURE_DELETE, "After newFlag 0, btsFlags should be 1");
    destroyBtree(p);
}

// Test 5: initial flags = 3, newFlag = 1 should clear fast-secure and set BTS_SECURE_DELETE
// Expect resulting return value 1
static void test_initial_three_newFlag_one_returns_one(){
    Btree* p = makeBtreeWithFlags(BTS_FAST_SECURE); // 3
    int res = sqlite3BtreeSecureDelete(p, 1);
    TEST_EQ(res, 1, "Initial 3 with newFlag 1 should return 1");
    TEST_EQ(p->pBt->btsFlags, BTS_SECURE_DELETE, "After newFlag 1, btsFlags should be 1");
    destroyBtree(p);
}

// Test 6: initial flags = 3, newFlag = 2 should clear fast-secure and set value 2
// Expect resulting return value 2
static void test_initial_three_newFlag_two_returns_two(){
    Btree* p = makeBtreeWithFlags(BTS_FAST_SECURE); // 3
    int res = sqlite3BtreeSecureDelete(p, 2);
    TEST_EQ(res, 2, "Initial 3 with newFlag 2 should return 2");
    TEST_EQ(p->pBt->btsFlags, BTS_SECURE_DELETE*2, "After newFlag 2, btsFlags should be 2");
    destroyBtree(p);
}

// Test 7: After a sequence of calls, verify that multiple newFlag values lead to expected outcomes
static void test_multiple_flag_changes_sequence(){
    // Start with 3
    Btree* p = makeBtreeWithFlags(BTS_FAST_SECURE); // 3
    int res1 = sqlite3BtreeSecureDelete(p, 0); // expect 1
    TEST_EQ(res1, 1, "Seq1: newFlag 0 on initial 3 should yield 1");
    // Now btsFlags is 1
    int res2 = sqlite3BtreeSecureDelete(p, 1); // clears 3 and sets 1 -> final 1
    TEST_EQ(res2, 1, "Seq2: newFlag 1 after prior -> should yield 1");
    TEST_EQ(p->pBt->btsFlags, BTS_SECURE_DELETE, "Seq2: after operations, btsFlags should be 1");
    destroyBtree(p);
}

// Run all tests and report summary
int main(){
    // Run tests
    test_null_pointer_returns_zero();
    test_negative_flag_initial_zero();
    test_initial_three_newFlag_negative_one_returns_three();
    test_initial_three_newFlag_zero_returns_one_and_flags_after();
    test_initial_three_newFlag_one_returns_one();
    test_initial_three_newFlag_two_returns_two();
    test_multiple_flag_changes_sequence();

    // Summary
    if(g_failures == 0){
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    }else{
        std::cout << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}