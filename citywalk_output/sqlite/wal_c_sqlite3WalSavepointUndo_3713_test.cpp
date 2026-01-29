/*
  Unit Test Suite for sqlite3WalSavepointUndo (wal.c)

  Objective:
  - Validate behavior of sqlite3WalSavepointUndo(Wal *pWal, u32 *aWalData)
  - Cover true/false branches of key predicates, wraparound handling, and non-wraparound updates.
  - Exercise both wraparound (aWalData[3] != pWal->nCkpt) and non-wraparound paths.
  - Use a lightweight C++11 test harness without GoogleTest; rely only on the C headers provided by the project.

  Assumptions:
  - The project is WAL-enabled (SQLITE_OMIT_WAL not defined).
  - wal.h and sqlite3.h headers provide necessary types (Wal, u32, SQLITE_OK, etc.).
  - walCleanupHash is a static no-op in this build (as per provided snippet), so invoking it is safe in tests.
  - Tests are linked with wal.c and its dependencies to resolve internal static calls and SEH_TRY/EXCEPT macros.
  - We avoid private/mutable internal methods; we only use the public prototype for sqlite3WalSavepointUndo and the Wal struct.

  Test coverage goals:
  - Branch 1: aWalData[3] != pWal->nCkpt (wraparound case)
  - Branch 2: aWalData[3] == pWal->nCkpt and aWalData[0] < pWal->hdr.mxFrame (standard update)
  - Branch 3: aWalData[3] == pWal->nCkpt and aWalData[0] >= pWal->hdr.mxFrame (no update)
  - Assertions are non-terminating (EXPECT_*) to maximize coverage without aborting tests.

  Notes:
  - Test prints detailed failure messages on mismatches and aggregates a simple pass/fail summary.
  - Each test is annotated with comments describing the scenario.
*/

#include <Windows.h>
#include <sqlite3.h>
#include <cstring>
#include <iostream>
#include <wal.h>


// Import C headers with C linkage so names are preserved for linking with wal.c
extern "C" {
}

// Lightweight test harness
static int g_total_checks = 0;
static int g_failed_checks = 0;

#define EXPECT_EQ(a, b) do { \
    ++g_total_checks; \
    if ((a) != (b)) { \
        std::cerr << "Test failure: " << __FILE__ << ":" << __LINE__ \
                  << "  Expected (" << #a << ") == (" << #b << "), but got " \
                  << static_cast<unsigned long long>(a) << " != " \
                  << static_cast<unsigned long long>(b) << "\n"; \
        ++g_failed_checks; \
    } \
} while (0)

#define EXPECT_TRUE(cond) do { \
    ++g_total_checks; \
    if (!(cond)) { \
        std::cerr << "Test failure: " << __FILE__ << ":" << __LINE__ \
                  << "  Condition failed: " << #cond << "\n"; \
        ++g_failed_checks; \
    } \
} while (0)

// Test 1: Wraparound path (aWalData[3] != pWal->nCkpt) should reset aWalData[0] to 0
// and aWalData[3] to nCkpt, then update mxFrame to 0 and copy checksums.
void test_WalSavepointUndo_wraparound_updates_fields() {
    // Prepare Wal with a known state
    Wal w;
    std::memset(&w, 0, sizeof(Wal));
    w.writeLock = 1;           // Must be non-zero to satisfy assert
    w.hdr.mxFrame = 100;       // Current maximum frame
    w.nCkpt = 5;                // Current checkpoint

    // aWalData: wraparound scenario (aWalData[3] != nCkpt initially)
    // The function will set aWalData[0] = 0 and aWalData[3] = nCkpt (5)
    // Then since aWalData[0] < mxFrame (0 < 100), it will update mxFrame to 0
    // and copy aWalData[1..2] into hdr.aFrameCksum.
    u32 aWalData[4] = { 50, 0x11111111, 0x22222222, 6 };

    int rc = sqlite3WalSavepointUndo(&w, aWalData);

    // rc should be SQLITE_OK (no exception path triggered)
    EXPECT_EQ(rc, SQLITE_OK);

    // After wraparound handling:
    EXPECT_EQ(aWalData[0], 0u);
    EXPECT_EQ(aWalData[3], w.nCkpt);

    // hdr.mxFrame should be updated to 0
    EXPECT_EQ(w.hdr.mxFrame, 0u);

    // aFrameCksum should be copied from aWalData[1], aWalData[2]
    EXPECT_EQ(w.hdr.aFrameCksum[0], 0x11111111u);
    EXPECT_EQ(w.hdr.aFrameCksum[1], 0x22222222u);
}

// Test 2: Non-wraparound path (aWalData[3] == nCkpt) and aWalData[0] < mxFrame.
// Expect mxFrame and aFrameCksum to reflect aWalData[0], aWalData[1], aWalData[2].
void test_WalSavepointUndo_no_wraparound_updates_frame_and_cksum() {
    Wal w;
    std::memset(&w, 0, sizeof(Wal));
    w.writeLock = 1;
    w.hdr.mxFrame = 100;
    w.nCkpt = 5;

    // aWalData: no wraparound, smaller savepoint than current mxFrame
    u32 aWalData[4] = { 10, 0xAAAA, 0xBBBB, 5 };

    int rc = sqlite3WalSavepointUndo(&w, aWalData);

    EXPECT_EQ(rc, SQLITE_OK);

    // Expect that the function updated mxFrame and aFrameCksum
    EXPECT_EQ(w.hdr.mxFrame, 10u);
    EXPECT_EQ(w.hdr.aFrameCksum[0], 0xAAAAu);
    EXPECT_EQ(w.hdr.aFrameCksum[1], 0xBBBBu);

    // aWalData should remain unchanged in its contents except for the earlier wrap logic,
    // so ensure its first and fourth elements remain as provided.
    EXPECT_EQ(aWalData[0], 10u);
    EXPECT_EQ(aWalData[3], 5u);
}

// Test 3: No update path when aWalData[0] is equal to mxFrame (no update expected).
// This ensures the equality branch does not modify internal state.
void test_WalSavepointUndo_no_update_when_equal_or_greater() {
    Wal w;
    std::memset(&w, 0, sizeof(Wal));
    w.writeLock = 1;
    w.hdr.mxFrame = 100;
    w.nCkpt = 5;

    // aWalData: equality case (aWalData[0] == mxFrame), no wraparound
    u32 aWalData[4] = { 100, 0x1234, 0x5678, 5 };

    int rc = sqlite3WalSavepointUndo(&w, aWalData);

    EXPECT_EQ(rc, SQLITE_OK);

    // No internal state should change
    EXPECT_EQ(w.hdr.mxFrame, 100u);
    EXPECT_EQ(w.hdr.aFrameCksum[0], 0u);
    EXPECT_EQ(w.hdr.aFrameCksum[1], 0u);

    // aWalData should remain unchanged
    EXPECT_EQ(aWalData[0], 100u);
    EXPECT_EQ(aWalData[3], 5u);
}

int main() {
    // Run tests
    test_WalSavepointUndo_wraparound_updates_fields();
    test_WalSavepointUndo_no_wraparound_updates_frame_and_cksum();
    test_WalSavepointUndo_no_update_when_equal_or_greater();

    // Summary
    std::cout << "Total checks: " << g_total_checks
              << ", Failures: " << g_failed_checks << std::endl;

    // Return non-zero if any test failed
    return (g_failed_checks == 0) ? 0 : 1;
}