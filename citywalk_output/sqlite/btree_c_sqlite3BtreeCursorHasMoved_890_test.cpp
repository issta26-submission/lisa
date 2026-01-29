/*
  Test suite for the focal method:
  int sqlite3BtreeCursorHasMoved(BtCursor *pCur)

  Notes:
  - The test is written in C++11 (no GTest). It directly exercises the C function
    from the SQLite btree module as provided in the repository.
  - The tests rely on the actual BtCursor type and CURSOR_VALID macro exposed by
    btreeInt.h (as per the dependencies in <FOCAL_CLASS_DEP>).
  - We ensure 8-byte alignment for BtCursor instances to satisfy the EIGHT_BYTE_ALIGNMENT
    assertion in sqlite3BtreeCursorHasMoved.
  - We test both branches of the predicate:
      CURSOR_VALID != *(u8*)pCur
    by setting the first byte (the eState field) to CURSOR_VALID and to a different
    value, respectively.
  - Static and internal helpers in the production file are not mocked; only public C APIs
    are used to keep tests faithful to the real behavior.

  Implementation details:
  - We use a lightweight non-terminating test harness (no aborts on failures) with
    simple pass/fail counting to maximize coverage and execution.
  - Domain knowledge considerations:
    - The first member of BtCursor must be eState (as asserted by offsetof).
    - We explicitly ensure 8-byte alignment for the BtCursor instance to satisfy
      EIGHT_BYTE_ALIGNMENT(pCur) condition.
  - Domain constraints also imply we should avoid private fields/methods; we only rely on
    public interfaces exposed via btreeInt.h.

  Candidate Keywords extracted (core dependencies):
  - BtCursor, eState, CURSOR_VALID, sqlite3BtreeCursorHasMoved, sqlite3BtreeFakeValidCursor,
    EIGHT_BYTE_ALIGNMENT, offsetof, sizeof(pCur->eState), u8, CURSOR_VALID, pointer casts
*/

#include <type_traits>
#include <btreeInt.h>
#include <cstring>
#include <iostream>
#include <cstdint>


// Bring in the C definitions from the SQLite btree module
extern "C" {
  // Function under test
  int sqlite3BtreeCursorHasMoved(BtCursor *pCur);
  // Helper exposed by the same module (not used directly in tests to avoid coupling)
  BtCursor *sqlite3BtreeFakeValidCursor(void);
}

// Lightweight test harness (non-terminating; prints results and aggregates stats)
static int g_tests_run = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define TEST_OK(cond, label) do { \
  ++g_tests_run; \
  if (cond) { \
    ++g_tests_passed; \
  } else { \
    ++g_tests_failed; \
    std::cerr << "Test FAILED: " << label << std::endl; \
  } \
} while(0)

static void test_cursor_has_moved_eState_eq_cursor_valid()
{
  // Ensure 8-byte alignment for the BtCursor instance
  alignas(8) BtCursor cur;
  // Zero-initialize to satisfy offsetof(BtCursor, eState) == 0 and sizeof(eState) == 1
  std::memset(&cur, 0, sizeof(BtCursor));

  // Set the eState to CURSOR_VALID to exercise the false-branch of CURSOR_VALID != *(u8*)pCur
  cur.eState = (unsigned char)CURSOR_VALID;

  // Call the function under test
  int moved = sqlite3BtreeCursorHasMoved(&cur);

  // Expected: CURSOR_VALID != CURSOR_VALID -> 0
  int expected = (CURSOR_VALID != (unsigned char)cur.eState);

  TEST_OK(moved == expected,
          "sqlite3BtreeCursorHasMoved should return 0 when eState equals CURSOR_VALID");
}

static void test_cursor_has_moved_eState_not_eq_cursor_valid()
{
  // Ensure 8-byte alignment for the BtCursor instance
  alignas(8) BtCursor cur;
  std::memset(&cur, 0, sizeof(BtCursor));

  // Set the eState to a value different from CURSOR_VALID to exercise the true-branch
  // Use 0 to represent a non-valid state (as static initialization above uses 0)
  cur.eState = 0;

  // Call the function under test
  int moved = sqlite3BtreeCursorHasMoved(&cur);

  // Expected: CURSOR_VALID != 0
  int expected = (CURSOR_VALID != (unsigned char)cur.eState);

  TEST_OK(moved == expected,
          "sqlite3BtreeCursorHasMoved should return non-zero when eState != CURSOR_VALID");
}

int main()
{
  // Run tests
  test_cursor_has_moved_eState_eq_cursor_valid();
  test_cursor_has_moved_eState_not_eq_cursor_valid();

  // Summary
  std::cout << "Test Summary: "
            << "Total=" << g_tests_run
            << ", Passed=" << g_tests_passed
            << ", Failed=" << g_tests_failed
            << std::endl;

  // Return non-zero if any test failed
  return (g_tests_failed == 0) ? 0 : 1;
}