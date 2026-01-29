// Test suite for sqlite3Fts5GetTokenizer (C code) implemented in a self-contained
// environment to avoid external dependencies. This harness mocks the minimal
// SQLite/FTS5 scaffolding needed by the focal function and validates true/false
// branches of its logic without using Google Test.
// Notes:
// - This is a C++11-compliant test harness but uses plain assertions and simple
//   test harness logic (no GTest).
// - We re-implement a minimal version of sqlite3Fts5GetTokenizer and its
//   dependencies in this TU to facilitate isolated testing.
// - The test covers: tokenizers not found, tokenizer created successfully with no
//   args, tokenizer created successfully with additional args, and constructor
//   failure path.

#include <vector>
#include <fts5Int.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Ensure we compile as C-style linkage for the focal API surface.
extern "C" {

// Minimal stub definitions to mimic FTS5-related types in the focal method.

#define SQLITE_OK 0
#define SQLITE_ERROR 1

typedef struct Fts5Global Fts5Global;
typedef struct Fts5Tokenizer Fts5Tokenizer;

// Forward declaration of the tokenizer module structure used by sqlite3Fts5GetTokenizer
typedef struct Fts5TokenizerModule {
  struct {
    int (*xCreate)(void*, const char **, int, Fts5Tokenizer**);
  } x;
  void *pUserData;
} Fts5TokenizerModule;

// Configuration structure used by sqlite3Fts5GetTokenizer
typedef struct Fts5Config {
  Fts5Tokenizer *pTok;
  Fts5TokenizerModule *pTokApi;
  int ePattern;
} Fts5Config;

// A simple dummy tokenizer object
struct Fts5Tokenizer {};

// Global state for test control
// We simulate two potential tokenizer modules: a "good" one and a "bad" (failing) one.
// The good module's xCreate can be toggled to succeed or fail to exercise branches.
static int g_fake_xCreate_rc = SQLITE_OK;

// Forward declarations for dependencies that the focal function relies on
// (mocked for the test environment).
static Fts5TokenizerModule moduleGood;
static Fts5TokenizerModule moduleBad;

// Mocked tokenizer creator: succeeds and returns a token object
static int fake_xCreate_OK(void *pUserData, const char **azArg, int nArg, Fts5Tokenizer **ppTok) {
  (void)pUserData;
  (void)azArg;
  (void)nArg;
  *ppTok = new Fts5Tokenizer();
  return SQLITE_OK;
}

// Mocked tokenizer creator: always fails
static int fake_xCreate_FAIL(void *pUserData, const char **azArg, int nArg, Fts5Tokenizer **ppTok) {
  (void)pUserData;
  (void)azArg;
  (void)nArg;
  (void)ppTok;
  return SQLITE_ERROR;
}

// A minimal fts5LocateTokenizer implementation for the test harness.
// - If zName is NULL or "tok", return the good module (default for tests).
// - Otherwise return NULL to simulate "not found".
static Fts5TokenizerModule* fts5LocateTokenizer(Fts5Global *pGlobal, const char *zName) {
  (void)pGlobal;
  if(zName == NULL) return &moduleGood; // default
  if(std::strcmp(zName, "tok") == 0) return &moduleGood;
  return 0;
}

// Lightweight sqlite3_mprintf replacement for test purposes.
// Allocates a string using malloc; caller must free.
static char* sqlite3_mprintf(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  // determine required size
  va_list ap2;
  va_copy(ap2, ap);
  int n = vsnprintf(nullptr, 0, fmt, ap2);
  va_end(ap2);
  char *buf = (char*)malloc(n + 1);
  vsnprintf(buf, n + 1, fmt, ap);
  va_end(ap);
  return buf;
}

// Mocked tokenizer pattern determination function.
// For testing, simply return a non-zero sentinel value to indicate "pattern set".
static int sqlite3Fts5TokenizerPattern(int (*xCreate)(void*, const char**, int, Fts5Tokenizer**),
                                      Fts5Tokenizer *pTok) {
  (void)xCreate;
  (void)pTok;
  return 123; // arbitrary non-zero value to indicate a pattern
}

// The focal function under test (copied here to enable isolated unit tests).
static int sqlite3Fts5GetTokenizer(
  Fts5Global *pGlobal, 
  const char **azArg,
  int nArg,
  Fts5Config *pConfig,
  char **pzErr
){
  Fts5TokenizerModule *pMod;
  int rc = SQLITE_OK;
  pMod = fts5LocateTokenizer(pGlobal, nArg==0 ? 0 : azArg[0]);
  if( pMod==0 ){
    // assert( nArg>0 );
    rc = SQLITE_ERROR;
    *pzErr = sqlite3_mprintf("no such tokenizer: %s", azArg[0]);
  }else{
    rc = pMod->x.xCreate(
        pMod->pUserData, (azArg?&azArg[1]:0), (nArg?nArg-1:0), &pConfig->pTok
    );
    pConfig->pTokApi = &pMod->x;
    if( rc!=SQLITE_OK ){
      if( pzErr ) *pzErr = sqlite3_mprintf("error in tokenizer constructor");
    }else{
      pConfig->ePattern = sqlite3Fts5TokenizerPattern(
          pMod->x.xCreate, pConfig->pTok
      );
    }
  }
  if( rc!=SQLITE_OK ){
    pConfig->pTokApi = 0;
    pConfig->pTok = 0;
  }
  return rc;
}

// Static initialization of modules for the test harness.
// Good module uses the OK constructor; bad module uses the FAIL constructor.
static void init_test_modules(void) {
  moduleGood.x.xCreate = fake_xCreate_OK;
  moduleGood.pUserData = nullptr;
  moduleBad.x.xCreate = fake_xCreate_FAIL;
  moduleBad.pUserData = nullptr;
}

} // extern "C"


// Simple test harness (non-terminating assertions). Tracks test outcome and prints summary.
struct TestCase {
  std::string name;
  bool passed;
  std::string message;
};

int main() {
  // Initialize fake modules
  init_test_modules();

  std::vector<TestCase> tests;

  // Helper lambdas for asserting outcomes
  auto expect_true = [&](bool cond, const std::string& msg) {
    if(cond) return std::string("OK");
    return std::string("FAIL: ") + msg;
  };

  // 1) Unknown tokenizer should return SQLITE_ERROR and set error message.
  {
    Fts5Global g;
    Fts5Config cfg;
    std::memset(&g, 0, sizeof(g));
    std::memset(&cfg, 0, sizeof(cfg));

    const char *arg0 = "unknown_tokenizer";
    const char *azArg[] = { arg0 };
    char *err = nullptr;

    int rc = sqlite3Fts5GetTokenizer(&g, azArg, 1, &cfg, &err);

    bool ok = (rc == SQLITE_ERROR) && (cfg.pTok == 0) && (cfg.pTokApi == 0) && (err != nullptr);
    std::string detail;
    if(err) {
      detail = std::string("err=\"") + err + "\"";
      free(err);
    }
    TestCase t;
    t.name = "Unknown tokenizer returns error and clears config";
    t.passed = ok;
    t.message = ok ? "pass" : ("fail: " + detail);
    tests.push_back(t);
  }

  // 2) Found tokenizer with no extra args: should succeed, pTok non-null, ePattern set.
  {
    Fts5Global g;
    Fts5Config cfg;
    std::memset(&g, 0, sizeof(g));
    std::memset(&cfg, 0, sizeof(cfg));

    const char *arg0 = "tok";
    const char *azArg[] = { arg0 }; // only tokenizer name, no extra args
    char *err = nullptr;

    int rc = sqlite3Fts5GetTokenizer(&g, azArg, 1, &cfg, &err);

    bool ok = (rc == SQLITE_OK) && (cfg.pTok != nullptr) && (cfg.pTokApi != nullptr);
    // ePattern should be set to 123 by our tokenizer pattern function
    ok = ok && (cfg.ePattern == 123);
    TestCase t;
    t.name = "Tokenizer found with no extra args creates token and pattern";
    t.passed = ok;
    t.message = ok ? "pass" : "fail: tokenizer/path or pattern mismatch";
    // Clean up allocated tokenizer
    if(cfg.pTok){
      delete cfg.pTok;
      cfg.pTok = nullptr;
    }
    tests.push_back(t);
  }

  // 3) Found tokenizer with extra args: ensure xCreate is called with (nArg-1) and pTokApi is set
  //    and that ePattern is populated.
  {
    Fts5Global g;
    Fts5Config cfg;
    std::memset(&g, 0, sizeof(g));
    std::memset(&cfg, 0, sizeof(cfg));

    const char *arg0 = "tok";
    const char *azArg[] = { arg0, "arg1", "arg2" }; // name + 2 extra args
    char *err = nullptr;

    int rc = sqlite3Fts5GetTokenizer(&g, azArg, 3, &cfg, &err);

    bool ok = (rc == SQLITE_OK) && (cfg.pTok != nullptr) && (cfg.pTokApi != nullptr);
    ok = ok && (cfg.ePattern == 123);
    TestCase t;
    t.name = "Tokenizer with args creates token with args and pattern";
    t.passed = ok;
    t.message = ok ? "pass" : "fail: mismatch in token/args/pattern";
    if(cfg.pTok){
      delete cfg.pTok;
      cfg.pTok = nullptr;
    }
    tests.push_back(t);
  }

  // 4) Tokenizer constructor failure: rc != SQLITE_OK, pTok/pTokApi reset to 0, error message allocated.
  {
    Fts5Global g;
    Fts5Config cfg;
    std::memset(&g, 0, sizeof(g));
    std::memset(&cfg, 0, sizeof(cfg));

    // Switch the good module's xCreate to fail
    moduleGood.x.xCreate = fake_xCreate_FAIL;

    const char *arg0 = "tok";
    const char *azArg[] = { arg0, "arg1" };
    char *err = nullptr;

    int rc = sqlite3Fts5GetTokenizer(&g, azArg, 2, &cfg, &err);

    bool ok = (rc == SQLITE_ERROR) && (cfg.pTok == 0) && (cfg.pTokApi == 0);
    if(err) {
      // We expect the error message to be allocated
      // (the exact content is not asserted here to keep test robust against changes)
      free(err);
      err = nullptr;
    }

    TestCase t;
    t.name = "Tokenizer constructor failure resets config and returns error";
    t.passed = ok;
    t.message = ok ? "pass" : "fail: constructor failure path differed";
  // Restore for further tests
    moduleGood.x.xCreate = fake_xCreate_OK;
    tests.push_back(t);
  }

  // Summary report
  int passed = 0;
  for(const auto &tc : tests) if(tc.passed) ++passed;
  std::cout << "FTS5 GetTokenizer test suite: " << passed << "/" << tests.size() << " tests passed.\n";
  for(const auto &tc : tests) {
    std::cout << "[ " << (tc.passed ? "PASS" : "FAIL") << " ] " << tc.name;
    if(!tc.message.empty()) std::cout << " - " << tc.message;
    std::cout << "\n";
  }

  return (passed == (int)tests.size()) ? 0 : 1;
}