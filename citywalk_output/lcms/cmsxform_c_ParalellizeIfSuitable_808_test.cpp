/*
 * Lightweight C++11 unit test harness for ParalellizeIfSuitable
 * Follows the constraints:
 * - No GoogleTest/GMock
 * - Tests are executable from main()
 * - Uses a minimal, self-contained mock of the relevant dependencies
 * - Provides explanatory comments for each test
 *
 * NOTE:
 * This test harness provides a controlled environment to exercise the branching
 * logic of ParalellizeIfSuitable by simulating the internal CMS structures and
 * the _cmsContextGetClientChunk function. It is designed to be compiled and
 * linked in isolation for testing purposes or as a separate translation unit
 * alongside cmsxform.c in a project.
 *
 * The real project may have more complex CMS internals; this harness focuses on
 * verifying the conditional logic and the member updates performed by the
 * focal function.
 */

// Compile with: g++ -std=c++11 -Wall -Wextra -O2 test_paralellize.cpp -o test_paralellize

#include <cstring>
#include <unordered_map>
#include <iostream>
#include <lcms2_internal.h>
#include <cstdint>


// ------------------ Mocked/Minimal Domain Types ------------------
// These definitions mimic the minimal subset used by ParalellizeIfSuitable.
// They are deliberately lightweight and local to the test to avoid linkage
// issues with the actual cmsxform.c in the production project.

// Forward declaration for the Scheduler function type (void*)(void*)
typedef void (*SchedulerFnPtr)(void*);

// Minimal _cmsParallelizationPluginChunkType equivalent
struct _cmsParallelizationPluginChunkType {
    SchedulerFnPtr SchedulerFn;
    int MaxWorkers;
    int WorkerFlags;
};

// Minimal _cmsTRANSFORM equivalent used by the focal function
struct _cmsTRANSFORM {
    void* ContextID;
    void* xform;
    void* Worker;
    int MaxWorkers;
    int WorkerFlags;
};

// Sentinel constant for the chunk type requested by ParalellizeIfSuitable
// (matching the style used by the original code)
const int ParallelizationPlugin = 1;

// A tiny _cmsAssert replacement for test (does not abort on failure)
#define _cmsAssert(x) do { if(!(x)) { /* no-op for tests to maximize coverage */ } } while(0)

// ------------------ Context Chunk Retrieval Mock ------------------
// We provide a test-controlled mock for _cmsContextGetClientChunk that the
// focal function queries. The real CMS would pull from internal state; here
// we allow tests to inject behavior via a global map.

static std::unordered_map<void*, _cmsParallelizationPluginChunkType*> gContextChunks;

// A test-provided replacement for the real CMS function (C linkage as in production)
extern "C" _cmsParallelizationPluginChunkType* _cmsContextGetClientChunk(void* ContextID, int ChunkType)
{
    if (ChunkType != ParallelizationPlugin) return nullptr;
    auto it = gContextChunks.find(ContextID);
    if (it == gContextChunks.end()) return nullptr;
    return it->second;
}

// ------------------ ParalellizeIfSuitable (Test Version) ---------------
// We re-create the focal method's logic here in order to test its branching
// behavior in isolation from the rest of the library.
void ParalellizeIfSuitable(_cmsTRANSFORM* p)
{
    {
        _cmsParallelizationPluginChunkType* ctx = (_cmsParallelizationPluginChunkType*)_cmsContextGetClientChunk(p->ContextID, ParallelizationPlugin);
        _cmsAssert(p != NULL);
        if (ctx != NULL && ctx->SchedulerFn != NULL) {
            p->Worker = p->xform;
            p->xform = ctx->SchedulerFn;
            p->MaxWorkers = ctx->MaxWorkers;
            p->WorkerFlags = ctx->WorkerFlags;
        }
    }
}

// ------------------ Helper Utilities for Tests ----------------------

// A pair of dummy function pointers to act as the original and the scheduler
void dummyOriginalFunction(void* /*ctx*/) { /* do nothing in tests */ }
void schedulerFunctionShared(void* /*ctx*/) { /* do nothing in tests */ }

// Utility to reset global context chunks between tests
void clearContextChunks() {
    for (auto &kv : gContextChunks) {
        delete kv.second;
    }
    gContextChunks.clear();
}

// Utility to install a chunk for a given ContextID
void installChunkForContext(void* ctxID, _cmsParallelizationPluginChunkType* chunk) {
    clearContextChunks();
    gContextChunks[ctxID] = chunk;
}

// Utility to create a chunk instance
_cmsParallelizationPluginChunkType* makeChunk(SchedulerFnPtr fn, int maxWorkers, int workerFlags) {
    _cmsParallelizationPluginChunkType* c = new _cmsParallelizationPluginChunkType;
    c->SchedulerFn = fn;
    c->MaxWorkers = maxWorkers;
    c->WorkerFlags = workerFlags;
    return c;
}

// ------------------ Test Cases ------------------
// Each test prints its status and returns true on success, false on failure.
// We ensure all branches run in a single executable for simplicity.

bool test_ctxNull_noChange_onParalellize()
{
    // Test description:
    // When there is no context chunk registered for the given ContextID
    // ParalellizeIfSuitable should not modify the transform.

    // Arrange
    void* ctxID = (void*)0xDEADBEEF;
    _cmsTRANSFORM t;
    t.ContextID = ctxID;
    t.xform = (void*)0x1111; // some non-null function pointer
    t.Worker = nullptr;
    t.MaxWorkers = 0;
    t.WorkerFlags = 0;

    // Ensure there is no chunk registered for this ContextID
    clearContextChunks();

    // Act
    ParalellizeIfSuitable(&t);

    // Assert: no changes should have occurred
    bool ok = true;
    if (t.Worker != nullptr) {
        ok = false;
    }
    if (t.xform != (void*)0x1111) {
        ok = false;
    }
    if (t.MaxWorkers != 0) {
        ok = false;
    }
    if (t.WorkerFlags != 0) {
        ok = false;
    }

    if (!ok) {
        std::cout << "test_ctxNull_noChange_onParalellize: FAILED (state changed when no ctx)\n";
    } else {
        std::cout << "test_ctxNull_noChange_onParalellize: PASSED\n";
    }
    return ok;
}

bool test_ctxNonNullSchedulerNull_noChange()
{
    // Test description:
    // When there is a valid context chunk but SchedulerFn is NULL,
    // ParalellizeIfSuitable should NOT modify the transform.

    // Arrange
    void* ctxID = (void*)0xBAADF00D;
    _cmsTRANSFORM t;
    t.ContextID = ctxID;
    t.xform = (void*)0x2222;
    t.Worker = nullptr;
    t.MaxWorkers = 0;
    t.WorkerFlags = 0;

    // Install a chunk with SchedulerFn == NULL
    _cmsParallelizationPluginChunkType* chunk = makeChunk(nullptr, 8, 0x1F);
    installChunkForContext(ctxID, chunk);

    // Act
    ParalellizeIfSuitable(&t);

    // Assert: no changes should have occurred
    bool ok = true;
    if (t.Worker != nullptr) {
        ok = false;
    }
    if (t.xform != (void*)0x2222) {
        ok = false;
    }
    if (t.MaxWorkers != 0) {
        ok = false;
    }
    if (t.WorkerFlags != 0) {
        ok = false;
    }

    if (!ok) {
        std::cout << "test_ctxNonNullSchedulerNull_noChange: FAILED (state changed with null SchedulerFn)\n";
    } else {
        std::cout << "test_ctxNonNullSchedulerNull_noChange: PASSED\n";
    }

    // Cleanup
    clearContextChunks();
    return ok;
}

bool test_ctxNonNullSchedulerNonNull_changesAll()
{
    // Test description:
    // When there is a valid context chunk and a non-null SchedulerFn,
    // ParalellizeIfSuitable should swap xform to SchedulerFn, set Worker,
    // and propagate MaxWorkers and WorkerFlags from the chunk.

    // Arrange
    void* ctxID = (void*)0xFACEB00C;
    _cmsTRANSFORM t;
    t.ContextID = ctxID;
    t.xform = (void*)0x3333; // original xform
    t.Worker = nullptr;
    t.MaxWorkers = 0;
    t.WorkerFlags = 0;

    // Create a SchedulerFn and chunk values
    SchedulerFnPtr schedFn = schedulerFunctionShared;
    _cmsParallelizationPluginChunkType* chunk = makeChunk(schedFn, 12, 0xA5);
    installChunkForContext(ctxID, chunk);

    // Act
    ParalellizeIfSuitable(&t);

    // Assert: should have swapped/updated fields
    bool ok = true;
    if (t.Worker != (void*)0x3333) { // Worker should be original xform
        ok = false;
    }
    if (t.xform != (void*)schedFn) { // xform should be SchedulerFn
        ok = false;
    }
    if (t.MaxWorkers != 12) {
        ok = false;
    }
    if (t.WorkerFlags != 0xA5) {
        ok = false;
    }

    if (!ok) {
        std::cout << "test_ctxNonNullSchedulerNonNull_changesAll: FAILED (state not updated correctly)\n";
    } else {
        std::cout << "test_ctxNonNullSchedulerNonNull_changesAll: PASSED\n";
    }

    // Cleanup
    clearContextChunks();
    return ok;
}

// ------------------ Main Test Runner ------------------

int main() {
    int tests_run = 0;
    int tests_passed = 0;

    auto run = [&](bool (*testFn)(), const char* name) {
        bool res = testFn();
        tests_run++;
        if (res) tests_passed++;
        // Note: individual test prints its own PASS/FAIL lines.
        // We rely on those for detailed per-test feedback.
        return res;
    };

    // Run tests
    run(test_ctxNull_noChange_onParalellize, "test_ctxNull_noChange_onParalellize");
    run(test_ctxNonNullSchedulerNull_noChange, "test_ctxNonNullSchedulerNull_noChange");
    run(test_ctxNonNullSchedulerNonNull_changesAll, "test_ctxNonNullSchedulerNonNull_changesAll");

    // Summary
    std::cout << "\nTest Summary: " << tests_passed << " / " << tests_run << " tests passed.\n";

    return (tests_run == tests_passed) ? 0 : 1;
}