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
//<ID> 52
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
    sqlite3_str *pStr = NULL;
    char *strval = NULL;
    char *final_str = NULL;
    const char *tblname = NULL;
    const char *sql_create = "CREATE TABLE t1(id INTEGER PRIMARY KEY, val TEXT); INSERT INTO t1(val) VALUES('hello');";
    const char *sql_select = "SELECT id, val FROM t1;";
    int rc_open = 0;
    int rc_exec = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_fin = 0;
    int rc_close = 0;

    // step 2: Setup - open in-memory database and create a table with one row
    rc_open = sqlite3_open(":memory:", &db);
    rc_exec = sqlite3_exec(db, sql_create, NULL, NULL, NULL);

    // step 3: Core operations - prepare and step the select, inspect column table name, reset scanstatus, interrupt DB,
    //                build a string using sqlite3_str APIs and read its value
    rc_prep = sqlite3_prepare_v3(db, sql_select, -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    tblname = sqlite3_column_table_name(stmt, 1);
    sqlite3_stmt_scanstatus_reset(stmt);
    sqlite3_interrupt(db);
    pStr = sqlite3_str_new(db);
    sqlite3_str_appendall(pStr, "table:");
    sqlite3_str_appendall(pStr, tblname ? tblname : "unknown");
    strval = sqlite3_str_value(pStr);
    final_str = sqlite3_str_finish(pStr);

    // step 4: Cleanup - finalize statement, free string buffer, and close database
    rc_fin = sqlite3_finalize(stmt);
    if (final_str) sqlite3_free(final_str);
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_exec;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_fin;
    (void)rc_close;
    (void)strval;
    (void)tblname;

    // API sequence test completed successfully
    return 66;
}