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
//<ID> 425
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *src = NULL;
    sqlite3 *dst = NULL;
    sqlite3_backup *backup = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *create_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT);";
    const char *insert_sql = "INSERT INTO t(val) VALUES('row1');";
    const char *select_sql = "SELECT COUNT(*) FROM t;";
    int rc_open_src = 0;
    int rc_open_dst = 0;
    int rc_exec_create = 0;
    int rc_exec_insert = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_backup_step = 0;
    int rc_db_config = 0;
    int rc_ext = 0;
    sqlite3_int64 backup_remaining = 0;

    // step 2: Setup - open source and destination in-memory databases and populate source
    rc_open_src = sqlite3_open(":memory:", &src);
    rc_open_dst = sqlite3_open(":memory:", &dst);
    rc_exec_create = sqlite3_exec(src, create_sql, NULL, NULL, NULL);
    rc_exec_insert = sqlite3_exec(src, insert_sql, NULL, NULL, NULL);

    // step 3: Configure / Operate / Validate - enable extended result codes, tweak DB config, perform backup, and query destination
    rc_ext = sqlite3_extended_result_codes(src, 1);
    rc_db_config = sqlite3_db_config(src, 0);
    backup = sqlite3_backup_init(dst, "main", src, "main");
    rc_backup_step = sqlite3_backup_step(backup, -1);
    backup_remaining = sqlite3_backup_remaining(backup);
    sqlite3_backup_finish(backup);
    rc_prepare = sqlite3_prepare_v2(dst, select_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);

    // step 4: Cleanup - finalize statement and close databases
    sqlite3_finalize(stmt);
    stmt = NULL;
    sqlite3_close_v2(dst);
    sqlite3_close_v2(src);
    dst = NULL;
    src = NULL;

    (void)rc_open_src;
    (void)rc_open_dst;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_backup_step;
    (void)rc_db_config;
    (void)rc_ext;
    (void)backup_remaining;
    (void)backup;

    // API sequence test completed successfully
    return 66;
}