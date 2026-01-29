// Unit test suite for sqlite3Fts5IndexEntryCksum
// Step-by-step planning comments:
// - Step 1 (Program Understanding): The target function computes a 64-bit checksum-like value:
//     ret = iRowid
//     ret += (ret<<3) + iCol
//     ret += (ret<<3) + iPos
//     if (iIdx >= 0) ret += (ret<<3) + (FTS5_MAIN_PREFIX + iIdx)
//     for each character in pTerm (length nTerm): ret += (ret<<3) + pTerm[i]
// - Step 2 (Unit Test Generation): We'll cover true/false branches for iIdx >= 0 and for-loop execution
//   using a combination of tests with nTerm = 0 and nTerm > 0. When nTerm == 0 the behavior is deterministic
//   with respect to iIdx (differences can be validated without knowing FTS5_MAIN_PREFIX numerically).
// - Step 3 (Test Case Refinement): We'll implement a small custom test harness (no GTest) that uses
//   the project's own types/macros (fts5Int.h) to compute expected values where possible and verify
//   invariants (e.g., for nTerm == 0 with iIdx >= 0, successive iIdx values increment the result by 1).
//   For other cases, we compute direct expected values by re-emulating the arithmetic in tests.

#include <iostream>
#include <cstring>
#include <cstdint>
#include <fts5Int.h>


// Include project dependencies to obtain type definitions (u64, i64) and FTS5_MAIN_PREFIX.

// Declare the focal function with C linkage to avoid name mangling issues when linking with the C file.
extern "C" u64 sqlite3Fts5IndexEntryCksum(i64 iRowid, int iCol, int iPos, int iIdx,
                                        const char *pTerm, int nTerm);

// Simple test harness (non-terminating assertions): accumulates failures and reports at the end.
static int g_failures = 0;

// Helper: compute the base portion A = iRowid; A += (A<<3) + iCol; A += (A<<3) + iPos;
static inline u64 baseA(i64 iRowid, int iCol, int iPos) {
    u64 A = static_cast<u64>(iRowid);
    A += (A << 3) + static_cast<u64>(iCol);
    A += (A << 3) + static_cast<u64>(iPos);
    return A;
}

// Helper: test reporting
static void report_fail(const char* test_name, uint64_t expected, uint64_t actual) {
    std::cerr << "FAIL: " << test_name << " expected " << expected << " actual " << actual << "\n";
    g_failures++;
}

// Test Case 1: iIdx < 0, nTerm == 0
// Verifies the branch where the iIdx-dependent addition is skipped and only the base arithmetic is executed.
static void test_case_negative_idx_no_term() {
    const char* test_name = "Case1: iIdx<0, nTerm=0";
    i64 iRowid = 1;
    int iCol = 2;
    int iPos = 3;
    int iIdx = -1;
    const char* pTerm = nullptr;
    int nTerm = 0;

    u64 actual = sqlite3Fts5IndexEntryCksum(iRowid, iCol, iPos, iIdx, pTerm, nTerm);
    u64 expected = baseA(iRowid, iCol, iPos); // no iIdx term, no pTerm loop

    if (actual != expected) report_fail(test_name, expected, actual);
}

// Test Case 2: iIdx >= 0, nTerm == 0
// Verifies the iIdx branch adds (FTS5_MAIN_PREFIX + iIdx) in addition to the base.
static void test_case_positive_idx_no_term() {
    const char* test_name = "Case2: iIdx>=0, nTerm=0";
    i64 iRowid = 7;
    int iCol = 4;
    int iPos = 1;
    int iIdx = 0;
    const char* pTerm = nullptr;
    int nTerm = 0;

    u64 actual = sqlite3Fts5IndexEntryCksum(iRowid, iCol, iPos, iIdx, pTerm, nTerm);
    u64 A = baseA(iRowid, iCol, iPos);
    // expected = A + (A<<3) + (FTS5_MAIN_PREFIX + iIdx)
    u64 expected = A;
    expected += (expected << 3) + (FTS5_MAIN_PREFIX + iIdx);

    if (actual != expected) report_fail(test_name, expected, actual);
}

// Test Case 3: iIdx >= 0, nTerm == 0 with non-zero iIdx to exercise multiple values
// Verifies that the difference between two positive iIdx results equals the delta in iIdx
// (i.e., R(k) - R(j) = k - j) when nTerm == 0.
static void test_case_positive_idx_no_term_delta() {
    const char* test_name = "Case3: iIdx delta for nTerm=0";
    i64 iRowid = 3;
    int iCol = 2;
    int iPos = 5;
    int iIdx_a = 0;
    int iIdx_b = 5;
    const char* pTerm = nullptr;
    int nTerm = 0;

    u64 Ra = sqlite3Fts5IndexEntryCksum(iRowid, iCol, iPos, iIdx_a, pTerm, nTerm);
    u64 Rb = sqlite3Fts5IndexEntryCksum(iRowid, iCol, iPos, iIdx_b, pTerm, nTerm);

    // For nTerm == 0, R_b - R_a should equal (iIdx_b - iIdx_a) = 5
    uint64_t delta = Rb - Ra;
    if (delta != static_cast<uint64_t>(iIdx_b - iIdx_a)) {
        report_fail(test_name, static_cast<uint64_t>(iIdx_b - iIdx_a), delta);
    }
}

// Test Case 4: iIdx < 0, nTerm > 0
// Ensures the for-loop over pTerm executes and contributes to the result.
static void test_case_negative_idx_with_term() {
    const char* test_name = "Case4: iIdx<0, nTerm>0";
    i64 iRowid = 10;
    int iCol = 1;
    int iPos = 2;
    int iIdx = -1;
    const char pTerm[] = "ABC"; // nTerm = 3
    int nTerm = 3;

    u64 actual = sqlite3Fts5IndexEntryCksum(iRowid, iCol, iPos, iIdx, pTerm, nTerm);

    // Manually compute expected using the exact same arithmetic as the function
    // A = baseA(iRowid, iCol, iPos)
    u64 A = baseA(iRowid, iCol, iPos);
    // Since iIdx < 0, skip the iIdx term. Process pTerm[0..2] in order.
    u64 expected = A;
    for (int i = 0; i < nTerm; ++i) {
        expected += (expected << 3) + static_cast<uint8_t>(pTerm[i]);
    }

    if (actual != expected) report_fail(test_name, expected, actual);
}

// Test Case 5: iIdx < 0, nTerm > 0 with longer input
static void test_case_negative_idx_with_long_term() {
    const char* test_name = "Case5: iIdx<0, nTerm>0 (long term)";
    i64 iRowid = -15;
    int iCol = 7;
    int iPos = 9;
    int iIdx = -2;
    const char pTerm[] = "DEFG"; // nTerm = 4
    int nTerm = 4;

    u64 actual = sqlite3Fts5IndexEntryCksum(iRowid, iCol, iPos, iIdx, pTerm, nTerm);

    // Manual compute
    u64 A = baseA(iRowid, iCol, iPos);
    u64 expected = A;
    for (int i = 0; i < nTerm; ++i) {
        expected += (expected << 3) + static_cast<uint8_t>(pTerm[i]);
    }

    if (actual != expected) report_fail(test_name, expected, actual);
}

// Test Case 6: iIdx >= 0, nTerm > 0
// Verifies that we hit both the iIdx branch and the term loop, giving a nontrivial composite result.
// We validate against a hand-computed expected value.
static void test_case_positive_idx_with_term() {
    const char* test_name = "Case6: iIdx>=0, nTerm>0";
    i64 iRowid = 2;
    int iCol = 5;
    int iPos = 8;
    int iIdx = 1;
    const char pTerm[] = "QZ"; // nTerm = 2
    int nTerm = 2;

    u64 actual = sqlite3Fts5IndexEntryCksum(iRowid, iCol, iPos, iIdx, pTerm, nTerm);

    // Manual compute
    u64 A = baseA(iRowid, iCol, iPos);
    u64 expected = A;
    expected += (expected << 3) + (FTS5_MAIN_PREFIX + iIdx);
    // Term loop
    for (int i = 0; i < nTerm; ++i) {
        expected += (expected << 3) + static_cast<uint8_t>(pTerm[i]);
    }

    if (actual != expected) report_fail(test_name, expected, actual);
}

// Driver
int main() {
    // Run all test cases
    test_case_negative_idx_no_term();
    test_case_positive_idx_no_term();
    test_case_positive_idx_no_term_delta();
    test_case_negative_idx_with_term();
    test_case_negative_idx_with_long_term();
    test_case_positive_idx_with_term();

    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED\n";
        return 1;
    }
}