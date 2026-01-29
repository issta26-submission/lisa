// Lightweight, self-contained C++11 unit-test scaffolding for sqlite3ValueApplyAffinity
// NOTE: This is a self-contained mock/test harness to exercise the wrapper behavior described.
// It does not rely on GTest or any external sqlite3 libraries.
// It redefines minimal structures and affinity logic to validate that the wrapper
// forwards control to applyAffinity with the correct parameters and that various
// branch-like scenarios are exercised.

#include <cmath>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>


// Lightweight, self-contained mock of internal sqlite3_value/mem abstractions
// and affinity constants used by sqlite3ValueApplyAffinity.

using u8 = unsigned char;

// Mocked internal MEM flags (subset sufficient for tests)
enum MemFlag {
  MEM_Null   = 0x01,
  MEM_Int    = 0x02,
  MEM_Real   = 0x04,
  MEM_Str    = 0x08,
  MEM_Blob   = 0x10,
  MEM_IntReal= 0x20,
  MEM_Zero   = 0x40,
  MEM_Ephem  = 0x80,
  MEM_Static = 0x100
};

// Public-ish affinity constants (subset)
enum SQLITE_AFF {
  SQLITE_AFF_NONE   = 0,
  SQLITE_AFF_NUMERIC= 1,
  SQLITE_AFF_TEXT   = 2,
  SQLITE_AFF_BLOB   = 3,
  SQLITE_AFF_REAL   = 4,
  SQLITE_AFF_INTEGER= 5
};

// Minimal Mem-like structure to back tests
struct Mem {
  unsigned int flags; // combination of MemFlag
  int u;                // integer value (Mem_Int)
  double r;             // real value (Mem_Real)
  char* z;              // string/blob data
  int n;                  // length of z
  unsigned char enc;     // encoding placeholder
  Mem() : flags(MEM_Null), u(0), r(0.0), z(nullptr), n(0), enc(0) {}
};

// sqlite3_value is an opaque handle in SQLite; for test we alias to Mem
using sqlite3_value = Mem;

// Global flag to verify that applyAffinity() is invoked by the wrapper
static bool g_applyAffinity_called = false;

// Forward declaration of the helper used by wrapper
static void applyAffinity(Mem *pRec, unsigned char affinity, unsigned char enc);

// The focal wrapper under test (as per the provided snippet)
// In the real project this would be in vdbe.c; here we replicate behavior for unit testing.
extern "C" void sqlite3ValueApplyAffinity(
  sqlite3_value *pVal,
  u8 affinity,
  u8 enc
){
  applyAffinity((Mem *)pVal, affinity, enc);
}

// Minimal, self-contained implementation of applyAffinity used for tests.
// This is a simplified stand-in to exercise the wrapper's control flow.
// It mutates Mem in predictable ways, enabling assertions in tests.
static void applyAffinity(Mem *pRec, unsigned char affinity, unsigned char enc){
  // Record that the function was invoked (testability)
  g_applyAffinity_called = true;
  // Always propagate the encoding parameter to the Mem (as the real code would)
  pRec->enc = enc;

  // Extremely small, self-contained affinity logic (subset of real behavior)
  // - NUMERIC: try to interpret string data as int or real; otherwise no-op
  // - TEXT: convert numeric values to text strings (INT/REAL -> STR)
  if (affinity == SQLITE_AFF_NUMERIC) {
    // If value is a string, attempt numeric parsing
    if (pRec->flags & MEM_Str && pRec->z && pRec->n > 0) {
      std::string s(pRec->z, pRec->n);
      // Try integer
      try {
        size_t pos = 0;
        long long val = std::stoll(s, &pos);
        if (pos == s.size()) {
          pRec->flags = MEM_Int;
          pRec->u = (int)val;
          return;
        }
      } catch (...) {
        // not an integer
      }
      // Try floating point
      try {
        size_t pos = 0;
        double dv = std::stod(s, &pos);
        if (pos == s.size()) {
          pRec->flags = MEM_Real;
          pRec->r = dv;
          return;
        }
      } catch (...) {
        // not a real
      }
    }
    // If not numeric convertible, leave as-is (per simplified contract)
  } else if (affinity == SQLITE_AFF_TEXT) {
    // If currently numeric, convert to textual representation
    if (pRec->flags & MEM_Int) {
      char buf[32];
      std::snprintf(buf, sizeof(buf), "%d", pRec->u);
      pRec->z = new char[strlen(buf) + 1];
      std::strcpy(pRec->z, buf);
      pRec->n = (int)strlen(pRec->z);
      pRec->flags = MEM_Str;
    } else if (pRec->flags & MEM_Real) {
      char buf[64];
      std::snprintf(buf, sizeof(buf), "%g", pRec->r);
      pRec->z = new char[strlen(buf) + 1];
      std::strcpy(pRec->z, buf);
      pRec->n = (int)strlen(pRec->z);
      pRec->flags = MEM_Str;
    }
  }
  // Other affinities are not exercised in these tests (kept minimal)
}

// Lightweight test scaffolding
#define RUN_TEST(func) do { \
  std::cerr << "RUN  " #func "...\n"; \
  bool ok = func(); \
  std::cerr << (ok ? "PASS" : "FAIL") << "  " #func "\n"; \
  if(!ok) all_pass = false; \
} while(0)

static void freeMemString(Mem &m){
  if (m.z) { delete[] m.z; m.z = nullptr; m.n = 0; }
}

// Test 1: NUMERIC affinity on a numeric string should convert to MEM_Int and set value
bool test_numeric_affinity_converts_string_to_int() {
  g_applyAffinity_called = false;
  Mem m;
  m.flags = MEM_Str;
  m.n = 3;
  m.z = new char[4];
  std::strcpy(m.z, "123");
  m.enc = 0;

  sqlite3_value val = m;
  sqlite3ValueApplyAffinity(&val, SQLITE_AFF_NUMERIC, 0);

  bool result = g_applyAffinity_called
                && (val.flags & MEM_Int)
                && (val.u == 123)
                && (val.enc == 0);
  freeMemString(val);
  return result;
}

// Test 2: NUMERIC affinity on a decimal string should convert to MEM_Real
bool test_numeric_affinity_converts_string_to_real() {
  g_applyAffinity_called = false;
  Mem m;
  m.flags = MEM_Str;
  m.n = 5;
  m.z = new char[6];
  std::strcpy(m.z, "123.45");
  m.enc = 0;

  sqlite3_value val = m;
  sqlite3ValueApplyAffinity(&val, SQLITE_AFF_NUMERIC, 0);

  bool result = g_applyAffinity_called
                && (val.flags & MEM_Real)
                && std::abs(val.r - 123.45) < 1e-9
                && (val.enc == 0);
  freeMemString(val);
  return result;
}

// Test 3: NUMERIC affinity on a non-numeric string should leave as Str
bool test_numeric_affinity_on_non_numeric_string_keeps_string() {
  g_applyAffinity_called = false;
  Mem m;
  m.flags = MEM_Str;
  m.n = 3;
  m.z = new char[4];
  std::strcpy(m.z, "abc");
  m.enc = 1;

  sqlite3_value val = m;
  sqlite3ValueApplyAffinity(&val, SQLITE_AFF_NUMERIC, 1);

  bool result = g_applyAffinity_called
                && (val.flags & MEM_Str)
                && val.n == 3
                && std::strncmp(val.z, "abc", 3) == 0
                && val.enc == 1;
  freeMemString(val);
  return result;
}

// Test 4: TEXT affinity on an INT should convert to textual representation
bool test_text_affinity_converts_int_to_text() {
  g_applyAffinity_called = false;
  Mem m;
  m.flags = MEM_Int;
  m.u = 42;
  m.z = nullptr;
  m.n = 0;
  m.enc = 2;

  sqlite3_value val = m;
  sqlite3ValueApplyAffinity(&val, SQLITE_AFF_TEXT, 2);

  bool result = g_applyAffinity_called
                && (val.flags & MEM_Str)
                && val.n == 2
                && std::strcmp(val.z, "42") == 0
                && val.enc == 2;
  freeMemString(val);
  return result;
}

// Test 5: Encoding parameter is propagated into the Mem via the wrapper
bool test_encoding_propagated_to_mem() {
  g_applyAffinity_called = false;
  Mem m;
  m.flags = MEM_Str;
  m.n = 2;
  m.z = new char[3];
  std::strcpy(m.z, "9");
  m.enc = 0;

  sqlite3_value val = m;
  // Use NUMERIC affinity and a non-zero enc to ensure it propagates
  sqlite3ValueApplyAffinity(&val, SQLITE_AFF_NUMERIC, 0xAB);

  bool result = g_applyAffinity_called
                && val.enc == 0xAB;
  freeMemString(val);
  return result;
}

// Test 6: Null value pathway - ensure wrapper calls applyAffinity even for Nulls
bool test_null_value_pathway_invokes_applyAffinity() {
  g_applyAffinity_called = false;
  Mem m;
  m.flags = MEM_Null;
  m.n = 0;
  m.z = nullptr;

  sqlite3_value val = m;
  sqlite3ValueApplyAffinity(&val, SQLITE_AFF_NUMERIC, 0);

  bool result = g_applyAffinity_called; // we still expect the call to occur
  return result;
}

// Entry point for tests
int main() {
  bool all_pass = true;

  // Run tests
  RUN_TEST(test_numeric_affinity_converts_string_to_int);
  RUN_TEST(test_numeric_affinity_converts_string_to_real);
  RUN_TEST(test_numeric_affinity_on_non_numeric_string_keeps_string);
  RUN_TEST(test_text_affinity_converts_int_to_text);
  RUN_TEST(test_encoding_propagated_to_mem);
  RUN_TEST(test_null_value_pathway_invokes_applyAffinity);

  // Cleanup potential allocations in tests (safe-guard)
  // Note: individual tests already freed Mem.z where allocated.

  if(all_pass){
    std::cerr << "ALL TESTS PASSED\n";
    return 0;
  }else{
    std::cerr << "SOME TESTS FAILED\n";
    return 1;
  }
}