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
//<ID> 375
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
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB, name TEXT);";
    const char *sql_insert = "INSERT INTO t(data, name) VALUES(x'0102', 'Alice');";
    const char *sql_select = "SELECT data, name FROM t WHERE id = 1;";
    const char *pzTail = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    sqlite3_value *val = NULL;
    int data_bytes = 0;
    const char *col0 = NULL;
    const char *col1 = NULL;
    sqlite3_int64 last_rowid = 0;

    // step 2: Setup - open in-memory database
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Operate - create table, insert a row, select the row and inspect
    rc_prepare = sqlite3_prepare_v2(db, sql_create, -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prepare = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;

    last_rowid = sqlite3_last_insert_rowid(db);

    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    col0 = sqlite3_column_name(stmt, 0);
    col1 = sqlite3_column_name(stmt, 1);
    val = sqlite3_column_value(stmt, 0);
    data_bytes = sqlite3_value_bytes(val);
    sqlite3_finalize(stmt);
    stmt = NULL;

    // step 4: Cleanup - close database and tidy up
    sqlite3_close(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)pzTail;
    (void)col0;
    (void)col1;
    (void)data_bytes;
    (void)val;
    (void)last_rowid;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;
    (void)db;
    (void)stmt;

    // API sequence test completed successfully
    return 66;
}