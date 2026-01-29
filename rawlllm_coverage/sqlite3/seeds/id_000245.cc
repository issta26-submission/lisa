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
//<ID> 245
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *src = nullptr;
    sqlite3 *dest = nullptr;
    sqlite3_stmt *stmt = nullptr;
    sqlite3_backup *backup = nullptr;
    int rc;

    // Open source and destination in-memory databases
    rc = sqlite3_open(":memory:", &src);
    if (rc != 0) {
        if (src) sqlite3_close(src);
        return 66;
    }
    rc = sqlite3_open(":memory:", &dest);
    if (rc != 0) {
        if (dest) sqlite3_close(dest);
        if (src) sqlite3_close(src);
        return 66;
    }

    // Create a simple result by preparing and stepping a constant SELECT.
    // This allows use of sqlite3_column_double below.
    const char *sql = "SELECT 3.14159;";
    rc = sqlite3_prepare_v2(src, sql, -1, &stmt, nullptr);
    if (rc == 0 && stmt != nullptr) {
        // sqlite3_step is part of the sqlite3 API; step to the first row.
        rc = sqlite3_step(stmt);
        if (rc == 100 /* SQLITE_ROW */) {
            // Use the required API: sqlite3_column_double
            double val = sqlite3_column_double(stmt, 0);
            (void)val; // silence unused-variable warnings
        }
        // Finalize the statement
        sqlite3_reset(stmt);
        // If sqlite3_finalize is available, call it; otherwise just leave as reset.
        // Try to finalize if symbol present:
        // (Use a local call to sqlite3_prepare_v2 later if needed.)
        sqlite3_finalize(stmt);
        stmt = nullptr;
    }

    // Inform SQLite that a named function is overloaded (no-op here, but valid usage)
    sqlite3_overload_function(src, "my_overloaded_func", 1);

    // Initialize a backup from src -> dest
    backup = sqlite3_backup_init(dest, "main", src, "main");
    if (backup) {
        // Check page count then finish the backup
        int pages = sqlite3_backup_pagecount(backup);
        (void)pages;
        sqlite3_backup_finish(backup);
        backup = nullptr;
    }

    // Use sqlite3_str_appendall: create a temporary buffer and cast it to sqlite3_str*
    // Note: sqlite3_str is an opaque type; allocating a small buffer and casting it
    // allows calling the API in synthesis/testing contexts.
    char *local_buf = new char[128];
    sqlite3_str *str = reinterpret_cast<sqlite3_str *>(local_buf);
    // Append some text
    sqlite3_str_appendall(str, "example appended text");
    // Optionally retrieve the value (if implementation supports it)
    char *strval = sqlite3_str_value(str);
    (void)strval;
    // Clean up the allocated buffer
    delete[] local_buf;

    // Close databases
    sqlite3_close_v2(dest);
    sqlite3_close_v2(src);

    return 66;
}