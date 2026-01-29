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
//<ID> 239
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_str *str = NULL;
    sqlite3_context *ctx = NULL;
    sqlite3_value *val = NULL;
    sqlite3_mutex *mtx = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int held = 0;
    int err = 0;
    sqlite3_int64 lastid = 0;

    rc_open = sqlite3_open(":memory:", &db);
    rc_prep = sqlite3_prepare16_v2(db, (const void*)L"CREATE TABLE IF NOT EXISTS t(x INTEGER);", -1, &stmt, NULL);
    sqlite3_result_value(ctx, val);
    held = sqlite3_mutex_held(mtx);
    err = sqlite3_errcode(db);
    lastid = sqlite3_last_insert_rowid(db);
    sqlite3_finalize(stmt);
    sqlite3_db_cacheflush(db);

    (void)rc_open;
    (void)rc_prep;
    (void)held;
    (void)err;
    (void)lastid;
    (void)str;
    (void)ctx;
    (void)val;
    (void)mtx;

    return 66;
}