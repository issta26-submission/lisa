// Test suite for sqlite3PCacheSetDefault (pcache1.c)
// This test suite is designed to be compiled with C++11 (no Google Test).
// It uses a lightweight in-process mock of sqlite3_config to observe
// inputs to sqlite3PCacheSetDefault and to validate the behavior of the
// default PCache2 methods pointer being passed.
//
// Important notes (domain knowledge applied):
// - We treat the pcache2 methods struct's first field as iVersion (expected 1).
// - The tests exercise the interaction via a mocked sqlite3_config function.
// - Static/internal functions of pcache1.c are not accessed directly; we validate
//   through the observable interface (sqlite3_config call and its argument).
// - We avoid relying on private/private static details beyond what is observable
//   via the config hook (non-terminating assertions are used via macros).
// - This test uses only the C/C++ standard library and the provided method under test.
//
// Candidate Keywords (derived from Step 1): sqlite3PCacheSetDefault, sqlite3_config, SQLITE_CONFIG_PCACHE2,
// defaultMethods, pcache1Init, pcache1Shutdown, pcache1Create, pcache1Cachesize, pcache1Pagecount,
// pcache1Fetch, pcache1Unpin, pcache1Rekey, pcache1Truncate, pcache1Destroy, pcache1Shrink,
// sqlite3_pcache_methods2, iVersion, pArg.
//
// Source layout expectations:
// - The test relies on the existence of a function: void sqlite3PCacheSetDefault(void); (provided by pcache1.c)
// - sqlite3_config has a variadic signature: void sqlite3_config(int op, ...);
// - The second argument to sqlite3_config in this usage is a pointer to sqlite3_pcache_methods2
//   (address of defaultMethods in sqlite3PCacheSetDefault).

#include <cstdarg>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Forward declaration of the focal function under test.
// The function is defined in pcache1.c; ensure it's linked in during build.
extern "C" void sqlite3PCacheSetDefault(void);

// Mock state to observe sqlite3_config invocations and parameters.
static bool g_config_called = false;
static int g_config_call_count = 0;
static void* g_pcache2_ptr = nullptr;

// Lightweight, C-linkage mock of sqlite3_config to capture inputs.
// This mock assumes the usage: sqlite3_config(SQLITE_CONFIG_PCACHE2, &defaultMethods);
extern "C" void sqlite3_config(int op, ...) {
    g_config_called = true;
    ++g_config_call_count;

    va_list ap;
    va_start(ap, op);
    // Attempt to retrieve the second variadic argument as a void*.
    // This corresponds to the pointer to sqlite3_pcache_methods2 (defaultMethods).
    void* arg2 = nullptr;
    // The actual function call in pcache1.c passes one additional argument after 'op'.
    // We defensively try to fetch it; if not present, arg2 remains nullptr.
    arg2 = va_arg(ap, void*);
    g_pcache2_ptr = arg2;
    va_end(ap);
}

// Simple simulation of the sqlite3_pcache_methods2 structure layout to inspect iVersion.
// Only the first two fields are used in tests (iVersion and pArg).
struct sqlite3_pcache_methods2_sim {
    int iVersion;
    void* pArg;
    // The rest of the fields are omitted for the test harness.
};

// Simple test harness utilities (non-terminating assertions)
static int g_failures = 0;
#define EXPECT_TRUE(cond) \
  do { if(!(cond)) { ++g_failures; std::cerr << "EXPECT_TRUE failed: " #cond " at line " << __LINE__ << "\n"; } } while(0)

#define EXPECT_EQ(a,b) \
  do { if((a) != (b)) { ++g_failures; std::cerr << "EXPECT_EQ failed: " #a " != " #b " (actual: " << (a) << ", " << (b) << ") at line " << __LINE__ << "\n"; } } while(0)

#define EXPECT_NOT_NULL(p) \
  do { if((p) == nullptr) { ++g_failures; std::cerr << "EXPECT_NOT_NULL failed: " #p " is null at line " << __LINE__ << "\n"; } } while(0)


// Test 1: Basic behavior of sqlite3PCacheSetDefault
// - Verifies that sqlite3_config is called.
// - Verifies that a non-null pointer to a sqlite3_pcache_methods2 (defaultMethods) is passed.
// - Verifies that the iVersion field in the passed struct equals 1 and pArg is nullptr.
void test_sqlite3PCacheSetDefault_basic() {
    // Reset mock state
    g_config_called = false;
    g_config_call_count = 0;
    g_pcache2_ptr = nullptr;

    // Execute the focal method
    sqlite3PCacheSetDefault();

    // Observations
    EXPECT_TRUE(g_config_called);
    EXPECT_EQ(g_config_call_count, 1);
    EXPECT_NOT_NULL(g_pcache2_ptr);

    // Inspect the concrete memory layout of the passed defaultMethods pointer
    if (g_pcache2_ptr != nullptr) {
        sqlite3_pcache_methods2_sim* m = reinterpret_cast<sqlite3_pcache_methods2_sim*>(g_pcache2_ptr);
        // Validate that the first field is iVersion == 1
        EXPECT_TRUE(m->iVersion == 1);
        // Validate that pArg is 0 (nullptr)
        EXPECT_TRUE(m->pArg == nullptr);
    }
}

// Test 2: Idempotence of sqlite3PCacheSetDefault
// - Ensures multiple invocations lead to multiple sqlite3_config calls (no internal state prevents repeated configuration).
// - Verifies that the pointer to methods remains non-null after subsequent calls.
void test_sqlite3PCacheSetDefault_idempotent() {
    // Reset mock state
    g_config_called = false;
    g_config_call_count = 0;
    g_pcache2_ptr = nullptr;

    // Invoke multiple times
    sqlite3PCacheSetDefault();
    sqlite3PCacheSetDefault();

    // Observations
    EXPECT_EQ(g_config_call_count, 2);
    EXPECT_NOT_NULL(g_pcache2_ptr);
}

// Run all tests and report summary
int main() {
    // Global test suite execution
    test_sqlite3PCacheSetDefault_basic();
    test_sqlite3PCacheSetDefault_idempotent();

    // Summary
    if (g_failures > 0) {
        std::cerr << "Total test failures: " << g_failures << "\n";
        return 1;
    } else {
        std::cout << "All tests passed (non-terminating assertions).\n";
        return 0;
    }
}