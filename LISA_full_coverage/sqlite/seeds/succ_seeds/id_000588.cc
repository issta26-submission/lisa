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
//<ID> 588
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
    char *tmp_buf = NULL;
    const char *tail = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int complete_flag = 0;
    int threadsafe_flag = 0;

    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    threadsafe_flag = sqlite3_threadsafe();

    // step 2: Configure
    builder = sqlite3_str_new(db);
    sqlite3_str_append(builder, "CREATE TABLE t(id INTEGER PRIMARY KEY, value TEXT);", 51);
    sqlite3_str_append(builder, "INSERT INTO t(value) VALUES('one'),('two'),('three');", 53);
    sql_text = sqlite3_str_finish(builder);
    complete_flag = sqlite3_complete(sql_text);
    rc_prep = sqlite3_prepare_v3(db, sql_text, -1, 0, &stmt, &tail);

    // step 3: Operate / Validate
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    tmp_buf = (char *)sqlite3_realloc(NULL, 256);
    sqlite3_result_null((sqlite3_context *)NULL);
    tmp_buf = (char *)sqlite3_realloc(tmp_buf, 0);

    // step 4: Cleanup
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)threadsafe_flag;
    (void)builder;
    (void)sql_text;
    (void)complete_flag;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_finalize;
    (void)tmp_buf;
    (void)tail;
    (void)rc_close;
    (void)stmt;
    (void)db;

    // API sequence test completed successfully
    return 66;
}