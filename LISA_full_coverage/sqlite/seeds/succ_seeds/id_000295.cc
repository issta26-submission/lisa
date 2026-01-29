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
//<ID> 295
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
    void *realloc_ptr = NULL;
    const char *prepared_sql = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_blob_close = 0;
    int compileopt_used = 0;
    int str_err = 0;
    int rc_close = 0;

    // step 2: Setup - open an in-memory database
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Core operations - prepare a statement, inspect SQL, build a string, query compile option, allocate memory, close blob
    str_builder = sqlite3_str_new(db);
    rc_prep = sqlite3_prepare16(db, (const void *)"CREATE TABLE IF NOT EXISTS t(x);", -1, &stmt, NULL);
    prepared_sql = sqlite3_sql(stmt);
    sqlite3_str_appendall(str_builder, prepared_sql ? prepared_sql : "");
    compileopt_used = sqlite3_compileoption_used("THREADSAFE");
    str_err = sqlite3_str_errcode(str_builder);
    realloc_ptr = sqlite3_realloc(NULL, 16 + (str_err & 0xff));
    rc_blob_close = sqlite3_blob_close(blob);

    // step 4: Cleanup - finalize statement, free builder and buffer, close database
    sqlite3_finalize(stmt);
    sqlite3_free(realloc_ptr);
    sqlite3_free(str_builder);
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prep;
    (void)prepared_sql;
    (void)compileopt_used;
    (void)str_err;
    (void)rc_blob_close;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}