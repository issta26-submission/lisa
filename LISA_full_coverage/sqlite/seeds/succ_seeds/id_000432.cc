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
//<ID> 432
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    const void *filename = (const void *)L":memory:";
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_insert = "INSERT INTO t(data) VALUES(x'01020304');";
    const char *sql_select = "SELECT data FROM t WHERE id = 1;";
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_stmt *stmt_select = NULL;
    sqlite3_blob *blob = NULL;
    void *read_buf = NULL;
    int rc_open = sqlite3_open16(filename, &db);
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_blob_open = 0;
    int rc_blob_read = 0;
    int stmt_status_val = 0;
    int autocommit_flag = 0;

    // step 2: Configure / Operate - create table and insert a blob literal
    rc_prepare = sqlite3_prepare_v2(db, sql_create, -1, &stmt_create, NULL);
    rc_step = sqlite3_step(stmt_create);
    sqlite3_finalize(stmt_create);
    stmt_create = NULL;

    rc_prepare = sqlite3_prepare_v2(db, sql_insert, -1, &stmt_insert, NULL);
    rc_step = sqlite3_step(stmt_insert);
    sqlite3_finalize(stmt_insert);
    stmt_insert = NULL;

    // step 3: Operate / Validate - prepare a select, inspect statement status and autocommit, open blob and read it
    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt_select, NULL);
    stmt_status_val = sqlite3_stmt_status(stmt_select, 0, 0);
    autocommit_flag = sqlite3_get_autocommit(db);

    rc_blob_open = sqlite3_blob_open(db, "main", "t", "data", 1, 0, &blob);

    read_buf = sqlite3_malloc(4);
    memset(read_buf, 0, 4);
    rc_blob_read = sqlite3_blob_read(blob, read_buf, 4, 0);

    // step 4: Cleanup
    sqlite3_finalize(stmt_select);
    stmt_select = NULL;
    sqlite3_blob_close(blob);
    blob = NULL;
    sqlite3_realloc(read_buf, 0);
    read_buf = NULL;
    sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_blob_open;
    (void)rc_blob_read;
    (void)stmt_status_val;
    (void)autocommit_flag;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;

    // API sequence test completed successfully
    return 66;
}