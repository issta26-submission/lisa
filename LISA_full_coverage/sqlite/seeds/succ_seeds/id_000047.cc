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
//<ID> 47
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *sql = "SELECT 42;";
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int threadsafe = 0;
    int col_type = -1;
    int explain_rc = 0;

    // step 2: Setup - open database and query thread-safety
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    threadsafe = sqlite3_threadsafe();

    // step 3: Operate / Validate - prepare, step, inspect column type, and explain the statement
    rc_prep = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    col_type = sqlite3_column_type(stmt, 0);
    explain_rc = sqlite3_stmt_explain(stmt, threadsafe & 3);

    // step 4: Cleanup - finalize statement and close database
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)threadsafe;
    (void)col_type;
    (void)explain_rc;
    // API sequence test completed successfully
    return 66;
}