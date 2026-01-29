// Focal method analysis and test suite for sqlite3VdbeEndCoroutine
// Focal method (from <FOCAL_METHOD>):
// void sqlite3VdbeEndCoroutine(Vdbe *v, int regYield){
//   sqlite3VdbeAddOp1(v, OP_EndCoroutine, regYield);
//   /* Clear the temporary register cache, thereby ensuring that each
//   ** co-routine has its own independent set of registers, because co-routines
//   ** might expect their registers to be preserved across an OP_Yield, and
//   ** that could cause problems if two or more co-routines are using the same
//   ** temporary register.
//   */
//   v->pParse->nTempReg = 0;
//   v->pParse->nRangeReg = 0;
// }
// </FOCAL_METHOD>

// Step 1: Candidate Keywords extracted from the focal method and dependencies
// - OP_EndCoroutine
// - Vdbe
// - Parse
// - v (Vdbe pointer)
// - v->pParse (Parse pointer)
// - v->pParse->nTempReg
// - v->pParse->nRangeReg
// - sqlite3VdbeAddOp1
// These keywords guide the test construction to verify correct opcode emission
// and correct register cache reset behavior.

// Step 2 & 3: Minimal C++11 test harness (without GTest) that exercises the focal method
// Note: The SQLite project uses many static/internal helpers. For unit testing
// purposes we provide minimal stubbed definitions sufficient to verify the focal logic.

#include <iostream>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <string>


// Expose a minimal environment mimicking the parts of vdbeaux.c/SQLite used by
// sqlite3VdbeEndCoroutine. This is intentionally tiny and self-contained for
// unit testing purposes in a C++11 test harness.

// Candidate Keywords (for reference in comments)
//
// OP_EndCoroutine: opcode constant used by EndCoroutine path.
// Vdbe: the VM container used by the function under test.
// Parse: the parsing state holding register caches.
// v->pParse: pointer to Parse inside the Vdbe.
// v: Vdbe instance.
// v->pParse->nTempReg and v->pParse->nRangeReg: internal caches reset by EndCoroutine.
// sqlite3VdbeAddOp1: helper that records the emitted opcode and operand for test verification.

#define OP_EndCoroutine 1234  // Minimal stand-in for the actual SQLite opcode

// Minimal stub types to support the focal function
struct Parse {
    int nTempReg;
    int nRangeReg;
};

struct Vdbe {
    Parse *pParse;
};

// Global test hook to capture the effect of sqlite3VdbeAddOp1(v, op, p1)
static int g_lastOp1 = -1;
static int g_lastP1 = -1;

// Stubbed counterpart to sqlite3VdbeAddOp1 used by the focal function
extern "C" void sqlite3VdbeAddOp1(Vdbe *p, int op, int p1) {
    // Record last emitted opcode and its first parameter for test assertions
    g_lastOp1 = op;
    g_lastP1 = p1;
}

// Focal method under test (reproduced in test harness for self-contained compilation)
extern "C" void sqlite3VdbeEndCoroutine(Vdbe *v, int regYield) {
    // This mirrors the logic in the provided source:
    sqlite3VdbeAddOp1(v, OP_EndCoroutine, regYield);
    v->pParse->nTempReg = 0;
    v->pParse->nRangeReg = 0;
}

// Simple test logger to accumulate failures without terminating the test run
class TestLogger {
public:
    int failures;
    TestLogger() : failures(0) {}
    void fail(const std::string &msg) {
        ++failures;
        std::cerr << "Test failure: " << msg << std::endl;
    }
    template<typename T, typename U>
    void equal(const T &a, const U &b, const std::string &msg) {
        if (!(a == b)) {
            fail(msg + " | expected: " + to_string(b) + " actual: " + to_string(a));
        }
    }
private:
    // Simple helper to convert basic types to string for messaging
    template<typename T>
    std::string to_string(const T &val) {
        // Fallback for generic types
        return std::to_string(val);
    }
    // Overloads for common types to avoid template issues in some compilers
    std::string to_string(const char* s) { return std::string(s); }
    std::string to_string(const std::string &s) { return s; }
};

// Test 1: Verify that sqlite3VdbeEndCoroutine emits OP_EndCoroutine with the correct regYield
// and resets the temporary/register caches to zero.
void test_endCoroutine_emits_correct_op_and_resets_registers(TestLogger &log) {
    // Arrange
    Parse p;
    p.nTempReg = 7;   // non-zero initial value to verify reset
    p.nRangeReg = 9;  // non-zero initial value to verify reset
    Vdbe v;
    v.pParse = &p;

    // Act
    int testYield = 5;
    sqlite3VdbeEndCoroutine(&v, testYield);

    // Assert
    log.equal(g_lastOp1, OP_EndCoroutine, "sqlite3VdbeEndCoroutine should emit OP_EndCoroutine");
    log.equal(g_lastP1, testYield, "sqlite3VdbeEndCoroutine should pass regYield to sqlite3VdbeAddOp1");
    log.equal(p.nTempReg, 0, "Parse.nTempReg should be reset to 0");
    log.equal(p.nRangeReg, 0, "Parse.nRangeReg should be reset to 0");
}

// Test 2: Idempotent behavior check: ensure repeated calls reset registers again and emit correct opcode
void test_endCoroutine_idempotent_behavior(TestLogger &log) {
    // Arrange
    Parse p;
    p.nTempReg = 1;
    p.nRangeReg = 2;
    Vdbe v;
    v.pParse = &p;

    // Act: first call with a non-zero regYield
    int yield1 = 3;
    sqlite3VdbeEndCoroutine(&v, yield1);

    // Assert first call
    log.equal(g_lastOp1, OP_EndCoroutine, "First call should emit OP_EndCoroutine");
    log.equal(g_lastP1, yield1, "First call should pass regYield to sqlite3VdbeAddOp1");
    log.equal(p.nTempReg, 0, "Parse.nTempReg should be reset to 0 after first call");
    log.equal(p.nRangeReg, 0, "Parse.nRangeReg should be reset to 0 after first call");

    // Act: second call with a different yield
    int yield2 = 0;
    // Prepare non-zero caches again to ensure reset occurs again
    p.nTempReg = 13;
    p.nRangeReg = 27;
    sqlite3VdbeEndCoroutine(&v, yield2);

    // Assert second call
    log.equal(g_lastOp1, OP_EndCoroutine, "Second call should again emit OP_EndCoroutine");
    log.equal(g_lastP1, yield2, "Second call should pass regYield to sqlite3VdbeAddOp1");
    log.equal(p.nTempReg, 0, "Parse.nTempReg should be reset to 0 after second call");
    log.equal(p.nRangeReg, 0, "Parse.nRangeReg should be reset to 0 after second call");
}

int main() {
    // Run the focused unit tests for sqlite3VdbeEndCoroutine
    TestLogger log;

    // Explanatory comments for each unit test
    // Test 1: Core functionality – ensure the opcode is emitted with the correct argument
    test_endCoroutine_emits_correct_op_and_resets_registers(log);

    // Test 2: Idempotence / repeated invocation – ensure repeated calls still reset state and emit correctly
    test_endCoroutine_idempotent_behavior(log);

    if (log.failures == 0) {
        std::cout << "All tests passed for sqlite3VdbeEndCoroutine.\n";
        return 0;
    } else {
        std::cout << log.failures << " test failure(s) detected for sqlite3VdbeEndCoroutine.\n";
        return 1;
    }
}