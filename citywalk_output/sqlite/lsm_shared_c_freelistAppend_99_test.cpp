// C++11 test suite for freelistAppend in a simplified lsm_shared.c-like environment
// This test does not rely on Google Test. It uses a lightweight, non-terminating
// assertion mechanism to exercise the core logic of freelistAppend, including
// allocation growth, insertion, and clobbering of existing entries.

#include <lsmInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Domain-specific type aliases to mimic the original codebase
typedef unsigned int u32;
typedef long long i64;

// Result codes (simplified)
const int LSM_OK = 0;
const int LSM_NOMEM_BKPT = -1;

// Minimal FreelistEntry representation used by freelistAppend
struct FreelistEntry {
  u32 iBlk;
  i64 iId;
};

// Minimal Freelist structure
struct Freelist {
  int nAlloc;      // allocated capacity in terms of number of entries
  int nEntry;      // current number of valid entries
  FreelistEntry *aEntry; // dynamic array of entries
};

// Minimal lsm_env placeholder
struct lsm_env { };

// Minimal Worker wrapper containing a freelist
struct Worker {
  Freelist freelist;
};

// The lsm_db structure used by freelistAppend (simplified)
struct lsm_db {
  lsm_env *pEnv;
  Freelist *pFreelist;
  Worker *pWorker;
  bool bUseFreelist;
};

// Forward declaration of the realloc helper used by freelistAppend
FreelistEntry* lsmRealloc(lsm_env*, FreelistEntry*, int);

// The focal method replicated in C++ for testing
int freelistAppend(lsm_db *db, u32 iBlk, i64 iId){
  lsm_env *pEnv = db->pEnv;
  Freelist *p;
  int i; 
  assert( iId==-1 || iId>=0 );
  p = db->bUseFreelist ? db->pFreelist : &db->pWorker->freelist;
  /* Extend the space allocated for the freelist, if required */
  assert( p->nAlloc>=p->nEntry );
  if( p->nAlloc==p->nEntry ){
    int nNew; 
    int nByte; 
    FreelistEntry *aNew;
    nNew = (p->nAlloc==0 ? 4 : p->nAlloc*2);
    nByte = sizeof(FreelistEntry) * nNew;
    aNew = (FreelistEntry *)lsmRealloc(pEnv, p->aEntry, nByte);
    if( !aNew ) return LSM_NOMEM_BKPT;
    p->nAlloc = nNew;
    p->aEntry = aNew;
  }
  for(i=0; i<p->nEntry; i++){
    assert( i==0 || p->aEntry[i].iBlk > p->aEntry[i-1].iBlk );
    if( p->aEntry[i].iBlk>=iBlk ) break;
  }
  if( i<p->nEntry && p->aEntry[i].iBlk==iBlk ){
    /* Clobber an existing entry */
    p->aEntry[i].iId = iId;
  }else{
    /* Insert a new entry into the list */
    int nByte = sizeof(FreelistEntry)*(p->nEntry-i);
    memmove(&p->aEntry[i+1], &p->aEntry[i], nByte);
    p->aEntry[i].iBlk = iBlk;
    p->aEntry[i].iId = iId;
    p->nEntry++;
  }
  return LSM_OK;
}

// Simple non-terminating test assertion helpers
#define TEST_LOG(msg) do { std::cout << "[TEST] " << msg << std::endl; } while(0)
#define EXPECT_TRUE(cond, ... /* unused varargs for compatibility */) \
  do { if(!(cond)){ std::cerr << "EXPECT_TRUE failed: " #cond << "\n"; } } while(0)
#define EXPECT_EQ(a, b, ... ) \
  do { if((a) != (b)){ std::cerr << "EXPECT_EQ failed: " #a " != " #b << " (" << (a) << " vs " << (b) << ")\n"; } } while(0)
#define EXPECT_GE(a, b, ... ) \
  do { if((a) < (b)){ std::cerr << "EXPECT_GE failed: " #a " < " #b << " (" << (a) << " < " << (b) << ")\n"; } } while(0)

// Simple lsmRealloc implementation using malloc/realloc
FreelistEntry* lsmRealloc(lsm_env* /*pEnv*/, FreelistEntry* pOld, int nByte){
  if( nByte <= 0 ){
    std::free(pOld);
    return nullptr;
  }
  FreelistEntry* pNew = (FreelistEntry*)std::realloc(pOld, nByte);
  return pNew;
}

// Helper to initialize a fresh test database context
static void initialize_db_for_test(lsm_db &db, Freelist &freelist, Worker &worker){
  static lsm_env dummyEnv;
  db.pEnv = &dummyEnv;
  db.pFreelist = &freelist;
  db.pWorker = &worker;
  db.bUseFreelist = true;
  // Fresh freelist
  freelist.nAlloc = 0;
  freelist.nEntry = 0;
  freelist.aEntry = nullptr;
  // Worker freelist mirror (not used in tests since bUseFreelist = true)
  worker.freelist.nAlloc = 0;
  worker.freelist.nEntry = 0;
  worker.freelist.aEntry = nullptr;
}

// Clean up a freelist
static void cleanup_freelist(Freelist &freelist){
  if(freelist.aEntry){
    std::free(freelist.aEntry);
    freelist.aEntry = nullptr;
  }
  freelist.nAlloc = 0;
  freelist.nEntry = 0;
}

// Test 1: Basic allocation path should allocate initial block and insert first entry
static void test_basic_allocation(){
  TEST_LOG("Test 1: Basic allocation path creates initial storage and inserts first entry");
  Freelist freelist;
  Worker worker;
  lsm_db db;
  initialize_db_for_test(db, freelist, worker);

  int rc = freelistAppend(&db, 10, 1); // insert first entry
  EXPECT_EQ(rc, LSM_OK);
  EXPECT_TRUE(db.pFreelist == &freelist);

  // Validate post-conditions
  EXPECT_EQ(freelist.nEntry, 1);
  if(freelist.nEntry == 1){
    EXPECT_EQ(freelist.aEntry[0].iBlk, 10);
    EXPECT_EQ(freelist.aEntry[0].iId, 1);
  }

  // Cleanup
  cleanup_freelist(freelist);
  // worker.freelist cleanup not strictly necessary as it's a copy, but keep symmetry
  cleanup_freelist(worker.freelist);
}

// Test 2: Clobber an existing entry with the same iBlk
static void test_clobber_existing_entry(){
  TEST_LOG("Test 2: Clobber existing entry when inserting same iBlk (iBlk matches)");
// Setup fresh context
  Freelist freelist;
  Worker worker;
  lsm_db db;
  initialize_db_for_test(db, freelist, worker);

  // Insert first entry
  int rc = freelistAppend(&db, 10, 1);
  EXPECT_EQ(rc, LSM_OK);

  // Clobber same iBlk with a new iId
  rc = freelistAppend(&db, 10, 5);
  EXPECT_EQ(rc, LSM_OK);

  // Validate
  EXPECT_EQ(freelist.nEntry, 1);
  if(freelist.nEntry == 1){
    EXPECT_EQ(freelist.aEntry[0].iBlk, 10);
    EXPECT_EQ(freelist.aEntry[0].iId, 5);
  }

  // Cleanup
  cleanup_freelist(freelist);
  cleanup_freelist(worker.freelist);
}

// Test 3: Insertions in middle and end maintain sorted order by iBlk
static void test_insert_middle_and_end(){
  TEST_LOG("Test 3: Insertions maintain sorted iBlk order (middle and end)");
  Freelist freelist;
  Worker worker;
  lsm_db db;
  initialize_db_for_test(db, freelist, worker);

  // Start with two entries
  int rc = freelistAppend(&db, 10, 1); // [ (10,1) ]
  EXPECT_EQ(rc, LSM_OK);
  rc = freelistAppend(&db, 20, 2);       // [ (10,1), (20,2) ]
  EXPECT_EQ(rc, LSM_OK);

  // Insert in the middle
  rc = freelistAppend(&db, 15, 3);       // expect [ (10,1), (15,3), (20,2) ]
  EXPECT_EQ(rc, LSM_OK);

  // Insert at end
  rc = freelistAppend(&db, 25, 4);       // expect [10,15,20,25]
  EXPECT_EQ(rc, LSM_OK);
  EXPECT_EQ(freelist.nEntry, 4);

  if(freelist.nEntry == 4){
    EXPECT_EQ(freelist.aEntry[0].iBlk, 10);
    EXPECT_EQ(freelist.aEntry[1].iBlk, 15);
    EXPECT_EQ(freelist.aEntry[2].iBlk, 20);
    EXPECT_EQ(freelist.aEntry[3].iBlk, 25);
  }

  // Cleanup
  cleanup_freelist(freelist);
  cleanup_freelist(worker.freelist);
}

// Test 4: Growth path when inserting beyond current allocation
static void test_growth_path(){
  TEST_LOG("Test 4: Growth path triggers allocation growth on 5th insertion");
  Freelist freelist;
  Worker worker;
  lsm_db db;
  initialize_db_for_test(db, freelist, worker);

  // Create 4 entries to fill initial allocation (nAlloc becomes 4)
  int rc = freelistAppend(&db, 10, 1);
  EXPECT_EQ(rc, LSM_OK);
  rc = freelistAppend(&db, 20, 2);
  EXPECT_EQ(rc, LSM_OK);
  rc = freelistAppend(&db, 30, 3);
  EXPECT_EQ(rc, LSM_OK);
  rc = freelistAppend(&db, 40, 4);
  EXPECT_EQ(rc, LSM_OK);
  EXPECT_EQ(freelist.nAlloc, 4);
  EXPECT_EQ(freelist.nEntry, 4);

  // This insertion should trigger growth (nAlloc == nEntry before op)
  rc = freelistAppend(&db, 5, 5); // insert at position 0 due to smallest iBlk
  EXPECT_EQ(rc, LSM_OK);
  // After growth, nAlloc should be 8
  EXPECT_EQ(freelist.nAlloc, 8);
  EXPECT_EQ(freelist.nEntry, 5); // one new entry added

  // Validate that ordering still holds after growth
  if(freelist.nEntry >= 5){
    EXPECT_EQ(freelist.aEntry[0].iBlk, 5);  // smallest at front
    EXPECT_EQ(freelist.aEntry[4].iBlk, 40); // last element remains
  }

  // Cleanup
  cleanup_freelist(freelist);
  cleanup_freelist(worker.freelist);
}

static bool run_all_tests(){
  TEST_LOG("Starting freelistAppend unit test suite...");
  test_basic_allocation();
  test_clobber_existing_entry();
  test_insert_middle_and_end();
  test_growth_path();
  TEST_LOG("All tests executed.");
  return true;
}

// Main entry: run the tests
int main(){
  run_all_tests();
  return 0;
}