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
//<ID> 6
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_sqlite3_api_sequence() {
    sqlite3 *db = 0;
    sqlite3_stmt *stmt = 0;
    sqlite3_stmt *next_stmt = 0;
    sqlite3_file *file_obj = 0;
    const char *tail = 0;
    const char *colname = 0;
    const char *tblname = 0;
    int rc_open = sqlite3_open(":memory:", &db);
    int rc_prepare = sqlite3_prepare_v2(db, "SELECT 42 AS col", -1, &stmt, &tail);
    int rc_step = sqlite3_step(stmt);
    sqlite3_int64 val = sqlite3_column_int64(stmt, 0);
    colname = sqlite3_column_name(stmt, 0);
    tblname = sqlite3_column_table_name(stmt, 0);
    next_stmt = sqlite3_next_stmt(db, stmt);
    int busy = sqlite3_stmt_busy(stmt);
    sqlite3 *db_from_stmt = sqlite3_db_handle(stmt);
    int changes = sqlite3_changes(db);
    sqlite3_stmt_scanstatus_reset(stmt);
    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)val;
    (void)colname;
    (void)tblname;
    (void)file_obj;
    (void)next_stmt;
    (void)busy;
    (void)db_from_stmt;
    (void)changes;
    return 66;
}