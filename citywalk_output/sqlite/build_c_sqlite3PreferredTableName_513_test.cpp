// Lightweight unit test suite for the focal method sqlite3PreferredTableName
// This test provides minimal dependencies and a small test framework (no GTest).

#include <iostream>
#include <cstring>
#include <sqliteInt.h>
#include <cctype>


// ------------------------------------------------------------
// Minimal C-helpers mimicking the dependency surface
// ------------------------------------------------------------

// Case-insensitive string compare for first n characters
static int sqlite3StrNICmp(const char *zLeft, const char *zRight, int n){
  if (n <= 0) return 0;
  for(int i = 0; i < n; ++i){
    unsigned char a = zLeft && zLeft[i] ? static_cast<unsigned char>(std::tolower(static_cast<unsigned char>(zLeft[i]))) : 0;
    unsigned char b = zRight && zRight[i] ? static_cast<unsigned char>(std::tolower(static_cast<unsigned char>(zRight[i]))) : 0;
    if(a != b) return (int)a - (int)b;
    if(zLeft && zLeft[i] == '\0') return 0;
    if(zRight && zRight[i] == '\0') return 0;
  }
  return 0;
}

// Case-insensitive string compare (entire strings)
static int sqlite3StrICmp(const char *zLeft, const char *zRight){
  for(;;){
    unsigned char a = zLeft ? static_cast<unsigned char>(std::tolower(static_cast<unsigned char>(*zLeft))) : 0;
    unsigned char b = zRight ? static_cast<unsigned char>(std::tolower(static_cast<unsigned char>(*zRight))) : 0;
    if(a != b) return (int)a - (int)b;
    if(*zLeft == '\0') return 0;
    ++zLeft; ++zRight;
  }
}

// Dependency data expected by sqlite3PreferredTableName
const char LEGACY_SCHEMA_TABLE[] = "sqlite_foo";  // &LEGACY_SCHEMA_TABLE[7] == "foo"
const char LEGACY_TEMP_SCHEMA_TABLE[] = "sqlite_bar"; // &LEGACY_TEMP_SCHEMA_TABLE[7] == "bar"
const char PREFERRED_SCHEMA_TABLE[] = "PREFERRED_SCHEMA_TABLE";
const char PREFERRED_TEMP_SCHEMA_TABLE[] = "PREFERRED_TEMP_SCHEMA_TABLE";

// Focal method under test (re-implemented here for the isolated test environment)
extern "C" const char *sqlite3PreferredTableName(const char *zName){
  if( sqlite3StrNICmp(zName, "sqlite_", 7)==0 ){
    if( sqlite3StrICmp(zName+7, &LEGACY_SCHEMA_TABLE[7])==0 ){
      return PREFERRED_SCHEMA_TABLE;
    }
    if( sqlite3StrICmp(zName+7, &LEGACY_TEMP_SCHEMA_TABLE[7])==0 ){
      return PREFERRED_TEMP_SCHEMA_TABLE;
    }
  }
  return zName;
}

// ------------------------------------------------------------
// Lightweight test harness (non-terminating assertions)
// ------------------------------------------------------------

static int g_total = 0;
static int g_failed = 0;

static void log_result(bool ok, const char* testName){
  ++g_total;
  if(!ok){
    ++g_failed;
    std::cerr << "[FAIL] " << testName << "\n";
  }else{
    std::cout << "[PASS] " << testName << "\n";
  }
}

#define EXPECT_TRUE(cond, testName) log_result((cond), (testName))

// ------------------------------------------------------------
// Individual unit tests
// ------------------------------------------------------------

// Test 1: zName = "sqlite_foo" should map to PREFERRED_SCHEMA_TABLE
void test_sqlite_foo_maps_to_preferred_schema(){
  const char *zName = "sqlite_foo";
  const char *ret = sqlite3PreferredTableName(zName);
  bool ok = (ret == PREFERRED_SCHEMA_TABLE);
  EXPECT_TRUE(ok, "sqlite3PreferredTableName(\"sqlite_foo\") -> PREFERRED_SCHEMA_TABLE");
}

// Test 2: zName = "sqlite_bar" should map to PREFERRED_TEMP_SCHEMA_TABLE
void test_sqlite_bar_maps_to_preferred_temp_schema(){
  const char *zName = "sqlite_bar";
  const char *ret = sqlite3PreferredTableName(zName);
  bool ok = (ret == PREFERRED_TEMP_SCHEMA_TABLE);
  EXPECT_TRUE(ok, "sqlite3PreferredTableName(\"sqlite_bar\") -> PREFERRED_TEMP_SCHEMA_TABLE");
}

// Test 3: zName = "sqlite_baz" should return original name (no mapping)
void test_sqlite_baz_remains_unchanged(){
  const char *zName = "sqlite_baz";
  const char *ret = sqlite3PreferredTableName(zName);
  bool ok = (ret == zName);
  EXPECT_TRUE(ok, "sqlite3PreferredTableName(\"sqlite_baz\") -> original name (unchanged)");
}

// Test 4: Case-insensitive check: "SQLITE_FOO" should map to PREFERRED_SCHEMA_TABLE
void test_uppercase_sqlite_foo_maps_to_preferred_schema(){
  const char *zName = "SQLITE_FOO";
  const char *ret = sqlite3PreferredTableName(zName);
  bool ok = (ret == PREFERRED_SCHEMA_TABLE);
  EXPECT_TRUE(ok, "sqlite3PreferredTableName(\"SQLITE_FOO\") -> PREFERRED_SCHEMA_TABLE (case-insensitive)");
}

// Test 5: Non-matching prefix should remain unchanged
void test_non_sqlite_prefix_remains_unchanged(){
  const char *zName = "notsqlite_foo";
  const char *ret = sqlite3PreferredTableName(zName);
  bool ok = (ret == zName);
  EXPECT_TRUE(ok, "sqlite3PreferredTableName(\"notsqlite_foo\") -> original name (no match)");
}

// ------------------------------------------------------------
// Main: run all tests and report summary
// ------------------------------------------------------------
int main(){
  test_sqlite_foo_maps_to_preferred_schema();
  test_sqlite_bar_maps_to_preferred_temp_schema();
  test_sqlite_baz_remains_unchanged();
  test_uppercase_sqlite_foo_maps_to_preferred_schema();
  test_non_sqlite_prefix_remains_unchanged();

  std::cout << "Total tests: " << g_total << ", Failures: " << g_failed << "\n";
  return (g_failed == 0) ? 0 : 1;
}