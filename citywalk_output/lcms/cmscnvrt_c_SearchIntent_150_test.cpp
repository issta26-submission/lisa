// Custom test suite for the focal method: SearchIntent
// This test harness is designed to be compiled together with cmscnvrt.c
// under C++11 without using Google Test (GTest). It uses a lightweight
// in-file test runner with non-terminating assertions to maximize
// code coverage.
//
// The tests mock the dependency _cmsContextGetClientChunk and the global
// DefaultIntents to exercise the SearchIntent logic in isolation.

#include <vector>
#include <lcms2_internal.h>
#include <iostream>


//-------------------------------------------------------------------------------------------------
// Minimal type definitions adapted to match the usage inside cmsIntentsList and SearchIntent.
// We provide only the pieces required by the focal method to compile and run in tests.
//-------------------------------------------------------------------------------------------------

typedef void* cmsContext;
typedef unsigned int cmsUInt32Number;

// Linked list node representing an intent entry.
typedef struct cmsIntentsList {
    cmsUInt32Number Intent;
    struct cmsIntentsList* Next;
} cmsIntentsList;

// Plugin chunk type that (through the real library) holds a list of intents for a context.
typedef struct _cmsIntentsPluginChunkType {
    cmsIntentsList* Intents;
} _cmsIntentsPluginChunkType;

// Forward declaration of the focal method (as it would be seen in cmscnvrt.c).
extern "C" cmsIntentsList* SearchIntent(cmsContext ContextID, cmsUInt32Number Intent);

//-----------------------------------------------------------------------------------------------
// Mocked dependency and test scaffolding
// We provide a test-scoped implementation of _cmsContextGetClientChunk and a
// testable DefaultIntents global to drive SearchIntent's logic.
//-----------------------------------------------------------------------------------------------

// Global mock: the current client chunk returned by _cmsContextGetClientChunk.
static _cmsIntentsPluginChunkType* g_CurrentChunk = nullptr;

// Global mock: DefaultIntents for the library code to consult when not found in the
// plugin chunk's Intents list.
static cmsIntentsList* DefaultIntents = nullptr;

// Mocked function that mirrors the library's internal function used by SearchIntent.
// This is provided in the test executable so the focal method can be exercised in isolation.
extern "C" _cmsIntentsPluginChunkType* _cmsContextGetClientChunk(cmsContext ContextID, cmsUInt32Number Plugin)
{
    (void)ContextID; (void)Plugin; // Suppress unused parameter warnings in tests
    return g_CurrentChunk;
}

// Helper: allocate a singly linked list from a vector of intents and return the head pointer.
static cmsIntentsList* buildList(const std::vector<cmsUInt32Number>& intents)
{
    cmsIntentsList* head = nullptr;
    cmsIntentsList** tail = &head;
    for (cmsUInt32Number v : intents) {
        cmsIntentsList* node = new cmsIntentsList{v, nullptr};
        *tail = node;
        tail = &node->Next;
    }
    return head;
}

// Helper: recursively free a linked list.
static void freeList(cmsIntentsList* head)
{
    while (head) {
        cmsIntentsList* n = head->Next;
        delete head;
        head = n;
    }
}

// Simple non-terminating assertion framework
static int g_total = 0;
static int g_passed = 0;

#define ASSERT_TRUE(cond, msg) do {                                           \
    ++g_total;                                                                 \
    if (cond) {                                                                \
        ++g_passed;                                                            \
    } else {                                                                   \
        std::cerr << "ASSERT FAILED: " << msg << std::endl;                   \
    }                                                                          \
} while (0)

#define ASSERT_PTR_EQ(a, b, msg) do {                                          \
    ++g_total;                                                                 \
    if ((a) == (b)) {                                                          \
        ++g_passed;                                                            \
    } else {                                                                   \
        std::cerr << "ASSERT FAILED: " << msg << " (expected "                 \
                  << (void*)(b) << " got " << (void*)(a) << ")" << std::endl; \
    }                                                                          \
} while (0)


//-------------------------------------------------------------------------------------------------
// Test 1: Found in ctx.Intents (first node is the match)
// Expected: SearchIntent returns pointer to the matching node from ctx.Intents.
//-------------------------------------------------------------------------------------------------
static bool test_SearchIntent_FoundInCtx_FirstMatch()
{
    // Prepare context chunk with two nodes; first matches the requested Intent.
    cmsIntentsList* node2 = new cmsIntentsList{ 7, nullptr };
    cmsIntentsList* node1 = new cmsIntentsList{ 42, node2 };

    _cmsIntentsPluginChunkType chunk;
    chunk.Intents = node1;

    g_CurrentChunk = &chunk;
    DefaultIntents = nullptr;

    cmsIntentsList* res = SearchIntent(nullptr, 42);

    // Expectation: res should be node1
    ASSERT_PTR_EQ(res, node1, "SearchIntent should return first matching node from ctx.Intents");

    // Cleanup
    freeList(node1); // frees node1 and node2
    chunk.Intents = nullptr;
    g_CurrentChunk = nullptr;
    return true;
}

//-------------------------------------------------------------------------------------------------
// Test 2: Not found in ctx.Intents but found in DefaultIntents
// Expected: SearchIntent returns the matching node from DefaultIntents.
//-------------------------------------------------------------------------------------------------
static bool test_SearchIntent_NotInCtxButInDefault()
{
    // ctx has no intents
    _cmsIntentsPluginChunkType chunk;
    chunk.Intents = nullptr;

    // DefaultIntents contains one matching node
    cmsIntentsList* def = new cmsIntentsList{ 77, nullptr };

    // Apply setup
    g_CurrentChunk = &chunk;
    DefaultIntents = def;

    cmsIntentsList* res = SearchIntent(nullptr, 77);

    ASSERT_PTR_EQ(res, def, "SearchIntent should return matching node from DefaultIntents when not found in ctx");

    // Cleanup
    delete def;
    chunk.Intents = nullptr;
    g_CurrentChunk = nullptr;
    DefaultIntents = nullptr;
    return true;
}

//-------------------------------------------------------------------------------------------------
// Test 3: Not found anywhere (NULL is returned)
//-------------------------------------------------------------------------------------------------
static bool test_SearchIntent_NotFoundAnywhere()
{
    // ctx has no intents; DefaultIntents also empty
    _cmsIntentsPluginChunkType chunk;
    chunk.Intents = nullptr;

    g_CurrentChunk = &chunk;
    DefaultIntents = nullptr;

    cmsIntentsList* res = SearchIntent(nullptr, 999);

    ASSERT_TRUE(res == nullptr, "SearchIntent should return NULL when Intent is not present anywhere");

    // Cleanup
    chunk.Intents = nullptr;
    g_CurrentChunk = nullptr;
    DefaultIntents = nullptr;
    return true;
}

//-------------------------------------------------------------------------------------------------
// Test 4: Found in DefaultIntents even when multiple non-matching nodes exist in ctx
//-------------------------------------------------------------------------------------------------
static bool test_SearchIntent_FoundInDefaultWithMultipleCtxNodes()
{
    // Context has two nodes, none match
    cmsIntentsList* a = new cmsIntentsList{ 11, nullptr };
    cmsIntentsList* b = new cmsIntentsList{ 12, a };
    a->Next = nullptr;

    _cmsIntentsPluginChunkType chunk;
    chunk.Intents = b; // Intent 12, not our target 99

    g_CurrentChunk = &chunk;

    // Default list contains a matching node
    cmsIntentsList* def = new cmsIntentsList{ 99, nullptr };
    DefaultIntents = def;

    cmsIntentsList* res = SearchIntent(nullptr, 99);

    ASSERT_PTR_EQ(res, def, "SearchIntent should return matching node from DefaultIntents when not found in ctx");

    // Cleanup
    freeList(a);
    freeList(def);
    chunk.Intents = nullptr;
    g_CurrentChunk = nullptr;
    DefaultIntents = nullptr;
    return true;
}

//-------------------------------------------------------------------------------------------------
// Test runner and main
//-------------------------------------------------------------------------------------------------
int main()
{
    std::cout << "Running tests for SearchIntent (cmsIntentsList) ..." << std::endl;

    // Run tests
    if (!test_SearchIntent_FoundInCtx_FirstMatch()) {
        std::cerr << "Test 1 reported failure (unexpected early exit)." << std::endl;
    }
    if (!test_SearchIntent_NotInCtxButInDefault()) {
        std::cerr << "Test 2 reported failure." << std::endl;
    }
    if (!test_SearchIntent_NotFoundAnywhere()) {
        std::cerr << "Test 3 reported failure." << std::endl;
    }
    if (!test_SearchIntent_FoundInDefaultWithMultipleCtxNodes()) {
        std::cerr << "Test 4 reported failure." << std::endl;
    }

    // Summary
    std::cout << "Tests completed. Passed "
              << g_passed << " / " << g_total << " checks." << std::endl;

    return (g_total == g_passed) ? 0 : 1;
}