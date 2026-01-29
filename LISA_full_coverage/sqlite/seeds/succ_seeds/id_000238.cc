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
//<ID> 238
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
    sqlite3_str *builder = NULL;
    char *sql_utf8 = NULL;
    const void *tail = NULL;
    sqlite3_value *val = NULL;
    sqlite3_mutex *mtx = NULL;
    sqlite3_context *ctx = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int held = 0;
    int err = 0;

    // step 2: Setup (open DB and build SQL using sqlite3_str APIs)
    rc_open = sqlite3_open(":memory:", &db);
    builder = sqlite3_str_new(db);
    sqlite3_str_appendall(builder, "CREATE TABLE IF NOT EXISTS t(x INTEGER); SELECT 123;");
    sql_utf8 = sqlite3_str_finish(builder);

    // step 3: Core operations (prepare a UTF-16 statement, execute, extract value, propagate to result, inspect mutex and error code)
    rc_prep = sqlite3_prepare16(db, (const void *)L"SELECT 123;", -1, &stmt, &tail);
    rc_step = sqlite3_step(stmt);
    val = sqlite3_column_value(stmt, 0);
    sqlite3_result_value(ctx, val);
    mtx = sqlite3_db_mutex(db);
    held = sqlite3_mutex_held(mtx);
    err = sqlite3_errcode(db);

    // step 4: Cleanup and validation
    rc_finalize = sqlite3_finalize(stmt);
    sqlite3_free(sql_utf8);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)builder;
    (void)tail;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)held;
    (void)err;
    (void)val;
    (void)ctx;
    (void)mtx;

    // API sequence test completed successfully
    return 66;
}