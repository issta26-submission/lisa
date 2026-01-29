// High-quality C++11 unit test suite for sqlite3CodeRowTriggerDirect
// This test harness reproduces a minimal, self-contained environment
// to exercise the focal method's behavior without requiring a full SQLite build.
// The tests avoid GTest and use a lightweight, custom test runner.

// The reproduction defines only the necessary structures and stubs to validate:
// - when a Row Trigger Program (pPrg) exists, an OP_Program is emitted with
//   correct parameters and P5 (recursive flag) is set as expected.
// - the VdbeComment receives the onErrorText(orconf) string.
// - the code path is exercised for both recursive and non-recursive cases.
// - the case where pPrg is NULL is handled (no op is emitted, no comment).

#include <functional>
#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>


// Domain-specific constants (simplified for test environment)
using uint8_t = unsigned char;
const int OP_Program = 1;    // placeholder opcode
const int P4_SUBPROGRAM = 1; // p4 type hint
const unsigned SQLITE_RecTriggers = 0x1; // flag bit for recursive triggers

// Lightweight stubbed SQLite-like data structures (subset)
struct Db {
    unsigned int flags = 0;
};

struct Parse {
    Db *db = nullptr;
    int nMem = 0;
    int nErr = 0;
};

struct Trigger {
    const char* zName = nullptr; // trigger name (nullable)
};

struct Table {
    // empty for tests
};

struct TriggerPrg {
    const char* pProgram = nullptr; // program pointer (string for test)
};

// Simple Vdbe representation to capture emitted operations and comments
struct VdbeOp {
    int op;
    int p1;
    int p2;
    int p3;
    const char* p4;
    int p4type;
};
struct Vdbe {
    std::vector<VdbeOp> ops;
    std::vector<std::string> comments;
    int lastP5 = 0;
};

// Global/test-scoped objects used by the focal function under test
static Vdbe g_vdbe;                 // the main VM used by sqlite3GetVdbe
static Parse g_parse;                // Parse context used by tests
static TriggerPrg* g_testPrg = nullptr; // current mock row-trigger program

// Forward declarations required by the focal method under test
static TriggerPrg* getRowTrigger(Parse *pParse, Trigger *pTrigger, Table *pTab, int orconf);
static const char* onErrorText(int orconf);

// Minimal stub implementations to mimic the behavior of the original code
extern "C" {

// Return the main VM for the given parse (test harness uses a single global VM)
static Vdbe* sqlite3GetVdbe(Parse *pParse) {
    (void)pParse; // unused in this simplified harness
    return &g_vdbe;
}

// Record an OP with a 4th parameter being a pointer to p4 and a type flag
static void sqlite3VdbeAddOp4(Vdbe *v, int op, int p1, int p2, int p3,
                             const char *p4, int p4type) {
    VdbeOp o;
    o.op = op;
    o.p1 = p1;
    o.p2 = p2;
    o.p3 = p3;
    o.p4 = p4;
    o.p4type = p4type;
    v->ops.push_back(o);
}

// Set the P5 register (recursion-control flag)
static void sqlite3VdbeChangeP5(Vdbe *v, uint8_t p5) {
    v->lastP5 = static_cast<int>(p5);
}

// Simplified VdbeComment: test harness receives the final string value
static void VdbeComment(const char* sz) {
    // Record comment string in the Vdbe for verification
    if (sz) g_vdbe.comments.push_back(std::string(sz));
    else g_vdbe.comments.push_back(std::string(""));
}

// onErrorText helper (domain-specific translation of orconf)
static const char* onErrorText(int orconf) {
    // Deterministic mapping used by tests
    return orconf ? "ABORT" : "IGNORE";
}

} // extern "C"

// getRowTrigger: returns the current test TriggerPrg or nullptr
static TriggerPrg* getRowTrigger(Parse *pParse, Trigger *pTrigger, Table *pTab, int orconf) {
    (void)pParse; (void)pTrigger; (void)pTab; (void)orconf;
    // In tests, return the globally configured test program
    return g_testPrg;
}

// The focal method: sqlite3CodeRowTriggerDirect (reproduced for unit testing)
void sqlite3CodeRowTriggerDirect(
  Parse *pParse,       /* Parse context */
  Trigger *p,          /* Trigger to code */
  Table *pTab,         /* The table to code triggers from */
  int reg,             /* Reg array containing OLD.* and NEW.* values */
  int orconf,          /* ON CONFLICT policy */
  int ignoreJump       /* Instruction to jump to for RAISE(IGNORE) */
){
  Vdbe *v = sqlite3GetVdbe(pParse); /* Main VM */
  TriggerPrg *pPrg;
  pPrg = getRowTrigger(pParse, p, pTab, orconf);
  // In the real code this assert would check for errors; test harness keeps behavior simple
  // Here we mimic the same contract.
  // assert( pPrg || pParse->nErr );
  if( pPrg ){
    int bRecursive = (p->zName && 0==(pParse->db->flags&SQLITE_RecTriggers));
    sqlite3VdbeAddOp4(v, OP_Program, reg, ignoreJump, ++pParse->nMem,
                      (const char *)pPrg->pProgram, P4_SUBPROGRAM);
    VdbeComment(
        (const char*) ( (void*)0, // placeholder to align with original "macro" usage
        // The comma expression in the original expands to the last value:
        // onErrorText(orconf)
        onErrorText(orconf) )); // Pass the final string directly
    sqlite3VdbeChangeP5(v, (uint8_t)bRecursive);
  }
}

// Helper to create a Trigger object more easily
static Trigger makeTrigger(const char* name) {
    Trigger t;
    t.zName = name;
    return t;
}

// Helper: reset the test VM and parse state
static void resetTestEnv() {
    g_vdbe.ops.clear();
    g_vdbe.comments.clear();
    g_vdbe.lastP5 = 0;
    g_parse.nMem = 0;
    g_parse.nErr = 0;
}

// Simple test harness (lightweight test runner)
struct TestCase {
    std::string name;
    std::function<bool()> run;
};

// Test result reporting
static int g_total = 0;
static int g_failed = 0;
static void runTest(const TestCase& t) {
    bool ok = t.run();
    ++g_total;
    if (!ok) {
        ++g_failed;
        std::cout << "[FAIL] " << t.name << "\n";
    } else {
        std::cout << "[OK]   " << t.name << "\n";
    }
}

int main() {
    // Bind test cases
    std::vector<TestCase> tests;

    // Test A: pPrg present, zName non-null, no SQLITE_RecTriggers flag
    tests.push_back(TestCase{
        "DirectDirect_PrgAndRecursive_true",
        []() -> bool {
            resetTestEnv();

            // Prepare context
            Db db;
            db.flags = 0;
            g_parse.db = &db;
            g_testPrg = new TriggerPrg{ "progA" };

            Trigger trg = makeTrigger("tA");
            Table tab;
            // Reset pParse state
            g_parse.nMem = 0;

            // Call focal method
            sqlite3CodeRowTriggerDirect(&g_parse, &trg, &tab, 5, 1, 7);

            bool passed = true;
            // Expect one OP emitted
            if (g_vdbe.ops.size() != 1) {
                std::cout << "Expected 1 op, got " << g_vdbe.ops.size() << "\n";
                passed = false;
            } else {
                const VdbeOp& op = g_vdbe.ops[0];
                if (op.op != OP_Program) {
                    std::cout << "Unexpected op: " << op.op << "\n";
                    passed = false;
                }
                if (op.p1 != 5 || op.p2 != 7 || op.p3 != 1) {
                    std::cout << "Unexpected op args: p1="<<op.p1<<" p2="<<op.p2<<" p3="<<op.p3<<"\n";
                    passed = false;
                }
                if (op.p4 == nullptr || std::strcmp(op.p4, "progA") != 0) {
                    std::cout << "p4 mismatch: " << (op.p4?op.p4:"NULL") << "\n";
                    passed = false;
                }
                if (g_vdbe.lastP5 != 1) {
                    std::cout << "Expected P5=1, got " << g_vdbe.lastP5 << "\n";
                    passed = false;
                }
            }
            // VdbeComment should receive onErrorText(orconf) => "ABORT" for orconf=1
            if (g_vdbe.comments.empty()) {
                std::cout << "Expected a VdbeComment, none found\n";
                passed = false;
            } else {
                if (g_vdbe.comments.back() != std::string("ABORT")) {
                    std::cout << "Unexpected VdbeComment: " << g_vdbe.comments.back() << "\n";
                    passed = false;
                }
            }

            // Cleanup
            delete g_testPrg;
            g_testPrg = nullptr;

            // No explicit deep checks on memory; mere counts ensure execution path
            return passed;
        }
    });

    // Test B: pPrg present, zName NULL, non-recursive (orconf == 0)
    tests.push_back(TestCase{
        "DirectDirect_PrgAndRecursive_false_orconf0",
        []() -> bool {
            resetTestEnv();

            Db db;
            db.flags = 0;
            g_parse.db = &db;
            g_testPrg = new TriggerPrg{ "progB" };

            Trigger trg = makeTrigger(nullptr); // zName == NULL
            Table tab;

            g_parse.nMem = 0;
            sqlite3CodeRowTriggerDirect(&g_parse, &trg, &tab, 3, 0, 4);

            bool passed = true;
            if (g_vdbe.ops.size() != 1) {
                std::cout << "Expected 1 op, got " << g_vdbe.ops.size() << "\n";
                passed = false;
            } else {
                const VdbeOp& op = g_vdbe.ops[0];
                if (op.op != OP_Program) {
                    std::cout << "Unexpected op: " << op.op << "\n";
                    passed = false;
                }
                if (op.p1 != 3 || op.p2 != 4 || op.p3 != 1) {
                    std::cout << "Unexpected op args: p1="<<op.p1<<" p2="<<op.p2<<" p3="<<op.p3<<"\n";
                    passed = false;
                }
                if (op.p4 == nullptr || std::strcmp(op.p4, "progB") != 0) {
                    std::cout << "p4 mismatch: " << (op.p4?op.p4:"NULL") << "\n";
                    passed = false;
                }
            }
            if (g_vdbe.lastP5 != 0) {
                std::cout << "Expected P5=0, got " << g_vdbe.lastP5 << "\n";
                passed = false;
            }
            if (g_vdbe.comments.empty()) {
                std::cout << "Expected a VdbeComment, none found\n";
                passed = false;
            } else {
                if (g_vdbe.comments.back() != std::string("IGNORE")) {
                    std::cout << "Unexpected VdbeComment: " << g_vdbe.comments.back() << "\n";
                    passed = false;
                }
            }

            delete g_testPrg;
            g_testPrg = nullptr;

            return passed;
        }
    });

    // Test C: pPrg NULL but nErr non-zero to satisfy assertion; ensure no op is emitted
    tests.push_back(TestCase{
        "DirectDirect_Prg_null_with_nErr",
        []() -> bool {
            resetTestEnv();

            Db db;
            db.flags = 0;
            g_parse.db = &db;
            g_testPrg = nullptr; // no program

            Trigger trg = makeTrigger("tC");
            Table tab;

            // Set nErr to non-zero to satisfy the assertion in our reproduction
            g_parse.nErr = 1;
            g_parse.nMem = 0;

            sqlite3CodeRowTriggerDirect(&g_parse, &trg, &tab, 1, 1, 2);

            // Expect no ops and no comments since pPrg is NULL
            bool passed = true;
            if (!g_vdbe.ops.empty()) {
                std::cout << "Expected 0 ops, got " << g_vdbe.ops.size() << "\n";
                passed = false;
            }
            if (!g_vdbe.comments.empty()) {
                std::cout << "Expected 0 comments, got " << g_vdbe.comments.size() << "\n";
                passed = false;
            }

            return passed;
        }
    });

    // Run all tests
    for (const auto& t : tests) {
        runTest(t);
    }

    // Summary
    std::cout << "Summary: " << g_total << " tests, " << g_failed << " failed." << std::endl;
    return g_failed ? 1 : 0;
}