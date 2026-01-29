/*
  Unit test suite for sqlite3Fts3EvalPhraseCleanup (fts3.c)
  - Implemented in C, tested from C++11 harness without GoogleTest.
  - Uses real fts3.h types to exercise the focal method via the actual
    translation unit fts3.c when linking the test binary.
  - Provides lightweight test harness with non-terminating assertions.
  - Includes stub for sqlite3_free to enable observation of freeing behavior
    by tracking the argument passed to sqlite3_free (pointer equality with a
    globally tracked expected pointer).
  - Tests cover:
    1) null input
    2) non-null input with a non-empty doclist and two tokens (verifies doclist reset,
       token segcsr pointers reset, and free path invocation)
    3) non-null input with zero tokens (ensures proper zeroing behavior and no crash)
  - The tests rely on the public header(s) (fts3.h) to define Fts3Phrase and related
    structures used by sqlite3Fts3EvalPhraseCleanup.
  - Note: This harness assumes fts3.c and fts3.h are available in the build environment.

  Build and run guidance (not part of the code):
  - g++ -std=c++11 -I<path_to_includes> test_fts3_cleanup.cpp fts3.c -o test_fts3_cleanup
  - ./test_fts3_cleanup
*/

#include <fts3Int.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <fts3.h>
#include <assert.h>
#include <cstdio>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <cstdlib>
#include <cstdint>


// Include the FTS3 related types and function under test.
// This header is expected to provide definitions for Fts3Phrase, Fts3Doclist,
// and declarations for sqlite3Fts3EvalPhraseCleanup, etc.

extern "C" {

// Forward declaration to satisfy C linkage in C++ test file.
// We provide a simple mock for sqlite3_free to observe which pointer gets freed.
void sqlite3_free(void*);

// Global state to observe free() invocations on the test doclist pointer.
static void* g_expected_free_ptr = nullptr;
static bool g_doclist_freed = false;

// A small wrapper to intercept sqlite3_free calls.
// If the freed pointer matches the expected doclist aAll pointer, mark it observed.
void sqlite3_free(void* p) {
    if (p == g_expected_free_ptr) {
        g_doclist_freed = true;
    }
    // Use the standard free() to release memory back to the system.
    std::free(p);
}
} // extern "C"

// Lightweight test harness (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define CHECK(expr, msg) do { \
    ++g_tests_run; \
    if(!(expr)) { \
        ++g_tests_failed; \
        std::fprintf(stderr, "TEST FAILED: %s\n", (msg)); \
    } \
} while(0)

#define RUN_TEST(func) do { func(); } while(0)

// Test 1: Null input should be a no-op and not crash.
static void test_null_input() {
    // Reset state
    g_doclist_freed = false;
    g_expected_free_ptr = nullptr;
    // Call the focal function with a null pointer
    sqlite3Fts3EvalPhraseCleanup(nullptr);
    // No assertion required beyond ensuring no crash occurs.
    CHECK(true, "Null input should not crash (no-op).");
}

// Test 2: Non-null input with a doclist pointer and two tokens.
// Validates: doclist is freed, doclist becomes zeroed, and aToken[].pSegcsr pointers
// are reset to NULL. Also asserts that the sqlite3_free path was invoked for the doclist aAll.
static void test_cleanup_with_doclist_and_tokens() {
    // We'll construct a Fts3Phrase with a flexible array member for tokens.
    const int nToken = 2;
    // size = sizeof(Fts3Phrase) + (nToken - 1) * sizeof(Fts3PhraseToken)
    size_t size = sizeof(Fts3Phrase) + (nToken - 1) * sizeof(Fts3PhraseToken);
    Fts3Phrase* p = (Fts3Phrase*) std::malloc(size);
    // Initialize basic fields
    p->nToken = nToken;

    // Allocate and assign a doclist.aAll pointer
    char* doc = (char*) std::malloc(32);
    // Note: Fts3Doclist is defined inside fts3.h; we assume it has member aAll
    p->doclist.aAll = doc;

    // Track the expected pointer for sqlite3_free interception
    g_expected_free_ptr = (void*)doc;
    g_doclist_freed = false;

    // Initialize tokens' pSegcsr to NULL to keep the test simple and safe
    for (int i = 0; i < nToken; ++i) {
        p->aToken[i].pSegcsr = nullptr;
    }

    // Execute the focal function
    sqlite3Fts3EvalPhraseCleanup(p);

    // Post-conditions:
    //  - doclist.aAll should be NULL after cleanup (due to memset)
    //  - all pSegcsr fields should be set to NULL
    //  - sqlite3_free should have been invoked for the original doc pointer
    CHECK(p->doclist.aAll == nullptr, "doclist.aAll should be NULL after cleanup (memory freed and struct zeroed).");
    CHECK(p->aToken[0].pSegcsr == nullptr, "pSegcsr for token 0 should be NULL after cleanup.");
    CHECK(p->aToken[1].pSegcsr == nullptr, "pSegcsr for token 1 should be NULL after cleanup.");
    CHECK(g_doclist_freed, "sqlite3_free should be invoked for doclist.aAll during cleanup.");

    // Clean up the phrase structure
    std::free(p);
}

// Test 3: Non-null input with zero tokens but with a non-null doclist pointer.
// Ensures no crash and that doclist gets zeroed as per the code path, and free called.
static void test_cleanup_doclist_with_no_tokens() {
    const int nToken = 0;
    size_t size = sizeof(Fts3Phrase) + (nToken - 1) * sizeof(Fts3PhraseToken);
    Fts3Phrase* p = (Fts3Phrase*) std::malloc(size);
    p->nToken = nToken;

    char* doc = (char*) std::malloc(16);
    p->doclist.aAll = doc;

    g_expected_free_ptr = (void*)doc;
    g_doclist_freed = false;

    // Execute
    sqlite3Fts3EvalPhraseCleanup(p);

    // Post-conditions
    CHECK(p->doclist.aAll == nullptr, "doclist.aAll should be NULL after cleanup (nToken=0).");
    // No tokens to check
    CHECK(g_doclist_freed, "sqlite3_free should be invoked for doclist.aAll (nToken=0).");

    std::free(p);
}

// Entry point
int main() {
    // Run tests
    RUN_TEST(test_null_input);
    RUN_TEST(test_cleanup_with_doclist_and_tokens);
    RUN_TEST(test_cleanup_doclist_with_no_tokens);

    // Summary
    std::printf("FTS3 Cleanup Tests: ran=%d, failed=%d\n", g_tests_run, g_tests_failed);
    return g_tests_failed ? 1 : 0;
}