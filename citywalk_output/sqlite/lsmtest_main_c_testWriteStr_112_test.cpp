// Unit test suite for the focal method testWriteStr
// This test is written for C++11 compilation (no GTest).
// It mocks the tdb_write symbol to capture arguments and verify
// that testWriteStr forwards correct values (nKey, nVal, key/val data).

#include <lsmtest.h>
#include <cstdio>
#include <sys/time.h>
#include <sqlite3.h>
#include <cstring>
#include <sys/resource.h>
#include <iostream>
#include <cstdlib>


// Forward declare the TestDb type to match the focal code.
// The real implementation is elsewhere; here we only need a placeholder.
struct TestDb;

// Forward declare the focal function under test.
// Signature exactly as provided in the focal method.
extern "C" void testWriteStr(TestDb *pDb, const char *zKey, const char *zVal, int *pRc);

// Provide a C linkage mock for tdb_write used by testWrite and testWriteStr.
// The real tests in the focal project call tdb_write(pDb, pKey, nKey, pVal, nVal).
// We implement a minimal mock that records the last written key/value for assertions.
extern "C" int tdb_write(TestDb *pDb, void *pKey, int nKey, void *pVal, int nVal);

// Dummy TestDb instance for tests.
static TestDb g_dummyDb;

// Global storage to verify the arguments passed to tdb_write by testWriteStr/testWrite
static int g_lastKeyLen = -1;
static unsigned char g_lastKeyBuf[128];
static int g_lastValLen = -1;
static unsigned char g_lastValBuf[256];
static TestDb *g_lastDbPtr = nullptr;

// Reset the mock state before each test
static void resetMocks() {
    g_lastKeyLen = -1;
    std::memset(g_lastKeyBuf, 0, sizeof(g_lastKeyBuf));
    g_lastValLen = -1;
    std::memset(g_lastValBuf, 0, sizeof(g_lastValBuf));
    g_lastDbPtr = nullptr;
}

// Minimal test framework (non-terminating assertions)
static int g_testFailures = 0;
#define EXPECT_TRUE(cond, msg) do { if(!(cond)) { std::cerr << "TEST FAIL: " << (msg) << "\n"; g_testFailures++; } } while(0)
#define EXPECT_EQ(a, b, msg) do { if((a)!=(b)) { std::cerr << "TEST FAIL: " << (msg) << " (expected " << (a) << " == " << (b) << ")\n"; g_testFailures++; } } while(0)
#define EXPECT_MEM_EQ(buf, ref, len, msg) do { if(std::memcmp((buf), (ref), (len)) != 0) { std::cerr << "TEST FAIL: " << (msg) << "\n"; g_testFailures++; } } while(0)

// Implement the tdb_write mock with C linkage so the focal code links correctly.
// It copies the input key/value data into the global buffers for inspection.
int tdb_write(TestDb *pDb, void *pKey, int nKey, void *pVal, int nVal) {
    // Record the database pointer used
    g_lastDbPtr = pDb;
    // Save key data (up to buffer size)
    int kCopy = nKey < (int)sizeof(g_lastKeyBuf) - 1 ? nKey : (int)sizeof(g_lastKeyBuf) - 1;
    if (kCopy > 0 && pKey) {
        std::memcpy(g_lastKeyBuf, pKey, kCopy);
    }
    g_lastKeyBuf[kCopy] = '\0';
    g_lastKeyLen = nKey;

    // Save value data
    int vCopy = nVal < (int)sizeof(g_lastValBuf) - 1 ? nVal : (int)sizeof(g_lastValBuf) - 1;
    if (vCopy > 0 && pVal) {
        std::memcpy(g_lastValBuf, pVal, vCopy);
    }
    g_lastValBuf[vCopy] = '\0';
    g_lastValLen = nVal;

    // Return success to avoid triggering error handling in test environment
    return 0;
}

// Helper to expose a simple TestDb handle to testWriteStr
static TestDb* makeTestDbHandle() {
    return &g_dummyDb;
}

// Test 1: zVal is non-null. Expect nVal == strlen(zVal) and key/value contents are forwarded correctly.
static void testWriteStr_nonNullVal() {
    resetMocks();
    TestDb *pDb = makeTestDbHandle();
    const char *zKey = "abc";
    const char *zVal = "hello";
    int rc = 0;

    testWriteStr(pDb, zKey, zVal, &rc);

    // rc should remain 0 (tdb_write returns 0)
    EXPECT_EQ(rc, 0, "rc should be 0 when tdb_write succeeds");

    // Key forwarded
    EXPECT_EQ(g_lastKeyLen, (int)strlen(zKey), "Key length should equal strlen(zKey)");
    EXPECT_MEM_EQ(g_lastKeyBuf, zKey, strlen(zKey), "Key data should match zKey");

    // Value forwarded
    EXPECT_EQ(g_lastValLen, (int)strlen(zVal), "Value length should equal strlen(zVal)");
    EXPECT_MEM_EQ(g_lastValBuf, zVal, strlen(zVal), "Value data should match zVal");

    // Db pointer forwarded
    EXPECT_TRUE(g_lastDbPtr == pDb, "tdb_write should receive the same TestDb*");
}

// Test 2: zVal is NULL. Expect nVal == 0 and value data not forwarded (empty buffer).
static void testWriteStr_nullVal() {
    resetMocks();
    TestDb *pDb = makeTestDbHandle();
    const char *zKey = "key";
    const char *zVal = nullptr;
    int rc = 0;

    testWriteStr(pDb, zKey, zVal, &rc);

    EXPECT_EQ(rc, 0, "rc should be 0 when tdb_write succeeds (null val)");
    EXPECT_EQ(g_lastKeyLen, (int)strlen(zKey), "Key length should equal strlen(zKey)");
    EXPECT_MEM_EQ(g_lastKeyBuf, zKey, strlen(zKey), "Key data should match zKey");

    // nVal should be 0 and no meaningful value copied
    EXPECT_EQ(g_lastValLen, 0, "Value length should be 0 for NULL zVal");
    // We expect an empty value buffer content after 0-length copy
    if (g_lastValLen == 0) {
        // No further bytes to compare; just confirm empty string representation
        EXPECT_TRUE(g_lastValBuf[0] == '\0', "Value buffer should be empty for zero-length value");
    }

    EXPECT_TRUE(g_lastDbPtr == pDb, "tdb_write should receive the same TestDb*");
}

// Test 3: zKey is empty string. Ensure keys with length 0 are handled gracefully.
static void testWriteStr_emptyKey() {
    resetMocks();
    TestDb *pDb = makeTestDbHandle();
    const char *zKey = "";
    const char *zVal = "val";
    int rc = 0;

    testWriteStr(pDb, zKey, zVal, &rc);

    EXPECT_EQ(rc, 0, "rc should be 0 for empty key with valid value");
    EXPECT_EQ(g_lastKeyLen, (int)strlen(zKey), "Key length should be 0 for empty key");
    // Since length 0, the stored key should be an empty string
    EXPECT_TRUE(g_lastKeyBuf[0] == '\0', "Stored key should be empty when zKey is empty");

    EXPECT_EQ(g_lastValLen, (int)strlen(zVal), "Value length should equal strlen(zVal)");
    EXPECT_MEM_EQ(g_lastValBuf, zVal, strlen(zVal), "Value data should match zVal for non-empty value");
    EXPECT_TRUE(g_lastDbPtr == pDb, "tdb_write should receive the same TestDb*");
}

// Forward declare the focal function for compilation in C++ environment.
// The real implementation exists in the C file; we just declare it for linking.
extern "C" void testWrite(TestDb *pDb, void *pKey, int nKey, void *pVal, int nVal, int *pRc) __attribute__((unused));

// Since testWriteStr is defined in the provided focal code and uses testWrite,
// we provide a compatible extern declaration here to ensure linkage compatibility.
extern "C" void testWriteStr(TestDb *pDb, const char *zKey, const char *zVal, int *pRc);

// Test runner
static void run_all_tests() {
    testWriteStr_nonNullVal();
    testWriteStr_nullVal();
    testWriteStr_emptyKey();
}

// Simple summary
static void summarize_results() {
    if(g_testFailures){
        std::cerr << "Total test failures: " << g_testFailures << "\n";
    }else{
        std::cout << "All tests passed.\n";
    }
}

int main() {
    // Run tests and report
    run_all_tests();
    summarize_results();
    return g_testFailures;
}