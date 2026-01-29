// Test suite for sqlite3_stricmp function with a mock sqlite3StrICmp.
// This test follows the provided guidance: no GoogleTest, a lightweight test harness,
// and careful handling of null pointers as per the focal method's logic.

// Step 1 & 2 concepts (embedded as comments for traceability):
// - Focal method: int sqlite3_stricmp(const char *zLeft, const char *zRight)
//   Behavior:
//     if zLeft is null -> return (zRight ? -1 : 0)
//     else if zRight is null -> return 1
//     else -> return sqlite3StrICmp(zLeft, zRight)
// - sqlite3StrICmp(s1, s2) should perform a case-insensitive comparison and return:
//     <0 if s1 < s2, 0 if equal, >0 if s1 > s2
// - We provide a mock implementation of sqlite3StrICmp to drive deterministic behavior.

// Lightweight test harness notes:
// - Uses non-terminating checks: reports PASS/FAIL for each case and continues.
// - Tests true/false branches of each predicate at least once.
// - Uses only standard library (no GTest) and a single translation unit implementation.

#include <math.h>
#include <cctype>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>


// Forward declaration for the sqlite3StrICmp mock used by sqlite3_stricmp
int sqlite3StrICmp(const char *zLeft, const char *zRight);

// Focal method under test: sqlite3_stricmp
int sqlite3_stricmp(const char *zLeft, const char *zRight){
  if( zLeft==0 ){
    return zRight ? -1 : 0;
  }else if( zRight==0 ){
    return 1;
  }
  return sqlite3StrICmp(zLeft, zRight);
}

// Mock implementation of sqlite3StrICmp to emulate SQLite's case-insensitive compare.
// This implementation is intentionally straightforward and suitable for unit testing.
int sqlite3StrICmp(const char *zLeft, const char *zRight){
  if (zLeft == nullptr || zRight == nullptr) {
    // For safety: mimic behavior if either pointer is null (not expected in normal calls)
    if (zLeft == nullptr && zRight == nullptr) return 0;
    return (zLeft == nullptr) ? -1 : 1;
  }

  // Compare characters case-insensitively
  while (*zLeft && *zRight) {
    unsigned char c1 = static_cast<unsigned char>(std::tolower(static_cast<unsigned char>(*zLeft)));
    unsigned char c2 = static_cast<unsigned char>(std::tolower(static_cast<unsigned char>(*zRight)));
    if (c1 != c2) return (c1 < c2) ? -1 : 1;
    ++zLeft;
    ++zRight;
  }

  // If both strings ended together, they are equal.
  if (*zLeft == '\0' && *zRight == '\0') return 0;
  // If one string ended, the shorter one is considered smaller.
  return (*zLeft == '\0') ? -1 : 1;
}

// Simple test harness
static int gTotal = 0;
static int gFailed = 0;

// Helper to run a single test case
void run_case(const char* name, const char* L, const char* R, int expected) {
  ++gTotal;
  int res = sqlite3_stricmp(L, R);
  if (res != expected) {
    std::cout << "FAIL: " << name
              << " | sqlite3_stricmp(" << (L ? L : "NULL") << ", " << (R ? R : "NULL")
              << ") = " << res << "  (expected " << expected << ")\n";
    ++gFailed;
  } else {
    std::cout << "PASS: " << name
              << " | sqlite3_stricmp(" << (L ? L : "NULL") << ", " << (R ? R : "NULL")
              << ") == " << res << "\n";
  }
}

int main() {
  // Test 1: Both inputs are null -> expect 0
  // True branch: zLeft==0 and zRight==0
  run_case("BothNull", nullptr, nullptr, 0);

  // Test 2: Left null, Right non-null -> expect -1
  // True branch: zLeft==0 and zRight!=0
  run_case("LeftNull_RightNonNull", nullptr, "foo", -1);

  // Test 3: Left non-null, Right null -> expect 1
  // True branch: zLeft!=0 and zRight==0
  run_case("LeftNonNull_RightNull", "bar", nullptr, 1);

  // Test 4: Both non-null, equal ignoring case -> expect 0
  // False branch inside sqlite3StrICmp returns 0 for equal case-insensitive
  run_case("CaseInsensitive_Equal", "Apple", "apple", 0);

  // Test 5: Both non-null, left < right in case-insensitive comparison
  run_case("CaseInsensitive_LeftLess", "apple", "banana", -1);

  // Test 6: Both non-null, left > right in case-insensitive comparison
  run_case("CaseInsensitive_LeftGreater", "banana", "apple", 1);

  // Test 7: Non-null, non-equal strings with left < right
  run_case("LeftLessRight", "abc", "abd", -1);

  // Test 8: Non-null, non-equal strings with left > right
  run_case("LeftGreaterRight", "abd", "abc", 1);

  // Test 9: Single char comparison (case-insensitive)
  run_case("LeftGreaterSingleChar", "Z", "a", 1);

  // Test 10: Prefix relation with left longer than right
  run_case("LeftLonger", "alpha", "alph", 1);

  // Summary
  std::cout << "Summary: Total=" << gTotal << " Passed=" << (gTotal - gFailed)
            << " Failed=" << gFailed << "\n";

  // Return non-zero if any test failed to signal failure to build systems
  return (gFailed == 0) ? 0 : 1;
}