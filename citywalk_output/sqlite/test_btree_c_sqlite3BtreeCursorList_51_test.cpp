// Test suite for sqlite3BtreeCursorList
// - No Google Test; a lightweight in-process test harness.
// - Uses actual project headers to build realistic scenarios.
// - SQLITE_DEBUG is enabled to exercise the debug printing path.
// - We provide a mock sqlite3DebugPrintf to capture output for assertions.

#define SQLITE_DEBUG 1

#include <vector>
#include <cstdio>
#include <btreeInt.h>
#include <cstdarg>
#include <cstring>
#include <string>
#include <cstdlib>


// Include the sqlite internal b-tree definitions.
// These definitions are expected to be compatible with the production code.
// We wrap the C header in extern "C" to avoid name mangling for C++.
extern "C" {

// Forward declare the function under test with C linkage.
void sqlite3BtreeCursorList(Btree *p);

// We provide a simple, test-friendly replacement for sqlite3DebugPrintf.
// The real sqlite code uses this in the SQLITE_DEBUG path; we capture its output to a string.
void sqlite3DebugPrintf(const char *zFormat, ...);
}

// Global test log to capture debug output produced by sqlite3DebugPrintf
static std::string g_debugLog;

// Variadic wrapper to capture output from sqlite3DebugPrintf
void sqlite3DebugPrintf(const char *zFormat, ...) {
    // Simple, safe vsnprintf-based capture
    char buffer[1024];
    va_list ap;
    va_start(ap, zFormat);
    vsnprintf(buffer, sizeof(buffer), zFormat, ap);
    va_end(ap);
    g_debugLog.append(buffer);
}

// Small test harness helpers
static void resetLog() { g_debugLog.clear(); }

// Simple assertion macro that does not terminate the program (non-terminating).
#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        fprintf(stderr, "ASSERT FAIL: %s\n", msg); \
        return false; \
    } \
} while(0)

static int countSubstring(const std::string &haystack, const std::string &needle) {
    if(needle.empty()) return 0;
    int count = 0;
    size_t pos = 0;
    while((pos = haystack.find(needle, pos)) != std::string::npos) {
        ++count;
        pos += needle.length();
    }
    return count;
}

// Test 1: Validate that sqlite3BtreeCursorList prints three cursors with correct mode and eof handling.
//         - Cursor 1: ro, non-null page, valid state
//         - Cursor 2: rw, null page, eof state
//         - Cursor 3: ro, non-null page, valid state
static bool test_cursor_list_basic_output() {
    // Prepare a Btree with a shared BtShared and a linked list of three BtCursor objects.

    // Create BtShared (pBt)
    BtShared *pBt = new BtShared();
    // Create Btree (p) and hook up pBt
    Btree *p = new Btree();
    p->pBt = pBt;

    // Build three BtCursor objects
    BtCursor *c1 = new BtCursor();
    BtCursor *c2 = new BtCursor();
    BtCursor *c3 = new BtCursor();

    // Link them: c1 -> c2 -> c3
    c1->pNext = c2;
    c2->pNext = c3;
    c3->pNext = nullptr;

    // Page 1: ro, page with pgno 42
    c1->curFlags = 0;                 // not BTCF_WriteFlag
    c1->iPage = 0;
    c1->pgnoRoot = 123;
    c1->eState = CURSOR_VALID;
    c1->apPage = new MemPage*[1];
    c1->apPage[0] = new MemPage();
    c1->apPage[0]->pgno = 42;         // non-null page
    c1->aiIdx = new int[1];
    c1->aiIdx[0] = 7;

    // Page 2: rw, null page, eof state
    c2->curFlags = BTCF_WriteFlag;
    c2->iPage = 0;
    c2->pgnoRoot = 0;
    c2->eState = CURSOR_INVALID; // represents non-VALID
    c2->apPage = new MemPage*[1];
    c2->apPage[0] = nullptr;        // null page

    c2->aiIdx = new int[1];
    c2->aiIdx[0] = 2;

    // Page 3: ro, non-null page
    c3->curFlags = 0;
    c3->iPage = 0;
    c3->pgnoRoot = 999;
    c3->eState = CURSOR_VALID;
    c3->apPage = new MemPage*[1];
    c3->apPage[0] = new MemPage();
    c3->apPage[0]->pgno = 77;
    c3->aiIdx = new int[1];
    c3->aiIdx[0] = 3;

    // Attach to BtShared: pCursor
    pBt->pCursor = c1;

    // Reset log and invoke the function under test
    resetLog();
    sqlite3BtreeCursorList(p);

    // Analyze the log to ensure three cursors were printed, with correct mode and eof handling.
    // We rely on substring checks to validate modes and eof flags and a line count approach.
    std::vector<std::string> lines;
    { // split log by newline
        std::string acc;
        for(char ch : g_debugLog) {
            if(ch == '\n') {
                if(!acc.empty()) lines.push_back(acc);
                acc.clear();
            } else {
                acc.push_back(ch);
            }
        }
        if(!acc.empty()) lines.push_back(acc);
    }

    // Expect three "CURSOR" lines
    int cursorLines = 0;
    int roCount = 0;
    int rwCount = 0;
    int eofCount = 0;
    for(const auto &line : lines) {
        if(line.find("CURSOR") != std::string::npos) {
            ++cursorLines;
            if(line.find("(ro)") != std::string::npos) ++roCount;
            if(line.find("(rw)") != std::string::npos) ++rwCount;
            if(line.find(" eof") != std::string::npos) ++eofCount;
        }
    }

    TEST_ASSERT(cursorLines == 3, "Expected 3 CURSOR lines in debug output");
    TEST_ASSERT(roCount >= 1, "Expected at least one ro cursor");
    TEST_ASSERT(rwCount >= 1, "Expected at least one rw cursor");
    TEST_ASSERT(eofCount >= 1, "Expected at least one eof indicator");

    // Clean up allocations
    delete c1->aiIdx;
    delete c1->apPage[0];
    delete[] c1->apPage;
    delete c1;

    delete c2->aiIdx;
    delete[] c2->apPage;
    delete c2; // apPage[0] is null, safe to delete the array object
    // Note: c2->apPage[0] is nullptr; no delete for that
    // We already deleted c2 itself.

    delete c3->aiIdx;
    delete c3->apPage[0];
    delete[] c3->apPage;
    delete c3;

    // Clean up BtShared and Btree
    delete p;
    delete pBt;

    // Also reset log for next tests
    resetLog();

    return true;
}

// Test 2: When there are no cursors (pCursor == NULL), the function should produce no output.
static bool test_cursor_list_no_cursor() {
    BtShared *pBt = new BtShared();
    Btree *p = new Btree();
    p->pBt = pBt;
    pBt->pCursor = nullptr; // no cursors

    resetLog();
    sqlite3BtreeCursorList(p);

    // Expect no debug output
    if(!g_debugLog.empty()) {
        fprintf(stderr, "ASSERT FAIL: Expected no output when no cursors exist.\n");
        delete p;
        delete pBt;
        return false;
    }

    delete p;
    delete pBt;
    resetLog();
    return true;
}

// Test 3: Ensure the function handles mixed eState values (one valid, one eof) and page presence.
// This test reuses a scenario with two cursors to stress the eof and page-non-null branches.
static bool test_cursor_list_eof_and_pages_variant() {
    BtShared *pBt = new BtShared();
    Btree *p = new Btree();
    p->pBt = pBt;

    // Cursor A: ro, valid page
    BtCursor *a = new BtCursor();
    a->pNext = nullptr;
    a->curFlags = 0;
    a->iPage = 0;
    a->pgnoRoot = 11;
    a->eState = CURSOR_VALID;
    a->apPage = new MemPage*[1];
    a->apPage[0] = new MemPage();
    a->apPage[0]->pgno = 101;
    a->aiIdx = new int[1];
    a->aiIdx[0] = 1;

    // Cursor B: rw, page present but eof
    BtCursor *b = new BtCursor();
    b->pNext = nullptr;
    a->pNext = b;
    b->curFlags = BTCF_WriteFlag;
    b->iPage = 0;
    b->pgnoRoot = 0;
    b->eState = CURSOR_INVALID;
    b->apPage = new MemPage*[1];
    b->apPage[0] = new MemPage();
    b->apPage[0]->pgno = 202;
    b->aiIdx = new int[1];
    b->aiIdx[0] = 2;

    // Attach to BtShared
    pBt->pCursor = a;

    resetLog();
    sqlite3BtreeCursorList(p);

    // Basic assertions on debug output
    int cursorLines = countSubstring(g_debugLog, "CURSOR");
    int hasRo = (g_debugLog.find("(ro)") != std::string::npos) ? 1 : 0;
    int hasRw = (g_debugLog.find("(rw)") != std::string::npos) ? 1 : 0;
    int hasEof = (g_debugLog.find(" eof") != std::string::npos) ? 1 : 0;

    TEST_ASSERT(cursorLines >= 2, "Expected at least 2 CURSOR lines");
    TEST_ASSERT(hasRo, "Expected at least one ro line");
    TEST_ASSERT(hasRw, "Expected at least one rw line");
    TEST_ASSERT(hasEof, "Expected an eof indicator line");

    // Cleanup allocations
    delete a->aiIdx;
    delete a->apPage[0];
    delete[] a->apPage;
    delete a;

    delete b->aiIdx;
    delete b->apPage[0];
    delete[] b->apPage;
    delete b;

    delete p;
    delete pBt;
    resetLog();

    return true;
}

int main() {
    int allPassed = 1;

    printf("Starting sqlite3BtreeCursorList test suite (C++11, no GTest, output captured)\n");

    if(!test_cursor_list_basic_output()) {
        printf("Test 1 (cursor list basic output) FAILED\n");
        allPassed = 0;
    } else {
        printf("Test 1 (cursor list basic output) PASSED\n");
    }

    if(!test_cursor_list_no_cursor()) {
        printf("Test 2 (no cursor case) FAILED\n");
        allPassed = 0;
    } else {
        printf("Test 2 (no cursor case) PASSED\n");
    }

    if(!test_cursor_list_eof_and_pages_variant()) {
        printf("Test 3 (eof and pages variant) FAILED\n");
        allPassed = 0;
    } else {
        printf("Test 3 (eof and pages variant) PASSED\n");
    }

    if(allPassed) {
        printf("All tests PASSED\n");
        return 0;
    } else {
        printf("Some tests FAILED\n");
        return 1;
    }
}