// A lightweight C++11 test suite for the focal function lsmCheckCompressionId
// NOTE: This test uses the actual project headers and avoids a full GTest framework.
// It relies on a small hand-rolled test harness with non-terminating expectations.

#include <lsmInt.h>
#include <iostream>
#include <cstring>


// Bring in the project's C headers in a C-compatible way
extern "C" {
}

// Declaration of the focal function (C linkage)
extern "C" int lsmCheckCompressionId(lsm_db *pDb, u32 iReq);

// If the repository does not expose a standalone lsm_shared.h, the above prototype
// and lsmInt.h must be present and visible to compile this test together with lsm_shared.c

// Simple test harness (non-terminating assertions)
static int g_totalTests = 0;
static int g_passedTests = 0;

#define TEST_OK(cond, name) do {                       \
    ++g_totalTests;                                      \
    if(cond) { ++g_passedTests; }                        \
    else { std::cerr << "[FAIL] " << name << "\n"; }     \
} while(0)

// Factory context used to observe internal behavior (via pCtx)
struct FactoryContext {
    bool observedDuringCall;
};

// -------------------------
// Test factories
// -------------------------

// Factory that changes the target iId to the requested iReq.
// It also records whether it was invoked with pDb->bInFactory == 1 inside.
static int factoryChangesId(void *pCtx, lsm_db *pDb, u32 iReq) {
    FactoryContext *ctx = reinterpret_cast<FactoryContext *>(pCtx);
    // We expect lsmCheckCompressionId to set bInFactory = 1 before invoking the factory.
    // If not, we still record an observation flag but the test will fail if needed.
    ctx->observedDuringCall = (pDb->bInFactory == 1);
    // Change the compression id to simulate a "successful" factory migration
    pDb->compress.iId = iReq;
    return 0;
}

// Factory that does not change iId, simulating an incompatible post-factory state.
// It also records whether it was invoked during the expected window.
static int factoryDoesNotChangeId(void *pCtx, lsm_db *pDb, u32 iReq) {
    FactoryContext *ctx = reinterpret_cast<FactoryContext *>(pCtx);
    ctx->observedDuringCall = (pDb->bInFactory == 1);
    // Do not modify pDb->compress.iId to simulate "no change"
    (void)iReq;
    return 0;
}

// -------------------------
// Individual test cases
// -------------------------

// Test 1: When iReq is LSM_COMPRESSION_EMPTY, the function should short-circuit with LSM_OK
// and not modify the compression id or enter the factory path.
static void test_emptyReq_shouldBeOK_noFactoryInvocation() {
    // Setup
    lsm_db db;
    std::memset(&db, 0, sizeof(db));
    db.compress.iId = 0x12345678;
    db.factory.xFactory = nullptr;
    db.factory.pCtx = nullptr;
    db.bInFactory = 0;

    // Execute
    int res = lsmCheckCompressionId(&db, LSM_COMPRESSION_EMPTY);

    // Verify
    TEST_OK(res == LSM_OK, "test_emptyReq_shouldBeOK_noFactoryInvocation: return OK");
    TEST_OK(db.compress.iId == 0x12345678, "test_emptyReq_shouldBeOK_noFactoryInvocation: iId unchanged");
    TEST_OK(db.bInFactory == 0, "test_emptyReq_shouldBeOK_noFactoryInvocation: bInFactory unchanged");
}

// Test 2: When iReq != EMPTY and pDb.compress.iId already matches iReq,
// no factory should be invoked and the result should be OK.
static void test_matchingId_noFactoryInvocation() {
    // Setup
    lsm_db db;
    std::memset(&db, 0, sizeof(db));
    db.compress.iId = 7;
    db.factory.xFactory = nullptr;
    db.factory.pCtx = nullptr;
    db.bInFactory = 0;

    // Execute
    int res = lsmCheckCompressionId(&db, 7);

    // Verify
    TEST_OK(res == LSM_OK, "test_matchingId_noFactoryInvocation: return OK when iId matches");
    TEST_OK(db.compress.iId == 7, "test_matchingId_noFactoryInvocation: iId unchanged");
    TEST_OK(db.bInFactory == 0, "test_matchingId_noFactoryInvocation: bInFactory unchanged");
}

// Test 3: When iReq != EMPTY and pDb.compress.iId != iReq and no factory is available,
// the function should return LSM_MISMATCH.
static void test_mismatch_noFactory_shouldReturnMismatch() {
    // Setup
    lsm_db db;
    std::memset(&db, 0, sizeof(db));
    db.compress.iId = 1;
    db.factory.xFactory = nullptr;
    db.factory.pCtx = nullptr;
    db.bInFactory = 0;

    // Execute
    int res = lsmCheckCompressionId(&db, 5);

    // Verify
    TEST_OK(res == LSM_MISMATCH, "test_mismatch_noFactory_shouldReturnMismatch: expect MISMATCH");
    TEST_OK(db.compress.iId == 1, "test_mismatch_noFactory_shouldReturnMismatch: iId unchanged");
    TEST_OK(db.bInFactory == 0, "test_mismatch_noFactory_shouldReturnMismatch: bInFactory unchanged");
}

// Test 4: When factory exists and changes iId to iReq, the function should return OK.
// Also verify that the factory is called with bInFactory == 1 during execution.
static void test_factoryChangesId_shouldResolveToOK() {
    // Setup
    lsm_db db;
    std::memset(&db, 0, sizeof(db));
    db.compress.iId = 2;
    db.factory.xFactory = factoryChangesId;
    static FactoryContext ctx; // local static to persist through call for observation
    ctx.observedDuringCall = false;
    db.factory.pCtx = &ctx;
    db.bInFactory = 0;

    // Execute
    int res = lsmCheckCompressionId(&db, 5);

    // Verify
    TEST_OK(res == LSM_OK, "test_factoryChangesId_shouldResolveToOK: factory can fix iId -> OK");
    TEST_OK(db.compress.iId == 5, "test_factoryChangesId_shouldResolveToOK: iId updated by factory to iReq");
    TEST_OK(ctx.observedDuringCall == true, "test_factoryChangesId_shouldResolveToOK: factory observed with bInFactory == 1");
    TEST_OK(db.bInFactory == 0, "test_factoryChangesId_shouldResolveToOK: bInFactory reset after call");
}

// Test 5: When factory exists but does not change iId, the function should return MISMATCH
// after factory invocation.
static void test_factoryDoesNotChange_shouldReturnMismatch() {
    // Setup
    lsm_db db;
    std::memset(&db, 0, sizeof(db));
    db.compress.iId = 1;
    db.factory.xFactory = factoryDoesNotChangeId;
    static FactoryContext ctx;
    ctx.observedDuringCall = false;
    db.factory.pCtx = &ctx;
    db.bInFactory = 0;

    // Execute
    int res = lsmCheckCompressionId(&db, 2);

    // Verify
    TEST_OK(res == LSM_MISMATCH, "test_factoryDoesNotChange_shouldReturnMismatch: mismatch persists after factory");
    TEST_OK(db.compress.iId == 1, "test_factoryDoesNotChange_shouldReturnMismatch: iId unchanged by factory");
    TEST_OK(ctx.observedDuringCall == true, "test_factoryDoesNotChange_shouldReturnMismatch: factory observed with bInFactory == 1");
    TEST_OK(db.bInFactory == 0, "test_factoryDoesNotChange_shouldReturnMismatch: bInFactory reset after call");

    // Note: the function type used for this test uses the same factoryDoesNotChangeId function definition.
}

// Extra factory for Test 5 to keep code coherent (alias)
static int factoryDoesNotChangeId(void *pCtx, lsm_db *pDb, u32 iReq) {
    FactoryContext *ctx = reinterpret_cast<FactoryContext *>(pCtx);
    ctx->observedDuringCall = (pDb->bInFactory == 1);
    // Do not modify iId
    (void)iReq;
    return 0;
}

// -------------------------
// Main entrypoint
// -------------------------

int main() {
    // Execute all test cases in a fixed order
    test_emptyReq_shouldBeOK_noFactoryInvocation();
    test_matchingId_noFactoryInvocation();
    test_mismatch_noFactory_shouldReturnMismatch();
    test_factoryChangesId_shouldResolveToOK();
    test_factoryDoesNotChange_shouldReturnMismatch();

    // Report summary
    std::cout << "Total tests: " << g_totalTests
              << ", Passed: " << g_passedTests
              << ", Failed: " << (g_totalTests - g_passedTests) << std::endl;

    // Return non-zero if any test failed
    return (g_totalTests == g_passedTests) ? 0 : 1;
}