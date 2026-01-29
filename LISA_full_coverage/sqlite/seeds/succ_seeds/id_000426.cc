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
//<ID> 426
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
    sqlite3_backup *backup = NULL;
    const char *create_sql = "CREATE TABLE IF NOT EXISTS demo(id INTEGER PRIMARY KEY, val TEXT);";
    const char *insert_sql = "INSERT INTO demo(val) VALUES('x');";
    const char *select_sql = "SELECT id, val FROM demo;";
    int rc_open = 0;
    int rc_dbconf = 0;
    int rc_ext = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_backup_step = 0;
    int rc_close = 0;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open(":memory:", &db);
    rc_ext = sqlite3_extended_result_codes(db, 1);
    rc_dbconf = sqlite3_db_config(db, 0);

    // step 3: Operate (prepare/step statements) and exercise backup_step
    rc_prepare = sqlite3_prepare_v2(db, create_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prepare = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prepare = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;
    rc_backup_step = sqlite3_backup_step(backup, 1);

    // step 4: Validate / Cleanup
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_dbconf;
    (void)rc_ext;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_backup_step;
    (void)rc_close;
    (void)backup;
    (void)create_sql;
    (void)insert_sql;
    (void)select_sql;

    // API sequence test completed successfully
    return 66;
}