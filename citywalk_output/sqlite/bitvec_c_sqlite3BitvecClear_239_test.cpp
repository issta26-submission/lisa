/*
Unit test suite for sqlite3BitvecClear(Bitvec *p, u32 i, void *pBuf)

Overview:
- No Google Test; a lightweight, self-contained test harness is implemented.
- Tests aim to exercise key logical branches in sqlite3BitvecClear as defined in the provided focal method.
- Tests rely on the actual Bitvec structure and constants (from sqliteInt.h) to ensure faithful coverage.
- All tests avoid terminating on assertion failures by implementing a lightweight non-terminating CHECK macro.
- Tests are designed to be executable under C++11.

Notes:
- The tests cover:
  1) Null pointer return (p == 0)
  2) Simple bitmap clear path (iDivisor == 0, iSize <= BITVEC_NBIT)
  3) Large bitmap path (iSize > BITVEC_NBIT) where aHash is rebuilt
  4) Pre-traversal null path (iDivisor != 0 but apSub[bin] == NULL)
- Uses direct access to Bitvec fields as defined in sqliteInt.h (iSize, iDivisor, u.aBitmap, u.aHash, u.apSub, and nSet).

Build notes:
- Ensure the include path for sqliteInt.h is configured in your build environment.
- sqlite3BitvecClear is assumed to have C linkage; the test harness accesses it via extern "C".
- The test code avoids terminating on failures; it reports results and continues executing remaining tests.

*/

// Include standard library for I/O and memory operations
#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Include SQLite internal definitions to access Bitvec layout and constants
// Path may need adjustment based on your build setup

// Ensure C linkage for the C function under test
extern "C" {
  void sqlite3BitvecClear(Bitvec *p, u32 i, void *pBuf);
}

/* Lightweight test framework: non-terminating checks with reporting.
   This mirrors a minimal EXPECT-like behavior suitable when GTest is not allowed. */
static int tests_passed = 0;
static int tests_failed = 0;

#define CHECK(cond, msg) do { \
  if (cond) { \
    ++tests_passed; \
  } else { \
    ++tests_failed; \
    std::cerr << "Test failure: " << msg << std::endl; \
  } \
} while(0)

// Helper to create a simple Bitvec suitable for bitmap-path tests (iDivisor == 0, iSize <= BITVEC_NBIT)
static Bitvec* make_bitmap_simple(uint32_t bitToClear) {
  Bitvec *p = (Bitvec*)malloc(sizeof(Bitvec));
  if (!p) return nullptr;
  std::memset(p, 0, sizeof(Bitvec));

  // Configure for simple bitmap path
  p->iDivisor = 0;
  p->iSize = BITVEC_NBIT; // ensure small bitmap path is taken (<= BITVEC_NBIT)
  // Initialize the bitmap to all ones; ensure there is a bit to clear at position (bitToClear-1)
  // bitToClear is 1-based index into bits; if it is out of range, behavior is undefined but tests keep it valid
  p->u.aBitmap[0] = 0xFFFFFFFFu;

  // If test wants to clear a bit outside the first word, this simplistic helper still covers the primary branch.
  // The code under test uses i mapping with i/BITVEC_SZELEM and i&(BITVEC_SZELEM-1).
  // We keep bitToClear within [1, BITVEC_NBIT]
  return p;
}

// Test 1: Null pointer should return without performing any operation
static void test_null_pointer() {
  // Purpose: Ensure a null Bitvec pointer does not crash and simply returns.
  sqlite3BitvecClear(nullptr, 1, nullptr);
  // If we reach here without crash, test passes (no state to validate in this test).
  CHECK(true, "Null pointer call should not crash (p == nullptr)"); // always true, signifies test executed
}

// Test 2: Clear a bit in a small bitmap (iDivisor == 0, iSize <= BITVEC_NBIT)
static void test_clear_bit_in_bitmap() {
  // Arrange: create a Bitvec with a bitmap and ensure bit 0 is set
  Bitvec *p = make_bitmap_simple(1);
  if (!p) {
    std::cerr << "Memory allocation failure in test_clear_bit_in_bitmap" << std::endl;
    ++tests_failed;
    return;
  }

  // Capture before-state to verify the bit cleared
  uint32_t before = p->u.aBitmap[0];

  // Act: clear bit corresponding to i == 1
  sqlite3BitvecClear(p, 1, nullptr);

  // Assert: bit 0 should be cleared; other bits may remain unchanged
  uint32_t after = p->u.aBitmap[0];
  uint32_t expected = before & ~(1u << 0);
  CHECK(after == expected, "bitmap clear: bit 0 cleared while other bits preserved");

  // Cleanup
  free(p);
}

// Test 3: Clear path for large Bitvec (iSize > BITVEC_NBIT) ensuring the aHash rebuild path is exercised
static void test_clear_large_bitvec_hash_rebuild() {
  // Arrange: set up a Bitvec with iDivisor not used, iSize > BITVEC_NBIT to trigger hash path
  Bitvec *p = (Bitvec*)malloc(sizeof(Bitvec));
  if (!p) {
    std::cerr << "Memory allocation failure in test_clear_large_bitvec_hash_rebuild" << std::endl;
    ++tests_failed;
    return;
  }
  std::memset(p, 0, sizeof(Bitvec));

  p->iDivisor = 0;
  p->iSize = BITVEC_NBIT + 1; // force hash path
  p->nSet = 0;

  // Initialize aHash with two values that should be copied back into aHash after clear
  // Use values that are nonzero and not equal to (i+1) for i == 1 -> (i+1) == 2
  p->u.aHash[0] = 7;
  p->u.aHash[1] = 20;
  for (size_t idx = 2; idx < BITVEC_NINT; ++idx) {
    p->u.aHash[idx] = 0;
  }

  // Prepare buffer to capture the copy of aHash before modification
  u32 pBuf[BITVEC_NINT];
  // Act: clear i == 1
  sqlite3BitvecClear(p, 1, pBuf);

  // Assert:
  //  - The original aHash values should have been copied into pBuf
  CHECK(pBuf[0] == 7 && pBuf[1] == 20, "large-hash-path: initial aHash values copied to pBuf");

  //  - nSet should reflect two inserted values (7 and 20)
  CHECK(p->nSet == 2, "large-hash-path: nSet == 2 after rebuilding aHash");

  //  - The value '2' (i+1) should not appear in the rebuilt aHash
  bool contains_i_plus_1 = false;
  for (size_t k = 0; k < BITVEC_NINT; ++k) {
    if (p->u.aHash[k] == 2) { contains_i_plus_1 = true; break; }
  }
  CHECK(!contains_i_plus_1, "large-hash-path: rebuilt aHash should not contain i+1 (which is 2)");

  // Optional: ensure at least one nonzero value present after rebuild
  bool has_nonzero = false;
  for (size_t k = 0; k < BITVEC_NINT; ++k) {
    if (p->u.aHash[k] != 0) { has_nonzero = true; break; }
  }
  CHECK(has_nonzero, "large-hash-path: rebuilt aHash contains nonzero entries");

  // Cleanup
  free(p);
}

// Test 4: Pre-traversal null path: iDivisor != 0, but apSub[bin] is NULL causing early return
static void test_pre_traversal_null_subnode() {
  // Arrange: root Bitvec with iDivisor forcing traversal, but subnode is NULL
  Bitvec *root = (Bitvec*)malloc(sizeof(Bitvec));
  if (!root) {
    std::cerr << "Memory allocation failure in test_pre_traversal_null_subnode" << std::endl;
    ++tests_failed;
    return;
  }
  std::memset(root, 0, sizeof(Bitvec));

  root->iDivisor = 1; // ensure while loop executes
  // Ensure apSub[0] is NULL to trigger early return
  root->u.apSub[0] = NULL;

  // Act: with i = 1 (0 after decrement), the first step will attempt to traverse and hit NULL
  sqlite3BitvecClear(root, 1, nullptr);

  // If no crash occurred, the path is handled safely
  CHECK(true, "pre-traversal-null-subnode: function handles NULL subnode without crash");

  // Cleanup
  free(root);
}

int main() {
  std::cout << "Starting sqlite3BitvecClear unit tests (no GTest)." << std::endl;

  test_null_pointer();
  test_clear_bit_in_bitmap();
  test_clear_large_bitvec_hash_rebuild();
  test_pre_traversal_null_subnode();

  // Report summary
  std::cout << "Tests run: " << (tests_passed + tests_failed)
            << ", Passed: " << tests_passed
            << ", Failed: " << tests_failed << std::endl;

  // Exit with code 0 even if some tests fail to allow CI to continue collecting results
  return (tests_failed == 0) ? 0 : 1;
}