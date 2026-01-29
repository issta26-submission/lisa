// High-quality C++11 unit test for the focal method sqlite3MultiWrite
// This test provides a minimal, self-contained environment to verify that
// sqlite3MultiWrite correctly marks the toplevel Parse object as multi-write.
//
// Notes:
// - We avoid GTest. Instead, a lightweight test harness with EXPECT-like macros is used.
// - We provide a small C linkage override for sqlite3ParseToplevel to return a
//   controllable toplevel object so we can observe the effect of sqlite3MultiWrite.
// - The Parse type is kept deliberately simple with a single member isMultiWrite to
//   match the observable behavior required by the focal method.
// - The actual sqlite3MultiWrite implementation lives in build.c (as per the prompt).

#include <iostream>
#include <sqliteInt.h>


// Step 1: Minimal bridging declarations to allow linking with sqlite3MultiWrite
extern "C" {
    // LightweightParse type used by the focal method (simplified for testing).
    struct Parse {
        int isMultiWrite;
    };

    // Forward declaration of the focal function under test.
    // The real implementation is in build.c; we link against it.
    Parse* sqlite3ParseToplevel(Parse* pParse);
    void sqlite3MultiWrite(Parse* pParse);
}

// Step 2: Test scaffolding
// We provide a controllable toplevel object that sqlite3ParseToplevel will return.
// This avoids depending on the real parser internals and focuses on the observable effect.
static Parse gToplevel = {0};

// Override sqlite3ParseToplevel to return our test toplevel.
// This is a test double (fake) replacement that the linker will prefer for the translation unit.
extern "C" Parse* sqlite3ParseToplevel(Parse* /*pParse*/) {
    // Always return the same toplevel object for deterministic tests.
    return &gToplevel;
}

// Lightweight test harness (non-terminating expectations)
static int totalTests = 0;
static int failedTests = 0;

#define EXPECT_TRUE(cond, msg) do { \
  ++totalTests; \
  if(!(cond)) { \
    ++failedTests; \
    std::cerr << "Test FAILED: " << msg << " (line " << __LINE__ << ")" << std::endl; \
  } \
} while(0)

// Step 3: Unit tests for sqlite3MultiWrite

// Test 1: sqlite3MultiWrite sets the toplevel's isMultiWrite to 1
void test_sqlite3MultiWrite_sets_toplevel() {
    // Prepare a fresh pParse input (its content is ignored by our test double)
    Parse p;
    p.isMultiWrite = 0; // initial value (not relied upon by the test double)

    // Ensure the toplevel is reset
    gToplevel.isMultiWrite = 0;

    // Invoke the focal method
    sqlite3MultiWrite(&p);

    // Verify the effect on the toplevel object
    EXPECT_TRUE(gToplevel.isMultiWrite == 1,
                "sqlite3MultiWrite should set toplevel.isMultiWrite to 1");
}

// Test 2: sqlite3MultiWrite is idempotent w.r.t toplevel state (second call keeps isMultiWrite at 1)
void test_sqlite3MultiWrite_idempotent() {
    // Pre-set a non-zero value to simulate pre-existing state
    gToplevel.isMultiWrite = 123;

    Parse p;
    p.isMultiWrite = 0;

    sqlite3MultiWrite(&p);

    // After the call, isMultiWrite should be forced to 1 regardless of previous state
    EXPECT_TRUE(gToplevel.isMultiWrite == 1,
                "sqlite3MultiWrite should set isMultiWrite to 1 even if it was non-zero before");
}

// Main harness: run tests and report summary
int main() {
    // Run tests
    test_sqlite3MultiWrite_sets_toplevel();
    test_sqlite3MultiWrite_idempotent();

    // Report summary
    std::cout << "Total tests run: " << totalTests
              << ", Failures: " << failedTests << std::endl;

    return (failedTests == 0) ? 0 : 1;
}