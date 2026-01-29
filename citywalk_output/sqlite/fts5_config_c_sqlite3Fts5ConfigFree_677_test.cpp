// C++11 test harness for sqlite3Fts5ConfigFree (from fts5_config.c)
// This suite uses a small, self-contained test framework (no Google Test or GMock).
// It includes a lightweight sqlite3_free mock and verifies correct freeing behavior
// including both branches of the pTok conditional and proper cleanup of all fields.

#include <vector>
#include <fts5Int.h>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>


// Include the focal header which defines Fts5Config and related types.
// The project under test is assumed to provide these headers in the expected paths.

// Forward declaration of the focal function
extern "C" void sqlite3Fts5ConfigFree(Fts5Config *pConfig);

// Global state for test verification
static std::vector<void*> g_freed_ptrs;
static bool g_tokApiXDeleteCalled;

// Mock implementation of sqlite3_free to track memory deallocation
extern "C" void sqlite3_free(void* p){
    // Track freed pointers for assertion checks
    g_freed_ptrs.push_back(p);
    // Perform actual free to avoid leaks
    free(p);
}

// Mock tokenizer delete function to verify correct branch execution
extern "C" void mock_xDelete(void* pTok){
    // Mark that the xDelete has been invoked when pTok is non-null
    g_tokApiXDeleteCalled = true;
}

// Utility: reset test harness state
static void test_reset_state(){
    g_freed_ptrs.clear();
    g_tokApiXDeleteCalled = false;
}

// Utility: whether a pointer is contained in the Freed list
static bool freed_list_contains(void* p){
    return std::find(g_freed_ptrs.begin(), g_freed_ptrs.end(), p) != g_freed_ptrs.end();
}

// Lightweight assertion macros (non-terminating)
static int g_test_failures = 0;
#define EXPECT_TRUE(cond) do { \
    if(!(cond)){ \
        printf("EXPECT_TRUE failed at %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        ++g_test_failures; \
    } \
} while(0)

#define EXPECT_FALSE(cond) do { \
    if((cond)){ \
        printf("EXPECT_FALSE failed at %s:%d: %s\n", __FILE__, __LINE__, #cond); \
        ++g_test_failures; \
    } \
} while(0)

#define EXPECT_EQ(a, b) do { \
    if((a) != (b)){ \
        printf("EXPECT_EQ failed at %s:%d: %s == %lld, actual %lld\n", __FILE__, __LINE__, #a, (long long)(b), (long long)(a)); \
        ++g_test_failures; \
    } \
} while(0)

#define TEST_PASSED(msg) do { \
    if(g_test_failures == 0){ \
        printf("PASS: %s\n", msg); \
    } else { \
        printf("FAIL: %s with %d failure(s)\n", msg, g_test_failures); \
    } \
} while(0)

// Test 1: Free should do nothing for NULL input
static void test_free_null_input(){
    test_reset_state();

    // Call the focal function with NULL; expect no frees to occur
    sqlite3Fts5ConfigFree(NULL);

    EXPECT_EQ((int)g_freed_ptrs.size(), 0); // no frees performed
    TEST_PASSED("sqlite3Fts5ConfigFree(NULL) performs no action");
}

// Test 2: Free a fully-populated Fts5Config where pTok is non-NULL
static void test_free_full_with_tok_nonnull(){
    test_reset_state();

    // Allocate and populate a Fts5Config instance
    Fts5Config *p = (Fts5Config*) malloc(sizeof(Fts5Config));
    // Non-NULL pTok to exercise the true branch
    p->pTok = malloc(4);
    // Allocate a dummy pTokApi with xDelete function
    p->pTokApi = (Fts5TokApi*) malloc(sizeof(Fts5TokApi));
    p->pTokApi->xDelete = mock_xDelete; // hook to verify call
    // Ensure the xDelete would be invoked
    // Note: The actual structure names are assumed to be defined in fts5Int.h

    // Populate string and array fields (all allocated via malloc to track frees)
    p->zDb = (char*) malloc(3); strcpy(p->zDb, "db");
    p->zName = (char*) malloc(6); strcpy(p->zName, "name");

    p->nCol = 2;
    p->azCol = (char**) malloc(p->nCol * sizeof(char*));
    p->azCol[0] = (char*) malloc(6); strcpy(p->azCol[0], "col0");
    p->azCol[1] = (char*) malloc(6); strcpy(p->azCol[1], "col1");

    p->aPrefix = (char*) malloc(4); strcpy(p->aPrefix, "pre");

    p->zRank = (char*) malloc(4); strcpy(p->zRank, "rnk");
    p->zRankArgs = (char*) malloc(4); strcpy(p->zRankArgs, "arg");

    p->zContent = (char*) malloc(5); strcpy(p->zContent, "cont");
    p->zContentRowid = (char*) malloc(7); strcpy(p->zContentRowid, "rowid");
    p->zContentExprlist = (char*) malloc(8); strcpy(p->zContentExprlist, "exprlst");

    // Call the function under test
    sqlite3Fts5ConfigFree(p);

    // Verify that pTokApi.xDelete was invoked
    EXPECT_TRUE(g_tokApiXDeleteCalled);
    // Verify that all non-NULL allocated pointers were freed
    EXPECT_TRUE(freed_list_contains(p->pTok));           // pTok
    EXPECT_TRUE(freed_list_contains(p->zDb));            // zDb
    EXPECT_TRUE(freed_list_contains(p->zName));           // zName
    EXPECT_TRUE(freed_list_contains(p->azCol[0]));        // azCol[0]
    EXPECT_TRUE(freed_list_contains(p->azCol[1]));        // azCol[1]
    EXPECT_TRUE(freed_list_contains(p->azCol));           // azCol array
    EXPECT_TRUE(freed_list_contains(p->aPrefix));         // aPrefix
    EXPECT_TRUE(freed_list_contains(p->zRank));           // zRank
    EXPECT_TRUE(freed_list_contains(p->zRankArgs));       // zRankArgs
    EXPECT_TRUE(freed_list_contains(p->zContent));        // zContent
    EXPECT_TRUE(freed_list_contains(p->zContentRowid));   // zContentRowid
    EXPECT_TRUE(freed_list_contains(p->zContentExprlist)); // zContentExprlist
    EXPECT_TRUE(freed_list_contains(p));                  // pConfig itself

    // Clean up: p should be freed by sqlite3_free; no further action needed

    TEST_PASSED("sqlite3Fts5ConfigFree frees all allocated members when pTok is non-NULL and calls xDelete");
}

// Test 3: Free with pTok == NULL should skip xDelete and still free other fields
static void test_free_tok_null_branch(){
    test_reset_state();

    // Allocate and populate a Fts5Config instance with pTok = NULL
    Fts5Config *p = (Fts5Config*) malloc(sizeof(Fts5Config));
    p->pTok = NULL;
    p->pTokApi = NULL; // not used when pTok is NULL

    // Minimal non-NULL allocations for other fields
    p->zDb = (char*) malloc(3); strcpy(p->zDb, "db");
    p->zName = (char*) malloc(6); strcpy(p->zName, "name");
    p->nCol = 0; // zero columns to exercise loop edge
    p->azCol = NULL;
    p->aPrefix = (char*) malloc(3); strcpy(p->aPrefix, "pfx");
    p->zRank = (char*) malloc(3); strcpy(p->zRank, "rk");
    p->zRankArgs = (char*) malloc(3); strcpy(p->zRankArgs, "ra");
    p->zContent = (char*) malloc(4); strcpy(p->zContent, "cont");
    p->zContentRowid = (char*) malloc(5); strcpy(p->zContentRowid, "row");
    p->zContentExprlist = (char*) malloc(6); strcpy(p->zContentExprlist, "expr");

    // Call the function under test
    sqlite3Fts5ConfigFree(p);

    // xDelete should not have been called
    EXPECT_FALSE(g_tokApiXDeleteCalled);

    // All non-NULL allocated pointers should have been freed
    EXPECT_TRUE(freed_list_contains(p->zDb));
    EXPECT_TRUE(freed_list_contains(p->zName));
    EXPECT_TRUE(freed_list_contains(p->aPrefix));
    EXPECT_TRUE(freed_list_contains(p->zRank));
    EXPECT_TRUE(freed_list_contains(p->zRankArgs));
    EXPECT_TRUE(freed_list_contains(p->zContent));
    EXPECT_TRUE(freed_list_contains(p->zContentRowid));
    EXPECT_TRUE(freed_list_contains(p->zContentExprlist));
    EXPECT_TRUE(freed_list_contains(p)); // pConfig itself

    TEST_PASSED("sqlite3Fts5ConfigFree frees fields when pTok is NULL and does not call xDelete");
}

// Runner
int main(){
    printf("Starting sqlite3Fts5ConfigFree test suite (C++11 harness, non-GTest)\n");

    test_free_null_input();
    test_free_full_with_tok_nonnull();
    test_free_tok_null_branch();

    // Summary
    if(g_test_failures == 0){
        printf("All tests completed: PASS\n");
        return 0;
    } else {
        printf("Tests completed with %d failure(s)\n", g_test_failures);
        return 1;
    }
}