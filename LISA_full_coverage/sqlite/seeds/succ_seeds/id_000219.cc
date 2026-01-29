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
//<ID> 219
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations and open database (Initialize)
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_mutex *mtx = NULL;
    char *mp = NULL;
    const char *uri_val = NULL;
    sqlite3_int64 cur = 0;
    sqlite3_int64 hi = 0;
    const char *dbfname = NULL;
    int rc_open = 0;
    int rc_status = 0;
    int rc_explain = 0;
    int rc_close = 0;

    // step 2: Setup and configuration
    rc_open = sqlite3_open(":memory:", &db);
    mtx = sqlite3_mutex_alloc(0);
    rc_status = sqlite3_status64(0, &cur, &hi, 0);

    // step 3: Operate (create formatted SQL text, inspect URI parameter, obtain stmt and explain)
    mp = sqlite3_mprintf("CREATE TABLE IF NOT EXISTS t(x INTEGER);");
    uri_val = sqlite3_uri_parameter("file:mydb?mode=memory&cache=shared", "mode");
    stmt = sqlite3_next_stmt(db, NULL);
    rc_explain = sqlite3_stmt_explain(stmt, 0);

    // step 4: Validate and cleanup (finalize stmt, examine db filename, close)
    sqlite3_finalize(stmt);
    dbfname = sqlite3_db_filename(db, "main");
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)mtx;
    (void)rc_status;
    (void)mp;
    (void)uri_val;
    (void)rc_explain;
    (void)dbfname;
    (void)rc_close;
    (void)cur;
    (void)hi;

    // API sequence test completed successfully
    return 66;
}