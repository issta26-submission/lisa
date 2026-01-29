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
//<ID> 377
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
    sqlite3_value *colval = NULL;
    const char *create_sql = "CREATE TABLE IF NOT EXISTS t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *insert_sql = "INSERT INTO t(data) VALUES(x'010203');";
    const char *select_sql = "SELECT data FROM t WHERE id=1;";
    const char *colname0 = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int bytes = 0;
    sqlite3_int64 lastrowid = 0;

    // step 2: Setup - open in-memory DB
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Operate - create table, insert a row, query the row
    rc_prepare = sqlite3_prepare_v2(db, create_sql, -1, &stmt_create, NULL);
    rc_step = sqlite3_step(stmt_create);
    rc_finalize = sqlite3_finalize(stmt_create);
    stmt_create = NULL;

    rc_prepare = sqlite3_prepare_v2(db, insert_sql, -1, &stmt_insert, NULL);
    rc_step = sqlite3_step(stmt_insert);
    rc_finalize = sqlite3_finalize(stmt_insert);
    stmt_insert = NULL;

    lastrowid = sqlite3_last_insert_rowid(db);

    rc_prepare = sqlite3_prepare_v2(db, select_sql, -1, &stmt_select, NULL);
    rc_step = sqlite3_step(stmt_select);
    colval = sqlite3_column_value(stmt_select, 0);
    bytes = sqlite3_value_bytes(colval);
    colname0 = sqlite3_column_name(stmt_select, 0);

    // step 4: Validate & Cleanup
    rc_finalize = sqlite3_finalize(stmt_select);
    stmt_select = NULL;
    rc_finalize = sqlite3_close(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)bytes;
    (void)colname0;
    (void)colval;
    (void)lastrowid;
    (void)create_sql;
    (void)insert_sql;
    (void)select_sql;

    // API sequence test completed successfully
    return 66;
}