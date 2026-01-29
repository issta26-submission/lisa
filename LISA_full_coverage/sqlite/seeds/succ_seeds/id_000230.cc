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
//<ID> 230
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
    sqlite3_str *str = NULL;
    sqlite3_context *ctx = NULL;
    sqlite3_value *val = NULL;
    sqlite3_mutex *mtx = NULL;
    const void *pzTail = NULL;
    const void *sql16 = (const void*)u"CREATE TABLE t(x INTEGER);";
    int rc_init = 0;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int rc_err = 0;
    int held = 0;

    // step 2: Initialize library and open an in-memory database
    rc_init = sqlite3_initialize();
    rc_open = sqlite3_open(":memory:", &db);
    rc_err = sqlite3_errcode(db);

    // step 3: Prepare a UTF-16 SQL statement, operate on string and context/value, check mutex
    rc_prep = sqlite3_prepare16_v2(db, sql16, -1, &stmt, &pzTail);
    str = sqlite3_str_new(db);
    sqlite3_str_appendall(str, " -- appended SQL metadata");
    mtx = sqlite3_mutex_alloc(0);
    sqlite3_mutex_free(mtx);
    sqlite3_result_value(ctx, val);

    // step 4: Validate and cleanup
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_init;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_finalize;
    (void)rc_close;
    (void)rc_err;
    (void)held;
    (void)ctx;
    (void)val;
    (void)mtx;
    (void)pzTail;
    (void)sql16;
    (void)str;

    // API sequence test completed successfully
    return 66;
}