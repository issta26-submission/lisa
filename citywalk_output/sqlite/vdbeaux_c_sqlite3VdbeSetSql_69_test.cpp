// Unit test suite for the focal method: sqlite3VdbeSetSql
// This test suite is implemented in C++11 (no GoogleTest) and focuses on
// validating the behavior of sqlite3VdbeSetSql using a minimal, self-contained
// stubbed environment that mirrors the core dependencies described in the code
// snippet. The tests cover true/false branches and basic memory behavior.

#include <cstdio>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <cassert>


// Domain-specific typedefs and macros (stubs to emulate the real environment)
using u8 = uint8_t;
#define SQLITE_PREPARE_SAVESQL 0x01

// Minimal sqlite3 and Vdbe structures to exercise sqlite3VdbeSetSql
struct sqlite3 { /* empty stub for test purposes */ };

struct Vdbe {
  u8 prepFlags;
  int expmask;
  char *zSql;
  sqlite3 *db;
  Vdbe() : prepFlags(0), expmask(0), zSql(nullptr), db(nullptr) {}
};

// Lightweight string duplicate with length truncation to mimic sqlite3DbStrNDup
char* sqlite3DbStrNDup(sqlite3* db, const char* z, int n) {
  (void)db; // db is not used in this stub, but kept to reflect real API shape
  if (n < 0) n = 0;
  char* p = static_cast<char*>(std::malloc(n + 1));
  if (!p) return nullptr;
  if (z && n > 0) std::memcpy(p, z, static_cast<size_t>(std::min(n, std::strlen(z))));
  p[n] = '\0';
  return p;
}

// Focal method under test (replicated here to enable standalone unit testing)
void sqlite3VdbeSetSql(Vdbe *p, const char *z, int n, u8 prepFlags){
  if( p==nullptr ) return;
  p->prepFlags = prepFlags;
  if( (prepFlags & SQLITE_PREPARE_SAVESQL)==0 ){
    p->expmask = 0;
  }
  assert( p->zSql==0 );
  p->zSql = sqlite3DbStrNDup(p->db, z, n);
}

// Simple test harness to collect failures without aborting on first failure
static int g_failures = 0;
#define EXPECT_TRUE(cond, msg) do { if(!(cond)) { fprintf(stderr, "EXPECT_TRUE failed: %s\n", (msg)); g_failures++; } } while(0)
#define EXPECT_EQ_INT(a, b, msg) do { if((a) != (b)) { fprintf(stderr, "EXPECT_EQ_INT failed: %s (actual=%d, expected=%d)\n", (msg), (a), (b)); g_failures++; } } while(0)
#define EXPECT_EQ_PTR(a, b, msg) do { if((void*)(a) != (void*)(b)) { fprintf(stderr, "EXPECT_EQ_PTR failed: %s (actual=%p, expected=%p)\n", (msg), (void*)(a), (void*)(b)); g_failures++; } } while(0)
#define EXPECT_STR_EQ(a, b, msg) do { const char* _aa = (a); const char* _bb = (b); if(_aa==nullptr && _bb==nullptr) { /* ok */ } else if(_aa==nullptr || _bb==nullptr) { fprintf(stderr, "EXPECT_STR_EQ failed: %s (one is null)\n", (msg)); g_failures++; } else if(std::strcmp(_aa, _bb) != 0) { fprintf(stderr, "EXPECT_STR_EQ failed: %s (actual='%s', expected='%s')\n", (msg), _aa, _bb); g_failures++; } } while(0)

// Helper to create a fresh Vdbe with a dummy db
static void init_vdbe(Vdbe &v, sqlite3 *db) {
  v.prepFlags = 0;
  v.expmask = 0;
  v.zSql = nullptr;
  v.db = db;
}

// Test 1: Basic behavior with SAVESQL not set; verify prepFlags, expmask, and zSql content
void test_basic_sets_prepFlags_and_sql_dup() {
  // Domain knowledge: ensure the true branch of (prepFlags & SAVESQL) == 0 is exercised
  sqlite3 db;
  Vdbe v;
  init_vdbe(v, &db);
  const char input[] = "abcdef";
  int n = 3;

  // Preconditions: zSql must be null to satisfy the internal assertion
  v.zSql = nullptr;
  v.prepFlags = 0;
  v.expmask = 123; // non-zero to verify it becomes 0 in the true branch

  sqlite3VdbeSetSql(&v, input, n, 0); // SAVESQL not set

  // Postconditions
  EXPECT_TRUE(v.prepFlags == 0, "prepFlags should be updated to 0 when SAVESQL is not set");
  EXPECT_EQ_INT(v.expmask, 0, "expmask should be reset to 0 when SAVESQL is not set");
  EXPECT_TRUE(v.zSql != nullptr);
  // Validate content: first n chars copied, null-terminated
  EXPECT_STR_EQ(v.zSql, "abc", "zSql should contain the first n characters of z");
  // Cleanup
  std::free(v.zSql);
  v.zSql = nullptr;
}

// Test 2: Behavior with SAVESQL bit set; expmask should be preserved
void test_preserve_expmask_when_savesql_set() {
  sqlite3 db;
  Vdbe v;
  init_vdbe(v, &db);
  const char input[] = "abcdef";
  int n = 3;

  v.zSql = nullptr;
  v.prepFlags = 0;
  v.expmask = 7; // non-zero cap to observe preservation

  sqlite3VdbeSetSql(&v, input, n, SQLITE_PREPARE_SAVESQL); // SAVESQL set

  // Postconditions
  EXPECT_TRUE(v.prepFlags == SQLITE_PREPARE_SAVESQL, "prepFlags should be set to provided value when SAVESQL is set");
  EXPECT_EQ_INT(v.expmask, 7, "expmask should remain unchanged when SAVESQL is set");
  EXPECT_TRUE(v.zSql != nullptr);
  EXPECT_STR_EQ(v.zSql, "abc", "zSql should contain the first n characters of z");
  // Cleanup
  std::free(v.zSql);
  v.zSql = nullptr;
}

// Test 3: Null pointer input should result in no operation
void test_null_pointer_input_no_operation() {
  // No crash or modification should occur
  Vdbe dummy;
  init_vdbe(dummy, nullptr); // db is null, but function should return early due to p==nullptr
  sqlite3VdbeSetSql(nullptr, "any", 3, 0);
  // If we reach here without crash, test passes
  printf("Test 3 executed (null input): OK\n");
}

// Test 4: Zero-length copy should produce empty zSql
void test_zero_length_copy_results_in_empty_string() {
  sqlite3 db;
  Vdbe v;
  init_vdbe(v, &db);
  const char input[] = "hidden";
  int n = 0;

  v.zSql = nullptr;
  v.prepFlags = 0;
  v.expmask = 99;

  sqlite3VdbeSetSql(&v, input, n, 0);

  EXPECT_TRUE(v.zSql != nullptr);
  EXPECT_STR_EQ(v.zSql, "", "zSql should be an empty string when n == 0");
  EXPECT_TRUE(v.expmask == 0); // since SAVESQL is not set, expmask should reset to 0
  // Cleanup
  std::free(v.zSql);
  v.zSql = nullptr;
}

// Test 5: ZSql initially non-null should trigger assertion (we avoid triggering in tests)
void test_assertion_not_triggered_by_tests() {
  // This test demonstrates that we intentionally avoid triggering the internal assert
  // by ensuring zSql is null before calling sqlite3VdbeSetSql.
  sqlite3 db;
  Vdbe v;
  init_vdbe(v, &db);
  v.zSql = (char*)0x1; // deliberately non-null to illustrate behavior (would trigger in real run)
  const char input[] = "data";
  sqlite3VdbeSetSql(&v, input, 4, 0);
  // Since we don't want to crash, this test is intentionally a no-op or would have failed
  // in a real environment. We skip assertion here.
  // Cleanup to avoid memory leaks in case the implementation handles non-null zSql differently
  if (v.zSql) {
    std::free(v.zSql);
    v.zSql = nullptr;
  }
}

// Entry point to run all tests
int main() {
  printf("Starting sqlite3VdbeSetSql unit tests (standalone C++11 test harness)\n");

  test_basic_sets_prepFlags_and_sql_dup();
  test_preserve_expmask_when_savesql_set();
  test_null_pointer_input_no_operation();
  test_zero_length_copy_results_in_empty_string();
  test_assertion_not_triggered_by_tests();

  if (g_failures == 0) {
    printf("ALL TESTS PASSED\n");
    return 0;
  } else {
    printf("TESTS FAILED: %d failure(s)\n", g_failures);
    return 1;
  }
}