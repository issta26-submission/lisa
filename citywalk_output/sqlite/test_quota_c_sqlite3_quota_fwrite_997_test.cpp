// C++11 test suite for sqlite3_quota_fwrite (non-GTest, no GMock)
#include <string.h>
#include <vector>
#include <assert.h>
#include <cstdio>
#include <sqlite3.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <test_quota.h>
#include <os_setup.h>
#include <iostream>
#include <cstdlib>


// Include the quota-related types and the function under test.
// This header is expected to be provided by the project (as per the test_quota.c context).
extern "C" {
}

// Lightweight test framework (non-terminating assertions)
static int g_tests_run = 0;
static int g_tests_pass = 0;

#define EXPECT_TRUE(cond, msg) do { \
  ++g_tests_run; \
  if(cond) { ++g_tests_pass; } \
  else { std::cerr << "[FAILED] " << msg << " (line " << __LINE__ << ")\n"; } \
} while(0)

#define EXPECT_EQ(a, b, msg) do { \
  ++g_tests_run; \
  if((a) == (b)) { ++g_tests_pass; } \
  else { std::cerr << "[FAILED] " << msg << " | Expected: " << (b) << ", Found: " << (a) << " (line " << __LINE__ << ")\n"; } \
} while(0)

#define EXPECT_NEQ(a, b, msg) do { \
  ++g_tests_run; \
  if((a) != (b)) { ++g_tests_pass; } \
  else { std::cerr << "[FAILED] " << msg << " | Not Expected: " << (a) << " (line " << __LINE__ << ")\n"; } \
} while(0)

static void cleanup_file(const char* path) {
    if(path) {
        std::remove(path);
    }
}

// Mock quota callback to simulate quota limit adjustment
static void quotaCallbackMock(
    const char *zFilename,
    sqlite3_int64 *piLimit,
    sqlite3_int64 iSize,
    void *pArg
){
    (void)zFilename;
    (void)iSize;
    (void)pArg;
    // For test scenarios we artificially reduce the limit to exercise the inner branch
    if(piLimit) *piLimit = 50;
}

// Helper to create a unique temporary file path (POSIX-friendly)
static std::string make_temp_path() {
    // Using tmpnam to avoid platform-specific boilerplate in this test environment.
    // tmpnam may return a path in /tmp on POSIX systems.
    char tmpBuf[L_tmpnam] = {0};
    if(std::tmpnam(tmpBuf)) {
        return std::string(tmpBuf);
    }
    // Fallback
    std::string fallback = "./quota_tmp";
    return fallback;
}

// Test 1: When there is no quota_file associated (p->pFile == NULL), sqlite3_quota_fwrite should
// perform a normal fwrite without touching quota tracking.
static void test_no_quota_file() {
    // Prepare environment
    std::string path = make_temp_path();
    FILE *f = std::fopen(path.c_str(), "w+b");
    if(!f){
        std::cerr << "[ERROR] Failed to create temp file for test_no_quota_file\n";
        return;
    }

    quota_FILE q;
    // Ensure pFile is NULL to avoid quota tracking
    // Note: The actual struct layout is provided by test_quota.h; we assume member name pFile exists.
    std::memset(&q, 0, sizeof(q));
    q.f = f;
    q.pFile = NULL;

    // Prepare write buffer
    const char payload[] = "ABCD"; // 4 bytes
    size_t size = 1;
    size_t nmemb = sizeof(payload) - 1;
    size_t rc = sqlite3_quota_fwrite(payload, size, nmemb, &q);

    // Expect full write
    EXPECT_EQ(rc, nmemb, "test_no_quota_file: rc should equal nmemb");
    // Verify file length is 4
    std::fseek(f, 0, SEEK_END);
    long flen = std::ftell(f);
    EXPECT_EQ((size_t)flen, nmemb, "test_no_quota_file: file length should be 4");

    // Cleanup
    std::fclose(f);
    cleanup_file(path.c_str());

    (void)rc;
}

// Test 2: With pFile and iLimit == 0 (no quota limit). Expect quota tracking to adjust iSize correctly.
static void test_quota_no_limit_behaviour() {
    std::string path = make_temp_path();
    FILE *f = std::fopen(path.c_str(), "w+b");
    if(!f){
        std::cerr << "[ERROR] Failed to create temp file for test_quota_no_limit_behaviour\n";
        return;
    }

    // Set up quota structures
    quotaGroup g;
    g.iSize = 0;
    g.iLimit = 0; // no limit
    g.xCallback = NULL;
    g.pArg = NULL;
    g.zPattern = NULL; // not used in this test

    quotaFile fEntry;
    fEntry.iSize = 0;
    fEntry.pGroup = &g;
    fEntry.zFilename = "test_no_limit_file";

    quota_FILE q;
    std::memset(&q, 0, sizeof(q));
    q.f = f;
    q.pFile = &fEntry;

    // Write 4 bytes
    const char payload[] = "WXYZ";
    size_t rc = sqlite3_quota_fwrite(payload, 1, 4, &q);

    // Expect full write
    EXPECT_EQ(rc, (size_t)4, "test_quota_no_limit_behaviour: rc should be 4");
    // Group and file sizes should reflect the new total
    EXPECT_EQ(g.iSize, (sqlite3_int64)4, "test_quota_no_limit_behaviour: group iSize should be 4");
    EXPECT_EQ(fEntry.iSize, (sqlite3_int64)4, "test_quota_no_limit_behaviour: file iSize should be 4");

    // Cleanup
    std::fclose(f);
    cleanup_file(path.c_str());

    (void)path; (void)f; (void)fEntry;
}

// Test 3: With an active quota group and a callback, the callback adjusts the limit and triggers inner re-calculation.
// This exercises the callback invocation path and inner limit-based recalculation.
static void test_quota_with_callback_and_limit() {
    std::string path = make_temp_path();
    FILE *f = std::fopen(path.c_str(), "w+b");
    if(!f){
        std::cerr << "[ERROR] Failed to create temp file for test_quota_with_callback_and_limit\n";
        return;
    }

    // Setup group with initial values to trigger callback path
    sqlite3_int64 initialGroupSize = 60;
    sqlite3_int64 initialFileSize  = 10;

    quotaGroup g;
    g.iSize = initialGroupSize;
    g.iLimit = 80; // positive limit to trigger callback
    g.xCallback = quotaCallbackMock;
    g.pArg = NULL;
    g.zPattern = NULL;

    quotaFile fEntry;
    fEntry.iSize = initialFileSize;
    fEntry.pGroup = &g;
    fEntry.zFilename = "test_with_callback_file";

    quota_FILE q;
    std::memset(&q, 0, sizeof(q));
    q.f = f;
    q.pFile = &fEntry;

    // Prepare a large payload to trigger the calculation path
    const size_t payloadSize = 100;
    std::vector<char> payload(payloadSize, 'A'); // 100 bytes
    size_t rc = sqlite3_quota_fwrite(payload.data(), 1, payloadSize, &q);

    // Expect that the write attempted (rc likely equals payloadSize or until error); since file is writable, rc==payloadSize
    EXPECT_EQ(rc, payloadSize, "test_quota_with_callback_and_limit: rc should be payloadSize (100)");

    // After callback, group size should reflect recalculation (50 as per mock logic when limiter applied)
    EXPECT_EQ(g.iSize, (sqlite3_int64)50, "test_quota_with_callback_and_limit: group iSize should be 50 after limit adjustment");
    // File iSize updated to iEnd (0 in our calculation when the inner branch runs)
    EXPECT_EQ(fEntry.iSize, (sqlite3_int64)0, "test_quota_with_callback_and_limit: file iSize should be 0 after adjustment");

    // Verify the file actually contains payload (even if iSize changed, the file content should reflect the write)
    std::fseek(f, 0, SEEK_END);
    long written_len = std::ftell(f);
    EXPECT_EQ((long)written_len, (long)payloadSize, "test_quota_with_callback_and_limit: actual file length should be payloadSize (100)");

    // Cleanup
    std::fclose(f);
    cleanup_file(path.c_str());

    (void)initialGroupSize; (void)initialFileSize;
}

// Main runner
int main() {
    std::cout << "Starting sqlite3_quota_fwrite test suite (non-GTest, C++11)\n";

    test_no_quota_file();
    test_quota_no_limit_behaviour();
    test_quota_with_callback_and_limit();

    std::cout << "Tests completed: " << g_tests_pass << " / " << g_tests_run << " passed.\n";

    // Return non-zero if any test failed
    if(g_tests_pass != g_tests_run) {
        return 1;
    }
    return 0;
}