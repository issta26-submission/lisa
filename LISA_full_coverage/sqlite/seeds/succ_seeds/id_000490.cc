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
//<ID> 490
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
    sqlite3_str *pStr = NULL;
    sqlite3_context *ctx = NULL;
    const char *sql = "SELECT 1";
    const char *pzTail = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_reset = 0;
    int rc_finalize = 0;
    int rc_get = 0;
    int rc_dbcfg = 0;
    char **result = NULL;
    int nRow = 0;
    int nCol = 0;
    char *zErrMsg = NULL;
    char *str_fin = NULL;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    pStr = sqlite3_str_new(db);
    sqlite3_str_appendchar(pStr, 1, 'A');
    str_fin = sqlite3_str_finish(pStr);

    // step 3: Operate / Validate
    rc_get = sqlite3_get_table(db, sql, &result, &nRow, &nCol, &zErrMsg);
    sqlite3_free_table(result);
    rc_prepare = sqlite3_prepare_v2(db, sql, -1, &stmt, &pzTail);
    rc_reset = sqlite3_reset(stmt);

    // step 4: Cleanup / Finalize
    rc_finalize = sqlite3_finalize(stmt);
    rc_dbcfg = sqlite3_db_config(db, 0);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_reset;
    (void)rc_finalize;
    (void)rc_get;
    (void)rc_dbcfg;
    (void)pStr;
    (void)ctx;
    (void)pzTail;
    (void)stmt;
    (void)sql;
    (void)str_fin;
    (void)zErrMsg;
    (void)nRow;
    (void)nCol;

    // API sequence test completed successfully
    return 66;
}