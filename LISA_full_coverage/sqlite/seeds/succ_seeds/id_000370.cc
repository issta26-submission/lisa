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
//<ID> 370
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
    const char *create_sql = "CREATE TABLE IF NOT EXISTS t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *insert_sql = "INSERT INTO t(data) VALUES(x'010203');";
    const char *select_sql = "SELECT data, id FROM t WHERE id=1;";
    const char *tail = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    sqlite3_value *val = NULL;
    int val_bytes = 0;
    const char *colname0 = NULL;
    const char *colname1 = NULL;
    sqlite3_int64 lastid = 0;

    // step 2: Initialize - open in-memory database
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Operate - create table, insert a row, select and inspect results
    rc_prepare = sqlite3_prepare_v2(db, create_sql, -1, &stmt_create, &tail);
    rc_step = sqlite3_step(stmt_create);
    sqlite3_finalize(stmt_create);
    stmt_create = NULL;

    rc_prepare = sqlite3_prepare_v2(db, insert_sql, -1, &stmt_insert, NULL);
    rc_step = sqlite3_step(stmt_insert);
    sqlite3_finalize(stmt_insert);
    stmt_insert = NULL;

    lastid = sqlite3_last_insert_rowid(db);

    rc_prepare = sqlite3_prepare_v2(db, select_sql, -1, &stmt_select, NULL);
    rc_step = sqlite3_step(stmt_select);
    val = sqlite3_column_value(stmt_select, 0);
    val_bytes = sqlite3_value_bytes(val);
    colname0 = sqlite3_column_name(stmt_select, 0);
    colname1 = sqlite3_column_name(stmt_select, 1);
    sqlite3_finalize(stmt_select);
    stmt_select = NULL;

    // step 4: Cleanup - close database and tidy up
    sqlite3_close(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)tail;
    (void)val;
    (void)val_bytes;
    (void)colname0;
    (void)colname1;
    (void)lastid;
    (void)create_sql;
    (void)insert_sql;
    (void)select_sql;
    // API sequence test completed successfully
    return 66;
}