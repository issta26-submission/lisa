// Test suite for sqlite3_value_dup (internal helper in vdbeapi.c)
// This test harness is written in C++11, avoids GTest and uses a lightweight
// in-process test runner with non-terminating assertions to maximize coverage.
// Explanatory comments are provided for each unit test.

#include <opcodes.h>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>


// The test relies on internal SQLite structures and APIs.
// Include internal headers to access structure fields like sqlite3_value, MEM_* flags.
extern "C" {
}

// Declarations of the focal function and related memory management.
// We assume these symbols are available in the build environment.
extern "C" sqlite3_value *sqlite3_value_dup(const sqlite3_value *pOrig);
extern "C" void sqlite3ValueFree(sqlite3_value *pVal);
extern "C" void *sqlite3_malloc(size_t);
extern "C" void sqlite3_free(void*);

//////////////////////
// Lightweight test framework
//////////////////////
static int g_pass = 0;
static int g_fail = 0;

static void log_pass(const char* testName) {
    ++g_pass;
    std::cout << "[PASS] " << testName << std::endl;
}

static void log_fail(const char* testName, const char* reason) {
    ++g_fail;
    std::cout << "[FAIL] " << testName << " - " << reason << std::endl;
}

// Convenience macro-like helpers (non-terminating)
#define ASSERT_TRUE(cond, testName, reason) \
    do { if(cond) log_pass(testName); else log_fail(testName, reason); } while(0)

#define ASSERT_NOT_NULL(ptr, testName) \
    do { if((ptr) != nullptr) log_pass(testName); else log_fail(testName, "returned null"); } while(0)

#define ASSERT_EQ(a, b, testName, reason) \
    do { if((a) == (b)) log_pass(testName); else log_fail(testName, reason); } while(0)

//////////////////////
// Helpers to construct sqlite3_value instances for testing
//////////////////////

// Create a sqlite3_value with given flags and db pointer.
// We rely on internal layout: struct sqlite3_value contains fields 'flags' and 'db'.
// MEMCELLSIZE is the size used by sqlite3_value_dup when copying memory.
static sqlite3_value* make_test_value(unsigned int flags, void* db_ptr) {
    size_t sz = MEMCELLSIZE;
    sqlite3_value* p = (sqlite3_value*)sqlite3_malloc(sz);
    if(!p) return nullptr;
    // Zero initialize to a clean baseline
    memset(p, 0, sz);

    // Initialize fields that sqlite3_value_dup will read/write.
    // Cast to the correct types based on internal definitions in sqliteInt.h.
    // The actual field types are internal, but typically include:
    //   unsigned char flags;
    //   sqlite3* db;
    // We assume direct member access is valid in this internal test context.
    p->flags = static_cast<unsigned char>(flags);
    p->db = db_ptr;

    // Other internal fields are left zeroed for simplicity.
    return p;
}

// Free a test sqlite3_value using the internal API
static void free_test_value(sqlite3_value* p) {
    if(p) {
        sqlite3ValueFree(p);
    }
}

//////////////////////
// Unit Tests
//////////////////////

// Test 1: When pOrig is NULL, sqlite3_value_dup should return NULL.
// This validates the early exit branch.
static void test_dup_null_input() {
    // Focal behavior: if pOrig==0 return 0
    sqlite3_value *pDup = sqlite3_value_dup(nullptr);
    ASSERT_TRUE(pDup == nullptr, "sqlite3_value_dup(NULL) returns NULL",
                "Expected NULL return for NULL input");
}

// Test 2: Duplicating a value with MEM_Str (string) or MEM_Blob should
// clear MEM_Dyn, clear MEM_Dyn/Static on the copy appropriately, set MEM_Ephem,
// and keep MEM_Str set. Also ensure db pointer is reset to 0.
static void test_dup_mem_str_branch() {
    // Prepare original with MEM_Str flag and a non-null db pointer to ensure it
    // changes to 0 in the duplicate as per function logic.
    unsigned int flags = MEM_Str; // triggers the string/blob branch
    void* dummyDb = (void*)0xDEADBEEF;

    sqlite3_value *pOrig = make_test_value(flags, dummyDb);
    if(!pOrig) {
        log_fail("test_dup_mem_str_branch", "Failed to allocate pOrig");
        return;
    }

    sqlite3_value *pDup = sqlite3_value_dup(pOrig);
    free_test_value(pOrig);

    if(pDup == nullptr) {
        log_fail("test_dup_mem_str_branch", "sqlite3_value_dup returned NULL");
        return;
    }

    // Expect MEM_Str still set
    bool hasStr = (pDup->flags & MEM_Str) != 0;
    // Expect MEM_Ephem to be set
    bool isEphem = (pDup->flags & MEM_Ephem) != 0;
    // Expect MEM_Static and MEM_Dyn to be cleared
    bool dynCleared = (pDup->flags & MEM_Dyn) == 0;
    bool staticCleared = (pDup->flags & MEM_Static) == 0;
    // db pointer should be zero
    bool dbReset = pDup->db == 0;

    // Validate
    std::string testName = "test_dup_mem_str_branch";
    ASSERT_TRUE(hasStr, "sqlite3_value_dup preserves MEM_Str",
                "MEM_Str should be preserved in the duplicate");
    ASSERT_TRUE(isEphem, "sqlite3_value_dup sets MEM_Ephem",
                "MEM_Ephem should be set for string/blob duplicates");
    ASSERT_TRUE(dynCleared && staticCleared, "sqlite3_value_dup clears MEM_Dyn and MEM_Static",
                "Copied value should have MEM_Dyn and MEM_Static cleared");
    ASSERT_TRUE(dbReset, "sqlite3_value_dup resets db pointer to 0",
                "Duplicate's db pointer must be 0");

    // Cleanup
    free_test_value(pDup);
}

// Test 3: Duplicating a Null MEM_Null value should clear MEM_Term and MEM_Subtype
// as per the path else-if (pNew->flags & MEM_Null) branch.
static void test_dup_mem_null_branch() {
    unsigned int flags = MEM_Null;
    void* dummyDb = 0;

    sqlite3_value *pOrig = make_test_value(flags, dummyDb);
    if(!pOrig) {
        log_fail("test_dup_mem_null_branch", "Failed to allocate pOrig");
        return;
    }

    sqlite3_value *pDup = sqlite3_value_dup(pOrig);
    free_test_value(pOrig);

    if(pDup == nullptr) {
        log_fail("test_dup_mem_null_branch", "sqlite3_value_dup returned NULL");
        return;
    }

    // MEM_Term and MEM_Subtype should be cleared
    bool termCleared = (pDup->flags & MEM_Term) == 0;
    bool subtypeCleared = (pDup->flags & MEM_Subtype) == 0;

    std::string testName = "test_dup_mem_null_branch";
    ASSERT_TRUE(termCleared, "MEM_Term cleared for MEM_Null duplicate",
                "MEM_Term should be cleared when duplicating MEM_Null");
    ASSERT_TRUE(subtypeCleared, "MEM_Subtype cleared for MEM_Null duplicate",
                "MEM_Subtype should be cleared when duplicating MEM_Null");

    free_test_value(pDup);
}

// Test 4: Duplicating a value with MEM_Dyn set should result in MEM_Dyn being cleared
// in the duplicate (the top of sqlite3_value_dup does pNew->flags &= ~MEM_Dyn;).
static void test_dup_mem_dyn_branch() {
    unsigned int flags = MEM_Dyn;
    void* dummyDb = 0xCAFEBABE;

    sqlite3_value *pOrig = make_test_value(flags, dummyDb);
    if(!pOrig) {
        log_fail("test_dup_mem_dyn_branch", "Failed to allocate pOrig");
        return;
    }

    sqlite3_value *pDup = sqlite3_value_dup(pOrig);
    free_test_value(pOrig);

    if(pDup == nullptr) {
        log_fail("test_dup_mem_dyn_branch", "sqlite3_value_dup returned NULL");
        return;
    }

    bool dynCleared = (pDup->flags & MEM_Dyn) == 0;
    std::string testName = "test_dup_mem_dyn_branch";
    ASSERT_TRUE(dynCleared, "MEM_Dyn cleared in duplicate",
                "Duplicate should have MEM_Dyn cleared regardless of input");

    free_test_value(pDup);
}

//////////////////////
// Main
//////////////////////

int main() {
    std::cout << "Starting sqlite3_value_dup unit tests (vdbeapi.c)..." << std::endl;

    test_dup_null_input();
    test_dup_mem_str_branch();
    test_dup_mem_null_branch();
    test_dup_mem_dyn_branch();

    std::cout << "Tests completed. Summary: "
              << g_pass << " passed, "
              << g_fail << " failed." << std::endl;

    // Return non-zero if any test failed to signal issues in CI environments.
    return (g_fail > 0) ? 1 : 0;
}