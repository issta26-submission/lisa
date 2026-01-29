// C++11 test suite for sqlite3UnlinkAndDeleteIndex (no GoogleTest; plain C++ test harness)

// This test suite uses a minimal mock of the sqlite3 index/hash/table structures
// and provides stub implementations of the dependencies required by
// sqlite3UnlinkAndDeleteIndex to exercise its logic (head removal and non-head removal).

#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <unordered_map>
#include <iostream>


// Forward declare the C symbol under test
extern "C" void sqlite3UnlinkAndDeleteIndex(void *db, int iDb, const char *zIdxName);

// Minimal opaque C types to match signatures used by the focal method.
// We only rely on pointers and do not access internals in the test harness.
typedef struct sqlite3 sqlite3;
typedef struct Hash Hash;
typedef struct Index Index;
typedef struct Table Table;

// Mocked to satisfy sqlite3UnlinkAndDeleteIndex internals
// Note: We do not attempt to mirror full SQLite layout; we only support the needed pointers.
struct Index {
  Table *pTable;
  Index *pNext;
  const char *zName;
};

struct Table {
  Index *pIndex; // head of the index list
};

struct Schema {
  Hash idxHash;
};

struct Db {
  Schema *pSchema;
};

struct sqlite3 {
  Db aDb[2];
  int mDbFlags;
};

// Global state used by the mocks to simulate hash insert/unlink behavior
static std::unordered_map<void*, Table*> gHashToTable; // maps Hash* key to its Table
static std::unordered_map<void*, std::unordered_map<std::string, Index*>> gHashIndexMap; // per-Hash index map by name
static std::vector<Index*> gFreedIndices; // track freed indices (do not actually free to inspect)

#define TEST_DBFLAG_SchemaChange 0x01 // not used directly; instead we test that modification happened

// Forward declare the mock APIs expected by the focal method's implementation
extern "C" {
  // Simulated internal mutex check (always true for test)
  int sqlite3SchemaMutexHeld(sqlite3*, int, int);

  // Hash insert: returns an existing or newly created Index* for zIdxName
  Index* sqlite3HashInsert(Hash*, const char*, int);

  // Free index: remove from list and record for verification
  void sqlite3FreeIndex(sqlite3*, Index*);
}

// Implementation of mocks

extern "C" int sqlite3SchemaMutexHeld(sqlite3* db, int iDb, int flags) {
  // In tests, assume the schema mutex is always held
  (void)db; (void)iDb; (void)flags;
  return 1;
}

// sqlite3HashInsert mock: attaches new indices to a per-Hash Table and tracks in a per-Hash map
extern "C" Index* sqlite3HashInsert(Hash* pHash, const char* zIdxName, int dummy) {
  void* key = (void*)pHash;
  // Get or create the per-hash table
  Table* tbl;
  auto itTable = gHashToTable.find(key);
  if (itTable == gHashToTable.end()) {
    tbl = new Table{ nullptr };
    gHashToTable[key] = tbl;
  } else {
    tbl = itTable->second;
  }

  // Look up existing index by name for this hash
  auto &nameMap = gHashIndexMap[key];
  auto it = nameMap.find(zIdxName);
  if (it != nameMap.end()) {
    // Return existing index
    return it->second;
  }

  // Create a new index and insert at head of the table's index list
  Index* idx = new Index{ tbl, tbl->pIndex, zIdxName };
  tbl->pIndex = idx;
  nameMap[zIdxName] = idx;
  return idx;
}

// sqlite3FreeIndex mock: records the freed pointer (and unlinks it from the table)
extern "C" void sqlite3FreeIndex(sqlite3* db, Index* p) {
  if (p) {
    // Unlink from its table list if still connected
    if (p->pTable) {
      if (p->pTable->pIndex == p) {
        p->pTable->pIndex = p->pNext;
      } else {
        Index* prev = p->pTable->pIndex;
        while (prev && prev->pNext != p) {
          prev = prev->pNext;
        }
        if (prev && prev->pNext == p) {
          prev->pNext = p->pNext;
        }
      }
    }
  }
  // Track freed index for assertions; do not delete to allow inspection of fields in tests
  gFreedIndices.push_back(p);
  (void)db;
}

// Simple testing infrastructure (non-terminating assertions)

static int gFailures = 0;
static std::vector<std::string> gLog;

// Convenience assertion helpers (non-terminating)
#define EXPECT_TRUE(expr, msg) do { \
  if(!(expr)) { \
    gFailures++; \
    gLog.push_back(std::string("EXPECT_TRUE failed: ") + (msg)); \
  } \
} while(0)

#define EXPECT_STR_EQ(a, b, msg) do { \
  if(std::strcmp((a), (b)) != 0) { \
    gFailures++; \
    gLog.push_back(std::string("EXPECT_STR_EQ failed: ") + (msg) \
      + " | got \"" + (a) + "\" expected \"" + (b) + "\""); \
  } \
} while(0)

#define EXPECT_PTR_NE(a, b, msg) do { \
  if((void*)(a) == (void*)(b)) { \
    gFailures++; \
    gLog.push_back(std::string("EXPECT_PTR_NE failed: ") + (msg)); \
  } \
} while(0)

static void reset_mock_state() {
  gHashToTable.clear();
  gHashIndexMap.clear();
  gFreedIndices.clear();
  // Note: We do not free dynamically allocated objects to keep test introspection simple
}

// Helper to create a new simple Index with a given name bound to a table
static Index* make_index(Table* tbl, const char* name) {
  Index* idx = new Index{ tbl, nullptr, name };
  tbl->pIndex = idx;
  return idx;
}

// Test 1: True branch where the newly inserted index becomes the head and is unlink'ed
static void test_head_removal() {
  reset_mock_state();

  // Create db and minimal schema
  sqlite3* db = new sqlite3;
  db->mDbFlags = 0;
  Schema* s = new Schema;
  db->aDb[0].pSchema = s;

  // Prepare a simple table with a single existing index A (head)
  Table* t = new Table{ nullptr };
  Index* A = new Index{ t, nullptr, "A" };
  t->pIndex = A;

  // The hash for this test is the address of s->idxHash
  Hash* pHash = &(s->idxHash);

  // Ensure there is a hash-to-table mapping for the test hash (not strictly required, but clarifies intent)
  gHashToTable[(void*)pHash] = t;

  // Insert a new index "C" which will become the new head, but then get unlink'ed
  const char* newName = "C";

  // Call the focal method
  sqlite3UnlinkAndDeleteIndex((sqlite3*)db, 0, newName);

  // Assertions:
  // 1) The new index "C" should have been freed
  bool freedContainsC = false;
  for (auto idx : gFreedIndices) {
    if (idx && idx->zName && std::strcmp(idx->zName, "C") == 0) {
      freedContainsC = true;
      break;
    }
  }
  EXPECT_TRUE(freedContainsC, "Freed index should be the newly inserted 'C' index");

  // 2) The head of the index list should now be the old head A
  EXPECT_TRUE(t->pIndex == A, "Head of index list should be the original head 'A' after unlink");

  // 3) The original head's next should be nullptr (since C was the only new node)
  EXPECT_TRUE(A->pNext == nullptr, "Original head 'A' should have no next after unlink");

  // 4) The schema flag should be marked as changed (non-zero check, independent of exact bit position)
  EXPECT_TRUE(db->mDbFlags != 0, "Schema change flag should be set after unlink/delete");

  // Cleanup: do not delete allocated objects to keep inspection simple (omitted for brevity)
  // In a larger test suite, we would traverse and delete allocated objects here.

  // Print summary for this test
  if (!gFailures) {
    std::cout << "[TestHeadRemoval] PASSED\n";
  } else {
    std::cout << "[TestHeadRemoval] FAILED with " << gFailures << " failure(s)\n";
    for (const auto& m : gLog) std::cout << "  " << m << "\n";
  }
}

// Test 2: Else branch where the index is not the head (removal by bypassing via pIndex->pNext)
static void test_remove_non_head() {
  reset_mock_state();

  // Create db and schema
  sqlite3* db = new sqlite3;
  db->mDbFlags = 0;
  Schema* s = new Schema;
  db->aDb[0].pSchema = s;

  // Prepare a table with a list A -> B (A is head, B is next)
  Table* t = new Table{ nullptr };
  Index* A = new Index{ t, nullptr, "A" };
  Index* B = new Index{ t, nullptr, "B" };
  // Link A -> B
  A->pNext = B;
  t->pIndex = A;
  // Bind B to the same table as in the scenario
  B->pTable = t;
  A->pTable = t;

  // Prepare hash to contain B so that sqlite3HashInsert returns B (existing)
  Hash* pHash = &(s->idxHash);
  gHashToTable[(void*)pHash] = t;
  gHashIndexMap[(void*)pHash]["B"] = B;

  // Ensure there is a hash map for "A" if needed (not used in this test)
  gHashIndexMap[(void*)pHash]["A"] = A;

  // Call the focal method with zIdxName = "B" to trigger the non-head removal path
  sqlite3UnlinkAndDeleteIndex((sqlite3*)db, 0, "B");

  // Assertions:
  // 1) B should be freed
  bool freedB = false;
  for (auto idx : gFreedIndices) {
    if (idx && idx->zName && std::strcmp(idx->zName, "B") == 0) {
      freedB = true;
      break;
    }
  }
  EXPECT_TRUE(freedB, "Freed index should be non-head 'B'");

  // 2) List should now be just A (head) with no next
  EXPECT_TRUE(t->pIndex == A, "Head should remain 'A' after unlink of non-head");
  EXPECT_TRUE(A->pNext == nullptr, "After removing non-head, head's next should be nullptr");

  // 3) Schema change flag should be set
  EXPECT_TRUE(db->mDbFlags != 0, "Schema change flag should be set after unlink/delete");

  // Print summary
  if (!gFailures) {
    std::cout << "[TestRemoveNonHead] PASSED\n";
  } else {
    std::cout << "[TestRemoveNonHead] FAILED with " << gFailures << " failure(s)\n";
    for (const auto& m : gLog) std::cout << "  " << m << "\n";
  }
}

// Entry point to run tests
int main() {
  std::cout << "Running sqlite3UnlinkAndDeleteIndex unit tests (C++11, no GTest)..." << std::endl;

  // Run tests
  test_head_removal();
  test_remove_non_head();

  // Summary
  if (gFailures == 0) {
    std::cout << "ALL TESTS PASSED" << std::endl;
    return 0;
  } else {
    std::cout << gFailures << " test(s) FAILED" << std::endl;
    return 1;
  }
}

// Note: The actual sqlite3UnlinkAndDeleteIndex function is defined in build.c and is capable
// of interacting with our mock environment as described. The tests focus on exercising
// the two critical branches: head-removal (new index unlinking itself as head) and
// non-head removal (unlinking by bypassing the head in the linked list).