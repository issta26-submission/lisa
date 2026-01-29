/*
 * Unit tests for sqlite3Fts3DeferToken
 * Focus: validate correct linking and token association when deferring tokens
 * Notes:
 * - This test suite is written in C++11 but uses C style tests (no Google Test).
 * - It assumes the project headers for Fts3Cursor, Fts3PhraseToken, and Fts3DeferredToken
 *   are available in the include path (e.g., fts3Int.h or equivalent).
 * - We avoid terminating tests on failure by using a lightweight CHECK macro that logs results.
 * - The tests cover the main success path (no OOM) and verify the linked-list wiring and pDeferred relationships.
 * - The tests rely on the real sqlite3Fts3DeferToken implementation in the linked project.
 *
 * Structure:
 * - Test 1: Deferring a single token updates cursor head and token pointer correctly.
 * - Test 2: Deferring a second token pushes a new head and links to the previous head; both tokens point to their respective deferred nodes.
 *
 * Candidate Keywords (from the focal method and its dependencies):
 * - pCsr (Fts3Cursor*)
 * - pToken (Fts3PhraseToken*)
 * - pDeferred (Fts3DeferredToken*)
 * - iCol (int)
 * - pNext (Fts3DeferredToken*)
 * - pToken->pDeferred (Fts3DeferredToken*)
 * - sqlite3_malloc64 / SQLITE_OK / SQLITE_NOMEM (allocation and result codes)
 * - NULL checks / linking behavior
 * - Memory initialization via memset
 *
 * Domain notes:
 * - We only use public/visible APIs and types from the project (no private members accessed directly).
 * - Test harness prints PASS/FAIL lines and summarizes failures at the end.
 */

#include <fts3Int.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <cstdlib>


// The project should provide the Fts3Cursor, Fts3PhraseToken, and Fts3DeferredToken types.
// Include the header(s) that declare sqlite3Fts3DeferToken and related types.
// If the project uses fts3Int.h or fts3.h, adjust the include path accordingly.

// Ensure the function prototype is visible to the linker for testing.
extern "C" int sqlite3Fts3DeferToken(
  Fts3Cursor *pCsr,               /* Fts3 table cursor */
  Fts3PhraseToken *pToken,        /* Token to defer */
  int iCol                        /* Column that token must appear in (or -1) */
);

// Lightweight test harness (non-terminating assertions)
static int g_failures = 0;
static void LOG(const char* fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stdout, fmt, ap);
  va_end(ap);
}
#define CHECK(cond, msg) do { \
  if (cond) { \
    LOG("PASS: %s\n", msg); \
  } else { \
    LOG("FAIL: %s\n", msg); \
    ++g_failures; \
  } \
} while(0)

static void test_defer_token_single_deferral() {
  // Test: Deferring a single token should:
  // - Allocate a new Fts3DeferredToken
  // - Attach it to the cursor's pDeferred
  // - Set the token's pDeferred to the newly created node
  // - Record iCol and link pNext to previous head (NULL on first call)

  // Prepare structures (initial state: no deferred tokens)
  Fts3Cursor csr;
  csr.pDeferred = NULL;

  Fts3PhraseToken token;
  token.pDeferred = NULL;

  // Action: Defer token in column 3
  int rc = sqlite3Fts3DeferToken(&csr, &token, 3);

  CHECK(rc == 0, "sqlite3Fts3DeferToken returns SQLITE_OK on success (single token)");
  CHECK(csr.pDeferred != NULL, "Cursor now has a non-NULL pDeferred head after first defer");

  Fts3DeferredToken *pHead = csr.pDeferred;
  // Validate the head node fields
  CHECK(pHead->pToken == &token, "Head pDeferred's pToken points to the correct token (token1)");
  CHECK(pHead->iCol == 3, "Head pDeferred's iCol is correctly set to 3");
  CHECK(pHead->pNext == NULL, "Head pDeferred's pNext is NULL for the first defer");

  // Token should reference the same deferred node
  CHECK(token.pDeferred == pHead, "Token's pDeferred points to the head node (token1 linkage)");
}

static void test_defer_token_pushes_second_deferral() {
  // Test: Deferring a second token should push a new head and link to previous head
  // - New head's pNext should point to old head
  // - New head's pToken should point to the second token
  // - Second token's pDeferred should point to the new head

  // Setup initial state with first token deferred
  Fts3Cursor csr;
  csr.pDeferred = NULL;

  Fts3PhraseToken token1;
  token1.pDeferred = NULL;

  int rc1 = sqlite3Fts3DeferToken(&csr, &token1, 3);
  CHECK(rc1 == 0, "First defer (token1) returns SQLITE_OK");

  // Verify first state to ensure the second test starts from a known state
  CHECK(csr.pDeferred != NULL, "Cursor head exists after first defer (preparation for second defer)");
  Fts3DeferredToken *head1 = csr.pDeferred;
  Fts3PhraseToken token2;
  token2.pDeferred = NULL;

  // Action: Defer token2 in column -1
  int rc2 = sqlite3Fts3DeferToken(&csr, &token2, -1);
  CHECK(rc2 == 0, "Second defer (token2) returns SQLITE_OK");

  // New head should be at csr.pDeferred
  Fts3DeferredToken *head2 = csr.pDeferred;
  CHECK(head2 != NULL, "New head exists after second defer");
  CHECK(head2 != head1, "New head is different from the old head (new list head)");

  // Validate new head fields
  CHECK(head2->pToken == &token2, "New head pDeferred's pToken points to token2");
  CHECK(head2->iCol == -1, "New head pDeferred's iCol is -1 as specified");
  CHECK(head2->pNext == head1, "New head's pNext links to previous head (head1)");

  // Validate token linkage
  CHECK(token2.pDeferred == head2, "Second token's pDeferred points to the new head (token2 linkage)");
  // Also re-verify first token remains linked to the old head
  CHECK(token1.pDeferred == head1, "First token's pDeferred remains pointing to the old head (token1 linkage)");
}

int main(void) {
  LOG("Starting sqlite3Fts3DeferToken unit tests\n");

  test_defer_token_single_deferral();
  test_defer_token_pushes_second_deferral();

  if (g_failures == 0) {
    LOG("All sqlite3Fts3DeferToken tests PASSED\n");
    return 0;
  } else {
    LOG("sqlite3Fts3DeferToken tests finished with %d failure(s)\n", g_failures);
    return 1;
  }
}