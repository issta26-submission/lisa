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
//<ID> 317
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
    const void *pzTail = NULL;
    const void *sql16 = (const void *)L"SELECT id AS oid, name FROM test;";
    const void *origin_name16 = NULL;
    const char *filename = "file:memdb1?mode=memory&cache=shared";
    const char *uri_cache_val = NULL;
    int rc_open = 0;
    int rc_exec1 = 0;
    int rc_exec2 = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_reset = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int opt_enabled = 0;

    // step 2: Setup - open database and create a table + insert a row
    rc_open = sqlite3_open_v2(filename, &db, 0, NULL);
    rc_exec1 = sqlite3_exec(db, "CREATE TABLE test(id INTEGER PRIMARY KEY, name TEXT);", NULL, NULL, NULL);
    rc_exec2 = sqlite3_exec(db, "INSERT INTO test(name) VALUES('Alice');", NULL, NULL, NULL);

    // step 3: Operate & Validate - prepare a UTF-16 statement, step it, inspect column origin and URI/compile options
    rc_prepare = sqlite3_prepare16_v2(db, sql16, -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    origin_name16 = sqlite3_column_origin_name16(stmt, 1);
    uri_cache_val = sqlite3_uri_parameter((sqlite3_filename)filename, "cache");
    opt_enabled = sqlite3_compileoption_used("ENABLE_COLUMN_METADATA");

    // step 4: Cleanup - reset/finalize statement and close database
    rc_reset = sqlite3_reset(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_exec1;
    (void)rc_exec2;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_reset;
    (void)rc_finalize;
    (void)rc_close;
    (void)origin_name16;
    (void)uri_cache_val;
    (void)opt_enabled;
    (void)pzTail;
    (void)sql16;
    // API sequence test completed successfully
    return 66;
}