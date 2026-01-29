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
//<ID> 505
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    /* step 1: Declarations & Initialize */
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_int64 target_row = (sqlite3_int64)2;
    int rc_init = 0;
    int rc_cfg = 0;
    int rc_status = 0;
    int rc_blob_open = 0;
    int rc_blob_read = 0;
    int rc_blob_reopen = 0;
    int col_count = 0;
    int busy = 0;
    int data_count = 0;
    const char *colname0 = NULL;
    const void *tblname16 = NULL;
    int coltype0 = 0;
    int colint0 = 0;
    sqlite3_int64 colint64_0 = 0;
    double coldbl0 = 0.0;
    sqlite3_int64 total_changes = 0;

    /* step 2: Configure / global calls */
    rc_init = sqlite3_initialize();
    sqlite3_enable_shared_cache(1);
    rc_cfg = sqlite3_config(0);
    rc_status = sqlite3_status(0, &rc_cfg, &rc_blob_open, 0);
    sqlite3_randomness((int)sizeof(target_row), &target_row);
    sqlite3_thread_cleanup();

    /* step 3: Operate & Validate (note: db/stmt/blob may be NULL at runtime; calls compile) */
    rc_blob_open = sqlite3_blob_open(db, "main", "t", "data", target_row, 0, &blob);
    rc_blob_read = sqlite3_blob_read(blob, &target_row, 0, 0);
    rc_blob_reopen = sqlite3_blob_reopen(blob, target_row);

    col_count = sqlite3_column_count(stmt);
    busy = sqlite3_stmt_busy(stmt);
    data_count = sqlite3_data_count(stmt);
    colname0 = sqlite3_column_name(stmt, 0);
    tblname16 = sqlite3_column_table_name16(stmt, 0);
    coltype0 = sqlite3_column_type(stmt, 0);
    colint0 = sqlite3_column_int(stmt, 0);
    colint64_0 = sqlite3_column_int64(stmt, 0);
    coldbl0 = sqlite3_column_double(stmt, 0);
    total_changes = sqlite3_total_changes64(db);

    /* step 4: Cleanup */
    sqlite3_close_v2(db);

    /* silence unused-variable warnings */
    (void)blob;
    (void)stmt;
    (void)rc_init;
    (void)rc_cfg;
    (void)rc_status;
    (void)rc_blob_open;
    (void)rc_blob_read;
    (void)rc_blob_reopen;
    (void)col_count;
    (void)busy;
    (void)data_count;
    (void)colname0;
    (void)tblname16;
    (void)coltype0;
    (void)colint0;
    (void)colint64_0;
    (void)coldbl0;
    (void)total_changes;
    (void)target_row;
    (void)db;

    return 66;
}