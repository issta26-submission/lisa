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
//<ID> 56
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
    const char *sql_create = "CREATE TABLE t(id INTEGER);";
    const char *sql_insert = "INSERT INTO t(id) VALUES(1);";
    const char *sql_select = "SELECT id FROM t;";
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    const char *col_table = NULL;
    char *raw_buf = new char[256];
    memset(raw_buf, 0, 256);
    sqlite3_str *sstr = reinterpret_cast<sqlite3_str*>(raw_buf);
    char *sval = NULL;

    // step 2: Setup - open an in-memory database
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Operate - create table, insert a row, select and inspect column table name, build string, interrupt
    rc_prep = sqlite3_prepare_v3(db, sql_create, -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    rc_prep = sqlite3_prepare_v3(db, sql_insert, -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    rc_prep = sqlite3_prepare_v3(db, sql_select, -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    col_table = sqlite3_column_table_name(stmt, 0);
    sqlite3_stmt_scanstatus_reset(stmt);
    sqlite3_str_appendall(sstr, col_table ? col_table : ""); 
    sval = sqlite3_str_value(sstr);
    sqlite3_interrupt(db);

    // step 4: Cleanup - finalize, close, free buffer
    sqlite3_finalize(stmt);
    sqlite3_close_v2(db);
    delete [] raw_buf;
    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)col_table;
    (void)sval;
    // API sequence test completed successfully
    return 66;
}