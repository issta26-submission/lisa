#include <iostream>
#include <cstring>
#include <sqliteInt.h>


// Step 1/2 domain guidance: Include internal SQLite definitions so that the
// sqlite3WritableSchema function can be linked against and the sqlite3 struct
// layout is known to the compiler. We wrap C headers in extern "C" to ensure
// proper linkage from C++.
extern "C" {
}

// Step 2: Forward declare the focal function with C linkage.
// The test harness will link against the C implementation in build.c.
extern "C" int sqlite3WritableSchema(sqlite3 *db);

// Global test failure counter to allow non-terminating assertions.
// This aligns with the requirement to use non-terminating assertions to maximize coverage.
static int g_failures = 0;

// Lightweight assertion helper that reports failures but does not terminate the test.
// This helps exercise multiple scenarios in a single run and accumulate coverage data.
#define EXPECT_EQ_INT(actual, expected) do {                    \
    if ((actual) != (expected)) {                                \
        std::cerr << "EXPECT_EQ at line " << __LINE__ << ": "  \
                  << actual << " != " << expected << "\n";      \
        ++g_failures;                                             \
    }                                                             \
} while (0)

int main() {
    // Step 3: Test scaffolding
    // We rely on the actual sqlite3WritableSchema implementation from build.c.
    // The tests manipulate the db->flags field to cover true/false branches of
    // the condition predicates inside sqlite3WritableSchema.

    // Prepare a sqlite3 instance. Using memset ensures all fields start from a known state.
    sqlite3 db;
    std::memset(&db, 0, sizeof(db));

    // Test Case A: flags = 0 (neither SQLITE_WriteSchema nor SQLITE_Defensive set)
    // Expected: predicate1 true, and overall return value should be 0.
    // This covers the "no flags" branch.
    db.flags = 0;
    int res = sqlite3WritableSchema(&db);
    EXPECT_EQ_INT(res, 0);
    // Additional diagnostics for this scenario
    // (The four testcases inside sqlite3WritableSchema will be evaluated with this flag set.)

    // Test Case B: flags = SQLITE_WriteSchema (only WriteSchema bit set)
    // Expected: overall return value should be 1 (since mask equals SQLITE_WriteSchema).
    db.flags = SQLITE_WriteSchema;
    res = sqlite3WritableSchema(&db);
    EXPECT_EQ_INT(res, 1);

    // Test Case C: flags = SQLITE_Defensive (only Defensive bit set)
    // Expected: overall return value should be 0 (mask does not equal SQLITE_WriteSchema).
    db.flags = SQLITE_Defensive;
    res = sqlite3WritableSchema(&db);
    EXPECT_EQ_INT(res, 0);

    // Test Case D: flags = SQLITE_WriteSchema | SQLITE_Defensive (both bits set)
    // Expected: overall return value should be 0 (mask equals (Write|Defensive), not Write).
    db.flags = SQLITE_WriteSchema | SQLITE_Defensive;
    res = sqlite3WritableSchema(&db);
    EXPECT_EQ_INT(res, 0);

    // Finalize test results
    if (g_failures == 0) {
        std::cout << "All tests passed without terminating on false assertions.\n";
        return 0;
    } else {
        std::cerr << "Total failures: " << g_failures << "\n";
        // Return non-zero to indicate failing test suite
        return 1;
    }
}