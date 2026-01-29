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
//<ID> 431
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
    const void *filename = (const void *)L":memory:";
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_insert = "INSERT INTO t(data) VALUES(x'010203');";
    const char *sql_select = "SELECT id, data FROM t WHERE id = 1;";
    char *errmsg = NULL;
    void *buf = NULL;
    int rc_open = 0;
    int rc_exec_create = 0;
    int rc_exec_insert = 0;
    int rc_prepare = 0;
    int rc_blob_open = 0;
    int rc_blob_read = 0;
    int autocommit = 0;
    int stmt_status = 0;
    int buf_size = 16;

    // step 2: Setup / Initialize
    rc_open = sqlite3_open16(filename, &db);
    rc_exec_create = sqlite3_exec(db, sql_create, NULL, NULL, &errmsg);
    rc_exec_insert = sqlite3_exec(db, sql_insert, NULL, NULL, &errmsg);

    // step 3: Operate - prepare a statement, inspect statement status, open blob and read it
    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    stmt_status = sqlite3_stmt_status(stmt, 0, 0);
    buf = sqlite3_malloc(buf_size);
    memset(buf, 0, (size_t)buf_size);
    rc_blob_open = sqlite3_blob_open(db, "main", "t", "data", 1, 0, &blob);
    rc_blob_read = sqlite3_blob_read(blob, buf, buf_size, 0);
    autocommit = sqlite3_get_autocommit(db);

    // step 4: Cleanup / Validate / Finalize
    sqlite3_realloc(buf, 0);
    sqlite3_finalize(stmt);
    sqlite3_blob_close(blob);
    sqlite3_close_v2(db);
    db = NULL;
    stmt = NULL;
    blob = NULL;
    buf = NULL;
    errmsg = NULL;

    (void)rc_open;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)rc_prepare;
    (void)rc_blob_open;
    (void)rc_blob_read;
    (void)autocommit;
    (void)stmt_status;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;

    // API sequence test completed successfully
    return 66;
}