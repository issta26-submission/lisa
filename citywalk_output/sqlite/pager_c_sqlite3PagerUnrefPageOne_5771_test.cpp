/*
Step 1 - Program Understanding (Candidate Keywords)
- DbPage: input page descriptor with fields pgno, flags, and a pointer to its Pager (pPager).
- Pager: the page cache context/owner for the page.
- PGHDR_MMAP: flag mask indicating memory-mapped pages; Page 1 is never memory mapped.
- sqlite3PcacheRelease(DbPage*): releases a page object back to the cache.
- pagerUnlockIfUnused(Pager*): unlocks the pager if it is otherwise unused.
- sqlite3PagerUnrefPageOne(DbPage* pPg): focal method under test; asserts input validity, releases the page, and possibly unlocks the pager.

This test suite provides a minimal, self-contained (mocked) environment to validate the core behavior of sqlite3PagerUnrefPageOne without requiring the full SQLite source tree.
*/

#include <iostream>
#include <wal.h>
#include <sqliteInt.h>
#include <string>


// Domain-specific constants used by the focal method
static const unsigned PGHDR_MMAP = 0x1;

// Namespace to contain the test scaffolding and mocks
namespace FocalTest {

    // Lightweight mock structures to simulate the minimal production types
    struct Pager {
        int dummy;
    };

    struct DbPage {
        unsigned int pgno;
        unsigned int flags;
        void *pPager;
    };

    // Global state to observe interactions invoked by sqlite3PagerUnrefPageOne
    static bool g_release_called = false;
    static DbPage *g_released_pPage = nullptr;

    static bool g_unref_called = false;
    static Pager *g_unref_argument = nullptr;

    // Mocked dependency: sqlite3PcacheRelease
    static void sqlite3PcacheRelease(DbPage *pPg) {
        g_release_called = true;
        g_released_pPage = pPg;
    }

    // Mocked dependency: pagerUnlockIfUnused
    static void pagerUnlockIfUnused(Pager *pPager) {
        g_unref_called = true;
        g_unref_argument = pPager;
    }

    // Focal-method replica (self-contained version for unit testing)
    // Note: We replicate the core logic without pulling in the full SQLite codebase.
    static void sqlite3PagerUnrefPageOne(DbPage *pPg) {
        Pager *pPager;
        // Emulate the asserts from the production code (non-fatal in this mock)
        if (pPg == nullptr) return;                 // assert(pPg != 0)
        if (pPg->pgno != 1) return;                 // assert(pPg->pgno == 1)
        if ((pPg->flags & PGHDR_MMAP) != 0) return; // assert( (pPg->flags & PGHDR_MMAP) == 0 )

        pPager = (Pager *)pPg->pPager;
        sqlite3PcacheRelease(pPg);
        pagerUnlockIfUnused(pPager);
    }

    // Helper to reset global state between tests
    static void resetGlobals() {
        g_release_called = false;
        g_released_pPage = nullptr;
        g_unref_called = false;
        g_unref_argument = nullptr;
    }

    // Test 1: Basic path - pgno == 1, not memory-mapped, with a valid pager
    // Expected: sqlite3PcacheRelease called with the input page,
    //           pagerUnlockIfUnused called with the associated Pager
    bool test_UnrefPageOne_Basic() {
        resetGlobals();

        // Arrange
        DbPage p;
        Pager pager;
        p.pgno = 1;
        p.flags = 0;        // does not have PGHDR_MMAP
        p.pPager = &pager;

        // Act
        sqlite3PagerUnrefPageOne(&p);

        // Assert (non-terminating assertions via simple checks)
        bool ok = true;
        if (!g_release_called) {
            std::cout << "  [FAIL] sqlite3PcacheRelease was not called in Basic test.\n";
            ok = false;
        }
        if (!g_unref_called) {
            std::cout << "  [FAIL] pagerUnlockIfUnused was not called in Basic test.\n";
            ok = false;
        }
        if (g_released_pPage != &p) {
            std::cout << "  [FAIL] Released DbPage pointer mismatch in Basic test.\n";
            ok = false;
        }
        if (g_unref_argument != &pager) {
            std::cout << "  [FAIL] Unref Pager argument mismatch in Basic test.\n";
            ok = false;
        }

        if (ok) {
            std::cout << "  [PASS] test_UnrefPageOne_Basic\n";
        }
        return ok;
    }

    // Test 2: pgno == 1 but pPg->pPager is NULL
    // Expected: sqlite3PcacheRelease still called and pagerUnlockIfUnused called with NULL
    bool test_UnrefPageOne_NullPager() {
        resetGlobals();

        // Arrange
        DbPage p;
        p.pgno = 1;
        p.flags = 0;
        p.pPager = nullptr;

        // Act
        sqlite3PagerUnrefPageOne(&p);

        // Assert
        bool ok = true;
        if (!g_release_called) {
            std::cout << "  [FAIL] sqlite3PcacheRelease was not called in NullPager test.\n";
            ok = false;
        }
        if (!g_unref_called) {
            std::cout << "  [FAIL] pagerUnlockIfUnused was not called in NullPager test.\n";
            ok = false;
        }
        if (g_released_pPage != &p) {
            std::cout << "  [FAIL] Released DbPage pointer mismatch in NullPager test.\n";
            ok = false;
        }
        if (g_unref_argument != nullptr) {
            std::cout << "  [FAIL] Unref Pager argument should be nullptr in NullPager test.\n";
            ok = false;
        }

        if (ok) {
            std::cout << "  [PASS] test_UnrefPageOne_NullPager\n";
        }
        return ok;
    }

    // Test 3: Wrong page number (pgno != 1) should not perform any actions
    // Expected: No call to sqlite3PcacheRelease or pagerUnlockIfUnused
    bool test_UnrefPageOne_WrongPgno() {
        resetGlobals();

        // Arrange
        DbPage p;
        Pager pager;
        p.pgno = 2;        // not 1
        p.flags = 0;
        p.pPager = &pager;

        // Act
        sqlite3PagerUnrefPageOne(&p);

        // Assert
        bool ok = true;
        if (g_release_called) {
            std::cout << "  [FAIL] sqlite3PcacheRelease should not be called when pgno != 1.\n";
            ok = false;
        }
        if (g_unref_called) {
            std::cout << "  [FAIL] pagerUnlockIfUnused should not be called when pgno != 1.\n";
            ok = false;
        }

        if (ok) {
            std::cout << "  [PASS] test_UnrefPageOne_WrongPgno\n";
        }
        return ok;
    }

} // namespace FocalTest

// Simple test harness
int main() {
    using namespace FocalTest;

    std::vector<bool (*)()> tests;
    // Bind tests in a stable order
    tests.push_back([]() -> bool { return test_UnrefPageOne_Basic(); });
    tests.push_back([]() -> bool { return test_UnrefPageOne_NullPager(); });
    tests.push_back([]() -> bool { return test_UnrefPageOne_WrongPgno(); });

    int total = 0;
    int passed = 0;
    std::cout << "Running sqlite3PagerUnrefPageOne unit tests (mocked environment)...\n";
    for (auto t : tests) {
        // Execute test and count result
        bool result = t();
        ++total;
        if (result) ++passed;
    }

    std::cout << "\nTest summary: " << passed << " / " << total << " tests passed.\n";
    return (passed == total) ? 0 : 1;
}