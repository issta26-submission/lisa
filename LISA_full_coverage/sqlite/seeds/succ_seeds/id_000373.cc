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
//<ID> 373
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
    const char *create_sql = "CREATE TABLE IF NOT EXISTS t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *insert_sql = "INSERT INTO t(data) VALUES('hello');";
    const char *select_sql = "SELECT data FROM t WHERE id=1;";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    const char *colname = NULL;
    sqlite3_value *val = NULL;
    int val_bytes = 0;
    sqlite3_int64 lastid = 0;

    // step 2: Setup - open in-memory database
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Core operations - create table, insert a row, query it and inspect column/value
    rc_prepare = sqlite3_prepare_v2(db, create_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prepare = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;

    lastid = sqlite3_last_insert_rowid(db);

    rc_prepare = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    val = sqlite3_column_value(stmt, 0);
    val_bytes = sqlite3_value_bytes(val);
    colname = sqlite3_column_name(stmt, 0);

    // step 4: Cleanup - finalize statement and close database
    sqlite3_finalize(stmt);
    stmt = NULL;
    sqlite3_close(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)colname;
    (void)val;
    (void)val_bytes;
    (void)lastid;
    (void)create_sql;
    (void)insert_sql;
    (void)select_sql;
    // API sequence test completed successfully
    return 66;
}