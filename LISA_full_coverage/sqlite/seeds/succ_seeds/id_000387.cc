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
//<ID> 387
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations & Initialize
    sqlite3 *db = (sqlite3 *)0;
    sqlite3_stmt *stmt = (sqlite3_stmt *)0;
    sqlite3_str *pStr = (sqlite3_str *)0;
    char *numstr = (char *)0;
    char *finalstr = (char *)0;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, v INT);";
    const char *sql_insert = "INSERT INTO t(v) VALUES(42);";
    const char *sql_select = "SELECT v FROM t WHERE id=1;";
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_coll = 0;
    int col_value = 0;
    int scan_out = 0;

    sqlite3_initialize();

    // step 2: Configure (open DB and register a collation)
    rc_open = sqlite3_open(":memory:", &db);
    rc_coll = sqlite3_create_collation_v2(db, "mycoll", 1, (void *)0, (int (*)(void *, int, const void *, int, const void *))0, (void (*)(void *))0);

    // step 3: Operate (create table, insert row, select and read integer, inspect scan status)
    rc_prep = sqlite3_prepare_v2(db, sql_create, -1, &stmt, (const char **)0);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = (sqlite3_stmt *)0;

    rc_prep = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, (const char **)0);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = (sqlite3_stmt *)0;

    rc_prep = sqlite3_prepare_v2(db, sql_select, -1, &stmt, (const char **)0);
    rc_step = sqlite3_step(stmt);
    col_value = sqlite3_column_int(stmt, 0);
    rc_step = sqlite3_stmt_scanstatus(stmt, 0, 0, (void *)&scan_out);

    // step 4: Validate & Cleanup (build a small string with sqlite3_str and clean resources)
    pStr = sqlite3_str_new(db);
    sqlite3_str_appendall(pStr, "v=");
    numstr = sqlite3_mprintf("%d", col_value);
    sqlite3_str_appendall(pStr, numstr);
    finalstr = sqlite3_str_finish(pStr);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = (sqlite3_stmt *)0;
    rc_open = sqlite3_close(db);
    db = (sqlite3 *)0;

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_coll;
    (void)col_value;
    (void)scan_out;
    (void)numstr;
    (void)finalstr;
    (void)pStr;

    // API sequence test completed successfully
    return 66;
}