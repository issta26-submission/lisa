// Minimal C++11 unit test harness for sqlite3BtreePayloadChecked
// This test isolates the focal function by providing lightweight stubs
// for its dependent components (BtCursor, cursor ownership check, payload access).
// The tests cover true/false branches of the key predicate and validate
// that the correct helper is invoked in each case.

#include <btreeInt.h>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <cassert>


// Domain types (subset for testing)
using u32 = uint32_t;

// Focal class/state definitions (simplified)
enum { CURSOR_INVALID = 0, CURSOR_VALID = 1 };

struct BtCursor {
    int eState;
};

// Static/global test state to mock dependencies
static bool g_cursorOwnsBtShared = false;

// Track calls to accessPayload (when CURSOR_VALID)
static int g_lastAccessPayloadCalled = 0;
static uint32_t g_lastOffset = 0, g_lastAmt = 0;
static void *g_lastPayloadBuf = nullptr;
static int g_lastAccessPayload_eOp = -1;

// Track calls to accessPayloadChecked (when not CURSOR_VALID)
static int g_lastAccessPayloadCheckedCalled = 0;
static uint32_t g_lastOffset2 = 0, g_lastAmt2 = 0;
static void *g_lastPayloadBuf2 = nullptr;
static int g_lastAccessPayloadChecked_ret = 0;

// Dependency: cursorOwnsBtShared(pCur)
// In real code this would be a static or complex check; here we mock it.
bool cursorOwnsBtShared(BtCursor* pCur) {
    (void)pCur;
    return g_cursorOwnsBtShared;
}

// Dependency: accessPayload(pCur, offset, amt, pBuf, eOp)
// Mock to emulate payload access and to verify arguments
int accessPayload(BtCursor *pCur, uint32_t offset, uint32_t amt, unsigned char *pBuf, int eOp) {
    (void)pCur;
    g_lastAccessPayloadCalled = 1;
    g_lastOffset = offset;
    g_lastAmt = amt;
    g_lastPayloadBuf = (void*)pBuf;
    g_lastAccessPayload_eOp = eOp;

    // Simulate payload write to buffer for verification
    if (pBuf != nullptr && amt > 0) {
        size_t w = (amt < 16) ? amt : 16;
        for (size_t i = 0; i < w; ++i) {
            pBuf[i] = 0xAB;
        }
    }
    return 42; // arbitrary payload return code for testing
}

// Dependency: accessPayloadChecked(pCur, offset, amt, pBuf)
// Mock to emulate checked payload access
int accessPayloadChecked(BtCursor *pCur, uint32_t offset, uint32_t amt, void *pBuf) {
    (void)pCur;
    g_lastAccessPayloadCheckedCalled = 1;
    g_lastOffset2 = offset;
    g_lastAmt2 = amt;
    g_lastPayloadBuf2 = pBuf;
    return 99; // arbitrary return code for testing
}

// The focal method under test (reproduced in this test harness for isolation)
int sqlite3BtreePayloadChecked(BtCursor *pCur, u32 offset, u32 amt, void *pBuf){
    if( pCur->eState==CURSOR_VALID ){
        assert( cursorOwnsBtShared(pCur) );
        return accessPayload(pCur, offset, amt, (unsigned char*)pBuf, 0);
    }else{
        return accessPayloadChecked(pCur, offset, amt, pBuf);
    }
}

// Lightweight test framework (non-terminating assertions)
static int g_test_passed = 0;
static int g_test_total  = 0;
#define TEST_OK(cond, name) \
    do { \
        g_test_total++; \
        if(cond) { g_test_passed++; } \
        else { std::cerr << "Test failed: " << name << " (at " << __FILE__ << ":" << __LINE__ << ")\n"; } \
    } while(0)

// Test 1: When pCur->eState == CURSOR_VALID and cursorOwnsBtShared(pCur) is true,
// the function should call accessPayload and return its value.
// This covers the true branch of the predicate and validates interaction.
void test_true_branch_calls_accessPayload() {
    BtCursor cur;
    cur.eState = CURSOR_VALID;

    g_cursorOwnsBtShared = true;      // ensure assertion passes
    g_lastAccessPayloadCalled = 0;

    unsigned char buf[16] = {0};
    int ret = sqlite3BtreePayloadChecked(&cur, 5, 3, buf);

    // Validate return value from accessPayload and that it was invoked with correct args
    TEST_OK(ret == 42, "return value from accessPayload is propagated");
    TEST_OK(g_lastAccessPayloadCalled == 1, "accessPayload was called exactly once");
    TEST_OK(g_lastOffset == 5, "offset passed to accessPayload is correct");
    TEST_OK(g_lastAmt == 3, "amt passed to accessPayload is correct");
    TEST_OK(g_lastPayloadBuf == buf, "buffer pointer passed to accessPayload is correct");
    TEST_OK(g_lastAccessPayload_eOp == 0, "eOp passed to accessPayload is correct");

    // Optional: verify that the buffer was written as part of the mock
    TEST_OK(buf[0] == 0xAB && buf[1] == 0xAB && buf[2] == 0xAB, "buffer content written by accessPayload mock");
}

// Test 2: When pCur->eState != CURSOR_VALID, the function should call accessPayloadChecked
// and return its value (true branch of the else). Also ensure accessPayload is not invoked.
void test_false_branch_calls_accessPayloadChecked() {
    BtCursor cur;
    cur.eState = CURSOR_INVALID;

    g_cursorOwnsBtShared = false; // irrelevant in this branch
    g_lastAccessPayloadCalled = 0;
    g_lastAccessPayloadCheckedCalled = 0;
    unsigned char buf2[8] = {0};

    int ret = sqlite3BtreePayloadChecked(&cur, 7, 4, buf2);

    // Validate that accessPayloadChecked was called and accessPayload was not
    TEST_OK(ret == 99, "return value from accessPayloadChecked is propagated");
    TEST_OK(g_lastAccessPayloadCalled == 0, "accessPayload was not called in else branch");
    TEST_OK(g_lastAccessPayloadCheckedCalled == 1, "accessPayloadChecked was called exactly once");
    TEST_OK(g_lastOffset2 == 7, "offset passed to accessPayloadChecked is correct");
    TEST_OK(g_lastAmt2 == 4, "amt passed to accessPayloadChecked is correct");
    TEST_OK(g_lastPayloadBuf2 == buf2, "buffer pointer passed to accessPayloadChecked is correct");
}

int main() {
    // Run tests
    test_true_branch_calls_accessPayload();
    test_false_branch_calls_accessPayloadChecked();

    std::cout << "Tests run: " << g_test_total << ", Passed: " << g_test_passed
              << ", Failed: " << (g_test_total - g_test_passed) << std::endl;

    // Return non-zero if any test failed
    return (g_test_total == g_test_passed) ? 0 : 1;
}