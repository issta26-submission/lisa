// Test suite for sqlite3VdbeMemCast (C++11, no GTest) with lightweight test harness.
// The tests interpose minimal stub implementations for selected SQLite internal helpers
// to achieve deterministic, isolated behavior without needing the full runtime.
//
// Notes:
// - The test uses internal SQLite structures (Mem, sqlite3) from the project headers.
// - We provide non-terminating checks so that all tests run and report failures.
// - We rely on the actual sqlite3VdbeMemCast signature and behavior as shown in the focal code.
// - Extern "C" wrappers are used where appropriate to ensure proper linkage with C code.

#include <vector>
#include <cstdio>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <cstdint>


// Include SQLite internal headers to obtain Mem type and constants.
// These headers exist in the provided project; ensure this test is compiled in the same
// source tree and with the same include paths as the project.
extern "C" {
}

// Forward declaration of the function under test (from the production code).
// This prototype must match the real signature.
extern "C" int sqlite3VdbeMemCast(Mem *pMem, uint8_t aff, uint8_t encoding);

// Global counters to verify which helper stubs were invoked.
static int gValueApplyAffinityCalls = 0;
static int gNumerifyCalls = 0;
static int gIntegerifyCalls = 0;
static int gRealifyCalls = 0;

// Lightweight, non-terminating test assertion helpers.
static int gTestFailures = 0;
#define CHECK(cond, msg) do { \
    if(!(cond)) { \
        std::fprintf(stderr, "TEST_FAIL: %s\n", (msg)); \
        ++gTestFailures; \
    } \
} while(0)


// Stubbed helpers to interpose and observe behavior without depending on the full SQLite runtime.
extern "C" {

// Simulate applying a TEXT affinity to a Mem value.
// We intentionally mark the value as a string so that the caller's subsequent logic (e.g., setting MEM_Blob) can proceed.
int sqlite3ValueApplyAffinity(Mem *pMem, uint8_t aff, uint8_t encoding) {
    (void)aff; (void)encoding; // unused in our simplified stub
    ++gValueApplyAffinityCalls;

    // Simulate that affinity was applied by marking the memory as a string.
    // In real SQLite, this would attempt to convert or reinterpret the value; here we only affect flags.
    pMem->flags |= MEM_Str;
    return 0;
}

// Mark a memory cell as a specific type flag (e.g., MEM_Blob).
void MemSetTypeFlag(Mem *pMem, uint32_t flag) {
    pMem->flags |= flag;
}

// Numerify, Integerify, Realify stubs to observe which path is taken.
int sqlite3VdbeMemNumerify(Mem *pMem) {
    (void)pMem;
    ++gNumerifyCalls;
    return 0;
}
int sqlite3VdbeMemIntegerify(Mem *pMem) {
    (void)pMem;
    ++gIntegerifyCalls;
    return 0;
}
int sqlite3VdbeMemRealify(Mem *pMem) {
    (void)pMem;
    ++gRealifyCalls;
    return 0;
}

// Minimal stubs for encoding changes and cleanup to satisfy linker references.
// They don't affect the test outcomes beyond ensuring the function under test returns SQLITE_OK.
int sqlite3VdbeChangeEncoding(Mem *pMem, uint8_t desiredEnc) {
    (void)pMem; (void)desiredEnc;
    return 0;
}
void sqlite3VdbeMemZeroTerminateIfAble(Mem *pMem) {
    (void)pMem;
}

} // extern "C"


// Test harness: run all unit tests and report the summary.
static void run_all_tests() {
    // Reset counters before tests
    gValueApplyAffinityCalls = 0;
    gNumerifyCalls = 0;
    gIntegerifyCalls = 0;
    gRealifyCalls = 0;
    gTestFailures = 0;

    // Helper to create a zero-initialized Mem object.
    auto makeMem = []() -> Mem {
        Mem m;
        std::memset(&m, 0, sizeof(Mem));
        // The actual Mem structure in SQLite is large; zero-init to a clean base.
        return m;
    };

    // We rely on internal flags/constants provided by sqliteInt.h/vdbeInt.h.
    // Note: We only exercise true branches and avoid default TEXT branch to prevent
    // triggering hard-to-satisfy asserts in the default clause.

    // Test 1: If MEM_Null flag is set, sqlite3VdbeMemCast should return SQLITE_OK immediately.
    {
        Mem m = makeMem();
        m.flags = MEM_Null;
        int rc = sqlite3VdbeMemCast(&m, SQLITE_AFF_BLOB, SQLITE_UTF8);
        CHECK(rc == SQLITE_OK, "sqlite3VdbeMemCast should return SQLITE_OK for MEM_Null input");
    }

    // Test 2: Blob cast when not already a blob:
    // - Expect the code path to call sqlite3ValueApplyAffinity once.
    // - Expect MEM_Str to be set by the affinity stub, and MEM_Blob to be set by MemSetTypeFlag.
    {
        Mem m = makeMem();
        m.flags = 0;                  // NOT a blob initially
        sqlite3 dbDummy = {0};         // dummy db to satisfy layout; not dereferenced in this path
        m.db = &dbDummy;

        int rc = sqlite3VdbeMemCast(&m, SQLITE_AFF_BLOB, SQLITE_UTF8);
        CHECK(rc == SQLITE_OK, "Blob cast when not blob should succeed");
        CHECK(gValueApplyAffinityCalls == 1, "ValueApplyAffinity should be called once for blob cast");
        // We set MEM_Str in the stub; then MemSetTypeFlag should mark MEM_Blob.
        CHECK((m.flags & MEM_Blob) != 0, "MEM_Blob should be set after blob-cast with non-blob input");
    }

    // Test 3: Blob cast when MEM_Blob is already set:
    // - Expect the code path to skip applying affinity.
    // - Some systems may still manipulate type flags, but we primarily verify
    //   that the affinity path is not triggered.
    {
        Mem m = makeMem();
        m.flags = MEM_Blob; // Already a blob
        int rc = sqlite3VdbeMemCast(&m, SQLITE_AFF_BLOB, SQLITE_UTF8);
        CHECK(rc == SQLITE_OK, "Blob cast when already blob should succeed");
        CHECK(gValueApplyAffinityCalls == 0, "ValueApplyAffinity should not be called when MEM_Blob is already set");
    }

    // Reset numeric-related counters for subsequent tests
    gNumerifyCalls = 0;
    gIntegerifyCalls = 0;
    gRealifyCalls = 0;

    // Test 4: Numeric affinity should call sqlite3VdbeMemNumerify
    {
        Mem m = makeMem();
        int rc = sqlite3VdbeMemCast(&m, SQLITE_AFF_NUMERIC, SQLITE_UTF8);
        CHECK(rc == SQLITE_OK, "NUMERIC cast should succeed");
        CHECK(gNumerifyCalls == 1, "sqlite3VdbeMemNumerify should be called once for NUMERIC");
    }

    // Test 5: INTEGER affinity should call sqlite3VdbeMemIntegerify
    {
        Mem m = makeMem();
        int rc = sqlite3VdbeMemCast(&m, SQLITE_AFF_INTEGER, SQLITE_UTF8);
        CHECK(rc == SQLITE_OK, "INTEGER cast should succeed");
        CHECK(gIntegerifyCalls == 1, "sqlite3VdbeMemIntegerify should be called once for INTEGER");
    }

    // Test 6: REAL affinity should call sqlite3VdbeMemRealify
    {
        Mem m = makeMem();
        int rc = sqlite3VdbeMemCast(&m, SQLITE_AFF_REAL, SQLITE_UTF8);
        CHECK(rc == SQLITE_OK, "REAL cast should succeed");
        CHECK(gRealifyCalls == 1, "sqlite3VdbeMemRealify should be called once for REAL");
    }

    // Summary
    if(gTestFailures == 0) {
        std::printf("ALL TESTS PASSED\n");
    } else {
        std::fprintf(stderr, "%d TEST(S) FAILED\n", gTestFailures);
    }
}

// Entry point
int main() {
    run_all_tests();
    return gTestFailures ? 1 : 0;
}