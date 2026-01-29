// Lightweight C++11 unit tests for sqlite3AuthContextPush (no GTest required)
//
// This test harness calls the C function sqlite3AuthContextPush from the auth.c
// file against the real SQLite internal types (Parse, AuthContext) provided by
// sqliteInt.h. It verifies the correct mutation of the context and parse objects
// according to the function's contract.
//
// Notes:
// - We do not test the assert(pParse) branch (as asserts abort execution) but
//   focus on the true-path behavior.
// - Tests are designed to be non-terminating on failures (they continue to
//   execute and report all failures).
// - A minimal test runner is implemented to keep dependencies low and avoid GTest.

#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Include the internal SQLite definitions used by sqlite3AuthContextPush.
// This header provides definitions for Parse, AuthContext, and related fields.

extern "C" {
  // Declare the function under test with C linkage to match the implementation in auth.c
  void sqlite3AuthContextPush(Parse *pParse, AuthContext *pContext, const char *zContext);
}

// Simple non-GTest style test harness
static int g_failures = 0;

// Helper: safe string equality (handles NULLs)
static bool str_eq_safe(const char* a, const char* b) {
  if (a == nullptr && b == nullptr) return true;
  if (a == nullptr || b == nullptr) return false;
  return std::strcmp(a, b) == 0;
}

// Assertion helpers (non-terminating)
#define ASSERT_PTR_EQ(a, b, msg)                                           \
  do {                                                                     \
    if ((void*)(a) != (void*)(b)) {                                      \
      std::cerr << "[FAIL] " << __func__ << ": " << (msg)              \
                << " | Expected pointer " << (void*)(b)                \
                << " but got " << (void*)(a) << " (address mismatch).\n"; \
      ++g_failures;                                                       \
    }                                                                      \
  } while (0)

#define ASSERT_STR_EQ(a, b, msg)                                           \
  do {                                                                     \
    if (!str_eq_safe((a), (b))) {                                        \
      std::cerr << "[FAIL] " << __func__ << ": " << (msg)                 \
                << " | Expected \"" << (b) << "\" but got \""                 \
                << (a ? a : "NULL") << "\".\n";                          \
      ++g_failures;                                                       \
    }                                                                      \
  } while (0)

#define ASSERT_TRUE(cond, msg)                                               \
  do {                                                                       \
    if (!(cond)) {                                                           \
      std::cerr << "[FAIL] " << __func__ << ": " << (msg) << "\n";           \
      ++g_failures;                                                         \
    }                                                                        \
  } while (0)

// Test 1: Basic push should set pContext->pParse, capture old zAuthContext, and update pParse->zAuthContext
static void test_basic_push() {
  // Prepare objects (value-initialized to avoid uninitialized fields)
  Parse pParse = {};
  pParse.zAuthContext = "OLD_CTX";

  AuthContext ctx = {};
  ctx.pParse = nullptr;
  ctx.zAuthContext = nullptr;

  // Act
  sqlite3AuthContextPush(&pParse, &ctx, "NEW_CTX");

  // Assert
  ASSERT_PTR_EQ(ctx.pParse, &pParse, "ctx.pParse should point to the same pParse instance");
  ASSERT_STR_EQ(ctx.zAuthContext, "OLD_CTX", "ctx.zAuthContext should capture the previous pParse->zAuthContext");
  ASSERT_STR_EQ(pParse.zAuthContext, "NEW_CTX", "pParse.zAuthContext should be updated to the new context string");
}

// Test 2: Push with NULL zContext should set pParse->zAuthContext to NULL and preserve previous pParse value in context
static void test_push_with_null_zContext() {
  // Prepare objects
  Parse pParse = {};
  pParse.zAuthContext = "OLD";

  AuthContext ctx = {};
  ctx.pParse = nullptr;
  ctx.zAuthContext = nullptr;

  // Act: push with NULL zContext
  sqlite3AuthContextPush(&pParse, &ctx, nullptr);

  // Assert
  ASSERT_PTR_EQ(ctx.pParse, &pParse, "ctx.pParse should point to the same pParse instance");
  ASSERT_STR_EQ(ctx.zAuthContext, "OLD", "ctx.zAuthContext should capture the previous pParse->zAuthContext");
  ASSERT_TRUE(pParse.zAuthContext == nullptr, "pParse.zAuthContext should be NULL after pushing NULL zContext");
}

// Test runner
static void run_all_tests() {
  test_basic_push();
  test_push_with_null_zContext();

  // Summary
  if (g_failures == 0) {
    std::cout << "[OK] All tests passed.\n";
  } else {
    std::cerr << "[WARN] Tests finished with " << g_failures << " failure(s).\n";
  }
}

int main() {
  run_all_tests();
  return (g_failures == 0) ? 0 : 1;
}