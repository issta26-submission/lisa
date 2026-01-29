// Unit tests for sqlite3_free_table implemented in C++11 without GoogleTest.
// The tests exercise the behavior of sqlite3_free_table as given in <FOCAL_METHOD>.
// The test harness uses non-terminating assertions and a minimal custom framework.

// Explanatory overview (Step 1-3):
// - Focus: sqlite3_free_table(char **azResult) which:
//     - Frees each non-null entry azResult[i] for i in [1, n-1], where n is read from
//       the header stored at azResult[0] after azResult--.
//     - Frees the azResult array itself.
//     - Safely does nothing if azResult is NULL.
// - Key dependencies and behaviors to cover:
//     - NULL input: no action.
//     - One non-null entry: only that entry and the array freed.
//     - Multiple non-null entries: all non-null entries freed plus the array.
//     - A mix of NULL and non-NULL entries to exercise inner if condition.
// - Candidate keywords guiding the tests: azResult, azResult--, SQLITE_PTR_TO_INT, n, i, sqlite3_free, sqlite3_free_table, non-null/NULL element freeing, memory allocation, freeing, NULL checks.

// The implementation below provides:
// - A minimal, self-contained reimplementation of sqlite3_free_table tailored for tests.
// - A lightweight memory-free tracker via a mock sqlite3_free to verify that the right
//   pointers are freed (both the individual strings and the azResult array).
// - A small test harness with non-terminating EXPECT-like assertions (via boolean checks),
//   no external test framework, and a main() that runs all tests in sequence.

#include <vector>
#include <cstdio>
#include <algorithm>
#include <string>
#include <cstring>
#include <sqliteInt.h>
#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <cassert>


using namespace std;

// ------------------------------------------------------------
// Mocked/free-tracker environment to simulate sqlite3_free
// and to observe which pointers were freed by sqlite3_free_table.
// ------------------------------------------------------------

static std::vector<void*> g_freed; // records addresses passed to sqlite3_free

extern "C" void sqlite3_free(void* p) {
    // Record the freed pointer and actually free memory to avoid leaks.
    g_freed.push_back(p);
    free(p);
}

// Helper to reset the freed-pointer tracker between tests
static void reset_freed() {
    g_freed.clear();
}

// Helper to check if a given pointer was freed (by value comparison)
static bool was_freed(void* p) {
    return std::find(g_freed.begin(), g_freed.end(), p) != g_freed.end();
}

// Define the SQLITE_PTR_TO_INT macro used by the focal method.
// We implement it as a safe cast to integer type that preserves the address.
// Using uintptr_t avoids truncation on 64-bit platforms.
#define SQLITE_PTR_TO_INT(p) (static_cast<int>(static_cast<std::uintptr_t>(p)))

// ------------------------------------------------------------
// Focal method under test: sqlite3_free_table
// The implementation mirrors the provided snippet (adjusted for test isolation).
// ------------------------------------------------------------

void sqlite3_free_table(char **azResult) {
    if( azResult ){
        int i, n;
        azResult--;
        assert( azResult!=0 );
        n = SQLITE_PTR_TO_INT(azResult[0]);
        for(i=1; i<n; i++){ if( azResult[i] ) sqlite3_free(azResult[i]); }
        sqlite3_free(azResult);
    }
}

// ------------------------------------------------------------
// Test harness: three tests plus a NULL input test
// Each test asserts expected freed pointers and overall behavior without aborting on failure.
// ------------------------------------------------------------

// Test 1: Basic case with one non-null entry and one NULL entry
// - n = 3
// - block layout (memory block): [ header=n, base[0], base[1], base[2] ]
//   We pass azResult = &block[1] to sqlite3_free_table.
//   Expected: Freed(base[1]) and Freed(block) (the azResult array base).
bool test_free_table_basic_single_nonnull() {
    reset_freed();

    // Prepare memory layout
    const int n = 3;
    char** block = (char**)malloc((n + 1) * sizeof(char*)); // block[0]..block[3]
    block[0] = (char*)n; // header: n
    char* s1 = strdup("row1"); // non-null to be freed
    block[1] = s1;
    block[2] = NULL;           // NULL should not be freed in the loop
    block[3] = NULL;           // unused (safe)

    // Call the focal method with azResult = block + 1
    sqlite3_free_table(block + 1);

    // Verify frees:
    // - s1 should have been freed
    // - block (the base pointer) should have been freed
    bool ok = true;
    if (!was_freed((void*)s1)) {
        std::cerr << "test_free_table_basic_single_nonnull: ERROR: base[1] not freed as expected.\n";
        ok = false;
    }
    if (!was_freed((void*)block)) {
        std::cerr << "test_free_table_basic_single_nonnull: ERROR: block base not freed as expected.\n";
        ok = false;
    }

    // Verify no extra frees for NULL entries (no extra non-null pointers were allocated)
    // Expect exactly 2 frees: s1 and block
    if ((int)g_freed.size() != 2) {
        std::cerr << "test_free_table_basic_single_nonnull: ERROR: Expected 2 frees, got " << g_freed.size() << ".\n";
        ok = false;
    }

    // Do not access block after this since it's freed; memory safety preserved by not dereferencing.
    return ok;
}

// Test 2: Multiple non-null entries
// - n = 4
// - block layout: [ header=n, base[1]=rowA, base[2]=rowB, base[3]=NULL ]
// - Expect frees for base[1], base[2], and block (the array)
bool test_free_table_multiple_nonnull() {
    reset_freed();

    const int n = 4;
    char** block = (char**)malloc((n + 1) * sizeof(char*));
    block[0] = (char*)n;
    char* s1 = strdup("rowA");
    char* s2 = strdup("rowB");
    block[1] = s1;
    block[2] = s2;
    block[3] = NULL;
    block[4] = NULL; // safety

    sqlite3_free_table(block + 1);

    bool ok = true;
    if (!was_freed((void*)s1)) {
        std::cerr << "test_free_table_multiple_nonnull: ERROR: base[1] (rowA) not freed.\n";
        ok = false;
    }
    if (!was_freed((void*)s2)) {
        std::cerr << "test_free_table_multiple_nonnull: ERROR: base[2] (rowB) not freed.\n";
        ok = false;
    }
    if (!was_freed((void*)block)) {
        std::cerr << "test_free_table_multiple_nonnull: ERROR: block base not freed.\n";
        ok = false;
    }

    if ((int)g_freed.size() != 3) {
        std::cerr << "test_free_table_multiple_nonnull: ERROR: Expected 3 frees, got " << g_freed.size() << ".\n";
        ok = false;
    }

    return ok;
}

// Test 3: All NULL entries (no per-entry frees, only the array)
bool test_free_table_all_null_entries() {
    reset_freed();

    const int n = 3;
    char** block = (char**)malloc((n + 1) * sizeof(char*));
    block[0] = (char*)n;
    block[1] = NULL;
    block[2] = NULL;
    block[3] = NULL;

    sqlite3_free_table(block + 1);

    bool ok = true;
    if (!g_freed.empty()) {
        std::cerr << "test_free_table_all_null_entries: ERROR: Expected no per-entry frees, but found " << g_freed.size() << " frees.\n";
        ok = false;
    }

    // The array should still be freed
    if ((int)g_freed.size() != 1) {
        std::cerr << "test_free_table_all_null_entries: ERROR: Expected 1 free (the block), got " << g_freed.size() << ".\n";
        ok = false;
    } else {
        // The only freed pointer should be the block base
        if (g_freed[0] != (void*)block) {
            std::cerr << "test_free_table_all_null_entries: ERROR: Freed pointer is not the block base as expected.\n";
            ok = false;
        }
    }

    return ok;
}

// Test 4: NULL azResult should be a no-op
bool test_free_table_null_input_no_op() {
    reset_freed();
    sqlite3_free_table(nullptr);

    if (!g_freed.empty()) {
        std::cerr << "test_free_table_null_input_no_op: ERROR: Frees occurred for NULL input.\n";
        return false;
    }
    return true;
}

// ------------------------------------------------------------
// Main: Run all tests and summarize results
// ------------------------------------------------------------

int main() {
    std::cout << "Running sqlite3_free_table unit tests (custom harness, no GTest)..." << std::endl;

    int total = 0;
    int passed = 0;

    auto run_case = [&](const char* name, bool (*test)()) {
        ++total;
        bool ok = test();
        if (ok) {
            ++passed;
            std::cout << "[PASS] " << name << std::endl;
        } else {
            std::cout << "[FAIL] " << name << std::endl;
        }
        // Separate test output for readability
        return ok;
    };

    // Execute tests
    bool t1 = run_case("test_free_table_basic_single_nonnull", test_free_table_basic_single_nonnull);
    bool t2 = run_case("test_free_table_multiple_nonnull", test_free_table_multiple_nonnull);
    bool t3 = run_case("test_free_table_all_null_entries", test_free_table_all_null_entries);
    bool t4 = run_case("test_free_table_null_input_no_op", test_free_table_null_input_no_op);

    // Final summary
    std::cout << "\nTest results: " << passed << " / " << total << " tests passed." << std::endl;
    return (passed == total) ? 0 : 1;
}