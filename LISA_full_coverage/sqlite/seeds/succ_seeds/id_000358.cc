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
//<ID> 358
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    const char *dbfile = ":memory:";
    const char *create_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *insert_sql = "INSERT INTO t(id, data) VALUES(1, x'010203');";
    const char *select_sql = "SELECT data FROM t WHERE id=1;";
    const void *col_blob = NULL;
    const char *errmsg = NULL;
    int rc_open = 0;
    int rc_enable = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_blob_open = 0;
    int rc_blob_close = 0;
    int rc_close = 0;

    // step 2: Setup - open database and enable loadable extensions
    rc_open = sqlite3_open(dbfile, &db);
    rc_enable = sqlite3_enable_load_extension(db, 1);

    // step 3: Core operations - create table, insert a row, select blob, inspect stmt, manage blob handle
    rc_prepare = sqlite3_prepare_v2(db, create_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prepare = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prepare = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    col_blob = sqlite3_column_blob(stmt, 0);
    sqlite3_stmt_scanstatus_reset(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_blob_open = sqlite3_blob_open(db, "main", "t", "data", 1, 0, &blob);
    rc_blob_close = sqlite3_blob_close(blob);
    errmsg = sqlite3_errmsg(db);

    // step 4: Cleanup - close database and tidy up
    rc_close = sqlite3_close(db);

    (void)col_blob;
    (void)errmsg;
    (void)rc_open;
    (void)rc_enable;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_blob_open;
    (void)rc_blob_close;
    (void)rc_close;
    (void)dbfile;
    // API sequence test completed successfully
    return 66;
}