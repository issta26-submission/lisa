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
//<ID> 244
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = nullptr;
    int rc = sqlite3_open(":memory:", &db);
    if (rc != 0 || db == nullptr) {
        if (db) sqlite3_close_v2(db);
        return 1;
    }

    // Busy handler callback: always return 0 (do not retry)
    int (*busy_cb)(void *, int) = [](void *p, int count) -> int {
        (void)p; (void)count;
        return 0;
    };
    // Install busy handler (ignore non-zero return for this test)
    sqlite3_busy_handler(db, busy_cb, nullptr);

    // Create a scalar function that returns a double via sqlite3_result_double.
    void (*myfunc)(sqlite3_context *, int, sqlite3_value **) =
        [](sqlite3_context *ctx, int argc, sqlite3_value **argv) -> void {
            (void)argc; (void)argv;
            // Provide a constant double result
            sqlite3_result_double(ctx, 2.718281828);
        };
    // Register the function as "mydouble" with 1 argument (eTextRep set to 1)
    sqlite3_create_function(db, "mydouble", 1, 1, nullptr, myfunc, nullptr, nullptr);

    // Create a table
    sqlite3_stmt *stmt = nullptr;
    const char *sql_create = "CREATE TABLE t(a INTEGER);";
    rc = sqlite3_prepare_v2(db, sql_create, -1, &stmt, nullptr);
    if (rc == 0 && stmt) {
        sqlite3_step(stmt);            // execute
        sqlite3_reset(stmt);          // required usage
        // Note: sqlite3_finalize is not used here (not in allowed list)
    }

    // Insert a row so last_insert_rowid() has a value
    const char *sql_insert = "INSERT INTO t(a) VALUES (NULL);";
    rc = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, nullptr);
    if (rc == 0 && stmt) {
        sqlite3_step(stmt);            // perform insert
        sqlite3_reset(stmt);          // required usage after step
    }

    // Retrieve the last insert rowid
    sqlite3_int64 last_row = sqlite3_last_insert_rowid(db);
    (void)last_row; // use to avoid unused-variable warning

    // Query the custom function to ensure sqlite3_result_double path is exercised
    const char *sql_select = "SELECT mydouble(a) FROM t;";
    rc = sqlite3_prepare_v2(db, sql_select, -1, &stmt, nullptr);
    if (rc == 0 && stmt) {
        if (sqlite3_step(stmt) == 100 /* SQLITE_ROW */) {
            double val = sqlite3_column_double(stmt, 0);
            (void)val; // use to avoid unused-variable warning
        }
        sqlite3_reset(stmt); // required usage
    }

    // Close the database
    sqlite3_close_v2(db);

    return 66;
}