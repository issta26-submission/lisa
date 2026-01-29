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
//<ID> 318
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
    const void *sql16 = (const void *)L"CREATE TABLE demo(id INTEGER);";
    const void *pzTail = NULL;
    const void *origin_name = NULL;
    const char *uri_val = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_compileopt = 0;
    int rc_reset = 0;
    int rc_close = 0;
    sqlite3_int64 changes = 0;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_compileopt = sqlite3_compileoption_used("SQLITE_ENABLE_FTS5");
    uri_val = sqlite3_uri_parameter((sqlite3_filename)":memory:", "mode");

    // step 3: Operate
    rc_prepare = sqlite3_prepare16_v2(db, sql16, -1, &stmt, &pzTail);
    origin_name = sqlite3_column_origin_name16(stmt, 0);
    rc_reset = sqlite3_reset(stmt);

    // step 4: Validate / Cleanup
    changes = sqlite3_changes64(db);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_compileopt;
    (void)uri_val;
    (void)pzTail;
    (void)origin_name;
    (void)rc_reset;
    (void)changes;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}