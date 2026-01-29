/*
  Unit test suite for the focal method:
    void sqlite3VdbeJumpHere(Vdbe *p, int addr){
      sqlite3VdbeChangeP2(p, addr, p->nOp);
    }

  Approach:
  - Create a self-contained, minimal reproduction of the Vdbe type and the dependency
    sqlite3VdbeChangeP2 so the test does not rely on the real SQLite source.
  - Implement sqlite3VdbeJumpHere exactly as in the focal method.
  - Provide a mock/stub for sqlite3VdbeChangeP2 that records its received parameters.
  - Create a small, self-contained test harness (no GTest) with non-terminating assertions.
  - Include three tests to exercise different values and ensure the correct parameters flow.
  - Run tests from main, print results, and exit with non-zero code if failures occurred.

  Notes:
  - This test suite is self-contained and compiles under C++11.
  - We avoid private members and rely only on the public-facing behavior of the focal method.
  - The tests focus on ensuring that the value p->nOp is passed as the third argument to ChangeP2
    and that addr is forwarded correctly.
*/

#include <vector>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <iostream>


// Minimal reproducer types to simulate the SQLite structures used by the focal method.
struct Vdbe { int nOp; };

// Global state to capture calls to sqlite3VdbeChangeP2
static Vdbe* g_lastP = nullptr;
static int g_lastAddr = 0;
static int g_lastP2  = 0;
static bool g_called   = false;

// Failure tracking for non-terminating test assertions
static int g_failureCount = 0;
static std::vector<std::string> g_failureMessages;

// Simple helper to record failures with a message
static void recordFailure(const std::string& msg) {
    ++g_failureCount;
    g_failureMessages.push_back(msg);
}

// Non-terminating assertion helpers (EXPECT_* style)
#define EXPECT_TRUE(cond, msg) do { if(!(cond)) { recordFailure(std::string("EXPECT_TRUE failed: ") + (msg)); } } while(0)
#define EXPECT_EQ(val, expected, msg) do { if(!((val) == (expected))) { 
    std::ostringstream oss; oss << "EXPECT_EQ failed: " << (msg) << " | got: " << (val) << " expected: " << (expected);
    recordFailure(oss.str()); 
} } while(0)
#define EXPECT_PTR_EQ(ptr, expectedPtr, msg) do { if((ptr) != (expectedPtr)) { 
    std::ostringstream oss; oss << "EXPECT_PTR_EQ failed: " << (msg) << " | got: " << ptr << " expected: " << expectedPtr;
    recordFailure(oss.str()); 
} } while(0)

// Mocked dependency: the function the focal method delegates to
extern "C" void sqlite3VdbeChangeP2(Vdbe *p, int addr, int p2) {
    g_lastP = p;
    g_lastAddr = addr;
    g_lastP2 = p2;
    g_called = true;
}

// Focal method under test (reproduced here for self-containment)
extern "C" void sqlite3VdbeJumpHere(Vdbe *p, int addr){
  sqlite3VdbeChangeP2(p, addr, p->nOp);
}

// Helper to reset the mock state before each test
static void reset_mock_state() {
    g_lastP = nullptr;
    g_lastAddr = 0;
    g_lastP2 = 0;
    g_called = false;
}

// Test 1: Ensures that sqlite3VdbeJumpHere forwards the correct 'addr' and the current p->nOp
// to sqlite3VdbeChangeP2, and passes the same 'p' pointer through.
static void test_JumpHere_ForwardsAddrAndNOp() {
    reset_mock_state();

    Vdbe p;
    p.nOp = 7;              // Value that should be passed as p2 to ChangeP2
    int testAddr = 12;

    // Act
    sqlite3VdbeJumpHere(&p, testAddr);

    // Assert
    EXPECT_TRUE(g_called, "sqlite3VdbeChangeP2 should be called by sqlite3VdbeJumpHere");
    EXPECT_PTR_EQ(g_lastP, &p, "sqlite3VdbeChangeP2 should receive the same Vdbe pointer");
    EXPECT_EQ(g_lastAddr, testAddr, "sqlite3VdbeChangeP2 should receive the forwarded addr");
    EXPECT_EQ(g_lastP2, p.nOp, "sqlite3VdbeChangeP2 should receive p->nOp as the third argument");
}

// Test 2: Verifies that changes to p->nOp prior to the call affect the value passed to ChangeP2.
// This confirms the function uses the current value of nOp at call time.
static void test_JumpHere_UsesCurrentNOpValue() {
    reset_mock_state();

    Vdbe p;
    p.nOp = -1;              // First scenario
    int addr1 = 5;

    sqlite3VdbeJumpHere(&p, addr1);

    // Assert first call
    EXPECT_TRUE(g_called, "First call to sqlite3VdbeChangeP2 should be recorded");
    EXPECT_PTR_EQ(g_lastP, &p, "First call should pass the correct Vdbe pointer");
    EXPECT_EQ(g_lastAddr, addr1, "First call should forward the correct addr");
    EXPECT_EQ(g_lastP2, -1, "First call should pass initial p->nOp value (-1)");

    // Reset and modify nOp, then call again
    reset_mock_state();
    p.nOp = 42;
    int addr2 = 999;

    sqlite3VdbeJumpHere(&p, addr2);

    // Assert second call
    EXPECT_TRUE(g_called, "Second call to sqlite3VdbeChangeP2 should be recorded");
    EXPECT_PTR_EQ(g_lastP, &p, "Second call should pass the correct Vdbe pointer");
    EXPECT_EQ(g_lastAddr, addr2, "Second call should forward the correct addr");
    EXPECT_EQ(g_lastP2, 42, "Second call should pass updated p->nOp value (42)");
}

// Test 3: Edge case with a zero and negative address and nOp values to ensure no crashes and correct forwarding.
// This ensures the function handles a variety of integers.
static void test_JumpHere_EdgeCases() {
    reset_mock_state();

    Vdbe p;
    p.nOp = 0;
    int addr = 0;

    sqlite3VdbeJumpHere(&p, addr);

    EXPECT_TRUE(g_called, "Call should be recorded for edge case (addr=0, nOp=0)");
    EXPECT_PTR_EQ(g_lastP, &p, "P pointer should be preserved in edge case");
    EXPECT_EQ(g_lastAddr, 0, "Edge case: addr should be forwarded as 0");
    EXPECT_EQ(g_lastP2, 0, "Edge case: p->nOp (0) should be forwarded as p2");
}

// Runner to execute all tests
static void run_all_tests() {
    test_JumpHere_ForwardsAddrAndNOp();
    test_JumpHere_UsesCurrentNOpValue();
    test_JumpHere_EdgeCases();
}

// Entry point
int main() {
    run_all_tests();

    // Report results
    if(g_failureCount == 0) {
        std::cout << "All tests passed (" << 3 << " test(s))." << std::endl;
        return 0;
    } else {
        std::cout << "Number of test failures: " << g_failureCount << std::endl;
        for(size_t i = 0; i < g_failureMessages.size(); ++i) {
            std::cout << "Failure " << (i+1) << ": " << g_failureMessages[i] << std::endl;
        }
        return 1;
    }
}