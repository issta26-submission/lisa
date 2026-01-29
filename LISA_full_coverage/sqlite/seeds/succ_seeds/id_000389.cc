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
//<ID> 389
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
    char *strOut = NULL;
    char *numbuf = NULL;
    const char *sql_create = "CREATE TABLE t(a INTEGER);";
    const char *sql_insert = "INSERT INTO t(a) VALUES(123);";
    const char *sql_select = "SELECT a FROM t;";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_coll = 0;
    int colval = 0;
    int scan_status = 0;
    int dummy = 0;

    // step 2: Setup - open DB and register a collation
    rc_open = sqlite3_open(":memory:", &db);
    rc_coll = sqlite3_create_collation_v2(db, "mycoll", 1, NULL, (int (*)(void *, int, const void *, int, const void *))NULL, (void (*)(void *))NULL);

    // step 3: Operate - create table, insert, select and inspect
    rc_prepare = sqlite3_prepare_v2(db, sql_create, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prepare = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prepare = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    colval = sqlite3_column_int(stmt, 0);
    rc_step = sqlite3_stmt_scanstatus(stmt, 0, 0, &scan_status);

    // step 4: Validate and Cleanup - build a string using sqlite3_str and finalize/close
    pStr = sqlite3_str_new(db);
    numbuf = sqlite3_mprintf("%d", colval);
    sqlite3_str_appendall(pStr, "col=");
    sqlite3_str_appendall(pStr, numbuf);
    strOut = sqlite3_str_finish(pStr);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    sqlite3_free_table((char **)0); /* harmless call to use an extra API; no-op here */
    sqlite3_close(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_coll;
    (void)colval;
    (void)scan_status;
    (void)pStr;
    (void)strOut;
    (void)numbuf;
    (void)dummy;

    // API sequence test completed successfully
    return 66;
}