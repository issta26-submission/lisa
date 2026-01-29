// A small C++11 test harness for the focal function sqlite3Fts3DoclistNext
// This test suite is designed to be compilable without GTest and links against
// the C implementation of the sqlite3 FTS3 doclist code (fts3.c).
// NOTE: We avoid private/internal details and exercise the public behavior of
// sqlite3Fts3DoclistNext through carefully crafted inputs.

#include <fts3Int.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <fts3.h>
#include <assert.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <cstdint>


// Domain-specific: provide minimal C linkage for the C functions used from fts3.c
extern "C" {
    // Typedefs matching the C declaration in fts3.c
    typedef long long sqlite3_int64;
    typedef unsigned char u8;

    // Declaration of the focal function under test (C linkage)
    void sqlite3Fts3DoclistNext(
        int bDescIdx,
        char *aDoclist,
        int nDoclist,
        char **ppIter,
        sqlite3_int64 *piDocid,
        u8 *pbEof
    );
}

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define TEST_ASSERT(cond, msg) \
    do { \
        if(!(cond)) { \
            fprintf(stderr, "Test assertion failed: %s\n", msg); \
            ++g_failed_tests; \
        } \
        ++g_total_tests; \
    } while(0)

// Test 1: Initial iteration when ppIter is NULL (p==0 path).
// - Data carries a single-byte varint encoding of the initial docid (e.g., 5).
// - Expected: docid becomes 5, ppIter advances past the varint, pbEof remains 0.
bool test_doclistNext_initial_call() {
    // Single-byte varint 5 (typical for small values in varint schemes)
    unsigned char data[1] = { 0x05 };
    const int nDoclist = 1;
    char *iter = nullptr;
    sqlite3_int64 docid = 0;
    unsigned char eof = 0;

    // Call with p==0 path (ppIter initially NULL)
    sqlite3Fts3DoclistNext(0, (char*)data, nDoclist, &iter, &docid, &eof);

    bool ok = (iter == (char*)data + 1) && (docid == 5) && (eof == 0);
    if(!ok) {
        fprintf(stderr, "test_doclistNext_initial_call: expected iter=%p docid=5 eof=0 but got iter=%p docid=%lld eof=%u\n",
                (void*)(data + 1), (void*)iter, (long long)docid, (unsigned)eof);
    }
    TEST_ASSERT(ok, "Initial doclistNext should set docid=5 and advance iter by 1 byte");
    return ok;
}

// Test 2: Non-null path (p != 0) with a single varint delta.
// - Data carries a single-byte varint encoding of delta (e.g., 3).
// - Start with docid = 7; bDescIdx = 0 (desc false); After call, docid = 10.
// - ppIter should advance past the varint; pbEof remains 0.
bool test_doclistNext_nonzero_varint_path() {
    unsigned char data[1] = { 0x03 }; // varint delta = 3
    const int nDoclist = 1;
    char *iter = (char*)data;       // p points to the first byte (non-null)
    sqlite3_int64 docid = 7;
    unsigned char eof = 0;

    sqlite3Fts3DoclistNext(0, (char*)data, nDoclist, &iter, &docid, &eof);

    bool ok = (iter == (char*)data + 1) && (docid == 10) && (eof == 0);
    if(!ok) {
        fprintf(stderr, "test_doclistNext_nonzero_varint_path: expected iter=%p docid=10 eof=0 but got iter=%p docid=%lld eof=%u\n",
                (void*)(data + 1), (void*)iter, (long long)docid, (unsigned)eof);
    }
    TEST_ASSERT(ok, "Non-null path should add delta (3) to initial docid (7) resulting in 10");
    return ok;
}

// Test 3: Non-null path with bDescIdx = 1 (negative delta).
// - Data carries a single-byte varint encoding of delta (e.g., 4).
// - Start with docid = 20; bDescIdx = 1 (desc true); After call, docid = 16.
// - ppIter advances; pbEof remains 0.
bool test_doclistNext_descending_path() {
    unsigned char data[1] = { 0x04 }; // varint delta = 4
    const int nDoclist = 1;
    char *iter = (char*)data;
    sqlite3_int64 docid = 20;
    unsigned char eof = 0;

    sqlite3Fts3DoclistNext(1, (char*)data, nDoclist, &iter, &docid, &eof);

    bool ok = (iter == (char*)data + 1) && (docid == 16) && (eof == 0);
    if(!ok) {
        fprintf(stderr, "test_doclistNext_descending_path: expected iter=%p docid=16 eof=0 but got iter=%p docid=%lld eof=%u\n",
                (void*)(data + 1), (void*)iter, (long long)docid, (unsigned)eof);
    }
    TEST_ASSERT(ok, "Descending delta should subtract delta (4) from initial docid (20) => 16");
    return ok;
}

// Test 4: End-of-file path when doclist ends with zeros.
// - Data contains zeros that lead the inner loop to reach end of aDoclist.
// - Start with docid arbitrary (0) and pbEof initially 0; After call, pbEof should be 1.
// - iter should point to end of the document list buffer.
bool test_doclistNext_eof_path() {
    // Three zeros to ensure the while loop advances to end
    unsigned char data[3] = { 0x00, 0x00, 0x00 };
    const int nDoclist = 3;
    char *iter = (char*)data;
    sqlite3_int64 docid = 0;
    unsigned char eof = 0;

    sqlite3Fts3DoclistNext(0, (char*)data, nDoclist, &iter, &docid, &eof);

    bool ok = (iter == (char*)data + 3) && (eof == 1);
    if(!ok) {
        fprintf(stderr, "test_doclistNext_eof_path: expected iter=%p eof=1 but got iter=%p eof=%u\n",
                (void*)(data + 3), (void*)iter, (unsigned)eof);
    }
    TEST_ASSERT(ok, "EOF path should set pbEof=1 when trailing poslist is exhausted");
    return ok;
}

// Test 5: Multiple consecutive calls on a small doclist.
// - First call uses p==0 path (docid becomes 2).
// - Second call uses p non-null path starting at the next byte (docid increases by next delta).
// - This validates stateful iteration via ppIter across calls.
bool test_doclistNext_multiple_calls() {
    // Data encodes two successive single-byte varints: 0x02 then 0x05
    // We'll simulate by performing first call with p==0, then a second call with p pointing inside.
    unsigned char data[2] = { 0x02, 0x05 };
    const int nDoclist = 2;

    // First call: p==0 path
    char *iter = nullptr;
    sqlite3_int64 docid = 0;
    unsigned char eof = 0;

    sqlite3Fts3DoclistNext(0, (char*)data, nDoclist, &iter, &docid, &eof);
    bool firstOk = (iter == (char*)data + 1) && (docid == 2) && (eof == 0);
    if(!firstOk) {
        fprintf(stderr, "test_doclistNext_multiple_calls (1st): expected iter=%p docid=2 eof=0 but got iter=%p docid=%lld eof=%u\n",
                (void*)(data + 1), (void*)iter, (long long)docid, (unsigned)eof);
        TEST_ASSERT(firstOk, "First call should yield docid=2 and advance iter by 1 byte");
        return false;
    }

    // Second call: p points inside the data to the second byte (0x05)
    iter = (char*)data + 1;
    docid = 2; // carry over from first docid
    eof = 0;

    sqlite3Fts3DoclistNext(0, (char*)data, nDoclist, &iter, &docid, &eof);
    bool secondOk = (iter == (char*)data + 2) && (docid == 7) && (eof == 0);
    if(!secondOk) {
        fprintf(stderr, "test_doclistNext_multiple_calls (2nd): expected iter=%p docid=7 eof=0 but got iter=%p docid=%lld eof=%u\n",
                (void*)(data + 2), (void*)iter, (long long)docid, (unsigned)eof);
    }
    TEST_ASSERT(secondOk, "Second call should add delta 5 to previous docid 2 => 7");
    return firstOk && secondOk;
}

int main() {
    // Run tests
    bool t1 = test_doclistNext_initial_call();
    bool t2 = test_doclistNext_nonzero_varint_path();
    bool t3 = test_doclistNext_descending_path();
    bool t4 = test_doclistNext_eof_path();
    bool t5 = test_doclistNext_multiple_calls();

    // Summary
    if(g_failed_tests == 0) {
        printf("ALL TESTS PASSED (%d tests).\n", g_total_tests);
        return 0;
    } else {
        printf("%d/%d TESTS FAILED.\n", g_failed_tests, g_total_tests);
        return 1;
    }
}