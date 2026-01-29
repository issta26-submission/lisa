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
//<ID> 311
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
    const void *sql16 = (const void *)L"SELECT 1 AS col;";
    const void *pzTail = NULL;
    const void *col_origin = NULL;
    const char *uri_param = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_reset = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int opt_used = 0;

    // step 2: Setup (open an in-memory database)
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 3: Operate and validate (check compile option, inspect URI parameter, prepare UTF-16 statement, step it, query column origin)
    opt_used = sqlite3_compileoption_used("ENABLE_FTS5");
    uri_param = sqlite3_uri_parameter((sqlite3_filename)"file:memdb?mode=memory", "mode");
    rc_prepare = sqlite3_prepare16_v2(db, sql16, -1, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    col_origin = sqlite3_column_origin_name16(stmt, 0);

    // step 4: Cleanup (reset and finalize statement, close database)
    rc_reset = sqlite3_reset(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_reset;
    (void)rc_finalize;
    (void)rc_close;
    (void)opt_used;
    (void)uri_param;
    (void)pzTail;
    (void)col_origin;
    (void)sql16;
    // API sequence test completed successfully
    return 66;
}