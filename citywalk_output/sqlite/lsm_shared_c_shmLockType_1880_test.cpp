/*
  Unit test suite for the focal method:
  int shmLockType(lsm_db *db, int iLock)

  approach:
  - Provide a minimal test harness (no gtest, no gmock) with lightweight
    non-terminating assertions to maximize code coverage.
  - Create a compatible lsm_db layout locally to exercise shmLockType.
  - Assume the following conventional macro values used by similar LSM code:
      LSM_LOCK_EXCL  = 1
      LSM_LOCK_SHARED= 2
      LSM_LOCK_UNLOCK= 0
    These are defined in the test to compare the function's return value.
  - Cover true/false branches for iLock in a safe range (1..32) which
    ensures (iLock-1) and (iLock+32-1) stay within 64-bit bounds.
  - Provide explanatory comments for each unit test.
  - The test uses a C-compatible prototype for shmLockType and a local
    lsm_db structure with the mLock member at the expected offset (first member).

  Notes:
  - This test is designed to be compiled and linked with the existing C
    source offering shmLockType, under a C linkage (extern "C").
  - The test does not rely on gtest/gmock per instructions.
*/

#include <lsmInt.h>
#include <cstdio>
#include <iomanip>
#include <cinttypes>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Domain knowledge constraints:
extern "C" {
  // Forward declaration to link against the focus function from the C source.
  // We define a local lsm_db struct with the mLock field as the first member
  // to mirror the usage in shmLockType (db->mLock).
  struct lsm_db;
  typedef struct lsm_db lsm_db;
  // Prototyping shmLockType with C linkage (as declared in the source under test)
  int shmLockType(lsm_db *db, int iLock);
}

// Local re-declaration of the lsm_db shape used for tests.
// We place mLock as the first field to ensure db->mLock resolves correctly.
struct lsm_db {
  uint64_t mLock; // Primary lock bitmask used by shmLockType
};

// Local constants matching (typical) values used by the library under test.
// If the actual binary uses different values, these tests would need alignment
// with the library's constants.
static const int LSM_LOCK_EXCL  = 1;
static const int LSM_LOCK_SHARED = 2;
static const int LSM_LOCK_UNLOCK = 0;

// Lightweight non-terminating test framework
static int g_total_tests = 0;
static int g_failed_tests = 0;

template <typename T, typename U>
static void expect_eq(const char* expr, const T& a, const U& b, const char* note = "") {
  ++g_total_tests;
  if (!(a == b)) {
    ++g_failed_tests;
    std::cerr << "Test failed: " << expr << " | "
              << "Expected: " << b << ", Actual: " << a;
    if (note && note[0] != '\0') {
      std::cerr << " | Note: " << note;
    }
    std::cerr << std::endl;
  }
}

// Helper macro for concise expectations (only for integral-ish types)
#define EXPECT_EQ(A,B) expect_eq(#A " == " #B, (A), (B))

// Test 1: iLock = 1, mLock has exclusive bit set (me). Expect EXCL.
static void test_shmLockType_excl_when_me_bit_set_iLock1() {
  lsm_db db;
  db.mLock = 0;

  // Set exclusive bit for iLock = 1:
  // me = 1 << (iLock-1) = 1 << 0 = 1
  db.mLock = (uint64_t)1 << (1 - 1); // 1

  int rc = shmLockType(&db, 1);
  EXPECT_EQ( rc, LSM_LOCK_EXCL );
}

// Test 2: iLock = 1, mLock has shared bit set (ms) only. Expect SHARED.
static void test_shmLockType_shared_when_ms_bit_set_iLock1() {
  lsm_db db;
  db.mLock = 0;

  // ms = 1 << (iLock+32-1) = 1 << 32
  db.mLock = ((uint64_t)1) << (1 + 32 - 1); // 1ULL << 32

  int rc = shmLockType(&db, 1);
  EXPECT_EQ( rc, LSM_LOCK_SHARED );
}

// Test 3: iLock = 1, both bits set. Expect EXCL due to first check.
static void test_shmLockType_excl_overrides_shared_when_both_bits_set_iLock1() {
  lsm_db db;
  db.mLock = 0;

  uint64_t me = ((uint64_t)1 << (1 - 1));        // 1
  uint64_t ms = ((uint64_t)1 << (1 + 32 - 1));   // 1 << 32
  db.mLock = me | ms;

  int rc = shmLockType(&db, 1);
  EXPECT_EQ( rc, LSM_LOCK_EXCL );
}

// Test 4: iLock = 32, mLock has exclusive bit set (me) at high position. Expect EXCL.
static void test_shmLockType_excl_with_iLock32_me_only() {
  lsm_db db;
  db.mLock = 0;

  // iLock = 32 -> me at bit 31 (1ULL << 31)
  db.mLock = ((uint64_t)1) << (32 - 1);

  int rc = shmLockType(&db, 32);
  EXPECT_EQ( rc, LSM_LOCK_EXCL );
}

// Test 5: iLock = 32, mLock has shared bit set (ms) only. Expect SHARED.
static void test_shmLockType_shared_with_iLock32_ms_only() {
  lsm_db db;
  db.mLock = 0;

  // iLock = 32 -> ms at bit 63 (1ULL << 63)
  db.mLock = ((uint64_t)1) << (32 + 32 - 1);

  int rc = shmLockType(&db, 32);
  EXPECT_EQ( rc, LSM_LOCK_SHARED );
}

// Test 6: iLock = 32, neither bit set. Expect UNLOCK.
static void test_shmLockType_unlock_when_none_set_iLock32() {
  lsm_db db;
  db.mLock = 0;

  int rc = shmLockType(&db, 32);
  EXPECT_EQ( rc, LSM_LOCK_UNLOCK );
}

// Function to run all tests
static void run_all_tests() {
  test_shmLockType_excl_when_me_bit_set_iLock1();
  test_shmLockType_shared_when_ms_bit_set_iLock1();
  test_shmLockType_excl_overrides_shared_when_both_bits_set_iLock1();
  test_shmLockType_excl_with_iLock32_me_only();
  test_shmLockType_shared_with_iLock32_ms_only();
  test_shmLockType_unlock_when_none_set_iLock32();

  // Summary
  std::cout << "\nTest Summary: "
            << g_total_tests << " executed, "
            << g_failed_tests << " failed.\n";
}

int main() {
  // Run tests and return non-zero if any failed test to indicate CI failure.
  run_all_tests();
  if (g_failed_tests != 0) {
    std::cerr << "SHM Lock Type tests FAILED." << std::endl;
    return 1;
  }
  std::cout << "SHM Lock Type tests PASSED." << std::endl;
  return 0;
}