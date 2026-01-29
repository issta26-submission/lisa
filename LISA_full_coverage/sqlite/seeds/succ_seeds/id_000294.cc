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
//<ID> 294
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
    sqlite3_str *builder = NULL;
    sqlite3_blob *blob = NULL;
    void *temp_mem = NULL;
    const char *stmt_sql = NULL;
    char *built_value = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int rc_blob_close = 0;
    int compile_opt_used = 0;
    int str_errcode = 0;

    // step 2: Setup - open in-memory DB, create a string builder, and prepare a statement (UTF-16)
    rc_open = sqlite3_open(":memory:", &db);
    builder = sqlite3_str_new(db);
    rc_prepare = sqlite3_prepare16(db, (const void *)L"CREATE TABLE t1(a);", -1, &stmt, NULL);

    // step 3: Operate and Validate - inspect prepared SQL, operate on the string builder, use realloc and compile-option query
    stmt_sql = sqlite3_sql(stmt);
    sqlite3_str_append(builder, " -- appended comment", 20);
    str_errcode = sqlite3_str_errcode(builder);
    compile_opt_used = sqlite3_compileoption_used("ENABLE_FTS3");
    temp_mem = sqlite3_realloc(NULL, 64);
    if (temp_mem) memset(temp_mem, 0, 64);
    sqlite3_free(temp_mem);
    rc_blob_close = sqlite3_blob_close(blob);
    built_value = sqlite3_str_value(builder);
    sqlite3_free(built_value);

    // step 4: Cleanup - finalize statement and close DB, coerce unused values
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close_v2(db);
    db = NULL;
    builder = NULL;
    blob = NULL;

    (void)rc_open;
    (void)rc_prepare;
    (void)stmt_sql;
    (void)str_errcode;
    (void)compile_opt_used;
    (void)rc_blob_close;
    (void)rc_finalize;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}