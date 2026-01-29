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
//<ID> 438
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    const void *filename = (const void *)L":memory:";
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_insert = "INSERT INTO t(data) VALUES(x'01020304');";
    const char *sql_select = "SELECT data FROM t WHERE id=1;";
    char *errmsg = NULL;
    void *buffer = NULL;
    int rc_initialize = 0;
    int rc_open = 0;
    int rc_exec_create = 0;
    int rc_exec_insert = 0;
    int rc_prepare = 0;
    int rc_blob_open = 0;
    int rc_blob_read = 0;
    int rc_blob_close = 0;
    int rc_stmt_status = 0;
    int autocommit = 0;
    int buffer_size = 16;
    int stmt_status_op = 0;
    (void)errmsg;

    // step 2: Setup - initialize SQLite and open an in-memory database, create table and insert a row
    rc_initialize = sqlite3_initialize();
    rc_open = sqlite3_open16(filename, &db);
    rc_exec_create = sqlite3_exec(db, sql_create, NULL, NULL, &errmsg);
    rc_exec_insert = sqlite3_exec(db, sql_insert, NULL, NULL, &errmsg);

    // step 3: Operate - prepare a statement, open a blob handle to the inserted row, allocate buffer and read blob, query statement status and autocommit state
    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc_blob_open = sqlite3_blob_open(db, "main", "t", "data", 1, 0, &blob);
    buffer = sqlite3_malloc(buffer_size);
    rc_blob_read = sqlite3_blob_read(blob, buffer, buffer_size, 0);
    rc_stmt_status = sqlite3_stmt_status(stmt, stmt_status_op, 0);
    autocommit = sqlite3_get_autocommit(db);

    // step 4: Cleanup / Validate - close blob, close DB, free buffer, clear locals
    rc_blob_close = sqlite3_blob_close(blob);
    sqlite3_close_v2(db);
    free(buffer);
    db = NULL;
    stmt = NULL;
    blob = NULL;
    buffer = NULL;
    errmsg = NULL;

    (void)rc_initialize;
    (void)rc_open;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)rc_prepare;
    (void)rc_blob_open;
    (void)rc_blob_read;
    (void)rc_blob_close;
    (void)rc_stmt_status;
    (void)autocommit;
    (void)buffer_size;
    (void)stmt_status_op;

    // API sequence test completed successfully
    return 66;
}