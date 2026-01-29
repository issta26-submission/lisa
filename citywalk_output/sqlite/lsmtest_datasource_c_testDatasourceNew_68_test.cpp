// C++11 test suite for the focal method: testDatasourceNew
// This test suite is self-contained and does not rely on GTest.
// It re-creates the minimal production-like environment to exercise
// the exact logic in the provided focal method.

#include <lsmtest.h>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Domain-specific constants and types (reconstructed for test purposes)
#ifndef MAX
#define MAX(a,b) (( (a) > (b) ) ? (a) : (b))
#endif

// Emulated Datasource structures matching the focal method's usage
typedef struct Datasource {
  int eType;
  int nMinKey;
  int nMinVal;
  int nMaxKey;
  int nMaxVal;
  char *aKey;
  char *aVal;
} Datasource;

typedef struct DatasourceDefn {
  int eType;
  int nMinKey;
  int nMinVal;
  int nMaxKey;
  int nMaxVal;
} DatasourceDefn;

// Enum placeholder for data source type
enum {
  TEST_DATASOURCE_SEQUENCE = 1
};

// Simple test malloc wrapper to mimic test environment memory allocation.
// In production, testMalloc could have tracking; here we simply wrap malloc.
static void* testMalloc(size_t sz) {
  void* p = std::malloc(sz);
  if(p == nullptr) {
    std::fprintf(stderr, "testMalloc failed to allocate %zu bytes\n", sz);
    std::exit(1);
  }
  std::memset(p, 0, sz); // Optional: zero-fill to catch misuses
  return p;
}

// Focal method under test (reproduced to make the unit tests executable here)
Datasource *testDatasourceNew(const DatasourceDefn *pDefn){
  Datasource *p;
  int nMinKey; 
  int nMaxKey;
  int nMinVal;
  int nMaxVal; 
  if( pDefn->eType==TEST_DATASOURCE_SEQUENCE ){
    nMinKey = 128;
    nMaxKey = 128;
  }else{
    nMinKey = MAX(0, pDefn->nMinKey);
    nMaxKey = MAX(nMinKey, pDefn->nMaxKey);
  }
  nMinVal = MAX(0, pDefn->nMinVal);
  nMaxVal = MAX(nMinVal, pDefn->nMaxVal);
  p = (Datasource *)testMalloc(sizeof(Datasource) + nMaxKey + nMaxVal + 1);
  p->eType = pDefn->eType;
  p->nMinKey = nMinKey;
  p->nMinVal = nMinVal;
  p->nMaxKey = nMaxKey;
  p->nMaxVal = nMaxVal;
  
  p->aKey = (char *)&p[1];
  p->aVal = &p->aKey[nMaxKey];
  return p;
};

// Simple test harness allowing non-terminating assertions
static int g_totalTests = 0;
static int g_failedTests = 0;

#define EXPECT_EQ_INT(a,b) do { \
  ++g_totalTests; \
  if ((a) != (b)) { \
    ++g_failedTests; \
    std::fprintf(stderr, "EXPECT_EQ_INT failed: %s == %s (actual %d vs %d) at %s:%d\n", #a, #b, (a), (b), __FILE__, __LINE__); \
  } \
} while(0)

#define EXPECT_PTR_EQ(a,b) do { \
  ++g_totalTests; \
  if ((void*)(a) != (void*)(b)) { \
    ++g_failedTests; \
    std::fprintf(stderr, "EXPECT_PTR_EQ failed: %s == %s (actual %p vs %p) at %s:%d\n", #a, #b, (void*)(a), (void*)(b), __FILE__, __LINE__); \
  } \
} while(0)

#define EXPECT_NOT_NULL(a) do { \
  ++g_totalTests; \
  if ((a) == nullptr) { \
    ++g_failedTests; \
    std::fprintf(stderr, "EXPECT_NOT_NULL failed: %s is null at %s:%d\n", #a, __FILE__, __LINE__); \
  } \
} while(0)

// Test 1: Sequence type should fix nMinKey and nMaxKey to 128, and compute min/max values correctly
static void test_sequence_type_values() {
  DatasourceDefn defn;
  defn.eType = TEST_DATASOURCE_SEQUENCE;
  defn.nMinKey = 0;     // ignored for sequence path
  defn.nMinVal = -2;    // should be clamped to 0
  defn.nMaxKey = 999;   // ignored for sequence path
  defn.nMaxVal = 3;     // used to compute nMaxVal

  Datasource *p = testDatasourceNew(&defn);
  EXPECT_NOT_NULL(p);

  // Basic type and bounds checks
  EXPECT_EQ_INT(p->eType, defn.eType);
  EXPECT_EQ_INT(p->nMinKey, 128);
  EXPECT_EQ_INT(p->nMaxKey, 128);

  int expectedNMinVal = MAX(0, defn.nMinVal);        // 0 in this case
  int expectedNMaxVal = MAX(expectedNMinVal, defn.nMaxVal); // MAX(0, 3) => 3
  EXPECT_EQ_INT(p->nMinVal, expectedNMinVal);
  EXPECT_EQ_INT(p->nMaxVal, expectedNMaxVal);

  // Pointer arithmetic checks
  EXPECT_PTR_EQ(p->aKey, reinterpret_cast<char*>(p + 1));
  EXPECT_PTR_EQ(p->aVal, p->aKey + p->nMaxKey);

  std::free(p);
}

// Test 2: Non-sequence type should honor provided min/max ranges with clamping
static void test_non_sequence_values() {
  DatasourceDefn defn;
  defn.eType = 0;           // non-sequence
  defn.nMinKey = -5;         // should clamp to 0
  defn.nMinVal = 1;          // positive
  defn.nMaxKey = 2;           // small range
  defn.nMaxVal = 0;           // possibly less than nMinVal

  Datasource *p = testDatasourceNew(&defn);
  EXPECT_NOT_NULL(p);

  EXPECT_EQ_INT(p->eType, defn.eType);
  EXPECT_EQ_INT(p->nMinKey, 0);
  EXPECT_EQ_INT(p->nMaxKey, 2);
  EXPECT_EQ_INT(p->nMinVal, 1);
  EXPECT_EQ_INT(p->nMaxVal, 1); // MAX(1, 0) => 1

  // Pointer checks
  EXPECT_PTR_EQ(p->aKey, reinterpret_cast<char*>(p + 1));
  EXPECT_PTR_EQ(p->aVal, p->aKey + p->nMaxKey);

  std::free(p);
}

// Test 3: Edge case where all values are forced to zero due to negatives
static void test_edge_zero_values_sequence() {
  DatasourceDefn defn;
  defn.eType = TEST_DATASOURCE_SEQUENCE;
  defn.nMinKey = 0;      // ignored
  defn.nMinVal = -100;   // clamped to 0
  defn.nMaxKey = 0;      // ignored
  defn.nMaxVal = -50;     // MAX(0, -50) => 0

  Datasource *p = testDatasourceNew(&defn);
  EXPECT_NOT_NULL(p);

  EXPECT_EQ_INT(p->eType, defn.eType);
  EXPECT_EQ_INT(p->nMinKey, 128);
  EXPECT_EQ_INT(p->nMaxKey, 128);
  EXPECT_EQ_INT(p->nMinVal, 0);
  EXPECT_EQ_INT(p->nMaxVal, 0);

  EXPECT_PTR_EQ(p->aKey, reinterpret_cast<char*>(p + 1));
  EXPECT_PTR_EQ(p->aVal, p->aKey + p->nMaxKey);

  std::free(p);
}

// Test 4: Non-sequence with increasing min/val constraints
static void test_non_sequence_increasing_minmax() {
  DatasourceDefn defn;
  defn.eType = 0;
  defn.nMinKey = 5;
  defn.nMinVal = 5;
  defn.nMaxKey = 10;
  defn.nMaxVal = 7;

  Datasource *p = testDatasourceNew(&defn);
  EXPECT_NOT_NULL(p);

  EXPECT_EQ_INT(p->eType, defn.eType);
  EXPECT_EQ_INT(p->nMinKey, 5);
  EXPECT_EQ_INT(p->nMaxKey, 10);
  EXPECT_EQ_INT(p->nMinVal, 5);
  EXPECT_EQ_INT(p->nMaxVal, 7);

  EXPECT_PTR_EQ(p->aKey, reinterpret_cast<char*>(p + 1));
  EXPECT_PTR_EQ(p->aVal, p->aKey + p->nMaxKey);

  std::free(p);
}

int main() {
  // Run all test scenarios
  test_sequence_type_values();
  test_non_sequence_values();
  test_edge_zero_values_sequence();
  test_non_sequence_increasing_minmax();

  // Summary
  std::cout << "Unit Test Summary: " << g_totalTests << " tests run, "
            << g_failedTests << " failures." << std::endl;

  // Return non-zero if any test failed
  return (g_failedTests != 0) ? 1 : 0;
}