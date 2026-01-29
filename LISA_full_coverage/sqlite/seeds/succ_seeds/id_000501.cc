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
//<ID> 501
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations (Initialize)
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_context *ctx = NULL;
    const char *sql = "SELECT 1 AS one";
    const char *decl = NULL;
    const char *errstr = NULL;
    void *ptr = NULL;
    sqlite3_int64 rowid = (sqlite3_int64)1;
    int rc_open = 0;
    int rc_ext = 0;
    int rc_prep = 0;
    int rc_blob = 0;

    // step 2: Setup (Open database and configure)
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_ext = sqlite3_extended_result_codes(db, 1);

    // step 3: Operate (Prepare a statement, query metadata, attempt blob open, attach result pointer)
    rc_prep = sqlite3_prepare(db, sql, -1, &stmt, NULL);
    decl = sqlite3_column_decltype(stmt, 0);
    rc_blob = sqlite3_blob_open(db, "main", "t1", "data", rowid, 0, &blob);
    errstr = sqlite3_errstr(rc_blob);
    ptr = (void *)db;
    sqlite3_result_pointer(ctx, ptr, "sqlite3-db", NULL);

    // step 4: Cleanup (Finalize and close; note: blob handle not allocated on success path here)
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    (void)rc_open;
    (void)rc_ext;
    (void)rc_prep;
    (void)rc_blob;
    (void)decl;
    (void)errstr;
    (void)blob;
    (void)ptr;
    (void)ctx;
    (void)sql;
    (void)rowid;

    // API sequence test completed successfully
    return 66;
}