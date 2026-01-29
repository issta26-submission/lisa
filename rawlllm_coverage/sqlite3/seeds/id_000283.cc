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
//<ID> 283
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    int rc = sqlite3_initialize();
    if (rc != 0) {
        // Initialization failed
        return rc;
    }

    sqlite3 *db = nullptr;
    // Open an in-memory database (use the usual open flags)
    rc = sqlite3_open_v2(":memory:", &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
    if (rc != 0 || db == nullptr) {
        // Failed to open DB
        return rc ? rc : -1;
    }

    const char *sql = "CREATE TABLE test(id INTEGER PRIMARY KEY, val TEXT);";
    sqlite3_stmt *stmt = nullptr;
    const char *pzTail = nullptr;

    // Prepare the SQL statement
    rc = sqlite3_prepare(db, sql, -1, &stmt, &pzTail);
    if (rc != 0) {
        // Clean up and return error
        sqlite3_close_v2(db);
        return rc;
    }

    // Optionally inspect the prepared SQL text (no-op here)
    (void)sqlite3_sql(stmt);

    // Reset the prepared statement to a clean state
    (void)sqlite3_reset(stmt);

    // Close the database connection
    sqlite3_close_v2(db);

    return 66;
}