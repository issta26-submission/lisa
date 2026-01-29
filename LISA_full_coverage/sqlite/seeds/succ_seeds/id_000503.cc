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
//<ID> 503
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
    const char *errstr = NULL;
    const char *decltype_str = NULL;
    char *exec_errmsg = NULL;
    int rc_open = 0;
    int rc_exec = 0;
    int rc_prep = 0;
    int rc_extended = 0;
    int rc_blob = 0;
    int rc_finalize = 0;
    int rc_close = 0;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_extended = sqlite3_extended_result_codes(db, 1);
    rc_exec = sqlite3_exec(db, "CREATE TABLE t(a BLOB); INSERT INTO t(a) VALUES(x'00');", NULL, NULL, &exec_errmsg);

    // step 3: Operate / Validate
    rc_prep = sqlite3_prepare(db, "SELECT a FROM t WHERE rowid=1;", -1, &stmt, NULL);
    decltype_str = sqlite3_column_decltype(stmt, 0);
    rc_blob = sqlite3_blob_open(db, "main", "t", "a", (sqlite3_int64)1, 0, &blob);
    errstr = sqlite3_errstr(rc_blob);
    sqlite3_result_pointer((sqlite3_context *)NULL, (void *)db, "sqlite3_db", (void (*)(void *))NULL);

    // step 4: Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)exec_errmsg;
    (void)rc_open;
    (void)rc_exec;
    (void)rc_prep;
    (void)rc_extended;
    (void)rc_blob;
    (void)rc_finalize;
    (void)rc_close;
    (void)errstr;
    (void)decltype_str;
    (void)blob;

    // API sequence test completed successfully
    return 66;
}