// Test suite for sqlite3IdListIndex(IdList *pList, const char *zName)
// This test harness is written in C++11 without Google Test.
// It relies on the existing sqliteInt.h / sqlite3StrICmp declarations from the
// FOCAL_CLASS_DEP section. We use non-terminating EXPECT-like assertions to
// maximize code coverage and execution flow.

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Bring in the SQLite internal declarations.
// The focal class dependencies expose IdList and sqlite3StrICmp via sqliteInt.h.
extern "C" {
}

// Local testing harness: a minimal non-terminating assertion facility.
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_TRUE(cond, msg) do { \
    ++g_tests_run; \
    if(!(cond)) { \
        std::cerr << "Test failed: " << __FILE__ << ":" << __LINE__ << " - " << (msg) \
                  << "\n    Condition: " #cond "\n"; \
        ++g_tests_failed; \
    } \
} while(0)

// Helper: allocate and populate an IdList with given names.
// Assumes IdList is defined with a flexible array member a[1] containing elements
// that have a member zName (char*). We allocate enough memory for n elements.
static IdList* makeIdList(int n, const char** names) {
    if(n <= 0) return nullptr;
    // Allocate memory for IdList with space for n elements in a[]
    IdList* p = (IdList*)malloc(sizeof(IdList) + (size_t)(n - 1) * sizeof(p->a[0]));
    if(!p) return nullptr;
    p->nId = n;
    for(int i = 0; i < n; ++i) {
        // Duplicate strings to mimic realistic IdList contents
        p->a[i].zName = strdup(names[i]);
    }
    return p;
}

// Helper: free an IdList allocated by makeIdList
static void freeIdList(IdList* p) {
    if(!p) return;
    for(int i = 0; i < p->nId; ++i) {
        free(p->a[i].zName);
    }
    free(p);
}

// Test 1: Basic case - match at first element
// Verifies that sqlite3IdListIndex returns 0 when zName matches the first entry.
static void test_sqlite3IdListIndex_basic_match_first() {
    const char* names[] = { "Alice", "Bob", "Charlie" };
    IdList* list = makeIdList(3, names);
    EXPECT_TRUE(list != nullptr, "List allocation failed in test_sqlite3IdListIndex_basic_match_first");

    int idx = sqlite3IdListIndex(list, "Alice");
    EXPECT_TRUE(idx == 0,
        "Expected index 0 for first element match in test_sqlite3IdListIndex_basic_match_first");

    freeIdList(list);
}

// Test 2: Basic case - match in the middle
// Verifies that sqlite3IdListIndex returns the correct middle index.
static void test_sqlite3IdListIndex_match_middle() {
    const char* names[] = { "Alice", "Bob", "Charlie", "David" };
    IdList* list = makeIdList(4, names);
    EXPECT_TRUE(list != nullptr, "List allocation failed in test_sqlite3IdListIndex_match_middle");

    int idx = sqlite3IdListIndex(list, "Charlie");
    EXPECT_TRUE(idx == 2,
        "Expected index 2 for the middle element in test_sqlite3IdListIndex_match_middle");

    freeIdList(list);
}

// Test 3: Not found - ensure -1 is returned for absent name
static void test_sqlite3IdListIndex_not_found() {
    const char* names[] = { "Alpha", "Beta", "Gamma" };
    IdList* list = makeIdList(3, names);
    EXPECT_TRUE(list != nullptr, "List allocation failed in test_sqlite3IdListIndex_not_found");

    int idx = sqlite3IdListIndex(list, "Delta");
    EXPECT_TRUE(idx == -1,
        "Expected -1 for non-existent element in test_sqlite3IdListIndex_not_found");

    freeIdList(list);
}

// Test 4: Case-insensitive matching - verify sqlite3StrICmp behavior via function under test
// Ensure that searching with a different case still finds a match.
static void test_sqlite3IdListIndex_case_insensitive() {
    const char* names[] = { "epsilon", "zeta", "eta" };
    IdList* list = makeIdList(3, names);
    EXPECT_TRUE(list != nullptr, "List allocation failed in test_sqlite3IdListIndex_case_insensitive");

    // Search with different case to trigger case-insensitive comparison
    int idx = sqlite3IdListIndex(list, "EPSILON");
    EXPECT_TRUE(idx == 0,
        "Expected index 0 for case-insensitive match in test_sqlite3IdListIndex_case_insensitive");

    freeIdList(list);
}

// Test 5: Large list - ensure performance/behavior scales and last element is found
static void test_sqlite3IdListIndex_large_last() {
    const int N = 10;
    const char* names[N] = {
        "name0","name1","name2","name3","name4",
        "name5","name6","name7","name8","name9"
    };
    IdList* list = makeIdList(N, names);
    EXPECT_TRUE(list != nullptr, "List allocation failed in test_sqlite3IdListIndex_large_last");

    int idx = sqlite3IdListIndex(list, "name9");
    EXPECT_TRUE(idx == 9,
        "Expected index 9 for last element in test_sqlite3IdListIndex_large_last");

    freeIdList(list);
}

// Main: run all tests and report results
int main() {
    test_sqlite3IdListIndex_basic_match_first();
    test_sqlite3IdListIndex_match_middle();
    test_sqlite3IdListIndex_not_found();
    test_sqlite3IdListIndex_case_insensitive();
    test_sqlite3IdListIndex_large_last();

    std::cout << "Tests run: " << g_tests_run
              << "  Failures: " << g_tests_failed << std::endl;

    // Return non-zero if any test failed
    return g_tests_failed ? 1 : 0;
}