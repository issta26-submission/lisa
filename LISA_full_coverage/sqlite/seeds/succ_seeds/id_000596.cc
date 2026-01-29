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
//<ID> 596
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
    sqlite3_stmt *stmt_select = NULL;
    char sqlbuf[256];
    char *create_sql = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int rc_bind = 0;
    int autocommit = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open(":memory:", &db);
    builder = sqlite3_str_new(db);
    sqlite3_str_appendf(builder, "CREATE TABLE t(id INTEGER PRIMARY KEY, value TEXT);");
    create_sql = sqlite3_str_finish(builder);
    builder = NULL;
    sqlite3_snprintf((int)sizeof(sqlbuf), sqlbuf, "INSERT INTO t(value) VALUES(?1);");

    // step 3: Operate / Validate
    rc_prep = sqlite3_prepare_v3(db, create_sql, -1, 0, &stmt_create, NULL);
    rc_step = sqlite3_step(stmt_create);
    rc_finalize = sqlite3_finalize(stmt_create);
    stmt_create = NULL;

    rc_prep = sqlite3_prepare_v3(db, sqlbuf, -1, 0, &stmt_insert, NULL);
    rc_bind = sqlite3_bind_text(stmt_insert, 1, "bound_value", -1, (void(*)(void *))0);
    rc_step = sqlite3_step(stmt_insert);
    rc_finalize = sqlite3_finalize(stmt_insert);
    stmt_insert = NULL;

    rc_prep = sqlite3_prepare_v3(db, "SELECT id, value FROM t;", -1, 0, &stmt_select, NULL);
    rc_step = sqlite3_step(stmt_select);
    autocommit = sqlite3_get_autocommit(db);

    // step 4: Cleanup
    rc_finalize = sqlite3_finalize(stmt_select);
    stmt_select = NULL;
    sqlite3_realloc(create_sql, 0);
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)rc_bind;
    (void)autocommit;
    (void)sqlbuf;
    (void)builder;
    (void)stmt_create;
    (void)stmt_insert;
    (void)stmt_select;

    // API sequence test completed successfully
    return 66;
}