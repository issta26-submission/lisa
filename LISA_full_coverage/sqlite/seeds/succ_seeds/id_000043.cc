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
//<ID> 43
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
    const char *sql = "SELECT 1 AS num, 'txt' AS txtcol;";
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_explain = 0;
    int col0_type = 0;
    int col1_type = 0;
    int threadsafe = 0;

    // step 2: Setup - open an in-memory database and query threadsafety
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    threadsafe = sqlite3_threadsafe();

    // step 3: Core operations - prepare a simple SELECT, step to a row, inspect column types and explain the statement
    rc_prep = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    col0_type = sqlite3_column_type(stmt, 0);
    col1_type = sqlite3_column_type(stmt, 1);
    rc_explain = sqlite3_stmt_explain(stmt, 0);

    // step 4: Cleanup - finalize statement and close database
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    (void)rc_open;
    (void)threadsafe;
    (void)rc_prep;
    (void)rc_step;
    (void)col0_type;
    (void)col1_type;
    (void)rc_explain;
    (void)sql;

    // API sequence test completed successfully
    return 66;
}