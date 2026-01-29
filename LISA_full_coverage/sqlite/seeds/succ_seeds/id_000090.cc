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
//<ID> 90
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    char *errmsg = NULL;
    int rc_open = sqlite3_open(":memory:", &db);
    const char *open_errstr = sqlite3_errstr(rc_open);

    sqlite3_progress_handler(db, 1000, NULL, NULL);

    int rc_create = sqlite3_exec(db, "CREATE TABLE t(x INTEGER);", NULL, NULL, &errmsg);
    const char *create_err = sqlite3_errstr(rc_create);
    int rc_insert = sqlite3_exec(db, "INSERT INTO t VALUES(1234567890123);", NULL, NULL, &errmsg);
    const char *insert_err = sqlite3_errstr(rc_insert);

    sqlite3_stmt *stmt = NULL;
    int rc_prepare = sqlite3_prepare_v2(db, "SELECT x FROM t LIMIT 1;", -1, &stmt, NULL);
    const char *prepare_err = sqlite3_errstr(rc_prepare);
    int rc_step = sqlite3_step(stmt);

    const unsigned char *col_text = sqlite3_column_text(stmt, 0);
    double col_double = sqlite3_column_double(stmt, 0);
    sqlite3_value *val = sqlite3_column_value(stmt, 0);
    sqlite3_int64 v64 = (sqlite3_int64)col_double;
    int data_count = sqlite3_data_count(stmt);
    char *expanded_sql = sqlite3_expanded_sql(stmt);

    sqlite3_finalize(stmt);

    int rc_drop = sqlite3_exec(db, "DROP TABLE IF EXISTS t;", NULL, NULL, &errmsg);
    int rc_close = sqlite3_close_v2(db);

    (void)open_errstr;
    (void)create_err;
    (void)insert_err;
    (void)prepare_err;
    (void)col_text;
    (void)v64;
    (void)val;
    (void)data_count;
    (void)expanded_sql;
    (void)errmsg;
    (void)rc_open;
    (void)rc_create;
    (void)rc_insert;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_drop;
    (void)rc_close;
    (void)db;
    (void)stmt;

    return 66;
}