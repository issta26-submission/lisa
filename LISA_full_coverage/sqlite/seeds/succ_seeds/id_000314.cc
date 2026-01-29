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
//<ID> 314
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
    const void *sql16 = (const void *)L"SELECT x.col FROM (SELECT 1 AS col) x;";
    const void *pzTail = NULL;
    const void *col_origin = NULL;
    const char *uri_val_mode = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int opt_used = 0;

    // step 2: Initialize & Configure
    rc_open = sqlite3_open_v2("file:testdb?mode=memory&cache=shared", &db, 0, NULL);
    opt_used = sqlite3_compileoption_used("ENABLE_FTS5");
    uri_val_mode = sqlite3_uri_parameter((sqlite3_filename)"file:testdb?mode=memory&cache=shared", "mode");

    // step 3: Operate & Validate
    rc_prepare = sqlite3_prepare16_v2(db, sql16, -1, &stmt, &pzTail);
    col_origin = sqlite3_column_origin_name16(stmt, 0);

    // step 4: Cleanup
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)opt_used;
    (void)uri_val_mode;
    (void)pzTail;
    (void)col_origin;

    // API sequence test completed successfully
    return 66;
}