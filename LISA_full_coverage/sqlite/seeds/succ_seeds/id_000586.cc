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
//<ID> 586
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
    char *temp_buf = NULL;
    char *final_sql = NULL;
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_step_create = 0;
    int rc_finalize_create = 0;
    int rc_prep_query = 0;
    int rc_step_query = 0;
    int rc_finalize_query = 0;
    int rc_close = 0;
    int threadsafe = 0;
    int sql_complete = 0;
    int count_val = 0;

    rc_open = sqlite3_open(":memory:", &db);
    threadsafe = sqlite3_threadsafe();
    temp_buf = (char*)sqlite3_realloc(NULL, 128);
    builder = sqlite3_str_new(db);

    // step 2: Configure
    sqlite3_str_append(builder, "CREATE TABLE t(id INTEGER PRIMARY KEY, value TEXT);", (int)(sizeof("CREATE TABLE t(id INTEGER PRIMARY KEY, value TEXT);") - 1));
    final_sql = sqlite3_str_finish(builder);
    sql_complete = sqlite3_complete(final_sql);

    // step 3: Operate / Validate
    rc_prep_create = sqlite3_prepare_v3(db, final_sql, -1, 0, &stmt, NULL);
    rc_step_create = sqlite3_step(stmt);
    rc_finalize_create = sqlite3_finalize(stmt);
    rc_prep_query = sqlite3_prepare_v3(db, "SELECT count(*) FROM t;", -1, 0, &stmt, NULL);
    rc_step_query = sqlite3_step(stmt);
    count_val = sqlite3_column_int(stmt, 0);
    sqlite3_result_null(ctx);
    rc_finalize_query = sqlite3_finalize(stmt);

    // step 4: Cleanup
    rc_close = sqlite3_close_v2(db);
    temp_buf = (char*)sqlite3_realloc(temp_buf, 0);

    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_finalize_create;
    (void)rc_prep_query;
    (void)rc_step_query;
    (void)rc_finalize_query;
    (void)rc_close;
    (void)threadsafe;
    (void)sql_complete;
    (void)count_val;
    (void)final_sql;
    (void)builder;
    (void)stmt;
    (void)ctx;

    // API sequence test completed successfully
    return 66;
}