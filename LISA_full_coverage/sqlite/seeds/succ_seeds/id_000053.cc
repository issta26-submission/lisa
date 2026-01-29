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
//<ID> 53
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_str *pStr = NULL;
    char **pazResult = NULL;
    char *pzErrmsg = NULL;
    char *str_val = NULL;
    int nRow = 0;
    int nCol = 0;
    int rc_open = 0;
    int rc_get = 0;
    const char *batch_sql = "CREATE TABLE t(a); INSERT INTO t(a) VALUES(1); SELECT a FROM t;";
    const char *select_sql = "SELECT a FROM t;";

    // Open an in-memory database (UTF-16 API)
    rc_open = sqlite3_open16((const void *)L":memory:", &db);

    // Execute create/insert/select as a batch and retrieve the table of results
    rc_get = sqlite3_get_table(db, batch_sql, &pazResult, &nRow, &nCol, &pzErrmsg);

    // Create a sqlite3_str and append the first returned column name (header)
    pStr = sqlite3_str_new(db);
    sqlite3_str_append(pStr, pazResult[0], (int)strlen(pazResult[0]));
    str_val = sqlite3_str_value(pStr);

    // Prepare a statement (so we have a sqlite3_stmt to pass to scanstatus reset),
    // then reset its scanstatus and finalize it.
    sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    sqlite3_stmt_scanstatus_reset(stmt);
    sqlite3_finalize(stmt);

    // Interrupt the database connection
    sqlite3_interrupt(db);

    // Silence unused-local-variable warnings for values we intentionally don't use further
    (void)rc_open;
    (void)rc_get;
    (void)nRow;
    (void)nCol;
    (void)pzErrmsg;
    (void)str_val;
    (void)batch_sql;
    (void)select_sql;

    return 66;
}