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
//<ID> 582
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
    char *sql_text = NULL;
    const char *tail = NULL;
    const char sql_template[] = "CREATE TABLE t(id INTEGER PRIMARY KEY, value TEXT);";
    sqlite3_context *ctx = NULL;
    void *buf = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int complete = 0;
    int threadsafe = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open(":memory:", &db);
    builder = sqlite3_str_new(db);
    sqlite3_str_append(builder, sql_template, (int)(sizeof(sql_template) - 1));
    sql_text = sqlite3_str_finish(builder);
    complete = sqlite3_complete(sql_text);
    threadsafe = sqlite3_threadsafe();
    buf = sqlite3_realloc(NULL, 128);
    memset(buf, 0, 128);

    // step 3: Operate / Validate
    rc_prep = sqlite3_prepare_v3(db, sql_text, -1, 0, &stmt, &tail);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    sqlite3_result_null(ctx);
    buf = sqlite3_realloc(buf, 256);
    memset(buf, 0, 256);

    // step 4: Cleanup
    sql_text = (char*)sqlite3_realloc(sql_text, 0);
    buf = sqlite3_realloc(buf, 0);
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)complete;
    (void)threadsafe;
    (void)builder;
    (void)stmt;
    (void)db;
    (void)sql_text;
    (void)tail;
    (void)ctx;
    (void)buf;
    (void)sql_template;

    // API sequence test completed successfully
    return 66;
}