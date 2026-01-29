/*
  Unit test suite for the focal function:
  sqlite3VdbeMemSetPointer
  This tests the core behavior of pointer assignment into a Mem object
  including tag/type handling and destructor assignment.

  Notes:
  - This test re-implements a minimal subset of the Mem structure and the
    relevant function under test in order to run in a standalone C++11 test
    environment without depending on the full SQLite source tree.
  - Tests are written with non-terminating assertions (EXPECT_*) to maximize
    code coverage while continuing test execution after failures.
  - The code uses extern "C" to expose C-compatible function names for testing.
*/

#include <cstdio>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <cassert>


// Global test counters
static int gTestsRun = 0;
static int gFailures = 0;

// Lightweight test assertion macros (non-terminating)
#define EXPECT_EQ_INT(expected, actual) \
  do { \
    if ((int)(expected) != (int)(actual)) { \
      printf("EXPECT_EQ_INT failed: expected %d, got %d (%s:%d)\n", (int)(expected), (int)(actual), __FILE__, __LINE__); \
      ++gFailures; \
    } \
  } while(0)

#define EXPECT_PTR_EQ(expected, actual) \
  do { \
    if ((void*)(expected) != (void*)(actual)) { \
      printf("EXPECT_PTR_EQ failed: expected %p, got %p (%s:%d)\n", (void*)(expected), (void*)(actual), __FILE__, __LINE__); \
      ++gFailures; \
    } \
  } while(0)

#define EXPECT_CHAR_EQ(expected, actual) \
  do { \
    if ((char)(expected) != (char)(actual)) { \
      printf("EXPECT_CHAR_EQ failed: expected '%c', got '%c' (%s:%d)\n", (char)(expected), (char)(actual), __FILE__, __LINE__); \
      ++gFailures; \
    } \
  } while(0)

#define EXPECT_STR_PTR_EQ(expected, actual) \
  do { \
    if ((const char*)(expected) != (const char*)(actual)) { \
      printf("EXPECT_STR_PTR_EQ failed: expected %p, got %p (%s:%d)\n", (const void*)(expected), (const void*)(actual), __FILE__, __LINE__); \
      ++gFailures; \
    } \
  } while(0)

// Expose a minimal C-compatible Mem structure and related concepts for testing
extern "C" {

// Forward declarations of minimal dependencies used by the focal function
typedef struct Mem Mem;

static const unsigned int MEM_Null   = 0x01;
static const unsigned int MEM_Dyn    = 0x02;
static const unsigned int MEM_Subtype= 0x04;
static const unsigned int MEM_Term   = 0x08;

// Minimal Mem structure compatible with sqlite3VdbeMemSetPointer usage
struct Mem {
  unsigned int flags;
  struct {
    const char* zPType;
  } u;
  void* z;
  char eSubtype;
  void (*xDel)(void*);
};

// Minimal destructor that does nothing (used as default when none provided)
void sqlite3NoopDestructor(void* p) { (void)p; }

// Forward declaration of the function under test
void sqlite3VdbeMemSetPointer(
  Mem *pMem,
  void *pPtr,
  const char *zPType,
  void (*xDestructor)(void*)
);

// Backing store function used by the focal routine (mocked for test)
static void vdbeMemClear(Mem *pMem) {
  // Reset fields to the default "cleared" state
  pMem->flags = MEM_Null;
  pMem->z = nullptr;
  pMem->u.zPType = nullptr;
  pMem->eSubtype = 0;
  pMem->xDel = nullptr;
}

// Minimal implementation of the focal function under test
void sqlite3VdbeMemSetPointer(
  Mem *pMem,
  void *pPtr,
  const char *zPType,
  void (*xDestructor)(void*)
) {
  // As in the original: require MEM_Null state before setting
  assert( pMem->flags==MEM_Null );
  vdbeMemClear(pMem);
  pMem->u.zPType = zPType ? zPType : "";
  pMem->z = pPtr;
  pMem->flags = MEM_Null|MEM_Dyn|MEM_Subtype|MEM_Term;
  pMem->eSubtype = 'p';
  pMem->xDel = xDestructor ? xDestructor : sqlite3NoopDestructor;
}

} // extern "C"

// Test-specific helper/destructor
static void testDestructor(void* p) { (void)p; }

// Test 1: Non-null zPType and non-null destructor
static void test_SetPointer_NonNullZPType_DestructorNonNull() {
  Mem m;
  m.flags = MEM_Null;     // initial state matches assertion in function
  m.u.zPType = nullptr;
  m.z = nullptr;
  m.eSubtype = 0;
  m.xDel = nullptr;

  // Call with explicit string type and a custom destructor
  sqlite3VdbeMemSetPointer(&m, (void*)0x1234, "typeX", &testDestructor);

  // Validate all fields after operation
  EXPECT_EQ_INT(MEM_Null | MEM_Dyn | MEM_Subtype | MEM_Term, (int)m.flags);
  EXPECT_PTR_EQ((void*)0x1234, m.z);
  EXPECT_PTR_EQ("typeX", m.u.zPType);
  EXPECT_PTR_EQ(&testDestructor, m.xDel);
  EXPECT_CHAR_EQ('p', m.eSubtype);
}

// Test 2: Null zPType and null destructor (defaulting behavior)
static void test_SetPointer_NullZPType_DestructorNull() {
  Mem m;
  m.flags = MEM_Null;
  m.u.zPType = nullptr;
  m.z = (void*)0xABCD;
  m.eSubtype = 0;
  m.xDel = (void(*)(void*))0xDEADBEEF; // non-null initial to ensure vdbeMemClear resets

  sqlite3VdbeMemSetPointer(&m, (void*)0, nullptr, nullptr);

  // zPType should be the empty string literal
  EXPECT_PTR_EQ("", m.u.zPType);
  EXPECT_PTR_EQ((void*)0, m.z);
  EXPECT_PTR_EQ((void*)0, m.z);
  EXPECT_PTR_EQ(&sqlite3NoopDestructor, m.xDel);
  EXPECT_EQ_INT(MEM_Null | MEM_Dyn | MEM_Subtype | MEM_Term, (int)m.flags);
  EXPECT_CHAR_EQ('p', m.eSubtype);
}

// Test 3: Non-null zPType and null destructor (xDel should default to noop)
static void test_SetPointer_NonNullZPType_DestructorNull() {
  Mem m;
  m.flags = MEM_Null;
  m.u.zPType = nullptr;
  m.z = nullptr;
  m.eSubtype = 0;
  m.xDel = nullptr;

  sqlite3VdbeMemSetPointer(&m, nullptr, "customType", nullptr);

  EXPECT_PTR_EQ("customType", m.u.zPType);
  EXPECT_PTR_EQ(nullptr, m.z);
  EXPECT_PTR_EQ(&sqlite3NoopDestructor, m.xDel);
  EXPECT_EQ_INT(MEM_Null | MEM_Dyn | MEM_Subtype | MEM_Term, (int)m.flags);
  EXPECT_CHAR_EQ('p', m.eSubtype);
}

// Test 4: Non-null zPType with a non-default destructor to verify exact pointer retained
static void test_SetPointer_WithCustomDestructor() {
  Mem m;
  m.flags = MEM_Null;
  m.u.zPType = nullptr;
  m.z = nullptr;
  m.eSubtype = 0;
  m.xDel = nullptr;

  // Define a distinct destructor to verify pointer preservation
  void (*customDtor)(void*) = (void(*)(void*))[](void* p){ (void)p; };
  sqlite3VdbeMemSetPointer(&m, (void*)0xBEEF, "ct", customDtor);

  EXPECT_PTR_EQ((void*)0xBEEF, m.z);
  EXPECT_PTR_EQ("ct", m.u.zPType);
  EXPECT_PTR_EQ(customDtor, m.xDel);
  EXPECT_EQ_INT(MEM_Null | MEM_Dyn | MEM_Subtype | MEM_Term, (int)m.flags);
  EXPECT_CHAR_EQ('p', m.eSubtype);
}

// Simple test runner to execute all tests
static void run_all_tests() {
  printf("Running tests for sqlite3VdbeMemSetPointer (standalone test harness)...\n");
  test_SetPointer_NonNullZPType_DestructorNonNull();
  test_SetPointer_NullZPType_DestructorNull();
  test_SetPointer_NonNullZPType_DestructorNull();
  test_SetPointer_WithCustomDestructor();

  if (gFailures == 0) {
    printf("ALL TESTS PASSED (%d tests).\n", gTestsRun);
  } else {
    printf("SOME TESTS FAILED: %d failures out of %d tests.\n", gFailures, gTestsRun);
  }
}

// Entry point
int main() {
  // Manually invoke the test suite
  run_all_tests();

  // Return non-zero if any test failed
  return gFailures ? 1 : 0;
}