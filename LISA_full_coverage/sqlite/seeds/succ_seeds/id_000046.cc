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
//<ID> 46
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
    const char *sql = "SELECT 1 AS val;";
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_final = 0;
    int rc_close = 0;
    int threadsafe = 0;
    int col_type = 0;
    int explain_rc = 0;

    // step 2: Setup - open an in-memory database and query threadsafety
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    threadsafe = sqlite3_threadsafe();

    // step 3: Operate / Validate - prepare, step, inspect column type and get statement explanation
    rc_prep = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    col_type = sqlite3_column_type(stmt, 0);
    explain_rc = sqlite3_stmt_explain(stmt, 0);

    // step 4: Cleanup - finalize statement and close database
    rc_final = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_final;
    (void)rc_close;
    (void)threadsafe;
    (void)col_type;
    (void)explain_rc;
    (void)sql;

    // API sequence test completed successfully
    return 66;
}