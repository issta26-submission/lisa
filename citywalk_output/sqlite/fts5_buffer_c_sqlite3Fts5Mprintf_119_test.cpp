// Test suite for sqlite3Fts5Mprintf using a lightweight, non-GTest framework.
// This test is designed to be compiled alongside the project's fts5_buffer.c
// and provides a minimal mock for sqlite3_vmprintf to control behavior.

#include <fts5Int.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <string>
#include <iostream>
#include <cstdlib>


// Visualization and assertion helpers (non-terminating assertions)
static int g_failCount = 0;
#define EXPECT_TRUE(cond, msg) do { if(!(cond)){ ++g_failCount; std::cerr << "EXPECT_TRUE FAILED: " << msg << "\n"; } } while(0)
#define EXPECT_EQ(a,b,msg) do { if((a)!=(b)){ ++g_failCount; std::cerr << "EXPECT_EQ FAILED: " << msg << " (expected " << (b) << ", got " << (a) << ")\n"; } } while(0)
#define EXPECT_NULL(p, msg) do { if((p) != nullptr){ ++g_failCount; std::cerr << "EXPECT_NULL FAILED: " << msg << "\n"; } } while(0)
#define EXPECT_NOT_NULL(p, msg) do { if((p) == nullptr){ ++g_failCount; std::cerr << "EXPECT_NOT_NULL FAILED: " << msg << "\n"; } } while(0)

// Define SQLITE_OK / SQLITE_NOMEM if not provided by the project headers.
// This helps this standalone test to compile in environments without the real sqlite3 headers.
// If the project provides these macros, the #ifndef guards prevent redefinition.
#ifndef SQLITE_OK
#define SQLITE_OK 0
#endif
#ifndef SQLITE_NOMEM
#define SQLITE_NOMEM 7
#endif

// Forward declaration of the focal function under test.
// In the real project this function is defined in fts5_buffer.c.
// The test code should link against the project's compilation units that define this function.
extern "C" char *sqlite3Fts5Mprintf(int *pRc, const char *zFmt, ...);

// Mock implementation for sqlite3_vmprintf to drive test scenarios.
// The real function is provided by SQLite; in this test, we override it with a controlled mock.
// We route calls through this symbol; the link should resolve to this definition in absence of
// a real sqlite3_vmprintf, or this will override the library one if linked together.
static int g_vmprintfCallCount = 0;
static int g_vmprintfForceFail = 0; // When non-zero, sqlite3_vmprintf returns nullptr to simulate OOM
static int g_vmprintfIgnoreCallsWhenBadRc = 0; // If set, signalled calls when pRc != SQLITE_OK are ignored

extern "C" char *sqlite3_vmprintf(const char *zFormat, va_list ap){
    ++g_vmprintfCallCount;
    if(g_vmprintfForceFail){
        return nullptr; // Simulate allocation failure
    }
    // Basic vsnprintf-based formatter to provide reasonable string results.
    va_list apCopy;
    va_copy(apCopy, ap);
    int needed = vsnprintf(nullptr, 0, zFormat, apCopy);
    va_end(apCopy);
    if(needed < 0){
        return nullptr;
    }
    char *out = (char*)std::malloc((size_t)needed + 1);
    if(!out){
        return nullptr;
    }
    va_copy(apCopy, ap);
    vsnprintf(out, (size_t)needed + 1, zFormat, apCopy);
    va_end(apCopy);
    return out;
}

// Test 1: Basic formatting when pRc == SQLITE_OK should allocate and format correctly.
static void test_basic_formatting(){
    // Reset flags
    g_vmprintfCallCount = 0;
    g_vmprintfForceFail = 0;

    int rc = SQLITE_OK;
    const char *fmt = "Hello %s";
    const char *arg = "World";

    char *res = sqlite3Fts5Mprintf(&rc, fmt, arg);
    EXPECT_NOT_NULL(res, "Expected non-null result when pRc is SQLITE_OK");
    if(res){
        int cmp = std::strcmp(res, "Hello World");
        EXPECT_EQ(cmp, 0, "Formatted string should be 'Hello World'");
        std::free(res);
    }
    EXPECT_TRUE(rc == SQLITE_OK, "pRc should remain SQLITE_OK after successful formatting");
    // At least one, but not strictly required, assertion about vmprintf usage
    EXPECT_TRUE(g_vmprintfCallCount > 0, "sqlite3_vmprintf should have been called on success");
}

// Test 2: When pRc is not SQLITE_OK, sqlite3Fts5Mprintf should not call vmprintf and return NULL.
static void test_no_call_when_bad_rc(){
    // Reset flags
    g_vmprintfCallCount = 0;
    g_vmprintfForceFail = 0;

    int rc = 1; // Not SQLITE_OK
    char *res = sqlite3Fts5Mprintf(&rc, "Hello %s", "World");
    EXPECT_NULL(res, "Expected NULL result when pRc != SQLITE_OK");
    EXPECT_TRUE(g_vmprintfCallCount == 0, "sqlite3_vmprintf should NOT be called when pRc != SQLITE_OK");
    EXPECT_TRUE(rc == 1, "pRc should remain unchanged when not OK");
}

// Test 3: Simulate an out-of-memory condition by forcing vmprintf to fail.
static void test_oom_handling(){
    // Reset flags
    g_vmprintfCallCount = 0;
    g_vmprintfForceFail = 1; // Force OOM

    int rc = SQLITE_OK;
    char *res = sqlite3Fts5Mprintf(&rc, "Number: %d", 12345);
    EXPECT_NULL(res, "Expected NULL result when vmprintf reports OOM");
    EXPECT_TRUE(rc == SQLITE_NOMEM, "pRc should be SQLITE_NOMEM on OOM");
    // Clean up if any (the function should not allocate on OOM)
    if(res) std::free(res);
    g_vmprintfForceFail = 0; // Reset for other tests
}

// Test 4: Ensure that subsequent tests correctly observe vmprintf usage after a successful path.
static void test_multiple_calls_consistency(){
    // Reset
    g_vmprintfCallCount = 0;
    g_vmprintfForceFail = 0;

    int rc = SQLITE_OK;
    char *r1 = sqlite3Fts5Mprintf(&rc, "A=%s", "1");
    if(r1) { std::free(r1); }

    int rc2 = SQLITE_OK;
    char *r2 = sqlite3Fts5Mprintf(&rc2, "B=%d", 2);
    if(r2) { std::free(r2); }

    EXPECT_TRUE(g_vmprintfCallCount >= 2, "vmprintf should be invoked on successive successful calls");
}

// Helper to reset debugging state between tests
static void reset_state(){
    g_vmprintfCallCount = 0;
    g_vmprintfForceFail = 0;
    g_vmprintfIgnoreCallsWhenBadRc = 0;
}

int main(){
    std::cout << "Running sqlite3Fts5Mprintf test suite (C++11, non-GTest)...\n";

    test_basic_formatting();
    reset_state();

    test_no_call_when_bad_rc();
    reset_state();

    test_oom_handling();
    reset_state();

    test_multiple_calls_consistency();
    reset_state();

    if(g_failCount == 0){
        std::cout << "All tests passed (" << 4 << " test scenarios).\n";
        return 0;
    }else{
        std::cerr << g_failCount << " test(s) failed.\n";
        return 1;
    }
}

// Notes for maintainers:
// - The tests rely on a mock implementation of sqlite3_vmprintf to control behavior.
// - The real project should provide the actual sqlite3_vmprintf and related symbols;
//   this test overrides behavior via the linker as needed, while keeping test logic portable.
// - All tests are designed to run without terminating on failure, to maximize code coverage.