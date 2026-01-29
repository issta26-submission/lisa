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
//<ID> 191
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = nullptr;
    // Open an in-memory database (use flags = 0 to avoid relying on SQLITE_* constants).
    int rc = sqlite3_open_v2(":memory:", &db, 0, nullptr);

    // Create a string builder attached to the DB (builder may be NULL on allocation failure).
    sqlite3_str *builder = sqlite3_str_new(db);
    if (builder) {
        // Append a simple SQL statement.
        sqlite3_str_appendall(builder, "CREATE TABLE demo(id INTEGER PRIMARY KEY, val TEXT);");
    }

    // Finalize the string builder to get a nul-terminated SQL string.
    char *sql = sqlite3_str_finish(builder);

    // Check whether the SQL string is a complete statement.
    int is_complete = 0;
    if (sql) {
        is_complete = sqlite3_complete(sql);
    }

    // Query the extended error code for the opened database handle (if any).
    int ext_err = 0;
    if (db) {
        ext_err = sqlite3_extended_errcode(db);
    }

    // Attempt a blob write with a NULL blob handle to exercise the API path.
    sqlite3_blob *blob = nullptr;
    const char sample_data[] = "data";
    int blob_rc = sqlite3_blob_write(blob, sample_data, (int)(sizeof(sample_data) - 1), 0);

    // Close the database if opened.
    if (db) {
        sqlite3_close_v2(db);
    }

    // Silence unused-variable warnings; the values above simulate an API usage sequence.
    (void)rc;
    (void)is_complete;
    (void)ext_err;
    (void)blob_rc;
    (void)sql; // intentionally not freed here (sqlite3_str_finish allocation management omitted)

    return 66;
}