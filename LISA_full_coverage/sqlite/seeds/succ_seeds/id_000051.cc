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
//<ID> 51
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
    sqlite3_str *pstr = NULL;
    char *sql_text = NULL;
    const char *table_name = NULL;
    int rc_open = 0;
    int rc_exec = 0;
    int rc_prep = 0;
    int rc_step = 0;

    // step 2: Setup - open in-memory DB, create a table and insert a row, build SQL using sqlite3_str
    rc_open = sqlite3_open(":memory:", &db);
    rc_exec = sqlite3_exec(db, "CREATE TABLE t(id INTEGER); INSERT INTO t VALUES(123);", NULL, NULL, NULL);
    pstr = sqlite3_str_new(db);
    sqlite3_str_appendall(pstr, "SELECT id FROM t;");
    sql_text = sqlite3_str_value(pstr);

    // step 3: Core operations - prepare and step the statement, inspect column table name, reset scanstatus, interrupt DB
    rc_prep = sqlite3_prepare_v3(db, sql_text, -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    table_name = sqlite3_column_table_name(stmt, 0);
    sqlite3_stmt_scanstatus_reset(stmt);
    sqlite3_interrupt(db);

    // step 4: Cleanup - finalize statement and close database; silence unused variables
    sqlite3_finalize(stmt);
    sqlite3_close_v2(db);
    (void)rc_open;
    (void)rc_exec;
    (void)rc_prep;
    (void)rc_step;
    (void)pstr;
    (void)sql_text;
    (void)table_name;

    // API sequence test completed successfully
    return 66;
}