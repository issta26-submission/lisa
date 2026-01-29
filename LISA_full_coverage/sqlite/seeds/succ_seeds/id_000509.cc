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
//<ID> 509
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_select = NULL;
    sqlite3_blob *blob = NULL;
    const char *sql_create = "CREATE TABLE t1(id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_select = "SELECT data FROM t1";
    const char *col_decltype = NULL;
    const char *errstr_open = NULL;
    const char *errstr_exec = NULL;
    const char *errstr_prep = NULL;
    const char *errstr_blob = NULL;
    int rc_open = 0;
    int rc_ext = 0;
    int rc_exec = 0;
    int rc_prep = 0;
    int rc_blob = 0;
    sqlite3_int64 rowid = 1;

    // step 2: Setup / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_ext = sqlite3_extended_result_codes(db, 1);

    // step 3: Operate / Validate
    rc_exec = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    rc_prep = sqlite3_prepare(db, sql_select, -1, &stmt_select, NULL);
    col_decltype = sqlite3_column_decltype(stmt_select, 0);
    rc_blob = sqlite3_blob_open(db, "main", "t1", "data", rowid, 0, &blob);
    sqlite3_result_pointer((sqlite3_context *)NULL, (void *)blob, "sqlite3_blob", (void(*)(void *))NULL);
    errstr_open = sqlite3_errstr(rc_open);
    errstr_exec = sqlite3_errstr(rc_exec);
    errstr_prep = sqlite3_errstr(rc_prep);
    errstr_blob = sqlite3_errstr(rc_blob);

    // step 4: Cleanup
    sqlite3_finalize(stmt_select);
    (void)sqlite3_close_v2(db);

    (void)col_decltype;
    (void)errstr_open;
    (void)errstr_exec;
    (void)errstr_prep;
    (void)errstr_blob;
    (void)rc_ext;
    (void)rc_open;
    (void)rc_exec;
    (void)rc_prep;
    (void)rc_blob;
    (void)blob;
    (void)rowid;

    // API sequence test completed successfully
    return 66;
}