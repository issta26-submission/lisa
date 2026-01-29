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
//<ID> 354
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
    const char *msg = NULL;
    const char *sql_create = "CREATE TABLE IF NOT EXISTS demo(id INTEGER PRIMARY KEY, val TEXT);";
    const char *sql_insert = "INSERT INTO demo(val) VALUES('hello');";
    const char *sql_select = "SELECT val FROM demo;";
    int rc_open = 0;
    int rc_enable = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;

    // step 2: Setup - open in-memory database and enable loadable extensions
    rc_open = sqlite3_open(":memory:", &db);
    rc_enable = sqlite3_enable_load_extension(db, 1);

    // step 3: Core operations - create table, insert a row, select and inspect statement, reset scan status, finalize statements
    rc_prepare = sqlite3_prepare_v2(db, sql_create, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prepare = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_stmt_scanstatus_reset(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    msg = sqlite3_errmsg(db);

    // step 4: Cleanup - close blob (NULL-safe in this sequence context), disable extension loading, and close database
    sqlite3_blob_close(blob);
    sqlite3_enable_load_extension(db, 0);
    rc_close = sqlite3_close(db);

    (void)msg;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;
    (void)rc_open;
    (void)rc_enable;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)blob;
    // API sequence test completed successfully
    return 66;
}