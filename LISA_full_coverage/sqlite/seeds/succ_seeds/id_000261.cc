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
//<ID> 261
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_stmt *stmt_select = NULL;
    const unsigned char *col_text = NULL;
    const char *sql_text = NULL;
    char *buf = NULL;
    sqlite3_uint64 buf_size = (sqlite3_uint64)256;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_close = 0;

    // step 2: Setup - open an in-memory database
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Core operations - create table, insert a row, select the row, read column and SQL, format into buffer
    rc_prep = sqlite3_prepare_v3(db, "CREATE TABLE t(x TEXT);", -1, 0, &stmt_create, NULL);
    rc_step = sqlite3_step(stmt_create);
    rc_prep = sqlite3_prepare_v3(db, "INSERT INTO t(x) VALUES('hello');", -1, 0, &stmt_insert, NULL);
    rc_step = sqlite3_step(stmt_insert);
    rc_prep = sqlite3_prepare_v3(db, "SELECT x FROM t;", -1, 0, &stmt_select, NULL);
    rc_step = sqlite3_step(stmt_select);
    col_text = sqlite3_column_text(stmt_select, 0);
    sql_text = sqlite3_sql(stmt_select);
    buf = (char *)sqlite3_realloc64(NULL, buf_size);
    sqlite3_snprintf((int)buf_size, buf, "col: %s; sql: %s", col_text ? (const char *)col_text : "", sql_text ? sql_text : "");

    // step 4: Validate and Cleanup - free buffer, finalize statements, and close database using close_v2
    sqlite3_free(buf);
    sqlite3_finalize(stmt_select);
    sqlite3_finalize(stmt_insert);
    sqlite3_finalize(stmt_create);
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_close;
    (void)col_text;
    (void)sql_text;
    (void)buf_size;

    // API sequence test completed successfully
    return 66;
}