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
//<ID> 312
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
    const void *sql16 = (const void *)L"SELECT 1 AS col;";
    const char *filename = ":memory:";
    const char *uri_param = NULL;
    const void *origin_name = NULL;
    int rc_open = 0;
    int rc_prepare16 = 0;
    int rc_reset = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int opt_used = 0;

    // step 2: Initialize (open database, check compile option and URI parameter)
    rc_open = sqlite3_open_v2(filename, &db, 0, NULL);
    opt_used = sqlite3_compileoption_used("ENABLE_FTS3");
    uri_param = sqlite3_uri_parameter((sqlite3_filename)filename, "cache");

    // step 3: Operate and validate (prepare UTF-16 statement, inspect origin name, reset)
    rc_prepare16 = sqlite3_prepare16_v2(db, sql16, -1, &stmt, &pzTail);
    origin_name = sqlite3_column_origin_name16(stmt, 0);
    rc_reset = sqlite3_reset(stmt);

    // step 4: Cleanup (finalize statement and close database)
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prepare16;
    (void)rc_reset;
    (void)rc_finalize;
    (void)rc_close;
    (void)opt_used;
    (void)uri_param;
    (void)origin_name;
    (void)pzTail;
    (void)sql16;
    (void)filename;
    // API sequence test completed successfully
    return 66;
}