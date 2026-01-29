// Comprehensive unit test suite for sqlite3VdbeCheckFk using a handcrafted harness.
// This test suite is designed to be compiled with C++11, without Google Test.
// It relies on the project's internal headers to obtain the actual Vdbe and sqlite3
// structures and related macros (e.g., SQLITE_OK, SQLITE_ERROR, SQLITE_CONSTRAINT_FOREIGNKEY,
// SQLITE_PREPARE_SAVESQL, OE_Abort, etc.).
// The test uses non-terminating expectations to maximize code coverage.

#include <vdbeInt.h>
#include <sqliteInt.h>
#include <cstring>
#include <iostream>
#include <cstdlib>


// Bring in internal SQLite/Vdbe declarations.
// These headers expose the internal structs (e.g., Vdbe, sqlite3) and constants needed
// to exercise sqlite3VdbeCheckFk correctly.
extern "C" {
}

// Lightweight test harness: non-terminating assertions that record failures but continue.
static int g_failures = 0;

#define EXPECT_EQ(a,b) do { \
    auto _A = (a); \
    auto _B = (b); \
    if(_A != _B) { \
        std::cerr << "EXPECT_EQ failed: " #a " (" << _A << ") != " #b " (" << _B << ")" << std::endl; \
        ++g_failures; \
    } \
} while(0)

#define EXPECT_TRUE(a) do { \
    if(!(a)) { std::cerr << "EXPECT_TRUE failed: " #a " is false" << std::endl; ++g_failures; } \
} while(0)

#define EXPECT_FALSE(a) do { \
    if(a) { std::cerr << "EXPECT_FALSE failed: " #a " is true" << std::endl; ++g_failures; } \
} while(0)

// Helper to create a minimal Vdbe instance attached to a dummy sqlite3 db.
// We rely on the real Vdbe struct layout provided by the included headers.
static Vdbe* makeVdbe(sqlite3 *db, int nFkConstraint, int prepFlags) {
    // Allocate a zero-initialized Vdbe to avoid uninitialized fields.
    Vdbe *p = (Vdbe*)calloc(1, sizeof(Vdbe));
    if(!p) return nullptr;
    p->db = db;
    p->nFkConstraint = nFkConstraint;
    p->prepFlags = prepFlags;
    // p->rc and p->errorAction will be left as 0 by calloc, which is a sensible default
    return p;
}

// Helper to create a dummy sqlite3 db object with specified deferred-constraint counters.
// We rely on the internal sqlite3 struct fields nDeferredCons and nDeferredImmCons.
// The actual sqlite3 struct in the project must expose these fields (as per <FOCAL_CLASS_DEP>).
static sqlite3* makeDb(int nDeferredCons, int nDeferredImmCons) {
    sqlite3 *db = (sqlite3*)calloc(1, sizeof(sqlite3));
    if(!db) return nullptr;
    // The internal structure is defined in sqliteInt.h; these members are used by sqlite3VdbeCheckFk.
    db->nDeferredCons = nDeferredCons;
    db->nDeferredImmCons = nDeferredImmCons;
    return db;
}

// Entry point for all tests.
int main() {
    // Test 1: No constraints, both deferred and non-deferred paths should return SQLITE_OK.
    // Expect no changes to p->rc and p->errorAction; function should simply return SQLITE_OK.
    {
        sqlite3 *db = makeDb(0, 0);           // no deferred constraints
        Vdbe *p = makeVdbe(db, 0, 0);         // no FK constraint, no SAVESQL
        int ret = sqlite3VdbeCheckFk(p, 0);   // deferred = 0
        // Expected: OK and no error state set
        EXPECT_EQ(ret, SQLITE_OK);
        // rc not set by this path; ensure it remains its initial value (0 due to calloc)
        EXPECT_EQ(p->rc, 0);
        // errorAction should remain default (0)
        EXPECT_EQ(p->errorAction, 0);
        free(p);
        free(db);
    }

    // Test 2: Deferred and there exists a deferred constraint; since SAVESQL is not set,
    // the function should return SQLITE_ERROR and set rc to FOREINGKEY and action to Abort.
    {
        sqlite3 *db = makeDb(1, 0);            // some deferred constraints exist
        Vdbe *p = makeVdbe(db, 0, 0);          // no FK constraint
        int ret = sqlite3VdbeCheckFk(p, 1);     // deferred = 1
        // Expect: error code due to constraint failure
        EXPECT_EQ(ret, SQLITE_ERROR);
        // rc should indicate a foreign key constraint failure
        EXPECT_EQ(p->rc, SQLITE_CONSTRAINT_FOREIGNKEY);
        // action should be aborted
        EXPECT_EQ(p->errorAction, OE_Abort);
        free(p);
        free(db);
    }

    // Test 3: Deferred path with SAVESQL flag set should return SQLITE_CONSTRAINT_FOREIGNKEY
    // (and not SQLITE_ERROR) while still setting rc and errorAction.
    {
        sqlite3 *db = makeDb(2, 0);                 // two deferred constraints
        Vdbe *p = makeVdbe(db, 0, SQLITE_PREPARE_SAVESQL); // defer path, with SAVESQL flag
        int ret = sqlite3VdbeCheckFk(p, 1);        // deferred = 1
        // Expect: FOREIGNKEY constraint (ret equals the specific code)
        EXPECT_EQ(ret, SQLITE_CONSTRAINT_FOREIGNKEY);
        // rc and action should be set
        EXPECT_EQ(p->rc, SQLITE_CONSTRAINT_FOREIGNKEY);
        EXPECT_EQ(p->errorAction, OE_Abort);
        free(p);
        free(db);
    }

    // Test 4: Non-deferred path with p->nFkConstraint > 0; without SAVESQL -> SQLITE_ERROR
    // with rc set to FOREIGNKEY and action Abort.
    {
        sqlite3 *db = makeDb(0, 0);
        Vdbe *p = makeVdbe(db, 1, 0);           // one FK constraint present
        int ret = sqlite3VdbeCheckFk(p, 0);     // deferred = 0
        EXPECT_EQ(ret, SQLITE_ERROR);
        EXPECT_EQ(p->rc, SQLITE_CONSTRAINT_FOREIGNKEY);
        EXPECT_EQ(p->errorAction, OE_Abort);
        free(p);
        free(db);
    }

    // Test 5: Non-deferred path with p->nFkConstraint > 0; SAVESQL flag set
    // -> SQLITE_CONSTRAINT_FOREIGNKEY, rc/abort values set.
    {
        sqlite3 *db = makeDb(0, 0);
        Vdbe *p = makeVdbe(db, 1, SQLITE_PREPARE_SAVESQL);
        int ret = sqlite3VdbeCheckFk(p, 0);
        EXPECT_EQ(ret, SQLITE_CONSTRAINT_FOREIGNKEY);
        EXPECT_EQ(p->rc, SQLITE_CONSTRAINT_FOREIGNKEY);
        EXPECT_EQ(p->errorAction, OE_Abort);
        free(p);
        free(db);
    }

    // Summary
    if(g_failures == 0) {
        std::cout << "All tests passed (no failures reported)." << std::endl;
        return 0;
    } else {
        std::cerr << g_failures << " test(s) failed." << std::endl;
        return 1;
    }
}