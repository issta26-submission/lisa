// File: mock_zip4_64.h
#ifndef MOCK_ZIP4_64_H
#define MOCK_ZIP4_64_H

#include <cstring>
#include <string.h>
#include <time.h>
#include <mock_zip4_64.h>
#include <stdio.h>
#include <stdint.h>
#include <crypt.h>
#include <iostream>
#include <zlib.h>
#include <stdlib.h>
#include <zip.h>


#ifdef __cplusplus
extern "C" {
#endif

// Snapshot of the parameters passed to zipOpenNewFileInZip4_64
typedef struct {
  void* file;
  const char* filename;
  const zip_fileinfo* zipfi;
  const void* extrafield_local;
  unsigned int size_extrafield_local;
  const void* extrafield_global;
  unsigned int size_extrafield_global;
  const char* comment;
  int method;
  int level;
  int raw;
  int windowBits;
  int memLevel;
  int strategy;
  const char* password;
  unsigned long crcForCrypting;
  unsigned long versionMadeBy;
  unsigned long flagBase;
  int zip64;
} Zip4_64_Call;

// Retrieve the last captured call to zipOpenNewFileInZip4_64
extern const Zip4_64_Call* zipOpenNewFileInZip4_64_mock_last_call(void);

// Reset the captured call state
extern void zipOpenNewFileInZip4_64_mock_reset(void);

#ifdef __cplusplus
}
#endif

#endif // MOCK_ZIP4_64_H

// File: mock_zip4_64.c

static Zip4_64_Call g_last_call;
static int g_has_last = 0;

extern "C" {

int zipOpenNewFileInZip4_64(zipFile file, const char* filename, const zip_fileinfo* zipfi,
                          const void* extrafield_local, uInt size_extrafield_local,
                          const void* extrafield_global, uInt size_extrafield_global,
                          const char* comment, int method, int level, int raw,
                          int windowBits, int memLevel, int strategy,
                          const char* password, uLong crcForCrypting,
                          uLong versionMadeBy, uLong flagBase, int zip64)
{
  // Capture all parameters for verification by tests
  g_last_call.file = (void*)file;
  g_last_call.filename = filename;
  g_last_call.zipfi = zipfi;
  g_last_call.extrafield_local = extrafield_local;
  g_last_call.size_extrafield_local = size_extrafield_local;
  g_last_call.extrafield_global = extrafield_global;
  g_last_call.size_extrafield_global = size_extrafield_global;
  g_last_call.comment = comment;
  g_last_call.method = method;
  g_last_call.level = level;
  g_last_call.raw = raw;
  g_last_call.windowBits = windowBits;
  g_last_call.memLevel = memLevel;
  g_last_call.strategy = strategy;
  g_last_call.password = password;
  g_last_call.crcForCrypting = crcForCrypting;
  g_last_call.versionMadeBy = versionMadeBy;
  g_last_call.flagBase = flagBase;
  g_last_call.zip64 = zip64;

  g_has_last = 1;
  return 12345; // sentinel return value to verify forwarding
}

const Zip4_64_Call* zipOpenNewFileInZip4_64_mock_last_call(void) {
  if(!g_has_last) return NULL;
  return &g_last_call;
}

void zipOpenNewFileInZip4_64_mock_reset(void) {
  memset(&g_last_call, 0, sizeof(g_last_call));
  g_has_last = 0;
}

} // extern "C"

// File: test_zip_open3.cpp

extern "C" {
}

// Simple non-terminating test framework
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define ASSERT(cond, msg) do { \
    if(!(cond)) { \
        std::cerr << "Test FAILED: " << (msg) << " (line " << __LINE__ << ")" << std::endl; \
        ++g_tests_failed; \
    } else { \
        ++g_tests_passed; \
    } \
} while(0)

using namespace std;

// Test 1: Basic forwarding of all parameters from zipOpenNewFileInZip3 to zipOpenNewFileInZip4_64
void test_basic_forwarding() {
    // Reset mock state
    zipOpenNewFileInZip4_64_mock_reset();

    zipFile file = NULL; // use NULL as zipFile handle
    zip_fileinfo zipfi;
    std::memset(&zipfi, 0, sizeof(zipfi));

    const char* filename = "test.txt";
    const void* extrafield_local = NULL;
    uInt size_extrafield_local = 0;
    const void* extrafield_global = NULL;
    uInt size_extrafield_global = 0;
    const char* comment = "comment";
    int method = 9;
    int level = 5;
    int raw = 0;
    int windowBits = 15;
    int memLevel = 8;
    int strategy = 0;
    const char* password = "pwd";
    uLong crcForCrypting = 0x1234;

    int ret = zipOpenNewFileInZip3(file, filename, &zipfi,
                                   extrafield_local, size_extrafield_local,
                                   extrafield_global, size_extrafield_global,
                                   comment, method, level, raw,
                                   windowBits, memLevel, strategy,
                                   password, crcForCrypting);

    ASSERT(ret == 12345, "zipOpenNewFileInZip3 should return mock value");

    const Zip4_64_Call* last = zipOpenNewFileInZip4_64_mock_last_call();
    ASSERT(last != NULL, "zipOpenNewFileInZip4_64 should be invoked");

    if (last) {
        ASSERT((void*)last->file == (void*)file, "Forwarded file should match input");
        ASSERT(strcmp(last->filename, filename) == 0, "Forwarded filename should match");
        ASSERT(last->zipfi == &zipfi, "Forwarded zipfi should match");
        ASSERT(last->extrafield_local == extrafield_local, "Forwarded extrafield_local should match");
        ASSERT(last->size_extrafield_local == size_extrafield_local, "Forwarded size_extrafield_local should match");
        ASSERT(last->extrafield_global == extrafield_global, "Forwarded extrafield_global should match");
        ASSERT(last->size_extrafield_global == size_extrafield_global, "Forwarded size_extrafield_global should match");
        ASSERT(strcmp(last->comment, comment) == 0, "Forwarded comment should match");
        ASSERT(last->method == method, "Forwarded method should match");
        ASSERT(last->level == level, "Forwarded level should match");
        ASSERT(last->raw == raw, "Forwarded raw should match");
        ASSERT(last->windowBits == windowBits, "Forwarded windowBits should match");
        ASSERT(last->memLevel == memLevel, "Forwarded memLevel should match");
        ASSERT(last->strategy == strategy, "Forwarded strategy should match");
        ASSERT(strcmp(last->password, password) == 0, "Forwarded password should match");
        ASSERT(last->crcForCrypting == crcForCrypting, "Forwarded crcForCrypting should match");
        ASSERT(last->versionMadeBy == VERSIONMADEBY, "versionMadeBy should be VERSIONMADEBY");
        ASSERT(last->flagBase == 0, "flagBase should be 0");
        ASSERT(last->zip64 == 0, "zip64 should be 0");
    }
}

// Test 2: Forwarding with non-null extrafield_local/global to verify pointers and sizes are passed through
void test_extrafield_forwarding() {
    zipOpenNewFileInZip4_64_mock_reset();

    zipFile file = NULL;
    zip_fileinfo zipfi;
    std::memset(&zipfi, 0, sizeof(zipfi));

    const char* filename = "foo.zip";
    const unsigned char local[4] = {1, 2, 3, 4};
    const void* extrafield_local = local;
    uInt size_extrafield_local = 4;
    const unsigned char global[2] = {9, 9};
    const void* extrafield_global = global;
    uInt size_extrafield_global = 2;
    const char* comment = "cmnt";
    int method = 1;
    int level = 2;
    int raw = 0;
    int windowBits = 9;
    int memLevel = 6;
    int strategy = 7;
    const char* password = NULL;
    uLong crcForCrypting = 0;

    int ret = zipOpenNewFileInZip3(file, filename, &zipfi,
                                   extrafield_local, size_extrafield_local,
                                   extrafield_global, size_extrafield_global,
                                   comment, method, level, raw,
                                   windowBits, memLevel, strategy,
                                   password, crcForCrypting);

    ASSERT(ret == 12345, "zipOpenNewFileInZip3 should return mock value for extrafields test");

    const Zip4_64_Call* last = zipOpenNewFileInZip4_64_mock_last_call();
    ASSERT(last != NULL, "zipOpenNewFileInZip4_64 should be invoked for extrafields test");

    if (last) {
        ASSERT(last->extrafield_local == extrafield_local, "extrafield_local should be forwarded");
        ASSERT(last->size_extrafield_local == size_extrafield_local, "size_extrafield_local should be forwarded");
        ASSERT(last->extrafield_global == extrafield_global, "extrafield_global should be forwarded");
        ASSERT(last->size_extrafield_global == size_extrafield_global, "size_extrafield_global should be forwarded");
        ASSERT(strcmp(last->comment, comment) == 0, "comment should match");
        ASSERT(strcmp(last->filename, filename) == 0, "filename should match");
        ASSERT(last->password == password, "password pointer should be forwarded (NULL)");
    }
}

// Run all tests
int main() {
    std::cout << "Running tests for zipOpenNewFileInZip3 forwarding..." << std::endl;

    test_basic_forwarding();
    test_extrafield_forwarding();

    std::cout << "Tests passed: " << g_tests_passed << std::endl;
    std::cout << "Tests failed: " << g_tests_failed << std::endl;
    return g_tests_failed ? 1 : 0;
}