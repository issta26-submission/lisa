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
//<ID> 500
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    const char *sql_setup = "CREATE TABLE t(x BLOB); INSERT INTO t(rowid, x) VALUES(1, zeroblob(16));";
    const char *sql_select = "SELECT x FROM t";
    const char *decl = NULL;
    const char *err_string = NULL;
    char *errmsg = NULL;
    int rc_open = 0;
    int rc_exec = 0;
    int rc_prep = 0;
    int rc_ext = 0;
    int rc_blob = 0;
    int blob_size = 0;

    // step 2: Initialize / Prepare
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_exec = sqlite3_exec(db, sql_setup, NULL, NULL, &errmsg);
    rc_prep = sqlite3_prepare(db, sql_select, -1, &stmt, NULL);

    // step 3: Configure / Operate / Validate
    decl = sqlite3_column_decltype(stmt, 0);
    rc_ext = sqlite3_extended_result_codes(db, 1);
    rc_blob = sqlite3_blob_open(db, "main", "t", "x", (sqlite3_int64)1, 0, &blob);
    blob_size = sqlite3_blob_bytes(blob);
    sqlite3_result_pointer((sqlite3_context *)NULL, (void *)blob, "sqlite3_blob", (void (*)(void *))NULL);
    err_string = sqlite3_errstr(rc_blob);

    // step 4: Cleanup
    sqlite3_finalize(stmt);
    sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_exec;
    (void)rc_prep;
    (void)rc_ext;
    (void)rc_blob;
    (void)blob_size;
    (void)decl;
    (void)err_string;
    (void)errmsg;
    (void)blob;

    // API sequence test completed successfully
    return 66;
}