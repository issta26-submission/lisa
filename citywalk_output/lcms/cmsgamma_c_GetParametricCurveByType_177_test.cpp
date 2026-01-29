// Lightweight, self-contained C++11 unit test suite for the focal method:
// GetParametricCurveByType
// The tests are designed to be self-sufficient (no external test framework).
// They mock the essential environment and dependencies, focusing on coverage
// of true/false branches, pointer/index handling, and edge cases.
// Note: This is a self-contained approximation of the focal method behavior
// implemented in a single translation unit to enable deterministic testing.

// Compile with: g++ -std=c++11 -O2 -Wall -Wextra this_test.cpp -o this_test
// Run: ./this_test

#include <lcms2_internal.h>
#include <iostream>
#include <cstddef>


// ------------------------------------------------------------
// Minimal type/macro scaffolding to mirror the focal method's APIs.
// These definitions are stand-ins to exercise the core logic in isolation.
// ------------------------------------------------------------

// Typedef representing the opaque context handle (as in the real project)
typedef void* cmsContext;

// Forward declaration of the parametric curve collection node.
typedef struct _cmsParametricCurvesCollection _cmsParametricCurvesCollection;

// Forward declaration of the curves plugin chunk type stored in the context.
typedef struct _cmsCurvesPluginChunkType _cmsCurvesPluginChunkType;

// Function prototype of the focal method under test (mimicked in this TU)
extern "C" _cmsParametricCurvesCollection* GetParametricCurveByType(cmsContext ContextID, int Type, int* index);

// Dependency injections used by the focal method
// (In the real project, these would be provided elsewhere.)
extern "C" int IsInSet(int Type, _cmsParametricCurvesCollection* c);

// The CurvesPlugin chunk enum value (simplified)
const int CurvesPlugin = 1;

// Global default curves list (as in the original code: a static DefaultCurves head)
static _cmsParametricCurvesCollection DefaultCurvesHead;

// ------------------------------------------------------------------------------------------------
// Mocked environment to exercise the focal method logic in isolation
// ------------------------------------------------------------------------------------------------

// Node type representing a parametric curve entry
struct _cmsParametricCurvesCollection {
    _cmsParametricCurvesCollection* Next; // next in list
    int Type;                              // key used by IsInSet(Type, node)
    int Position;                          // position/index value returned by IsInSet
};

// Curves plugin chunk type containing a list of parametric curves
struct _cmsCurvesPluginChunkType {
    _cmsParametricCurvesCollection* ParametricCurves;
};

// A lightweight fake _cmsContextGetClientChunk() replacement.
// In the real library this returns a client chunk from the cms context.
// Here, we simply return a global chunk suitable for tests.
static _cmsCurvesPluginChunkType g_testCurvesChunk;

// Minimal implementation mirroring the expected behavior
extern "C" _cmsCurvesPluginChunkType* _cmsContextGetClientChunk(cmsContext ContextID, int ChunkType) {
    // We only support CurvesPlugin in this mock environment.
    (void)ContextID; (void)ChunkType;
    return &g_testCurvesChunk;
}

// Simple IsInSet implementation used by the focal method under test.
// Returns c->Position if Type matches c->Type, otherwise -1.
extern "C" int IsInSet(int Type, _cmsParametricCurvesCollection* c) {
    if (c == NULL) return -1;
    return (Type == c->Type) ? c->Position : -1;
}

// ------------------------------------------------------------------------------------------------
// The focal method under test (self-contained version for this TU).
// In the real project, this function is defined in cmsgamma.c. Here we provide
// a faithful, testable replica to validate behavior in isolation.
// ------------------------------------------------------------------------------------------------
extern "C" _cmsParametricCurvesCollection* GetParametricCurveByType(cmsContext ContextID, int Type, int* index)
{
    {
        _cmsParametricCurvesCollection* c;
        int Position;
        _cmsCurvesPluginChunkType* ctx = ( _cmsCurvesPluginChunkType*) _cmsContextGetClientChunk(ContextID, CurvesPlugin);
        for (c = ctx->ParametricCurves; c != NULL; c = c ->Next) {
            Position = IsInSet(Type, c);
            if (Position != -1) {
                if (index != NULL)
                    *index = Position;
                return c;
            }
        }
        // If none found, revert for defaults
        for (c = &DefaultCurvesHead; c != NULL; c = c ->Next) {
            Position = IsInSet(Type, c);
            if (Position != -1) {
                if (index != NULL)
                    *index = Position;
                return c;
            }
        }
        return NULL;
    }
}

// ------------------------------------------------------------------------------------------------
// Test scaffolding utilities
// Helper to create a new node in the parametric curves list
static _cmsParametricCurvesCollection* make_node(int Type, int Position) {
    _cmsParametricCurvesCollection* n = new _cmsParametricCurvesCollection;
    n->Next = NULL;
    n->Type = Type;
    n->Position = Position;
    return n;
}

// Append a node to a singly-linked list tail (maintains insertion order)
static void append_node(_cmsParametricCurvesCollection*& head, _cmsParametricCurvesCollection* node) {
    if (head == NULL) {
        head = node;
        node->Next = NULL;
        return;
    }
    _cmsParametricCurvesCollection* cur = head;
    while (cur->Next != NULL) cur = cur->Next;
    cur->Next = node;
    node->Next = NULL;
}

// Reset all test state
static void reset_test_state() {
    g_testCurvesChunk.ParametricCurves = NULL;
    // Reset DefaultCurvesHead to an empty list
    DefaultCurvesHead.Next = NULL;
    DefaultCurvesHead.Type = -1;
    DefaultCurvesHead.Position = -1;
    DefaultCurvesHead.Next = NULL;
}

// Print summary
static void print_summary(int total, int failures) {
    std::cout << "\nTest summary: " << total << " tests, "
              << failures << " failures.\n";
}

// ----------------------------------
// Unit tests
// We'll implement five tests to cover:

// 1) Found in ctx ParametricCurves (first element matches) -> returns first node, index set
// 2) Not found in ctx, found in DefaultCurves -> returns default curve, index set
// 3) Not found anywhere -> returns NULL, index untouched
// 4) Found with NULL index pointer -> should not dereference index
// 5) Correct first-match precedence when multiple matches exist in ctx

static int g_total = 0;
static int g_failures = 0;

// Simple expectations that don't abort test execution (non-terminating)
#define EXPECT_TRUE(cond, desc) \
    do { \
        ++g_total; \
        if (!(cond)) { \
            std::cerr << "EXPECT_TRUE FAILED: " << (desc) << " [Line " << __LINE__ << "]\n"; \
            ++g_failures; \
        } \
    } while (0)

#define EXPECT_PTR_EQ(p, q, desc) \
    do { \
        ++g_total; \
        if ((p) != (q)) { \
            std::cerr << "EXPECT_PTR_EQ FAILED: " << (desc) << " (0x" << std::hex << (uintptr_t)(p) \
                      << " != 0x" << (uintptr_t)(q) << ") [Line " << std::dec << __LINE__ << "]\n"; \
            ++g_failures; \
        } \
    } while (0)

#define EXPECT_INT_EQ(a, b, desc) \
    do { \
        ++g_total; \
        if ((a) != (b)) { \
            std::cerr << "EXPECT_INT_EQ FAILED: " << (desc) << " (" << (a) << " != " << (b) << ") " \
                      << "[Line " << __LINE__ << "]\n"; \
            ++g_failures; \
        } \
    } while (0)

static void test_found_in_ctx_first_element() {
    // Scenario: ctx ParametricCurves has at least one entry whose IsInSet(Type) returns a non -1
    // Expected: GetParametricCurveByType returns the first node, index equals its Position.
    reset_test_state();

    // Build ctx chain: A(Type=10, Position=0) -> B(Type=99, Position=5)
    _cmsParametricCurvesCollection* A = make_node(10, 0);
    _cmsParametricCurvesCollection* B = make_node(99, 5);
    A->Next = B;
    B->Next = NULL;
    g_testCurvesChunk.ParametricCurves = A;

    // No default curves
    DefaultCurvesHead.Next = NULL;

    int idx = -1;
    _cmsParametricCurvesCollection* res = GetParametricCurveByType(nullptr, 10, &idx);

    EXPECT_PTR_EQ(res, A, "Return the first ctx element when Type matches");
    EXPECT_INT_EQ(idx, 0, "Index should be the Position of the matching node (0)");
}

static void test_found_in_default_curves_when_ctx_none() {
    // Scenario: ctx has no matching entry; DefaultCurves contains a matching node.
    reset_test_state();

    // ctx is empty
    g_testCurvesChunk.ParametricCurves = NULL;

    // DefaultCurves: D(Type=200, Position=1)
    DefaultCurvesHead.Next = NULL;
    _cmsParametricCurvesCollection* D = make_node(200, 1);
    DefaultCurvesHead.Next = D;
    D->Next = NULL;

    int idx = -999;
    _cmsParametricCurvesCollection* res = GetParametricCurveByType(nullptr, 200, &idx);

    EXPECT_PTR_EQ(res, D, "Return default curve when ctx has no match");
    EXPECT_INT_EQ(idx, 1, "Index should be the Position of the default curve (1)");
}

static void test_not_found_anywhere() {
    // Scenario: No match in ctx or defaults -> NULL return
    reset_test_state();

    // ctx: one non-matching node
    _cmsParametricCurvesCollection* C = make_node(11, 7);
    g_testCurvesChunk.ParametricCurves = C;
    C->Next = NULL;

    // DefaultCurves: one non-matching node
    DefaultCurvesHead.Next = NULL;
    _cmsParametricCurvesCollection* D = make_node(99, 3);
    DefaultCurvesHead.Next = D;
    D->Next = NULL;

    int idx = -1;
    _cmsParametricCurvesCollection* res = GetParametricCurveByType(nullptr, 12345, &idx);

    EXPECT_TRUE(res == NULL, "Return NULL when there is no matching curve");
    // In this case, index is not touched by the function (remains as initialized)
    EXPECT_INT_EQ(idx, -1, "Index should remain unchanged when no match is found");
}

static void test_index_pointer_can_be_NULL() {
    // Scenario: When index is NULL, function should not attempt to write to it
    reset_test_state();

    // ctx: first node matches
    _cmsParametricCurvesCollection* A = make_node(42, 2);
    g_testCurvesChunk.ParametricCurves = A;
    A->Next = NULL;

    // DefaultCurves not used
    DefaultCurvesHead.Next = NULL;

    int tmp = -12345;
    _cmsParametricCurvesCollection* res = GetParametricCurveByType(nullptr, 42, NULL);

    // Even though index is NULL, function returns the correct node
    EXPECT_PTR_EQ(res, A, "Return matching ctx node when index is NULL");
    // Since index is NULL, we can't verify value; just ensure no crash/undef behavior
    (void)tmp; // suppress unused warning for tmp
}

static void test_multiple_matches_first_precedence() {
    // Scenario: First node in ctx matches with Position != 0; ensure first match is returned
    reset_test_state();

    // ctx: A(Type=55, Position=2) -> B(Type=55, Position=0)
    _cmsParametricCurvesCollection* A = make_node(55, 2);
    _cmsParametricCurvesCollection* B = make_node(55, 0);
    A->Next = B;
    B->Next = NULL;
    g_testCurvesChunk.ParametricCurves = A;

    DefaultCurvesHead.Next = NULL;

    int idx = -999;
    _cmsParametricCurvesCollection* res = GetParametricCurveByType(nullptr, 55, &idx);

    // Should return the first node (A) despite a later node having a different Position order
    EXPECT_PTR_EQ(res, A, "First matching node in ctx should be returned (precedence)");
    EXPECT_INT_EQ(idx, 2, "Index should reflect the Position of the first matching node (2)");
}

// ----------------------------------
// Main runner
int main() {
    reset_test_state();

    test_found_in_ctx_first_element();
    test_found_in_default_curves_when_ctx_none();
    test_not_found_anywhere();
    test_index_pointer_can_be_NULL();
    test_multiple_matches_first_precedence();

    print_summary(g_total, g_failures);
    // Return non-zero if there were failures, to facilitate integration with simple scripts.
    return (g_failures == 0) ? 0 : 1;
}

// End of test suite
// The tests above exercise key branches of the focal method's logic:
// - Branching when a match is found in the context-provided curve list.
// - Branching to defaults when not found in the context list.
// - The NULL 'index' handling path.
// - The behavior when no match exists anywhere.
// - Precedence of first matching node in a multi-match scenario.
// Comments accompany each test to clarify intent and coverage.