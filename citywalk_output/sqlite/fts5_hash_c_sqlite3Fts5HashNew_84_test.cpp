#include <fts5Int.h>
#include <sqlite3.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Import necessary dependencies for the focal method under test

// Global counters for lightweight test harness
static int g_total_tests = 0;
static int g_failed_tests = 0;

// Simple non-terminating assertion helpers
#define LOG_ASSERT(cond, msg) do { \
    ++g_total_tests; \
    if(!(cond)){ \
        ++g_failed_tests; \
        std::cerr << "[FAIL] " << msg << "\n"; \
    } else { \
        std::cout << "[OK] " << msg << "\n"; \
    } \
} while(0)

#define ASSERT_EQUAL(expr1, expr2, msg) do { \
    ++g_total_tests; \
    if((expr1) != (expr2)){ \
        ++g_failed_tests; \
        std::cerr << "[FAIL] " << msg << " | expected: " << (expr2) << " actual: " << (expr1) << "\n"; \
    } else { \
        std::cout << "[OK] " << msg << "\n"; \
    } \
} while(0)


// Memory allocator wrappers to simulate allocation success/failure
// We override sqlite3_malloc/sqlite3_malloc64/sqlite3_free to control behavior of allocations
enum class MallocFailMode { NONE, FIRST, SECOND };
static MallocFailMode s_mallocFailMode = MallocFailMode::NONE;
static int s_mallocCallCount = 0;

// Forward declarations to satisfy C linkage for the sqlite3_* wrappers
extern "C" void* sqlite3_malloc(size_t n);
extern "C" void* sqlite3_malloc64(unsigned long long n);
extern "C" void  sqlite3_free(void* p);

// Override sqlite3_malloc
extern "C" void* sqlite3_malloc(size_t n){
    ++s_mallocCallCount;
    if( (s_mallocFailMode == MallocFailMode::FIRST && s_mallocCallCount == 1) ||
        (s_mallocFailMode == MallocFailMode::SECOND && s_mallocCallCount == 2) ){
        return NULL;
    }
    return std::malloc(n);
}

// Override sqlite3_malloc64
extern "C" void* sqlite3_malloc64(unsigned long long n){
    ++s_mallocCallCount;
    if( (s_mallocFailMode == MallocFailMode::FIRST && s_mallocCallCount == 1) ||
        (s_mallocFailMode == MallocFailMode::SECOND && s_mallocCallCount == 2) ){
        return NULL;
    }
    return std::malloc(static_cast<size_t>(n));
}

// Override sqlite3_free
extern "C" void sqlite3_free(void* p){
    std::free(p);
}


// Thin test suite for sqlite3Fts5HashNew
// Step 2: Generate test cases focused on the focal function buried in fts5_hash.c
// The tests rely on the actual structure definitions exposed by fts5Int.h and sqlite3.h.

void test_sqlite3Fts5HashNew_normal_path(){
    // Test the normal allocation path: both pNew and pNew->aSlot are allocated
    Fts5Config cfg;
    cfg.eDetail = 2; // arbitrary non-default detail level

    int providedPnByte = 12345;
    Fts5Hash *pHash = nullptr;

    int rc = sqlite3Fts5HashNew(&cfg, &pHash, &providedPnByte);

    LOG_ASSERT(rc == SQLITE_OK, "sqlite3Fts5HashNew should return SQLITE_OK on success");
    if( rc == SQLITE_OK ){
        ASSERT_EQUAL((pHash != nullptr) ? 1 : 0, 1, "pHash should be non-null on success");
        if(pHash != nullptr){
            ASSERT_EQUAL(pHash->nSlot, 1024, "pHash->nSlot should be 1024");
            ASSERT_EQUAL(pHash->eDetail, cfg.eDetail, "pHash->eDetail should copy from config");
            ASSERT_TRUE(pHash->aSlot != nullptr, "pHash->aSlot should be allocated");
            if(pHash->aSlot != nullptr){
                // All entries should be initialized to NULL
                ASSERT_EQUAL(pHash->aSlot[0], (Fts5HashEntry*)nullptr, "pHash->aSlot[0] should be NULL");
            }
            // pnByte should point to the provided int
            ASSERT_TRUE(pHash->pnByte != nullptr, "pHash->pnByte should be non-null");
            if(pHash->pnByte != nullptr){
                ASSERT_EQUAL(*(pHash->pnByte), providedPnByte, "pnByte should point to the provided int value");
            }
        }
    }

    // Cleanup (the actual implementation provides a harmless empty free function in test scope)
    sqlite3Fts5HashFree(pHash);
}

void test_sqlite3Fts5HashNew_firstMallocFailure(){
    // Simulate first allocation failure for pNew
    Fts5Config cfg;
    cfg.eDetail = 0;
    Fts5Hash *pHash = nullptr;
    int pnByte = 0;

    // Configure allocator to fail on the very first allocation
    g_mallocFailMode = 1; // placeholder if needed (we'll use dedicated mode below)
    s_mallocCallCount = 0;
    s_mallocFailMode = MallocFailMode::FIRST;

    int rc = sqlite3Fts5HashNew(&cfg, &pHash, &pnByte);

    LOG_ASSERT(rc == SQLITE_NOMEM, "sqlite3Fts5HashNew should return SQLITE_NOMEM when first malloc fails");
    LOG_ASSERT(pHash == nullptr, "ppNew should be null when first malloc fails");

    // Reset failure mode for subsequent tests
    s_mallocFailMode = MallocFailMode::NONE;
    s_mallocCallCount = 0;
}

void test_sqlite3Fts5HashNew_secondMallocFailure(){
    // Simulate second allocation failure: first succeeds (pNew), second (aSlot) fails
    Fts5Config cfg;
    cfg.eDetail = 1;
    Fts5Hash *pHash = nullptr;
    int pnByte = 0;

    s_mallocCallCount = 0;
    s_mallocFailMode = MallocFailMode::SECOND;

    int rc = sqlite3Fts5HashNew(&cfg, &pHash, &pnByte);

    LOG_ASSERT(rc == SQLITE_NOMEM, "sqlite3Fts5HashNew should return SQLITE_NOMEM when second malloc fails");
    LOG_ASSERT(pHash == nullptr, "ppNew should be null when second malloc fails");

    s_mallocFailMode = MallocFailMode::NONE;
    s_mallocCallCount = 0;
}


// Minimal wrapper to expose test-friendly boolean check for NULL pointer non-null in tests
inline bool is_null(void* p){ return p == nullptr; }
inline bool is_not_null(void* p){ return p != nullptr; }

// Since the test suite uses C-style structs defined by sqlite3/fts5 headers, provide
// a set of small helpers for readability in tests.
#define ASSERT_TRUE(cond, msg) LOG_ASSERT((cond), msg)

// Dummy compile-time constants used in assertions to avoid depending on internal enum values directly
#ifndef NULL
#define NULL nullptr
#endif

// Entrypoint for the test harness
int main() {
    std::cout << "Starting sqlite3Fts5HashNew test suite (C++11, no GTest)..." << std::endl;

    // Run tests
    test_sqlite3Fts5HashNew_normal_path();
    test_sqlite3Fts5HashNew_firstMallocFailure();
    test_sqlite3Fts5HashNew_secondMallocFailure();

    // Summary
    std::cout << "Test Summary: total = " << g_total_tests
              << ", failed = " << g_failed_tests << std::endl;

    // Return non-zero if there were failures to assist CI systems
    return g_failed_tests;
}