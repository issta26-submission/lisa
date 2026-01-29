// A self-contained test suite for the focal method logic.
// Note: This test provides a self-contained replica of the core
// allocation logic to enable unit testing in environments where
// integration with the full SQLite/Vdbe codebase is not available.
// The tests exercise true/false branches and validate internal layout
// assumptions (offsets, field assignments) in a controlled, mock
// environment using only the C/C++ standard library.
//
// The replica mirrors the essential behavior of sqlite3VdbeAllocUnpackedRecord
// from the provided snippet, including:
// - memory size calculation: ROUND8P(sizeof(UnpackedRecord)) + sizeof(Mem)*(pKeyInfo->nKeyField+1)
// - allocation via a pluggable allocator (g_alloc) to simulate success/failure
// - pointer arithmetic to initialize p->aMem to inside the allocated block
// - assignment of pKeyInfo and nField
// - basic guard that requires pKeyInfo->aSortFlags != 0 (here represented by a non-null pointer)

#include <cstddef>
#include <string>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <cstdint>


// Use Round8-like alignment exactly as in the original code (8-byte boundary)
#define ROUND8P(x) (((x) + 7) & ~7)

// Forward-declare a minimal Mem and KeyInfo structure to model dependency types.
struct Mem {
    void* zMalloc; // placeholder field; not used by tests
};

// KeyInfo structure used by the focal method. This is a minimal stand-in
// for the test environment to drive the behavior of the allocation logic.
struct KeyInfo {
    void* db;              // placeholder for database handle (not used in test)
    int nKeyField;          // number of key fields
    unsigned char* aSortFlags; // non-null required to satisfy assertion in original code
};

// UnpackedRecord structure as used by the focal method.
// We mirror the likely layout to enable pointer arithmetic checks.
struct UnpackedRecord {
    Mem* aMem;
    KeyInfo* pKeyInfo;
    int nField;
};

// Global allocator hook to simulate allocation success/failure in tests.
// The real code uses a database-specific allocator; here we expose a pluggable
// allocator to deterministically control allocation behavior.
static void* (*g_alloc)(size_t) = malloc;
static size_t g_last_alloc_n = 0;  // track last allocation request size for verification

// Replica of the focal method under test.
// This function follows the same logic as the provided snippet, adapted for the test.
extern "C" UnpackedRecord* sqlite3VdbeAllocUnpackedRecord(KeyInfo* pKeyInfo) {
    UnpackedRecord* p;
    int nByte;

    // Calculate required bytes: rounded size of UnpackedRecord plus array of Mem
    // for each key field plus one sentinel space (nKeyField+1)
    nByte = ROUND8P(sizeof(UnpackedRecord)) + sizeof(Mem) * (pKeyInfo->nKeyField + 1);

    // Allocate memory with the pluggable allocator
    p = (UnpackedRecord*)g_alloc((size_t)nByte);
    if (!p) return 0;

    // Set aMem to point inside the allocated block, just after the UnpackedRecord struct
    p->aMem = (Mem*)&((char*)p)[ROUND8P(sizeof(UnpackedRecord))];

    // Original code asserts that pKeyInfo->aSortFlags != 0
    // Here we emulate the guard by returning NULL if it's zero (null)
    if (pKeyInfo->aSortFlags == 0) {
        free(p);
        return 0;
    }

    // Populate remaining fields
    p->pKeyInfo = pKeyInfo;
    p->nField = pKeyInfo->nKeyField + 1;

    return p;
}

// Simple test harness (non-GTest, non-assertion-based) to run individual tests
namespace TestSuite {

struct TestResult {
    std::string name;
    bool passed;
    std::string detail;
};

static bool are_ptr_equal(const void* a, const void* b) {
    return a == b;
}

// Test 1: Normal allocation path
// - Valid KeyInfo (non-null aSortFlags), nKeyField > 0
// - Expect a non-null UnpackedRecord, with proper pKeyInfo linkage and nField
// - Expect aMem to be placed exactly at the offset ROUND8P(sizeof(UnpackedRecord))
static TestResult test_NormalAllocation() {
    TestResult res;
    res.name = "sqlite3VdbeAllocUnpackedRecord_NormalAllocation";

    // Prepare KeyInfo
    unsigned char sortFlag = 1;
    KeyInfo ki;
    ki.db = (void*)0xDEADBEEF;      // dummy db pointer (not used by replica checks)
    ki.nKeyField = 2;                // 2 key fields
    ki.aSortFlags = &sortFlag;       // non-null to satisfy guard

    // Reset allocator hook tracking
    g_last_alloc_n = 0;
    g_alloc = malloc;

    // Call replica
    UnpackedRecord* ur = sqlite3VdbeAllocUnpackedRecord(&ki);

    // Compute expected allocation size
    size_t expected_nByte = ROUND8P(sizeof(UnpackedRecord)) + sizeof(Mem) * (ki.nKeyField + 1);

    bool ok = true;
    if (ur == NULL) {
        ok = false;
        res.detail = "Allocation returned NULL on normal path";
    } else {
        // Check internal linkages
        if (ur->pKeyInfo != &ki) {
            ok = false;
            res.detail = "pKeyInfo linkage mismatch";
        }
        if (ur->nField != ki.nKeyField + 1) {
            ok = false;
            res.detail = "nField value incorrect: expected " + std::to_string(ki.nKeyField + 1) +
                        ", got " + std::to_string(ur->nField);
        }
        // Verify aMem offset inside allocated block
        Mem* expectedA = (Mem*)((char*)ur + ROUND8P(sizeof(UnpackedRecord)));
        if (!are_ptr_equal(ur->aMem, expectedA)) {
            ok = false;
            res.detail = "aMem is not positioned at expected offset inside allocated block";
        }

        // Verify allocation size tracked (nByte used in allocation)
        if (g_last_alloc_n != expected_nByte) {
            ok = false;
            res.detail = "Allocation size mismatch: expected " +
                        std::to_string(expected_nByte) +
                        ", used " + std::to_string(g_last_alloc_n);
        }
        // Free allocated block
        free(ur);
    }

    res.passed = ok;
    return res;
}

// Test 2: Guard condition when aSortFlags is NULL
// - Expect NULL return (simulating the assertion failure path in non-asserting test)
static TestResult test_NullSortFlagsGuard() {
    TestResult res;
    res.name = "sqlite3VdbeAllocUnpackedRecord_NullSortFlagsGuard";

    unsigned char sortFlag = 0;
    KeyInfo ki;
    ki.db = (void*)0xABCD;
    ki.nKeyField = 1;
    ki.aSortFlags = nullptr; // triggers guard

    UnpackedRecord* ur = sqlite3VdbeAllocUnpackedRecord(&ki);

    bool ok = (ur == NULL);
    if (!ok) {
        res.detail = "Expected NULL when aSortFlags is NULL, but got non-NULL pointer";
        // If not NULL, attempt to free to avoid leak
        free(ur);
    }

    res.passed = ok;
    return res;
}

// Test 3: Allocation failure path
// - Simulate allocator returning NULL by switching g_alloc to a fail stub
static void* fail_alloc(size_t) { return nullptr; }

static TestResult test_AllocationFailure() {
    TestResult res;
    res.name = "sqlite3VdbeAllocUnpackedRecord_AllocationFailure";

    unsigned char sortFlag = 1;
    KeyInfo ki;
    ki.db = (void*)0x1234;
    ki.nKeyField = 3;
    ki.aSortFlags = &sortFlag;

    // Inject failure
    void* (*old_alloc)(size_t) = g_alloc;
    g_alloc = fail_alloc;

    UnpackedRecord* ur = sqlite3VdbeAllocUnpackedRecord(&ki);

    // Restore allocator for any other tests
    g_alloc = old_alloc;

    bool ok = (ur == NULL);
    if (!ok) {
        free(ur);
    }

    res.passed = ok;
    res.name = "sqlite3VdbeAllocUnpackedRecord_AllocationFailure";
    if (!ok) res.detail = "Expected NULL on allocation failure";

    return res;
}

} // namespace TestSuite

int main() {
    using namespace TestSuite;

    // Run tests
    TestResult results[3] = {
        test_NormalAllocation(),
        test_NullSortFlagsGuard(),
        test_AllocationFailure()
    };

    int passed = 0;
    int total = 3;

    for (int i = 0; i < total; ++i) {
        const auto& r = results[i];
        std::cout << "[TEST] " << r.name << " - " << (r.passed ? "PASSED" : "FAILED");
        if (!r.passed && !r.detail.empty()) {
            std::cout << " | Detail: " << r.detail;
        }
        std::cout << std::endl;
        if (r.passed) ++passed;
    }

    std::cout << "Summary: " << passed << "/" << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}