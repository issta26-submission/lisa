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
//<ID> 585
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_str *builder = NULL;
    char *built_sql = NULL;
    void *mem = NULL;
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_step_create = 0;
    int rc_finalize_create = 0;
    int rc_prep_insert = 0;
    int rc_step_insert = 0;
    int rc_finalize_insert = 0;
    int rc_close = 0;
    int complete_res_literal = 0;
    int complete_res_built = 0;
    int threadsafe = 0;
    const char *create_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, value TEXT);";
    const char *insert_fragment = "INSERT INTO t(value) VALUES('built');";

    // step 2: Configure - open DB and create table
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep_create = sqlite3_prepare_v3(db, create_sql, -1, 0, &stmt_create, NULL);
    rc_step_create = sqlite3_step(stmt_create);
    rc_finalize_create = sqlite3_finalize(stmt_create);

    // step 3: Operate / Validate - build SQL with sqlite3_str_append, test completeness, execute insert
    builder = sqlite3_str_new(db);
    sqlite3_str_append(builder, insert_fragment, (int)(sizeof("INSERT INTO t(value) VALUES('built');") - 1));
    built_sql = sqlite3_str_finish(builder);
    complete_res_literal = sqlite3_complete("SELECT 1;");
    complete_res_built = sqlite3_complete(built_sql);
    rc_prep_insert = sqlite3_prepare_v3(db, built_sql, -1, 0, &stmt_insert, NULL);
    rc_step_insert = sqlite3_step(stmt_insert);
    rc_finalize_insert = sqlite3_finalize(stmt_insert);

    // step 4: Additional API usage and cleanup
    mem = sqlite3_malloc64(16);
    mem = sqlite3_realloc(mem, 32);
    mem = sqlite3_realloc(mem, 0); /* attempt to free/release via realloc to size 0 */
    threadsafe = sqlite3_threadsafe();
    sqlite3_result_null((sqlite3_context *)NULL);
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_finalize_create;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)rc_finalize_insert;
    (void)rc_close;
    (void)complete_res_literal;
    (void)complete_res_built;
    (void)threadsafe;
    (void)built_sql;
    (void)mem;
    (void)stmt_create;
    (void)stmt_insert;
    (void)builder;
    (void)create_sql;
    (void)insert_fragment;

    // API sequence test completed successfully
    return 66;
}