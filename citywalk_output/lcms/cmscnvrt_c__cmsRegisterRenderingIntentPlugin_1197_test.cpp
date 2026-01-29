// Lightweight C++11 unit test harness for cmsBool _cmsRegisterRenderingIntentPlugin
// This test provides a self-contained minimal environment (stubs) to exercise
// the focal method behaviour described in the prompt, without relying on GTest.
//
// Design notes:
// - Provides minimal stubs for dependencies used by _cmsRegisterRenderingIntentPlugin.
// - Tests cover: Data == NULL branch, successful insertion with linking to existing list,
//   and malloc failure scenario.
// - Uses non-terminating EXPECT_* style assertions to maximize execution coverage.
// - All code is self-contained in a single file for easy compilation under C++11.

#include <lcms2_internal.h>
#include <iostream>
#include <cstring>
#include <cstdlib>


// Domain-specific types and constants (simplified stubs for testing)
typedef int cmsBool;
typedef int cmsContext;
#define TRUE 1
#define FALSE 0

struct cmsPluginBase { int Dummy; }; // Base type (not used directly in tests, but required for the cast)

/* Forward declarations of structures used by the focal method */
struct cmsIntentsList;
struct cmsPluginRenderingIntent;

/* _cmsIntentsPluginChunkType holds the head of the Intents list for a context */
struct _cmsIntentsPluginChunkType {
    cmsIntentsList* Intents;
};

/* Linked list node representing an individual rendering intent descriptor */
struct cmsIntentsList {
    int Intent;
    char Description[256];
    char Link[256];
    cmsIntentsList* Next;
};

/* Rendering intent plugin data provided by the caller (derived from cmsPluginBase in real code) */
struct cmsPluginRenderingIntent {
    int Intent;
    char Description[256];
    char Link[256];
};

/* Context-level fake storage for plugin chunks (per-test singleton)
   We pretend there is a single "IntentPlugin" chunk for simplicity. */
static _cmsIntentsPluginChunkType g_intentsChunk;

/* Simulation controls for test scenarios */
static bool g_mallocShouldFail = false;

/* Stub: context->client chunk retrieval
   In real code, this would fetch a per-context chunk by type. We return our test chunk. */
static void* _cmsContextGetClientChunk(cmsContext id, int chunkType)
{
    (void)id; (void)chunkType;
    return &g_intentsChunk;
}

/* Stub: plugin malloc wrapper
   Can be configured to simulate allocation failure to cover the FALSE branch. */
static void* _cmsPluginMalloc(cmsContext id, size_t size)
{
    (void)id;
    if (g_mallocShouldFail) return NULL;
    return std::malloc(size);
}

/* The focal method under test (re-created here for self-contained testing) */
/* In the real project, this function exists in cmscnvrt.c. */
static cmsBool _cmsRegisterRenderingIntentPlugin(cmsContext id, cmsPluginBase* Data)
{
    {
        _cmsIntentsPluginChunkType* ctx = ( _cmsIntentsPluginChunkType*) _cmsContextGetClientChunk(id, 0 /* IntentPlugin */);
        cmsPluginRenderingIntent* Plugin = (cmsPluginRenderingIntent*) Data;
        cmsIntentsList* fl;
        // Do we have to reset the custom intents?
        if (Data == NULL) {
            ctx->Intents = NULL;
            return TRUE;
        }
        fl = (cmsIntentsList*) _cmsPluginMalloc(id, sizeof(cmsIntentsList));
        if (fl == NULL) return FALSE;

        fl ->Intent  = Plugin ->Intent;
        std::strncpy(fl ->Description, Plugin ->Description, sizeof(fl ->Description)-1);
        fl ->Description[sizeof(fl ->Description)-1] = 0;
        fl ->Link    = Plugin ->Link;
        fl ->Next = ctx ->Intents;
        ctx ->Intents = fl;
        return TRUE;
    }
}

/* ===================== Minimal Test Framework ===================== */

// Simple non-terminating EXPECT_* macros to preserve execution flow
static int gTestFailures = 0;

#define EXPECT_TRUE(cond) do { \
    if(!(cond)) { \
        std::cerr << "[EXPECT_TRUE FAILED] " #cond " at " __FILE__ ":" << __LINE__ << "\n"; \
        ++gTestFailures; \
    } \
} while(0)

#define EXPECT_FALSE(cond) EXPECT_TRUE(!(cond))

#define EXPECT_EQ(a, b) do { \
    if(!((a) == (b))) { \
        std::cerr << "[EXPECT_EQ FAILED] " #a " != " #b " : " << (a) << " != " << (b) \
                  << " at " __FILE__ ":" << __LINE__ << "\n"; \
        ++gTestFailures; \
    } \
} while(0)

#define EXPECT_PTR_EQ(a, b) do { \
    if((void*)(a) != (void*)(b)) { \
        std::cerr << "[EXPECT_PTR_EQ FAILED] " #a " != " #b \
                  << " : " << (void*)(a) << " vs " << (void*)(b) \
                  << " at " __FILE__ ":" << __LINE__ << "\n"; \
        ++gTestFailures; \
    } \
} while(0)

#define BEGIN_TEST() do { gTestFailures = 0; } while(0)
#define END_TEST(name) do { \
    if (gTestFailures == 0) { \
        std::cout << "[PASS] " << name << "\n"; \
    } else { \
        std::cout << "[FAIL] " << name << " (" << gTestFailures << " failures)\n"; \
    } \
} while(0)

/* Helpers to clean up allocated lists between tests */
static void freeIntentsList(cmsIntentsList* head)
{
    while (head) {
        cmsIntentsList* next = head->Next;
        std::free(head);
        head = next;
    }
}

/* Tests */

// Test 1: When Data is NULL, the function should reset Intents to NULL and return TRUE.
static void test_DataNullResetsIntents()
{
    BEGIN_TEST();

    // Setup: ensure there is an existing list to reset
    cmsIntentsList dummy;
    dummy.Intent = 11;
    std::strcpy(dummy.Description, "dummy");
    std::strcpy(dummy.Link, "dummy-link");
    dummy.Next = NULL;
    g_intentsChunk.Intents = &dummy; // non-NULL so we can observe the reset

    // Exercise
    cmsBool res = _cmsRegisterRenderingIntentPlugin(0, NULL);

    // Verify
    EXPECT_TRUE(res == TRUE);
    EXPECT_TRUE(g_intentsChunk.Intents == NULL);

    END_TEST("test_DataNullResetsIntents");
}

// Test 2: Successful insertion of a rendering intent with linking to existing list.
// Verifies all fields are copied and that Next points to previous head.
static void test_DataNonNullSuccessWithLinking()
{
    BEGIN_TEST();

    // Prepare environment: previous head is a real node (stack-allocated)
    cmsIntentsList existing;
    existing.Intent = 99;
    std::strcpy(existing.Description, "Existing");
    std::strcpy(existing.Link, "http://existing");
    existing.Next = NULL;
    g_intentsChunk.Intents = &existing;

    // Prepare Plugin data to insert
    cmsPluginRenderingIntent Plugin;
    Plugin.Intent = 42;
    std::strcpy(Plugin.Description, "Test Intent");
    std::strcpy(Plugin.Link, "http://example");

    // Exercise
    cmsBool res = _cmsRegisterRenderingIntentPlugin(0, (cmsPluginBase*) &Plugin);

    // Verify
    EXPECT_TRUE(res == TRUE);
    // New head should be created by malloc
    cmsIntentsList* newHead = g_intentsChunk.Intents;
    EXPECT_TRUE(newHead != NULL);
    EXPECT_EQ(newHead->Intent, 42);
    EXPECT_EQ(std::strcmp(newHead->Description, "Test Intent"), 0);
    EXPECT_EQ(std::strcmp(newHead->Link, "http://example"), 0);
    // Next should point to previous head (existing)
    EXPECT_PTR_EQ(newHead->Next, &existing);

    // Cleanup: free the newly allocated node (leaves existing on stack)
    freeIntentsList(newHead); // only frees the newly allocated head since Next points to existing
    g_intentsChunk.Intents = NULL;

    END_TEST("test_DataNonNullSuccessWithLinking");
}

// Test 3: Allocation failure should cause FALSE return and no List modification.
static void test_DataMallocFailure()
{
    BEGIN_TEST();

    // Ensure malloc will fail
    g_mallocShouldFail = true;

    // Prepare Plugin data
    cmsPluginRenderingIntent Plugin;
    Plugin.Intent = 7;
    std::strcpy(Plugin.Description, "WillFail");
    std::strcpy(Plugin.Link, "http://fail");

    // Exercise
    cmsBool res = _cmsRegisterRenderingIntentPlugin(0, (cmsPluginBase*) &Plugin);

    // Verify
    EXPECT_TRUE(res == FALSE);
    EXPECT_TRUE(g_intentsChunk.Intents == NULL);

    // Reset for other tests
    g_mallocShouldFail = false;

    END_TEST("test_DataMallocFailure");
}

/* Main entry: run tests and summarise */
int main()
{
    std::cout << "Running unit tests for _cmsRegisterRenderingIntentPlugin (self-contained stubs)\n";

    test_DataNullResetsIntents();
    test_DataNonNullSuccessWithLinking();
    test_DataMallocFailure();

    std::cout << "Finished tests.\n";
    return 0;
}