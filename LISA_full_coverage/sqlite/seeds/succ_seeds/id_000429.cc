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
//<ID> 429
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
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY);";
    const char *sql_insert = "INSERT INTO t(id) VALUES(42);";
    const char *sql_select = "SELECT id FROM t;";
    int rc_open = 0;
    int rc_extended = 0;
    int rc_dbconfig = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_backup_step = 0;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open(":memory:", &db);
    rc_extended = sqlite3_extended_result_codes(db, 1);
    rc_dbconfig = sqlite3_db_config(db, 0);

    // step 3: Operate - create table, insert a row, query it, and exercise backup step
    rc_prepare = sqlite3_prepare_v2(db, sql_create, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);

    rc_prepare = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);

    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);

    rc_backup_step = sqlite3_backup_step(backup, 1);

    // step 4: Cleanup / Validate / Finalize
    sqlite3_close_v2(db);
    db = NULL;
    stmt = NULL;
    backup = NULL;

    (void)rc_open;
    (void)rc_extended;
    (void)rc_dbconfig;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_backup_step;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;

    // API sequence test completed successfully
    return 66;
}