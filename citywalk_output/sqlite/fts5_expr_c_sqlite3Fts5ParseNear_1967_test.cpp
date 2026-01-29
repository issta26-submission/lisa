/*
  Unit test suite for the focal method:
  void sqlite3Fts5ParseNear(Fts5Parse *pParse, Fts5Token *pTok)

  Approach:
  - Provide minimal test doubles for Fts5Parse and Fts5Token to invoke the function.
  - Mock sqlite3Fts5ParseError to capture the error message without terminating.
  - Verify both branches of the condition:
      - No error when n == 4 and p == "NEAR"
      - Error when n != 4 or p != "NEAR"
  - Use simple, non-terminating assertions (return booleans) and informative messages.
  - All code is self-contained and relies only on the C/C++ standard library.
*/

#include <fts5Int.h>
#include <cstdio>
#include <fts5parse.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <cstdarg>
#include <iostream>


// Minimal forward declaration to mirror the real code's types
struct Fts5Parse; // opaque in the production code
typedef struct Fts5Parse Fts5ParseOpaque;

// Lightweight Fts5Token structure used by the focal method
typedef struct Fts5Token {
  const char *p; // pointer to the token text
  int n;         // length of the token text
} Fts5Token;

// Forward declare the focal function with C linkage to mirror the original C implementation
extern "C" void sqlite3Fts5ParseNear(Fts5Parse *pParse, Fts5Token *pTok);

// Mocking infrastructure: capture error messages instead of printing or aborting
static std::string lastErrorMessage;

// Mock implementation of sqlite3Fts5ParseError used by the focal method during tests.
// It formats the message according to the provided printf-style format.
extern "C" void sqlite3Fts5ParseError(Fts5Parse *pParse, const char *zFmt, ...) {
  (void)pParse; // pParse is not used by the tests; keep signature compatible
  va_list ap;
  char buf[1024];
  va_start(ap, zFmt);
  vsnprintf(buf, sizeof(buf), zFmt, ap);
  va_end(ap);
  lastErrorMessage = buf;
}

// Test 1: Verify that no error is reported when token is exactly "NEAR" with length 4
// This covers the true branch: pTok->n==4 and memcmp("NEAR", pTok->p, 4) == 0
bool test_parseNear_no_error_branch() {
  lastErrorMessage.clear();

  Fts5Parse pParseInstance; // dummy parse context
  Fts5Token tok = { "NEAR", 4 }; // token exactly equal to "NEAR"

  sqlite3Fts5ParseNear(&pParseInstance, &tok);

  bool ok = lastErrorMessage.empty();
  if(!ok){
    std::cout << "[Test 1] Failed: expected no error, got: " << lastErrorMessage << std::endl;
  } else {
    std::cout << "[Test 1] Passed: no error reported as expected." << std::endl;
  }
  return ok;
}

// Test 2: Verify that an error is reported when token length is incorrect (n != 4)
// This covers the false branch due to n != 4, even if the text would be "NEAR"
bool test_parseNear_wrong_length_branch() {
  lastErrorMessage.clear();

  Fts5Parse pParseInstance;
  Fts5Token tok = { "NEAR", 3 }; // incorrect length

  sqlite3Fts5ParseNear(&pParseInstance, &tok);

  const std::string expected = "fts5: syntax error near \"NEA\"";
  bool ok = lastErrorMessage == expected;
  if(!ok){
    std::cout << "[Test 2] Failed: expected '" << expected
              << "' but got '" << lastErrorMessage << "'\n";
  } else {
    std::cout << "[Test 2] Passed: correct error message for wrong length." << std::endl;
  }
  return ok;
}

// Test 3: Verify that an error is reported when token text differs from "NEAR" (n == 4 but text != "NEAR")
// This covers the false branch due to memcmp != 0
bool test_parseNear_wrong_token_branch() {
  lastErrorMessage.clear();

  Fts5Parse pParseInstance;
  Fts5Token tok = { "NEAQ", 4 }; // different token text of length 4

  sqlite3Fts5ParseNear(&pParseInstance, &tok);

  const std::string expected = "fts5: syntax error near \"NEAQ\"";
  bool ok = lastErrorMessage == expected;
  if(!ok){
    std::cout << "[Test 3] Failed: expected '" << expected
              << "' but got '" << lastErrorMessage << "'\n";
  } else {
    std::cout << "[Test 3] Passed: correct error message for wrong token." << std::endl;
  }
  return ok;
}

int main() {
  // Run all tests and report overall result
  int total = 0;
  int passed = 0;

  if(test_parseNear_no_error_branch()) passed++;
  total++;

  if(test_parseNear_wrong_length_branch()) passed++;
  total++;

  if(test_parseNear_wrong_token_branch()) passed++;
  total++;

  std::cout << "\nSummary: " << passed << " / " << total << " tests passed." << std::endl;

  // Return non-zero if any test failed
  return (passed == total) ? 0 : 1;
}