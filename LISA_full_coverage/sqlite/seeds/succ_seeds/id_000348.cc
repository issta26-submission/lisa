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
//<ID> 348
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
    sqlite3_mutex *mtx = NULL;
    void *mem = NULL;
    const char *create_sql = "CREATE TABLE t(a INTEGER);";
    const char *insert_sql = "INSERT INTO t(a) VALUES(?);";
    const char *select_sql = "SELECT a FROM t;";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_bind = 0;
    int rc_reset = 0;
    int complete_flag = 0;
    sqlite3_int64 rowval = 0;
    int rc_close = 0;

    // step 2: Setup
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    complete_flag = sqlite3_complete(create_sql);
    mem = sqlite3_malloc64((sqlite3_uint64)128);

    // step 3: Core operations
    rc_prepare = sqlite3_prepare_v3(db, create_sql, -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_reset = sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prepare = sqlite3_prepare_v3(db, insert_sql, -1, 0, &stmt, NULL);
    rc_bind = sqlite3_bind_int(stmt, 1, 42);
    rc_step = sqlite3_step(stmt);
    rc_reset = sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prepare = sqlite3_prepare_v3(db, select_sql, -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rowval = sqlite3_column_int64(stmt, 0);
    sqlite3_set_last_insert_rowid(db, rowval);
    rc_reset = sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    stmt = NULL;

    mem = sqlite3_realloc64(mem, 0);
    mem = NULL;
    sqlite3_mutex_leave(mtx);

    // step 4: Cleanup
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_bind;
    (void)rc_reset;
    (void)complete_flag;
    (void)rowval;
    (void)rc_close;
    (void)db;
    (void)stmt;
    (void)mtx;
    (void)mem;
    // API sequence test completed successfully
    return 66;
}