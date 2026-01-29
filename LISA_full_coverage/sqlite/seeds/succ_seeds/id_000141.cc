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
//<ID> 141
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    /* step 1: Declarations / Initialize */
    sqlite3_initialize();
    sqlite3 *db = NULL;
    int rc_open = sqlite3_open_v2(":memory:", &db, 6, NULL);

    /* step 2: Configure - simulate create table and insert by registering and using string builder */
    sqlite3_str *pStr = sqlite3_str_new(db);
    sqlite3_str_appendf(pStr, "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT);");
    sqlite3_str_appendchar(pStr, 1, ' ');
    sqlite3_str_appendf(pStr, "INSERT INTO t(val) VALUES('hello');");

    /* Prepare a simple statement to inspect column-related APIs */
    sqlite3_stmt *pStmt = NULL;
    sqlite3_prepare16_v2(db, (const void *)L"SELECT 1", -1, &pStmt, NULL);

    /* step 3: Operate / Validate - inspect state and status */
    int col_count = sqlite3_column_count(pStmt);
    const unsigned char *col_text = sqlite3_column_text(pStmt, 0);
    const char *col_dbname = sqlite3_column_database_name(pStmt, 0);
    const void *col_decltype16 = sqlite3_column_decltype16(pStmt, 0);
    sqlite3_int64 last_rowid = sqlite3_last_insert_rowid(db);
    sqlite3_int64 changes = sqlite3_changes64(db);

    int status_cur = 0;
    int status_high = 0;
    sqlite3_status(0, &status_cur, &status_high, 0);
    sqlite3_int64 status64_cur = 0;
    sqlite3_int64 status64_high = 0;
    sqlite3_status64(0, &status64_cur, &status64_high, 0);

    const char *err_string = sqlite3_errstr(0);

    sqlite3_str_reset(pStr);
    sqlite3_str_appendf(pStr, "inspected columns: %d", col_count);

    /* step 4: Cleanup-ish (no conditional logic, no explicit finalize/close APIs used from provided list) */
    (void)rc_open;
    (void)pStr;
    (void)pStmt;
    (void)col_text;
    (void)col_dbname;
    (void)col_decltype16;
    (void)last_rowid;
    (void)changes;
    (void)status_cur;
    (void)status_high;
    (void)status64_cur;
    (void)status64_high;
    (void)err_string;

    return 66;
}