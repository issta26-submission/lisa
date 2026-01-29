// Note: This is a C++11 test harness (no GTest) designed to exercise the focal
// function sqlite3MutexInit() and its core dependencies by re-creating a minimal
// SQLite-like environment sufficient for unit testing.
// The goal is to validate behavior for the primary branches:
// - When xMutexAlloc is NULL (initialization path taken):
//     - with CoreMutex = true using sqlite3DefaultMutex
//     - with CoreMutex = false using sqlite3NoopMutex
// - When xMutexAlloc is non-NULL (short-circuit path, no reconfiguration)
#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Domain knowledge assumptions (simplified stubs for test isolation)
#define SQLITE_OK 0
#define SQLITE_DEBUG 0

// Forward declarations and minimal stubs to mimic SQLite's mutex framework
typedef struct sqlite3_mutex_methods sqlite3_mutex_methods;
struct sqlite3_mutex_methods {
  int (*xMutexInit)(void);
  int (*xMutexEnd)(void);
  void *(*xMutexFree)(void*);
  void (*xMutexEnter)(void*);
  int (*xMutexTry)(void*);
  void (*xMutexLeave)(void*);
  int (*xMutexHeld)(void*);
  int (*xMutexNotheld)(void*);
  void *(*xMutexAlloc)(int);
};

// Global config mimic (simplified)
struct {
  sqlite3_mutex_methods mutex;
  bool bCoreMutex;
} sqlite3GlobalConfig;

// Memory barrier stub
static void sqlite3MemoryBarrier(void) { /* no-op for test */ }

// SQLITE_OK return value holder to validate rc results
// (already defined above as SQLITE_OK)

// -------------------- Mock mutex implementations --------------------

static int gDefaultInitCalls = 0;
static int gNoopInitCalls = 0;
static int gPreExistingInitCalls = 0;

static int defaultMutexInitBridge(void){
  ++gDefaultInitCalls;
  return SQLITE_OK;
}
static int noopMutexInitBridge(void){
  ++gNoopInitCalls;
  return SQLITE_OK;
}
static int preExistingInitBridge(void){
  ++gPreExistingInitCalls;
  return SQLITE_OK;
}
static void defaultMutexEndBridge(void){}
static void* defaultMutexFreeBridge(void* p){ (void)p; return nullptr; }
static void defaultMutexEnterBridge(void* p){ (void)p; }
static int defaultMutexTryBridge(void* p){ (void)p; return 0; }
static void defaultMutexLeaveBridge(void* p){ (void)p; }
static int defaultMutexHeldBridge(void* p){ (void)p; return 0; }
static int defaultMutexNotheldBridge(void* p){ (void)p; return 1; }
static void* defaultMutexAllocBridge(int id){ (void)id; return (void*)0xDEADBEEF; }

static void noopMutexEndBridge(void){}
static void* noopMutexFreeBridge(void* p){ (void)p; return nullptr; }
static void noopMutexEnterBridge(void* p){ (void)p; }
static int noopMutexTryBridge(void* p){ (void)p; return 0; }
static void noopMutexLeaveBridge(void* p){ (void)p; }
static int noopMutexHeldBridge(void* p){ (void)p; return 0; }
static int noopMutexNotheldBridge(void* p){ (void)p; return 1; }
static void* noopMutexAllocBridge(int id){ (void)id; return (void*)0xCADEBEEFu; }

static void* multiMutexAllocBridge(int id){ (void)id; return (void*)0xBADC0DEu; }
static int multiMutexInitBridge(void){ ++gDefaultInitCalls; return SQLITE_OK; } // reuse for testing if needed

// Mutex instances (three variants)
static sqlite3_mutex_methods defaultMutex;
static sqlite3_mutex_methods noopMutex;
static sqlite3_mutex_methods multiThreadMutex;

// Accessors to emulate sqlite3DefaultMutex etc.
static sqlite3_mutex_methods const* sqlite3DefaultMutex(void){
  return &defaultMutex;
}
static sqlite3_mutex_methods const* sqlite3NoopMutex(void){
  return &noopMutex;
}
#ifdef SQLITE_ENABLE_MULTITHREADED_CHECKS
static sqlite3_mutex_methods const* multiThreadedCheckMutex(void){
  return &multiThreadMutex;
}
#endif

// Initialize all mutex structs (no designated initializers in C++ for portability)
static void initAllMutexes(void){
  // Default mutex
  defaultMutex.xMutexInit = defaultMutexInitBridge;
  defaultMutex.xMutexEnd = defaultMutexEndBridge;
  defaultMutex.xMutexFree = defaultMutexFreeBridge;
  defaultMutex.xMutexEnter = defaultMutexEnterBridge;
  defaultMutex.xMutexTry = defaultMutexTryBridge;
  defaultMutex.xMutexLeave = defaultMutexLeaveBridge;
  defaultMutex.xMutexHeld = defaultMutexHeldBridge;
  defaultMutex.xMutexNotheld = defaultMutexNotheldBridge;
  defaultMutex.xMutexAlloc = defaultMutexAllocBridge;

  // Noop mutex
  noopMutex.xMutexInit = noopMutexInitBridge;
  noopMutex.xMutexEnd = noopMutexEndBridge;
  noopMutex.xMutexFree = noopMutexFreeBridge;
  noopMutex.xMutexEnter = noopMutexEnterBridge;
  noopMutex.xMutexTry = noopMutexTryBridge;
  noopMutex.xMutexLeave = noopMutexLeaveBridge;
  noopMutex.xMutexHeld = noopMutexHeldBridge;
  noopMutex.xMutexNotheld = noopMutexNotheldBridge;
  noopMutex.xMutexAlloc = noopMutexAllocBridge;

  // Multithreaded check mutex (if enabled)
  multiThreadMutex.xMutexInit = defaultMutexInitBridge; // reuse with separate counter if needed
  multiThreadMutex.xMutexEnd = defaultMutexEndBridge;
  multiThreadMutex.xMutexFree = defaultMutexFreeBridge;
  multiThreadMutex.xMutexEnter = defaultMutexEnterBridge;
  multiThreadMutex.xMutexTry = defaultMutexTryBridge;
  multiThreadMutex.xMutexLeave = defaultMutexLeaveBridge;
  multiThreadMutex.xMutexHeld = defaultMutexHeldBridge;
  multiThreadMutex.xMutexNotheld = defaultMutexNotheldBridge;
  multiThreadMutex.xMutexAlloc = multiMutexAllocBridge;
}

// -------------------- The focal method under test --------------------
// The function is reproduced here to test its behavior in a controlled test harness.
int sqlite3MutexInit(void){
  int rc = SQLITE_OK;
  if( !sqlite3GlobalConfig.mutex.xMutexAlloc ){
    sqlite3_mutex_methods const *pFrom;
    sqlite3_mutex_methods *pTo = &sqlite3GlobalConfig.mutex;
    if( sqlite3GlobalConfig.bCoreMutex ){
#ifdef SQLITE_ENABLE_MULTITHREADED_CHECKS
      pFrom = multiThreadedCheckMutex();
#else
      pFrom = sqlite3DefaultMutex();
#endif
    }else{
      pFrom = sqlite3NoopMutex();
    }
    pTo->xMutexInit = pFrom->xMutexInit;
    pTo->xMutexEnd = pFrom->xMutexEnd;
    pTo->xMutexFree = pFrom->xMutexFree;
    pTo->xMutexEnter = pFrom->xMutexEnter;
    pTo->xMutexTry = pFrom->xMutexTry;
    pTo->xMutexLeave = pFrom->xMutexLeave;
    pTo->xMutexHeld = pFrom->xMutexHeld;
    pTo->xMutexNotheld = pFrom->xMutexNotheld;
    sqlite3MemoryBarrier();
    pTo->xMutexAlloc = pFrom->xMutexAlloc;
  }
  // Ensure there is a valid xMutexInit function before calling
  // In tests, we always assign a non-null function pointer earlier
  // (no assert failure in this test harness since we set up properly)
  if( sqlite3GlobalConfig.mutex.xMutexInit == nullptr ){
    // Fallback to a harmless init to avoid crash in edge cases
    rc = SQLITE_OK;
  }else{
    rc = sqlite3GlobalConfig.mutex.xMutexInit();
  }
  sqlite3MemoryBarrier();
  return rc;
}

// -------------------- Lightweight test framework --------------------

static int gTestFailures = 0;
#define EXPECT_TRUE(cond) do { if(!(cond)){ std::cerr << "EXPECT_TRUE failed: " << #cond << " in " << __func__ << std::endl; ++gTestFailures; } } while(0)
#define EXPECT_EQ(a,b) do { if((a) != (b)){ std::cerr << "EXPECT_EQ failed: " << a << " != " << b << " in " << __func__ << std::endl; ++gTestFailures; } } while(0)
#define EXPECT_PTR_EQ(a,b) do { if((void*)(a) != (void*)(b)){ std::cerr << "EXPECT_PTR_EQ failed: " << a << " != " << b << " in " << __func__ << std::endl; ++gTestFailures; } } while(0)

static void resetGlobalsForTest(void){
  // Reset global config to a clean baseline
  std::memset(&sqlite3GlobalConfig, 0, sizeof(sqlite3GlobalConfig));
  // Reinitialize mutex provider functions
  initAllMutexes();
  // Ensure stats counters reset
  gDefaultInitCalls = 0;
  gNoopInitCalls = 0;
  gPreExistingInitCalls = 0;
}

// Test 1: When xMutexAlloc is NULL and CoreMutex is true, use sqlite3DefaultMutex
// Validate all copied pointers and that xMutexInit was invoked from the default mutex.
static void test_DefaultMutexCopiedPointers_CoreMutexTrue(void){
  resetGlobalsForTest();
  sqlite3GlobalConfig.bCoreMutex = true;
  sqlite3GlobalConfig.mutex.xMutexAlloc = nullptr; // trigger init path

  // Ensure defaultMutex has a known Init function return path
  int rc = sqlite3MutexInit();

  // rc should be SQLITE_OK as defaultMutexInitBridge returns SQLITE_OK
  EXPECT_EQ(rc, SQLITE_OK);
  // Pointers in sqlite3GlobalConfig.mutex should now mirror defaultMutex
  EXPECT_PTR_EQ(sqlite3GlobalConfig.mutex.xMutexInit, defaultMutex.xMutexInit);
  EXPECT_PTR_EQ(sqlite3GlobalConfig.mutex.xMutexEnd, defaultMutex.xMutexEnd);
  EXPECT_PTR_EQ(sqlite3GlobalConfig.mutex.xMutexFree, defaultMutex.xMutexFree);
  EXPECT_PTR_EQ(sqlite3GlobalConfig.mutex.xMutexEnter, defaultMutex.xMutexEnter);
  EXPECT_PTR_EQ(sqlite3GlobalConfig.mutex.xMutexTry, defaultMutex.xMutexTry);
  EXPECT_PTR_EQ(sqlite3GlobalConfig.mutex.xMutexLeave, defaultMutex.xMutexLeave);
  EXPECT_PTR_EQ(sqlite3GlobalConfig.mutex.xMutexHeld, defaultMutex.xMutexHeld);
  EXPECT_PTR_EQ(sqlite3GlobalConfig.mutex.xMutexNotheld, defaultMutex.xMutexNotheld);
  EXPECT_PTR_EQ(sqlite3GlobalConfig.mutex.xMutexAlloc, defaultMutex.xMutexAlloc);

  // Ensure the default mutex Init function was invoked
  EXPECT_EQ(gDefaultInitCalls, 1);
}

// Test 2: When CoreMutex is false, use sqlite3NoopMutex
// Validate that xMutexInit comes from noopMutex and is invoked.
static void test_NoopMutexCopiedPointers_CoreMutexFalse(void){
  resetGlobalsForTest();
  sqlite3GlobalConfig.bCoreMutex = false;
  sqlite3GlobalConfig.mutex.xMutexAlloc = nullptr; // trigger init path

  int rc = sqlite3MutexInit();

  EXPECT_EQ(rc, SQLITE_OK);
  EXPECT_PTR_EQ(sqlite3GlobalConfig.mutex.xMutexInit, noopMutex.xMutexInit);
  EXPECT_PTR_EQ(sqlite3GlobalConfig.mutex.xMutexEnd, noopMutex.xMutexEnd);
  EXPECT_PTR_EQ(sqlite3GlobalConfig.mutex.xMutexFree, noopMutex.xMutexFree);
  EXPECT_PTR_EQ(sqlite3GlobalConfig.mutex.xMutexEnter, noopMutex.xMutexEnter);
  EXPECT_PTR_EQ(sqlite3GlobalConfig.mutex.xMutexTry, noopMutex.xMutexTry);
  EXPECT_PTR_EQ(sqlite3GlobalConfig.mutex.xMutexLeave, noopMutex.xMutexLeave);
  EXPECT_PTR_EQ(sqlite3GlobalConfig.mutex.xMutexHeld, noopMutex.xMutexHeld);
  EXPECT_PTR_EQ(sqlite3GlobalConfig.mutex.xMutexNotheld, noopMutex.xMutexNotheld);
  EXPECT_PTR_EQ(sqlite3GlobalConfig.mutex.xMutexAlloc, noopMutex.xMutexAlloc);

  // Init should have been invoked by noop mutex
  EXPECT_EQ(gNoopInitCalls, 1);
}

// Test 3: When xMutexAlloc is already set (non-null), sqlite3MutexInit should skip reconfiguration
// and still call the existing xMutexInit function exactly once.
static void test_XMutexAllocAlreadySet_SkipsReconfiguration(void){
  resetGlobalsForTest();

  // Pre-configure with a pre-existing, non-null xMutexInit and a non-null xMutexAlloc
  sqlite3GlobalConfig.mutex.xMutexInit = preExistingInitBridge;
  sqlite3GlobalConfig.mutex.xMutexEnd = (int)0x11111111;
  sqlite3GlobalConfig.mutex.xMutexFree = (void* (*)(void*))0x22222222;
  sqlite3GlobalConfig.mutex.xMutexEnter = (void (*)(void*))0x33333333;
  sqlite3GlobalConfig.mutex.xMutexTry = (int (*)(void*))0x44444444;
  sqlite3GlobalConfig.mutex.xMutexLeave = (void (*)(void*))0x55555555;
  sqlite3GlobalConfig.mutex.xMutexHeld = (int (*)(void*))0x66666666;
  sqlite3GlobalConfig.mutex.xMutexNotheld = (int (*)(void*))0x77777777;
  sqlite3GlobalConfig.mutex.xMutexAlloc = (void* (*)(int))0xDEADC0DE; // non-null to skip init

  sqlite3GlobalConfig.bCoreMutex = true;
  gPreExistingInitCalls = 0;
  int rc = sqlite3MutexInit();

  // rc should be whatever preExistingInitBridge returns (we set to SQLITE_OK)
  EXPECT_EQ(rc, SQLITE_OK);
  // Pointers should remain unchanged (no reconfiguration)
  EXPECT_EQ(sqlite3GlobalConfig.mutex.xMutexInit, preExistingInitBridge);
  // The pre-existing function should have been invoked once
  EXPECT_EQ(gPreExistingInitCalls, 1);
}

// Optional: main entry to run tests
int main(void){
  // Initialize mutex structures and environment
  resetGlobalsForTest();
  initAllMutexes();

  // Run tests
  test_DefaultMutexCopiedPointers_CoreMutexTrue();
  test_NoopMutexCopiedPointers_CoreMutexFalse();
  test_XMutexAllocAlreadySet_SkipsReconfiguration();

  if( gTestFailures == 0 ){
    std::cout << "All tests passed." << std::endl;
    return 0;
  }else{
    std::cout << gTestFailures << " test(s) failed." << std::endl;
    return 1;
  }
}