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
//<ID> 362
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_sqlite3_api_sequence() {
    /* step 1: Declarations */
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const void *errmsg16 = NULL;
    const char *create_sql = "CREATE TABLE IF NOT EXISTS t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *insert_sql = "INSERT INTO t(data) VALUES(x'010203');";
    const char *select_sql = "SELECT data FROM t WHERE id=1;";
    sqlite3_int64 target_row = 1;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_reset = 0;
    int rc_stmt_status = 0;
    int data_count = 0;
    const void *col_blob = NULL;
    const void *col_text16 = NULL;

    /* step 2: Setup - open in-memory DB and cancel any auto-extension (no-op if none) */
    rc_open = sqlite3_open(":memory:", &db);
    sqlite3_cancel_auto_extension(NULL);

    /* step 3: Core operations - prepare statements and call a sequence of APIs (straight-line) */
    rc_prepare = sqlite3_prepare_v2(db, create_sql, -1, &stmt, NULL);
    rc_reset = sqlite3_reset(stmt);
    rc_stmt_status = sqlite3_stmt_status(stmt, 0, 0);
    sqlite3_stmt_scanstatus_reset(stmt);

    rc_prepare = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    rc_reset = sqlite3_reset(stmt);
    rc_stmt_status = sqlite3_stmt_status(stmt, 0, 1);

    rc_prepare = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    rc_reset = sqlite3_reset(stmt);
    data_count = sqlite3_data_count(stmt);
    col_blob = sqlite3_column_blob(stmt, 0);
    col_text16 = sqlite3_column_text16(stmt, 0);

    errmsg16 = sqlite3_errmsg16(db);

    /* step 4: Cleanup - reset statement and close DB */
    rc_reset = sqlite3_reset(stmt);
    sqlite3_close(db);

    /* silence unused variable warnings while preserving the straight-line API sequence */
    (void)rc_open;
    (void)rc_prepare;
    (void)rc_reset;
    (void)rc_stmt_status;
    (void)data_count;
    (void)col_blob;
    (void)col_text16;
    (void)errmsg16;
    (void)create_sql;
    (void)insert_sql;
    (void)select_sql;
    (void)target_row;

    /* API sequence test completed successfully */
    return 66;
}