// Unit test suite for the focal method: sqlite3UpsertOfIndex
// This test suite is designed to run without GTest, using a lightweight
// in-file test harness and non-terminating assertions to maximize coverage.
// The tests mock minimal dependencies (Upsert, Index) with the exact fields
// used by the focal method to avoid relying on external project headers.
// The focal method logic is implemented here in the same style as the snippet
// provided, ensuring behavior parity for test purposes.

#include <vector>
#include <sstream>
#include <string>
#include <sqliteInt.h>
#include <iostream>


// DOMAIN_KNOWLEDGE highlights:
//  - We implement the Upsert and Index types locally with only the fields the
//    function uses: pNextUpsert, pUpsertTarget, pUpsertIdx.
//  - We avoid private/private-static concerns by exposing a tiny, test-focused API.
//  - We provide non-terminating assertions to exercise code paths even on failures.


// <FOCAL_CLASS_DEP>
// Minimal mock definitions to mirror the dependencies used by sqlite3UpsertOfIndex.

struct Index; // Forward declaration for pointer usage.

struct Upsert {
    Upsert* pNextUpsert;    // Next node in the linked list
    void*    pUpsertTarget;  // Non-zero means a valid target
    Index*   pUpsertIdx;      // The index this Upsert is associated with
};

// </FOCAL_CLASS_DEP>

// The focal method implemented as per the provided snippet.
// It walks the linked list of Upsert nodes until it finds a node whose
// pUpsertIdx equals pIdx or until termination conditions fail.
static Upsert* sqlite3UpsertOfIndex(Upsert* pUpsert, Index* pIdx) {
    while (pUpsert
           && pUpsert->pUpsertTarget != 0
           && pUpsert->pUpsertIdx != pIdx) {
        pUpsert = pUpsert->pNextUpsert;
    }
    return pUpsert;
}


// ----------------------
// Lightweight test harness
// ----------------------

static int g_failures = 0;

// Helper to stringify pointers for logging
static std::string ptrStr(const void* p) {
    std::ostringstream oss;
    oss << p;
    return oss.str();
}

// Non-terminating assertion: compares two pointers for equality
static void assertPtrEq(const std::string& testName,
                        const std::string& description,
                        const Upsert* got,
                        const Upsert* expected) {
    if (got != expected) {
        std::cerr << "[FAIL] " << testName << ": " << description
                  << " | expected pointer " << ptrStr(static_cast<const void*>(expected))
                  << " but got " << ptrStr(static_cast<const void*>(got)) << "\n";
        ++g_failures;
    } else {
        std::cout << "[PASS] " << testName << ": " << description << "\n";
    }
}

// Non-terminating assertion: compares two pointers for equality (Non-UPCAST-safe variant)
static void assertPtrEqGeneric(const std::string& testName,
                               const std::string& description,
                               const void* got,
                               const void* expected) {
    if (got != expected) {
        std::cerr << "[FAIL] " << testName << ": " << description
                  << " | expected " << ptrStr(expected)
                  << " but got " << ptrStr(got) << "\n";
        ++g_failures;
    } else {
        std::cout << "[PASS] " << testName << ": " << description << "\n";
    }
}

// Convenience function for tests that return Upsert* but we only compare address equality.
static void testNullPointerBehavior() {
    // Focal case: when pUpsert is nullptr, function should return nullptr.
    Upsert* res = sqlite3UpsertOfIndex(nullptr, nullptr);
    assertPtrEq("case_null_input", "returns nullptr when input is null", res, nullptr);
}

// Test 1: First node matches the target index (pIdx). Expect return of first node.
static void testFirstNodeMatchesIdx() {
    Upsert a;
    Index idxX;
    Upsert* head = &a;
    a.pNextUpsert = nullptr;
    a.pUpsertTarget = reinterpret_cast<void*>(1); // non-zero
    a.pUpsertIdx = &idxX;

    Upsert* res = sqlite3UpsertOfIndex(head, &idxX);
    assertPtrEq("case_first_node_matches_idx", "first node should be returned when idx matches", res, head);
}

// Test 2: First node does not match, and there is a next node which matches.
// Expect the function to traverse and return the matching next node.
static void testSecondNodeMatchesIdxAfterTraversal() {
    Upsert a;
    Upsert b;
    Index idxX;
    Index idxOther;

    a.pNextUpsert = &b;
    a.pUpsertTarget = reinterpret_cast<void*>(2); // non-zero
    a.pUpsertIdx = &idxOther;                    // does not match

    b.pNextUpsert = nullptr;
    b.pUpsertTarget = reinterpret_cast<void*>(3); // non-zero
    b.pUpsertIdx = &idxX;                         // matches

    Upsert* res = sqlite3UpsertOfIndex(&a, &idxX);
    assertPtrEq("case_second_node_matches_after_traversal",
                "should return the second node when it matches after traversal",
                res, &b);
}

// Test 3: First node does not match, but there is no matching node in the chain.
// The traversal ends at null; expect null.
static void testTraversalEndsWithNull() {
    Upsert a;
    Upsert b;
    Index idxX;
    Index idxY;

    a.pNextUpsert = &b;
    a.pUpsertTarget = reinterpret_cast<void*>(4); // non-zero
    a.pUpsertIdx = &idxY;                         // does not match

    b.pNextUpsert = nullptr;
    b.pUpsertTarget = reinterpret_cast<void*>(5); // non-zero
    b.pUpsertIdx = &idxY;                         // still does not match

    Upsert* res = sqlite3UpsertOfIndex(&a, &idxX);
    assertPtrEq("case_traversal_ends_with_null", "no matching idx in chain -> null", res, nullptr);
}

// Test 4: First node has a target of 0 (null). According to the while condition,
// loop should not execute, and function should return the first node regardless of idx.
static void testFirstNodeTargetZeroReturnsFirst() {
    Upsert a;
    Upsert b;
    Index idxX;
    Index idxOther;

    a.pNextUpsert = &b;
    a.pUpsertTarget = nullptr;                    // zero
    a.pUpsertIdx = &idxOther;                     // does not matter

    b.pNextUpsert = nullptr;
    b.pUpsertTarget = reinterpret_cast<void*>(1); // non-zero
    b.pUpsertIdx = &idxX;                         // would match, but traversal shouldn't occur

    Upsert* res = sqlite3UpsertOfIndex(&a, &idxX);
    assertPtrEq("case_first_node_target_zero_returns_first",
                "first node with target 0 should be returned without traversal",
                res, &a);
}

// Test 5: First node matches despite a longer chain; ensure early return behavior is preserved.
// We reuse a scenario where first node idx equals pIdx.
static void testFirstNodeMatchesIdxEvenWithLongerChain() {
    Upsert a;
    Upsert b;
    Index idxTarget;
    Index idxOther;

    a.pNextUpsert = &b;
    a.pUpsertTarget = reinterpret_cast<void*>(10);
    a.pUpsertIdx = &idxTarget;  // matches

    b.pNextUpsert = nullptr;
    b.pUpsertTarget = reinterpret_cast<void*>(11);
    b.pUpsertIdx = &idxOther;

    Upsert* res = sqlite3UpsertOfIndex(&a, &idxTarget);
    assertPtrEq("case_first_node_matches_idx_even_with_longer_chain",
                "first node should be returned when its idx matches, regardless of chain",
                res, &a);
}


// Entry point
int main() {
    // Run tests
    testNullPointerBehavior();
    testFirstNodeMatchesIdx();
    testSecondNodeMatchesIdxAfterTraversal();
    testTraversalEndsWithNull();
    testFirstNodeTargetZeroReturnsFirst();
    testFirstNodeMatchesIdxEvenWithLongerChain();

    std::cout << "Total test failures: " << g_failures << std::endl;
    // Return non-zero if there were failures to signal an unsuccessful test run
    return (g_failures == 0) ? 0 : 1;
}