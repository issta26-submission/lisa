// Lightweight C++11 test suite for sqlite3_cancel_auto_extension
// This suite is designed to be compiled without GoogleTest and uses
// simple non-terminating EXPECT_ macros to accumulate failures.
// It re-implements minimal SQLite-like environment required by the focal method.

#include <vector>
#include <sqlite3ext.h>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <mutex>


// BEGIN: Minimal environment to simulate SQLite-like auto-extension state

// Macro stubs to match preprocessing in focal method
#define SQLITE_THREADSAFE 1
#define SQLITE_MUTEX_STATIC_MAIN 0
#define SQLITE_ENABLE_API_ARMOR 1
#define wsdAutoextInit ((void)0)

// Typedefs and structures to mimic required parts
typedef void (*ExtInit)(void);

struct sqlite3_mutex {
  std::mutex m;
};

static sqlite3_mutex* g_mutex = nullptr;
static sqlite3_mutex* sqlite3MutexAlloc(int){
  if(!g_mutex) g_mutex = new sqlite3_mutex{};
  return g_mutex;
}
static void sqlite3_mutex_enter(sqlite3_mutex* m){
  if(m) m->m.lock();
}
static void sqlite3_mutex_leave(sqlite3_mutex* m){
  if(m) m->m.unlock();
}

// Global structure mimicking the critical wsdAutoext data
struct WsdAutoExt {
  int nExt;
  ExtInit* aExt;
};

static WsdAutoExt wsdAutoext = {0, nullptr};

// Lightweight allocator helper for test
static ExtInit extA, extB, extC, extD, extE;

// Implement the focal method under test in a standalone C-like form
extern "C" int sqlite3_cancel_auto_extension(void (*xInit)(void)) {
#if SQLITE_THREADSAFE
  sqlite3_mutex *mutex = sqlite3MutexAlloc(SQLITE_MUTEX_STATIC_MAIN);
#endif
  int i;
  int n = 0;
  wsdAutoextInit;
#ifdef SQLITE_ENABLE_API_ARMOR
  if( xInit==0 ) return 0;
#endif
#if SQLITE_THREADSAFE
  sqlite3_mutex_enter(mutex);
#else
  // If threading absent, no-op mutex
  (void)nullptr;
#endif
  for(i=(int)wsdAutoext.nExt-1; i>=0; i--){
    if( wsdAutoext.aExt[i]==xInit ){
      wsdAutoext.nExt--;
      wsdAutoext.aExt[i] = wsdAutoext.aExt[wsdAutoext.nExt];
      n++;
      break;
    }
  }
#if SQLITE_THREADSAFE
  sqlite3_mutex_leave(mutex);
#endif
  return n;
}

// The following stubs mimic other required API surface in class dep (unused by tests)
extern "C" void sqlite3_reset_auto_extension(void) {}
extern "C" void sqlite3AutoLoadExtensions(void*) {}

// END: Minimal environment

// Global test state
static int g_failures = 0;

// Simple non-terminating assertions
#define EXPECT_TRUE(cond) \
  do { if(!(cond)) { std::cerr << "EXPECT_TRUE failed: " #cond " at " __FILE__ ":" << __LINE__ << "\n"; ++g_failures; } } while(0)

#define EXPECT_EQ(a,b) \
  do { if(!((a) == (b))) { std::cerr << "EXPECT_EQ failed: " #a " (" << (a) << ") != " #b " (" << (b) << ") at " __FILE__ ":" << __LINE__ << "\n"; ++g_failures; } } while(0)

#define EXPECT_PTR_EQ(a,b) \
  do { if(!((void*)(a) == (void*)(b))) { std::cerr << "EXPECT_PTR_EQ failed: " #a " (" << a << ") != " #b " (" << b << ") at " __FILE__ ":" << __LINE__ << "\n"; ++g_failures; } } while(0)


// Test helper to reset global state
static void reset_state(ExtInit* arr, int count) {
  wsdAutoext.nExt = count;
  wsdAutoext.aExt = arr;
}

// Test 1: Removing a middle element updates array and returns 1
// Purpose: Ensure that when xInit is found at a non-edge position, the element is removed,
//          the last element is moved into its slot, and the count is decremented.
static void test_remove_middle() {
  std::cout << "Test 1: Remove middle element\n";

  // Prepare fixtures
  ExtInit pool[3];
  pool[0] = extA;
  pool[1] = extB; // to be removed
  pool[2] = extC;

  reset_state(pool, 3);

  // Execute
  int removed = sqlite3_cancel_auto_extension(extB);

  // Validate
  EXPECT_EQ(removed, 1);
  EXPECT_EQ(wsdAutoext.nExt, 2);
  EXPECT_PTR_EQ(wsdAutoext.aExt[0], extA);
  // After removal, aExt[1] should be previous aExt[2] (extC)
  EXPECT_PTR_EQ(wsdAutoext.aExt[1], extC);
  std::cout << "Test 1 completed\n";
}

// Test 2: Removing the last element should shift nothing meaningful and decrement count
// Purpose: Ensure that removing the last element correctly updates nExt and that the array
//          reflects the last element moved into its own slot (no crash or corruption).
static void test_remove_last() {
  std::cout << "Test 2: Remove last element\n";

  ExtInit pool[2];
  pool[0] = extA;
  pool[1] = extB; // last element

  reset_state(pool, 2);

  int removed = sqlite3_cancel_auto_extension(extB);

  EXPECT_EQ(removed, 1);
  EXPECT_EQ(wsdAutoext.nExt, 1);
  EXPECT_PTR_EQ(wsdAutoext.aExt[0], extA);
  // aExt[0] is extA; aExt[1] is now irrelevant since nExt == 1
  std::cout << "Test 2 completed\n";
}

// Test 3: Not found in array should return 0 and leave state unchanged
static void test_not_found() {
  std::cout << "Test 3: Element not found\n";

  ExtInit pool[2];
  pool[0] = extA;
  pool[1] = extB;

  reset_state(pool, 2);

  int removed = sqlite3_cancel_auto_extension(extC); // not in pool

  EXPECT_EQ(removed, 0);
  EXPECT_EQ(wsdAutoext.nExt, 2);
  EXPECT_PTR_EQ(wsdAutoext.aExt[0], extA);
  EXPECT_PTR_EQ(wsdAutoext.aExt[1], extB);

  std::cout << "Test 3 completed\n";
}

// Test 4: API armor path - when xInit is null and API_ARMOR enabled
// Purpose: Ensure that with API armor enabled, passing a null pointer results in immediate 0 return
static void test_api_armor_null() {
  std::cout << "Test 4: API armor with null pointer\n";

  ExtInit pool[2];
  pool[0] = extA;
  pool[1] = extB;
  reset_state(pool, 2);

  int removed = sqlite3_cancel_auto_extension(nullptr);

  EXPECT_EQ(removed, 0);
  // State should remain unchanged
  EXPECT_EQ(wsdAutoext.nExt, 2);
  EXPECT_PTR_EQ(wsdAutoext.aExt[0], extA);
  EXPECT_PTR_EQ(wsdAutoext.aExt[1], extB);

  std::cout << "Test 4 completed\n";
}

// Simple function implementations to populate function pointers
void extA() {}
void extB() {}
void extC() {}
void extD() {}
void extE() {}

int main() {
  // Run tests
  test_remove_middle();
  test_remove_last();
  test_not_found();
  test_api_armor_null();

  // Summary
  if(g_failures == 0) {
    std::cout << "All tests passed.\n";
  } else {
    std::cout << "Tests finished with " << g_failures << " failure(s).\n";
  }

  return g_failures ? 1 : 0;
}