// Test suite for sqlite3Fts5HashClear
// This test suite is written in C++11 without Google Test.
// It constructs a minimal in-memory representation of Fts5Hash and Fts5HashEntry,
// then exercises sqlite3Fts5HashClear to verify that all hash entries are freed,
// the aSlot array is zeroed, and nEntry is reset to 0.
// To observe deallocation, we provide a custom sqlite3_free implementation to track frees
// (overriding the SQLite allocator if linked in). This allows non-terminating checks.
// The tests do not rely on private/internal methods beyond the structure layout used by sqlite3Fts5HashClear.

#include <vector>
#include <fts5Int.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// ---------------------------------------------------------------------------
// Minimal type definitions that mirror the usage inside sqlite3Fts5HashClear
// These definitions are kept intentionally small and compatible with the
// expectations of the focal function (nSlot, aSlot, nEntry, and pHashNext).
// ---------------------------------------------------------------------------

extern "C" {

// Forward declaration of the focal function (from fts5_hash.c)
void sqlite3Fts5HashClear(struct Fts5Hash *pHash);

// Mock of the static helper types used by sqlite3Fts5HashClear
typedef struct Fts5HashEntry {
    struct Fts5HashEntry *pHashNext;
} Fts5HashEntry;

typedef struct Fts5Hash {
    int nSlot;
    Fts5HashEntry **aSlot;
    int nEntry;
} Fts5Hash;

}

// ---------------------------------------------------------------------------
// Allocation tracking via sqlite3_free override
// We provide a C-linkage sqlite3_free to intercept deallocation calls performed
// inside sqlite3Fts5HashClear. We count frees to verify that all entries are freed.
// Note: This overrides the symbol in the linked binary for test purposes.
// ---------------------------------------------------------------------------

static int g_freed_count = 0;

extern "C" void sqlite3_free(void *p){
    // Free the memory (safe for memory allocated via malloc/free in test)
    if (p) {
        free(p);
    }
    // Track deallocation count
    ++g_freed_count;
}

// Helper to create a chain entry with a provided next pointer
static Fts5HashEntry* make_entry(Fts5HashEntry* next){
    Fts5HashEntry* e = (Fts5HashEntry*)malloc(sizeof(Fts5HashEntry));
    if (!e) return nullptr;
    e->pHashNext = next;
    return e;
}

// Simple assertion helpers that do not terminate the test on failure
static int g_tests_failed = 0;
#define FAIL(msg) do { std::cerr << "TEST FAILED: " << msg << " (line " << __LINE__ << ")" << std::endl; ++g_tests_failed; } while(0)
#define TRUE_EXPECT(cond, msg) do { if(!(cond)) { FAIL(msg); } } while(0)
#define EQUAL_EXPECT(a, b, msg) do { if((a) != (b)) { FAIL(msg); } } while(0)

// Utilities for running tests
static void reset_freed_counter(){ g_freed_count = 0; }

// ---------------------------------------------------------------------------
// Test Case 1: Clear a hash with a single non-empty slot containing a chain of N entries
// - Build aHash with nSlot = 4
// - In slot 0, create a chain of 3 Fts5HashEntry objects
// - Call sqlite3Fts5HashClear
// - Verify: nEntry == 0, all aSlot[i] == NULL, and exactly 3 frees occurred
// ---------------------------------------------------------------------------

static void test_Clear_SingleSlotWithChain(){
    // Prepare hash with one non-empty chain
    Fts5Hash hash;
    hash.nSlot = 4;
    hash.nEntry = 0;
    hash.aSlot = (Fts5HashEntry**)malloc(hash.nSlot * sizeof(Fts5HashEntry*));
    for (int i = 0; i < hash.nSlot; ++i) hash.aSlot[i] = NULL;

    // Create a chain of 3 entries in slot 0: e1 -> e2 -> e3 -> NULL
    Fts5HashEntry* e3 = make_entry(NULL);
    Fts5HashEntry* e2 = make_entry(e3);
    Fts5HashEntry* e1 = make_entry(e2);
    hash.aSlot[0] = e1;
    hash.nEntry = 3;

    reset_freed_counter();
    // Call the focal method
    sqlite3Fts5HashClear(&hash);

    // Assertions
    EQUAL_EXPECT(hash.nEntry, 0, "nEntry should be reset to 0 after Clear");
    for (int i = 0; i < hash.nSlot; ++i) {
        TRUE_EXPECT(hash.aSlot[i] == NULL, "aSlot["<<i<<"] should be NULL after Clear");
    }
    TRUE_EXPECT(g_freed_count == 3, "Expected 3 frees to have occurred for 3 entries");

    // Cleanup: free aSlot array (entries already freed)
    free(hash.aSlot);
    hash.aSlot = NULL;
}

// ---------------------------------------------------------------------------
// Test Case 2: Clear a hash with multiple slots containing separate chains
// - Slot 1: chain of 1 entry
// - Slot 2: chain of 2 entries
// - Other slots: empty
// - After Clear: all entries freed, all slots NULL, nEntry == 0
// ---------------------------------------------------------------------------

static void test_Clear_MultipleSlotsWithSeparateChains(){
    Fts5Hash hash;
    hash.nSlot = 5;
    hash.nEntry = 0;
    hash.aSlot = (Fts5HashEntry**)malloc(hash.nSlot * sizeof(Fts5HashEntry*));
    for (int i = 0; i < hash.nSlot; ++i) hash.aSlot[i] = NULL;

    // Slot 1: one entry
    Fts5HashEntry* s1 = make_entry(NULL);
    hash.aSlot[1] = s1;
    hash.nEntry += 1;

    // Slot 2: two entries (head -> next -> NULL)
    Fts5HashEntry* s2b = make_entry(NULL);
    Fts5HashEntry* s2a = make_entry(s2b);
    hash.aSlot[2] = s2a;
    hash.nEntry += 2;

    reset_freed_counter();
    sqlite3Fts5HashClear(&hash);

    EQUAL_EXPECT(hash.nEntry, 0, "nEntry should be reset to 0 after Clear (multislot)");
    for (int i = 0; i < hash.nSlot; ++i) {
        TRUE_EXPECT(hash.aSlot[i] == NULL, "aSlot["<<i<<"] should be NULL after Clear (multislot)");
    }
    TRUE_EXPECT(g_freed_count == 3, "Expected 3 frees to have occurred for 3 entries (multislot)");

    // Cleanup
    free(hash.aSlot);
    hash.aSlot = NULL;
}

// ---------------------------------------------------------------------------
// Test Case 3: Clear an empty hash (no entries in any slot)
// - nSlot > 0, but all aSlot[i] are NULL
// - After Clear: no frees, nEntry remains 0, aSlot entries remain NULL
// ---------------------------------------------------------------------------

static void test_Clear_EmptyHash(){
    Fts5Hash hash;
    hash.nSlot = 3;
    hash.nEntry = 0;
    hash.aSlot = (Fts5HashEntry**)malloc(hash.nSlot * sizeof(Fts5HashEntry*));
    for (int i = 0; i < hash.nSlot; ++i) hash.aSlot[i] = NULL;

    reset_freed_counter();
    sqlite3Fts5HashClear(&hash);

    EQUAL_EXPECT(hash.nEntry, 0, "nEntry should remain 0 for empty hash");
    for (int i = 0; i < hash.nSlot; ++i) {
        TRUE_EXPECT(hash.aSlot[i] == NULL, "aSlot["<<i<<"] should remain NULL for empty hash");
    }
    TRUE_EXPECT(g_freed_count == 0, "No frees should occur for empty hash");

    // Cleanup
    free(hash.aSlot);
    hash.aSlot = NULL;
}

// ---------------------------------------------------------------------------
// Test Case 4: Boundary case - multiple small slots with mixed content,
// plus additional sanity check that aSlot is zeroed after Clear.
// - We reuse Test Case 2 style but add explicit check for aSlot zeroing.
// ---------------------------------------------------------------------------

static void test_Clear_VerifySlotZeroing(){
    Fts5Hash hash;
    hash.nSlot = 4;
    hash.nEntry = 0;
    hash.aSlot = (Fts5HashEntry**)malloc(hash.nSlot * sizeof(Fts5HashEntry*));
    for (int i = 0; i < hash.nSlot; ++i) hash.aSlot[i] = NULL;

    // Slot 3: two entries
    Fts5HashEntry* a = make_entry(NULL);
    Fts5HashEntry* b = make_entry(NULL);
    a->pHashNext = b;
    hash.aSlot[3] = a;
    hash.nEntry += 2;

    reset_freed_counter();
    sqlite3Fts5HashClear(&hash);

    // Post-conditions
    EQUAL_EXPECT(hash.nEntry, 0, "nEntry should be 0 after Clear (slot zeroing test)");
    for (int i = 0; i < hash.nSlot; ++i) {
        TRUE_EXPECT(hash.aSlot[i] == NULL, "aSlot["<<i<<"] should be NULL after Clear (slot zeroing test)");
    }
    TRUE_EXPECT(g_freed_count == 2, "Expected 2 frees for 2 entries (slot zeroing test)");

    // Cleanup
    free(hash.aSlot);
    hash.aSlot = NULL;
}

// ---------------------------------------------------------------------------
// Test runner and main
// ---------------------------------------------------------------------------

int main(){
    // Run tests
    test_Clear_SingleSlotWithChain();
    test_Clear_MultipleSlotsWithSeparateChains();
    test_Clear_EmptyHash();
    test_Clear_VerifySlotZeroing();

    // Summary
    if (g_tests_failed == 0) {
        std::cout << "ALL TESTS PASSED" << std::endl;
        return 0;
    } else {
        std::cerr << g_tests_failed << " TEST(S) FAILED" << std::endl;
        return 1;
    }
}