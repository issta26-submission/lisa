// Test suite for sqlite3Fts5ParseNearsetFree
// This test suite is designed for C++11 without GoogleTest.
// It exercises the focal function by simulating frees and observing
// which pointers are handed to sqlite3_free via a custom mock.

// Include needed headers from the FTS5 codebase to get proper type layouts.
#include <vector>
#include <fts5Int.h>
#include <fts5parse.h>
#include <stdio.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Import C headers under C linkage to obtain complete type definitions.
// This assumes the headers are available in the include path.
extern "C" {
}

// Forward declarations for the function under test (provided by fts5_expr.c)
extern "C" void sqlite3Fts5ParseNearsetFree(Fts5Parse *pParse, Fts5ExprNearset *pNear, Fts5ExprPhrase *pPhrase);

// Mocking sqlite3_free to observe which pointers are freed by sqlite3Fts5ParseNearsetFree.
// We intentionally do not free memory here to avoid double frees in test.
// The real sqlite3_free would release memory; our mock records the freed addresses.
static std::vector<void*> g_freed_ptrs;
extern "C" void sqlite3_free(void* p) {
    if(p == nullptr) return;
    g_freed_ptrs.push_back(p);
    // Do not call free to avoid interfering with test heap management.
}

// Simple non-terminating test harness
static int g_tests_run = 0;
static int g_tests_failed = 0;

#define EXPECT_EQ(a, b, msg) do { if((a) != (b)) { \
    std::cerr << "EXPECT_EQ FAILED: " << (msg) \
              << " | expected: " << (b) << ", got: " << (a) << "\n"; \
    ++g_tests_failed; } } while(0)

#define EXPECT_PTR_EQ(a, b, msg) do { if((void*)(a) != (void*)(b)) { \
    std::cerr << "EXPECT_PTR_EQ FAILED: " << (msg) \
              << " | expected: " << (void*)(b) << ", got: " << (void*)(a) << "\n"; \
    ++g_tests_failed; } } while(0)

#define EXPECT_TRUE(x, msg) do { if(!(x)) { \
    std::cerr << "EXPECT_TRUE FAILED: " << (msg) << "\n"; \
    ++g_tests_failed; } } while(0)

#define RUN_TEST(fn) do { ++g_tests_run; fn(); } while(0)

// Test 1: When pNear is null, sqlite3Fts5ParseNearsetFree should do nothing (no frees).
void test_null_pNear_no_free() {
    // Clear any prior frees
    g_freed_ptrs.clear();

    // Call with null pNear
    sqlite3Fts5ParseNearsetFree(nullptr, nullptr, nullptr);

    // Expect no frees occurred
    EXPECT_EQ(static_cast<int>(g_freed_ptrs.size()), 0, "Free should not be called when pNear is NULL");
}

// Test 2: When pNear is non-null with a non-empty apPhrase array and non-null pColset,
// sqlite3Fts5ParseNearsetFree should free pColset and pNear (in that order in the implementation),
// while fts5ExprPhraseFree is a static/no-op in the focal file.
void test_nearset_free_frees_pColset_and_near() {
    g_freed_ptrs.clear();

    // Allocate a nearset on the heap and populate fields
    Fts5ExprNearset *pNear = (Fts5ExprNearset*)malloc(sizeof(Fts5ExprNearset));
    pNear->nPhrase = 3;
    pNear->apPhrase = (Fts5ExprPhrase**)malloc(sizeof(Fts5ExprPhrase*) * pNear->nPhrase);
    pNear->apPhrase[0] = nullptr;
    pNear->apPhrase[1] = (Fts5ExprPhrase*)0x1234; // non-null junk pointer to simulate a phrase
    pNear->apPhrase[2] = nullptr;
    pNear->pColset = (Fts5Colset*)malloc(sizeof(Fts5Colset)); // non-null colset

    // Call the function under test
    sqlite3Fts5ParseNearsetFree(nullptr, pNear, nullptr);

    // Verify that exactly two pointers were freed: pNear->pColset and pNear
    bool freed_pColset = false;
    bool freed_pNear = false;
    for(void* p : g_freed_ptrs){
        if(p == pNear->pColset) freed_pColset = true;
        if(p == pNear) freed_pNear = true;
    }

    EXPECT_TRUE(freed_pColset, "pColset should be freed by sqlite3_free");
    EXPECT_TRUE(freed_pNear, "pNear should be freed by sqlite3_free");
    EXPECT_EQ(static_cast<int>(g_freed_ptrs.size()), 2, "Exactly two pointers should be freed (pColset and pNear)");

    // Cleanup allocated memory (avoid leaking in test harness)
    // Note: We do not free via sqlite3_free to avoid double free; we manage directly.
    free(pNear->pColset);
    free(pNear);
    free(pNear->apPhrase);
}

// Test 3: When pNear is non-null but pColset is NULL, sqlite3Fts5ParseNearsetFree should free only pNear.
// This ensures proper handling of NULL pointers in the freeing sequence.
void test_nearset_free_with_null_pColset_only_near() {
    g_freed_ptrs.clear();

    // Allocate a nearset on the heap
    Fts5ExprNearset *pNear = (Fts5ExprNearset*)malloc(sizeof(Fts5ExprNearset));
    pNear->nPhrase = 2;
    pNear->apPhrase = (Fts5ExprPhrase**)malloc(sizeof(Fts5ExprPhrase*) * pNear->nPhrase);
    pNear->apPhrase[0] = nullptr;
    pNear->apPhrase[1] = (Fts5ExprPhrase*)0x5678;
    pNear->pColset = nullptr; // NULL pColset

    // Call the function under test
    sqlite3Fts5ParseNearsetFree(nullptr, pNear, nullptr);

    // Verify that only pNear was freed
    bool freed_pNear = false;
    bool freed_other = false;
    for(void* p : g_freed_ptrs){
        if(p == pNear) freed_pNear = true;
        // If any other pointer freed, mark as extra (should not happen here)
        if(p != pNear) freed_other = true;
    }

    EXPECT_TRUE(freed_pNear, "pNear should be freed when pColset is NULL");
    EXPECT_TRUE(!freed_other, "No other non-null pointers should be freed when pColset is NULL");
    EXPECT_EQ(static_cast<int>(g_freed_ptrs.size()), 1, "Only one pointer should be freed (the pNear itself)");

    // Cleanup
    free(pNear);
    free(pNear->apPhrase);
}

// Main: run all tests and report summary
int main() {
    std::cout << "Starting sqlite3Fts5ParseNearsetFree unit tests...\n";

    RUN_TEST(test_null_pNear_no_free);
    RUN_TEST(test_nearset_free_frees_pColset_and_near);
    RUN_TEST(test_nearset_free_with_null_pColset_only_near);

    std::cout << "Tests run: " << g_tests_run << ", Failures: " << g_tests_failed << "\n";

    if(g_tests_failed == 0){
        std::cout << "ALL TESTS PASSED\n";
    } else {
        std::cout << "SOME TESTS FAILED\n";
    }

    // Return non-zero if any test failed
    return g_tests_failed ? 1 : 0;
}