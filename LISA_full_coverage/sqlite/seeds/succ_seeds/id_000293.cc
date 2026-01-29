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
//<ID> 293
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
    void *realloced = NULL;
    const char *compiled_sql = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_finalize = 0;
    int rc_blob_close = 0;
    int rc_close = 0;
    int opt_used = 0;
    int str_err = 0;

    // step 2: Setup - open an in-memory database and prepare a statement
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep = sqlite3_prepare16(db, (const void *)u"CREATE TABLE t(x);", -1, &stmt, NULL);

    // step 3: Configure and Operate - inspect prepared SQL, string-builder, compile options, and memory ops
    compiled_sql = sqlite3_sql(stmt);
    opt_used = sqlite3_compileoption_used("THREADSAFE");
    str_builder = sqlite3_str_new(db);
    sqlite3_str_appendall(str_builder, compiled_sql);
    str_err = sqlite3_str_errcode(str_builder);
    mem = sqlite3_realloc(NULL, 64);
    realloced = sqlite3_realloc(mem, 128);
    rc_blob_close = sqlite3_blob_close(blob);

    // step 4: Cleanup and finalize
    rc_finalize = sqlite3_finalize(stmt);
    sqlite3_free(realloced);
    sqlite3_free(str_builder);
    rc_close = sqlite3_close_v2(db);
    db = NULL;
    stmt = NULL;
    str_builder = NULL;
    mem = NULL;
    realloced = NULL;
    blob = NULL;

    (void)rc_open;
    (void)rc_prep;
    (void)compiled_sql;
    (void)opt_used;
    (void)str_err;
    (void)rc_blob_close;
    (void)rc_finalize;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}