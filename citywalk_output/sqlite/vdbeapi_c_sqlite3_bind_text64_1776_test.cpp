// Lightweight C++11 test harness for the focal method sqlite3_bind_text64
// Note: This is a self-contained mock-like test to validate the transformation
// logic inside sqlite3_bind_text64 as described in the provided source snippet.
// It is not a GTest-based test; it uses a minimal, non-terminating assertion style
// and a separate test for the static/abort behavior (via signal handling).

#include <csetjmp>
#include <csignal>
#include <opcodes.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <cassert>


// Domain-like constants and types (mocked for this test harness)
typedef unsigned long long sqlite3_uint64;
typedef unsigned short u16;
typedef struct sqlite3_stmt sqlite3_stmt;
typedef void (*DelFn)(void*);

// Mocked constants to emulate sqlite3_utf encodings and dynamic
#define SQLITE_UTF8 1
#define SQLITE_UTF16 2
#define SQLITE_UTF16NATIVE 3
#define SQLITE_DYNAMIC ((DelFn)0) // Use NULL-like sentinel to simulate dynamic deleter in tests

// Global capture structure to observe parameters passed to the internal mock bindText
struct BindCall {
    void* pStmt;
    int i;
    const char* zData;
    sqlite3_uint64 nData;
    DelFn xDel;
    unsigned char enc;
};

static BindCall g_lastCall;
static bool g_callRegistered = false;

// Reset the capture before each test
static void reset_capture() {
    g_lastCall = { nullptr, 0, nullptr, 0, nullptr, 0 };
    g_callRegistered = false;
}

// Mock implementation of the internal bindText that sqlite3_bind_text64 would delegate to.
// This mock records all input parameters forwarded by the focal function after its
// internal pre-processing (encoding remapping and nData masking).
static int mock_bindText(
    sqlite3_stmt* pStmt,
    int i,
    const void* zData,
    sqlite3_uint64 nData,
    DelFn xDel,
    unsigned char enc
) {
    g_lastCall.pStmt = (void*)pStmt;
    g_lastCall.i = i;
    g_lastCall.zData = (const char*)zData;
    g_lastCall.nData = nData;
    g_lastCall.xDel = xDel;
    g_lastCall.enc = enc;
    g_callRegistered = true;
    // Return a harmless success code for testing purposes
    return 0;
}

// Reproduction of the focal method's logic for testing (stand-alone in this harness)
extern "C" int sqlite3_bind_text64(
    sqlite3_stmt *pStmt,
    int i,
    const char *zData,
    sqlite3_uint64 nData,
    void (*xDel)(void*),
    unsigned char enc
) {
    // The focal method asserts that xDel != SQLITE_DYNAMIC
    assert( xDel != SQLITE_DYNAMIC );

    // If encoding is not UTF8, perform specific remappings and nibble-masking on nData
    if( enc != SQLITE_UTF8 ){
        if( enc == SQLITE_UTF16 ) enc = SQLITE_UTF16NATIVE;
        nData &= ~(u16)1; // mask off the least significant bit of the low 16 bits
    }

    // Forward to the internal (mocked) bindText to observe final parameters
    return mock_bindText(pStmt, i, zData, nData, xDel, enc);
}

// -----------------------------
// Test harness (non-terminating tests)
// -----------------------------

static volatile std::sig_atomic_t g_abortCaught = 0;
static jmp_buf g_jmpBuf;

// Abort signal handler to catch deliberate assertion failure (for test coverage)
static void abort_handler(int) {
    // Jump back into test harness if an assertion in sqlite3_bind_text64 aborts the program
    longjmp(g_jmpBuf, 1);
}

// Simple test: UTF8 path should not remap enc or modify nData
bool test_utf8_path_keeps_params() {
    reset_capture();

    const char* z = "hello";
    sqlite3_uint64 n = 12345;
    int i = 7;
    DelFn xDel = nullptr; // no custom deleter

    int rc = sqlite3_bind_text64(nullptr, i, z, n, xDel, SQLITE_UTF8);

    // Validate behavior: no encoding remap, no nData masking
    if (!g_callRegistered) {
        std::cerr << "test_utf8_path_keeps_params: bindText was not called\n";
        return false;
    }
    if (rc != 0) {
        std::cerr << "test_utf8_path_keeps_params: unexpected return code " << rc << "\n";
        return false;
    }
    if (g_lastCall.pStmt != nullptr) {
        // pStmt is a test sentinel; allow null
    }
    if (g_lastCall.i != i) {
        std::cerr << "test_utf8_path_keeps_params: i mismatch\n";
        return false;
    }
    if (g_lastCall.zData != z) {
        std::cerr << "test_utf8_path_keeps_params: zData mismatch\n";
        return false;
    }
    if (g_lastCall.nData != n) {
        std::cerr << "test_utf8_path_keeps_params: nData should be unchanged\n";
        return false;
    }
    if (g_lastCall.enc != SQLITE_UTF8) {
        std::cerr << "test_utf8_path_keeps_params: enc should remain SQLITE_UTF8\n";
        return false;
    }
    return true;
}

// Test: enc == SQLITE_UTF16 should remap to SQLITE_UTF16NATIVE and mask nData's LSB
bool test_utf16_path_remap_and_mask() {
    reset_capture();

    const char* z = "data";
    sqlite3_uint64 n = 0xABCDULL; // arbitrary value with LSB 1 to test masking
    int i = 1;
    DelFn xDel = nullptr;

    int rc = sqlite3_bind_text64(nullptr, i, z, n, xDel, SQLITE_UTF16);

    if (!g_callRegistered) {
        std::cerr << "test_utf16_path_remap_and_mask: bindText was not called\n";
        return false;
    }
    if (rc != 0) {
        std::cerr << "test_utf16_path_remap_and_mask: unexpected return code " << rc << "\n";
        return false;
    }
    // Expect enc remapped to SQLITE_UTF16NATIVE
    if (g_lastCall.enc != SQLITE_UTF16NATIVE) {
        std::cerr << "test_utf16_path_remap_and_mask: enc should be SQLITE_UTF16NATIVE\n";
        return false;
    }
    // nData should have LSB cleared: 0xABCD -> 0xABCC
    if (g_lastCall.nData != (n & ~(u16)1)) {
        std::cerr << "test_utf16_path_remap_and_mask: nData masking incorrect\n";
        return false;
    }
    return true;
}

// Test: enc other than UTF8 and not exactly UTF16 should still mask nData but pass through enc
bool test_other_enc_path_masks_but_passes_enc() {
    reset_capture();

    const char* z = "zzz";
    sqlite3_uint64 n = 0x101; // even value
    int i = 3;
    DelFn xDel = nullptr;
    unsigned char enc = 0x7F; // some non-UTF8, non-UTF16 encoding

    int rc = sqlite3_bind_text64(nullptr, i, z, n, xDel, enc);

    if (!g_callRegistered) {
        std::cerr << "test_other_enc_path_masks_but_passes_enc: bindText was not called\n";
        return false;
    }
    if (rc != 0) {
        std::cerr << "test_other_enc_path_masks_but_passes_enc: unexpected return code " << rc << "\n";
        return false;
    }
    // enc should remain unchanged since it's not exactly SQLITE_UTF16
    if (g_lastCall.enc != enc) {
        std::cerr << "test_other_enc_path_masks_but_passes_enc: enc should pass through unchanged\n";
        return false;
    }
    // nData must be masked since enc != UTF8
    if (g_lastCall.nData != (n & ~(u16)1)) {
        std::cerr << "test_other_enc_path_masks_but_passes_enc: nData masking incorrect\n";
        return false;
    }
    return true;
}

// Test: when xDel == SQLITE_DYNAMIC, an assertion should fail (abort)
bool test_dynamic_del_assertion_aborts() {
    // Install a handler to catch abort and perform a non-terminating test
    g_abortCaught = 0;
    std::signal(SIGABRT, abort_handler);

    if (setjmp(g_jmpBuf) == 0) {
        // This should trigger an assertion failure inside sqlite3_bind_text64
        sqlite3_bind_text64(nullptr, 0, nullptr, 0, SQLITE_DYNAMIC, SQLITE_UTF8);
        // If we reach here, the assertion did not abort as expected
        return false;
    } else {
        // We jumped back here due to abort
        // Restore default handler
        std::signal(SIGABRT, SIG_DFL);
        return true;
    }
}

// Helper to run all tests and print results
void run_all_tests() {
    int total = 4;
    int passed = 0;

    std::cout << "Running tests for sqlite3_bind_text64 focal logic (mocked harness)..." << std::endl;

    if (test_utf8_path_keeps_params()) {
        std::cout << "[PASS] UTF8 path preserves i/zData/nData and enc." << std::endl;
        ++passed;
    } else {
        std::cout << "[FAIL] UTF8 path behavior incorrect." << std::endl;
    }

    if (test_utf16_path_remap_and_mask()) {
        std::cout << "[PASS] UTF16 path remaps to UTF16NATIVE and masks nData." << std::endl;
        ++passed;
    } else {
        std::cout << "[FAIL] UTF16 path remap/mask behavior incorrect." << std::endl;
    }

    if (test_other_enc_path_masks_but_passes_enc()) {
        std::cout << "[PASS] Other non-UTF8 enc path masks nData and passes enc." << std::endl;
        ++passed;
    } else {
        std::cout << "[FAIL] Other enc path behavior incorrect." << std::endl;
    }

    if (test_dynamic_del_assertion_aborts()) {
        std::cout << "[PASS] Dynamic xDel triggers abort via assertion as expected." << std::endl;
        ++passed;
    } else {
        std::cout << "[FAIL] Dynamic xDel did not abort as expected." << std::endl;
    }

    std::cout << "Tests passed: " << passed << "/" << total << std::endl;
}

// Entry point
int main() {
    run_all_tests();
    return 0;
}