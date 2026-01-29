// Unit test suite for the focal C function: sqlite3_dispose_module
// Target: verify correct disposal behavior of vtshim module wrapper.
// Environment: C++11, no GTest. A lightweight, self-contained test harness is used.
// Note: The goal is to exercise true/false branches and interaction with dependent structures.
// The test defines minimal stand-ins for vtshim_* structures to align with sqlite3_dispose_module's usage.
// The real function is provided by vtshim.c (linked in the test binary) and expects a specific
// memory layout for vtshim_aux, vtshim_vtab, vtshim_cursor, and vtshim_module.
// The tests build memory layouts that mimic those structures and verify the function's side effects.

#include <string.h>
#include <vector>
#include <assert.h>
#include <sqlite3ext.h>
#include <cstring>
#include <iostream>
#include <cstdint>
#include <cassert>


// Forward declaration of the focal function from vtshim.c (C linkage)
extern "C" void sqlite3_dispose_module(void* pX);

// Candidate Keywords derived from Step 1 analysis:
// - vtshim_aux: disposal state, list of vtabs, module pointer, child destroy hook
// - vtshim_vtab: linked list, contains a list of cursors and a child pointer
// - vtshim_cursor: linked list of cursors, contains a child handle to be closed
// - vtshim_module: interface with xClose and xDisconnect
// The tests below construct these structures and ensure sqlite3_dispose_module traverses and cleans up as expected.

// Minimal structure definitions to mirror the layout used by sqlite3_dispose_module.
// We assume a common 64-bit environment; field order matches the code path in the focal method.

typedef struct vtshim_cursor {
    struct vtshim_cursor* pNext;
    void* pChild;
} vtshim_cursor;

typedef struct vtshim_vtab {
    struct vtshim_vtab* pNext;
    vtshim_cursor* pAllCur;
    void* pChild;
} vtshim_vtab;

typedef struct vtshim_module {
    void (*xClose)(void*);
    void (*xDisconnect)(void*);
} vtshim_module;

typedef struct vtshim_aux {
    int bDisposed;
    vtshim_vtab* pAllVtab;
    vtshim_module* pMod;
    void (*xChildDestroy)(void*);
    void* pChildAux;
} vtshim_aux;

// Global logs to verify proper calls without terminating tests.
// These are reset per test to ensure isolation.
static std::vector<void*> g_close_calls;
static std::vector<void*> g_disconnect_calls;
static std::vector<void*> g_child_destroy_calls;

// Mock implementations to be plugged into vtshim_module for testing.
static void mock_xClose(void* pChild) {
    g_close_calls.push_back(pChild);
}
static void mock_xDisconnect(void* pChild) {
    g_disconnect_calls.push_back(pChild);
}
static void mock_xDestroy(void* pChildAux) {
    g_child_destroy_calls.push_back(pChildAux);
}

// Simple non-terminating assertion helpers.
static int g_test_failures = 0;

static void expect_ptr_eq(const char* desc, void* a, void* b) {
    if (a != b) {
        std::cerr << "FAIL: " << desc << " — pointer mismatch: " << a << " != " << b << std::endl;
        ++g_test_failures;
    }
}
static void expect_int_eq(const char* desc, int a, int b) {
    if (a != b) {
        std::cerr << "FAIL: " << desc << " — int mismatch: " << a << " != " << b << std::endl;
        ++g_test_failures;
    }
}
static void reset_logs() {
    g_close_calls.clear();
    g_disconnect_calls.clear();
    g_child_destroy_calls.clear();
}

// Test 1: Basic disposal path with multiple vtables and cursors.
// Verifies: all xClose for cursors invoked, then xDisconnect for each vt, then bDisposed = 1,
// and xChildDestroy invoked if present.
static bool test_dispose_module_basic() {
    g_test_failures = 0;
    reset_logs();

    // Prepare mock module
    static vtshim_module mockModule;
    mockModule.xClose = mock_xClose;
    mockModule.xDisconnect = mock_xDisconnect;

    // Build cursor chains for two vtabs
    static vtshim_cursor c1b = { nullptr, (void*)0x1002 };
    static vtshim_cursor c1a = { &c1b,      (void*)0x1001 };

    static vtshim_cursor c2a = { nullptr,   (void*)0x3001 };

    // vtab 1
    static vtshim_vtab vtab2 = { nullptr, &c2a, (void*)0x2002 }; // second vtab (pNext points to NULL)
    static vtshim_vtab vtab1 = { &vtab2, &c1a, (void*)0x2001 }; // first vtab (points to vtab2)

    vtshim_vtab* head = &vtab1;

    // Auxiliary struct
    static vtshim_aux aux;
    aux.bDisposed = 0;
    aux.pAllVtab = head;
    aux.pMod = &mockModule;
    aux.xChildDestroy = mock_xDestroy;
    aux.pChildAux = (void*)0x4000;

    // Clear any leftovers
    reset_logs();

    // Action
    sqlite3_dispose_module((void*)&aux);

    // Assertions
    expect_int_eq("Disposed flag after disposal", aux.bDisposed, 1);

    // Close should be called for each cursor: 0x1001, 0x1002, 0x3001
    expect_ptr_eq("First close called on 0x1001", g_close_calls.size() >= 1 ? g_close_calls[0] : nullptr, (void*)0x1001);
    // The order should be exactly as declared by the nested loops:
    // vtab1 cursors: 0x1001, 0x1002
    // vtab2 cursors: 0x3001
    // We'll check positions explicitly for robustness
    if (g_close_calls.size() >= 3) {
        expect_ptr_eq("Second close called on 0x1002", g_close_calls[1], (void*)0x1002);
        expect_ptr_eq("Third close called on 0x3001", g_close_calls[2], (void*)0x3001);
    } else {
        std::cerr << "FAIL: Insufficient close calls: " << g_close_calls.size() << std::endl;
        ++g_test_failures;
    }

    // Disconnects: for vtab1 (pChild 0x2001) and vtab2 (pChild 0x2002)
    expect_int_eq("Disconnect calls count", (int)g_disconnect_calls.size(), 2);
    if (g_disconnect_calls.size() >= 2) {
        expect_ptr_eq("First disconnect on 0x2001", g_disconnect_calls[0], (void*)0x2001);
        expect_ptr_eq("Second disconnect on 0x2002", g_disconnect_calls[1], (void*)0x2002);
    }

    // Child destroy callback
    expect_int_eq("Child destroy calls count", (int)g_child_destroy_calls.size(), 1);
    if (!g_child_destroy_calls.empty()) {
        expect_ptr_eq("Destroy callback arg", g_child_destroy_calls[0], (void*)0x4000);
    }

    // Result
    bool ok = (g_test_failures == 0);
    if (!ok) {
        std::cerr << "test_dispose_module_basic: FAILED with " << g_test_failures << " failures." << std::endl;
    }
    return ok;
}

// Test 2: If already disposed, sqlite3_dispose_module should be a no-op (no calls).
static bool test_dispose_module_already_disposed_noop() {
    g_test_failures = 0;
    reset_logs();

    // Prepare mock module
    static vtshim_module mockModule;
    mockModule.xClose = mock_xClose;
    mockModule.xDisconnect = mock_xDisconnect;

    // Simple single vtab with single cursor
    static vtshim_cursor c1 = { nullptr, (void*)0xDEAD1 };
    static vtshim_vtab vtab1 = { nullptr, &c1, (void*)0xB001 };

    static vtshim_aux aux;
    aux.bDisposed = 1; // Already disposed
    aux.pAllVtab = &vtab1;
    aux.pMod = &mockModule;
    aux.xChildDestroy = mock_xDestroy;
    aux.pChildAux = (void*)0xC001;

    reset_logs();
    sqlite3_dispose_module((void*)&aux);

    // Expect no calls since disposed flag is already set
    expect_int_eq("No xClose calls when already disposed", (int)g_close_calls.size(), 0);
    expect_int_eq("No xDisconnect calls when already disposed", (int)g_disconnect_calls.size(), 0);
    expect_int_eq("No child destroy calls when already disposed", (int)g_child_destroy_calls.size(), 0);

    bool ok = (g_test_failures == 0);
    if (!ok) {
        std::cerr << "test_dispose_module_already_disposed_noop: FAILED with " << g_test_failures << " failures." << std::endl;
    }
    return ok;
}

// Test 3: When xChildDestroy is NULL, dispose should not attempt to call a destroy callback.
static bool test_dispose_module_no_destroy_callback() {
    g_test_failures = 0;
    reset_logs();

    // Prepare mock module
    static vtshim_module mockModule;
    mockModule.xClose = mock_xClose;
    mockModule.xDisconnect = mock_xDisconnect;

    // Build a simple structure: one vtab, one cursor
    static vtshim_cursor c1 = { nullptr, (void*)0xD1 };
    static vtshim_vtab vtab1 = { nullptr, &c1, (void*)0xD0 };

    static vtshim_aux aux;
    aux.bDisposed = 0;
    aux.pAllVtab = &vtab1;
    aux.pMod = &mockModule;
    aux.xChildDestroy = nullptr; // No destroy callback
    aux.pChildAux = (void*)0xE0;

    reset_logs();
    sqlite3_dispose_module((void*)&aux);

    // Verify that xClose and xDisconnect were invoked
    expect_int_eq("Close calls count", (int)g_close_calls.size(), 1);
    expect_ptr_eq("Close on cursor child 0xD1", g_close_calls[0], (void*)0xD1);

    expect_int_eq("Disconnect calls count", (int)g_disconnect_calls.size(), 1);
    expect_ptr_eq("Disconnect on vtab child 0xD0", g_disconnect_calls[0], (void*)0xD0);

    // No destroy callback should be invoked
    expect_int_eq("Destroy callback calls count", (int)g_child_destroy_calls.size(), 0);

    bool ok = (g_test_failures == 0);
    if (!ok) {
        std::cerr << "test_dispose_module_no_destroy_callback: FAILED with " << g_test_failures << " failures." << std::endl;
    }
    return ok;
}

// Test harness
static void run_all_tests() {
    std::cout << "Starting sqlite3_dispose_module test suite (C++11)..." << std::endl;

    bool r1 = test_dispose_module_basic();
    std::cout << "Test 1 (basic disposal) " << (r1 ? "PASSED" : "FAILED") << std::endl;

    bool r2 = test_dispose_module_already_disposed_noop();
    std::cout << "Test 2 (already disposed no-op) " << (r2 ? "PASSED" : "FAILED") << std::endl;

    bool r3 = test_dispose_module_no_destroy_callback();
    std::cout << "Test 3 (no xChildDestroy) " << (r3 ? "PASSED" : "FAILED") << std::endl;

    int all = (r1 && r2 && r3) ? 0 : 1;
    if (all == 0) {
        std::cout << "All tests PASSED." << std::endl;
    } else {
        std::cout << "Some tests FAILED." << std::endl;
    }
}

// Entry point
int main() {
    run_all_tests();
    return 0;
}