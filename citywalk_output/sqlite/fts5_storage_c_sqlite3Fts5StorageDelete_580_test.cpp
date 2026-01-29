/*
Unit Test Suite for sqlite3Fts5StorageDelete
- Language: C++11
- No Google Test (GTest) usage; a tiny in-file test harness is provided.
- The tests exercise the focal method sqlite3Fts5StorageDelete from fts5_storage.c
- Assumptions:
  - The build environment provides SQLite3 (sqlite3.h, sqlite3_value, sqlite3_stmt, SQLITE_OK, etc.)
  - The repository provides fts5Int.h (which defines Fts5Config, Fts5Storage, Fts5Index, and related enums/types like FTS5_CONTENT_NORMAL)
  - Underlying static helper functions in fts5_storage.c (e.g., fts5StorageLoadTotals, sqlite3Fts5IndexBeginWrite, etc.) are implemented in the provided codebase (here, we rely on their behavior to return SQLITE_OK by default so that code paths can be exercised deterministically).
- Tests are designed to cover true/false branches for key predicates:
  1) Contentless delete path vs. non-contentless path
  2) Columnsize deletion path (Docsize) on vs off
  3) Normal content path vs non-NORMAL content path (as far as the test can influence without altering the focused code)
- Static members and functions: accessed via public types and existing project headers; static internals are not mocked here.
- Non-terminating assertions: the test harness uses a lightweight continuation model (collects failures and reports at the end) to maximize code execution.
- The tests deliberately do not rely on private/internal details beyond what is exposed via Fts5Config/Fts5Storage and the focal function’s signature.
- The code includes explanatory comments for each test case.

Compile/run note:
- Ensure your include paths contain the project headers (e.g., -I/path/to/project/include) so that fts5Int.h and sqlite3 headers resolve.
- Link against SQLite3 (e.g., -lsqlite3) to satisfy sqlite3 symbols.
- You may need to adapt the include path to locate fts5Int.h depending on your build setup.

*/

#include <vector>
#include <fts5Int.h>
#include <sqlite3.h>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdint>


extern "C" {
  // Forward declare the focal function with C linkage
  // The actual signature is provided by the project headers (fts5Int.h)
  // We rely on those headers in the test compilation environment.
  // If the compiler requires, include the proper header instead of re-declaration.
  // Example actual signature used by the project:
  // int sqlite3Fts5StorageDelete(Fts5Storage *p, i64 iDel, sqlite3_value **apVal);
  // For safety, include the header that defines i64 and Fts5 types in your environment.
}


// Lightweight test framework (non-GTest)
static int g_failCount = 0;
static std::vector<std::string> g_failMessages;

#define TEST_ASSERT(cond, msg) do { \
    if(!(cond)) { \
        g_failCount++; \
        g_failMessages.push_back(std::string("ASSERT FAIL: ") + (msg)); \
    } \
} while(0)

#define TEST_LOG(msg) do { \
    std::cout << "[TEST] " << (msg) << std::endl; \
} while(0)

using DomainI64 = i64; // Align with project typedef (i64 defined in fts5Int.h)

// Helper to print summary
static void printSummary(const std::string &suiteName) {
    std::cout << "=== " << suiteName << " Summary ===" << std::endl;
    if(g_failCount == 0) {
        std::cout << "ALL tests PASSED" << std::endl;
    } else {
        std::cout << g_failCount << " test(s) FAILED" << std::endl;
        for(const auto &s : g_failMessages) {
            std::cout << "  - " << s << std::endl;
        }
    }
    std::cout << "==============================" << std::endl;
}

// Test 1: Basic path where contentless delete is enabled and docsize deletion is requested (bColumnsize = 1)
static void test_sqlite3Fts5StorageDelete_Contentless_WithDocsize() {
    TEST_LOG("Running test: Contentless delete enabled; docsize deletion enabled; NORMAL content path");

    // Arrange
    Fts5Config config;
    // Initialize with explicit values using the project's types
    config.eContent = FTS5_CONTENT_NORMAL; // Normal content
    config.bContentlessDelete = 1;          // Enable contentless delete path
    config.bColumnsize = 1;                 // Enable %_docsize deletion

    Fts5Index index;
    // Ensure index is zero-initialized to avoid undefined neighbors
    std::memset(&index, 0, sizeof(index));

    Fts5Storage storage;
    storage.pConfig = &config;
    storage.pIndex = &index;

    DomainI64 delId = 42; // arbitrary row/deletion ID
    sqlite3_value *apVal = nullptr; // apVal not used for NORMAL content path in this test

    // Act
    int rc = sqlite3Fts5StorageDelete(&storage, delId, &apVal);

    // Assert
    TEST_ASSERT(rc == SQLITE_OK, "RC should be SQLITE_OK in typical path for contentless + docsize");

    // Cleanup not strictly necessary due to stack-based objects
}

// Test 2: Basic path where contentless delete is disabled and docsize deletion is disabled as well
static void test_sqlite3Fts5StorageDelete_NoContentAndNoDocsize() {
    TEST_LOG("Running test: Contentless delete disabled; docsize deletion disabled; NORMAL content path");

    // Arrange
    Fts5Config config;
    config.eContent = FTS5_CONTENT_NORMAL; // Normal content
    config.bContentlessDelete = 0;           // Disable contentless delete
    config.bColumnsize = 0;                  // Disable %_docsize deletion

    Fts5Index index;
    std::memset(&index, 0, sizeof(index));

    Fts5Storage storage;
    storage.pConfig = &config;
    storage.pIndex = &index;

    DomainI64 delId = 1001;
    sqlite3_value *apVal = nullptr;

    // Act
    int rc = sqlite3Fts5StorageDelete(&storage, delId, &apVal);

    // Assert
    TEST_ASSERT(rc == SQLITE_OK, "RC should be SQLITE_OK even when no content/docsize actions occur");
}

// Test 3: Path where content is not NORMAL (non-NORMAL path); ensure code reaches non-content chain
static void test_sqlite3Fts5StorageDelete_NonNormalContentPath() {
    TEST_LOG("Running test: Non-NORMAL content path (eContent != NORMAL); docsize deletion enabled");

    // Arrange
    Fts5Config config;
    // Force a non-NORMAL content value using safe cast to the enum type
    config.eContent = (decltype(config.eContent))1; // Likely NOT NORMAL (assuming NORMAL==0)
    config.bContentlessDelete = 1; // Enable contentless delete path (we'll exercise this)
    config.bColumnsize = 1;        // Enable docsize deletion

    Fts5Index index;
    std::memset(&index, 0, sizeof(index));

    Fts5Storage storage;
    storage.pConfig = &config;
    storage.pIndex = &index;

    DomainI64 delId = 7;
    sqlite3_value *apVal = nullptr;

    // Act
    int rc = sqlite3Fts5StorageDelete(&storage, delId, &apVal);

    // Assert
    TEST_ASSERT(rc == SQLITE_OK, "RC should be SQLITE_OK for non-NORMAL content path with docsize deletion");

    // Note: The actual content deletion branch is controlled by eContent == NORMAL; since we forced non-NORMAL,
    // content deletion should be skipped, but docsize deletion is exercised if bColumnsize == 1.
}

// Main runner
int main() {
    // Group 1: Core path variants
    test_sqlite3Fts5StorageDelete_Contentless_WithDocsize();
    test_sqlite3Fts5StorageDelete_NoContentAndNoDocsize();
    test_sqlite3Fts5StorageDelete_NonNormalContentPath();

    printSummary("sqlite3Fts5StorageDelete Unit Tests (C++11)");

    // Return non-zero if any test failed
    return g_failCount;
}