// Test suite for the focal method: sqlite3AffinityType
// This test reproduces a minimal environment to compile and exercise the function.
// The test avoids Google Test and uses a lightweight, non-terminating assertion style.

#include <cstdio>
#include <cctype>
#include <cstring>
#include <sqliteInt.h>
#include <cstdint>
#include <cassert>


// Domain-relevant definitions (simplified/mocked for unit testing)
typedef unsigned int u32;

// Affinity constants (chosen to align with the function's usage)
#define SQLITE_AFF_NUMERIC 0
#define SQLITE_AFF_TEXT    1
#define SQLITE_AFF_BLOB    2
#define SQLITE_AFF_REAL    3
#define SQLITE_AFF_INTEGER 4

// Column structure used by the focal method
struct Column {
  int colFlags;
  int szEst;
};

// Sorter reference flag mock (not used in tests, but defined for completeness)
#define COLFLAG_SORTERREF 0x01

// Minimal global config used in the function (guarded by #ifdef in production)
struct GlobalConfig {
  int szSorterRef;
} sqlite3GlobalConfig = { 0 };

// Hashing helper array to map ASCII characters to lowercase equivalents.
// This mirrors sqlite3UpperToLower used by the focal function.
static unsigned char sqlite3UpperToLower[256];

// Simple stub for isdigit/number parsing used by the focal function
static int sqlite3Isdigit(int c) {
  return (c >= '0' && c <= '9');
}
static int sqlite3GetInt32(const char *z, int *pVal) {
  // Parse a non-negative integer (simple implementation sufficient for tests)
  int val = 0;
  if (!z || !*z) return 0;
  const unsigned char *p = (const unsigned char*)z;
  // Optional sign support (not required for tests but harmless)
  int neg = 0;
  if (*p == '-') { neg = 1; p++; }
  while (*p && sqlite3Isdigit(*p)) {
    val = val * 10 + (*p - '0');
    p++;
  }
  if (neg) val = -val;
  *pVal = val;
  return 1;
}

// Forward declaration of the focal function (re-implemented here for unit testing)
static char sqlite3AffinityType(const char *zIn, Column *pCol);

// Implementation of the focal method (copied/replicated for isolated unit testing)
static char sqlite3AffinityType(const char *zIn, Column *pCol){
  uint32_t h = 0;
  char aff = SQLITE_AFF_NUMERIC;
  const char *zChar = 0;
  assert( zIn!=0 );
  while( zIn[0] ){
    h = (h<<8) + sqlite3UpperToLower[(unsigned char)(*zIn)&0xff];
    zIn++;
    if( h==(('c'<<24)+('h'<<16)+('a'<<8)+'r') ){             /* CHAR */
      aff = SQLITE_AFF_TEXT;
      zChar = zIn;
    }else if( h==(('c'<<24)+('l'<<16)+('o'<<8)+'b') ){       /* CLOB */
      aff = SQLITE_AFF_TEXT;
    }else if( h==(('t'<<24)+('e'<<16)+('x'<<8)+'t') ){       /* TEXT */
      aff = SQLITE_AFF_TEXT;
    }else if( h==(('b'<<24)+('l'<<16)+('o'<<8)+'b')          /* BLOB */
        && (aff==SQLITE_AFF_NUMERIC || aff==SQLITE_AFF_REAL) ){
      aff = SQLITE_AFF_BLOB;
      if( zIn[0]=='(' ) zChar = zIn;
#ifndef SQLITE_OMIT_FLOATING_POINT
    }else if( h==(('r'<<24)+('e'<<16)+('a'<<8)+'l')          /* REAL */
        && aff==SQLITE_AFF_NUMERIC ){
      aff = SQLITE_AFF_REAL;
    }else if( h==(('f'<<24)+('l'<<16)+('o'<<8)+'a')          /* FLOA */
        && aff==SQLITE_AFF_NUMERIC ){
      aff = SQLITE_AFF_REAL;
    }else if( h==(('d'<<24)+('o'<<16)+('u'<<8)+'b')          /* DOUB */
        && aff==SQLITE_AFF_NUMERIC ){
      aff = SQLITE_AFF_REAL;
#endif
    }else if( (h&0x00FFFFFF)==(('i'<<16)+('n'<<8)+'t') ){    /* INT */
      aff = SQLITE_AFF_INTEGER;
      break;
    }
  }
  /* If pCol is not NULL, store an estimate of the field size.  The
  ** estimate is scaled so that the size of an integer is 1.  */
  if( pCol ){
    int v = 0;   /* default size is approx 4 bytes */
    if( aff<SQLITE_AFF_NUMERIC ){
      if( zChar ){
        while( zChar[0] ){
          if( sqlite3Isdigit(zChar[0]) ){
            /* BLOB(k), VARCHAR(k), CHAR(k) -> r=(k/4+1) */
            sqlite3GetInt32(zChar, &v);
            break;
          }
          zChar++;
        }
      }else{
        v = 16;   /* BLOB, TEXT, CLOB -> r=5  (approx 20 bytes)*/
      }
    }
#ifdef SQLITE_ENABLE_SORTER_REFERENCES
    if( v>=sqlite3GlobalConfig.szSorterRef ){
      pCol->colFlags |= COLFLAG_SORTERREF;
    }
#endif
    v = v/4 + 1;
    if( v>255 ) v = 255;
    pCol->szEst = v;
  }
  return aff;
}

// Test framework: lightweight non-terminating assertions
static int g_failures = 0;
static void testFail(const char* msg) {
  fprintf(stderr, "Test failure: %s\n", msg);
  g_failures++;
}
#define EXPECT_EQ(expected, actual, msg) do { \
  if( (expected) != (actual) ){ \
    char buf[256]; \
    snprintf(buf, sizeof(buf), "Expected %d, got %d. %s", (int)(expected), (int)(actual), (msg)); \
    testFail(buf); \
  } \
} while(0)

static void testCase_CharacterCharMapping() {
  // Purpose: Verify CHAR -> TEXT affinity and default behavior (no size pCol)
  Column c; c.colFlags = 0; c.szEst = 0;

  char aff = sqlite3AffinityType("CHAR", nullptr);
  EXPECT_EQ(SQLITE_AFF_TEXT, aff, "CHAR should map to TEXT affinity");
}

static void testCase_CharWithParensSizeEst() {
  // CHAR(10) should map to TEXT and estimate size = 3 (10/4 + 1)
  Column c; c.colFlags = 0; c.szEst = 0;
  char aff = sqlite3AffinityType("CHAR(10)", &c);
  EXPECT_EQ(SQLITE_AFF_TEXT, aff, "CHAR(10) should map to TEXT affinity");
  EXPECT_EQ(3, c.szEst, "CHAR(10) should yield size estimate 3");
}

static void testCase_ClobAndTextVariants() {
  Column c;
  c.colFlags = 0; c.szEst = 0;
  EXPECT_EQ(SQLITE_AFF_TEXT, sqlite3AffinityType("CLOB", nullptr), "CLOB should map to TEXT affinity");
  EXPECT_EQ(SQLITE_AFF_TEXT, sqlite3AffinityType("TEXT", nullptr), "TEXT should map to TEXT affinity");
  // Case-insensitive mapping
  EXPECT_EQ(SQLITE_AFF_TEXT, sqlite3AffinityType("ChAr", nullptr), "ChAr should map to TEXT affinity (case-insensitive)");
}

static void testCase_BlobWithParensSizeEst() {
  Column c; c.colFlags = 0; c.szEst = 0;
  char aff = sqlite3AffinityType("BLOB(20)", &c);
  EXPECT_EQ(SQLITE_AFF_BLOB, aff, "BLOB(20) should map to BLOB affinity");
  // szEst path: due to the inner logic, with current test constants, ensure a valid est value is computed
  EXPECT_EQ(6, c.szEst, "BLOB(20) should yield size estimate 6 (20/4 + 1)");
}

static void testCase_BlobNoParensSizeEstDefault() {
  Column c; c.colFlags = 0; c.szEst = 0;
  char aff = sqlite3AffinityType("BLOB", &c);
  EXPECT_EQ(SQLITE_AFF_BLOB, aff, "BLOB should map to BLOB affinity");
  // Without parentheses, zChar==0, so default est path should yield 1
  EXPECT_EQ(1, c.szEst, "BLOB without size should yield est 1");
}

static void testCase_IntegerAffinity() {
  // INT should map to INTEGER affinity
  char aff = sqlite3AffinityType("INT", nullptr);
  EXPECT_EQ(SQLITE_AFF_INTEGER, aff, "INT should map to INTEGER affinity");
}

static void testCase_RealAffinity() {
  // REAL, FLOAT, DOUB should map to REAL in the presence of numeric state
  EXPECT_EQ(SQLITE_AFF_REAL, sqlite3AffinityType("REAL", nullptr), "REAL should map to REAL affinity");
  EXPECT_EQ(SQLITE_AFF_REAL, sqlite3AffinityType("FLOAT", nullptr), "FLOAT should map to REAL affinity");
  EXPECT_EQ(SQLITE_AFF_REAL, sqlite3AffinityType("DOUB", nullptr), "DOUB should map to REAL affinity");
}

static void testCase_UnknownAffinity() {
  // Unknown type should remain NUMERIC (default)
  char aff = sqlite3AffinityType("UNKNOWN", nullptr);
  EXPECT_EQ(SQLITE_AFF_NUMERIC, aff, "UNKNOWN should map to NUMERIC affinity");
}

// Populate the sqlite3UpperToLower mapping with lowercase equivalents
static void initUpperToLower() {
  for(int i=0; i<256; ++i){
    sqlite3UpperToLower[i] = (unsigned char)tolower(i);
  }
}

// Macro to drive all tests
static void runAllTests() {
  initUpperToLower();

  testCase_CharacterCharMapping();
  testCase_CharWithParensSizeEst();
  testCase_ClobAndTextVariants();
  testCase_BlobWithParensSizeEst();
  testCase_BlobNoParensSizeEstDefault();
  testCase_IntegerAffinity();
  testCase_RealAffinity();
  testCase_UnknownAffinity();

  if( g_failures == 0 ){
    printf("All tests passed.\n");
  }else{
    printf("Tests completed with %d failure(s).\n", g_failures);
  }
}

int main() {
  // Run the test suite
  runAllTests();
  return (g_failures == 0) ? 0 : 1;
}