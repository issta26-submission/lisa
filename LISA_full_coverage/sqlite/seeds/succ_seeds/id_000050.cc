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
//<ID> 50
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_str *str = NULL;
    char *strval = NULL;
    char *expanded_sql = NULL;
    const char *decltype0 = NULL;
    const char *select_sql = "SELECT 1;";
    int rc_open = 0;
    int rc_prep = 0;
    int colcount = 0;

    // step 2: Open an in-memory database and prepare a simple SELECT
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep = sqlite3_prepare_v3(db, select_sql, -1, 0, &stmt, NULL);

    // step 3: Inspect the prepared statement and build a string with sqlite3_str API
    colcount = sqlite3_column_count(stmt);
    expanded_sql = sqlite3_expanded_sql(stmt);
    str = sqlite3_str_new(db);
    sqlite3_str_appendchar(str, 6, 'x');
    strval = sqlite3_str_finish(str);
    decltype0 = sqlite3_column_decltype(stmt, 0);

    // step 4: Reset statement and free allocated strings (cleanup)
    sqlite3_reset(stmt);
    sqlite3_free(strval);
    sqlite3_free(expanded_sql);

    // silence unused-warning for variables that demonstrate API usage
    (void)rc_open;
    (void)rc_prep;
    (void)colcount;
    (void)decltype0;
    (void)str;
    (void)db;
    (void)stmt;

    return 66;
}