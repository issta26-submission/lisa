// Unit test suite for sqlite3Fts3UnicodeTokenizer in fts3_unicode.c
// Target: C++11, no GoogleTest. Use a lightweight, non-terminating assertion style.
// The test focuses on validating the public API surface exposed by sqlite3Fts3UnicodeTokenizer
// (i.e., the sqlite3_tokenizer_module pointer it returns) without relying on private/internal
// implementations.

#include <fts3Int.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fts3_tokenizer.h>
#include <stdio.h>
#include <string>
#include <iostream>


// Forward declarations to mirror the sqlite3 tokenizer API used by sqlite3Fts3UnicodeTokenizer.
// We do not rely on external sqlite3 headers; we reproduce just enough to compile and test.
extern "C" {

  // Opaque types used by the tokenizer interface
  typedef struct sqlite3_tokenizer sqlite3_tokenizer;
  typedef struct sqlite3_tokenizer_cursor sqlite3_tokenizer_cursor;

  // Public module structure as used by sqlite3
  typedef struct sqlite3_tokenizer_module {
    int iVersion;
    int (*xCreate)(int, const char * const *, sqlite3_tokenizer **);
    int (*xDestroy)(sqlite3_tokenizer*);
    int (*xOpen)(sqlite3_tokenizer*, const char*, int, sqlite3_tokenizer_cursor**);
    int (*xClose)(sqlite3_tokenizer_cursor*);
    int (*xNext)(sqlite3_tokenizer_cursor*, const char **, int *, int *, int *, int *);
    int (*xLanguageid)(void);
  } sqlite3_tokenizer_module;

  // Focal function under test: initializes the tokenizer module and returns it via ppModule
  void sqlite3Fts3UnicodeTokenizer(sqlite3_tokenizer_module const **ppModule);
}

// Lightweight test harness (non-terminating assertions)
static int g_total_tests = 0;
static int g_failed_tests = 0;

#define DO_ASSERT(cond, msg) do { \
  ++g_total_tests; \
  if (!(cond)) { \
    std::cerr << "Test failed: " << (msg) << " (line " << __LINE__ << ")" << std::endl; \
    ++g_failed_tests; \
  } \
} while (0)

int main() {
  // Test 1: The tokenizer module is initialized and has the expected static layout
  // - ppModule should be non-null
  // - iVersion should be 0 (as defined in the static module initializer)
  // - xCreate, xDestroy, xOpen, xClose, xNext should be non-null
  // - xLanguageid should be null (0) as per the initializer
  {
    sqlite3_tokenizer_module const *pModule = nullptr;
    sqlite3Fts3UnicodeTokenizer(&pModule);

    DO_ASSERT(pModule != nullptr, "Module pointer must not be null after initialization");
    if (pModule != nullptr) {
      DO_ASSERT(pModule->iVersion == 0, "iVersion must be initialized to 0");
      DO_ASSERT(pModule->xCreate != nullptr, "xCreate must be non-null");
      DO_ASSERT(pModule->xDestroy != nullptr, "xDestroy must be non-null");
      DO_ASSERT(pModule->xOpen != nullptr, "xOpen must be non-null");
      DO_ASSERT(pModule->xClose != nullptr, "xClose must be non-null");
      DO_ASSERT(pModule->xNext != nullptr, "xNext must be non-null");
      DO_ASSERT(pModule->xLanguageid == nullptr, "xLanguageid should be nullptr (0)");
    }
  }

  // Test 2: Multiple invocations return the same module instance (singleton-like behavior)
  // - The module is defined as static inside the focal file, so repeated calls should yield
  //   the same addressable object.
  {
    sqlite3_tokenizer_module const *pModule1 = nullptr;
    sqlite3_tokenizer_module const *pModule2 = nullptr;
    sqlite3Fts3UnicodeTokenizer(&pModule1);
    sqlite3Fts3UnicodeTokenizer(&pModule2);

    DO_ASSERT(pModule1 != nullptr, "First module pointer must not be null");
    DO_ASSERT(pModule2 != nullptr, "Second module pointer must not be null");
    DO_ASSERT(pModule1 == pModule2, "Both calls should return the same module instance");
  }

  // Summary of results
  std::cout << "Total tests: " << g_total_tests << std::endl;
  std::cout << "Failed tests: " << g_failed_tests << std::endl;

  // Return non-zero if any test failed to signal issues to CI/build systems
  return (g_failed_tests == 0) ? 0 : 1;
}