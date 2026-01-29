/*
Unit test suite for the focal method: lsmCheckpointLogoffset
Location under test: lsm_ckpt.c
Targeted method: void lsmCheckpointLogoffset(u32 *aCkpt, DbLog *pLog)

Approach summary:
- Use a lightweight C++11 test harness (no Google Test) with non-terminating assertions
  to maximize code coverage.
- Exercise both data-copy paths and computed fields:
  - cksum0 and cksum1 should reflect aCkpt[CKPT_HDR_LO_CKSUM1] and
    aCkpt[CKPT_HDR_LO_CKSUM2] respectively.
  - iSnapshotId should be set via lsmCheckpointId(aCkpt, 0).
  - aRegion[2].iStart should be assigned as (lsmCheckpointLogOffset(aCkpt) >> 1).
- Validate with multiple distinct aCkpt inputs to cover potential variations.
- Utilize the real types and symbols from the provided project headers (lsmInt.h)
  and declared external C functions to prevent private/static linkage issues.

Candidate Keywords (Step 1): aCkpt, pLog, aRegion, iStart, lsmCheckpointLogOffset, CKPT_HDR_LO_CKSUM1, CKPT_HDR_LO_CKSUM2, cksum0, cksum1, iSnapshotId, lsmCheckpointId, DbLog, u32, lsmCheckpointLogoffset, lsmCheckpointLogOffset, i64, CKPT header, checkpoint, logoffset.

The tests below rely on the project’s actual definitions for the above symbols. They are written to be non-terminating in the presence of failures (they report failures and continue).
*/

#include <lsmInt.h>
#include <cstdio>
#include <cstring>


// Bring in the project’s types and constants.
// The test harness must compile with the real project headers, so we include lsmInt.h.
extern "C" {
}

// Prototypes of the focal function and related helpers (as they exist in the project).
extern "C" void lsmCheckpointLogoffset(u32 *aCkpt, DbLog *pLog);
extern "C" i64 lsmCheckpointLogOffset(u32 *aCkpt);
extern "C" i64 lsmCheckpointId(u32 *aCkpt, int bDisk);

// Lightweight test harness (non-terminating assertions)
static int g_totalAssertions = 0;
static int g_failedAssertions = 0;

#define ASSERT_EQ(actual, expected) do { \
    ++g_totalAssertions; \
    if (!((actual) == (expected))) { \
        printf("ASSERT FAILED: %s == %s (actual=%lld, expected=%lld) at %s:%d\n", \
               #actual, #expected, (long long)(actual), (long long)(expected), __FILE__, __LINE__); \
        ++g_failedAssertions; \
    } \
} while (0)

#define ASSERT_INT_EQ(actual, expected) ASSERT_EQ((long long)(actual), (long long)(expected))

#define ASSERT_TRUE(cond) do { \
    ++g_totalAssertions; \
    if (!(cond)) { \
        printf("ASSERT FAILED: condition %s is false at %s:%d\n", #cond, __FILE__, __LINE__); \
        ++g_failedAssertions; \
    } \
} while (0)

// Helper to print a separator for test output
static void printSection(const char* name) {
    printf("\n=== TEST SECTION: %s ===\n", name);
}

// Test 1: Basic functionality — verifies that lsmCheckpointLogoffset correctly
// copies header checksums, sets iSnapshotId, and computes iStart using
// lsmCheckpointLogOffset(aCkpt) >> 1.
static void test_lsmCheckpointLogoffset_basic() {
    printSection("lsmCheckpointLogoffset_basic");

    // Prepare a checkpoint array with known header checksums
    const int kCkptSize = 128;
    u32 aCkpt[kCkptSize];
    std::memset(aCkpt, 0, sizeof(aCkpt));

    // Set header checksum fields to known values
    aCkpt[CKPT_HDR_LO_CKSUM1] = 0xDEADBEEF;
    aCkpt[CKPT_HDR_LO_CKSUM2] = 0xBAADF00D;

    // Prepare a DbLog structure with zeroed memory
    DbLog log;
    std::memset(&log, 0, sizeof(log));

    // Invoke the function under test
    lsmCheckpointLogoffset(aCkpt, &log);

    // Expected values computed via the project-provided helpers
    i64 offset = lsmCheckpointLogOffset(aCkpt);
    i64 expectedIStart = offset >> 1;
    i64 expectedSnapshotId = lsmCheckpointId(aCkpt, 0);

    // Assertions
    ASSERT_INT_EQ(log.cksum0, aCkpt[CKPT_HDR_LO_CKSUM1]);
    ASSERT_INT_EQ(log.cksum1, aCkpt[CKPT_HDR_LO_CKSUM2]);
    ASSERT_INT_EQ(log.iSnapshotId, expectedSnapshotId);

    // aRegion[2].iStart must equal (offset >> 1)
    // Cast to a compatible integral type for comparison
    // Note: We rely on the actual type of aRegion[].iStart from lsmInt.h
    // by comparing against the same computed value.
    ASSERT_INT_EQ(log.aRegion[2].iStart, (int)expectedIStart);
}

// Test 2: Variation test — ensure function behaves correctly for a different
// set of header checksums and offset computations. This ensures the copy path
// is robust against different input data.
static void test_lsmCheckpointLogoffset_variant_inputs() {
    printSection("lsmCheckpointLogoffset_variant_inputs");

    const int kCkptSize = 128;
    u32 aCkptA[kCkptSize];
    u32 aCkptB[kCkptSize];
    std::memset(aCkptA, 0, sizeof(aCkptA));
    std::memset(aCkptB, 0, sizeof(aCkptB));

    // Variant A
    aCkptA[CKPT_HDR_LO_CKSUM1] = 0x11111111;
    aCkptA[CKPT_HDR_LO_CKSUM2] = 0x22222222;

    // Variant B
    aCkptB[CKPT_HDR_LO_CKSUM1] = 0xAAAAAAAA;
    aCkptB[CKPT_HDR_LO_CKSUM2] = 0x55555555;

    DbLog logA;
    DbLog logB;
    std::memset(&logA, 0, sizeof(logA));
    std::memset(&logB, 0, sizeof(logB));

    lsmCheckpointLogoffset(aCkptA, &logA);
    lsmCheckpointLogoffset(aCkptB, &logB);

    // Cross-validate that the cksums reflect respective inputs
    ASSERT_INT_EQ(logA.cksum0, aCkptA[CKPT_HDR_LO_CKSUM1]);
    ASSERT_INT_EQ(logA.cksum1, aCkptA[CKPT_HDR_LO_CKSUM2]);

    ASSERT_INT_EQ(logB.cksum0, aCkptB[CKPT_HDR_LO_CKSUM1]);
    ASSERT_INT_EQ(logB.cksum1, aCkptB[CKPT_HDR_LO_CKSUM2]);

    // Validate iSnapshotId via cross-call
    ASSERT_INT_EQ(logA.iSnapshotId, lsmCheckpointId(aCkptA, 0));
    ASSERT_INT_EQ(logB.iSnapshotId, lsmCheckpointId(aCkptB, 0));

    // Validate iStart derived from lsmCheckpointLogOffset
    i64 offsetA = lsmCheckpointLogOffset(aCkptA);
    i64 offsetB = lsmCheckpointLogOffset(aCkptB);
    ASSERT_INT_EQ(logA.aRegion[2].iStart, (int)(offsetA >> 1));
    ASSERT_INT_EQ(logB.aRegion[2].iStart, (int)(offsetB >> 1));
}

// Main function to run tests (Step 3: tests invoked from main as gtest is not allowed)
int main() {
    // Run tests, allowing all to execute to maximize coverage
    test_lsmCheckpointLogoffset_basic();
    test_lsmCheckpointLogoffset_variant_inputs();

    // Print summary
    printf("\nTEST SUMMARY: total_assertions=%d, failed_assertions=%d\n",
           g_totalAssertions, g_failedAssertions);

    // Return non-zero if any assertion failed
    return (g_failedAssertions != 0) ? 1 : 0;
}