/*
  Test Suite for sqlite3_quota_fread

  Step 1 - Program Understanding (Key Components)
  - Core function under test: sqlite3_quota_fread
    size_t sqlite3_quota_fread(void *pBuf, size_t size, size_t nmemb, quota_FILE *p)
    This function delegates to the standard C fread to read from the FILE* contained
    inside the quota_FILE wrapper (p->f).
  - Dependencies: quota_FILE type (defined in test_quota.h in the project), and the
    sqlite3_quota_fread symbol (C function). Our tests rely on real behavior of fread
    via the quota wrapper.
  - Candidate Keywords (core dependent components):
    sqlite3_quota_fread, quota_FILE, p->f, fread, pBuf, size, nmemb, FILE*
  - Testing goal: validate normal reads, EOF/partial reads, and zero-length reads, ensuring
    correct return values and buffer behavior without terminating the test suite early.

  Step 2 - Unit Test Generation
  - Design: Provide a small C++11 test harness (no GTest) that uses the actual
    sqlite3_quota_fread function. We will
    - Create an in-memory temporary file (via tmpfile()) with known content.
    - Populate a quota_FILE container with the FILE* and call sqlite3_quota_fread
      with various size/nmemb combinations.
    - Verify:
      - Correct number of elements read (return value).
      - Buffer content matches expected bytes.
      - Unused buffer areas remain untouched for zero-read scenarios.
  - Tests included:
    1) test_quota_fread_normal: read exact content length with size=1, nmemb=length.
    2) test_quota_fread_eof_partial: request more than available; ensure it reads only available bytes.
    3) test_quota_fread_zero_length: size=0 or nmemb=0 should yield 0 and not modify buffer content.
    4) test_quota_fread_multi_byte: use size>1 and nmemb to ensure proper handling of multi-byte reads.

  Step 3 - Test Case Refinement
  - Use a lightweight test harness (no terminating assertions). Each test reports pass/fail.
  - Make use of the provided test_quota.h and quota_FILE type, which ensures
    compatibility with the project's quota wrapper.
  - Keep tests self-contained, using std::FILE* via tmpfile() to avoid filesystem side effects.

  Notes:
  - This test suite assumes test_quota.h provides and defines quota_FILE and related
    wrappers used by sqlite3_quota_fread. We link against the project sources providing
    sqlite3_quota_fread.
  - We expose a C-style function prototype for sqlite3_quota_fread with C linkage to
    avoid name mangling when called from C++.

  Build/run guidance:
  - Compile as C++11, linking with the project sources that define sqlite3_quota_fread
    and test_quota.h (as provided by the repository).
  - No GTest is used.
*/

#include <string.h>
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <test_quota.h>
#include <os_setup.h>
#include <iostream>


// Include the project's quota wrapper header to obtain quota_FILE definition.
// This header is expected to provide quota_FILE and related declarations used by
// sqlite3_quota_fread in the test environment.

// Ensure correct linkage for the C function under test
extern "C" {
  // Prototype for the quota-aware fread wrapper under test.
  // quota_FILE is defined in test_quota.h as part of the quota wrapper.
  size_t sqlite3_quota_fread(void *pBuf, size_t size, size_t nmemb, quota_FILE *p);
}

// Helper: create an in-memory temporary file with given content.
// Returns a FILE* opened in binary mode via tmpfile(), which will be closed by the caller.
static FILE* makeTempFileWithContent(const char* data, size_t len) {
  FILE* f = tmpfile();
  if (!f) {
    return nullptr;
  }
  if (len > 0) {
    fwrite(data, 1, len, f);
  }
  fflush(f);
  // Rewind to the beginning for reading via quota_fread
  fseek(f, 0, SEEK_SET);
  return f;
}

// Test 1: Normal read - read exactly the content length using size=1, nmemb=len
static bool test_quota_fread_normal() {
  // Domain: Read a known block of data and verify it matches exactly
  const char data[] = "0123456789ABCDEF"; // 16 bytes
  const size_t len = sizeof(data) - 1;

  FILE* f = makeTempFileWithContent(data, len);
  if (!f) {
    std::cerr << "test_quota_fread_normal: failed to create temp file\n";
    return false;
  }

  quota_FILE q;
  // Initialize quota_FILE wrapper with the underlying FILE*
  // The real structure is assumed to have a member 'f' (as used by sqlite3_quota_fread)
  q.f = f;

  // Prepare a buffer filled with a sentinel to detect overrun
  char buf[64];
  memset(buf, 0xAA, sizeof(buf));

  size_t got = sqlite3_quota_fread(buf, 1, len, &q);

  bool ok = true;
  if (got != len) {
    std::cerr << "test_quota_fread_normal: expected got=" << len << " but got=" << got << "\n";
    ok = false;
  }
  if (memcmp(buf, data, len) != 0) {
    std::cerr << "test_quota_fread_normal: buffer content mismatch\n";
    ok = false;
  }

  // Ensure bytes beyond what was read are untouched (sentinel 0xAA)
  for (size_t i = len; i < sizeof(buf); ++i) {
    if (buf[i] != (char)0xAA) {
      std::cerr << "test_quota_fread_normal: buffer sentinel altered at position " << i << "\n";
      ok = false;
      break;
    }
  }

  fclose(f);
  return ok;
}

// Test 2: EOF/partial read - request more bytes than exist; should read only available bytes
static bool test_quota_fread_eof_partial() {
  const char data[] = "abcdefghij"; // 10 bytes
  const size_t len = sizeof(data) - 1;

  FILE* f = makeTempFileWithContent(data, len);
  if (!f) {
    std::cerr << "test_quota_fread_eof_partial: failed to create temp file\n";
    return false;
  }

  quota_FILE q;
  q.f = f;

  // Buffer initialized with a sentinel; we'll read more than exists
  char buf[32];
  memset(buf, 0xBB, sizeof(buf));

  size_t got = sqlite3_quota_fread(buf, 1, 20, &q); // request 20 bytes

  bool ok = true;
  if (got != len) {
    std::cerr << "test_quota_fread_eof_partial: expected got=" << len << " but got=" << got << "\n";
    ok = false;
  }
  // First len bytes should match content
  if (memcmp(buf, data, len) != 0) {
    std::cerr << "test_quota_fread_eof_partial: buffer content mismatch for read data\n";
    ok = false;
  }
  // Bytes beyond len should be untouched (still 0xBB)
  for (size_t i = len; i < sizeof(buf); ++i) {
    if (buf[i] != (char)0xBB) {
      std::cerr << "test_quota_fread_eof_partial: buffer sentinel altered at position " << i << "\n";
      ok = false;
      break;
    }
  }

  fclose(f);
  return ok;
}

// Test 3: Zero-length reads - size=0 or nmemb=0 should return 0 and not modify buffer
static bool test_quota_fread_zero_length() {
  const char data[] = "QtFreadZero"; // 11 bytes
  const size_t len = sizeof(data) - 1;

  FILE* f = makeTempFileWithContent(data, len);
  if (!f) {
    std::cerr << "test_quota_fread_zero_length: failed to create temp file\n";
    return false;
  }

  quota_FILE q;
  q.f = f;

  char buf[32];
  memset(buf, 0xCC, sizeof(buf));

  // Case A: size=0
  size_t gotA = sqlite3_quota_fread(buf, 0, 10, &q);
  bool ok = true;
  if (gotA != 0) {
    std::cerr << "test_quota_fread_zero_length: size=0 case expected 0, got " << gotA << "\n";
    ok = false;
  }
  if (memcmp(buf, (const void*)buf, 0) != 0) {
    // No actual data to compare; this check is effectively a no-op to keep structure
  }

  // Case B: nmemb=0
  size_t gotB = sqlite3_quota_fread(buf, 1, 0, &q);
  if (gotB != 0) {
    std::cerr << "test_quota_fread_zero_length: nmemb=0 case expected 0, got " << gotB << "\n";
    ok = false;
  }

  // Ensure no modification occurred in buffer for both cases (all 0xCC)
  for (size_t i = 0; i < sizeof(buf); ++i) {
    if (buf[i] != (char)0xCC) {
      std::cerr << "test_quota_fread_zero_length: buffer modified at position " << i << "\n";
      ok = false;
      break;
    }
  }

  fclose(f);
  return ok;
}

// Test 4: Multi-byte read - size > 1, ensure correct handling of non-unit element sizes
static bool test_quota_fread_multi_byte() {
  const char data[] = "abcdefghij"; // 10 bytes
  const size_t len = sizeof(data) - 1;

  FILE* f = makeTempFileWithContent(data, len);
  if (!f) {
    std::cerr << "test_quota_fread_multi_byte: failed to create temp file\n";
    return false;
  }

  quota_FILE q;
  q.f = f;

  // Read 5 elements of size 2 = 10 bytes total
  char buf[64];
  memset(buf, 0xDD, sizeof(buf));

  size_t got = sqlite3_quota_fread(buf, 2, 5, &q); // total 10 bytes
  bool ok = true;
  if (got != 5) {
    std::cerr << "test_quota_fread_multi_byte: expected got=5, got=" << got << "\n";
    ok = false;
  }
  if (memcmp(buf, data, 10) != 0) {
    std::cerr << "test_quota_fread_multi_byte: buffer content mismatch for multi-byte read\n";
    ok = false;
  }

  fclose(f);
  return ok;
}

// Simple test runner
int main() {
  int total = 0;
  int passed = 0;

  struct { bool (*test)(void); const char* name; } tests[] = {
    { test_quota_fread_normal, "test_quota_fread_normal" },
    { test_quota_fread_eof_partial, "test_quota_fread_eof_partial" },
    { test_quota_fread_zero_length, "test_quota_fread_zero_length" },
    { test_quota_fread_multi_byte, "test_quota_fread_multi_byte" },
  };

  std::cout << "Running sqlite3_quota_fread test suite...\n";
  for (const auto& t : tests) {
    ++total;
    bool ok = t.test();
    if (ok) {
      ++passed;
      std::cout << "[PASS] " << t.name << "\n";
    } else {
      std::cout << "[FAIL] " << t.name << "\n";
    }
  }

  std::cout << "Tests passed: " << passed << " / " << total << "\n";
  return (passed == total) ? 0 : 1;
}