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
//<ID> 383
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = (sqlite3 *)0;
    sqlite3_stmt *stmt = (sqlite3_stmt *)0;
    sqlite3_str *pStr = (sqlite3_str *)0;
    const char *create_sql = "CREATE TABLE t(x INTEGER);";
    const char *insert_sql = "INSERT INTO t(x) VALUES(123);";
    const char *select_sql = "SELECT x FROM t LIMIT 1;";
    int rc_init = 0;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int rc_coll = 0;
    int rc_scan = 0;
    int colval = 0;
    int scan_out = 0;
    char *str_finished = (char *)0;
    const char *str_value = (const char *)0;

    // step 2: Setup (initialize and open)
    rc_init = sqlite3_initialize();
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Operate (create collation, create table, insert, query, inspect)
    rc_coll = sqlite3_create_collation_v2(db, "mycoll", 1, (void *)0, (int (*)(void *, int, const void *, int, const void *))0, (void (*)(void *))0);
    rc_prep = sqlite3_prepare_v2(db, create_sql, -1, &stmt, (const char **)0);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = (sqlite3_stmt *)0;
    rc_prep = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, (const char **)0);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = (sqlite3_stmt *)0;
    rc_prep = sqlite3_prepare_v2(db, select_sql, -1, &stmt, (const char **)0);
    rc_step = sqlite3_step(stmt);
    colval = sqlite3_column_int(stmt, 0);
    rc_scan = sqlite3_stmt_scanstatus(stmt, 0, 0, (void *)&scan_out);
    pStr = sqlite3_str_new(db);
    sqlite3_str_appendall(pStr, "value_from_str");
    str_value = sqlite3_str_value(pStr);
    str_finished = sqlite3_str_finish(pStr);
    pStr = (sqlite3_str *)0;

    // step 4: Validate & Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    stmt = (sqlite3_stmt *)0;
    rc_close = sqlite3_close(db);
    db = (sqlite3 *)0;

    (void)rc_init;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)rc_coll;
    (void)rc_scan;
    (void)colval;
    (void)scan_out;
    (void)str_value;
    (void)str_finished;

    // API sequence test completed successfully
    return 66;
}