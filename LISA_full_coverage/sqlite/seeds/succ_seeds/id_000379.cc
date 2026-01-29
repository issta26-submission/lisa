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
//<ID> 379
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
    const char *insert_sql = "INSERT INTO t(data) VALUES(x'0102');";
    const char *select_sql = "SELECT data FROM t WHERE id=?;";
    sqlite3_value *val = NULL;
    const char *colname = NULL;
    int bytes = 0;
    int rc = 0;
    sqlite3_int64 lastid = 0;

    // step 2: Setup - open database and create table
    rc = sqlite3_open(":memory:", &db);
    rc = sqlite3_prepare_v2(db, create_sql, -1, &stmt, NULL);
    rc = sqlite3_step(stmt);
    rc = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Core operations - insert a row, get last rowid, select and inspect value/column
    rc = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    rc = sqlite3_step(stmt);
    rc = sqlite3_finalize(stmt);
    stmt = NULL;
    lastid = sqlite3_last_insert_rowid(db);

    rc = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    rc = sqlite3_bind_int64(stmt, 1, lastid);
    rc = sqlite3_step(stmt);
    val = sqlite3_column_value(stmt, 0);
    bytes = sqlite3_value_bytes(val);
    colname = sqlite3_column_name(stmt, 0);
    rc = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 4: Cleanup
    rc = sqlite3_close(db);
    db = NULL;

    (void)rc;
    (void)val;
    (void)bytes;
    (void)colname;
    (void)lastid;

    // API sequence test completed successfully
    return 66;
}