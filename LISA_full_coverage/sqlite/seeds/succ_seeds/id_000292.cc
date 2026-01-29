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
//<ID> 292
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_str *str_builder = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    void *mem1 = NULL;
    void *mem2 = NULL;
    const char *stmt_sql = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_finalize = 0;
    int rc_blob_close = 0;
    int rc_close = 0;
    int str_err = 0;
    int compileopt_used = 0;

    // step 2: Setup - open an in-memory database
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Operate and validate - query compile option, work with realloc, prepare statement, inspect SQL, finalize, close blob
    compileopt_used = sqlite3_compileoption_used("ENABLE_FTS3");
    str_err = sqlite3_str_errcode(str_builder);
    mem1 = sqlite3_realloc(NULL, 64);
    mem2 = sqlite3_realloc(mem1, 128);
    rc_prepare = sqlite3_prepare16(db, (const void *)L"SELECT 1", -1, &stmt, NULL);
    stmt_sql = sqlite3_sql(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    rc_blob_close = sqlite3_blob_close(blob);

    // step 4: Cleanup - free allocated memory and close database
    sqlite3_free(mem2);
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)compileopt_used;
    (void)str_err;
    (void)mem1;
    (void)mem2;
    (void)rc_prepare;
    (void)stmt_sql;
    (void)rc_finalize;
    (void)rc_blob_close;
    (void)rc_close;
    (void)str_builder;
    (void)stmt;
    (void)blob;

    return 66;
    // API sequence test completed successfully
}