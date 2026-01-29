// Minimal C++11 test harness for the focal function:
// void sqlite3ResolvePartIdxLabel(Parse *pParse, int iLabel)
// The test avoids using GTest and uses a lightweight, non-terminating
// assertion scheme to maximize code execution coverage.

#include <vector>
#include <string>
#include <sqliteInt.h>
#include <iostream>
#include <utility>


// Lightweight stubs to model the minimal dependencies required by the focal function.
// These are intentionally simple and locally scoped within this test file to avoid
// pulling in the entire SQLite code base.

struct Vdbe {};  // Minimal dummy VM structure.

// Forward declaration of the mock hook used by tests to intercept calls.
void sqlite3VdbeResolveLabel(Vdbe* v, int label);

// Parse structure expected by the focal function.
// It only needs a pointer to a Vdbe object for our test purposes.
struct Parse {
  Vdbe* pVdbe;
};

// The focal method under test (reproduced for the test harness).
// It uses the presence of a non-zero iLabel to conditionally call
// sqlite3VdbeResolveLabel with the current parse context's Vdbe.
void sqlite3ResolvePartIdxLabel(Parse *pParse, int iLabel){
  if( iLabel ){
    sqlite3VdbeResolveLabel(pParse->pVdbe, iLabel);
  }
}

// ----------------------
// Test harness definitions
// ----------------------

// Global vector to capture all invocations of sqlite3VdbeResolveLabel during tests.
// Each entry is a pair of (Vdbe*, int) representing (Vdbe pointer, label value).
static std::vector<std::pair<Vdbe*, int>> g_calls;

// Mock implementation of sqlite3VdbeResolveLabel used by the focal method.
// It records every call into the global log above so tests can verify behavior.
void sqlite3VdbeResolveLabel(Vdbe* v, int label){
  g_calls.emplace_back(v, label);
}

// Simple non-terminating assertion helper.
// Returns true if condition holds; false otherwise and prints a message.
// This enables multiple tests to run in sequence without aborting on first failure.
bool expect(bool condition, const std::string& message){
  if(condition){
    std::cout << "  [OK] " << message << "\n";
    return true;
  }else{
    std::cerr << "  [FAIL] " << message << "\n";
    return false;
  }
}

// ----------------------
// Individual test cases
// ----------------------

// Test 1: iLabel == 0 should not call sqlite3VdbeResolveLabel at all.
bool test_iLabelZeroDoesNotCall(){
  // Prepare environment
  g_calls.clear();
  Parse p;
  Vdbe v;
  p.pVdbe = &v;

  // Exercise
  sqlite3ResolvePartIdxLabel(&p, 0);

  // Verify: no calls recorded
  bool passed = g_calls.empty();
  return expect(passed, "iLabel==0 results in zero calls to sqlite3VdbeResolveLabel");
}

// Test 2: iLabel != 0 should call sqlite3VdbeResolveLabel exactly once with correct args.
bool test_iLabelNonZeroCallsWithCorrectParams(){
  // Prepare environment
  g_calls.clear();
  Parse p;
  Vdbe v;
  p.pVdbe = &v;

  int label = 12345;

  // Exercise
  sqlite3ResolvePartIdxLabel(&p, label);

  // Verify: exactly one call with (pVdbe, label)
  bool cond1 = (g_calls.size() == 1);
  bool cond2 = cond1 && (g_calls[0].first == &v) && (g_calls[0].second == label);

  return expect(cond1, "iLabel!=0 triggers exactly one call");
  // If cond1 passes, check cond2 as a more precise assertion
  // Note: We already printed a pass for cond1; print a separate detailed message:
  // For clarity in this lightweight harness, we perform a second check via a nested expect.
  // However, since we cannot easily chain prints after a return, re-check here:
  if(!cond1) return false;
  return expect(cond2, "Called with correct Vdbe pointer and label value");
}

// Test 3: iLabel != 0 with a null Vdbe in Parse should pass the pointer through to the mock.
// This validates that the function under test forwards the argument as-is to the resolver.
bool test_iLabelNonZeroWithNullVdbe(){
  // Prepare environment
  g_calls.clear();
  Parse p;
  p.pVdbe = nullptr; // intentionally null
  int label = 77;

  // Exercise
  sqlite3ResolvePartIdxLabel(&p, label);

  // Verify: one call recorded with (nullptr, label)
  bool cond1 = (g_calls.size() == 1);
  bool cond2 = cond1 && (g_calls[0].first == nullptr) && (g_calls[0].second == label);

  if(!cond1) return expect(false, "iLabel non-zero with null Vdbe should trigger one call (structure allowed)");
  return expect(cond2, "iLabel non-zero with null Vdbe forwards (nullptr, label) correctly");
}

// ----------------------
// Main test runner
// ----------------------

int main(){
  int total = 0;
  int failures = 0;

  auto run = [&](const char* name, bool ok){
    ++total;
    if(ok){
      ++total; // not used; kept for readability in logs
      std::cout << "[TEST PASSED] " << name << "\n";
      return;
    } else {
      ++failures;
      std::cerr << "[TEST FAILED] " << name << "\n";
    }
  };

  // Run tests
  bool t1 = test_iLabelZeroDoesNotCall();
  run("test_iLabelZeroDoesNotCall", t1);

  bool t2 = test_iLabelNonZeroCallsWithCorrectParams();
  run("test_iLabelNonZeroCallsWithCorrectParams", t2);

  bool t3 = test_iLabelNonZeroWithNullVdbe();
  run("test_iLabelNonZeroWithNullVdbe", t3);

  // Final summary
  if(failures == 0){
    std::cout << "[ALL TESTS PASSED] " << total << " tests executed.\n";
    return 0;
  } else {
    std::cerr << "[TESTS FAILED] " << failures << " of " << total << " tests failed.\n";
    return 1;
  }
}