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
//<ID> 580
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_str *builder = NULL;
    char *constructed_sql = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_context *ctx = NULL;
    void *re_alloc_ptr = NULL;
    void *re_alloc_freed = NULL;
    const char *errmsg = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int complete_flag = 0;
    int threadsafe_flag = 0;

    rc_open = sqlite3_open(":memory:", &db);
    threadsafe_flag = sqlite3_threadsafe();
    builder = sqlite3_str_new(db);

    // step 2: Configure
    sqlite3_str_append(builder, "CREATE TABLE t(id INTEGER);", 27);
    constructed_sql = sqlite3_str_finish(builder);
    complete_flag = sqlite3_complete(constructed_sql);

    // step 3: Operate / Validate
    rc_prepare = sqlite3_prepare_v3(db, constructed_sql, -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_result_null(ctx);
    re_alloc_ptr = sqlite3_realloc(NULL, 128);
    re_alloc_freed = sqlite3_realloc(re_alloc_ptr, 0);
    errmsg = sqlite3_errmsg(db);

    // step 4: Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)threadsafe_flag;
    (void)builder;
    (void)constructed_sql;
    (void)rc_prepare;
    (void)rc_step;
    (void)re_alloc_ptr;
    (void)re_alloc_freed;
    (void)errmsg;
    (void)rc_finalize;
    (void)rc_close;
    (void)complete_flag;
    (void)ctx;
    (void)stmt;
    (void)db;

    // API sequence test completed successfully
    return 66;
}