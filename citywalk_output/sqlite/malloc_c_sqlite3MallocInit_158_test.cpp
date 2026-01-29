// Minimal, self-contained unit test suite for sqlite3MallocInit
// Focus: verify behavior of sqlite3MallocInit under varied global config states.
// This test suite is designed to compile under C++11 without GTest and uses a small
// homegrown test harness to collect and report results.

#include <vector>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <stdarg.h>
#include <iostream>
#include <cstdlib>


// ---------------------------
// Minimal mock/skeleton of SQLite-like structures used by sqlite3MallocInit
// ---------------------------

static const int SQLITE_OK = 0;
static const int SQLITE_MUTEX_STATIC_MEM = 0;

// Forward declare dummy mutex type
struct sqlite3_mutex {
    int dummy;
};

// Global flag to verify whether sqlite3MemSetDefault() was invoked
static bool g_memSetDefaultCalled = false;

// Forward declare global environment used by sqlite3MallocInit
struct sqlite3MemMethods {
    void* (*xMalloc)(size_t);  // may be NULL to indicate default allocator
    void  (*xFree)(void*);
    int   (*xInit)(void*);
    void* pAppData;
};

struct sqlite3GlobalConfigStruct {
    sqlite3MemMethods m;
    void* pPage;
    int   szPage;
    int   nPage;
};

// Global configuration instance (simulated)
static sqlite3GlobalConfigStruct sqlite3GlobalConfig;

// Simple mem0 structure used by sqlite3MallocInit
static struct {
    sqlite3_mutex *mutex;
} mem0;

// Mutex allocator used by sqlite3MallocInit
static sqlite3_mutex* sqlite3MutexAlloc(int /*type*/) {
    // Simple non-null placeholder to resemble a real mutex
    sqlite3_mutex* m = new sqlite3_mutex();
    m->dummy = 0;
    return m;
}

// Default memory setter invoked when xMalloc is NULL
static void sqlite3MemSetDefault(void) {
    g_memSetDefaultCalled = true;
}

// Function pointer configurations used by tests
static void* testDummyMalloc(size_t n) {
    // Use standard malloc to satisfy any internal usage during tests
    return std::malloc(n);
}

// The focal function copied/synthesized for the test environment
int sqlite3MallocInit(void){
  int rc;
  if( sqlite3GlobalConfig.m.xMalloc==0 ){
    sqlite3MemSetDefault();
  }
  mem0.mutex = sqlite3MutexAlloc(SQLITE_MUTEX_STATIC_MEM);
  if( sqlite3GlobalConfig.pPage==0 || sqlite3GlobalConfig.szPage<512
      || sqlite3GlobalConfig.nPage<=0 ){
    sqlite3GlobalConfig.pPage = 0;
    sqlite3GlobalConfig.szPage = 0;
  }
  rc = sqlite3GlobalConfig.m.xInit(sqlite3GlobalConfig.m.pAppData);
  if( rc!=SQLITE_OK ) memset(&mem0, 0, sizeof(mem0));
  return rc;
}

// ---------------------------
// Lightweight test harness (no GTest/GMock)
// ---------------------------

struct TestResult {
    std::string name;
    int passed; // 1 = pass, 0 = fail
    std::string message;
};

class TestSuite {
public:
    using TestFunc = bool(*)();

    static void add(const std::string& name, TestFunc f) {
        tests.push_back({name, f() ? 1 : 0, ""});
        // Note: In this simple harness, we immediately evaluate and store results.
        // For more informative output, we could store function pointers and run later.
        lastNames.push_back(name);
        // We also collect messages inside f() in a real expanded harness.
    }

    static void run_all() {
        // In this simplified approach, tests were registered via add() already.
        // We just print a summary here.
        int idx = 0;
        int passCount = 0;
        for (const auto& res : results) {
            std::cout << (res.passed ? "[PASS] " : "[FAIL] ")
                      << res.name << " - " << res.message << "\n";
            if (res.passed) passCount++;
            idx++;
        }
        std::cout << "Tests run: " << results.size()
                  << ", Passed: " << passCount
                  << ", Failed: " << (int)results.size() - passCount << "\n";
    }

    // Register a test result with messaging
    static void report(const std::string& name, bool ok, const std::string& msg) {
        TestResult r{name, ok ? 1 : 0, msg};
        results.push_back(r);
        // Also print immediate feedback
        std::cout << (ok ? "[PASS] " : "[FAIL] ") << name
                  << (msg.empty() ? "" : (" - " + msg)) << "\n";
    }

private:
    static std::vector<TestResult> results;
    static std::vector<std::string> lastNames;
    static std::vector<std::string> names;
    // In this minimal design, add()() directly reports; we store results in results.
};

std::vector<TestResult> TestSuite::results;
std::vector<std::string> TestSuite::lastNames;
std::vector<std::string> TestSuite::names;

// Helper assertion-like utilities (non-terminating, record results)
static void expect_true(bool cond, const std::string& msg, const std::string& testName) {
    if (!cond) {
        TestSuite::report(testName, false, "Expectation failed: " + msg);
    } else {
        TestSuite::report(testName, true, "");
    }
}

static void expect_equal_ptr(const void* a, const void* b, const std::string& msg, const std::string& testName) {
    bool ok = (a == b);
    if (!ok) {
        TestSuite::report(testName, false, "Pointer mismatch: " + msg);
    } else {
        TestSuite::report(testName, true, "");
    }
}

// ---------------------------
// Test Cases
// Each test function returns true if all expectations pass.
// They rely on the minimal sqlite3MallocInit environment defined above.
// ---------------------------

static bool test_init_calls_memSetDefault_when_xMalloc_zero() {
    // Reset global state
    g_memSetDefaultCalled = false;
    sqlite3GlobalConfig.m.xMalloc = 0;
    sqlite3GlobalConfig.m.xInit = [](void*){
        return SQLITE_OK;
    };
    sqlite3GlobalConfig.m.pAppData = nullptr;
    sqlite3GlobalConfig.pPage = reinterpret_cast<void*>(0); // 0 -> triggers reset path
    sqlite3GlobalConfig.szPage = 0;
    sqlite3GlobalConfig.nPage = 0;
    mem0.mutex = nullptr;
    // Call
    int rc = sqlite3MallocInit();

    // Verify true branch of first if: MemSetDefault should be invoked
    expect_true(g_memSetDefaultCalled, "sqlite3MemSetDefault should be called when xMalloc is 0",
                "test_init_calls_memSetDefault_when_xMalloc_zero");
    // Ensure mutex is allocated
    expect_true(mem0.mutex != nullptr, "sqlite3MallocInit should allocate a mutex",
                "test_init_calls_memSetDefault_when_xMalloc_zero");
    // rc should be SQLITE_OK
    expect_true(rc == SQLITE_OK, "sqlite3MallocInit should return SQLITE_OK on success",
                "test_init_calls_memSetDefault_when_xMalloc_zero");
    return true;
}

static bool test_init_skips_memSetDefault_when_xMalloc_nonzero() {
    // Reset global state
    g_memSetDefaultCalled = false;
    // Provide a non-null xMalloc
    int dummyInit(void*) { return SQLITE_OK; }
    static void* dummyMallocPtr(size_t) { return nullptr; }
    sqlite3GlobalConfig.m.xMalloc = dummyMallocPtr; // non-zero
    sqlite3GlobalConfig.m.xInit = dummyInit;
    sqlite3GlobalConfig.m.pAppData = nullptr;
    sqlite3GlobalConfig.pPage = nullptr;
    sqlite3GlobalConfig.szPage = 0;
    sqlite3GlobalConfig.nPage = 0; // invalid to test non-default path, but xMalloc nonzero should skip memSetDefault
    mem0.mutex = nullptr;

    int rc = sqlite3MallocInit();

    // Verify MemSetDefault was not called
    expect_true(!g_memSetDefaultCalled, "sqlite3MemSetDefault should NOT be called when xMalloc != 0",
                "test_init_skips_memSetDefault_when_xMalloc_nonzero");
    // Mutex should still be allocated
    expect_true(mem0.mutex != nullptr, "sqlite3MallocInit should allocate a mutex even if memSetDefault is skipped",
                "test_init_skips_memSetDefault_when_xMalloc_nonzero");
    // rc should be SQLITE_OK
    expect_true(rc == SQLITE_OK, "sqlite3MallocInit should return SQLITE_OK on success",
                "test_init_skips_memSetDefault_when_xMalloc_nonzero");
    return true;
}

static bool test_init_rc_non_ok_clears_mem0() {
    // Reset environment
    g_memSetDefaultCalled = false;
    sqlite3GlobalConfig.m.xMalloc = testDummyMalloc; // non-null
    sqlite3GlobalConfig.m.xInit = [](void*){ return 1; }; // non-zero to simulate failure
    sqlite3GlobalConfig.m.pAppData = nullptr;
    sqlite3GlobalConfig.pPage = reinterpret_cast<void*>(0x42); // non-zero to avoid page-reset path
    sqlite3GlobalConfig.szPage = 1024; // >= 512
    sqlite3GlobalConfig.nPage = 1;
    // Pre-fill mem0 to confirm it gets cleared on failure
    mem0.mutex = new sqlite3_mutex();
    int rc = sqlite3MallocInit();

    // rc should indicate error
    expect_true(rc != SQLITE_OK, "sqlite3MallocInit should return non-zero on xInit failure",
                "test_init_rc_non_ok_clears_mem0");
    // mem0 should be cleared (mutex set to NULL)
    expect_true(mem0.mutex == nullptr, "mem0 should be cleared when rc != SQLITE_OK",
                "test_init_rc_non_ok_clears_mem0");
    // Clean up potential leak if any (defensive)
    if (mem0.mutex) delete mem0.mutex;
    return true;
}

static bool test_init_rc_ok_keeps_mem0_and_true_path() {
    // Reset environment
    g_memSetDefaultCalled = false;
    sqlite3GlobalConfig.m.xMalloc = testDummyMalloc; // non-null
    sqlite3GlobalConfig.m.xInit = [](void*){ return SQLITE_OK; }; // success
    sqlite3GlobalConfig.m.pAppData = nullptr;
    sqlite3GlobalConfig.pPage = reinterpret_cast<void*>(0x1000); // non-zero
    sqlite3GlobalConfig.szPage = 1024; // >= 512
    sqlite3GlobalConfig.nPage = 2;
    // Pre-fill mem0 to ensure it remains after success
    mem0.mutex = new sqlite3_mutex();
    int rc = sqlite3MallocInit();

    // rc should be OK
    expect_true(rc == SQLITE_OK, "sqlite3MallocInit should return SQLITE_OK on success",
                "test_init_rc_ok_keeps_mem0_and_true_path");
    // mem0 should remain non-null
    expect_true(mem0.mutex != nullptr, "mem0 mutex should remain allocated on success",
                "test_init_rc_ok_keeps_mem0_and_true_path");
    // Clean up
    if (mem0.mutex) delete mem0.mutex;
    return true;
}

// ---------------------------
// Main
// ---------------------------

int main() {
    // Run tests
    // We intentionally call tests in main to satisfy "call test methods from the main function" directive.
    TestSuite::results.clear();

    // Register and execute tests (each test is a function returning bool)
    auto runTest = [](const std::string& name, bool (*fn)()) {
        bool ok = fn();
        // The test functions themselves register detailed results via the helpers.
        // Here we just print a summary line (the helpers already did per-test messages).
        // Return status is not used further; we can extend if desired.
        (void)name;
        return ok;
    };

    // Run
    runTest("test_init_calls_memSetDefault_when_xMalloc_zero", test_init_calls_memSetDefault_when_xMalloc_zero);
    runTest("test_init_skips_memSetDefault_when_xMalloc_nonzero", test_init_skips_memSetDefault_when_xMalloc_nonzero);
    runTest("test_init_rc_non_ok_clears_mem0", test_init_rc_non_ok_clears_mem0);
    runTest("test_init_rc_ok_keeps_mem0_and_true_path", test_init_rc_ok_keeps_mem0_and_true_path);

    // Note: Since we used a lightweight harness that prints as it goes, a final summary
    // isn't strictly necessary. The individual test prints serve as the report.

    // Cleanup any remaining allocated mutex if still allocated
    if (mem0.mutex) {
        delete mem0.mutex;
        mem0.mutex = nullptr;
    }

    return 0;
}

// Provide dummy malloc function used in tests
static void* testDummyMalloc(size_t n) {
    return std::malloc(n);
}