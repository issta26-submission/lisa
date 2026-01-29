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
//<ID> 433
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *create_stmt = NULL;
    sqlite3_stmt *insert_stmt = NULL;
    sqlite3_stmt *select_stmt = NULL;
    sqlite3_blob *blob = NULL;
    const void *filename = (const void *)L":memory:";
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_insert = "INSERT INTO t(data) VALUES(x'01020304');";
    const char *sql_select = "SELECT rowid, data FROM t;";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_blob_open = 0;
    int rc_blob_read = 0;
    long long rowid = 0;
    void *buf = NULL;
    int stmt_status_val = 0;
    int autocommit = 0;

    // step 2: Initialize / Configure / Setup
    sqlite3_initialize();
    rc_open = sqlite3_open16(filename, &db);
    rc_prepare = sqlite3_prepare_v2(db, sql_create, -1, &create_stmt, NULL);
    rc_step = sqlite3_step(create_stmt);
    rc_finalize = sqlite3_finalize(create_stmt);
    rc_prepare = sqlite3_prepare_v2(db, sql_insert, -1, &insert_stmt, NULL);
    rc_step = sqlite3_step(insert_stmt);
    rc_finalize = sqlite3_finalize(insert_stmt);
    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &select_stmt, NULL);
    rc_step = sqlite3_step(select_stmt);
    rowid = sqlite3_column_int64(select_stmt, 0);

    // step 3: Core operations
    rc_blob_open = sqlite3_blob_open(db, "main", "t", "data", (sqlite3_int64)rowid, 0, &blob);
    buf = sqlite3_malloc(4);
    memset(buf, 0, 4);
    rc_blob_read = sqlite3_blob_read(blob, buf, 4, 0);
    stmt_status_val = sqlite3_stmt_status(select_stmt, 0, 0);
    autocommit = sqlite3_get_autocommit(db);

    // step 4: Cleanup / Validate
    rc_finalize = sqlite3_finalize(select_stmt);
    sqlite3_blob_close(blob);
    buf = sqlite3_realloc(buf, 0);
    sqlite3_close_v2(db);
    db = NULL;
    blob = NULL;
    create_stmt = NULL;
    insert_stmt = NULL;
    select_stmt = NULL;
    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_blob_open;
    (void)rc_blob_read;
    (void)rowid;
    (void)buf;
    (void)stmt_status_val;
    (void)autocommit;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;

    // API sequence test completed successfully
    return 66;
}