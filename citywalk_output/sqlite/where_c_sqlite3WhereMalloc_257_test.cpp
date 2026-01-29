// This test suite targets the focal function: sqlite3WhereMalloc
// and its necessary dependencies, implemented in this standalone file
// to allow compilation under C++11 without GTest.
// Step 1 (Program Understanding) and Step 2/3 (Test Generation & Refinement)
// have been embedded as explanatory comments for traceability.

#include <whereInt.h>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Domain knowledge & dependencies (minimal stubs) to enable compilation

using u64 = uint64_t;

// Forward declarations of types used by sqlite3WhereMalloc
struct WhereMemBlock;
struct Parse;
struct WhereInfo;

// Lightweight memory allocator stub to simulate SQLite's allocator behavior.
// We add a global flag to deterministically fail allocations for test coverage.
static bool g_forceFailAlloc = false;

void* sqlite3DbMallocRawNN(void* db, size_t n){
    (void)db; // unused dummy
    if (g_forceFailAlloc) return NULL;
    // Allocate extra space for the header (WhereMemBlock) plus payload
    return std::malloc(n);
}

// The header block placed before each user payload allocated by sqlite3WhereMalloc
struct WhereMemBlock {
    WhereMemBlock *pNext;
    uint64_t sz;
};

// Minimal Parse structure containing a dummy database pointer
struct Parse {
    void* db;
};

// The WhereInfo structure used by sqlite3WhereMalloc
struct WhereInfo {
    Parse* pParse;
    WhereMemBlock* pMemToFree;
};

// Focal function under test (re-implemented here for standalone testing)
void *sqlite3WhereMalloc(WhereInfo *pWInfo, u64 nByte){
  WhereMemBlock *pBlock;
  pBlock = (WhereMemBlock*)sqlite3DbMallocRawNN(pWInfo->pParse->db, nByte+sizeof(*pBlock));
  if( pBlock ){
    pBlock->pNext = pWInfo->pMemToFree;
    pBlock->sz = nByte;
    pWInfo->pMemToFree = pBlock;
    pBlock++;
  }
  return (void*)pBlock;
}

// Simple test harness (no GTest). Uses non-terminating assertions pattern.
// Explanatory comments are provided for each test case.

// Global test failure counter
static int g_test_failures = 0;

// Helper: compute header pointer from returned data pointer
static WhereMemBlock* headerFromPayload(void* payload){
    return (WhereMemBlock*)((char*)payload - sizeof(WhereMemBlock));
}

// Helper: release allocated blocks to avoid leaks
static void freeAll(WhereInfo* pWInfo){
    WhereMemBlock* cur = pWInfo->pMemToFree;
    while (cur){
        WhereMemBlock* nxt = cur->pNext;
        // Free the entire allocated chunk starting at header
        std::free((void*)cur);
        cur = nxt;
    }
    pWInfo->pMemToFree = NULL;
}

// Convenience assertion macros (non-terminating): log but continue
#define EXPECT_TRUE(x) \
    do { if(!(x)) { std::cout << " EXPECT_TRUE FAILED: " #x " at " << __FILE__ << ":" << __LINE__ << "\n"; ++g_test_failures; } } while(0)

#define EXPECT_EQ(a,b) \
    do { if((a) != (b)) { \
        std::cout << " EXPECT_EQ FAILED: " #a " == " #b " (" << (a) << " != " << (b) \
        << ") at " << __FILE__ << ":" << __LINE__ << "\n"; ++g_test_failures; } } while(0)

// Test 1: Single allocation path should create a header, link it, and return payload pointer
static void test_sqlite3WhereMalloc_singleAllocation(){
    std::cout << "Running test_sqlite3WhereMalloc_singleAllocation...\n";

    // Setup dummy objects
    Parse p;
    p.db = (void*)0xDEADBEEF; // non-null dummy pointer

    WhereInfo w;
    w.pParse = &p;
    w.pMemToFree = NULL;

    // Ensure deterministic success
    g_forceFailAlloc = false;

    // Call with nByte = 32
    const uint64_t nByte = 32;
    void* payload = sqlite3WhereMalloc(&w, nByte);

    // Validate payload is non-null
    EXPECT_TRUE(payload != NULL);

    // Validate header is correct and linked to pMemToFree
    WhereMemBlock* header = headerFromPayload(payload);
    EXPECT_TRUE(header != NULL);
    EXPECT_EQ((uint64_t)header, (uint64_t)header); // sanity (no-op, ensures header type exists)

    // header fields
    EXPECT_EQ(header->sz, nByte);
    EXPECT_TRUE(header->pNext == NULL); // initial block, no prior memory

    // pMemToFree should now point to this header
    EXPECT_TRUE(w.pMemToFree == header);

    // Return payload must be exactly after header
    void* expectedPayloadLocation = (void*)(header + 1);
    EXPECT_TRUE(payload == expectedPayloadLocation);

    // Cleanup
    freeAll(&w);
    std::cout << "test_sqlite3WhereMalloc_singleAllocation completed.\n";
}

// Test 2: Second allocation should chain blocks
static void test_sqlite3WhereMalloc_secondAllocation_chain(){
    std::cout << "Running test_sqlite3WhereMalloc_secondAllocation_chain...\n";

    Parse p;
    p.db = (void*)0xCAFEBABE;

    WhereInfo w;
    w.pParse = &p;
    w.pMemToFree = NULL;

    g_forceFailAlloc = false;

    // First allocation
    void* a1 = sqlite3WhereMalloc(&w, 16);
    EXPECT_TRUE(a1 != NULL);
    WhereMemBlock* header1 = headerFromPayload(a1);
    EXPECT_TRUE(header1 != NULL);
    EXPECT_EQ(header1->sz, 16);
    EXPECT_TRUE(header1->pNext == NULL);

    // Second allocation
    void* a2 = sqlite3WhereMalloc(&w, 8);
    EXPECT_TRUE(a2 != NULL);
    WhereMemBlock* header2 = headerFromPayload(a2);
    EXPECT_TRUE(header2 != NULL);
    EXPECT_EQ(header2->sz, 8);

    // Validate chaining: header2->pNext should point to header1
    EXPECT_TRUE(header2->pNext == header1);

    // pMemToFree should now point to header2
    EXPECT_TRUE(w.pMemToFree == header2);

    // Optional: ensure payload location is after its header
    void* expectedPayload2 = (void*)(header2 + 1);
    EXPECT_TRUE(a2 == expectedPayload2);

    // Cleanup
    freeAll(&w);
    std::cout << "test_sqlite3WhereMalloc_secondAllocation_chain completed.\n";
}

// Test 3: Allocation failure should return NULL and not modify pMemToFree
static void test_sqlite3WhereMalloc_allocationFailure(){
    std::cout << "Running test_sqlite3WhereMalloc_allocationFailure...\n";

    Parse p;
    p.db = (void*)0xBADC0DE;

    WhereInfo w;
    w.pParse = &p;
    w.pMemToFree = NULL;

    // Force allocation failure
    g_forceFailAlloc = true;

    void* payload = sqlite3WhereMalloc(&w, 24);
    EXPECT_TRUE(payload == NULL);

    // pMemToFree should remain NULL (no allocation happened)
    EXPECT_TRUE(w.pMemToFree == NULL);

    // Cleanup (no blocks expected)
    freeAll(&w);
    std::cout << "test_sqlite3WhereMalloc_allocationFailure completed.\n";
}

// Test 4: Ensure multiple mixed scenarios still behave correctly after a failure in between
static void test_sqlite3WhereMalloc_mixedRecovery(){
    std::cout << "Running test_sqlite3WhereMalloc_mixedRecovery...\n";

    Parse p;
    p.db = (void*)0xFEEDFACE;

    WhereInfo w;
    w.pParse = &p;
    w.pMemToFree = NULL;

    // Successful first allocation
    g_forceFailAlloc = false;
    void* p1 = sqlite3WhereMalloc(&w, 12);
    EXPECT_TRUE(p1 != NULL);

    // Force failure for second
    g_forceFailAlloc = true;
    void* pFail = sqlite3WhereMalloc(&w, 6);
    EXPECT_TRUE(pFail == NULL);

    // Third allocation should succeed
    g_forceFailAlloc = false;
    void* p3 = sqlite3WhereMalloc(&w, 4);
    EXPECT_TRUE(p3 != NULL);

    // Check chain: header for p3 should point to header of p1
    WhereMemBlock* header3 = headerFromPayload(p3);
    WhereMemBlock* header1 = headerFromPayload(p1);
    EXPECT_TRUE(header3->pNext == header1);

    // Cleanup
    freeAll(&w);
    std::cout << "test_sqlite3WhereMalloc_mixedRecovery completed.\n";
}

int main(){
    std::cout << "Starting sqlite3WhereMalloc unit test suite (standalone, C++11).\n";

    test_sqlite3WhereMalloc_singleAllocation();
    test_sqlite3WhereMalloc_secondAllocation_chain();
    test_sqlite3WhereMalloc_allocationFailure();
    test_sqlite3WhereMalloc_mixedRecovery();

    if(g_test_failures == 0){
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    }else{
        std::cout << g_test_failures << " TEST(S) FAILED\n";
        return 1;
    }
}