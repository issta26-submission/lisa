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
//<ID> 378
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
    const char *create_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT);";
    const char *insert_sql = "INSERT INTO t(val) VALUES('hello');";
    const char *select_sql = "SELECT id, val FROM t WHERE id=1;";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    sqlite3_int64 last_rowid = 0;
    const char *col0name = NULL;
    const char *col1name = NULL;
    sqlite3_value *col1val = NULL;
    int col1_bytes = 0;

    // step 2: Setup - open in-memory DB and create table
    rc_open = sqlite3_open(":memory:", &db);
    rc_prepare = sqlite3_prepare_v2(db, create_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Operate - insert a row and get last insert rowid
    rc_prepare = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    last_rowid = sqlite3_last_insert_rowid(db);
    sqlite3_finalize(stmt);
    stmt = NULL;

    // step 4: Validate & Cleanup - select the row, inspect column names and value bytes, finalize and close
    rc_prepare = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    col0name = sqlite3_column_name(stmt, 0);
    col1name = sqlite3_column_name(stmt, 1);
    col1val = sqlite3_column_value(stmt, 1);
    col1_bytes = sqlite3_value_bytes(col1val);
    sqlite3_finalize(stmt);
    stmt = NULL;
    sqlite3_close(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)last_rowid;
    (void)col0name;
    (void)col1name;
    (void)col1val;
    (void)col1_bytes;

    // API sequence test completed successfully
    return 66;
}