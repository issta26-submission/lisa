// Test suite for sqlite3_create_filename (C function) using a self-contained harness.
// This test avoids GTest and uses a small in-file framework as required.
// The tests are written for C++11 compatibility and rely only on the C stdlib and C++ standard library.

#include <vector>
#include <cstdio>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cassert>


// Domain-specific helpers and minimal stubs to enable testing of sqlite3_create_filename

// Minimal sqlite3_int64 typedef (as in SQLite)
typedef long long sqlite3_int64;

// Global flag to simulate malloc failure in tests
static bool g_failMalloc = false;

// Stubbed allocation function to mimic sqlite3_malloc64
char* sqlite3_malloc64(sqlite3_int64 n){
    if(g_failMalloc) return nullptr;
    return (char*)std::malloc((size_t)n);
}

// Helper to mimic the appendText behavior used by sqlite3_create_filename
// It copies the string z into location p without injecting a terminator.
char* appendText(char *p, const char *z){
    if(z==nullptr) return p;
    while(*z){
        *p = *z;
        ++p;
        ++z;
    }
    return p;
}

// The focal function under test (copied/adapted from the prompt, with sane syntax)
const char *sqlite3_create_filename(
  const char *zDatabase,
  const char *zJournal,
  const char *zWal,
  int nParam,
  const char **azParam
){
  sqlite3_int64 nByte;
  int i;
  char *pResult, *p;
  nByte = strlen(zDatabase) + strlen(zJournal) + strlen(zWal) + 10;
  for(i=0; i<nParam*2; i++){
    nByte += strlen(azParam[i])+1;
  }
  pResult = p = sqlite3_malloc64( nByte );
  if( p==0 ) return 0;
  memset(p, 0, 4);
  p += 4;
  p = appendText(p, zDatabase);
  for(i=0; i<nParam*2; i++){
    p = appendText(p, azParam[i]);
  }
  *(p++) = 0;
  p = appendText(p, zJournal);
  p = appendText(p, zWal);
  *(p++) = 0;
  *(p++) = 0;
  assert( (sqlite3_int64)(p - pResult)==nByte );
  return pResult + 4;
}

// --------------------------------------------------------------------------------------
// Minimal test framework (non-terminating assertions as requested)
// --------------------------------------------------------------------------------------

static int g_failures = 0;
static void logFailure(const std::string &msg){
    ++g_failures;
    std::cerr << "[FAIL] " << msg << "\n";
}
static void logInfo(const std::string &msg){
    std::cout << "[INFO] " << msg << "\n";
}

// Helper to compare a memory block with an expected sequence
static bool memeq(const unsigned char* a, const unsigned char* b, size_t len){
    return std::memcmp(a, b, len) == 0;
}

// Test 1: No azParam (nParam == 0)
static void test_no_param(){
    logInfo("Test 1: sqlite3_create_filename with nParam=0 (no azParam strings)");
    g_failures = 0;
    const char *db = "db";
    const char *journal = "journal";
    const char *wal = "wal";
    const char *ret = sqlite3_create_filename(db, journal, wal, 0, nullptr);
    if(ret == nullptr){
        logFailure("Return pointer was NULL for test_no_param");
        logFailure("Test 1 failed");
        return;
    }
    // Base pointer (start of the allocated buffer)
    const unsigned char* base = (const unsigned char*)(ret - 4);

    // 1) First 4 bytes should be zeros (as per implementation)
    bool ok = true;
    for(int i=0; i<4; ++i){
        if(base[i] != 0){
            logFailure("Test 1: Leading 4 bytes are not zeros at base[" + std::to_string(i) + "]");
            ok = false;
            break;
        }
    }

    // 2) Expected content after the 4 leading zeros:
    // base[4..] should start with zDatabase ("db"), followed by a single 0, then zJournal, zWal, then two zeros at the end.
    // We will reconstruct the expected sequence for comparison (up to the point where we know content ends for zDatabase+journal+wal with nParam=0).
    std::vector<unsigned char> expected;
    // 4 leading zeros
    for(int i=0;i<4;i++) expected.push_back(0);
    // zDatabase
    for(size_t i=0;i<strlen(db);i++) expected.push_back((unsigned char)db[i]);
    // terminator after azParam (none in this case): 0
    expected.push_back(0);
    // zJournal
    for(size_t i=0;i<strlen(journal);i++) expected.push_back((unsigned char)journal[i]);
    // zWal
    for(size_t i=0;i<strlen(wal);i++) expected.push_back((unsigned char)wal[i]);
    // two trailing zeros
    expected.push_back(0);
    expected.push_back(0);

    // Compare the prefix of base with expected
    if(base.size() < expected.size()){ /* can't access size since base is not a std::vector; recompute length from expected */ }
    if(!memeq(base, expected.data(), expected.size())){
        // If mismatch, report difference details
        logFailure("Test 1: Memory content after base[4] differs from expected sequence for no-Param case.");
        ok = false;
    }

    // Report result
    if(ok) logInfo("Test 1 passed");
    else logFailure("Test 1 failed");
}

// Test 2: One pair of azParam strings (nParam = 1, two strings)
static void test_one_param(){
    logInfo("Test 2: sqlite3_create_filename with nParam=1 (two azParam strings)");
    g_failures = 0;
    const char *db = "db2";
    const char *journal = "journal2";
    const char *wal = "wal2";
    const char *p0 = "paramA";
    const char *p1 = "paramB";
    const char* azParam[2] = { p0, p1 };
    const char *ret = sqlite3_create_filename(db, journal, wal, 1, azParam);
    if(ret == nullptr){
        logFailure("Return pointer was NULL for test_one_param");
        logFailure("Test 2 failed");
        return;
    }
    const unsigned char* base = (const unsigned char*)(ret - 4);

    // Build expected sequence for nParam=1:
    // 4 zeros, db2, 0, "paramA" + "paramB", 0, "journal2", "wal2", 0, 0
    std::vector<unsigned char> expected;
    for(int i=0;i<4;i++) expected.push_back(0);
    // zDatabase
    for(size_t i=0;i<strlen(db);i++) expected.push_back((unsigned char)db[i]);
    // concatenated azParam[0] and azParam[1]
    for(size_t i=0;i<strlen(p0);i++) expected.push_back((unsigned char)p0[i]);
    for(size_t i=0;i<strlen(p1);i++) expected.push_back((unsigned char)p1[i]);
    // 0 terminator after azParam
    expected.push_back(0);
    // zJournal
    for(size_t i=0;i<strlen(journal);i++) expected.push_back((unsigned char)journal[i]);
    // zWal
    for(size_t i=0;i<strlen(wal);i++) expected.push_back((unsigned char)wal[i]);
    // two trailing zeros
    expected.push_back(0);
    expected.push_back(0);

    if(base.size() < expected.size()){
        // The base is a C array; we don't know its size beyond what was allocated.
        // We still compare the prefix to ensure correctness up to expected length.
        // If the actual allocated size is shorter than expected, that's a failure.
        logFailure("Test 2: Unexpected shorter base buffer; cannot compare full expected sequence.");
        logFailure("Test 2 failed");
        return;
    }

    if(!memeq(base, expected.data(), expected.size())){
        logFailure("Test 2: Memory content after base[4] differs from expected sequence for one-param case.");
        logFailure("Test 2 failed");
        return;
    }

    logInfo("Test 2 passed");
}

// Test 3: Simulate malloc failure
static void test_malloc_failure(){
    logInfo("Test 3: sqlite3_create_filename should return NULL on allocation failure");
    g_failMalloc = true;
    const char *ret = sqlite3_create_filename("db", "journal", "wal", 0, nullptr);
    if(ret != nullptr){
        logFailure("Test 3: Expected NULL return on malloc failure, got non-NULL");
        g_failMalloc = false;
        logFailure("Test 3 failed");
        return;
    }
    g_failMalloc = false;
    logInfo("Test 3 passed");
}

// Test 4: Empty azParam strings (nParam > 0 but strings are empty)
static void test_empty_azparam_strings(){
    logInfo("Test 4: nParam=1 with two empty azParam strings");
    g_failMalloc = false;
    const char *db = "x";
    const char *journal = "y";
    const char *wal = "z";
    const char *p0 = "";
    const char *p1 = "";
    const char *azParam[2] = { p0, p1 };
    const char *ret = sqlite3_create_filename(db, journal, wal, 1, azParam);
    if(ret == nullptr){
        logFailure("Test 4: Return NULL when it should not be (empty azParam strings)");
        logFailure("Test 4 failed");
        return;
    }
    const unsigned char* base = (const unsigned char*)(ret - 4);

    // Build expected: 4 zeros, "x", 0, "" + "", 0, "y", "z", 0, 0
    // Note: empty strings contribute nothing to the concatenated azParam part.
    std::vector<unsigned char> expected;
    for(int i=0;i<4;i++) expected.push_back(0);
    for(size_t i=0;i<strlen(db);i++) expected.push_back((unsigned char)db[i]);
    // azParam: two empty strings - nothing added
    expected.push_back(0);
    for(size_t i=0;i<strlen(journal);i++) expected.push_back((unsigned char)journal[i]);
    for(size_t i=0;i<strlen(wal);i++) expected.push_back((unsigned char)wal[i]);
    expected.push_back(0);
    expected.push_back(0);

    if(base.size() < expected.size()){
        logFailure("Test 4: Unexpected shorter base buffer; cannot compare full expected sequence.");
        logFailure("Test 4 failed");
        return;
    }

    if(!memeq(base, expected.data(), expected.size())){
        logFailure("Test 4: Memory content mismatch for empty azParam strings case.");
        logFailure("Test 4 failed");
        return;
    }

    logInfo("Test 4 passed");
}

// Entry point
int main(){
    // Run all tests
    test_no_param();
    test_one_param();
    test_malloc_failure();
    test_empty_azparam_strings();

    if(g_failures == 0){
        std::cout << "[RESULT] All tests passed.\n";
        return 0;
    }else{
        std::cout << "[RESULT] Tests completed with " << g_failures << " failure(s).\n";
        return 1;
    }
}