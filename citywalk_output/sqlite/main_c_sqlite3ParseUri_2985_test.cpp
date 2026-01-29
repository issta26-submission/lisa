/*
  Lightweight C++11 test harness for the focal method:
  int sqlite3ParseUri(const char *zDefaultVfs, const char *zUri, unsigned int *pFlags,
                      sqlite3_vfs **ppVfs, char **pzFile, char **pzErrMsg);

  This test suite does not rely on Google Test. It provides minimal C
  runtimes (stubs) for the dependencies used by sqlite3ParseUri so the
  function can be exercised in isolation. It includes a small, non-terminating
  assertion framework and a few targeted test cases that cover key code paths
  in sqlite3ParseUri (true/false branches of critical predicates).
  Each test is commented to explain intent and expected outcomes.
*/

#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <sqliteInt.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <cassert>


// ---------------------------
// Minimal C/runtime stubs for sqlite3ParseUri dependencies
// ---------------------------

extern "C" {

// Forward declaration of the focal function (as it would appear in C)
typedef struct sqlite3_vfs sqlite3_vfs;
int sqlite3ParseUri(
  const char *zDefaultVfs,
  const char *zUri,
  unsigned int *pFlags,
  sqlite3_vfs **ppVfs,
  char **pzFile,
  char **pzErrMsg
);

// A tiny fake sqlite3_vfs structure (contents are irrelevant for tests)
struct sqlite3_vfs {
  int dummy;
};

// A minimal global config to satisfy AtomicLoad usage in sqlite3ParseUri
struct sqlite3GlobalConfig_t { unsigned int bOpenUri; };
static struct sqlite3GlobalConfig_t sqlite3GlobalConfig = {0};

// AtomicLoad stub used by sqlite3ParseUri
extern "C" unsigned int AtomicLoad(const unsigned int *p){
  return (p ? *p : 0);
}

// A small dummy VFS registry
static sqlite3_vfs dummyVfs = {0};

// Fake implementation of sqlite3_vfs_find used by sqlite3ParseUri
extern "C" sqlite3_vfs* sqlite3_vfs_find(const char *zVfs){
  // Accept a known VFS name
  if( zVfs==0 || strcmp(zVfs, "defaultvfs")==0 || strcmp(zVfs, "memvfs")==0 ){
    return &dummyVfs;
  }
  // Unknown VFS -> not found
  return 0;
}

// Memory allocation helpers (minimal)
extern "C" void* sqlite3_malloc64(size_t n){
  return malloc(n);
}
extern "C" void sqlite3_free_filename(const char *p){
  if(p) free((void*)p);
}
extern "C" void* sqlite3_realloc(void* p, size_t n){
  return realloc(p, n);
}

// Simple string formatting stub (very small subset of sqlite3_mprintf)
extern "C" char* sqlite3_mprintf(const char *fmt, ...){
  va_list ap;
  va_start(ap, fmt);
  // First, try to format into a reasonably sized buffer
  char buf[512];
  int n = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);
  if(n < 0) n = 0;
  if((size_t)n < sizeof(buf)){
    char* out = (char*)malloc((size_t)n + 1);
    if(out){
      memcpy(out, buf, n);
      out[n] = '\0';
    }
    return out;
  }
  // Fallback: allocate exact size
  va_start(ap, fmt);
  int needed = vsnprintf(nullptr, 0, fmt, ap);
  va_end(ap);
  if(needed < 0) needed = 0;
  char* out = (char*)malloc((size_t)needed + 1);
  if(!out) return nullptr;
  va_start(ap, fmt);
  vsnprintf(out, (size_t)needed + 1, fmt, ap);
  va_end(ap);
  return out;
}

// String helpers
extern "C" int sqlite3Strlen30(const char* z){
  if(!z) return 0;
  size_t n = strlen(z);
  return (n > 30) ? 30 : (int)n;
}
extern "C" int sqlite3HexToInt(char c){
  if(c>='0' && c<='9') return c - '0';
  if(c>='a' && c<='f') return 10 + (c - 'a');
  if(c>='A' && c<='F') return 10 + (c - 'A');
  return 0;
}
extern "C" int sqlite3Isxdigit(int c){
  return (c>='0' && c<='9') || (c>='a' && c<='f') || (c>='A' && c<='F');
}

// Simple memory allocator wrappers for the test code
extern "C" void* sqlite3_malloc(size_t n){ return malloc(n); }
extern "C" void* sqlite3_realloc(void* p, size_t n); // provided above
extern "C" void sqlite3_free(void* p){ free(p); }

// A tiny representation of sqlite3_open/open_v2 flags constants (subset)
#define SQLITE_OK 0
#define SQLITE_ERROR 1
#define SQLITE_PERM 3
#define SQLITE_NOMEM_BKPT 7

#define SQLITE_OPEN_READONLY  0x00000001
#define SQLITE_OPEN_READWRITE 0x00000002
#define SQLITE_OPEN_CREATE    0x00000004
#define SQLITE_OPEN_MEMORY    0x00000008
#define SQLITE_OPEN_URI       0x00000040
#define SQLITE_OPEN_SHAREDCACHE 0x00000020
#define SQLITE_OPEN_PRIVATECACHE 0x00000080

};

// Convenience wrappers for C++ tests (namespace not required; using extern "C" above)

/* Test harness: lightweight non-terminating assertions */
static int g_tests_total = 0;
static int g_tests_passed = 0;

#define TEST_ASSERT(cond, msg) \
  do { \
    ++g_tests_total; \
    if(!(cond)){ \
      std::cerr << "TEST FAIL: " << (msg) << "\n"; \
    } else { \
      ++g_tests_passed; \
    } \
  } while(0)

static void test_non_file_uri_no_file(){
  // Test: Non-file URI (no "file:" prefix) should copy URI as filename
  const char* zDefaultVfs = "defaultvfs";
  const char* zUri = "memdb.db"; // not starting with "file:"
  unsigned int flags = 0;
  sqlite3_vfs* pVfs = nullptr;
  char* zFile = nullptr;
  char* zErr = 0;

  int rc = sqlite3ParseUri(zDefaultVfs, zUri, &flags, &pVfs, &zFile, &zErr);

  TEST_ASSERT(rc == SQLITE_OK, "RC should be SQLITE_OK for non-file URI");
  TEST_ASSERT(zErr == nullptr, "No error message for successful parse");
  TEST_ASSERT(zFile != nullptr, "zFile should be allocated");
  if(zFile){
    // The filename should be the input URI
    TEST_ASSERT(std::strcmp(zFile, "memdb.db") == 0, "Filename should match input URI");
    // Cleanup
    // sqlite3_free_filename is provided for freeing; use it
    sqlite3_free_filename(zFile);
  }
  // VFS should be resolved (our stub recognizes defaultvfs)
  TEST_ASSERT(pVfs != nullptr, "VFS should be found for defaultvfs");
  // ErrMsg should be untouched
  if(zErr) {
    std::free(zErr);
  }
}

static void test_file_uri_with_options(){
  // Test: Proper file URI with options including vfs, mode, and cache
  const char* zDefaultVfs = "defaultvfs";
  const char* zUri = "file:/tmp/test.db?mode=rw&cache=shared&vfs=defaultvfs";
  unsigned int flags = 0;
  sqlite3_vfs* pVfs = nullptr;
  char* zFile = nullptr;
  char* zErr = 0;

  int rc = sqlite3ParseUri(zDefaultVfs, zUri, &flags, &pVfs, &zFile, &zErr);

  TEST_ASSERT(rc == SQLITE_OK, "RC should be SQLITE_OK for valid file URI");
  TEST_ASSERT(zErr == nullptr, "No error message for successful parse");
  TEST_ASSERT(zFile != nullptr, "zFile should be allocated");

  if(zFile){
    // Check path part
    const char* path = zFile;
    TEST_ASSERT(std::strncmp(path, "/tmp/test.db", 12) == 0, "Path should be decoded as /tmp/test.db");
    // Check that options existed after path (presence of mode=rw and cache=shared)
    // We use substring search to verify that option strings are present in the buffer.
    std::string blob(zFile);
    TEST_ASSERT(blob.find("mode=rw") != std::string::npos, "Options should contain mode=rw");
    TEST_ASSERT(blob.find("cache=shared") != std::string::npos, "Options should contain cache=shared");
    // vfs should be applied (defaultvfs)
    TEST_ASSERT(pVfs != nullptr, "VFS should be resolved for vfs option");
    // Cleanup
    sqlite3_free_filename(zFile);
  }
  if(zErr) std::free(zErr);
}

static void test_invalid_authority_uri(){
  // Test: URIs with authority in file:// form should trigger invalid authority error
  const char* zDefaultVfs = "defaultvfs";
  const char* zUri = "file://host/path/to/db";
  unsigned int flags = 0;
  sqlite3_vfs* pVfs = nullptr;
  char* zFile = nullptr;
  char* zErr = 0;

  int rc = sqlite3ParseUri(zDefaultVfs, zUri, &flags, &pVfs, &zFile, &zErr);

  TEST_ASSERT(rc == SQLITE_ERROR, "RC should be SQLITE_ERROR for invalid authority");
  TEST_ASSERT(zErr != nullptr && strlen(zErr) > 0, "Error message should be set");
  if(zErr){
    // Expect that the error message mentions invalid uri authority
    std::string err(zErr);
    TEST_ASSERT(err.find("invalid uri authority") != std::string::npos,
                "Error message should mention invalid uri authority");
    std::free(zErr);
  }
  // Expect zFile to be freed on error
  if(zFile){
    sqlite3_free_filename(zFile);
  }
}

static void test_no_such_vfs_error(){
  // Test: Requesting a non-existent VFS via vfs=...
  const char* zDefaultVfs = "defaultvfs";
  const char* zUri = "file:/tmp/doesnotmatter?vfs=notexists";
  unsigned int flags = 0;
  sqlite3_vfs* pVfs = nullptr;
  char* zFile = nullptr;
  char* zErr = 0;

  int rc = sqlite3ParseUri(zDefaultVfs, zUri, &flags, &pVfs, &zFile, &zErr);

  TEST_ASSERT(rc == SQLITE_ERROR, "RC should be SQLITE_ERROR when VFS not found");
  if(zErr){
    std::string msg(zErr);
    TEST_ASSERT(msg.find("no such vfs") != std::string::npos,
                "Error message should indicate missing VFS");
    std::free(zErr);
  }
  // zFile should be freed on error
  if(zFile){
    sqlite3_free_filename(zFile);
  }
}

static int run_all_tests(){
  test_non_file_uri_no_file();
  test_file_uri_with_options();
  test_invalid_authority_uri();
  test_no_such_vfs_error();
  return g_tests_total;
}

int main(){
  int total = run_all_tests();
  std::cout << "Tests run: " << g_tests_total << ", Passed: " << g_tests_passed << "\n";
  if(g_tests_passed != g_tests_total){
    std::cerr << "Some tests FAILED.\n";
  } else {
    std::cout << "All tests PASSED.\n";
  }
  return (g_tests_total == g_tests_passed) ? 0 : 1;
}