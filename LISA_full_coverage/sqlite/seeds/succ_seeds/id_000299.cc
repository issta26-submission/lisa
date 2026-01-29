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
//<ID> 299
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
    void *buffer = NULL;
    const char *pzTail = NULL;
    const char *stmt_sql = NULL;
    const char *libver = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_close = 0;
    int str_errcode = 0;
    sqlite3_int64 changes = 0;

    // step 2: Initialize (open DB, create string builder, append SQL, allocate buffer, query libversion)
    rc_open = sqlite3_open(":memory:", &db);
    str_builder = sqlite3_str_new(db);
    sqlite3_str_appendall(str_builder, "CREATE TABLE demo(id INTEGER);");
    buffer = sqlite3_malloc(128);
    libver = sqlite3_libversion();

    // step 3: Operate (prepare a statement, extract SQL, check string-builder error, close any blob)
    rc_prepare = sqlite3_prepare_v3(db, "CREATE TABLE demo(id INTEGER);", -1, 0, &stmt, &pzTail);
    stmt_sql = sqlite3_sql(stmt);
    str_errcode = sqlite3_str_errcode(str_builder);
    sqlite3_blob_close(blob);

    // step 4: Cleanup (reset statement, inspect changes, close DB)
    sqlite3_reset(stmt);
    changes = sqlite3_changes64(db);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_close;
    (void)stmt_sql;
    (void)str_errcode;
    (void)buffer;
    (void)pzTail;
    (void)libver;
    (void)changes;

    // API sequence test completed successfully
    return 66;
}