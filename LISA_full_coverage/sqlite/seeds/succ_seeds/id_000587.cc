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
//<ID> 587
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
    char *built_sql = NULL;
    void *mem_block = NULL;
    const char *tail = NULL;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT);";
    const char *sql_insert = "INSERT INTO t(val) VALUES('x');";
    const char *sql_select = "SELECT id, val FROM t;";
    sqlite3_context *ctx = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int complete_create = 0;
    int complete_insert = 0;
    int complete_select = 0;
    int threadsafe = 0;
    sqlite3_uint64 mem_size = 0;
    int col_count = 0;

    // call thread-safety query early
    threadsafe = sqlite3_threadsafe();

    // quick completeness checks before preparing
    complete_create = sqlite3_complete(sql_create);
    complete_insert = sqlite3_complete(sql_insert);
    complete_select = sqlite3_complete(sql_select);

    // step 2: Setup / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    builder = sqlite3_str_new(db);
    sqlite3_str_append(builder, "prefix:", 7);
    sqlite3_str_append(builder, "table", 5);
    built_sql = sqlite3_str_finish(builder);
    builder = NULL;

    // step 3: Operate / Validate
    rc_prep = sqlite3_prepare_v3(db, sql_create, -1, 0, &stmt, &tail);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    rc_prep = sqlite3_prepare_v3(db, sql_insert, -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    rc_prep = sqlite3_prepare_v3(db, sql_select, -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    col_count = sqlite3_column_count(stmt);
    sqlite3_result_null(ctx);

    mem_block = sqlite3_realloc(NULL, 128);
    mem_size = sqlite3_msize(mem_block);
    mem_block = sqlite3_realloc(mem_block, 0);

    rc_finalize = sqlite3_finalize(stmt);

    // step 4: Cleanup
    rc_close = sqlite3_close_v2(db);
    built_sql = (char*)sqlite3_realloc(built_sql, 0);

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)complete_create;
    (void)complete_insert;
    (void)complete_select;
    (void)threadsafe;
    (void)mem_size;
    (void)col_count;
    (void)tail;
    (void)ctx;
    (void)stmt;
    (void)builder;
    (void)db;
    (void)mem_block;

    // API sequence test completed successfully
    return 66;
}