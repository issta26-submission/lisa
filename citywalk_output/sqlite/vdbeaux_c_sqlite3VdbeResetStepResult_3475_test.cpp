// Unit test suite for sqlite3VdbeResetStepResult
// This test is designed to be compiled with C++11 and linked against the project's sqlite sources.
// It exercises the focal function which sets p->rc to SQLITE_OK unconditionally.

#include <iostream>
#include <vdbeInt.h>
#include <sqliteInt.h>
#include <string>


// Bring in the C declarations with C linkage to ensure proper name mangling
extern "C" {
}

// Simple, non-terminating test harness.
// It reports PASS/FAIL per test case and returns non-zero if any test failed.

int main() {
    int tests = 0;
    int failures = 0;

    auto report = [&](const std::string &name, bool passed) {
        ++tests;
        if (passed) {
            std::cout << "[OK]   " << name << "\n";
        } else {
            ++failures;
            std::cerr << "[FAIL] " << name << "\n";
        }
    };

    // Test 1: When rc is non-OK, after reset it should be SQLITE_OK
    // This validates the unconditional assignment behavior of sqlite3VdbeResetStepResult.
    {
        Vdbe v;
        v.rc = -12345;                 // seed with a non-OK value
        sqlite3VdbeResetStepResult(&v);
        bool cond = (v.rc == SQLITE_OK);
        report("sqlite3VdbeResetStepResult sets rc to SQLITE_OK from non-OK", cond);
    }

    // Test 2: When rc is already SQLITE_OK, after reset it remains SQLITE_OK
    // This ensures we don't accidentally change a correctly-set value to something else.
    {
        Vdbe v;
        v.rc = SQLITE_OK;              // seed with OK
        sqlite3VdbeResetStepResult(&v);
        bool cond = (v.rc == SQLITE_OK);
        report("sqlite3VdbeResetStepResult preserves SQLITE_OK when already OK", cond);
    }

    std::cout << "Tests run: " << tests << ", Failures: " << failures << "\n";
    return (failures == 0) ? 0 : 1;
}