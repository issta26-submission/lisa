// Minimal C++11 test harness for the focal function lsmTreeCursorReset
// This test does not rely on any external testing framework (e.g., GTest)
// and uses handcrafted assertions to maximize executable coverage.

#include <cstdio>
#include <string.h>
#include <cstdint>


// Step 1-2: Provide minimal C-compatible declarations needed to link with the focal C function.
// The actual project defines TreeCursor in its C headers; for testing, we declare a compatible
// structure with the fields accessed by lsmTreeCursorReset.
// We intentionally keep this declaration minimal to avoid pulling in the entire project header graph.
extern "C" {
  // Minimal, test-focused representation of TreeCursor sufficient for lsmTreeCursorReset.
  struct TreeCursor {
    int iNode;   // Field accessed/modified by lsmTreeCursorReset
    void *pSave; // Field accessed/modified by lsmTreeCursorReset
  };

  // The function under test (as declared in the C codebase)
  void lsmTreeCursorReset(TreeCursor *pCsr);
}

// Simple test harness utilities
static int g_failed = 0;

#define ASSERT_INT_EQ(actual, expected, msg) \
  do { \
    if ((actual) != (expected)) { \
      std::fprintf(stderr, "ASSERT FAILED: %s | Expected %d, got %d\n", (msg), (expected), (actual)); \
      ++g_failed; \
    } \
  } while (0)

#define ASSERT_PTR_NULL(ptr, msg) \
  do { \
    if ((ptr) != nullptr) { \
      std::fprintf(stderr, "ASSERT FAILED: %s | Expected null pointer, got %p\n", (msg), (void*)(ptr)); \
      ++g_failed; \
    } \
  } while (0)

#define TEST_PASSED(msg) \
  do { \
    std::printf("TEST PASSED: %s\n", (msg)); \
  } while (0)

#define TEST_FAILED(msg) \
  do { \
    std::fprintf(stderr, "TEST FAILED: %s\n", (msg)); \
  } while (0)

// Step 3: Test cases

// Test 1: Non-null cursor should be reset:
// - iNode should become -1
// - pSave should become NULL (0)
// This exercise covers the true-branch inside lsmTreeCursorReset.
static void test_lsmTreeCursorReset_nonNullCursor()
{
  // Arrange: prepare a cursor with known non-zero state
  TreeCursor csr;
  csr.iNode = 42;
  csr.pSave = reinterpret_cast<void*>(0xDEADBEEF);

  // Act: call the function under test
  lsmTreeCursorReset(&csr);

  // Assert: verify both fields were reset as specified
  ASSERT_INT_EQ(csr.iNode, -1, "iNode should be reset to -1 when pCsr is non-null");
  ASSERT_PTR_NULL(csr.pSave, "pSave should be reset to NULL when pCsr is non-null");

  if (g_failed == 0) TEST_PASSED("test_lsmTreeCursorReset_nonNullCursor");
  else TEST_FAILED("test_lsmTreeCursorReset_nonNullCursor");
}

// Test 2: Null pointer should be handled gracefully (no operation, no crash):
// This covers the false-branch where the guard prevents dereferencing.
static void test_lsmTreeCursorReset_nullPointer()
{
  // Act: should be a no-op and not crash
  lsmTreeCursorReset(nullptr);

  // Since there is no state to verify (nullptr input), we simply ensure no crash occurred
  // by reaching this point. Report pass.
  TEST_PASSED("test_lsmTreeCursorReset_nullPointer (no crash)");
}

// Main entry: run tests and report aggregated results
int main() {
  // Run tests
  test_lsmTreeCursorReset_nonNullCursor();
  test_lsmTreeCursorReset_nullPointer();

  // Summary
  if (g_failed == 0) {
    std::printf("ALL TESTS PASSED (%d tests)\n", 2);
    return 0;
  } else {
    std::fprintf(stderr, "%d TEST(S) FAILED\n", g_failed);
    return 1;
  }
}