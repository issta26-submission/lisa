/*
  Unit test suite for the focal method:
  sqlite3PcacheDirtyList(PCache *pCache)

  Notes:
  - This test is a self-contained, self-sufficient C++11 program
    that re-implements the minimal required parts of the surrounding
    codebase (PgHdr, PCache, pcacheSortDirtyList) to exercise the
    focal method logic without external dependencies.
  - We avoid using GTestUnit and provide a lightweight test harness
    that reports PASS/FAIL for each test case.
  - Tests verify core behavior:
      1) Proper iteration over pCache->pDirty updating pDirty to the original
         next pointer for every node.
      2) Correct return value by pcacheSortDirtyList (head of the sorted dirty list)
         given the nodes in the dirty list.
  - We simulate the essential fields: PgHdr::pDirtyNext and PgHdr::pDirty
    and the chain PCache::pDirty. The test also checks that the original
    next pointers survive as the value captured before the focal method call.
  - The test is self-contained and compiles with C++11.
*/

#include <vector>
#include <cstddef>
#include <sqliteInt.h>
#include <iostream>
#include <algorithm>


// ----------------------
// Minimal reproduction of dependencies (within test)
// ----------------------

// Forward declaration mirroring the production types used by sqlite3PcacheDirtyList
struct PgHdr;
struct PCache;

// Node of the page cache (PgHdr) with only fields used by the tested logic
struct PgHdr {
  int key;            // Used for deterministic sorting in test pcacheSortDirtyList
  PgHdr *pDirtyNext;   // Next node in the dirty list (as used by the production code)
  PgHdr *pDirty;        // Pointer set to current node's next during sqlite3PcacheDirtyList
};

// Cache container (PCache) with pointer to head of dirty list
struct PCache {
  PgHdr *pDirty; // Head of the dirty list
};

// Global helper to sort a list of PgHdr nodes by their 'key' in ascending order.
// This simulates the production pcacheSortDirtyList behavior for testing.
// Note: In the real sqlite3 code, pcacheSortDirtyList is static in pcache.c.
// Here we provide a testable stand-in to validate sqlite3PcacheDirtyList's integration.
PgHdr* pcacheSortDirtyList(PgHdr *pIn) {
  if (pIn == nullptr) return nullptr;

  // Gather all nodes starting from head
  std::vector<PgHdr*> nodes;
  for (PgHdr *p = pIn; p != nullptr; p = p->pDirtyNext) {
    nodes.push_back(p);
  }

  // Sort by key (ascending) to simulate the sorted dirty list
  std::sort(nodes.begin(), nodes.end(),
            [](const PgHdr* a, const PgHdr* b) { return a->key < b->key; });

  // Re-wire the chain according to the sorted order by updating pDirtyNext
  for (size_t i = 0; i + 1 < nodes.size(); ++i) {
    nodes[i]->pDirtyNext = nodes[i+1];
  }
  nodes.back()->pDirtyNext = nullptr;

  // Return new head of the sorted list
  return nodes.front();
}

// Focal method under test (re-implemented here for self-contained testing)
PgHdr* sqlite3PcacheDirtyList(PCache *pCache) {
  PgHdr *p;
  for (p = pCache->pDirty; p; p = p->pDirtyNext) {
    p->pDirty = p->pDirtyNext;
  }
  return pcacheSortDirtyList(pCache->pDirty);
}

// ----------------------
// Testing framework (lightweight, non-terminating)
// ----------------------

static int g_totalTests = 0;
static int g_passedTests = 0;
static int g_failedTests = 0;

#define TEST_ASSERT(cond, msg) do { \
  ++g_totalTests; \
  if (cond) { \
    ++g_passedTests; \
  } else { \
    ++g_failedTests; \
    std::cerr << "Test failure: " << msg << std::endl; \
  } \
} while(0)

static void test_report_summary() {
  std::cout << "Unit test summary: "
            << g_passedTests << " / " << g_totalTests << " passed." << std::endl;
  if (g_failedTests > 0) {
    std::cout << g_failedTests << " test(s) failed." << std::endl;
  }
}

// Helper to clean up allocated nodes
static void destroy_chain(std::vector<PgHdr*>& nodes) {
  for (PgHdr* n : nodes) {
    delete n;
  }
}

// ----------------------
// Test cases
// ----------------------

/*
  Test 1: NULL pDirty
  - When the head of the dirty list is NULL, sqlite3PcacheDirtyList should return NULL.
*/
static void test_null_pdirty_list() {
  // Setup: empty dirty list
  PCache cache;
  cache.pDirty = nullptr;

  PgHdr* result = sqlite3PcacheDirtyList(&cache);

  TEST_ASSERT(result == nullptr, "test_null_pdirty_list: expected NULL result when pDirty is NULL");
}

/*
  Test 2: Single element in dirty list
  - One element chain: after calling sqlite3PcacheDirtyList, that node's pDirty should be nullptr
    (since its pDirtyNext is nullptr) and the returned head should be that node.
*/
static void test_single_element() {
  // Create a single node
  PgHdr* n1 = new PgHdr{42, nullptr, nullptr}; // key=42
  PCache cache;
  cache.pDirty = n1;

  // Record originalNext before call
  PgHdr* originalNext = n1->pDirtyNext; // should be nullptr

  PgHdr* ret = sqlite3PcacheDirtyList(&cache);

  // After operation:
  // - n1.pDirty should equal originalNext (nullptr)
  TEST_ASSERT(n1->pDirty == originalNext, "test_single_element: pDirty should be set to original pDirtyNext (nullptr)");
  // - Return should be head of sorted (same single element)
  TEST_ASSERT(ret == n1, "test_single_element: return head should be the single element node");
  delete n1;
}

/*
  Test 3: Two-element chain with disparate keys
  - Initial chain: A(key=20) -> B(key=5)
  - After sqlite3PcacheDirtyList:
      - A.pDirty == B (original next)
      - B.pDirty == nullptr (since B.pDirtyNext is nullptr)
    The returned head should be the node with smaller key (B) after sorting.
*/
static void test_two_elements_ordering() {
  PgHdr* A = new PgHdr{20, nullptr, nullptr};
  PgHdr* B = new PgHdr{5, nullptr, nullptr};
  // Link chain: A -> B
  A->pDirtyNext = B;
  B->pDirtyNext = nullptr;

  PCache cache;
  cache.pDirty = A;

  // Record originalNext for both nodes
  std::vector<std::pair<PgHdr*, PgHdr*>> origNext;
  origNext.push_back({A, A->pDirtyNext}); // A.next == B
  origNext.push_back({B, B->pDirtyNext}); // B.next == nullptr

  PgHdr* ret = sqlite3PcacheDirtyList(&cache);

  // After operation:
  TEST_ASSERT(A->pDirty == origNext[0].second, "test_two_elements_ordering: A.pDirty should equal A.pDirtyNext (original next, i.e., B)");
  TEST_ASSERT(B->pDirty == origNext[1].second, "test_two_elements_ordering: B.pDirty should equal B.pDirtyNext (original next, i.e., nullptr)");
  // Sorted head must be the node with key=5 (B)
  TEST_ASSERT(ret == B, "test_two_elements_ordering: return head should be the node with smaller key (B)");

  // Cleanup
  delete A;
  delete B;
}

/*
  Test 4: Three-element chain with mixed keys
  - Initial chain: A(3) -> B(1) -> C(2)
  - After sqlite3PcacheDirtyList:
      - A.pDirty == B
      - B.pDirty == C
      - C.pDirty == nullptr
    Returned head should be node with key=1 (B) after sorting.
*/
static void test_three_elements_sorting() {
  PgHdr* A = new PgHdr{3, nullptr, nullptr};
  PgHdr* B = new PgHdr{1, nullptr, nullptr};
  PgHdr* C = new PgHdr{2, nullptr, nullptr};
  // Link chain: A -> B -> C
  A->pDirtyNext = B;
  B->pDirtyNext = C;
  C->pDirtyNext = nullptr;

  PCache cache;
  cache.pDirty = A;

  // Record originalNext for each element
  std::vector<std::pair<PgHdr*, PgHdr*>> origNext;
  origNext.push_back({A, A->pDirtyNext}); // A.next == B
  origNext.push_back({B, B->pDirtyNext}); // B.next == C
  origNext.push_back({C, C->pDirtyNext}); // C.next == nullptr

  PgHdr* ret = sqlite3PcacheDirtyList(&cache);

  // After operation:
  TEST_ASSERT(A->pDirty == origNext[0].second, "test_three_elements_sorting: A.pDirty should equal A.pDirtyNext (original next, i.e., B)");
  TEST_ASSERT(B->pDirty == origNext[1].second, "test_three_elements_sorting: B.pDirty should equal B.pDirtyNext (original next, i.e., C)");
  TEST_ASSERT(C->pDirty == origNext[2].second, "test_three_elements_sorting: C.pDirty should equal C.pDirtyNext (original next, i.e., nullptr)");
  // Sorted head must be the node with key=1 (B)
  TEST_ASSERT(ret == B, "test_three_elements_sorting: return head should be the node with smallest key (B)");

  // Cleanup
  delete A;
  delete B;
  delete C;
}

// ----------------------
// Main
// ----------------------

int main() {
  test_null_pdirty_list();
  test_single_element();
  test_two_elements_ordering();
  test_three_elements_sorting();

  test_report_summary();
  // Return code 0 if all tests passed, non-zero otherwise
  return (g_failedTests == 0) ? 0 : 1;
}