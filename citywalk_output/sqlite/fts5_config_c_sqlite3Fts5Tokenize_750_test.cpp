// Lightweight C++11 test suite for sqlite3Fts5Tokenize (no GTest).
// This test focuses on the focal method behavior using a minimal, self-contained harness.
// The tests are designed to run in a typical C/C++ environment where the real
// sqlite3Fts5Tokenize is linked from fts5_config.c, and a mocked tokenizer is provided.
// Explanatory comments accompany each test case.

#include <vector>
#include <fts5Int.h>
#include <iomanip>
#include <string>
#include <cstring>
#include <iostream>


// ------------------------------------------------------------------------------------
// Lightweight test framework (non-terminating assertions)
// ------------------------------------------------------------------------------------
static int g_totalTests = 0;
static int g_totalFailures = 0;

#define EXPECT_TRUE(cond) \
  do { \
    ++g_totalTests; \
    if(!(cond)) { \
      ++g_totalFailures; \
      std::cerr << "EXPECT_TRUE failed at " << __LINE__ << " in " << __FUNCTION__ << "\n"; \
    } \
  } while(0)

#define EXPECT_EQ(a, b) \
  do { \
    ++g_totalTests; \
    if(!((a) == (b))) { \
      ++g_totalFailures; \
      std::cerr << "EXPECT_EQ failed: " #a " (" << (a) << ") != " #b " (" << (b) << ") at line " << __LINE__ << "\n"; \
    } \
  } while(0)

#define EXPECT_STR_EQ(a, b) \
  do { \
    ++g_totalTests; \
    if(std::strcmp((a), (b)) != 0) { \
      ++g_totalFailures; \
      std::cerr << "EXPECT_STR_EQ failed: \"" << (a) << "\" != \"" << (b) << "\" at line " << __LINE__ << "\n"; \
    } \
  } while(0)

#define TEST_REPORT() \
  do { \
    std::cout << "Tests run: " << g_totalTests << ", Failures: " << g_totalFailures << "\n"; \
  } while(0)

// ------------------------------------------------------------------------------------
// Minimal type/mocks to compile against the focal function's signature.
// We assume the following simplified definitions align with fts5_config.c expectations.
// This enables isolated unit testing without requiring the full project build.
// ------------------------------------------------------------------------------------

// Define SQLITE_OK if not provided by sqlite headers
#ifndef SQLITE_OK
#define SQLITE_OK 0
#endif

// Forward-declare the structures expected by sqlite3Fts5Tokenize
typedef struct Fts5TokApi Fts5TokApi;

typedef struct Fts5Config {
  void* pTok;
  Fts5TokApi* pTokApi;
} Fts5Config;

// Tokenizer API interface used by sqlite3Fts5Tokenize
struct Fts5TokApi {
  int (*xTokenize)(void*, void*, int, const char*, int, int (*)(void*, int, const char*, int, int, int));
};

// Prototype of the focal function (C linkage)
extern "C" int sqlite3Fts5Tokenize(
  Fts5Config *pConfig,
  int flags,
  const char *pText, int nText,
  void *pCtx,
  int (*xToken)(void*, int, const char*, int, int, int)
);

// ------------------------------------------------------------------------------------
// Test harness: a fake tokenizer and token collector to verify behavior.
// ------------------------------------------------------------------------------------

struct TokenAccumulator {
  std::vector<std::string> tokens;
};

// Mock xToken callback: collects tokens into TokenAccumulator
static int test_xToken(void* pCtx, int iTok, const char* z, int n, int a, int b) {
  TokenAccumulator* acc = static_cast<TokenAccumulator*>(pCtx);
  acc->tokens.emplace_back(z, z + n);
  return 0;
}

// Mock xTokenize implementation: splits input on spaces and triggers xToken for each token.
// Special case: if the input equals "fail" (4 chars), return a non-zero code (123)
// to exercise non-OK propagation.
static int test_xTokenize(void* pTok, void* pCtx, int flags,
                          const char* pText, int nText,
                          int (*xToken)(void*, int, const char*, int, int, int)) {
  // Simulate an early failure when the entire input is "fail"
  if (nText == 4 && std::strncmp(pText, "fail", 4) == 0) {
    return 123;
  }

  // If there is a callback, tokenize by spaces (ignore empty tokens).
  if (xToken) {
    int start = 0;
    for (int i = 0; i <= nText; ++i) {
      if (i == nText || pText[i] == ' ') {
        int len = i - start;
        if (len > 0) {
          int rc = xToken(pCtx, i, pText + start, len, 0, 0);
          if (rc) return rc;
        }
        start = i + 1;
      }
    }
  }
  return 0;
}

// Helper to set up a minimal Fts5Config with provided tokenization API
static void setup_config(Fts5Config* pCfg, Fts5TokApi* pApi) {
  pCfg->pTok = (void*)0x1; // arbitrary non-null
  pCfg->pTokApi = pApi;
}

// ------------------------------------------------------------------------------------
// Test cases
// ------------------------------------------------------------------------------------

// 1) Null text should return SQLITE_OK and call nothing on xToken
static void test_null_text() {
  TokenAccumulator acc;
  Fts5TokApi api = { test_xTokenize };
  Fts5Config cfg;
  setup_config(&cfg, &api);

  int rc = sqlite3Fts5Tokenize(&cfg, 0, nullptr, 0, &acc, test_xToken);
  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_TRUE(acc.tokens.empty()); // no tokens should be produced
}

// 2) Basic tokenization: "hello world" should yield ["hello", "world"]
static void test_basic_tokenization() {
  TokenAccumulator acc;
  Fts5TokApi api = { test_xTokenize };
  Fts5Config cfg;
  setup_config(&cfg, &api);

  const char* txt = "hello world";
  int rc = sqlite3Fts5Tokenize(&cfg, 0, txt, (int)std::strlen(txt), &acc, test_xToken);
  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_EQ(acc.tokens.size(), 2);
  EXPECT_EQ(acc.tokens[0], std::string("hello"));
  EXPECT_EQ(acc.tokens[1], std::string("world"));
}

// 3) Non-zero return from xTokenize propagates to sqlite3Fts5Tokenize
static void test_tokenize_failure_propagates() {
  TokenAccumulator acc;
  Fts5TokApi api = { test_xTokenize };
  Fts5Config cfg;
  setup_config(&cfg, &api);

  const char* txt = "fail";
  int rc = sqlite3Fts5Tokenize(&cfg, 0, txt, (int)std::strlen(txt), &acc, test_xToken);
  EXPECT_EQ(rc, 123);
  EXPECT_TRUE(acc.tokens.empty()); // no tokens should be produced due to early failure
}

// 4) Empty string with non-null pointer should return SQLITE_OK and produce no tokens
static void test_empty_string_with_nonnull_ptr() {
  TokenAccumulator acc;
  Fts5TokApi api = { test_xTokenize };
  Fts5Config cfg;
  setup_config(&cfg, &api);

  const char* txt = "";
  int rc = sqlite3Fts5Tokenize(&cfg, 0, txt, 0, &acc, test_xToken);
  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_TRUE(acc.tokens.empty());
}

// 5) Tokenization with multiple spaces should ignore empty tokens and yield correct tokens
static void test_multiple_spaces_tokenization() {
  TokenAccumulator acc;
  Fts5TokApi api = { test_xTokenize };
  Fts5Config cfg;
  setup_config(&cfg, &api);

  const char* txt = "a  b";
  int rc = sqlite3Fts5Tokenize(&cfg, 0, txt, (int)std::strlen(txt), &acc, test_xToken);
  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_EQ(acc.tokens.size(), 2);
  EXPECT_EQ(acc.tokens[0], std::string("a"));
  EXPECT_EQ(acc.tokens[1], std::string("b"));
}

// ------------------------------------------------------------------------------------
// Main entry point to run all tests
// ------------------------------------------------------------------------------------
int main() {
  std::cout << "Running sqlite3Fts5Tokenize unit tests (C++11 harness)..." << std::endl;

  test_null_text();
  test_basic_tokenization();
  test_tokenize_failure_propagates();
  test_empty_string_with_nonnull_ptr();
  test_multiple_spaces_tokenization();

  TEST_REPORT();

  if (g_totalFailures == 0) {
    std::cout << "All tests PASSED." << std::endl;
    return 0;
  } else {
    std::cout << g_totalFailures << " test(s) FAILED." << std::endl;
    return 1;
  }
}

// Note: The actual sqlite3Fts5Tokenize function is implemented in fts5_config.c.
// The above tests assume it is linked with the correct signature and uses
// the provided mocked XTokenize path for verification.
// The tests intentionally avoid terminating on assertion failures to maximize
// code coverage and allow multiple scenarios to execute in a single run.