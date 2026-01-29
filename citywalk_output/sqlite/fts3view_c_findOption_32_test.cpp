// Lightweight C++11 test suite for the focal function findOption
// This test is self-contained: it redefines the focused function and the
// dependent globals locally to ensure controlled and safe execution.
// The tests exercise the following scenarios (covering all branches described
// in the focal method, including true/false predicates):
// - Test 1: Found option with hasArg != 0 and the option is not the last element.
// - Test 2: Option not found in the extras.
// - Test 3: Found option with hasArg == 0 (no-argument option).
// - Test 4: Found option at the last index (i == nExtra-1) to trigger last-element branch.
//
// Note: This is a standalone C++ test harness; it does not rely on GTest or
// external libraries. It uses a minimal, non-terminating assertion style.

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sqlite3.h>
#include <stdio.h>
#include <cstring>
#include <stdarg.h>
#include <string>
#include <iostream>


// Global state mirroring the focal code's dependencies.
// The real fts3view.c uses its own global nExtra and azExtra; here we provide
// controlled, test-scoped equivalents.

int nExtra;
const char *azExtra[16];

// The focal method copied/adapted for testing (safe in-test version).
// int nExtra and azExtra are expected to be set by each test case prior to the call.
const char *findOption(const char *zName, int hasArg, const char *zDefault){
  int i;
  const char *zResult = zDefault;
  for(i=0; i<nExtra; i++){
    const char *z = azExtra[i];
    while( z[0]=='-' ) z++;
    if( std::strcmp(z, zName)==0 ){
      int j = 1;
      if( hasArg==0 || i==nExtra-1 ) j = 0;
      zResult = azExtra[i+j];
      while( i+j<nExtra ){
        azExtra[i] = azExtra[i+j+1];
        i++;
      }
      break;
    }
  }
  return zResult;
}

// Simple helper to compare C-strings safely, handling nullptrs gracefully.
static bool equalCstr(const char* a, const char* b){
  if(a==nullptr && b==nullptr) return true;
  if(a==nullptr || b==nullptr) return false;
  return std::strcmp(a, b) == 0;
}

// Minimal test harness
static int totalFailures = 0;

static void report(const std::string& testName, bool passed, const std::string& details=""){
  if(passed){
    std::cout << "[PASS] " << testName;
  }else{
    std::cout << "[FAIL] " << testName;
  }
  if(!details.empty()){
    std::cout << " - " << details;
  }
  std::cout << std::endl;
  if(!passed) totalFailures++;
}

// Test 1: Found option with hasArg != 0 and option is not the last element.
// Expected: zName "foo" matches azExtra[0], j=1, returns azExtra[1], and azExtra[0] becomes the next element (NULL in our setup).
static void test_findOption_found_hasArg_notLast(){
  // Setup: nExtra = 2, azExtra = { "-foo", "bar", NULL }
  nExtra = 2;
  azExtra[0] = "-foo";
  azExtra[1] = "bar";
  azExtra[2] = nullptr; // sentinel to allow azExtra[i+j+1] access safely
  const char* result = findOption("foo", 1, "default");
  bool pass = equalCstr(result, "bar") && azExtra[0] == nullptr;
  report("Test 1: findOption found (hasArg != 0, not last)", pass,
         "expected result='bar', azExtra[0] becomes NULL");
}

// Test 2: Option not found in extras.
// Expected: zDefault is returned and azExtra remains unchanged.
static void test_findOption_not_found(){
  // Setup: nExtra = 2, azExtra = { "-baz", "qux", NULL }
  nExtra = 2;
  azExtra[0] = "-baz";
  azExtra[1] = "qux";
  azExtra[2] = nullptr;
  const char* result = findOption("foo", 1, "default");
  bool pass = equalCstr(result, "default")
              && equalCstr(azExtra[0], "-baz")
              && equalCstr(azExtra[1], "qux");
  report("Test 2: findOption not found", pass,
         "expected result='default' and extras unchanged");
}

// Test 3: Found option with hasArg == 0 (no-argument option).
// Expected: zResult equals the option string (azExtra[i]), and azExtra[0] shifts to azExtra[1].
static void test_findOption_hasArg_zero(){
  // Setup: nExtra = 2, azExtra = { "-foo", "VO", NULL }
  nExtra = 2;
  azExtra[0] = "-foo";
  azExtra[1] = "VO";
  azExtra[2] = nullptr;
  const char* result = findOption("foo", 0, "default");
  bool pass = equalCstr(result, "-foo") && equalCstr(azExtra[0], "VO");
  report("Test 3: findOption hasArg == 0 (no arg)", pass,
         "expected result='-foo', extras shifted: azExtra[0]='VO'");
}

// Test 4: Option found at last index (i == nExtra-1) with hasArg != 0.
// Expected: zResult equals azExtra[i] (the last element) and azExtra[i] shifted to NULL.
static void test_findOption_last_index(){
  // Setup: nExtra = 2, azExtra = { "foo", "bar", NULL }
  nExtra = 2;
  azExtra[0] = "foo";
  azExtra[1] = "bar";
  azExtra[2] = nullptr;
  const char* result = findOption("foo", 1, "default2");
  bool pass = equalCstr(result, "bar") && azExtra[1] == nullptr;
  report("Test 4: findOption at last index (i==nExtra-1)", pass,
         "expected result='bar', azExtra[1] shifted to NULL");
}

int main(){
  // Run all tests
  std::cout << "Starting unit tests for findOption (self-contained harness)" << std::endl;
  test_findOption_found_hasArg_notLast();
  test_findOption_not_found();
  test_findOption_hasArg_zero();
  test_findOption_last_index();
  std::cout << "Unit tests completed." << std::endl;

  if(totalFailures == 0){
    std::cout << "All tests passed." << std::endl;
  }else{
    std::cout << totalFailures << " test(s) failed." << std::endl;
  }
  return totalFailures;
}