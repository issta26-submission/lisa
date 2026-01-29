// This test suite targets the focal method:
// int sqlite3VdbeDeletePriorOpcode(Vdbe *p, u8 op)
// which deletes (replaces with NOOP) the opcode at the position just before the
// current last operation if it matches the provided opcode.
// The test harness below provides minimal fake definitions for Vdbe and Op,
// and a mock for sqlite3VdbeChangeToNoop to verify the interaction and return
// values without invoking the real SQLite VM.
//
// Step 1 (Candidate Keywords):
// - Vdbe, Op, opcode, nOp, aOp, sqlite3VdbeChangeToNoop, sqlite3VdbeDeletePriorOpcode
// - Branch: last opcode matches vs does not match
// - Edge: no operations (nOp == 0)
// - Return semantics: - matches -> return value of ChangeToNoop; - otherwise -> 0
//
// Step 2: Test generation (below) is designed to cover true/false branches,
// as well as multiple-op scenarios to ensure coverage of the condition and
// the call-site behavior.
//
// Step 3: Test refinements (comments + assertions) use only C++11 stdlib and
// simple non-terminating EXPECT-like checks (do not abort on failure) to
// maximize code execution.

#include <iostream>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstdint>


// Domain Knowledge notes:
// - Use only standard library. No Google Test. Provide own lightweight assertion macros.
// - Do not rely on private methods/fields beyond what is defined here.
// - Access static members via class name if needed (not required in this test).
// - The test defines a minimal replica of Vdbe and Op sufficient for the focal function.

using namespace std;

// Candidate Keywords (as comments above) -> Vdbe, Op, opcode, nOp, aOp, ChangeToNoop, DeletePriorOpcode

// Type aliases to mimic the C API used by the focal method
typedef unsigned char u8;

// Minimal Op structure with an opcode field
struct Op {
    unsigned int opcode;
};

// Minimal Vdbe structure with nOp and aOp array pointer
struct Vdbe {
    int nOp;
    Op *aOp;
};

// Forward declaration of the function under test (the focal method in vdbeaux.c)
int sqlite3VdbeDeletePriorOpcode(Vdbe *p, u8 op);

// Mocked dependency: sqlite3VdbeChangeToNoop
// We will count calls to this function and record the address argument to verify behavior.
// The real SQLite function would alter the opcode; here we just simulate a successful call.
static int g_changeToNoop_calls = 0;
static int g_last_noop_addr = -1;

int sqlite3VdbeChangeToNoop(Vdbe *p, int addr) {
    (void)p; // unused by test stub
    g_changeToNoop_calls++;
    g_last_noop_addr = addr;
    // Return a sentinel value to verify propagation from the focal method
    return 123;
}

// Implementation of the focal method for testing purposes.
// This mirrors the logic exactly as in the provided snippet.
int sqlite3VdbeDeletePriorOpcode(Vdbe *p, u8 op){
  if( p->nOp>0 && p->aOp[p->nOp-1].opcode==op ){
    return sqlite3VdbeChangeToNoop(p, p->nOp-1);
  }else{
    return 0;
  }
}

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define EXPECT_TRUE(cond) do { \
    ++g_total_tests; \
    if(!(cond)) { \
        std::cerr << "EXPECT_TRUE failed at " << __FILE__ << ":" << __LINE__ << " - " #cond << std::endl; \
        ++g_failed_tests; \
    } \
} while(0)

#define EXPECT_EQ(expected, actual) do { \
    ++g_total_tests; \
    if((expected) != (actual)) { \
        std::cerr << "EXPECT_EQ failed at " << __FILE__ << ":" << __LINE__ \
                  << " - expected: " << (expected) << " actual: " << (actual) << std::endl; \
        ++g_failed_tests; \
    } \
} while(0)

// Helper to reset global mock state between tests
void resetMocks() {
    g_changeToNoop_calls = 0;
    g_last_noop_addr = -1;
}

// Test 1: Last opcode matches the given op; ChangeToNoop should be invoked.
// Expect the focal method to return the value from sqlite3VdbeChangeToNoop
// and to have recorded the address of the operation replaced (nOp-1).
void test_delete_prior_opcode_match_last_op() {
    // Arrange
    resetMocks();
    Vdbe v;
    Op ops[1];
    const unsigned int MATCH_OP = 5;
    ops[0].opcode = MATCH_OP;
    v.nOp = 1;
    v.aOp = ops;

    // Act
    int result = sqlite3VdbeDeletePriorOpcode(&v, (u8)MATCH_OP);

    // Assert
    EXPECT_EQ(123, result);
    EXPECT_EQ(1, g_changeToNoop_calls);
    EXPECT_EQ(0, g_last_noop_addr); // address should be 0 (nOp-1)
}

// Test 2: Last opcode does not match the given op; sqlite3VdbeChangeToNoop should NOT be called.
// Expect zero return value and no changes to the mock.
void test_delete_prior_opcode_no_match() {
    // Arrange
    resetMocks();
    Vdbe v;
    Op ops[2];
    ops[0].opcode = 1;
    ops[1].opcode = 2; // last op
    v.nOp = 2;
    v.aOp = ops;

    // Act
    int result = sqlite3VdbeDeletePriorOpcode(&v, (u8)99); // 99 does not match last opcode (2)

    // Assert
    EXPECT_EQ(0, result);
    EXPECT_EQ(0, g_changeToNoop_calls);
    EXPECT_EQ(-1, g_last_noop_addr);
}

// Test 3: No operations in Vdbe (nOp == 0); should return 0 and not call ChangeToNoop.
void test_delete_prior_opcode_no_ops() {
    // Arrange
    resetMocks();
    Vdbe v;
    Op ops[0];
    v.nOp = 0;
    v.aOp = ops;

    // Act
    int result = sqlite3VdbeDeletePriorOpcode(&v, (u8)1);

    // Assert
    EXPECT_EQ(0, result);
    EXPECT_EQ(0, g_changeToNoop_calls);
    EXPECT_EQ(-1, g_last_noop_addr);
}

// Test 4: Multiple operations with a match at the last index; ensure proper address is passed.
// This tests that the function uses p->nOp-1 correctly when multiple ops exist.
void test_delete_prior_opcode_match_in_large_ops() {
    // Arrange
    resetMocks();
    Vdbe v;
    Op ops[3];
    // Populate with various opcodes; ensure the last matches
    ops[0].opcode = 7;
    ops[1].opcode = 42;
    ops[2].opcode = 9; // last opcode
    v.nOp = 3;
    v.aOp = ops;

    // Act
    int result = sqlite3VdbeDeletePriorOpcode(&v, (u8)9);

    // Assert
    EXPECT_EQ(123, result);
    EXPECT_EQ(1, g_changeToNoop_calls);
    EXPECT_EQ(2, g_last_noop_addr); // address should be 2 (nOp-1)
}

// Main: run all tests and report summary
int main() {
    test_delete_prior_opcode_match_last_op();
    test_delete_prior_opcode_no_match();
    test_delete_prior_opcode_no_ops();
    test_delete_prior_opcode_match_in_large_ops();

    std::cout << "Tests run: " << g_total_tests
              << ", Failures: " << g_failed_tests << std::endl;
    return g_failed_tests ? 1 : 0;
}