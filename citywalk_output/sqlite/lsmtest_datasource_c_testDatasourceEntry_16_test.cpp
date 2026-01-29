/*
  Lightweight C++11 test harness for the focal function testDatasourceEntry
  - No external test framework (GTest) used.
  - Includes a small in-house test harness with non-terminating checks.
  - Covers key scenarios for TEST_DATASOURCE_RANDOM and TEST_DATASOURCE_SEQUENCE.
  - Verifies both presence/absence of key/value outputs and correctness of produced data
    using the project's own testPrngValue/testPrngString helpers.
*/

#include <lsmtest.h>
#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>


extern "C" {
  // Declaration of focal function and related items from the C source/header
  // Assuming lsmtest.h provides these declarations in the actual project.
  // If the declarations differ, adjust accordingly.
  #include "lsmtest.h" // Provides Datasource, TEST_DATASOURCE_RANDOM, TEST_DATASOURCE_SEQUENCE, etc.
  void testDatasourceEntry(Datasource *p, int iData,
                         void **ppKey, int *pnKey,
                         void **ppVal, int *pnVal);
  // PRNG helpers used by the focal method to generate expected results
  unsigned int testPrngValue(unsigned int seed);
  void testPrngString(unsigned int seed, char *dest, int n);
}

static int g_tests = 0;
static int g_failures = 0;

#define CHECK_EQ(a, b, msg) do { ++g_tests; if (!((a) == (b))) { ++g_failures; std::cerr << "FAIL: " << msg \
    << " | " << #a << " (= " << (a) << "), " << #b << " (= " << (b) << ")\n"; } } while(0)
#define CHECK_PTR_EQ(a, b, msg) do { ++g_tests; if ((a) != (b)) { ++g_failures; std::cerr << "FAIL: " << msg \
    << " | " << #a << " (= " << (a) << "), " << #b << " (= " << (b) << ")\n"; } } while(0)

static void test_null_pointers_case() {
  // Purpose: Ensure no crashes and no side-effects when no key/val requested
  Datasource p;
  // Configure a representative datasource
  p.eType = TEST_DATASOURCE_RANDOM;
  p.nMinKey = 0;
  p.nMaxKey = 9;
  p.nMinVal = 0;
  p.nMaxVal = 9;
  // Buffers are not used in this case
  testDatasourceEntry(&p, 7, nullptr, nullptr, nullptr, nullptr);
  // If we reach here, the call with all null pointers should be safe
  // No additional assertions needed; presence of asserts inside the focal function is avoided by input choice
  // We still record a successful test
  ++g_tests;
}

static void test_key_only_random_case(int iData) {
  // Purpose: Validate that when only the key is requested for RANDOM type, the function
  // writes the correct key pointer and length, and populates aKey accordingly.
  Datasource p;
  p.eType = TEST_DATASOURCE_RANDOM;
  p.nMinKey = 0;
  p.nMaxKey = 9;
  p.nMinVal = 0;
  p.nMaxVal = 9;

  void* ppKey = nullptr;
  int pnKey = 0;
  void* ppVal = nullptr;
  int pnVal = 0;

  testDatasourceEntry(&p, iData, &ppKey, &pnKey, nullptr, nullptr);

  // Key pointer should point to the datasource's aKey buffer
  CHECK_PTR_EQ(ppKey, (void*)p.aKey, "Key pointer should point to p.aKey after RANDOM key generation");

  // Compute expected key length
  unsigned int nRange = (unsigned int)(1 + p.nMaxKey - p.nMinKey);
  unsigned int seed = testPrngValue((unsigned int)iData);
  int expectedNKey = (int)(seed % nRange) + p.nMinKey;

  CHECK_EQ(pnKey, expectedNKey, "Computed key length (pnKey) should match expected for RANDOM path");

  // Verify the content of aKey matches testPrngString output
  char expectedKeyBuf[256];
  testPrngString((unsigned int)iData, expectedKeyBuf, expectedNKey);
  // Compare exact bytes written (no terminator assumption)
  int cmp = std::memcmp(p.aKey, expectedKeyBuf, expectedNKey);
  CHECK_EQ(cmp, 0, "Key content should match expected RNG-derived value for RANDOM path");
}

static void test_key_and_val_random_case(int iData) {
  // Purpose: Validate that when both key and value are requested for RANDOM type,
  // both outputs are correct and consistent with RNG/seeds.
  Datasource p;
  p.eType = TEST_DATASOURCE_RANDOM;
  p.nMinKey = 0;
  p.nMaxKey = 9;
  p.nMinVal = 0;
  p.nMaxVal = 9;

  void* ppKey = nullptr;
  int pnKey = 0;
  void* ppVal = nullptr;
  int pnVal = 0;

  testDatasourceEntry(&p, iData, &ppKey, &pnKey, &ppVal, &pnVal);

  // Validate key presence and correctness
  CHECK_PTR_EQ(ppKey, (void*)p.aKey, "Key pointer should point to p.aKey when both outputs requested");
  unsigned int nRange = (unsigned int)(1 + p.nMaxKey - p.nMinKey);
  unsigned int seed = testPrngValue((unsigned int)iData);
  int expectedNKey = (int)(seed % nRange) + p.nMinKey;
  CHECK_EQ(pnKey, expectedNKey, "Key length should match expected for RANDOM path");

  // Validate key content
  char expectedKeyBuf[256];
  testPrngString((unsigned int)iData, expectedKeyBuf, expectedNKey);
  int cmpKey = std::memcmp(p.aKey, expectedKeyBuf, expectedNKey);
  CHECK_EQ(cmpKey, 0, "Key content should match RNG-derived value for RANDOM path");

  // Validate value presence and correctness
  unsigned int nVal = testPrngValue((unsigned int)iData) % (1 + p.nMaxVal - p.nMinVal) + p.nMinVal;
  char expectedValBuf[256];
  testPrngString((unsigned int)~(unsigned int)iData, expectedValBuf, (int)nVal);

  CHECK_PTR_EQ(ppVal, (void*)p.aVal, "Value pointer should point to p.aVal when both outputs requested");
  CHECK_EQ(pnVal, (int)nVal, "Value length (pnVal) should equal computed nVal");

  int cmpVal = std::memcmp(p.aVal, expectedValBuf, (size_t)nVal);
  CHECK_EQ(cmpVal, 0, "Value content should match RNG-derived value for RANDOM path");
}

static void test_key_only_sequence_case(int iData) {
  // Purpose: Validate key generation for SEQUENCE path
  Datasource p;
  p.eType = TEST_DATASOURCE_SEQUENCE;
  p.nMinKey = 0;
  p.nMaxKey = 9;
  p.nMinVal = 0;
  p.nMaxVal = 9;

  void* ppKey = nullptr;
  int pnKey = 0;
  void* ppVal = nullptr;
  int pnVal = 0;

  testDatasourceEntry(&p, iData, &ppKey, &pnKey, nullptr, nullptr);

  // Key pointer should point to the aKey buffer
  CHECK_PTR_EQ(ppKey, (void*)p.aKey, "Key pointer should point to p.aKey for SEQUENCE path");

  // Expected length and content using sprintf("%012d")
  char tmpBuf[256];
  int expectedNKey = std::sprintf(tmpBuf, "%012d", iData);
  CHECK_EQ(pnKey, expectedNKey, "Key length should equal 12 for 0-padded SEQUENCE");

  int cmpKey = std::memcmp(p.aKey, tmpBuf, (size_t)expectedNKey);
  CHECK_EQ(cmpKey, 0, "Key content should be 12-digit zero-padded sequence for SEQUENCE path");
}

static void test_key_and_val_sequence_case(int iData) {
  // Purpose: Validate both key and value for SEQUENCE path
  Datasource p;
  p.eType = TEST_DATASOURCE_SEQUENCE;
  p.nMinKey = 0;
  p.nMaxKey = 9;
  p.nMinVal = 0;
  p.nMaxVal = 9;

  void* ppKey = nullptr;
  int pnKey = 0;
  void* ppVal = nullptr;
  int pnVal = 0;

  testDatasourceEntry(&p, iData, &ppKey, &pnKey, &ppVal, &pnVal);

  // Key checks
  CHECK_PTR_EQ(ppKey, (void*)p.aKey, "Key pointer should point to p.aKey for SEQUENCE path");
  char tmpBuf[256];
  int expectedNKey = std::sprintf(tmpBuf, "%012d", iData);
  CHECK_EQ(pnKey, expectedNKey, "Key length should equal 12 for SEQUENCE path");
  int cmpKey = std::memcmp(p.aKey, tmpBuf, (size_t)expectedNKey);
  CHECK_EQ(cmpKey, 0, "Key content should match SEQUENCE-generated value");

  // Value checks
  unsigned int nVal = testPrngValue((unsigned int)iData) % (1 + p.nMaxVal - p.nMinVal) + p.nMinVal;
  char expectedValBuf[256];
  testPrngString((unsigned int)~(unsigned int)iData, expectedValBuf, (int)nVal);

  CHECK_PTR_EQ(ppVal, (void*)p.aVal, "Value pointer should point to p.aVal for SEQUENCE path");
  CHECK_EQ(pnVal, (int)nVal, "Value length should equal computed nVal for SEQUENCE path");

  int cmpVal = std::memcmp(p.aVal, expectedValBuf, (size_t)nVal);
  CHECK_EQ(cmpVal, 0, "Value content should match RNG-derived value for SEQUENCE path");
}

static void run_all_tests() {
  test_null_pointers_case();

  // Test Random path: two iData samples to cover different branches
  test_key_only_random_case(0);
  test_key_only_random_case(7);

  test_key_and_val_random_case(0);
  test_key_and_val_random_case(7);

  // Test Sequence path: two iData samples
  test_key_only_sequence_case(0);
  test_key_only_sequence_case(123);
  test_key_and_val_sequence_case(0);
  test_key_and_val_sequence_case(123);
}

int main() {
  run_all_tests();
  std::cout << "Tests run: " << g_tests << ", Failures: " << g_failures << std::endl;
  return (g_failures == 0) ? 0 : 1;
}