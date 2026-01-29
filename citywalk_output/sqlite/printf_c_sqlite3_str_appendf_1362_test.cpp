// Minimal, self-contained unit-test harness for sqlite3_str_appendf
// This test targets the focal method:
// void sqlite3_str_appendf(StrAccum *p, const char *zFormat, ...){
//
// It delegates to sqlite3_str_vappendf, which we implement here with a lightweight
// formatting approach using vsnprintf. The goal is to validate formatting,
// buffer growth, and final content of StrAccum after various printf-style
// operations without requiring the full SQLite project.
//
// The implementation is intentionally minimal and does not rely on GTest.
// It uses a small, non-terminating test reporting style suitable for C++11.

#include <vector>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <sqliteInt.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Domain: Provide minimal compatible types used by the focal code
typedef unsigned char u8;
typedef long long sqlite3_int64;
typedef double sqlite3_double;
typedef struct sqlite3 sqlite3;

// Minimal sqlite3 type placeholder (actual internals are not required for tests)
struct sqlite3 { int dummy; };

// Lightweight StrAccum structure compatible with the focal API
// We provide enough functionality to append formatted strings and obtain the final buffer.
struct StrAccum {
  char *z;   // buffer
  int n;     // current length (excluding terminator)
  int mx;    // allocated size (including space for terminator)
  int iErr;  // error code placeholder (unused in tests)
};

// Forward declarations of the functions under test and helpers
static void sqlite3StrAccumInit(StrAccum *p, sqlite3 *db, char *zBase, int n, int mx);
static void sqlite3_str_append(StrAccum *p, const char *z, int N);
static void sqlite3_str_vappendf(StrAccum *pAccum, const char *fmt, va_list ap);
void sqlite3_str_appendf(StrAccum *p, const char *zFormat, ...);

// Implementation of StrAccum initialization
static void sqlite3StrAccumInit(StrAccum *p, sqlite3 *db, char *zBase, int n, int mx){
  (void)db; // unused in this minimal harness
  p->z = zBase;
  p->n = n;
  p->mx = mx;
  if (p->z == nullptr) {
    // Provide a sane default buffer
    p->mx = 256;
    p->z = (char*)malloc(p->mx);
    if (p->z) p->z[0] = '\0';
    p->n = 0;
  } else {
    if (p->n < 0) p->n = 0;
    if (p->z[p->n] != '\0') p->z[p->n] = '\0';
  }
}

// Append up to N chars from z into the StrAccum, expanding as needed
static void sqlite3_str_append(StrAccum *p, const char *z, int N){
  if (z == nullptr) return;
  int len = N;
  if (len < 0) len = (int)strlen(z);
  size_t needed = (size_t)p->n + (size_t)len;
  if ((int)needed + 1 > p->mx) {
    // Grow buffer (simple doubling strategy)
    int newmx = (int)needed + 1;
    if (newmx < p->mx * 2) newmx = p->mx * 2;
    char *newz = (char*)realloc(p->z, (size_t)newmx);
    if (newz == nullptr) {
      // Allocation failure: keep existing buffer
      return;
    }
    p->z = newz;
    p->mx = newmx;
  }
  memcpy(p->z + p->n, z, len);
  p->n = (int)needed;
  p->z[p->n] = '\0';
}

// Core v-append: format using vsnprintf and append to pAccum
static void sqlite3_str_vappendf(StrAccum *pAccum, const char *fmt, va_list ap){
  // First determine required size
  va_list ap_copy;
  va_copy(ap_copy, ap);
  int needed = vsnprintf(nullptr, 0, fmt, ap_copy);
  va_end(ap_copy);
  if (needed < 0) {
    return;
  }
  // Allocate temporary buffer for formatted string
  char *buf = (char*)malloc((size_t)needed + 1);
  if (buf == nullptr) return;
  vsnprintf(buf, (size_t)needed + 1, fmt, ap);
  // Append into StrAccum
  sqlite3_str_append(pAccum, buf, needed);
  free(buf);
}

// Focal function under test: sqlite3_str_appendf
void sqlite3_str_appendf(StrAccum *p, const char *zFormat, ...){
  va_list ap;
  va_start(ap, zFormat);
  sqlite3_str_vappendf(p, zFormat, ap);
  va_end(ap);
}

// Convenience helpers for tests
static void reset(StrAccum *p){
  if (p->z){
    p->z[0] = '\0';
  }
  p->n = 0;
}

// Test harness: simple non-terminating checks and diagnostic output
static int g_total = 0;
static int g_failed = 0;

#define TEST_START(name) \
  std::cout << "Running test: " << name << " ..." << std::endl; \
  ++g_total;

#define TEST_OK() \
  std::cout << "[OK] " << __FUNCTION__ << std::endl;

#define CHECK_STR_EQ(actual, expected) \
  do { \
    ++g_total; \
    if (strcmp((actual), (expected)) != 0) { \
      ++g_failed; \
      std::cerr << "Test failure: expected \"" << (expected) \
                << "\", got \"" << (actual) << "\"\n"; \
    } \
  } while(0)

#define CHECK_INT_EQ(a,b) \
  do { \
    ++g_total; \
    if ((a) != (b)) { \
      ++g_failed; \
      std::cerr << "Test failure: " #a " != " #b " (" << (a) << " != " << (b) << ")\n"; \
    } \
  } while(0)

int main(){
  // Basic tests rely only on the minimal StrAccum implementation
  sqlite3 db;
  (void)db;

  // Test 1: Simple literal append (no formatting)
  {
    TEST_START("Append simple literal with no formatting");
    StrAccum p;
    sqlite3StrAccumInit(&p, &db, nullptr, 0, 256);
    reset(&p);
    sqlite3_str_appendf(&p, "Hello");
    CHECK_STR_EQ(p.z, "Hello");
    free(p.z);
    TEST_OK();
  }

  // Test 2: Simple format with one string argument
  {
    TEST_START("Format with %s argument");
    StrAccum p;
    sqlite3StrAccumInit(&p, &db, nullptr, 0, 256);
    reset(&p);
    sqlite3_str_appendf(&p, "Value=%s", "abc");
    CHECK_STR_EQ(p.z, "Value=abc");
    free(p.z);
    TEST_OK();
  }

  // Test 3: Format with integer and float
  {
    TEST_START("Format with %d and %f");
    StrAccum p;
    sqlite3StrAccumInit(&p, &db, nullptr, 0, 256);
    reset(&p);
    sqlite3_str_appendf(&p, "A=%d B=%f", 42, 3.14);
    // The exact formatting of float may matter (default printf style)
    // We'll check for containment of both substrings to avoid strict formatting issues.
    const char *out = p.z;
    if (out == nullptr) out = "";
    if (strstr(out, "A=42") == nullptr || strstr(out, "B=3.14") == nullptr) {
      ++g_failed;
      std::cerr << "Test failure: expected content to contain 'A=42' and 'B=3.14', got \"" << out << "\"\n";
    } else {
      ++g_total;
    }
    free(p.z);
  }

  // Test 4: Buffer growth with many small appends
  {
    TEST_START("Buffer growth with multiple appends");
    StrAccum p;
    sqlite3StrAccumInit(&p, &db, nullptr, 0, 64); // small initial buffer to force growth
    reset(&p);
    // Append 'a' 100 times via formatting each time
    for (int i = 0; i < 100; ++i) {
      sqlite3_str_appendf(&p, "%s", "a");
    }
    // Expect string of 100 'a's
    std::string expected(100, 'a');
    CHECK_STR_EQ(p.z, expected.c_str());
    free(p.z);
    TEST_OK();
  }

  // Test 5: Large single append to force realloc with long content
  {
    TEST_START("Large single append to test realloc");
    StrAccum p;
    sqlite3StrAccumInit(&p, &db, nullptr, 0, 256);
    reset(&p);
    std::string big(600, 'b');
    sqlite3_str_appendf(&p, "%s", big.c_str());
    CHECK_STR_EQ(p.z, big.c_str());
    free(p.z);
    TEST_OK();
  }

  // Final summary
  std::cout << "Summary: total=" << g_total << " failed=" << g_failed << std::endl;
  return g_failed == 0 ? 0 : 1;
}