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
//<ID> 427
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
    const void *filename = (const void *)L":memory:";
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT);";
    const char *sql_insert = "INSERT INTO t(v) VALUES('alpha');";
    const char *sql_select = "SELECT id, v FROM t;";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step_create = 0;
    int rc_step_insert = 0;
    int rc_step_select = 0;
    int rc_dbcfg = 0;
    int rc_backup_step = 0;
    int rc_ext = 0;
    int col_count = 0;

    // step 2: Initialize
    rc_open = sqlite3_open16(filename, &db);

    // step 3: Configure
    rc_ext = sqlite3_extended_result_codes(db, 1);
    rc_dbcfg = sqlite3_db_config(db, 1);

    // step 4: Operate / Validate
    rc_prepare = sqlite3_prepare_v2(db, sql_create, -1, &stmt, NULL);
    rc_step_create = sqlite3_step(stmt);
    stmt = NULL;

    rc_prepare = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    rc_step_insert = sqlite3_step(stmt);
    stmt = NULL;

    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc_step_select = sqlite3_step(stmt);
    col_count = sqlite3_column_count(stmt);

    rc_backup_step = sqlite3_backup_step(backup, 1);

    // step 5: Cleanup
    sqlite3_close_v2(db);
    db = NULL;
    stmt = NULL;
    backup = NULL;

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step_create;
    (void)rc_step_insert;
    (void)rc_step_select;
    (void)rc_dbcfg;
    (void)rc_backup_step;
    (void)rc_ext;
    (void)col_count;
    (void)filename;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;

    // API sequence test completed successfully
    return 66;
}