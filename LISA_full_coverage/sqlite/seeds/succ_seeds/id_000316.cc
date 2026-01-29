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
//<ID> 316
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations and Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const void *pzTail = NULL;
    const void *sql16 = (const void *)L"SELECT id, val FROM t1;";
    const void *col_origin = NULL;
    const char *uri_param = NULL;
    int rc_open = 0;
    int rc_exec = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_reset = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int opt_used = 0;

    // step 2: Setup / Configure - open an in-memory database and create a table with one row
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_exec = sqlite3_exec(db, "CREATE TABLE t1(id INTEGER PRIMARY KEY, val TEXT); INSERT INTO t1(val) VALUES('alpha');", 0, 0, 0);
    opt_used = sqlite3_compileoption_used("ENABLE_COLUMN_METADATA");
    uri_param = sqlite3_uri_parameter((sqlite3_filename)":memory:", "mode");

    // step 3: Operate and Validate - prepare a UTF-16 SELECT, step once, inspect column origin
    rc_prepare = sqlite3_prepare16_v2(db, sql16, -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    col_origin = sqlite3_column_origin_name16(stmt, 1);

    // step 4: Cleanup - reset and finalize statement, close database
    rc_reset = sqlite3_reset(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_exec;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_reset;
    (void)rc_finalize;
    (void)rc_close;
    (void)opt_used;
    (void)uri_param;
    (void)col_origin;
    (void)pzTail;
    (void)sql16;

    // API sequence test completed successfully
    return 66;
}