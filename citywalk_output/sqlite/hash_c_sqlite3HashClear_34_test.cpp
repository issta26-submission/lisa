// Minimal unit-test harness for sqlite3HashClear(Hash*) without using GTest.
// This harness is self-contained and relies only on C++11 standard library.
//
// What this test covers:
// - Clears an empty hash: ensures pH->first, pH->ht, pH->htsize, pH->count are reset
// - Clears a non-empty hash with a chain of HashElem: ensures all elements are freed
// - Verifies sqlite3_free is invoked for the ht pointer and for each element
// - Uses non-terminating assertions via a lightweight logging mechanism

#include <vector>
#include <assert.h>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <algorithm>


// Domain-relevant scaffolding to mimic the sqlite hash structures.
// We provide just enough to compile and exercise sqlite3HashClear.

struct HashElem {
    HashElem* next;
};

// Opaque hash table header placeholder (details are not used in the test)
struct _ht {};

// Hash structure as expected by sqlite3HashClear
struct Hash {
    HashElem* first;
    _ht* ht;
    unsigned int htsize;
    int count;
};

// Global memory-free tracker to validate that sqlite3_free is invoked for expected allocations.
static std::vector<void*> g_freed_ptrs;

// Simulated sqlite3_free that records freed pointers, then frees memory.
// Declared with C linkage to mimic the real sqlite3_free behavior and symbol resolution.
extern "C" void sqlite3_free(void *ptr) {
    // Record the pointer freed for verification.
    g_freed_ptrs.push_back(ptr);
    // Use the standard free to actually release memory.
    free(ptr);
}

// The focal method under test, copied from the provided snippet.
// It relies on sqlite3_free and operates on the Hash structure.
void sqlite3HashClear(Hash *pH){
  HashElem *elem;         /* For looping over all elements of the table */
  assert( pH!=0 );
  elem = pH->first;
  pH->first = 0;
  sqlite3_free(pH->ht);
  pH->ht = 0;
  pH->htsize = 0;
  while( elem ){
    HashElem *next_elem = elem->next;
    sqlite3_free(elem);
    elem = next_elem;
  }
  pH->count = 0;
}

// Lightweight test framework (no GTest/GMock). Each test returns true on success.
namespace TestFramework {
    struct TestCase {
        const char* name;
        bool (*fn)(std::vector<std::string>& logs);
    };

    // Utility to print test result summary
    static void printSummary(int total, int passed) {
        std::cout << "\nTest results: " << passed << " / " << total << " tests passed.\n";
    }

    // Utility to run all tests
    static int runAll(const std::vector<TestCase>& tests) {
        int total = (int)tests.size();
        int passed = 0;
        for (const auto& tc : tests) {
            std::vector<std::string> logs;
            bool ok = tc.fn(logs);
            if (ok) {
                ++passed;
            }
            // Print per-test details
            std::cout << "Test: " << tc.name << " -> " << (ok ? "PASSED" : "FAILED") << "\n";
            for (const auto& line : logs) {
                std::cout << "  " << line << "\n";
            }
        }
        printSummary(total, passed);
        return total == passed ? 0 : 1;
    }
}

// Helper utilities for tests
namespace Helpers {
    // Helper to create a Hash with an allocated ht
    static Hash* makeHashWithHt(unsigned int ht_size = 16) {
        Hash* pH = (Hash*)std::malloc(sizeof(Hash));
        pH->first = nullptr;
        pH->ht = ( _ht* )std::malloc(sizeof(_ht));
        pH->htsize = ht_size;
        pH->count = 0;
        return pH;
    }

    // Reset global memory tracker between tests
    static void resetFreedTracker() {
        g_freed_ptrs.clear();
    }

    // Helper to check if a pointer was freed (present in g_freed_ptrs)
    static bool wasFreed(void* ptr) {
        return std::find(g_freed_ptrs.begin(), g_freed_ptrs.end(), ptr) != g_freed_ptrs.end();
    }
}

// Test 1: Clear an empty Hash
// Rationale: Ensure that clearing resets the fields and frees the ht pointer without touching elements (none exist).
static bool test_ClearEmptyHash(std::vector<std::string>& logs) {
    using namespace Helpers;
    resetFreedTracker();

    // Setup: allocate a Hash with no elements
    Hash* pH = makeHashWithHt(32);
    void* htAddr = pH->ht; // capture address to verify it's freed

    pH->first = nullptr;
    pH->count = 5;  // non-zero to ensure clearing resets it

    // Exercise
    sqlite3HashClear(pH);

    // Verify post-conditions
    bool ok = true;
    if (pH->first != nullptr) {
        logs.push_back("Failure: pH->first should be null after Clear");
        ok = false;
    }
    if (pH->ht != nullptr) {
        logs.push_back("Failure: pH->ht should be null after Clear");
        ok = false;
    }
    if (pH->htsize != 0) {
        logs.push_back("Failure: pH->htsize should be 0 after Clear");
        ok = false;
    }
    if (pH->count != 0) {
        logs.push_back("Failure: pH->count should be 0 after Clear");
        ok = false;
    }

    // Verify memory was freed for ht
    if (!wasFreed(htAddr)) {
        logs.push_back("Failure: ht memory was not freed by sqlite3_free");
        ok = false;
    }

    // Cleanup: pH itself is not freed by sqlite3HashClear; free it here
    std::free(pH);

    return ok;
}

// Test 2: Clear a Hash containing multiple elements
// Rationale: Ensure that all elements in the linked list are freed and that the hash state is reset.
static bool test_ClearHashWithElements(std::vector<std::string>& logs) {
    using namespace Helpers;
    resetFreedTracker();

    // Setup: create a hash with 3 elements in a linked list
    Hash* pH = makeHashWithHt(64);

    // Allocate elements (simulate generic elements to be freed)
    HashElem* e3 = (HashElem*)std::malloc(sizeof(HashElem));
    e3->next = nullptr;
    HashElem* e2 = (HashElem*)std::malloc(sizeof(HashElem));
    e2->next = e3;
    HashElem* e1 = (HashElem*)std::malloc(sizeof(HashElem));
    e1->next = e2;

    pH->first = e1;
    pH->count = 3;
    void* htAddr = pH->ht;

    // Exercise
    sqlite3HashClear(pH);

    // Verify post-conditions
    bool ok = true;
    if (pH->first != nullptr) {
        logs.push_back("Failure: pH->first should be null after Clear");
        ok = false;
    }
    if (pH->ht != nullptr) {
        logs.push_back("Failure: pH->ht should be null after Clear");
        ok = false;
    }
    if (pH->htsize != 0) {
        logs.push_back("Failure: pH->htsize should be 0 after Clear");
        ok = false;
    }
    if (pH->count != 0) {
        logs.push_back("Failure: pH->count should be 0 after Clear");
        ok = false;
    }

    // Verify that ht was freed and each element was freed
    if (!wasFreed(htAddr)) {
        logs.push_back("Failure: ht memory was not freed by sqlite3_free (element test)");
        ok = false;
    }
    if (!wasFreed(e1) || !wasFreed(e2) || !wasFreed(e3)) {
        logs.push_back("Failure: One or more HashElem nodes were not freed by sqlite3_free");
        ok = false;
    }

    // Cleanup: free the Hash structure
    std::free(pH);

    return ok;
}

int main() {
    // Assemble test suite
    using namespace TestFramework;
    std::vector<TestCase> tests = {
        { "sqlite3HashClear_ClearEmptyHash", test_ClearEmptyHash },
        { "sqlite3HashClear_ClearHashWithElements", test_ClearHashWithElements }
    };

    // Run tests
    int result = runAll(tests);

    // Return non-zero if any test failed (to integrate with simple CI)
    return result;
}