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
//<ID> 590
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_str *builder = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    char sqlbuf[256];
    char *sql_finished = NULL;
    const char *tail = NULL;
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_step_create = 0;
    int rc_finalize_create = 0;
    int rc_prep_insert = 0;
    int rc_bind = 0;
    int rc_step_insert = 0;
    int rc_finalize_insert = 0;
    int rc_close = 0;
    int autocommit = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep_create = sqlite3_prepare_v3(db, "CREATE TABLE t(id INTEGER PRIMARY KEY, value TEXT);", -1, 0, &stmt_create, &tail);
    rc_step_create = sqlite3_step(stmt_create);
    rc_finalize_create = sqlite3_finalize(stmt_create);
    builder = sqlite3_str_new(db);
    sqlite3_snprintf((int)sizeof(sqlbuf), sqlbuf, "INSERT INTO t(value) VALUES('%s');", "hello");
    sqlite3_str_appendf(builder, " -- appended literal");
    sql_finished = sqlite3_str_finish(builder);
    builder = NULL;

    // step 3: Operate / Validate
    rc_prep_insert = sqlite3_prepare_v3(db, "INSERT INTO t(value) VALUES(?);", -1, 0, &stmt_insert, NULL);
    rc_bind = sqlite3_bind_text(stmt_insert, 1, "world", -1, (void(*)(void *))NULL);
    rc_step_insert = sqlite3_step(stmt_insert);
    rc_finalize_insert = sqlite3_finalize(stmt_insert);
    autocommit = sqlite3_get_autocommit(db);

    // step 4: Cleanup
    sqlite3_realloc(sql_finished, 0);
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_finalize_create;
    (void)rc_prep_insert;
    (void)rc_bind;
    (void)rc_step_insert;
    (void)rc_finalize_insert;
    (void)rc_close;
    (void)autocommit;
    (void)tail;
    (void)stmt_create;
    (void)stmt_insert;
    (void)builder;
    (void)sqlbuf;

    // API sequence test completed successfully
    return 66;
}