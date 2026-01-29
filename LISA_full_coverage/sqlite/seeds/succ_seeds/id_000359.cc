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
//<ID> 359
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
    const char *create_sql = "CREATE TABLE IF NOT EXISTS t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *insert_sql = "INSERT INTO t(data) VALUES(x'0102');";
    const char *select_sql = "SELECT data FROM t WHERE id=1;";
    const char *errmsg = NULL;
    int rc_open = 0;
    int rc_enable_on = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;

    // step 2: Setup - open in-memory DB and enable extension loading
    rc_open = sqlite3_open(":memory:", &db);
    rc_enable_on = sqlite3_enable_load_extension(db, 1);

    // step 3: Core operations - create table, insert row, select and inspect statement status
    rc_prepare = sqlite3_prepare_v2(db, create_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prepare = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prepare = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_stmt_scanstatus_reset(stmt);
    errmsg = sqlite3_errmsg(db);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 4: Cleanup - close any blob handle, disable extension loading, tidy up
    sqlite3_blob_close(blob);
    sqlite3_enable_load_extension(db, 0);

    (void)rc_open;
    (void)rc_enable_on;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)errmsg;
    (void)blob;
    (void)db;
    (void)create_sql;
    (void)insert_sql;
    (void)select_sql;
    // API sequence test completed successfully
    return 66;
}