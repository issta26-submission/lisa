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
//<ID> 468
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *src_db = NULL;
    sqlite3 *dst_db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_backup *backup = NULL;
    sqlite3_context *ctx = NULL;
    const char *sql_create = "CREATE TABLE t(x INTEGER);";
    const char *sql_insert = "INSERT INTO t VALUES(42);";
    const char *sql_select = "SELECT x FROM t;";
    const char *libver = NULL;
    char *libver_copy = NULL;
    int rc_open_src = 0;
    int rc_open_dst = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_backup_step = 0;
    int fetched_value = 0;
    int len = 0;
    memset(&fetched_value, 0, sizeof(fetched_value));

    // step 2: Initialize / Configure
    rc_open_src = sqlite3_open_v2(":memory:", &src_db, 0, NULL);
    rc_open_dst = sqlite3_open_v2(":memory:", &dst_db, 0, NULL);

    rc_prepare = sqlite3_prepare_v2(src_db, sql_create, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prepare = sqlite3_prepare_v2(src_db, sql_insert, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Operate (backup + inspect library version)
    backup = sqlite3_backup_init(dst_db, "main", src_db, "main");
    rc_backup_step = sqlite3_backup_step(backup, -1);
    sqlite3_backup_finish(backup);
    backup = NULL;

    libver = sqlite3_libversion();
    len = (int)strlen(libver);
    libver_copy = (char *)sqlite3_malloc(len + 1);
    memcpy(libver_copy, libver, (size_t)len + 1);

    // step 4: Validate / Cleanup
    rc_prepare = sqlite3_prepare_v2(dst_db, sql_select, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    fetched_value = sqlite3_column_int(stmt, 0);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    sqlite3_result_pointer(ctx, (void *)libver_copy, "libver", sqlite3_free);
    rc_open_src = sqlite3_close_v2(src_db);
    rc_open_dst = sqlite3_close_v2(dst_db);
    src_db = NULL;
    dst_db = NULL;
    libver_copy = NULL;

    (void)rc_open_src;
    (void)rc_open_dst;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_backup_step;
    (void)libver;
    (void)fetched_value;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;

    // API sequence test completed successfully
    return 66;
}