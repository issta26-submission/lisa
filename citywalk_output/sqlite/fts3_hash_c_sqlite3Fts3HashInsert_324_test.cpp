/*
  C++11 test suite for sqlite3Fts3HashInsert (cts: non-GTest, standalone harness)

  This test suite exercises the focal method sqlite3Fts3HashInsert from the
  fts3_hash.c implementation used by SQLite's FTS3 hashing mechanism.

  Approach:
  - Use the public C API exposed by fts3_hash.h to create and manipulate
    Fts3Hash instances.
  - Validate both true/false branches of key predicates by driving:
    - Insertion into an empty/initial table (rehash path)
    - Insert of new keys
    - Update of existing key data
    - Deletion (removal) of an existing key by inserting NULL data
    - Searching via sqlite3Fts3HashFindElem to inspect internal element
      properties (e.g., copyKey behavior)
  - Confirm copyKey semantics by inspecting the actual stored key pointer
    through sqlite3Fts3HashFindElem.
  - Provide a lightweight, non-terminating assertion mechanism (EXPECT_* macros)
    to maximize coverage without stopping at the first failure.

  Notes:
  - All tests are written in C++11 but rely on the C API of sqlite3Fts3Hash*.
  - We rely on the pre-declared functions from fts3_hash.h:
      sqlite3Fts3HashInit, sqlite3Fts3HashInsert, sqlite3Fts3HashFindElem,
      sqlite3Fts3HashFind, sqlite3Fts3HashClear
  - The static helpers in fts3_hash.c (e.g., ftsHashMalloc) are part of that file;
    the test harness interacts through the public interface only.
*/

#include <fts3Int.h>
#include <stdlib.h>
#include <string.h>
#include <fts3_hash.h>
#include <assert.h>
#include <cstring>
#include <iostream>
#include <cstdint>


extern "C" {
}

// Simple non-terminating test assertions
static int g_failures = 0;

#define EXPECT_TRUE(cond) \
  do { \
    if(!(cond)) { \
      ++g_failures; \
      std::cerr << "EXPECT_TRUE failed: " #cond \
                << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
  } while(0)

#define EXPECT_EQ(a,b) \
  do { \
    if(!((a) == (b))) { \
      ++g_failures; \
      std::cerr << "EXPECT_EQ failed: " #a " == " #b \
                << " (actual: " << (a) << ", expected: " << (b) << ")" \
                << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
  } while(0)

#define EXPECT_EQ_PTR(a,b) \
  do { \
    if(((void*)(a)) != ((void*)(b))) { \
      ++g_failures; \
      std::cerr << "EXPECT_EQ_PTR failed: " #a " == " #b \
                << " (actual: " << (void*)(a) << ", expected: " << (void*)(b) << ")" \
                << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
  } while(0)

static void test_basic_insert_and_find_with_copy_true() {
  // Test: insertion into an initially empty hash with copyKey = true.
  // Expectation:
  // - sqlite3Fts3HashInsert returns 0 (no old data)
  // - sqlite3Fts3HashFindElem locates the element
  // - The stored data equals the original data pointer
  // - The stored key is a copied Key (pointer differs from input)
  Fts3Hash h;
  sqlite3Fts3HashInit(&h, 'S', 1); // keyClass 'S' (string), copyKey true

  const char keyA[] = "ABCD";
  int nA = 4;
  void* dataA = (void*)0x1001;

  void* ret = sqlite3Fts3HashInsert(&h, keyA, nA, dataA);
  EXPECT_EQ(ret, nullptr); // no old data on first insert

  Fts3HashElem* elem = sqlite3Fts3HashFindElem(&h, keyA, nA);
  EXPECT_TRUE(elem != nullptr);
  EXPECT_EQ(elem->data, dataA);

  // Copy semantics: stored key pointer should differ from input key
  EXPECT_TRUE(elem->pKey != (void*)keyA);
  if(elem->pKey) {
    // Key bytes should match
    EXPECT_EQ(memcmp(elem->pKey, keyA, nA), 0);
  }

  sqlite3Fts3HashClear(&h);
}

static void test_insert_update_and_remove() {
  // Test: insert a key, update its data, then remove it (data == 0)
  Fts3Hash h;
  sqlite3Fts3HashInit(&h, 'S', 0); // keyClass 'S', copyKey false

  const char keyA[] = "WXYZ";
  int nA = 4;
  void* data1 = (void*)0x2001;
  void* data2 = (void*)0x3001;

  // Insert first time
  void* ret1 = sqlite3Fts3HashInsert(&h, keyA, nA, data1);
  EXPECT_EQ(ret1, nullptr);

  // Retrieve and verify
  Fts3HashElem* elem = sqlite3Fts3HashFindElem(&h, keyA, nA);
  EXPECT_TRUE(elem != nullptr);
  EXPECT_EQ(elem->data, data1);
  EXPECT_EQ_PTR(elem->pKey, (void*)keyA); // copyKey=false => same key pointer

  // Update data for existing key
  void* retUpdate = sqlite3Fts3HashInsert(&h, keyA, nA, data2);
  EXPECT_EQ(retUpdate, data1);

  elem = sqlite3Fts3HashFindElem(&h, keyA, nA);
  EXPECT_TRUE(elem != nullptr);
  EXPECT_EQ(elem->data, data2);

  // Remove existing key (data == 0)
  void* retRemove = sqlite3Fts3HashInsert(&h, keyA, nA, 0);
  EXPECT_EQ(retRemove, data2);

  elem = sqlite3Fts3HashFindElem(&h, keyA, nA);
  EXPECT_TRUE(elem == nullptr); // should be removed

  sqlite3Fts3HashClear(&h);
}

static void test_rehash_on_empty_table() {
  // Test: htSize == 0 triggers a rehash that returns data rather than inserting
  Fts3Hash h;
  sqlite3Fts3HashInit(&h, 'S', 1); // copyKey true to trigger code path for new entries

  const char keyR[] = "REHASH";
  int nR = 6;
  void* dataR = (void*)0x4001;

  void* ret = sqlite3Fts3HashInsert(&h, keyR, nR, dataR);
  // If rehash occurred, should return dataR (non-null)
  EXPECT_EQ(ret, dataR);

  // No element should be present for keyR yet
  Fts3HashElem* elem = sqlite3Fts3HashFindElem(&h, keyR, nR);
  EXPECT_TRUE(elem == nullptr);

  // count should be reset to 0 after rehash path
  // (access to pH->count is not directly exposed; rely on no element presence)
  sqlite3Fts3HashClear(&h);
}

static void test_insert_no_effect_when_key_absent_and_data_null() {
  // Test: data == NULL for a key that does not exist should do nothing
  Fts3Hash h;
  sqlite3Fts3HashInit(&h, 'S', 1);

  const char keyN[] = "NOPE";
  int nN = 4;

  void* ret = sqlite3Fts3HashInsert(&h, keyN, nN, nullptr);
  EXPECT_EQ(ret, nullptr);

  Fts3HashElem* elem = sqlite3Fts3HashFindElem(&h, keyN, nN);
  EXPECT_TRUE(elem == nullptr);

  sqlite3Fts3HashClear(&h);
}

static void test_copyKey_true_semantics() {
  // Ensure that with copyKey = true, the stored key is a copy (not the same pointer)
  Fts3Hash h;
  sqlite3Fts3HashInit(&h, 'S', 1);

  const char keyC[] = "COPYK1";
  int nC = 6;
  void* dataC = (void*)0x5001;

  void* ret = sqlite3Fts3HashInsert(&h, keyC, nC, dataC);
  EXPECT_EQ(ret, nullptr);

  Fts3HashElem* elem = sqlite3Fts3HashFindElem(&h, keyC, nC);
  EXPECT_TRUE(elem != nullptr);
  EXPECT_TRUE(elem->pKey != (void*)keyC);
  EXPECT_EQ(memcmp(elem->pKey, keyC, nC), 0);

  sqlite3Fts3HashClear(&h);
}

static void test_copyKey_false_semantics() {
  // Ensure that with copyKey = false, the stored key pointer is the same as input
  Fts3Hash h;
  sqlite3Fts3HashInit(&h, 'S', 0);

  const char keyD[] = "COPYK2";
  int nD = 6;
  void* dataD = (void*)0x6001;

  void* ret = sqlite3Fts3HashInsert(&h, keyD, nD, dataD);
  EXPECT_EQ(ret, nullptr);

  Fts3HashElem* elem = sqlite3Fts3HashFindElem(&h, keyD, nD);
  EXPECT_TRUE(elem != nullptr);
  EXPECT_EQ_PTR(elem->pKey, (void*)keyD);

  sqlite3Fts3HashClear(&h);
}

// Simple test runner
static void run_all_tests() {
  std::cout << "Starting sqlite3Fts3HashInsert test suite (C++11 harness)..." << std::endl;

  test_basic_insert_and_find_with_copy_true();
  test_insert_update_and_remove();
  test_rehash_on_empty_table();
  test_insert_no_effect_when_key_absent_and_data_null();
  test_copyKey_true_semantics();
  test_copyKey_false_semantics();

  if(g_failures == 0) {
    std::cout << "All tests PASSED." << std::endl;
  } else {
    std::cout << g_failures << " tests FAILED." << std::endl;
  }
}

int main() {
  run_all_tests();
  return g_failures != 0 ? 1 : 0;
}