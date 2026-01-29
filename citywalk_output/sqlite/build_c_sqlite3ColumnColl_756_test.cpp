// Unit tests for the focal function sqlite3ColumnColl
// This test suite is designed to be self-contained (no GTest).
// It reproduces the essential behavior of sqlite3ColumnColl using a minimal
// Column-like structure sufficient for branch coverage.
// The tests verify true/false branches and the memory-pointer arithmetic
// implied by the function's logic.

#include <iostream>
#include <sqliteInt.h>


// Candidate Keywords extracted from the focal method (Step 1):
// - COLFLAG_HASCOLL
// - COLFLAG_HASTYPE
// - zCnName
// - pCol->colFlags
// - pCol->zCnName
// - return value semantics (0 vs. non-null)
// - pointer arithmetic (z+1 and subsequent increments)

// Minimal replication of the Column type and constants used by sqlite3ColumnColl
// (These are stand-ins to allow unit testing outside the full sqlite3 codebase).

// Flags (as used by the focal method)
static const unsigned int COLFLAG_HASCOLL = 0x01;
static const unsigned int COLFLAG_HASTYPE = 0x02;

// Minimal Column structure matching the usage in sqlite3ColumnColl
struct Column {
  unsigned int colFlags;
  const char *zCnName;
};

// Focal method under test (reproduced here for self-contained testing)
const char *sqlite3ColumnColl(Column *pCol){
  const char *z;
  if( (pCol->colFlags & COLFLAG_HASCOLL)==0 ) return 0;
  z = pCol->zCnName;
  while( *z ){ z++; }
  if( pCol->colFlags & COLFLAG_HASTYPE ){
    do{ z++; }while( *z );
  }
  return z+1;
}

// Simple test harness utilities
static bool report_test(const std::string &name, bool ok) {
  if(ok){
    std::cout << "[PASS] " << name << "\n";
    return true;
  }else{
    std::cout << "[FAIL] " << name << "\n";
    return false;
  }
}

// Test 1: False branch when COLFLAG_HASCOLL bit is not set.
// Expected: sqlite3ColumnColl returns nullptr (0).
bool test_noHasCollReturnsNull() {
  Column c;
  c.colFlags = 0;              // No COLFLAG_HASCOLL
  c.zCnName = "anything";        // zCnName content is irrelevant in this branch

  const char *res = sqlite3ColumnColl(&c);
  bool ok = (res == 0);
  // Explanatory comment: verify that without the HASCOLL flag the function
  // does not access zCnName and returns null as specified.
  return report_test("test_noHasCollReturnsNull (false branch)", ok);
}

// Test 2: True branch without HASTYPE.
// When HASCOLL is set and HASTYPE is not set, the function should
// return a pointer to the character immediately after the end of zCnName's
// name segment (i.e., after the first 0 terminator).
bool test_hasCollNoTypeReturnsPointerAfterName() {
  // zCnName layout: {'c','o','l','\0','X'}
  // Name: "col", terminator at index 3, expect return pointer at index 4 ('X')
  char data[5] = { 'c','o','l','\0','X' };

  Column c;
  c.colFlags = COLFLAG_HASCOLL; // HASCOLL set, HASTYPE not set
  c.zCnName = data;

  const char *ret = sqlite3ColumnColl(&c);
  bool ok = (ret == data + 4) && (*ret == 'X');
  // Explanatory comment: ensure non-type path returns a pointer to the byte
  // immediately following the end of the column name (i.e., the 'X' sentinel).
  return report_test("test_hasCollNoTypeReturnsPointerAfterName (true branch, no type)", ok);
}

// Test 3: True branch with HASTYPE.
// When HASCOLL and HASTYPE are both set, zCnName contains "name\0type\0" and the
// function should skip past both strings and return a pointer just after the final
// terminator. We provide extra sentinel data so the returned pointer is valid.
bool test_hasCollWithTypeReturnsPointerPastType() {
  // zCnName layout: {'c','o','l','\0','t','y','p','e','\0','Y','\0','Z'}
  // Name: "col", Type: "type", extra sentinel 'Z' at the end for verification.
  char data[12] = { 'c','o','l','\0','t','y','p','e','\0','Y','\0','Z' };

  Column c;
  c.colFlags = COLFLAG_HASCOLL | COLFLAG_HASTYPE;
  c.zCnName = data;

  const char *ret = sqlite3ColumnColl(&c);
  // After skipping "col" and "type", we expect ret to point to the 'Z'
  bool ok = (ret == data + 11) && (*ret == 'Z');
  // Explanatory comment: verify that the function correctly handles both HASCOLL
  // and HASTYPE by skipping the type annotation and returning the pointer after
  // the trailing terminator.
  return report_test("test_hasCollWithTypeReturnsPointerPastType (true branch with type)", ok);
}

// Entry point
int main() {
  int total = 0;
  int passed = 0;

  typedef bool (*TestFunc)();
  TestFunc tests[] = {
    test_noHasCollReturnsNull,
    test_hasCollNoTypeReturnsPointerAfterName,
    test_hasCollWithTypeReturnsPointerPastType
  };
  const char* testNames[] = {
    "test_noHasCollReturnsNull",
    "test_hasCollNoTypeReturnsPointerAfterName",
    "test_hasCollWithTypeReturnsPointerPastType"
  };

  const int N = sizeof(tests) / sizeof(tests[0]);
  for(int i = 0; i < N; ++i){
    bool result = tests[i]();
    total++;
    if(result) passed++;
  }

  std::cout << "\nSummary: " << passed << " / " << total << " tests passed.\n";

  // Return non-zero if any test failed
  return (passed == total) ? 0 : 1;
}