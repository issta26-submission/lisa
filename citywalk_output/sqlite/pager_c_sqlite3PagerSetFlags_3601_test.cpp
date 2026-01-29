// Unit test suite for sqlite3PagerSetFlags (C-style function) using C++11 without GTest.
// The tests are self-contained: a minimal Pager struct and constants are declared to mirror
// the behavior of the focal function. The test harness exercises true/false branches and
// critical paths to achieve high coverage.

#include <iomanip>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <wal.h>
#include <cstdint>


// Domain knowledge: Define necessary constants to mirror pager.c behavior.
// These are stand-ins for the real SQLite values but preserve logical behavior for tests.

static const unsigned PAGER_SYNCHRONOUS_MASK   = 0x3;
static const unsigned PAGER_SYNCHRONOUS_OFF      = 0x0;
static const unsigned PAGER_SYNCHRONOUS_NORMAL   = 0x1;
static const unsigned PAGER_SYNCHRONOUS_FULL     = 0x2;
static const unsigned PAGER_SYNCHRONOUS_EXTRA    = 0x3;

static const unsigned PAGER_FULLFSYNC        = 0x01;  // bit flag
static const unsigned PAGER_CKPT_FULLFSYNC   = 0x02;  // bit flag
static const unsigned PAGER_CACHESPILL        = 0x04;  // bit flag

static const unsigned SPILLFLAG_OFF = 0x01; // internal spill flag bit

static const unsigned SQLITE_SYNC_NORMAL = 1;
static const unsigned SQLITE_SYNC_FULL   = 2;

// Minimal Pager structure necessary for sqlite3PagerSetFlags
struct Pager {
    // Input state
    bool tempFile;

    // Internal state modified by sqlite3PagerSetFlags
    unsigned noSync;
    unsigned fullSync;
    unsigned extraSync;
    unsigned syncFlags;
    unsigned walSyncFlags;
    unsigned doNotSpill;

    // ctor for convenience
    Pager() : tempFile(false), noSync(0), fullSync(0), extraSync(0),
              syncFlags(0), walSyncFlags(0), doNotSpill(0) {}
};

// Implement a local replica of the focal function with the same logic.
// This mirrors the provided C code but uses the test constants above.
void sqlite3PagerSetFlags(
  Pager *pPager,        /* The pager to set safety level for */
  unsigned pgFlags      /* Various flags */
){
    unsigned level = pgFlags & PAGER_SYNCHRONOUS_MASK;
    if( pPager->tempFile ){
        pPager->noSync = 1;
        pPager->fullSync = 0;
        pPager->extraSync = 0;
    }else{
        pPager->noSync =  level==PAGER_SYNCHRONOUS_OFF ?1:0;
        pPager->fullSync = level>=PAGER_SYNCHRONOUS_FULL ?1:0;
        pPager->extraSync = level==PAGER_SYNCHRONOUS_EXTRA ?1:0;
    }
    if( pPager->noSync ){
        pPager->syncFlags = 0;
    }else if( pgFlags & PAGER_FULLFSYNC ){
        pPager->syncFlags = SQLITE_SYNC_FULL;
    }else{
        pPager->syncFlags = SQLITE_SYNC_NORMAL;
    }
    pPager->walSyncFlags = (pPager->syncFlags<<2);
    if( pPager->fullSync ){
        pPager->walSyncFlags |= pPager->syncFlags;
    }
    if( (pgFlags & PAGER_CKPT_FULLFSYNC) && !pPager->noSync ){
        pPager->walSyncFlags |= (SQLITE_SYNC_FULL<<2);
    }
    if( pgFlags & PAGER_CACHESPILL ){
        pPager->doNotSpill &= ~SPILLFLAG_OFF;
    }else{
        pPager->doNotSpill |= SPILLFLAG_OFF;
    }
}

// Simple non-terminating assertion helpers
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define TEST_ASSERT_EQ(expected, actual, msg) do { \
    ++g_tests_run; \
    if((expected) != (actual)) { \
        ++g_tests_failed; \
        std::cerr << "[FAIL] " << msg << " | expected: " << (expected) \
                  << ", actual: " << (actual) << std::endl; \
    } \
} while(0)

static void test_tempFile_branch_with_and_without_cachespill() {
    // Test 1a: tempFile = true, cachespill flag set, initial doNotSpill = SPILLFLAG_OFF
    {
        Pager p;
        p.tempFile = true;
        p.doNotSpill = SPILLFLAG_OFF; // initial non-off condition to observe effect
        unsigned pgFlags = PAGER_CACHESPILL; // trigger doNotSpill clearing

        sqlite3PagerSetFlags(&p, pgFlags);

        bool ok = true;
        TEST_ASSERT_EQ(1u, p.noSync, "tempFile: noSync should be 1");
        TEST_ASSERT_EQ(0u, p.fullSync, "tempFile: fullSync should be 0");
        TEST_ASSERT_EQ(0u, p.extraSync, "tempFile: extraSync should be 0");
        TEST_ASSERT_EQ(0u, p.syncFlags, "tempFile: syncFlags should be 0");
        TEST_ASSERT_EQ(0u, p.walSyncFlags, "tempFile: walSyncFlags should be 0");
        // doNotSpill should be cleared because SPILLFLAG_OFF is cleared when PAGER_CACHESPILL is set
        TEST_ASSERT_EQ(0u, p.doNotSpill, "tempFile: doNotSpill should be cleared to 0");

        if(p.noSync!=1 || p.fullSync!=0 || p.extraSync!=0 || p.syncFlags!=0 ||
           p.walSyncFlags!=0 || p.doNotSpill!=0) ok = false;
        if(!ok) {
            std::cerr << "  [Detail] tempFile branch 1a assertions failed.\n";
        }
    }

    // Test 1b: tempFile = true, cachespill not set, initial doNotSpill = 0
    {
        Pager p;
        p.tempFile = true;
        p.doNotSpill = 0;
        unsigned pgFlags = 0; // no cachespill

        sqlite3PagerSetFlags(&p, pgFlags);

        bool ok = true;
        TEST_ASSERT_EQ(1u, p.noSync, "tempFile: (b) noSync should be 1");
        TEST_ASSERT_EQ(0u, p.fullSync, "tempFile: (b) fullSync should be 0");
        TEST_ASSERT_EQ(0u, p.extraSync, "tempFile: (b) extraSync should be 0");
        TEST_ASSERT_EQ(0u, p.syncFlags, "tempFile: (b) syncFlags should be 0");
        TEST_ASSERT_EQ(0u, p.walSyncFlags, "tempFile: (b) walSyncFlags should be 0");
        // doNotSpill should be set to SPILLFLAG_OFF when cachespill not set
        TEST_ASSERT_EQ(SPILLFLAG_OFF, p.doNotSpill, "tempFile: (b) doNotSpill should be SPILLFLAG_OFF");

        if(p.noSync!=1 || p.fullSync!=0 || p.extraSync!=0 || p.syncFlags!=0 ||
           p.walSyncFlags!=0 || p.doNotSpill!=SPILLFLAG_OFF) ok = false;
        if(!ok) {
            std::cerr << "  [Detail] tempFile branch 1b assertions failed.\n";
        }
    }
}

static void test_notTemp_branch_basic_synchronous_levels() {
    // Test 2a: level OFF (pPager->noSync = 1)
    {
        Pager p;
        p.tempFile = false;
        p.doNotSpill = 0;
        // pgFlags: OFF
        unsigned pgFlags = PAGER_SYNCHRONOUS_OFF; // lower bits 0
        sqlite3PagerSetFlags(&p, pgFlags);

        bool ok = true;
        TEST_ASSERT_EQ(1u, p.noSync, "notTemp: level OFF -> noSync");
        TEST_ASSERT_EQ(0u, p.fullSync, "notTemp: level OFF -> fullSync");
        TEST_ASSERT_EQ(0u, p.extraSync, "notTemp: level OFF -> extraSync");
        TEST_ASSERT_EQ(0u, p.syncFlags, "notTemp: level OFF -> syncFlags (should be 0 due to noSync)");
        TEST_ASSERT_EQ(0u, p.walSyncFlags, "notTemp: level OFF -> walSyncFlags (0)");
        // SPILL: since no cachespill bit set, doNotSpill should become SPILLFLAG_OFF
        TEST_ASSERT_EQ(SPILLFLAG_OFF, p.doNotSpill, "notTemp: level OFF -> doNotSpill OR SPILLFLAG_OFF");

        if(p.noSync!=1 || p.fullSync!=0 || p.extraSync!=0 || p.syncFlags!=0 ||
           p.walSyncFlags!=0 || p.doNotSpill!=SPILLFLAG_OFF) ok = false;
        if(!ok) {
            std::cerr << "  [Detail] notTemp branch 2a assertions failed.\n";
        }
    }

    // Test 2b: level NORMAL, no FULLFSYNC; PAGER_CACHESPILL not set
    {
        Pager p;
        p.tempFile = false;
        p.doNotSpill = 0;
        // pgFlags: NORMAL with no extra bits
        unsigned pgFlags = PAGER_SYNCHRONOUS_NORMAL;
        sqlite3PagerSetFlags(&p, pgFlags);

        bool ok = true;
        // level NORMAL => noSync = 0, fullSync = 0, extraSync = 0
        TEST_ASSERT_EQ(0u, p.noSync, "notTemp: level NORMAL -> noSync");
        TEST_ASSERT_EQ(0u, p.fullSync, "notTemp: level NORMAL -> fullSync");
        TEST_ASSERT_EQ(0u, p.extraSync, "notTemp: level NORMAL -> extraSync");
        // Since FULLFSYNC not set, syncFlags should be NORMAL
        TEST_ASSERT_EQ(SQLITE_SYNC_NORMAL, p.syncFlags, "notTemp: level NORMAL -> syncFlags NORMAL");
        // walSyncFlags should be syncFlags<<2
        TEST_ASSERT_EQ((SQLITE_SYNC_NORMAL<<2), p.walSyncFlags, "notTemp: level NORMAL -> walSyncFlags");

        // doNotSpill should be SPILLFLAG_OFF when PAGER_CACHESPILL not set
        TEST_ASSERT_EQ(SPILLFLAG_OFF, p.doNotSpill, "notTemp: level NORMAL -> doNotSpill SPILLFLAG_OFF");

        if(p.noSync!=0 || p.fullSync!=0 || p.extraSync!=0 ||
           p.syncFlags!=SQLITE_SYNC_NORMAL || p.walSyncFlags!=(SQLITE_SYNC_NORMAL<<2) ||
           p.doNotSpill!=SPILLFLAG_OFF) ok = false;
        if(!ok) {
            std::cerr << "  [Detail] notTemp branch 2b assertions failed.\n";
        }
    }

    // Test 2c: level NORMAL with PAGER_FULLFSYNC set
    {
        Pager p;
        p.tempFile = false;
        p.doNotSpill = 0;
        unsigned pgFlags = PAGER_SYNCHRONOUS_NORMAL | PAGER_FULLFSYNC;
        sqlite3PagerSetFlags(&p, pgFlags);

        bool ok = true;
        // level NORMAL -> noSync=0; fullSync depends on level (>= FULL) -> 1; extraSync = 0
        TEST_ASSERT_EQ(0u, p.noSync, "notTemp: level NORMAL + FULLFSYNC -> noSync");
        TEST_ASSERT_EQ(1u, p.fullSync, "notTemp: level NORMAL + FULLFSYNC -> fullSync");
        TEST_ASSERT_EQ(0u, p.extraSync, "notTemp: level NORMAL + FULLFSYNC -> extraSync");
        // Now syncFlags: since FULLFSYNC is set, syncFlags = SQLITE_SYNC_FULL
        TEST_ASSERT_EQ(SQLITE_SYNC_FULL, p.syncFlags, "notTemp: FULLFSYNC path -> syncFlags FULL");
        // walSyncFlags should be (syncFlags<<2) OR (syncFlags) because fullSync is set
        unsigned expectedWal = (SQLITE_SYNC_FULL<<2) | SQLITE_SYNC_FULL;
        TEST_ASSERT_EQ(expectedWal, p.walSyncFlags, "notTemp: FULLFSYNC path -> walSyncFlags includes FULL");
        // doNotSpill: since no cachespill, should be SPILLFLAG_OFF
        TEST_ASSERT_EQ(SPILLFLAG_OFF, p.doNotSpill, "notTemp: FULLFSYNC path -> doNotSpill SPILLFLAG_OFF");

        if(p.noSync!=0 || p.fullSync!=1 || p.extraSync!=0 ||
           p.syncFlags!=SQLITE_SYNC_FULL || p.walSyncFlags!=expectedWal ||
           p.doNotSpill!=SPILLFLAG_OFF) ok = false;
        if(!ok) {
            std::cerr << "  [Detail] notTemp branch 2c assertions failed.\n";
        }
    }
}

static void test_notTemp_branch_ckpt_effects() {
    // Test 3: CKPT_FULLFSYNC bit OR-ed into walSyncFlags when not in noSync and appropriate level
    {
        Pager p;
        p.tempFile = false;
        p.doNotSpill = 0;
        // Use NORMAL level (noSync = 0)
        unsigned pgFlags = PAGER_SYNCHRONOUS_NORMAL | PAGER_CKPT_FULLFSYNC;
        sqlite3PagerSetFlags(&p, pgFlags);

        bool ok = true;
        // level NORMAL -> noSync 0; fullSync 0; extraSync 0
        TEST_ASSERT_EQ(0u, p.noSync, "ckpt: level NORMAL -> noSync");
        TEST_ASSERT_EQ(0u, p.fullSync, "ckpt: level NORMAL -> fullSync");
        TEST_ASSERT_EQ(0u, p.extraSync, "ckpt: level NORMAL -> extraSync");
        // Since FULLFSYNC flag not set, syncFlags should be NORMAL
        TEST_ASSERT_EQ(SQLITE_SYNC_NORMAL, p.syncFlags, "ckpt: level NORMAL -> syncFlags NORMAL");
        // walSyncFlags initial
        unsigned expectedWal = (SQLITE_SYNC_NORMAL<<2); // 4
        // CKPT_FULLFSYNC adds (SQLITE_SYNC_FULL<<2) = 2<<2 = 8
        expectedWal |= (SQLITE_SYNC_FULL<<2); // 4 | 8 = 12
        TEST_ASSERT_EQ(expectedWal, p.walSyncFlags, "ckpt: ckpt bit set -> walSyncFlags includes CKPT FULLFSYNC");

        // doNotSpill should be SPILLFLAG_OFF since cachespill not set
        TEST_ASSERT_EQ(SPILLFLAG_OFF, p.doNotSpill, "ckpt: doNotSpill should be SPILLFLAG_OFF when cachespill not set");

        if(p.syncFlags!=SQLITE_SYNC_NORMAL || p.walSyncFlags!=expectedWal) ok = false;
        if(!ok) {
            std::cerr << "  [Detail] notTemp branch 3 assertions failed.\n";
        }
    }
}

// Main entry point: run all tests and report results.
int main() {
    std::cout << "Running unit tests for sqlite3PagerSetFlags (simulated in C++11)...\n";

    test_tempFile_branch_with_and_without_cachespill();
    test_notTemp_branch_basic_synchronous_levels();
    test_notTemp_branch_ckpt_effects();

    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << "\n";

    if(g_tests_failed == 0) {
        std::cout << "All tests PASSED.\n";
        return 0;
    } else {
        std::cout << "Some tests FAILED.\n";
        return 1;
    }
}