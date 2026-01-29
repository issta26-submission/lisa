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
//<ID> 435
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_stmt *stmt_select = NULL;
    sqlite3_blob *blob = NULL;
    const void *filename = (const void *)L":memory:";
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_insert = "INSERT INTO t(data) VALUES(x'01020304');";
    const char *sql_select = "SELECT rowid FROM t LIMIT 1;";
    void *buf = NULL;
    int buf_size = 4;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_blob_open = 0;
    int rc_blob_read = 0;
    int stmt_status_val = 0;
    int autocommit = 0;
    sqlite3_int64 rowid = 0;

    // step 2: Setup / Configure - open DB and prepare statements
    rc_open = sqlite3_open16(filename, &db);
    rc_prepare = sqlite3_prepare_v2(db, sql_create, -1, &stmt_create, NULL);
    rc_step = sqlite3_step(stmt_create);
    rc_prepare = sqlite3_prepare_v2(db, sql_insert, -1, &stmt_insert, NULL);
    rc_step = sqlite3_step(stmt_insert);
    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt_select, NULL);

    // step 3: Operate / Validate - inspect statement status, get rowid, open blob and read into malloc'd buffer
    stmt_status_val = sqlite3_stmt_status(stmt_select, 0, 0);
    rc_step = sqlite3_step(stmt_select);
    rowid = sqlite3_column_int64(stmt_select, 0);
    rc_blob_open = sqlite3_blob_open(db, "main", "t", "data", rowid, 0, &blob);
    buf = sqlite3_malloc(buf_size);
    memset(buf, 0, buf_size);
    rc_blob_read = sqlite3_blob_read(blob, buf, buf_size, 0);
    autocommit = sqlite3_get_autocommit(db);

    // step 4: Cleanup - finalize statements, close blob and DB, free malloc'd buffer
    sqlite3_finalize(stmt_create);
    sqlite3_finalize(stmt_insert);
    sqlite3_finalize(stmt_select);
    sqlite3_blob_close(blob);
    sqlite3_realloc(buf, 0);
    sqlite3_close_v2(db);
    db = NULL;
    stmt_create = NULL;
    stmt_insert = NULL;
    stmt_select = NULL;
    blob = NULL;
    buf = NULL;

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_blob_open;
    (void)rc_blob_read;
    (void)stmt_status_val;
    (void)autocommit;
    (void)rowid;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;

    // API sequence test completed successfully
    return 66;
}