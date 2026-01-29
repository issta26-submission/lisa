// Lightweight C++11 unit tests for sqlite3VdbeIntegerAffinity (reproduced locally for testing)
// This test suite does not depend on GTest and uses a small custom lightweight test framework.
// It only relies on standard C++ library and minimal needed stubs to mimic the original C code behavior.

#include <climits>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdint>


// ------------------ Minimal stubs and definitions to support the focal function ------------------

// Typedefs to mirror sqlite internal types
typedef long long i64;

// Flags mimicking the sqlite Mem type flags
static const int MEM_Real    = 0x01;
static const int MEM_IntReal = 0x02;
static const int MEM_Int     = 0x04;

// Small, minimal Mem structure mirroring the parts used by sqlite3VdbeIntegerAffinity
struct Db {
  void* mutex;
};

struct Mem {
  int flags;
  union {
    double r;  // real value
    i64 i;     // integer value
  } u;
  Db* db;
};

// Convenience macro to set a type flag on Mem (mirrors MemSetTypeFlag)
static inline void MemSetTypeFlag(Mem *p, int flag) { p->flags |= flag; }

// Minimal stubs to satisfy assertions inside sqlite3VdbeIntegerAffinity
static inline int sqlite3VdbeMemIsRowSet(Mem *p) { (void)p; return 0; } // always not a row-set in tests
static inline int sqlite3_mutex_held(void* mutex) { (void)mutex; return 1; } // pretend mutex is always held

// Alignment check equivalent used in the original function (8-byte alignment)
static inline int EIGHT_BYTE_ALIGNMENT(Mem *p) { 
  return ((((uintptr_t)p) & 0x7) == 0);
}

// Real-to-i64 conversion used by the focal function
static inline i64 sqlite3RealToI64(double r) { return (i64) r; }

// Boundaries for 64-bit integer range
static const i64 SMALLEST_INT64 = LLONG_MIN;
static const i64 LARGEST_INT64  = LLONG_MAX;

// ------------------ Focal method under test (reproduced inline for self-contained tests) ------------------

void sqlite3VdbeIntegerAffinity(Mem *pMem){
  // Basic precondition checks (retained as asserts)
  // In tests we ensure these conditions hold to avoid assertion aborts.
  // Note: In production, these would be runtime checks waiting for debugging builds.
  if( pMem==nullptr ) { return; } // emulate assert( pMem!=0 );
  if( !(pMem->flags & (MEM_Real|MEM_IntReal)) ) { return; } // emulate assert
  if( sqlite3VdbeMemIsRowSet(pMem) ) { return; }        // emulate assert
  if( pMem->db!=nullptr && !sqlite3_mutex_held(pMem->db->mutex) ) { return; } // emulate assert
  if( !EIGHT_BYTE_ALIGNMENT(pMem) ) { return; } // emulate assert

  if( pMem->flags & MEM_IntReal ){
    MemSetTypeFlag(pMem, MEM_Int);
  }else{
    i64 ix = sqlite3RealToI64(pMem->u.r);
    if( pMem->u.r==ix && ix>SMALLEST_INT64 && ix<LARGEST_INT64 ){
      pMem->u.i = ix;
      MemSetTypeFlag(pMem, MEM_Int);
    }
  }
}

// ------------------ Lightweight test framework ------------------

static int g_failures = 0;
#define EXPECT_TRUE(cond) do { if(!(cond)){ std::cout << "EXPECT_TRUE FAILED: " #cond " at " << __FILE__ << ":" << __LINE__ << "\n"; ++g_failures; } } while(0)
#define EXPECT_FALSE(cond) EXPECT_TRUE(!(cond))
#define EXPECT_EQ(a,b) do { if((a)!=(b)){ std::cout << "EXPECT_EQ FAILED: " #a " == " #b " (" << (a) << " != " << (b) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; ++g_failures; } } while(0)
#define EXPECT_NE(a,b) do { if((a)==(b)){ std::cout << "EXPECT_NE FAILED: " #a " != " #b " (" << (a) << " == " << (b) << ") at " << __FILE__ << ":" << __LINE__ << "\n"; ++g_failures; } } while(0)

static void test_IntRealConvertsToInt() {
  // Scenario: MEM_IntReal path should set MEM_Int without touching the real/integer value semantics.
  // Key branch: if (pMem->flags & MEM_IntReal) { MemSetTypeFlag(pMem, MEM_Int); }
  Mem m;
  std::memset(&m, 0, sizeof(Mem));
  m.flags = MEM_IntReal;
  m.db = nullptr;
  m.u.r = 0.0;     // value irrelevant for MEM_IntReal path
  m.u.i = 0xDEADBEEFDEADBEEFull; // sentinel to ensure it's not overwritten unexpectedly

  sqlite3VdbeIntegerAffinity(&m);

  // Expect MEM_Int flag to be set, and existing u.i value preserved (as per function, it doesn't touch u.i in MEM_IntReal path)
  EXPECT_TRUE( (m.flags & MEM_Int) != 0 );
  EXPECT_EQ(m.u.i, 0xDEADBEEFDEADBEEFull );
  // If MEM_IntReal path changed the u.i, this test would fail, ensuring the path is correct.
}

static void test_RealExactIntConvertsToInt() {
  // Scenario: MEM_Real path with an exact integer value within 64-bit range should convert to MEM_Int and place value in u.i
  Mem m;
  std::memset(&m, 0, sizeof(Mem));
  m.flags = MEM_Real;  // only Real initially
  m.db = nullptr;
  m.u.r = 5.0;
  m.u.i = 0; // sentinel to ensure it's overwritten as appropriate
  sqlite3VdbeIntegerAffinity(&m);
  EXPECT_TRUE( (m.flags & MEM_Int) != 0 );
  EXPECT_EQ(m.u.i, (i64)5 );
}

static void test_RealNonExactNoConversion() {
  // Scenario: MEM_Real path with a non-integer real should NOT set MEM_Int and should leave u.i untouched
  Mem m;
  std::memset(&m, 0, sizeof(Mem));
  m.flags = MEM_Real;
  m.db = nullptr;
  const i64 sentinel = 0xDEADBEEFDEADC0LL;
  m.u.i = sentinel;
  m.u.r = 4.1; // non-integer real
  sqlite3VdbeIntegerAffinity(&m);
  EXPECT_TRUE( (m.flags & MEM_Int) == 0 );
  EXPECT_EQ(m.u.i, sentinel);
}

static void test_IntReal_withDbPointerPath() {
  // Scenario: Ensure that providing a non-null db pointer (with a dummy mutex) satisfies the assertion path
  // and still correctly handles MEM_IntReal path by setting MEM_Int.
  Mem m;
  std::memset(&m, 0, sizeof(Mem));
  m.flags = MEM_IntReal;
  m.db = new Db{ reinterpret_cast<void*>(0x1234) };
  // The internal sqlite3_mutex_held is mocked to always return true; thus the assertion passes.
  m.u = {}; // zeroed
  sqlite3VdbeIntegerAffinity(&m);
  EXPECT_TRUE( (m.flags & MEM_Int) != 0 );
  delete m.db;
}

// ------------------ Main runner ------------------

int main() {
  std::cout << "Starting sqlite3VdbeIntegerAffinity unit tests (minimal C++11 harness)\n";

  test_IntRealConvertsToInt();
  test_RealExactIntConvertsToInt();
  test_RealNonExactNoConversion();
  test_IntReal_withDbPointerPath();

  if( g_failures == 0 ){
    std::cout << "All tests PASSED.\n";
  } else {
    std::cout << "Tests completed with " << g_failures << " failure(s).\n";
  }
  return g_failures ? 1 : 0;
}