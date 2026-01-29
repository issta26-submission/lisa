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
//<ID> 290
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
    sqlite3_str *str_builder = NULL;
    sqlite3_blob *blob = NULL;
    void *realloc_buf = NULL;
    const char *stmt_sql = NULL;
    char *str_value = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int rc_blob_close = 0;
    int compile_opt_used = 0;
    int str_errcode = 0;

    // step 2: Setup - open an in-memory database and create a string-builder
    rc_open = sqlite3_open(":memory:", &db);
    str_builder = sqlite3_str_new(db);

    // step 3: Core operations - prepare a statement, query its SQL, test compile option,
    // allocate memory, and query string-builder error code and close a blob handle
    rc_prepare = sqlite3_prepare16(db, (const void *)L"SELECT 1", (int)(sizeof(wchar_t) * 9), &stmt, NULL);
    stmt_sql = sqlite3_sql(stmt);
    compile_opt_used = sqlite3_compileoption_used(stmt_sql ? stmt_sql : "UNKNOWN_OPTION");
    realloc_buf = sqlite3_realloc(NULL, 64);
    str_errcode = sqlite3_str_errcode(str_builder);
    rc_blob_close = sqlite3_blob_close(blob);

    // step 4: Cleanup - finalize statement, free allocated memory and string-builder, close DB
    rc_finalize = sqlite3_finalize(stmt);
    str_value = sqlite3_str_value(str_builder);
    sqlite3_free(str_value);
    sqlite3_free(realloc_buf);
    sqlite3_free(str_builder);
    rc_close = sqlite3_close_v2(db);
    db = NULL;
    stmt = NULL;
    str_builder = NULL;
    blob = NULL;
    realloc_buf = NULL;
    stmt_sql = NULL;
    str_value = NULL;

    (void)rc_open;
    (void)rc_prepare;
    (void)stmt_sql;
    (void)compile_opt_used;
    (void)str_errcode;
    (void)rc_blob_close;
    (void)rc_finalize;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}