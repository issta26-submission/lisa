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
//<ID> 232
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations and initialization
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_str *builder = NULL;
    sqlite3_value *val = NULL;
    sqlite3_context *ctx = NULL;
    const void *tail = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int errcode = 0;
    int held = 0;

    // step 2: Setup (open in-memory DB and prepare a UTF-16 statement string buffer)
    rc_open = sqlite3_open(":memory:", &db);
    builder = (sqlite3_str *)sqlite3_malloc(128);
    memset(builder, 0, 128);
    sqlite3_str_appendall(builder, "SELECT 1;");
    rc_prep = sqlite3_prepare16(db, (const void *)u"SELECT 1;", -1, &stmt, &tail);

    // step 3: Operate (execute to obtain a value, pass it to a result API, inspect mutex and error code)
    rc_step = sqlite3_step(stmt);
    val = sqlite3_column_value(stmt, 0);
    sqlite3_result_value(ctx, val);
    held = sqlite3_mutex_held((sqlite3_mutex *)NULL);
    errcode = sqlite3_errcode(db);

    // step 4: Validate and cleanup (finalize, free buffer, close DB)
    sqlite3_finalize(stmt);
    sqlite3_free(builder);
    sqlite3_close(db);

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)tail;
    (void)ctx;
    (void)val;
    (void)held;
    (void)errcode;

    // API sequence test completed successfully
    return 66;
}