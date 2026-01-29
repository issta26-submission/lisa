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
//<ID> 203
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    const char *filename = ":memory:";
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, value INTEGER);";
    const char *sql_insert = "INSERT INTO t(value) VALUES(42);";
    const char *sql_select = "SELECT value FROM t;";

    int rc_open = sqlite3_open(filename, &db);
    int rc_ext_on = sqlite3_extended_result_codes(db, 1);

    char *errmsg = NULL;
    int rc_exec_create = sqlite3_exec(db, sql_create, 0, 0, &errmsg);
    int rc_exec_insert = sqlite3_exec(db, sql_insert, 0, 0, &errmsg);

    sqlite3_int64 changes_after_insert = sqlite3_changes64(db);

    sqlite3_stmt *stmt = NULL;
    const char *pzTail = NULL;
    int rc_prepare = sqlite3_prepare(db, sql_select, -1, &stmt, &pzTail);

    const char *orig_sql = sqlite3_sql(stmt);
    char *expanded_sql = sqlite3_expanded_sql(stmt);

    sqlite3_str *s = sqlite3_str_new(db);
    sqlite3_str_appendall(s, expanded_sql);
    sqlite3_str_appendchar(s, 1, '\n');
    char *str_finished = sqlite3_str_finish(s);

    sqlite3_reset(stmt);
    sqlite3_clear_bindings(stmt);

    sqlite3_mutex *m = sqlite3_mutex_alloc(0);
    int mutex_held = sqlite3_mutex_held(m);

    int val_from_api = 0;

    sqlite3_free(expanded_sql);
    sqlite3_free(str_finished);
    sqlite3_free(errmsg);

    int rc_ext_off = sqlite3_extended_result_codes(db, 0);
    int rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_ext_on;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)changes_after_insert;
    (void)rc_prepare;
    (void)orig_sql;
    (void)m;
    (void)mutex_held;
    (void)val_from_api;
    (void)rc_ext_off;
    (void)rc_close;
    (void)stmt;
    (void)filename;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;
    (void)errmsg;
    (void)str_finished;

    return 66;
}