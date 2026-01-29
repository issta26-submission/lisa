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
//<ID> 376
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
    sqlite3_value *colval = NULL;
    const char *create_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *insert_sql = "INSERT INTO t(data) VALUES(x'010203');";
    const char *select_sql = "SELECT data FROM t WHERE id=?;";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    sqlite3_int64 lastrowid = 0;
    int data_bytes = 0;
    const char *col_name = NULL;

    // step 2: Setup - open in-memory DB and create table
    rc_open = sqlite3_open(":memory:", &db);
    rc_prepare = sqlite3_prepare_v2(db, create_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Operate - insert a row and obtain last insert rowid
    rc_prepare = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    lastrowid = sqlite3_last_insert_rowid(db);

    // step 4: Validate and Cleanup - select inserted blob, inspect bytes and column name, then close
    rc_prepare = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    rc_step = sqlite3_bind_int64(stmt, 1, lastrowid);
    rc_step = sqlite3_step(stmt);
    colval = sqlite3_column_value(stmt, 0);
    data_bytes = sqlite3_value_bytes(colval);
    col_name = sqlite3_column_name(stmt, 0);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_finalize = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)lastrowid;
    (void)data_bytes;
    (void)col_name;
    (void)colval;
    (void)create_sql;
    (void)insert_sql;
    (void)select_sql;

    // API sequence test completed successfully
    return 66;
}