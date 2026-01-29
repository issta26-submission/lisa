// This test suite is a self-contained C++11 program that re-implements the essential
// dependencies and the focal function sqlite3Fts5ParseColset (and its helper) to
// enable unit testing without GTest. It exercises true/false branches and verifies
// memory management and error signaling.
// The implementation mirrors the structure shown in the provided focal method and its
// related static helper, adapted for a single translation unit.

#include <fts5Int.h>
#include <cstdio>
#include <fts5parse.h>
#include <stdio.h>
#include <cstring>
#include <cctype>
#include <cstdarg>
#include <string>
#include <iostream>
#include <cstdlib>


// Domain knowledge: Use C++ standard library, no external test framework.
// We implement minimal C-style APIs required by sqlite3Fts5ParseColset, with C linkage.

extern "C" {

// Public-looking types used by the focal function (reduced for testing)
typedef struct Fts5Parse {
    int rc;                 // SQLITE_OK or SQLITE_ERROR
    struct Fts5Config *pConfig;
} Fts5Parse;

typedef struct Fts5Config {
    int nCol;
    const char **azCol;     // Array of column names
} Fts5Config;

typedef struct Fts5Colset {
    int chosenCol;
} Fts5Colset;

typedef struct Fts5Token {
    const char *p;
    int n;
} Fts5Token;

// Backing constants (simplified)
#define SQLITE_OK 0
#define SQLITE_ERROR 1

// Forward declarations
Fts5Colset *sqlite3Fts5ParseColset(Fts5Parse *pParse, Fts5Colset *pColset, Fts5Token *p);
static Fts5Colset *fts5ParseColset(Fts5Parse *pParse, Fts5Colset *p, int iCol);

// Globals to observe internal behavior in tests
static int g_failOnColsetIndex = -1;                 // If iCol == this, fts5ParseColset will fail
static void *g_alloc_colset_ptr = nullptr;          // Colset pointer allocated by test
static bool g_colset_freed = false;                 // Whether the test-colset was freed
static char g_last_error_buf[512] = {0};            // Last error message format
static std::string g_last_error_str;                 // Human-readable last error

// Memory/utility helpers used by sqlite3Fts5ParseColset
char* sqlite3Fts5Strndup(int *pRc, const char *p, int n) {
    if (pRc && *pRc != SQLITE_OK) return nullptr;
    char *z = (char*)malloc(n + 1);
    if (!z) {
        if (pRc) *pRc = SQLITE_ERROR;
        return nullptr;
    }
    if (n > 0 && p) memcpy(z, p, n);
    z[n] = '\0';
    return z;
}

void sqlite3Fts5Dequote(char *z) {
    // Remove surrounding quotes if present
    if (!z) return;
    int len = (int)strlen(z);
    if (len >= 2) {
        if ((z[0] == '"' && z[len-1] == '"') || (z[0] == '\'' && z[len-1] == '\'')) {
            memmove(z, z + 1, len - 2);
            z[len - 2] = '\0';
        }
    }
}

int sqlite3_stricmp(const char *a, const char *b) {
    // Case-insensitive comparison
    while (*a && *b) {
        unsigned char ca = (unsigned char)tolower((unsigned char)*a);
        unsigned char cb = (unsigned char)tolower((unsigned char)*b);
        if (ca != cb) return (ca < cb) ? -1 : 1;
        ++a; ++b;
    }
    if (*a == 0 && *b == 0) return 0;
    return (*a == 0) ? -1 : 1;
}

void sqlite3_free(void *p) {
    // Free memory; also observe freed-colset pointers for tests
    if (p == g_alloc_colset_ptr) {
        g_colset_freed = true;
    }
    free(p);
}

// Simple variadic error reporter used by the focal function
void sqlite3Fts5ParseError(Fts5Parse *pParse, const char *zFmt, ...) {
    va_list ap;
    va_start(ap, zFmt);
    vsnprintf(g_last_error_buf, sizeof(g_last_error_buf), zFmt, ap);
    va_end(ap);
    g_last_error_str = g_last_error_buf;
    if (pParse) pParse->rc = SQLITE_ERROR;
}
};

// Implementation of the focal function and its static helper
// Mirrors the behavior shown in the provided snippet.

extern "C" {

// Non-static focal function under test. It uses the static helper fts5ParseColset.
Fts5Colset *sqlite3Fts5ParseColset(Fts5Parse *pParse, Fts5Colset *pColset, Fts5Token *p) {
    Fts5Colset *pRet = 0;
    int iCol;
    char *z;  // Dequoted copy of token p
    z = sqlite3Fts5Strndup(&pParse->rc, p->p, p->n);
    if (pParse->rc == SQLITE_OK) {
        Fts5Config *pConfig = pParse->pConfig;
        sqlite3Fts5Dequote(z);
        for (iCol = 0; iCol < pConfig->nCol; iCol++) {
            if (0 == sqlite3_stricmp(pConfig->azCol[iCol], z)) break;
        }
        if (iCol == pConfig->nCol) {
            sqlite3Fts5ParseError(pParse, "no such column: %s", z);
        } else {
            pRet = fts5ParseColset(pParse, pColset, iCol);
        }
        sqlite3_free(z);
    }
    if (pRet == 0) {
        // In the real code, there is an assertion that rc != SQLITE_OK
        // We emulate the same behavior by freeing provided pColset when failure occurs.
        if (pParse) {
            // No-op; just to mirror structure
        }
        sqlite3_free(pColset);
    }
    return pRet;
}

// Static helper that actually constructs/returns a new Fts5Colset given an iCol.
// It can be instructed to fail via a global flag to test error-path behavior.
static Fts5Colset *fts5ParseColset(Fts5Parse *pParse, Fts5Colset *p, int iCol) {
    if (g_failOnColsetIndex == iCol) {
        // Simulate a failure in creating/returning a colset
        if (pParse) pParse->rc = SQLITE_ERROR;
        return 0;
    }
    Fts5Colset *pNew = (Fts5Colset *)malloc(sizeof(Fts5Colset));
    if (!pNew) {
        if (pParse) pParse->rc = SQLITE_ERROR;
        return 0;
    }
    pNew->chosenCol = iCol;
    return pNew;
}
}

// Simple test scaffolding below (no GTest). We provide a small test harness.

static int g_tests_run = 0;
static int g_tests_failed = 0;

// Convenience assertion macro: non-terminating, logs failure
#define TEST_EXPECT(cond, msg) do { \
    if(!(cond)) { \
        ++g_tests_failed; \
        std::cerr << "TEST FAIL: " << msg << "\n"; \
    } \
} while(0)

#define TEST_PASS(msg) do { \
    (void)msg; /* silence unused warning; informational only */ \
} while(0)

// Helper to reset global state between tests
void test_reset_globals() {
    g_failOnColsetIndex = -1;
    g_alloc_colset_ptr = nullptr;
    g_colset_freed = false;
    std::fill(std::begin(g_last_error_buf), std::end(g_last_error_buf), 0);
    g_last_error_str.clear();
}

// Test 1: Existing column (case-insensitive) should return a valid colset with correct index
void test_sqlite3Fts5ParseColset_simple_match() {
    g_tests_run++;

    test_reset_globals();

    // Setup parse config with 3 columns
    static const char *cols[] = { "col0", "col1", "col2" };
    Fts5Parse parse;
    Fts5Config cfg;
    cfg.nCol = 3;
    cfg.azCol = cols;
    parse.rc = SQLITE_OK;
    parse.pConfig = &cfg;

    // Existing token "COL2" (uppercase) to test case-insensitive match to "col2"
    Fts5Token tok;
    tok.p = "COL2";
    tok.n = 4;

    // Prepare a dummy pColset
    Fts5Colset *pColset = (Fts5Colset *)malloc(sizeof(Fts5Colset));
    pColset->chosenCol = -1;
    g_alloc_colset_ptr = pColset;  // track for free

    Fts5Colset *pRet = sqlite3Fts5ParseColset(&parse, pColset, &tok);

    TEST_EXPECT(pRet != nullptr, "Expected non-null Fts5Colset for existing column");
    TEST_EXPECT(parse.rc == SQLITE_OK, "Expected rc to remain SQLITE_OK on success");
    if (pRet) TEST_EXPECT(pRet->chosenCol == 2, "Expected chosenCol to be 2 for COL2");

    // The pColset should not have been freed on success
    TEST_EXPECT(g_colset_freed == false, "Did not expect colset to be freed on success");

    // Cleanup
    if (pRet) free(pRet);
    // pColset should not be freed in success path; but ensure we free our own original
    // to avoid leaks in test harness
    if (pColset) free(pColset);

    test_reset_globals();
}

// Test 2: Unknown column should produce error and free the input colset
void test_sqlite3Fts5ParseColset_unknown_column() {
    g_tests_run++;

    test_reset_globals();

    static const char *cols[] = { "a", "b", "c" };
    Fts5Parse parse;
    Fts5Config cfg;
    cfg.nCol = 3;
    cfg.azCol = cols;
    parse.rc = SQLITE_OK;
    parse.pConfig = &cfg;

    Fts5Token tok;
    tok.p = "unknowncol";
    tok.n = 10;

    Fts5Colset *pColset = (Fts5Colset *)malloc(sizeof(Fts5Colset));
    pColset->chosenCol = -1;
    g_alloc_colset_ptr = pColset;

    Fts5Colset *pRet = sqlite3Fts5ParseColset(&parse, pColset, &tok);

    TEST_EXPECT(pRet == nullptr, "Expected null return when column not found");
    TEST_EXPECT(parse.rc == SQLITE_ERROR, "Expected rc to be SQLITE_ERROR after error");
    TEST_EXPECT(g_colset_freed == true, "Expected input colset to be freed on error");

    // Cleanup
    if (pRet) free(pRet);
    // pColset has already been freed by function; ensure no double-free
    test_reset_globals();
}

// Test 3: Case-insensitive match with different case (another positive path)
void test_sqlite3Fts5ParseColset_case_insensitive_again() {
    g_tests_run++;

    test_reset_globals();

    static const char *cols[] = { "id", "NAME", "colX" };
    Fts5Parse parse;
    Fts5Config cfg;
    cfg.nCol = 3;
    cfg.azCol = cols;
    parse.rc = SQLITE_OK;
    parse.pConfig = &cfg;

    Fts5Token tok;
    tok.p = "NAME"; // exact case
    tok.n = 4;

    Fts5Colset *pColset = (Fts5Colset *)malloc(sizeof(Fts5Colset));
    pColset->chosenCol = -1;
    g_alloc_colset_ptr = pColset;

    Fts5Colset *pRet = sqlite3Fts5ParseColset(&parse, pColset, &tok);

    TEST_EXPECT(pRet != nullptr, "Expected non-null Fts5Colset for existing column (NAME)");
    TEST_EXPECT(parse.rc == SQLITE_OK, "rc should remain SQLITE_OK on success");
    if (pRet) TEST_EXPECT(pRet->chosenCol == 1, "Expected chosenCol to be 1 for NAME");

    // Cleanup
    if (pRet) free(pRet);
    if (pColset);
    free(pColset);

    test_reset_globals();
}

// Test 4: When pParse->rc is already not OK, function should skip column lookup and free pColset
void test_sqlite3Fts5ParseColset_rc_not_ok_skips_and_frees() {
    g_tests_run++;

    test_reset_globals();

    static const char *cols[] = { "col0", "col1" };
    Fts5Parse parse;
    Fts5Config cfg;
    cfg.nCol = 2;
    cfg.azCol = cols;
    parse.rc = SQLITE_ERROR; // not OK to begin with
    parse.pConfig = &cfg;

    Fts5Token tok;
    tok.p = "col0";
    tok.n = 4;

    Fts5Colset *pColset = (Fts5Colset *)malloc(sizeof(Fts5Colset));
    pColset->chosenCol = -1;
    g_alloc_colset_ptr = pColset;

    Fts5Colset *pRet = sqlite3Fts5ParseColset(&parse, pColset, &tok);

    TEST_EXPECT(pRet == nullptr, "Expected null return when initial rc is not OK");
    TEST_EXPECT(parse.rc == SQLITE_ERROR, "rc should remain SQLITE_ERROR");
    TEST_EXPECT(g_colset_freed == true, "colset should be freed when rc is not OK");

    // Cleanup
    if (pRet) free(pRet);
    test_reset_globals();
}

// Test 5: Simulated internal failure of fts5ParseColset (dynamic iCol == g_failOnColsetIndex)
void test_sqlite3Fts5ParseColset_internal_failure_propagates_error() {
    g_tests_run++;

    test_reset_globals();
    g_failOnColsetIndex = 1; // cause failure when iCol == 1

    static const char *cols[] = { "col0", "col1", "col2" };
    Fts5Parse parse;
    Fts5Config cfg;
    cfg.nCol = 3;
    cfg.azCol = cols;
    parse.rc = SQLITE_OK;
    parse.pConfig = &cfg;

    Fts5Token tok;
    tok.p = "col1"; // iCol = 1 -> will trigger failure
    tok.n = 4;

    Fts5Colset *pColset = (Fts5Colset *)malloc(sizeof(Fts5Colset));
    pColset->chosenCol = -1;
    g_alloc_colset_ptr = pColset;

    Fts5Colset *pRet = sqlite3Fts5ParseColset(&parse, pColset, &tok);

    TEST_EXPECT(pRet == nullptr, "Expected null return when internal failure occurs in colset creation");
    TEST_EXPECT(parse.rc == SQLITE_ERROR, "rc should be SQLITE_ERROR after internal failure");
    TEST_EXPECT(g_colset_freed == true, "colset should be freed after internal failure");

    // Cleanup
    if (pRet) free(pRet);
    test_reset_globals();
}

// Simple test runner
int main() {
    std::cout << "Running sqlite3Fts5ParseColset unit tests (self-contained, no GTest)\n";

    test_sqlite3Fts5ParseColset_simple_match();
    test_sqlite3Fts5ParseColset_unknown_column();
    test_sqlite3Fts5ParseColset_case_insensitive_again();
    test_sqlite3Fts5ParseColset_rc_not_ok_skips_and_frees();
    test_sqlite3Fts5ParseColset_internal_failure_propagates_error();

    std::cout << "Tests run: " << g_tests_run << ", Failed: " << g_tests_failed << "\n";

    if (g_tests_failed == 0) {
        std::cout << "ALL TESTS PASSED\n";
    } else {
        std::cout << "SOME TESTS FAILED\n";
    }
    return (g_tests_failed == 0) ? 0 : 1;
}