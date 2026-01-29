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
//<ID> 423
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3 *dst = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_backup *backup = NULL;
    const char *create_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, v TEXT);";
    const char *insert_sql = "INSERT INTO t(v) VALUES('hello');";
    const char *select_sql = "SELECT v FROM t;";
    int rc_open_src = 0;
    int rc_open_dst = 0;
    int rc_exec_create = 0;
    int rc_exec_insert = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_dbconf = 0;
    int rc_ext = 0;
    int rc_backup_step = 0;
    int rc_close_dst = 0;
    int rc_close_src = 0;

    // step 2: Setup - open source DB, create table and row, enable extended result codes, configure DB
    rc_open_src = sqlite3_open(":memory:", &db);
    rc_exec_create = sqlite3_exec(db, create_sql, NULL, NULL, NULL);
    rc_exec_insert = sqlite3_exec(db, insert_sql, NULL, NULL, NULL);
    rc_ext = sqlite3_extended_result_codes(db, 1);
    rc_dbconf = sqlite3_db_config(db, 0);

    // step 3: Operate - prepare and step a SELECT, then perform an in-memory backup of the DB
    rc_prepare = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;
    rc_open_dst = sqlite3_open(":memory:", &dst);
    backup = sqlite3_backup_init(dst, "main", db, "main");
    rc_backup_step = sqlite3_backup_step(backup, -1);
    sqlite3_backup_finish(backup);
    backup = NULL;

    // step 4: Cleanup - close databases and tidy local references
    rc_close_dst = sqlite3_close_v2(dst);
    rc_close_src = sqlite3_close_v2(db);
    dst = NULL;
    db = NULL;

    (void)rc_open_src;
    (void)rc_open_dst;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_dbconf;
    (void)rc_ext;
    (void)rc_backup_step;
    (void)rc_close_dst;
    (void)rc_close_src;
    (void)create_sql;
    (void)insert_sql;
    (void)select_sql;

    // API sequence test completed successfully
    return 66;
}