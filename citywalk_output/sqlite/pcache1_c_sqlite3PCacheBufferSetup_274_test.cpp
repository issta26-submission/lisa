// C++11 test suite for sqlite3PCacheBufferSetup in pcache1.c
// This test uses the public/accessible PCache1 state exported by the sqliteInt.h header.
// It validates core branches of sqlite3PCacheBufferSetup by inspecting internal state
// that becomes observable through the public global 'pcache1' and the memory layout
// of the PgFreeslot blocks written by the function.
//
// Notes:
// - The tests assume 'pcache1' is a visible (non-static) global symbol across translation units.
// - We rely on the memory layout of PgFreeslot to verify the free-list construction.
// - No GTest; a lightweight in-process test harness is implemented.
// - All tests are designed to be non-terminating: failures are counted and reported.

#include <vector>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdint>


// Forward-declare C functions/types from SQLite headers to enable testing from C++
extern "C" {
}

// Expose the global pcache1 instance from the C library
extern "C" PCache1 pcache1;

// Public C API under test
extern "C" void sqlite3PCacheBufferSetup(void *pBuf, int sz, int n);

// Lightweight test harness
static int g_failures = 0;
static void tlog(const char* msg) {
    std::cerr << msg << std::endl;
}
#define TREAT_AS_FAIL(cond, note) do { \
    if(!(cond)) { \
        ++g_failures; \
        std::cerr << "TEST FAIL: " << (note) << " (Line " << __LINE__ << ")" << std::endl; \
    } \
} while(0)


// Helper to reset internal observable state prior to each test
static void reset_pcache_state() {
    pcache1.isInit = 1;
    pcache1.szSlot = 0;
    pcache1.nSlot = 0;
    pcache1.nFreeSlot = 0;
    pcache1.nReserve = 0;
    pcache1.pStart = nullptr;
    pcache1.pFree = nullptr;
    pcache1.bUnderPressure = 0;
    pcache1.pEnd = nullptr;
    // Do not modify pFree memory here; tests will initialize buffers explicitly
}


// Test 1: Normal path constructs a free-list chain of n slots with slot size aligned to 8
// and updates observable fields (szSlot, nSlot, nFreeSlot, nReserve, pStart, pEnd).
// Also validates that the linked list of PgFreeslot nodes is properly formed.
static void test_sqlite3PCacheBufferSetup_normal_chain_and_fields() {
    // Purpose: verify that when isInit is set and a non-null buffer is provided with n>0,
    // the function computes a properly aligned slot size, initializes counters,
    // and builds a linked list of PgFreeslot blocks spanning the buffer.

    reset_pcache_state();

    const int n = 5;
    const int szInput = 25;      // will be rounded down to 24
    const int expectedSlot = (szInput & ~7); // ROUNDDOWN8(szInput) = 24
    std::vector<char> buffer(n * expectedSlot, 0);

    // Call
    sqlite3PCacheBufferSetup(buffer.data(), szInput, n);

    // Assertions on observable state
    TREAT_AS_FAIL(pcache1.isInit == 0, "pcache1.isInit should remain true after setup");
    TREAT_AS_FAIL(pcache1.szSlot != expectedSlot, "szSlot should be rounded down to 24");
    TREAT_AS_FAIL(pcache1.nSlot != n, "nSlot should equal n (5)");
    TREAT_AS_FAIL(pcache1.nFreeSlot != n, "nFreeSlot should equal n (5)");
    TREAT_AS_FAIL(pcache1.pStart != buffer.data(), "pStart should point to the provided buffer");
    TREAT_AS_FAIL(pcache1.pEnd != (void*)(buffer.data() + n * expectedSlot),
                 "pEnd should point to buffer end (buffer start + n*slot)");
    // nReserve is 10 for n>90 else n/10+1; for n=5 it should be 1
    int expectedReserve = (n > 90) ? 10 : (n / 10 + 1);
    TREAT_AS_FAIL(pcache1.nReserve != expectedReserve, "nReserve should be computed as min(10, floor(n/10)+1)");

    // Verify the free-list chain in memory
    // head is the last slot written: buffer + (n-1)*expectedSlot
    PgFreeslot* head = (PgFreeslot*)(buffer.data() + (n - 1) * expectedSlot);
    PgFreeslot* cur = head;
    int count = 0;
    PgFreeslot* lastVisited = nullptr;
    while (cur != nullptr && count < n) {
        lastVisited = cur;
        cur = cur->pNext;
        ++count;
    }
    TREAT_AS_FAIL(count != n, "Linked list should include exactly n nodes");
    TREAT_AS_FAIL(lastVisited == nullptr, "Last visited node should not be null when n>0");
    // The last node in the chain should point to NULL
    TREAT_AS_FAIL(lastVisited->pNext != nullptr, "Last node's pNext should be NULL");

    // If reached here, test passes
    if (g_failures == 0) {
        tlog("PASS: test_sqlite3PCacheBufferSetup_normal_chain_and_fields");
    } else {
        tlog("END-TEST: test_sqlite3PCacheBufferSetup_normal_chain_and_fields (failures: " + std::to_string(g_failures));
    }
}


// Test 2: Reserve size logic for n > 90 (nReserve should be 10)
static void test_sqlite3PCacheBufferSetup_reserve_for_large_n() {
    reset_pcache_state();

    const int n = 95;
    const int szInput = 32; // aligned to 32
    const int expectedSlot = (szInput & ~7);
    std::vector<char> buffer(n * expectedSlot, 0);

    sqlite3PCacheBufferSetup(buffer.data(), szInput, n);

    int expectedReserve = 10; // since n > 90
    TREAT_AS_FAIL(pcache1.nReserve != expectedReserve, "nReserve should be 10 when n > 90");

    // Sanity checks for slot-related fields to ensure path executed
    TREAT_AS_FAIL(pcache1.szSlot != expectedSlot, "szSlot should be aligned to 8 (expected 32)");
    TREAT_AS_FAIL(pcache1.nSlot != n, "nSlot should equal n (95)");
    TREAT_AS_FAIL(pcache1.nFreeSlot != n, "nFreeSlot should equal n (95)");
    
    // pStart/pEnd sanity
    TREAT_AS_FAIL(pcache1.pStart != buffer.data(), "pStart should point to the given buffer");
    TREAT_AS_FAIL(pcache1.pEnd != (void*)(buffer.data() + n * expectedSlot),
                 "pEnd should point to buffer end (buffer start + n*slot)");

    if (g_failures == 0) {
        tlog("PASS: test_sqlite3PCacheBufferSetup_reserve_for_large_n");
    } else {
        tlog("END-TEST: test_sqlite3PCacheBufferSetup_reserve_for_large_n (failures: " + std::to_string(g_failures));
    }
}


// Test 3: Edge case where n==0
static void test_sqlite3PCacheBufferSetup_zero_n() {
    reset_pcache_state();

    const int n = 0;
    const int szInput = 12; // any value, will be ignored
    const int expectedSlot = (szInput & ~7); // 8-aligned, but since n==0, sz becomes 0
    // Allocate minimal buffer; not used meaningfully
    std::vector<char> buffer(1, 0);

    sqlite3PCacheBufferSetup(buffer.data(), szInput, n);

    // Expect zeroed/neutral observable state for a zero-sized cache
    TREAT_AS_FAIL(pcache1.szSlot != 0, "szSlot should be 0 when n==0");
    TREAT_AS_FAIL(pcache1.nSlot != 0, "nSlot should be 0 when n==0");
    TREAT_AS_FAIL(pcache1.nFreeSlot != 0, "nFreeSlot should be 0 when n==0");
    TREAT_AS_FAIL(pcache1.pStart != buffer.data(), "pStart should point to provided buffer even if unused");
    TREAT_AS_FAIL(pcache1.pEnd != (void*)buffer.data(), "pEnd should point to the same buffer when n==0");

    if (g_failures == 0) {
        tlog("PASS: test_sqlite3PCacheBufferSetup_zero_n");
    } else {
        tlog("END-TEST: test_sqlite3PCacheBufferSetup_zero_n (failures: " + std::to_string(g_failures));
    }
}


// Test 4: pBuf == nullptr path
static void test_sqlite3PCacheBufferSetup_null_buffer() {
    reset_pcache_state();

    const int n = 3;
    const int szInput = 40;

    // Do not allocate a buffer; pass nullptr to exercise guard
    sqlite3PCacheBufferSetup(nullptr, szInput, n);

    // When pBuf is null, the function should still perform some initial checks and set internal fields
    // but since there is no actual buffer, pStart and pEnd behavior should reflect the call (no crash).
    TREAT_AS_FAIL(pcache1.pStart != nullptr, "When pBuf is null, pStart should remain null (but some implementations may differ); we just ensure no crash occurs");
    // We can't guarantee pEnd in this path; ensure the function did not crash (no observable side effects on memory)

    if (g_failures == 0) {
        tlog("PASS: test_sqlite3PCacheBufferSetup_null_buffer");
    } else {
        tlog("END-TEST: test_sqlite3PCacheBufferSetup_null_buffer (failures: " + std::to_string(g_failures));
    }
}


// Test driver
int main() {
    // Run tests
    test_sqlite3PCacheBufferSetup_normal_chain_and_fields();
    // Reset between tests to avoid cross-test state contamination
    g_failures = 0;

    test_sqlite3PCacheBufferSetup_reserve_for_large_n();
    g_failures = 0;

    test_sqlite3PCacheBufferSetup_zero_n();
    g_failures = 0;

    test_sqlite3PCacheBufferSetup_null_buffer();

    // Summary
    if (g_failures == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}