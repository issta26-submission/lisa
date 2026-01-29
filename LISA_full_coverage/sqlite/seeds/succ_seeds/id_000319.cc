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
//<ID> 319
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // Step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const void *sql16 = (const void *)L"CREATE TABLE demo(id INTEGER);";
    const void *pzTail = NULL;
    const void *origin_name = NULL;
    const char *uri_param = NULL;
    const char *dbfile = ":memory:";
    int rc_open = 0;
    int rc_prepare = 0;
    int opt_used = 0;

    // Step 2: Setup - check a compile-time option and open an in-memory database
    opt_used = sqlite3_compileoption_used("ENABLE_FTS5");
    rc_open = sqlite3_open_v2(dbfile, &db, 0, NULL);

    // Step 3: Core operations - prepare a UTF-16 statement, inspect URI parameter and column origin
    rc_prepare = sqlite3_prepare16_v2(db, sql16, -1, &stmt, &pzTail);
    uri_param = sqlite3_uri_parameter((sqlite3_filename)dbfile, "mode");
    origin_name = sqlite3_column_origin_name16(stmt, 0);

    // Step 4: Cleanup - reset statement, finalize and close database
    sqlite3_reset(stmt);
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    (void)opt_used;
    (void)rc_open;
    (void)rc_prepare;
    (void)uri_param;
    (void)origin_name;
    (void)pzTail;
    (void)dbfile;
    // API sequence test completed successfully
    return 66;
}