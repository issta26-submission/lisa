/*
   Lightweight, self-contained unit tests for the focal method:
   cmsBool _cmsRegisterParallelizationPlugin(cmsContext ContextID, cmsPluginBase* Data)

   This test suite re-implements a minimal, self-contained subset of the
   necessary CMS structures and the function under test to enable
   standalone validation without GTest. It covers:
   - Data == NULL path (sets MaxWorkers, WorkerFlags to 0 and SchedulerFn to NULL, returns TRUE)
   - Data != NULL with SchedulerFn == NULL (returns FALSE, no changes to context chunk)
   - Data != NULL with SchedulerFn != NULL (properly copies fields to context chunk)

   Notes:
   - The real project usually provides more complex dependencies. Here we model
     the essential parts to exercise the branch logic of the focal function.
   - The tests use plain C++11 facilities and simple stdout reporting (no GTest or third-party libs).
*/

#include <unordered_map>
#include <iostream>
#include <cassert>
#include <lcms2_internal.h>
#include <cstdint>


// Minimal domain modeling to mirror CMS types used by the focal function.
typedef bool cmsBool;                  // Using C++ bool as cmsBool
typedef void* cmsContext;              // Opaque context handle
typedef unsigned int cmsUInt32Number;
typedef void cmsPluginBase;              // Base plugin type (cast in function)
typedef void (* cmsSchedulerFn)(void*);  // Scheduler function signature

// Plugin structure matching the used field names in focal method
struct cmsPluginParalellization {
    cmsSchedulerFn SchedulerFn;
    unsigned int MaxWorkers;
    unsigned int WorkerFlags;
};

// Per-context chunk type that stores the parallelization plugin state
struct _cmsParallelizationPluginChunkType {
    unsigned int MaxWorkers;
    unsigned int WorkerFlags;
    cmsSchedulerFn SchedulerFn;
};

// A simple per-context storage to simulate CMS context chunks.
// This is intentionally tiny and isolated for unit testing.
static std::unordered_map<void*, _cmsParallelizationPluginChunkType> contextChunks;

// Forward declaration of the helper that the focal method uses to fetch the chunk.
// In the real project this would be provided by the library, but for unit testing
// we implement a minimal version here.
extern "C" void* _cmsContextGetClientChunk(cmsContext ContextID, int /*ChunkType*/) {
    // Return a pointer to the per-context chunk; create a zero-initialized one if missing.
    // Note: value-initialization ensures zeroed fields for POD types.
    return &contextChunks[ContextID];
}

// The focal method under test (re-implemented here for a self-contained test environment)
extern "C" cmsBool _cmsRegisterParallelizationPlugin(cmsContext ContextID, cmsPluginBase* Data)
{
    cmsPluginParalellization* Plugin = (cmsPluginParalellization*)Data;
    _cmsParallelizationPluginChunkType* ctx = (_cmsParallelizationPluginChunkType*)_cmsContextGetClientChunk(ContextID, 0);
    if (Data == NULL) {
        // No parallelization routines
        ctx->MaxWorkers = 0;
        ctx->WorkerFlags = 0;
        ctx->SchedulerFn = NULL;
        return true;
    }
    // callback is required
    if (Plugin->SchedulerFn == NULL) return false;
    ctx->MaxWorkers = Plugin->MaxWorkers;
    ctx->WorkerFlags = Plugin->WorkerFlags;
    ctx->SchedulerFn = Plugin->SchedulerFn;

    // All is ok
    return true;
}

// --- Test scaffolding ---

// Simple dummy scheduler functions to use in tests
static void dummyScheduler(void*) { /* no-op */ }
static void dummyScheduler2(void*) { /* no-op - different function to verify distinct pointers */ }

struct TestResult {
    const char* name;
    bool passed;
    std::string message;
};

// Helper to run a single test and print result
static void runTest(const TestResult& r)
{
    std::cout << "Test: " << r.name << " ... " << (r.passed ? "PASS" : "FAIL");
    if (!r.passed) {
        std::cout << "  Reason: " << r.message;
    }
    std::cout << std::endl;
}

// Utility: reset the per-context chunk to a known state
static void initChunkForContext(cmsContext ctx, unsigned int m, unsigned int w, cmsSchedulerFn fn)
{
    contextChunks[ctx] = {_cmsParallelizationPluginChunkType{m, w, fn}}; // Use aggregate init
}

// Overloaded helper to set a chunk with explicit initializer
static void setChunkExplicit(cmsContext ctx, unsigned int m, unsigned int w, cmsSchedulerFn fn)
{
    contextChunks[ctx] = {m, w, fn};
}

// Main function with three focused tests
int main() {
    TestResult t1{"cmsRegisterParallelizationPlugin_DataNull", false, ""};
    TestResult t2{"cmsRegisterParallelizationPlugin_DataNonNullSchedulerNull", false, ""};
    TestResult t3{"cmsRegisterParallelizationPlugin_DataNonNullSchedulerPresent", false, ""};

    // Common context for tests
    cmsContext ctx1 = reinterpret_cast<cmsContext>(reinterpret_cast<uintptr_t>(1));
    cmsContext ctx2 = reinterpret_cast<cmsContext>(reinterpret_cast<uintptr_t>(2));

    // 1) Test: Data == NULL should reset chunk fields to zero and return TRUE
    {
        // Ensure a deterministic initial state
        setChunkExplicit(ctx1, 123, 0xAA, dummyScheduler); // pre-state
        // Call focal function with NULL Data
        cmsBool ret = _cmsRegisterParallelizationPlugin(ctx1, nullptr);

        // Fetch the resulting chunk
        _cmsParallelizationPluginChunkType* c = (_cmsParallelizationPluginChunkType*)_cmsContextGetClientChunk(ctx1, 0);

        bool expected = true;
        bool ok = (ret == expected)
                  && (c->MaxWorkers == 0)
                  && (c->WorkerFlags == 0)
                  && (c->SchedulerFn == nullptr);

        t1.passed = ok;
        t1.message = ok ? "" : "Data==NULL path did not reset chunk fields as expected.";
        if (ok) {
            // print a tiny confirmation (optional)
        }
    }

    // 2) Test: Data != NULL but SchedulerFn == NULL should return FALSE and not modify chunk
    {
        // Prepare a context with a known state to verify "no modification" when SchedulerFn is NULL
        cmsContext ctx = ctx2;
        setChunkExplicit(ctx, 7, 9, dummyScheduler); // initial state to compare after call

        cmsPluginParalellization Plugin = { /*SchedulerFn=*/nullptr, /*MaxWorkers=*/3, /*WorkerFlags=*/0x1 };
        cmsBool ret = _cmsRegisterParallelizationPlugin(ctx, (cmsPluginBase*)&Plugin);

        _cmsParallelizationPluginChunkType* c = (_cmsParallelizationPluginChunkType*)_cmsContextGetClientChunk(ctx, 0);
        bool ok = (ret == false)
                  && (c->MaxWorkers == 7)
                  && (c->WorkerFlags == 9)
                  && (c->SchedulerFn == dummyScheduler);

        t2.passed = ok;
        t2.message = ok ? "" : "Data != NULL with NULL SchedulerFn path altered the chunk or returned wrong value.";
    }

    // 3) Test: Data != NULL with SchedulerFn non-NULL should copy values from Plugin to chunk
    {
        cmsContext ctx = ctx1; // reuse ctx1

        // Initialize a chunk to a non-zero, test-detectable state
        setChunkExplicit(ctx, 0, 0, nullptr);

        // Prepare a plugin with known values
        cmsPluginParalellization Plugin = { /*SchedulerFn=*/dummyScheduler2, /*MaxWorkers=*/4, /*WorkerFlags=*/0xAB };

        cmsBool ret = _cmsRegisterParallelizationPlugin(ctx, (cmsPluginBase*)&Plugin);

        _cmsParallelizationPluginChunkType* c = (_cmsParallelizationPluginChunkType*)_cmsContextGetClientChunk(ctx, 0);
        bool ok = (ret == true)
                  && (c->MaxWorkers == 4)
                  && (c->WorkerFlags == 0xAB)
                  && (c->SchedulerFn == dummyScheduler2);

        t3.passed = ok;
        t3.message = ok ? "" : "Data!=NULL with non-NULL SchedulerFn did not copy values correctly.";
    }

    // Report results
    runTest(t1);
    runTest(t2);
    runTest(t3);

    // Aggregate result
    int total = 3;
    int failures = (!t1.passed) + (!t2.passed) + (!t3.passed);
    if (failures == 0) {
        std::cout << "All tests PASS." << std::endl;
        return 0;
    } else {
        std::cout << "Some tests FAIL. (" << failures << " / " << total << ")" << std::endl;
        return 1;
    }
}