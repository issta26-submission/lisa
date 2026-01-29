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
//<ID> 502
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    char *errmsg = NULL;
    const char *sql_create = "CREATE TABLE t1(id INTEGER PRIMARY KEY, data BLOB, info TEXT); INSERT INTO t1(data, info) VALUES(x'010203', 'sample');";
    const char *sql_select = "SELECT data, info FROM t1;";
    const char *col_decltype = NULL;
    const char *errstr_exec = NULL;
    const char *errstr_blob = NULL;
    int rc_open = 0;
    int rc_exec = 0;
    int rc_prep = 0;
    int rc_ext = 0;
    int rc_blob = 0;
    int rc_finalize = 0;
    int rc_close = 0;

    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_exec = sqlite3_exec(db, sql_create, NULL, NULL, &errmsg);
    rc_prep = sqlite3_prepare(db, sql_select, -1, &stmt, NULL);
    col_decltype = sqlite3_column_decltype(stmt, 1);
    rc_ext = sqlite3_extended_result_codes(db, 1);
    rc_blob = sqlite3_blob_open(db, "main", "t1", "data", 1, 0, &blob);
    sqlite3_result_pointer(NULL, (void *)col_decltype, "decltype_ptr", NULL);
    errstr_exec = sqlite3_errstr(rc_exec);
    errstr_blob = sqlite3_errstr(rc_blob);
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)errmsg;
    (void)col_decltype;
    (void)errstr_exec;
    (void)errstr_blob;
    (void)blob;
    (void)rc_open;
    (void)rc_exec;
    (void)rc_prep;
    (void)rc_ext;
    (void)rc_blob;
    (void)rc_finalize;
    (void)rc_close;
    return 66;
}