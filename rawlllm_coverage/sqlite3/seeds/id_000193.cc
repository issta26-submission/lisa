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
//<ID> 193
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt = nullptr;
    // Open an in-memory database
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        return 1;
    }

    // Enable loadable extensions temporarily
    sqlite3_enable_load_extension(db, 1);

    // Prepare a simple statement
    const char *sql = "SELECT 1";
    int rc = sqlite3_prepare_v3(db, sql, -1, 0, &stmt, nullptr);
    if (rc == SQLITE_OK && stmt) {
        // Execute a single step (ignore result)
        sqlite3_step(stmt);

        // Reset any scan-status counters on the prepared statement
        sqlite3_stmt_scanstatus_reset(stmt);

        // Reset the statement so it can be reused/finalized
        sqlite3_reset(stmt);
    }

    // Use sqlite3_str utilities to build a small formatted string
    // Note: sqlite3_str_new is part of the public API and returns sqlite3_str*.
    sqlite3_str *str = sqlite3_str_new(db);
    if (str) {
        sqlite3_str_appendf(str, "example: %d", 12345);

        // Inspect any error code on the string builder
        int serr = sqlite3_str_errcode(str);

        // Obtain the current string value without finishing
        char *cur = sqlite3_str_value(str);

        // Suppress unused-variable warnings by casting to void
        (void)serr;
        (void)cur;

        // Finish the string (returns a heap-allocated char* in the sqlite API)
        // This also frees the sqlite3_str object internally.
        char *finished = sqlite3_str_finish(str);

        // The returned finished pointer is typically freed with sqlite3_free in real use.
        // To avoid depending on sqlite3_free here, just avoid further use.
        (void)finished;
    }

    // Disable loadable extensions again for safety
    sqlite3_enable_load_extension(db, 0);

    // Clean up prepared statement if present
    if (stmt) {
        // Finalize is the proper way to free a statement; use if available.
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }

    // Close the database connection
    sqlite3_close(db);
    db = nullptr;

    return 66;
}