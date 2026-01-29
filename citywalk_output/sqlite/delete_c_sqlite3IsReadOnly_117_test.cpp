// Test harness for the focal method: sqlite3IsReadOnly
// This standalone test reproduces the core decision logic of sqlite3IsReadOnly
// using small, self-contained stubs for Parse, Table, Trigger and related helpers.
// It deliberately uses a controllable tabIsReadOnly behavior via a function
// wrapper so we can simulate true/false branches of the dependent predicate.
// Note: In the real SQLite source, tabIsReadOnly is a static function inside delete.c.
// Here we expose a test-friendly wrapper to drive coverage in a single translation unit.

// The test focuses on the following candidates (Candidate Keywords):
// - sqlite3IsReadOnly, tabIsReadOnly, IsView
// - Parse, Table, Trigger structures
// - sqlite3ErrorMsg (simulated via storing errors in Parse)
// - pTrigger, pTab, pParse branches
// - View handling and tab/modification restrictions

#include <cstdio>
#include <cstdarg>
#include <string>
#include <sqliteInt.h>
#include <iostream>


// ----------------------------
// Lightweight test scaffolding
// ----------------------------

// Simple Parse structure to capture error messages
struct Parse {
    std::string zErrMsg;
};

// Lightweight Table structure with name and a flag indicating if it is a view
struct Table {
    const char* zName;
    bool isView;
};

// Lightweight Trigger structure with a "returning" flag and a next pointer
struct Trigger {
    bool bReturning;
    Trigger* pNext;
};

// ----------------------------
// Test harness helpers
// ----------------------------

// By default, provide a controllable override for tabIsReadOnly.
// If test overrides this flag, sqlite3IsReadOnly will treat the tab as read-only.
static int tabIsReadOnlyOverride = 0;

// Wrapper that mimics the behavior of the static tabIsReadOnly function.
// In the real code this would be a static function in delete.c; we mock its behavior here.
static int tabIsReadOnlyWrapper(Parse* /*pParse*/, Table* /*pTab*/) {
    return tabIsReadOnlyOverride;
}

// Redirect calls tabIsReadOnly(...) to our overridable wrapper for testability
#define tabIsReadOnly(pParse, pTab) tabIsReadOnlyWrapper(pParse, pTab)

// Simple IsView predicate to decide if a table is a view.
// In SQLite this is a macro/function; we implement a minimal version here.
static int IsView(Table* pTab) { return pTab->isView ? 1 : 0; }

// Simulated error reporter used by sqlite3IsReadOnly.
// It formats the message and stores it in pParse->zErrMsg.
static void sqlite3ErrorMsg(Parse* pParse, const char* fmt, ...) {
    char buf[512];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    if (pParse) pParse->zErrMsg = buf;
}

// The focal method under test (reproduced in this harness with small, testable hooks).
int sqlite3IsReadOnly(Parse *pParse, Table *pTab, Trigger *pTrigger){
    if( tabIsReadOnly(pParse, pTab) ){
        sqlite3ErrorMsg(pParse, "table %s may not be modified", pTab->zName);
        return 1;
    }
#ifndef SQLITE_OMIT_VIEW
    if( IsView(pTab)
     && (pTrigger==0 || (pTrigger->bReturning && pTrigger->pNext==0))
    ){
        sqlite3ErrorMsg(pParse,"cannot modify %s because it is a view",pTab->zName);
        return 1;
    }
#endif
    return 0;
}

// ----------------------------
// Test helpers
// ----------------------------

// Simple assertion-friendly test macro (non-terminating): logs failures but continues
#define EXPECT_TRUE(cond, msg) \
    do { if(!(cond)) { \
        std::cerr << "EXPECT_TRUE FAILED: " << (msg) << " in " << __FILE__ \
                  << ":" << __LINE__ << "\n"; \
        failures++; \
    } } while(0)

static int failures = 0;

// Helper to run a single test scenario
static bool runScenario(const std::string& name,
                        int tabReadOnlyValue,
                        bool tabIsView,
                        Trigger* pTrigger,
                        int expectReturn,
                        const std::string& expectedErrorSubstring = "")
{
    // Reset test state
    tabIsReadOnlyOverride = tabReadOnlyValue;

    Parse pParse;
    Table pTab;
    pTab.zName = "tbl";
    pTab.isView = tabIsView;

    int rc = sqlite3IsReadOnly(&pParse, &pTab, pTrigger);

    bool ok = true;
    if (rc != expectReturn) {
        std::cerr << "Test \"" << name << "\" failed: expected rc=" << expectReturn
                  << ", got rc=" << rc << "\n";
        ok = false;
        failures++;
    }

    if (!expectedErrorSubstring.empty()) {
        // Expect an error message substring
        if (pParse.zErrMsg.find(expectedErrorSubstring) == std::string::npos) {
            std::cerr << "Test \"" << name << "\" failed: expected error containing \""
                      << expectedErrorSubstring << "\", got: \"" << pParse.zErrMsg << "\"\n";
            ok = false;
            failures++;
        }
    } else {
        // No error expected
        if (!pParse.zErrMsg.empty()) {
            std::cerr << "Test \"" << name << "\" failed: unexpected error: "
                      << pParse.zErrMsg << "\n";
            ok = false;
            failures++;
        }
    }

    // Log a short success message
    if (ok) {
        std::cout << "PASS: " << name << "\n";
    } else {
        std::cout << "FAIL: " << name << "\n";
    }
    return ok;
}

// ----------------------------
// Individual test cases
// ----------------------------

// Test 1: tabIsReadOnly returns true -> should error: "may not be modified"
static bool test_tabReadOnly_true() {
    // Build scenario
    Trigger* pTrigger = nullptr;
    // Execute scenario
    return runScenario("tabIsReadOnly_true",
                       1,          // tabIsReadOnly returns non-zero
                       false,      // not a view
                       pTrigger,
                       1,            // expect read-only error
                       "may not be modified"); // error message fragment
}

// Test 2: tabIsReadOnly false, but pTab is a view and there is no returning trigger
// Should error: "cannot modify ... because it is a view"
static bool test_view_error_when_no_trigger() {
    Trigger* pTrigger = nullptr;
    // The tab is a view
    return runScenario("view_error_no_trigger",
                       0,          // tabIsReadOnly returns 0
                       true,       // is a view
                       pTrigger,
                       1,            // expect view modification error
                       "cannot modify"); // error message fragment
}

// Test 3: tabIsReadOnly false, not a view -> no error
static bool test_not_readOnly_and_not_view() {
    Trigger* pTrigger = nullptr;
    return runScenario("not_readOnly_and_not_view",
                       0,          // tabIsReadOnly returns 0
                       false,      // not a view
                       pTrigger,
                       0,            // no error
                       "");          // no expected error
}

// Test 4: tabIsReadOnly false, view with Returning and no next -> should error due to view
static bool test_view_withReturning_noNext() {
    Trigger t;
    t.bReturning = true;
    t.pNext = nullptr; // next is null
    Trigger* pTrigger = &t;

    return runScenario("view_withReturning_noNext",
                       0,          // tabIsReadOnly returns 0
                       true,       // is a view
                       pTrigger,
                       1,            // expect view modification error
                       "cannot modify"); // error fragment
}

// Test 5: tabIsReadOnly false, view with Returning and Next non-null -> should NOT error (allowed)
static bool test_view_withReturning_nextNonNull() {
    Trigger next;
    next.bReturning = false;
    next.pNext = nullptr;

    Trigger t;
    t.bReturning = true;
    t.pNext = &next; // next is non-null

    Trigger* pTrigger = &t;

    return runScenario("view_withReturning_nextNonNull",
                       0,          // tabIsReadOnly returns 0
                       true,       // is a view
                       pTrigger,
                       0,            // no error
                       "");          // no expected error
}

// Optional: test with OMIT_VIEW macro defined to simulate the compile-time omission of view checks.
// If the macro SQLITE_OMIT_VIEW is defined, the view-check branch is compiled out.
// We provide an alternative test function guarded by the macro so both build variants can be exercised.
// To enable this test, compile with -D SQLITE_OMIT_VIEW
#ifdef SQLITE_OMIT_VIEW
static bool test_view_check_omitted() {
    // In this variant, view checks are omitted; only tabIsReadOnly matters.
    Trigger* pTrigger = nullptr;
    // tabIsReadOnly false, tab not a view (has no effect since view-check is omitted)
    return runScenario("view_check_omitted",
                       0,
                       true,        // is a view (would be checked if not omitted)
                       pTrigger,
                       0,
                       "");         // no error expected since view check is omitted
}
#endif

// ----------------------------
// Main entry
// ----------------------------

int main() {
    std::cout << "Starting sqlite3IsReadOnly unit test suite (focal method) in a standalone harness...\n";

    // Run each test. The return value indicates pass/fail for the individual test.
    bool all_passed = true;
    // Test 1
    all_passed &= test_tabReadOnly_true();

    // Test 2
    all_passed &= test_view_error_when_no_trigger();

    // Test 3
    all_passed &= test_not_readOnly_and_not_view();

    // Test 4
    all_passed &= test_view_withReturning_noNext();

    // Test 5
    all_passed &= test_view_withReturning_nextNonNull();

#ifdef SQLITE_OMIT_VIEW
    // If built with OMIT_VIEW, attempt to exercise the omit-branch variant
    all_passed &= test_view_check_omitted();
#endif

    if (all_passed) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    } else {
        std::cout << "SOME TESTS FAILED\n";
        return 1;
    }
}