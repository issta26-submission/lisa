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
//<ID> 48
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
    int rc_open = 0;
    int threadsafe = 0;
    int explain_rc = 0;
    int col_type = 0;

    // step 2: Setup - open an in-memory database
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);

    // step 3: Operate & Validate - query threadsafety and exercise stmt-related APIs (stmt is NULL but still exercised)
    threadsafe = sqlite3_threadsafe();
    explain_rc = sqlite3_stmt_explain(stmt, 1);
    col_type = sqlite3_column_type(stmt, 0);

    // step 4: Cleanup - close database and silence unused warnings
    sqlite3_close(db);
    (void)rc_open;
    (void)threadsafe;
    (void)explain_rc;
    (void)col_type;
    (void)stmt;
    (void)db;

    // API sequence test completed successfully
    return 66;
}