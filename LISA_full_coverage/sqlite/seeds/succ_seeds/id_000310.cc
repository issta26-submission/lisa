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
//<ID> 310
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = (sqlite3 *)0;
    sqlite3_stmt *stmt = (sqlite3_stmt *)0;
    const void *pzTail = (const void *)0;
    const void *sql16 = (const void *)L"CREATE TABLE demo(id INTEGER);";
    sqlite3_filename filename = ":memory:";
    const char *uri_mode = (const char *)0;
    const void *col_origin = (const void *)0;
    int rc_open = 0;
    int rc_prepare = 0;
    int opt_used = 0;
    int rc_reset = 0;
    int rc_finalize = 0;
    int rc_close = 0;

    // step 2: Setup (open database, query compile option availability)
    rc_open = sqlite3_open_v2(":memory:", &db, 0, (const char *)0);
    opt_used = sqlite3_compileoption_used("ENABLE_FTS5");

    // step 3: Operate (inspect URI parameter, prepare UTF-16 statement, inspect column origin)
    uri_mode = sqlite3_uri_parameter(filename, "mode");
    rc_prepare = sqlite3_prepare16_v2(db, sql16, -1, &stmt, &pzTail);
    col_origin = sqlite3_column_origin_name16(stmt, 0);

    // step 4: Validate & Cleanup (reset, finalize and close)
    rc_reset = sqlite3_reset(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)opt_used;
    (void)uri_mode;
    (void)col_origin;
    (void)rc_reset;
    (void)rc_finalize;
    (void)rc_close;
    (void)pzTail;
    (void)sql16;
    (void)filename;

    // API sequence test completed successfully
    return 66;
}