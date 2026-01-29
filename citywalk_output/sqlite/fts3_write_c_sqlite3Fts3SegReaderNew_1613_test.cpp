/*
  Unit test suite for sqlite3Fts3SegReaderNew (fts3_write.c)

  Design notes:
  - No GoogleTest; a lightweight C++11 test harness is implemented.
  - Tests exercise the focal function against its core behaviors:
      1) Root-stored segment (iStartLeaf == 0) allocates extra storage and copies root data.
      2) Non-root case (iStartLeaf != 0) does not allocate extra root storage.
      3) Corrupt condition (iStartLeaf == 0 and iEndLeaf != 0) returns FTS_CORRUPT_VTAB.
      4) Edge case with zero root data (iStartLeaf == 0, iEndLeaf == 0, nRoot == 0) behaves as expected.

  - Static internal dependencies are accessed through the provided struct fields
    (as exposed by the project headers). The tests rely on the public/internal
    API provided by the fts3Int.h and related headers included in the project.

  - Test harness uses non-terminating assertions to maximize coverage.
    Each test returns a boolean indicating pass/fail; the main function aggregates results.

  - Static helpers within the test ensure that resources are freed after each test.

  - The test code is designed to compile with a C++11 toolchain and link against the
    existing project (which provides sqlite3Fts3SegReaderNew and related symbols).
*/

#include <fts3Int.h>
#include <stdlib.h>
#include <functional>
#include <string.h>
#include <vector>
#include <assert.h>
#include <stdio.h>
#include <cstring>
#include <iostream>


// Include the SQLite FTS3 internal headers to obtain type and constant definitions.
// The project under test should provide these headers; adjust include paths as needed.
extern "C" {
// The focal function is C linkage; including the header should expose necessary prototypes.
}

 // If the project headers do not declare the function with C linkage under C++,
 // keep a local extern 'C' prototype as a fallback. Most likely the project already
 // provides these declarations via its headers.
extern "C" {
  int sqlite3Fts3SegReaderNew(
      int iAge,
      int bLookup,
      sqlite3_int64 iStartLeaf,
      sqlite3_int64 iEndLeaf,
      sqlite3_int64 iEndBlock,
      const char *zRoot,
      int nRoot,
      Fts3SegReader **ppReader
  );
  void sqlite3Fts3SegReaderFree(Fts3SegReader *pReader);
}

// Lightweight test harness
struct TestCase {
    const char* name;
    std::function<bool()> func;
};

static bool test_root_storage_allocation_and_copy() {
    // Test that when iStartLeaf == 0 and iEndLeaf == 0, a root-stored segment is allocated,
    // copied into aNode, and padding is zeroed.
    int iAge = 5;
    int bLookup = 1;
    sqlite3_int64 iStartLeaf = 0;
    sqlite3_int64 iEndLeaf   = 0;
    sqlite3_int64 iEndBlock  = 12345;
    const char *zRoot = "ROOTDATA"; // 8 bytes including terminating zero, but copy uses nRoot
    int nRoot = 8;                  // copy 8 bytes of root data

    Fts3SegReader *pReader = nullptr;
    int rc = sqlite3Fts3SegReaderNew(iAge, bLookup, iStartLeaf, iEndLeaf, iEndBlock,
                                    zRoot, nRoot, &pReader);
    if (rc != SQLITE_OK || pReader == nullptr) {
        std::cerr << "test_root_storage_allocation_and_copy: allocation failed (rc=" << rc << ")\n";
        return false;
    }

    // Validate key fields
    if (pReader->iIdx != iAge) {
        std::cerr << "test_root_storage_allocation_and_copy: iIdx mismatch\n";
        sqlite3Fts3SegReaderFree(pReader);
        return false;
    }
    if (pReader->bLookup != (bLookup != 0)) {
        std::cerr << "test_root_storage_allocation_and_copy: bLookup mismatch\n";
        sqlite3Fts3SegReaderFree(pReader);
        return false;
    }
    if (pReader->iStartBlock != iStartLeaf || pReader->iLeafEndBlock != iEndLeaf) {
        std::cerr << "test_root_storage_allocation_and_copy: block range mismatch\n";
        sqlite3Fts3SegReaderFree(pReader);
        return false;
    }
    if (pReader->iEndBlock != iEndBlock) {
        std::cerr << "test_root_storage_allocation_and_copy: iEndBlock mismatch\n";
        sqlite3Fts3SegReaderFree(pReader);
        return false;
    }
    if (pReader->rootOnly != 1) {
        std::cerr << "test_root_storage_allocation_and_copy: rootOnly flag not set\n";
        sqlite3Fts3SegReaderFree(pReader);
        return false;
    }
    if (pReader->nNode != nRoot) {
        std::cerr << "test_root_storage_allocation_and_copy: nNode mismatch\n";
        sqlite3Fts3SegReaderFree(pReader);
        return false;
    }

    // aNode should point to the memory immediately after the Fts3SegReader struct
    if (pReader->aNode != (char *)&pReader[1]) {
        std::cerr << "test_root_storage_allocation_and_copy: aNode pointer mismatch\n";
        sqlite3Fts3SegReaderFree(pReader);
        return false;
    }

    // Root data should be copied exactly
    if (memcmp(pReader->aNode, zRoot, nRoot) != 0) {
        std::cerr << "test_root_storage_allocation_and_copy: root data mismatch\n";
        sqlite3Fts3SegReaderFree(pReader);
        return false;
    }

    // The padding region should be zeroed
    for (int i = nRoot; i < nRoot + FTS3_NODE_PADDING; ++i) {
        if (pReader->aNode[i] != 0) {
            std::cerr << "test_root_storage_allocation_and_copy: padding not zeroed at offset " << i << "\n";
            sqlite3Fts3SegReaderFree(pReader);
            return false;
        }
    }

    // In this path, iCurrentBlock should remain zero due to memset initialization
    if (pReader->iCurrentBlock != 0) {
        std::cerr << "test_root_storage_allocation_and_copy: iCurrentBlock should be 0 for root storage path\n";
        sqlite3Fts3SegReaderFree(pReader);
        return false;
    }

    sqlite3Fts3SegReaderFree(pReader);
    return true;
}

static bool test_startleaf_nonzero_sets_current_block_and_no_root_copy() {
    // Test that when iStartLeaf != 0, no root node is allocated and iCurrentBlock is set correctly.
    int iAge = 2;
    int bLookup = 0;
    sqlite3_int64 iStartLeaf = 5;
    sqlite3_int64 iEndLeaf = 0;
    sqlite3_int64 iEndBlock = 999;
    const char *zRoot = NULL;
    int nRoot = 0;

    Fts3SegReader *pReader = nullptr;
    int rc = sqlite3Fts3SegReaderNew(iAge, bLookup, iStartLeaf, iEndLeaf, iEndBlock,
                                    zRoot, nRoot, &pReader);
    if (rc != SQLITE_OK || pReader == nullptr) {
        std::cerr << "test_startleaf_nonzero_sets_current_block_and_no_root_copy: allocation failed (rc=" << rc << ")\n";
        return false;
    }

    // Validate key fields
    if (pReader->iIdx != iAge) {
        std::cerr << "test_startleaf_nonzero_sets_current_block_and_no_root_copy: iIdx mismatch\n";
        sqlite3Fts3SegReaderFree(pReader);
        return false;
    }
    if (pReader->bLookup != (bLookup != 0)) {
        std::cerr << "test_startleaf_nonzero_sets_current_block_and_no_root_copy: bLookup mismatch\n";
        sqlite3Fts3SegReaderFree(pReader);
        return false;
    }
    if (pReader->iStartBlock != iStartLeaf || pReader->iLeafEndBlock != iEndLeaf) {
        std::cerr << "test_startleaf_nonzero_sets_current_block_and_no_root_copy: block range mismatch\n";
        sqlite3Fts3SegReaderFree(pReader);
        return false;
    }
    if (pReader->iEndBlock != iEndBlock) {
        std::cerr << "test_startleaf_nonzero_sets_current_block_and_no_root_copy: iEndBlock mismatch\n";
        sqlite3Fts3SegReaderFree(pReader);
        return false;
    }
    if (pReader->rootOnly != 0) {
        std::cerr << "test_startleaf_nonzero_sets_current_block_and_no_root_copy: rootOnly should be 0\n";
        sqlite3Fts3SegReaderFree(pReader);
        return false;
    }

    // aNode should be NULL since no root data is stored
    if (pReader->aNode != nullptr) {
        std::cerr << "test_startleaf_nonzero_sets_current_block_and_no_root_copy: aNode should be NULL\n";
        sqlite3Fts3SegReaderFree(pReader);
        return false;
    }

    // iCurrentBlock should be iStartLeaf - 1
    if (pReader->iCurrentBlock != (iStartLeaf - 1)) {
        std::cerr << "test_startleaf_nonzero_sets_current_block_and_no_root_copy: iCurrentBlock incorrect\n";
        sqlite3Fts3SegReaderFree(pReader);
        return false;
    }

    sqlite3Fts3SegReaderFree(pReader);
    return true;
}

static bool test_corrupt_vtab_case() {
    // Test that iStartLeaf == 0 and iEndLeaf != 0 results in FTS_CORRUPT_VTAB
    int iAge = 1;
    int bLookup = 0;
    sqlite3_int64 iStartLeaf = 0;
    sqlite3_int64 iEndLeaf = 7;    // non-zero
    sqlite3_int64 iEndBlock = 10;
    const char *zRoot = NULL;
    int nRoot = 0;

    Fts3SegReader *pReader = nullptr;
    int rc = sqlite3Fts3SegReaderNew(iAge, bLookup, iStartLeaf, iEndLeaf, iEndBlock,
                                    zRoot, nRoot, &pReader);
    // Expect VTAB corruption error
    if (rc != FTS_CORRUPT_VTAB) {
        std::cerr << "test_corrupt_vtab_case: expected FTS_CORRUPT_VTAB, got " << rc << "\n";
        if (pReader) sqlite3Fts3SegReaderFree(pReader);
        return false;
    }
    // The reader should not be allocated
    if (pReader != nullptr) {
        std::cerr << "test_corrupt_vtab_case: reader should be null on corrupt case\n";
        sqlite3Fts3SegReaderFree(pReader);
        return false;
    }
    return true;
}

static bool test_zero_root_zero_leaf_case() {
    // iStartLeaf == 0, iEndLeaf == 0, nRoot == 0 (zRoot NULL) should allocate minimal reader
    int iAge = 3;
    int bLookup = 0;
    sqlite3_int64 iStartLeaf = 0;
    sqlite3_int64 iEndLeaf = 0;
    sqlite3_int64 iEndBlock = 42;
    const char *zRoot = NULL;
    int nRoot = 0;

    Fts3SegReader *pReader = nullptr;
    int rc = sqlite3Fts3SegReaderNew(iAge, bLookup, iStartLeaf, iEndLeaf, iEndBlock,
                                    zRoot, nRoot, &pReader);
    if (rc != SQLITE_OK || pReader == nullptr) {
        std::cerr << "test_zero_root_zero_leaf_case: allocation failed (rc=" << rc << ")\n";
        return false;
    }

    // iCurrentBlock should be iStartLeaf - 1 => -1
    if (pReader->iCurrentBlock != (iStartLeaf - 1)) {
        std::cerr << "test_zero_root_zero_leaf_case: iCurrentBlock mismatch\n";
        sqlite3Fts3SegReaderFree(pReader);
        return false;
    }
    // aNode should be NULL since there is no root data
    if (pReader->aNode != nullptr) {
        std::cerr << "test_zero_root_zero_leaf_case: aNode should be NULL\n";
        sqlite3Fts3SegReaderFree(pReader);
        return false;
    }

    // Cleanup
    sqlite3Fts3SegReaderFree(pReader);
    return true;
}

int main() {
    using namespace std;

    vector<TestCase> tests = {
        {"root_storage_allocation_and_copy", test_root_storage_allocation_and_copy},
        {"startleaf_nonzero_sets_current_block_and_no_root_copy", test_startleaf_nonzero_sets_current_block_and_no_root_copy},
        {"corrupt_vtab_case", test_corrupt_vtab_case},
        {"zero_root_zero_leaf_case", test_zero_root_zero_leaf_case}
    };

    int passed = 0;
    int failed = 0;

    cout << "Running tests for sqlite3Fts3SegReaderNew (fts3_write.c wrapper)\n";

    for (const auto& t : tests) {
        bool ok = t.func();
        if (ok) {
            cout << "[PASS] " << t.name << "\n";
            ++passed;
        } else {
            cout << "[FAIL] " << t.name << "\n";
            ++failed;
        }
    }

    cout << "Summary: " << passed << " passed, " << failed << " failed.\n";

    // Return non-zero if any test failed
    return failed ? 1 : 0;
}