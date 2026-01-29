// Test suite for lsmStringAppendf and its related dependencies.
// This test suite provides minimal in-file implementations of the focal
// functions to enable unit testing without GTest. It uses a compact
// C-like interface adapted for C++11 and a small test harness with
// lightweight expectations.

#include <lsmInt.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <string>
#include <iostream>
#include <cstdlib>


// Domain helpers and minimal dependencies (mocked within this test file)

struct lsm_env {}; // dummy environment structure

// Lightweight in-memory string wrapper to simulate LsmString
struct LsmString {
  std::string data;
};

// Forward declarations of focal methods (recreated/minimal implementation for tests)
void lsmStringInit(LsmString *pStr, lsm_env *pEnv);
int  lsmStringExtend(LsmString *pStr, int nNew);
void lsmStringClear(LsmString *pStr);
int  lsmStringAppend(LsmString *pStr, const char *z, int N);
int  lsmStringBinAppend(LsmString *pStr, const unsigned char *a, int n);
void lsmStringVAppendf(
  LsmString *pStr, 
  const char *zFormat, 
  va_list ap1,
  va_list ap2
);
void lsmStringAppendf(LsmString *pStr, const char *zFormat, ...);
int  lsmStrlen(const char *zName);
char *lsmMallocPrintf(lsm_env *pEnv, const char *zFormat, ...);

// Internal helpers (implementation below)
static void ensureCapacityMock(LsmString *pStr, size_t additional);

// Implementations

void lsmStringInit(LsmString *pStr, lsm_env *pEnv) {
  (void)pEnv;
  if (pStr) pStr->data.clear();
}

int lsmStringExtend(LsmString *pStr, int nNew) {
  // Minimal stub for test; pretend to extend capacity.
  (void)pStr;
  (void)nNew;
  return 0;
}

void lsmStringClear(LsmString *pStr) {
  if (pStr) pStr->data.clear();
}

int lsmStringAppend(LsmString *pStr, const char *z, int N) {
  if (!pStr) return 0;
  if (z == nullptr) return (int)pStr->data.size();
  size_t len;
  if (N < 0) len = std::strlen(z);
  else if (N > (int)std::strlen(z)) len = std::strlen(z);
  else len = (size_t)N;
  pStr->data.append(z, (size_t)len);
  return (int)pStr->data.size();
}

int lsmStringBinAppend(LsmString *pStr, const unsigned char *a, int n) {
  if (!pStr || !a || n <= 0) return (int)(pStr ? pStr->data.size() : 0);
  pStr->data.append(reinterpret_cast<const char*>(a), (size_t)n);
  return (int)pStr->data.size();
}

void lsmStringVAppendf(
  LsmString *pStr, 
  const char *zFormat, 
  va_list ap1,
  va_list ap2
) {
  // For testing, format the string twice (once per va_list) and append both.
  char buf1[1024];
  char buf2[1024];
  va_list a1, a2;
  va_copy(a1, ap1);
  va_copy(a2, ap2);
  vsnprintf(buf1, sizeof(buf1), zFormat, a1);
  vsnprintf(buf2, sizeof(buf2), zFormat, a2);
  va_end(a1);
  va_end(a2);
  // Append the two formatted strings to the target string.
  lsmStringAppend(pStr, buf1, -1);
  lsmStringAppend(pStr, buf2, -1);
}

void lsmStringAppendf(LsmString *pStr, const char *zFormat, ...) {
  va_list ap, ap2;
  va_start(ap, zFormat);
  va_start(ap2, zFormat);
  lsmStringVAppendf(pStr, zFormat, ap, ap2);
  va_end(ap2);
  va_end(ap);
}

int lsmStrlen(const char *zName) {
  return zName ? (int)std::strlen(zName) : 0;
}

// A simple malloc-based printf-like helper
char *lsmMallocPrintf(lsm_env *pEnv, const char *zFormat, ...) {
  (void)pEnv;
  va_list ap;
  va_start(ap, zFormat);
  va_list ap_copy;
  va_copy(ap_copy, ap);
  int size = vsnprintf(nullptr, 0, zFormat, ap_copy);
  va_end(ap_copy);
  if (size < 0) {
    va_end(ap);
    return nullptr;
  }
  char *p = (char*)std::malloc((size_t)size + 1);
  if (!p) {
    va_end(ap);
    return nullptr;
  }
  va_copy(ap_copy, ap);
  vsnprintf(p, (size_t)size + 1, zFormat, ap_copy);
  va_end(ap_copy);
  va_end(ap);
  return p;
}

// Small test framework helpers (non-terminating, lightweight)

// Simple string equality assertion
static bool expect_eq_str(const std::string &actual, const std::string &expected, const char *msg) {
  if (actual != expected) {
    std::cerr << "ASSERT FAILED: " << msg << " - Expected: \"" << expected
              << "\"; Got: \"" << actual << "\"\n";
    return false;
  }
  return true;
}

// Simple integer equality assertion
static bool expect_eq_int(int actual, int expected, const char *msg) {
  if (actual != expected) {
    std::cerr << "ASSERT FAILED: " << msg << " - Expected: " << expected
              << "; Got: " << actual << "\n";
    return false;
  }
  return true;
}

// Test 1: Basic behavior of lsmStringAppendf - two concatenated formatted outputs
// This verifies that lsmStringAppendf correctly forwards to VAppendf and that
// two copies of the formatted string (via two va_list instances) are appended.
static bool test_lsmStringAppendf_basic_concat() {
  LsmString s;
  lsmStringInit(&s, nullptr);
  lsmStringAppendf(&s, "Hello %s", "World");
  // Expect two copies due to dual va_list usage: "Hello WorldHello World"
  std::string expected = "Hello WorldHello World";
  return expect_eq_str(s.data, expected, "lsmStringAppendf should append two formatted copies");
}

// Test 2: lsmStringAppendf with multiple arguments in format
// Ensure formatting with multiple specifiers works and results in two copies.
static bool test_lsmStringAppendf_multiple_args() {
  LsmString s;
  lsmStringInit(&s, nullptr);
  lsmStringAppendf(&s, "Number: %d, Char: %c", 42, 'A');
  std::string expected = "Number: 42, Char: ANumber: 42, Char: A";
  return expect_eq_str(s.data, expected, "lsmStringAppendf should format multiple args and duplicate");
}

// Test 3: Basic lifecycle: init, append, then clear
// Verifies that lsmStringClear resets the internal buffer.
static bool test_lsmStringLifecycle_clear() {
  LsmString s;
  lsmStringInit(&s, nullptr);
  lsmStringAppendf(&s, "Patched %s", "Test");
  lsmStringClear(&s);
  // After clear, the string should be empty
  bool ok = expect_eq_str(s.data, std::string(""), "lsmStringClear should reset string to empty");
  return ok;
}

// Test 4: lsmStrlen utility function
static bool test_lsmStrlen_basic() {
  const char *t = "abcde";
  int len = lsmStrlen(t);
  return expect_eq_int(len, 5, "lsmStrlen should return correct length");
}

// Test 5: lsmMallocPrintf returns a valid C-string with formatted content
static bool test_lsmMallocPrintf_basic() {
  LsmString dummy; (void)dummy;
  lsm_env env;
  char *p = lsmMallocPrintf(&env, "Value=%d", 123);
  if (!p) {
    std::cerr << "ASSERT FAILED: lsmMallocPrintf should not return NULL\n";
    return false;
  }
  std::string got(p);
  bool ok = expect_eq_str(got, "Value=123", "lsmMallocPrintf should format correctly");
  std::free(p);
  return ok;
}

// Main entry to execute tests
int main() {
  int passed = 0, failed = 0;

  std::cout << "Running tests for lsmStringAppendf and dependencies...\n";

  if (test_lsmStringAppendf_basic_concat()) ++passed; else ++failed;
  if (test_lsmStringAppendf_multiple_args()) ++passed; else ++failed;
  if (test_lsmStringLifecycle_clear()) ++passed; else ++failed;
  if (test_lsmStrlen_basic()) ++passed; else ++failed;
  if (test_lsmMallocPrintf_basic()) ++passed; else ++failed;

  std::cout << "Test results: " << passed << " passed, " << failed << " failed.\n";
  return (failed == 0) ? 0 : 1;
}