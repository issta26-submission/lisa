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
//<ID> 472
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    char *errmsg = NULL;
    const char *sql_create = "CREATE TABLE t(a INT, b DOUBLE);";
    const char *sql_insert_literal = "INSERT INTO t(a, b) VALUES(1, 2.5);";
    const char *sql_prepare_insert = "INSERT INTO t(a, b) VALUES(?, ?);";
    const char *pzTail = NULL;

    int rc_open = sqlite3_open(":memory:", &db);
    int rc_exec_create = sqlite3_exec(db, sql_create, 0, 0, &errmsg);
    int rc_prepare = sqlite3_prepare_v3(db, sql_prepare_insert, -1, 0, &stmt, &pzTail);
    int rc_bind1 = sqlite3_bind_double(stmt, 1, 3.14);
    int rc_bind2 = sqlite3_bind_double(stmt, 2, 6.28);
    int rc_reset = sqlite3_reset(stmt);
    int rc_finalize = sqlite3_finalize(stmt);
    int rc_exec_insert = sqlite3_exec(db, sql_insert_literal, 0, 0, &errmsg);
    int changes = sqlite3_changes(db);
    int errcode = sqlite3_errcode(db);
    const char *errstr = sqlite3_errstr(errcode);
    int rc_close = sqlite3_close_v2(db);

    sqlite3_free(errmsg);

    (void)rc_open;
    (void)rc_exec_create;
    (void)rc_prepare;
    (void)rc_bind1;
    (void)rc_bind2;
    (void)rc_reset;
    (void)rc_finalize;
    (void)rc_exec_insert;
    (void)changes;
    (void)errcode;
    (void)errstr;
    (void)rc_close;
    (void)db;
    (void)stmt;
    (void)errmsg;
    (void)sql_create;
    (void)sql_insert_literal;
    (void)sql_prepare_insert;
    (void)pzTail;

    return 66;
}