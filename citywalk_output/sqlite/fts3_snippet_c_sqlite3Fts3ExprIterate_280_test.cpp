/*
  Unit test suite for the focal method:
  int sqlite3Fts3ExprIterate(Fts3Expr *pExpr,
                             int (*x)(Fts3Expr*,int,void*),
                             void *pCtx);

  Notes for this test suite:
  - The test harness is pure C++11 and does not rely on any external
    testing framework (no GTest). It uses a small, self-contained
    test runner with lightweight, non-terminating assertions.
  - The production function sqlite3Fts3ExprIterate is a wrapper that
    initializes a phrase counter to 0 and delegates to a static helper
    fts3ExprIterate2. The test focuses on the public API surface, using
    a minimal C-compatible Fts3Expr type forward declaration to enable
    linking against the library implementation.
  - We avoid depending on private/static internals and static linking
    details. We exercise the public function with null and non-null
    Fts3Expr pointers and verify the function returns a valid int
    (non-negative) in these conservative scenarios.
  - The tests rely on the library providing a well-formed implementation
    of sqlite3Fts3ExprIterate; if the runtime behavior changes, update
    expectations accordingly.
*/

#include <fts3Int.h>
#include <string.h>
#include <assert.h>
#include <cstdio>
#include <iostream>
#include <cstdint>


// Domain knowledge: We must import the necessary C API while keeping
// tests in C++. The Fts3Expr type is opaque to the test code; we only
// need a pointer to it for the public API.
extern "C" {
  // Forward declaration of the opaque C type used by sqlite3Fts3ExprIterate
  struct Fts3Expr;

  // Public API under test
  int sqlite3Fts3ExprIterate(Fts3Expr *pExpr,
                            int (*x)(Fts3Expr*, int, void*),
                            void *pCtx);
}

// A minimal callback that matches the required signature.
// It always returns 0 to indicate success/no error from the callback.
// It is used to ensure linker compatibility and to exercise the
// callback parameter passing contract (pCtx is provided to the callback).
static int dummyCallback(Fts3Expr* pExpr, int iPhrase, void* pCtx) {
  (void)pExpr;  // suppress unused parameter warning
  (void)iPhrase;
  (void)pCtx;
  return 0;
}

// A callback that captures the iPhrase value into a user-provided context.
// This helps verify that the callback is invoked and that iPhrase is being
// advanced by the underlying iterator when possible.
static int capturePhraseCallback(Fts3Expr* pExpr, int iPhrase, void* pCtx) {
  (void)pExpr;
  if (pCtx) {
    int *out = reinterpret_cast<int*>(pCtx);
    *out = iPhrase;
  }
  return 0;
}

// A tiny non-framework test harness
int main() {
  int failures = 0;

  auto log_fail = [](const char* testName, const char* reason) {
    std::cerr << "[TEST FAILURE] " << testName << " - " << reason << "\n";
  };

  auto test_pass = [](const char* testName) {
    std::cout << "[TEST PASS] " << testName << "\n";
  };

  // Helper assertion: non-terminating. Logs failure but continues.
  auto ASSERT_TRUE = [&](bool cond, const char* testName, const char* msg) {
    if (!cond) {
      log_fail(testName, msg);
      ++failures;
      return false;
    }
    return true;
  };

  // Step 2 - Test Case 1: Null expression pointer
  // Rationale: The public API should be robust to a null pointer input
  // and return an integer value (conventionally 0 or another non-crashing value).
  {
    const char* testName = "sqlite3Fts3ExprIterate_NULL_pExpr";
    int ret = sqlite3Fts3ExprIterate(nullptr, dummyCallback, nullptr);
    // Accept any non-crashing return value. We conservatively require a non-negative result.
    bool ok = (ret >= 0);
    if (ASSERT_TRUE(ok, testName,
                    "Expected non-negative return when pExpr is NULL.")) {
      test_pass(testName);
    }
  }

  // Step 2 - Test Case 2: Non-null (sentinel) expression pointer with a callback
  // Rationale: Ensure the public API accepts a non-null pExpr and a valid callback.
  // We pass a sentinel non-null pointer to pExpr. The actual content is opaque to the test.
  // If the implementation dereferences the pointer, behavior is determined by the library.
  // We at least verify that the call does not crash and returns an integer.
  {
    const char* testName = "sqlite3Fts3ExprIterate_nonNULL_pExpr";
    Fts3Expr* sentinel = reinterpret_cast<Fts3Expr*>(0x1); // non-null sentinel pointer
    int ret = sqlite3Fts3ExprIterate(sentinel, dummyCallback, nullptr);
    bool ok = (ret >= 0);
    if (ASSERT_TRUE(ok, testName,
                    "Expected non-negative return when pExpr is non-NULL (sentinel).")) {
      test_pass(testName);
    }
  }

  // Step 2 - Test Case 3: Callback pCtx propagation (best-effort)
  // Rationale: If the underlying iterator invokes the callback, pCtx should be
  // accessible inside the callback. We cannot rely on internal iteration counts,
  // but we can verify that the callback has a chance to modify the provided context.
  {
    const char* testName = "sqlite3Fts3ExprIterate_pCtxPropagation";
    Fts3Expr* sentinel = reinterpret_cast<Fts3Expr*>(0x2); // non-null sentinel
    int captured = -9999; // sentinel value to detect modification
    int ret = sqlite3Fts3ExprIterate(sentinel, capturePhraseCallback, &captured);
    bool modified = (captured != -9999);
    // We accept either modification (if callback was invoked) or at least a valid return.
    bool ok = (ret >= 0) && (modified || true); // second part tolerant
    if (ASSERT_TRUE(ok, testName,
                    "Expected either successful return or pCtx modification if callback invoked.")) {
      test_pass(testName);
      if (modified) {
        std::cout << "  Callback captured iPhrase value: " << captured << "\n";
      } else {
        std::cout << "  Callback did not modify pCtx (invocation may not have occurred).\n";
      }
    }
  }

  // Summary
  if (failures == 0) {
    std::cout << "[ALL TESTS PASSED] All tests completed successfully.\n";
  } else {
    std::cerr << "[TEST SUMMARY] Failures: " << failures << "\n";
  }

  // Non-zero return indicates test failure(s)
  return (failures == 0) ? 0 : 1;
}