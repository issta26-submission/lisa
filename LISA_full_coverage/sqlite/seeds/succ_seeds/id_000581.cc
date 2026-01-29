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
//<ID> 581
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_str *builder = NULL;
    sqlite3_context *ctx = NULL;
    const char init_sql[] = "CREATE TABLE t(id INTEGER PRIMARY KEY, value TEXT);";
    char *sql_from_builder = NULL;
    char *realloc_buf = NULL;
    const char *tail = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int complete_res = 0;
    int threadsafe_res = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open(":memory:", &db);
    builder = sqlite3_str_new(db);
    sqlite3_str_append(builder, init_sql, (int)(sizeof(init_sql) - 1));
    sql_from_builder = sqlite3_str_finish(builder);
    realloc_buf = (char *)sqlite3_realloc(NULL, 64);

    // step 3: Operate / Validate
    complete_res = sqlite3_complete(sql_from_builder);
    rc_prepare = sqlite3_prepare_v3(db, sql_from_builder, -1, 0, &stmt, &tail);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    threadsafe_res = sqlite3_threadsafe();
    sqlite3_result_null(ctx);

    // step 4: Cleanup
    sqlite3_realloc(realloc_buf, 0);
    sqlite3_realloc(sql_from_builder, 0);
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)builder;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)complete_res;
    (void)threadsafe_res;
    (void)stmt;
    (void)tail;
    (void)ctx;

    // API sequence test completed successfully
    return 66;
}