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
//<ID> 269
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = nullptr;
    int rc = sqlite3_open_v2(":memory:", &db, 0, nullptr);

    // Use sqlite3_get_table to create a table, insert a row, and select it.
    char **results = nullptr;
    int nrow = 0;
    int ncol = 0;
    char *errmsg = nullptr;
    if (db) {
        // Multiple statements: CREATE, INSERT, SELECT. The final SELECT's results (if any) are returned.
        sqlite3_get_table(db,
                          "CREATE TABLE IF NOT EXISTS t(a); INSERT INTO t(a) VALUES(7); SELECT a FROM t;",
                          &results, &nrow, &ncol, &errmsg);
    }

    // Prepare a statement and step it, then use sqlite3_column_int on the resulting row.
    sqlite3_stmt *stmt = nullptr;
    int extracted_value = 0;
    if (db) {
        rc = sqlite3_prepare_v2(db, "SELECT 123 AS num", -1, &stmt, nullptr);
        if (rc == 0 && stmt) {
            rc = sqlite3_step(stmt);
            // SQLITE_ROW is 100 in the SQLite C API. Check for a row before calling sqlite3_column_int.
            if (rc == 100) {
                extracted_value = sqlite3_column_int(stmt, 0);
            }
        }
    }

    // Allocate a mutex, query sqlite3_mutex_notheld, then free the mutex.
    sqlite3_mutex *mx = sqlite3_mutex_alloc(0);
    int mutex_notheld_result = 0;
    if (mx) {
        mutex_notheld_result = sqlite3_mutex_notheld(mx);
        sqlite3_mutex_free(mx);
    }

    // Call sqlite3_cancel_auto_extension with a null pointer (no-op cancellation).
    sqlite3_cancel_auto_extension(nullptr);

    // Clean up table results if any.
    if (results) {
        sqlite3_free_table(results);
    }

    // Silence unused-variable warnings in case some calls failed.
    (void)rc;
    (void)errmsg;
    (void)stmt; // sqlite3_finalize isn't referenced in provided API list; leave as-is.
    (void)extracted_value;
    (void)mutex_notheld_result;
    (void)db;

    return 66;
}