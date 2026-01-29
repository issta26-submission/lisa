#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <fcntl.h>
//<ID> 159
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    int rc = 0;
    // Open an in-memory database
    rc = sqlite3_open(":memory:", &db);
    if (rc != 0 || db == nullptr) {
        // Failed to open DB
        return 1;
    }

    // Create a simple collation using a non-capturing lambda convertible to a function pointer.
    int (*collate_fn)(void *, int, const void *, int, const void *) =
        [](void * /*pArg*/, int /*lenA*/, const void * /*a*/, int /*lenB*/, const void * /*b*/) -> int {
            // trivial collation: treat all values as equal
            return 0;
        };
    // eTextRep set to 1 as a placeholder (typical values are constants like SQLITE_UTF8)
    rc = sqlite3_create_collation(db, "my_trivial_collation", 1, nullptr, collate_fn);
    // Ignore rc here; proceed with cleanup/other calls below.

    // Prepare a simple SQL statement
    const char *sql = "CREATE TABLE IF NOT EXISTS demo(id INTEGER PRIMARY KEY, val TEXT);";
    const char *pzTail = nullptr;
    rc = sqlite3_prepare(db, sql, -1, &stmt, &pzTail);
    // If prepared successfully, finalize it. If not, just continue to other API calls.
    if (rc == 0 && stmt != nullptr) {
        // Normally would sqlite3_step() here, but not required for this sequence.
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }

    // Query status information using sqlite3_status64
    sqlite3_int64 current = 0;
    sqlite3_int64 highwater = 0;
    // op code 0 used as a placeholder; resetFlag = 0 (do not reset)
    (void)sqlite3_status64(0, &current, &highwater, 0);

    // Attempt to recover a snapshot for the "main" database (no-op for simple in-memory DB)
    (void)sqlite3_snapshot_recover(db, "main");

    // Demonstrate cancelling an auto-extension by passing a no-op function pointer.
    void (*cancel_fn)() = []() {};
    (void)sqlite3_cancel_auto_extension(cancel_fn);

    // Clean up and close DB
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }

    // Indicate success
    return 66;
}