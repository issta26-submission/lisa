// Lightweight C++11 test suite for sqlite3_str_append (no GoogleTest used)

#include <cstdio>
#include <cstring>
#include <sqliteInt.h>
#include <cstdlib>
#include <cassert>


// Minimal stand-in for the real sqlite3_str structure used by sqlite3_str_append
struct sqlite3_str {
    char *zText;     // pointer to allocated text buffer
    int nChar;       // number of characters currently used in zText
    int nAlloc;      // total allocated size for zText (excluding possible null terminator)
    int mxAlloc;     // maximum allowed allocation (not used in tests beyond presence)
    int accError;    // non-zero if there is an accumulated error
};

// Forward declaration of the helper that the focal method would call in the enlarge/append path
static void enlargeAndAppend(sqlite3_str *p, const char *z, int N);

// The focal method under test (reproduced for testing in this standalone harness)
void sqlite3_str_append(sqlite3_str *p, const char *z, int N){
    assert( z!=nullptr || N==0 );
    assert( p->zText!=nullptr || p->nChar==0 || p->accError );
    assert( N>=0 );
    assert( p->accError==0 || p->nAlloc==0 || p->mxAlloc==0 );
    if( p->nChar+N >= p->nAlloc ){
        enlargeAndAppend(p,z,N);
    }else if( N ){
        assert( p->zText );
        p->nChar += N;
        memcpy(&p->zText[p->nChar-N], z, N);
        // Keep a terminating zero for safer string handling in tests
        if (p->zText) p->zText[p->nChar] = '\0';
    }
}

// Helper: simulate dynamic enlargement behavior (simple, deterministic)
static void enlargeAndAppend(sqlite3_str *p, const char *z, int N){
    if (N <= 0) return;
    int needed = p->nChar + N;
    int newAlloc;
    if (p->nAlloc <= 0) {
        newAlloc = needed + 1;
    } else {
        // Simple growth strategy: double or ensure enough room
        newAlloc = p->nAlloc * 2;
        if (newAlloc < needed + 1) newAlloc = needed + 1;
    }

    char *newBuf = (char*)realloc(p->zText, newAlloc);
    if (!newBuf) {
        // Simulate error handling: mark as error state
        p->accError = 1;
        return;
    }
    p->zText = newBuf;
    p->nAlloc = newAlloc;

    // Copy the new segment to the end of the existing buffer
    memcpy(p->zText + p->nChar, z, N);
    p->nChar += N;
    // Maintain a terminator for testing convenience
    p->zText[p->nChar] = '\0';
}

// Lightweight test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_PASS() do { ++g_tests_run; } while(0)
#define TEST_FAIL(msg) do { ++g_tests_run; ++g_tests_failed; fprintf(stderr, "Test failed: %s\n", msg); } while(0)

static void run_all_tests();

// Test 1: append smaller segment without triggering enlargement
static bool test_append_no_enlarge()
{
    // Candidate Keywords: zText, nChar, nAlloc, accError, N, z
    sqlite3_str p;
    // Initial buffer: capacity 5, content "X"
    p.zText = (char*)malloc(6);
    strcpy(p.zText, "X"); // ensure initial content
    p.nChar = 1;
    p.nAlloc = 5;
    p.mxAlloc = 0;
    p.accError = 0;

    const char *z = "AB"; // N = 2
    int N = 2;

    sqlite3_str_append(&p, z, N);

    // Expected: buffer becomes "XAB" with length 3 and terminator
    bool ok = true;
    if (p.nChar != 3) {
        ok = false;
    }
    if (strncmp(p.zText, "XAB", 3) != 0) {
        ok = false;
    }
    if (p.zText[3] != '\0') {
        ok = false;
    }

    free(p.zText);
    return ok;
}

// Test 2: append triggers enlargement path; ensure content and lengths updated
static bool test_append_with_enlarge()
{
    // Candidate Keywords: enlargeAndAppend, nAlloc, nChar, zText, N, z
    sqlite3_str p;
    // Initial buffer: capacity 5, content "XYZ"
    p.zText = (char*)malloc(6);
    strcpy(p.zText, "XYZ"); // initial content length 3
    p.nChar = 3;
    p.nAlloc = 5;
    p.mxAlloc = 0;
    p.accError = 0;

    const char *z = "DEF"; // N = 3
    int N = 3;

    sqlite3_str_append(&p, z, N);

    // Expected: content "XYZDEF" and nAlloc expanded, nChar updated
    bool ok = true;
    if (p.nChar != 6) ok = false;
    if (p.nAlloc < 6 + 1) ok = false; // at least enough for terminator
    if (strncmp(p.zText, "XYZDEF", 6) != 0) ok = false;
    if (p.zText[6] != '\0') ok = false;

    free(p.zText);
    return ok;
}

// Test 3: N == 0 path (no changes when there is room)
static bool test_append_zero_N_no_change()
{
    // Candidate Keywords: N==0 path, p->nChar, p->nAlloc
    sqlite3_str p;
    p.zText = (char*)malloc(5);
    strcpy(p.zText, "AB"); // length 2
    p.nChar = 2;
    p.nAlloc = 5;
    p.mxAlloc = 0;
    p.accError = 0;

    const char *z = "XYZ"; // Z content irrelevant; N==0 means no copy
    int N = 0;

    sqlite3_str_append(&p, z, N);

    bool ok = true;
    if (p.nChar != 2) ok = false;
    if (strcmp(p.zText, "AB") != 0) ok = false;
    free(p.zText);
    return ok;
}

static void run_all_tests()
{
    // Run Test 1
    if (test_append_no_enlarge()) {
        TEST_PASS();
    } else {
        TEST_FAIL("test_append_no_enlarge failed");
    }

    // Run Test 2
    if (test_append_with_enlarge()) {
        TEST_PASS();
    } else {
        TEST_FAIL("test_append_with_enlarge failed");
    }

    // Run Test 3
    if (test_append_zero_N_no_change()) {
        TEST_PASS();
    } else {
        TEST_FAIL("test_append_zero_N_no_change failed");
    }
}

int main()
{
    run_all_tests();
    if (g_tests_failed == 0) {
        printf("All tests passed (%d tests).\n", g_tests_run);
        return 0;
    } else {
        printf("Tests completed with %d failures out of %d tests.\n", g_tests_failed, g_tests_run);
        return 1;
    }
}