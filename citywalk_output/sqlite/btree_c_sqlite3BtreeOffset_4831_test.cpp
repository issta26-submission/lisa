// BTree Offset Unit Tests
// Focus: sqlite3BtreeOffset(BtCursor *pCur)
// Notes:
// - This test suite targets the focal method described in <FOCAL_METHOD>
// - It relies on the project-provided internal types (BtCursor, MemPage, BtShared, etc.)
// - The tests exercise the computed offset logic under valid-cursor conditions
// - Due to asserts in the production code, these tests are written to run with asserts enabled.
// - Build with linking to the real btree.c and its headers (e.g., -I<include_dir> and link with btree.o/btree.c).
// - In environments where asserts must be avoided for test safety, compile with -DNDEBUG.
// - These tests focus on functional correctness of the return value, not on private/static internals.

#include <btreeInt.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Bring in the actual definitions from the project (BtCursor, MemPage, BtShared, etc.)
// The header btreeInt.h is expected to declare the public/internal types used by sqlite3BtreeOffset.
extern "C" {
}

// Type alias used by the focal method
// (Typically defined in the project headers; keep for clarity in tests)
typedef long long i64;

// Helper to print test results
static void print_result(const char* test_name, bool passed) {
    if (passed) {
        std::cout << "[PASS] " << test_name << std::endl;
    } else {
        std::cout << "[FAIL] " << test_name << std::endl;
    }
}

// Test 1: Basic correctness with a valid cursor
// Intent: Ensure that sqlite3BtreeOffset computes the expected offset:
//   offset = pageSize * (pgno - 1) + (payloadPtr - pageData)
static bool test_sqlite3BtreeOffset_basic() {
    // Setup BtShared (pCur->pBt) with a known pageSize
    BtShared btShared;
    // The actual type of pageSize in the project is likely a specific width (e.g., u16 or int).
    // We rely on the project's header to define it correctly.
    btShared.pageSize = 4096; // typical SQLite page size

    // Setup MemPage (pCur->pPage) with pgno and aData
    MemPage memPage;
    memPage.pgno = 3;
    unsigned char pageData[256];
    memPage.aData = pageData;

    // Setup BtCursor (pCur) with proper references
    BtCursor cur;
    cur.pBt = &btShared;
    cur.pPage = &memPage;

    // info.pPayload must be a pointer inside memPage.aData
    cur.info.pPayload = memPage.aData + 128; // offset 128 within page data

    // Cursor state must be CURSOR_VALID to satisfy the assertion in sqlite3BtreeOffset
    cur.eState = CURSOR_VALID;

    // Call the focal method
    i64 result = sqlite3BtreeOffset(&cur);

    // Expected computation
    i64 expected = (i64)btShared.pageSize * ((i64)memPage.pgno - 1) +
                   (i64)(cur.info.pPayload - memPage.aData);

    bool ok = (result == expected);
    return ok;
}

// Test 2: Another basic scenario with a different page, payload offset, to confirm arithmetic correctness
static bool test_sqlite3BtreeOffset_another_case() {
    BtShared btShared;
    btShared.pageSize = 1024;

    MemPage memPage;
    memPage.pgno = 5;
    unsigned char pageData[512];
    memPage.aData = pageData;

    BtCursor cur;
    cur.pBt = &btShared;
    cur.pPage = &memPage;

    // Place payload at a different offset
    cur.info.pPayload = memPage.aData + 0; // payload at start of page

    cur.eState = CURSOR_VALID;

    i64 result = sqlite3BtreeOffset(&cur);
    i64 expected = (i64)btShared.pageSize * ((i64)memPage.pgno - 1) +
                   (i64)(cur.info.pPayload - memPage.aData);

    return (result == expected);
}

// Test 3: Edge case where pgno is 1 and payload is at the very start (offset 0)
static bool test_sqlite3BtreeOffset_firstPageZeroPayload() {
    BtShared btShared;
    btShared.pageSize = 2048;

    MemPage memPage;
    memPage.pgno = 1;
    unsigned char pageData[128];
    memPage.aData = pageData;

    BtCursor cur;
    cur.pBt = &btShared;
    cur.pPage = &memPage;

    cur.info.pPayload = memPage.aData + 0;
    cur.eState = CURSOR_VALID;

    i64 result = sqlite3BtreeOffset(&cur);
    i64 expected = (i64)btShared.pageSize * ((i64)memPage.pgno - 1) +
                   (i64)(cur.info.pPayload - memPage.aData);

    return (result == expected);
}

// Main: Run all tests and summarize results
int main() {
    // Note: In production, build with -DNDEBUG to bypass runtime asserts for non-assert-safe test runs.
    // Here we assume a test environment where the production asserts are either satisfied (valid cursor)
    // or compiled out for the purpose of arithmetic verification.

    int total = 0;
    int passed = 0;

    total++; {
        bool r = test_sqlite3BtreeOffset_basic();
        print_result("sqlite3BtreeOffset_basic", r);
        if (r) ++passed; else;
    }

    total++; {
        bool r = test_sqlite3BtreeOffset_another_case();
        print_result("sqlite3BtreeOffset_another_case", r);
        if (r) ++passed; else;
    }

    total++; {
        bool r = test_sqlite3BtreeOffset_firstPageZeroPayload();
        print_result("sqlite3BtreeOffset_firstPageZeroPayload", r);
        if (r) ++passed; else;
    }

    std::cout << "Tests run: " << total << ", Passed: " << passed << std::endl;

    // Return non-zero if any test failed
    return (passed == total) ? 0 : 1;
}

// Explanatory notes for maintainers:
// - The tests instantiate concrete BtShared, MemPage, and BtCursor objects as required by sqlite3BtreeOffset.
// - The tests rely on the actual structure definitions from btreeInt.h and related headers.
// - To maximize coverage, tests validate the arithmetic outcome of the offset computation for multiple scenarios.
// - The true/false branches of the asserts in sqlite3BtreeOffset cannot be exercised directly without customizing
//   the assert behavior (e.g., by compiling with NDEBUG to disable asserts). The tests therefore focus on
//   verifying the computed offset under valid-cursor conditions and basic edge-case arithmetic.