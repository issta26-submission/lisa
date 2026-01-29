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
//<ID> 508
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
    sqlite3_blob *blob = NULL;
    sqlite3_context *ctx = NULL;
    const char *sql_create = "CREATE TABLE t(a BLOB);";
    const char *sql_insert = "INSERT INTO t(a) VALUES(x'010203');";
    const char *sql_select = "SELECT a FROM t;";
    const char *col_decltype = NULL;
    const char *err_text = NULL;
    int rc_open = 0;
    int rc_ext = 0;
    int rc_exec1 = 0;
    int rc_exec2 = 0;
    int rc_prep = 0;
    int rc_blob = 0;
    sqlite3_int64 rowid = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_ext = sqlite3_extended_result_codes(db, 1);
    rc_exec1 = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    rc_exec2 = sqlite3_exec(db, sql_insert, NULL, NULL, NULL);
    rowid = sqlite3_last_insert_rowid(db);

    // step 3: Operate / Validate
    rc_prep = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    col_decltype = sqlite3_column_decltype(stmt, 0);
    rc_blob = sqlite3_blob_open(db, "main", "t", "a", rowid, 0, &blob);
    err_text = sqlite3_errstr(rc_blob);
    sqlite3_result_pointer(ctx, (void *)col_decltype, "decltype", NULL);

    // step 4: Cleanup
    sqlite3_finalize(stmt);
    (void)sqlite3_blob_bytes(blob);
    sqlite3_close(db);

    (void)rc_open;
    (void)rc_ext;
    (void)rc_exec1;
    (void)rc_exec2;
    (void)rc_prep;
    (void)rc_blob;
    (void)col_decltype;
    (void)err_text;
    (void)rowid;
    (void)stmt;
    (void)blob;
    (void)ctx;

    // API sequence test completed successfully
    return 66;
}