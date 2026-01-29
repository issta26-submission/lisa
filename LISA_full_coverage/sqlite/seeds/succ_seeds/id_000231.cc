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
//<ID> 231
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_mutex *mtx = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_bind = 0;
    int rc_finalize = 0;
    int err_off = 0;
    sqlite3_int64 changes = 0;
    char sqlbuf[128];

    // Build a SQL fragment into a local buffer
    sqlite3_snprintf((int)sizeof(sqlbuf), sqlbuf, "CREATE TABLE IF NOT EXISTS t(x INTEGER);");

    // Open an in-memory database (UTF-16)
    rc_open = sqlite3_open16((const void *)u":memory:", &db);

    // Prepare a UTF-16 statement
    rc_prep = sqlite3_prepare16_v2(db, (const void *)u"SELECT 1;", -1, &stmt, NULL);

    // Bind a text value to the prepared statement (deliver a value to statement context)
    rc_bind = sqlite3_bind_text64(stmt, 1, "example", (sqlite3_uint64)7, NULL, (unsigned char)1);

    // Inspect the database mutex and an error-related value
    mtx = sqlite3_db_mutex(db);
    err_off = sqlite3_error_offset(db);
    changes = sqlite3_changes64(db);

    // Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    sqlite3_close_v2(db);

    // Silence unused-variable warnings
    (void)rc_open;
    (void)rc_prep;
    (void)rc_bind;
    (void)rc_finalize;
    (void)mtx;
    (void)err_off;
    (void)changes;
    (void)sqlbuf;

    return 66;
}