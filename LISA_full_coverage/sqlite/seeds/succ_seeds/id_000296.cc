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
//<ID> 296
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
    sqlite3_str *str = NULL;
    sqlite3_blob *blob = NULL;
    void *buffer = NULL;
    const char *stmt_sql = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int compile_opt_used = 0;
    int str_err = 0;
    int rc_blob_close = 0;

    // step 2: Setup / Configure - open in-memory DB, create string builder, allocate buffer
    rc_open = sqlite3_open(":memory:", &db);
    str = sqlite3_str_new(db);
    buffer = sqlite3_realloc(NULL, 128);

    // step 3: Operate / Validate - prepare a simple statement, inspect SQL and compile-option and str error
    rc_prep = sqlite3_prepare16(db, (const void *)L"SELECT 1", -1, &stmt, (const void **)NULL);
    stmt_sql = sqlite3_sql(stmt);
    compile_opt_used = sqlite3_compileoption_used("ENABLE_FTS3");
    sqlite3_str_append(str, "built:", 6);
    sqlite3_str_appendf(str, "%s", "ok");
    str_err = sqlite3_str_errcode(str);

    // step 4: Cleanup - finalize statement, close any blob, free resources and close DB
    rc_finalize = sqlite3_finalize(stmt);
    rc_blob_close = sqlite3_blob_close(blob);
    sqlite3_free(str);
    sqlite3_free(buffer);
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prep;
    (void)stmt_sql;
    (void)compile_opt_used;
    (void)str_err;
    (void)rc_finalize;
    (void)rc_blob_close;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}