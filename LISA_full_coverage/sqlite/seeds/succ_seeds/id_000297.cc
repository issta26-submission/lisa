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
//<ID> 297
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
    void *mem = NULL;
    const char *stmt_sql = NULL;
    char *str_value = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int rc_blob_close = 0;
    int compile_opt_used = 0;
    int str_errcode = 0;

    // step 2: Setup - open in-memory database and allocate temporary memory
    rc_open = sqlite3_open(":memory:", &db);
    mem = sqlite3_realloc(NULL, 128);

    // step 3: Configure and Operate - build a string, check compile option, prepare a statement, inspect SQL
    compile_opt_used = sqlite3_compileoption_used("ENABLE_FTS5");
    str_builder = sqlite3_str_new(db);
    sqlite3_str_append(str_builder, "prefix:", 7);
    str_errcode = sqlite3_str_errcode(str_builder);
    str_value = sqlite3_str_value(str_builder);
    rc_prepare = sqlite3_prepare16(db, (const void *)u"CREATE TABLE t(x);", -1, &stmt, NULL);
    stmt_sql = sqlite3_sql(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_blob_close = sqlite3_blob_close(blob);

    // step 4: Cleanup and finalize
    sqlite3_free(mem);
    sqlite3_free(str_value);
    rc_close = sqlite3_close(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_finalize;
    (void)rc_close;
    (void)rc_blob_close;
    (void)compile_opt_used;
    (void)str_errcode;
    (void)stmt_sql;
    (void)str_builder;
    (void)blob;

    // API sequence test completed successfully
    return 66;
}