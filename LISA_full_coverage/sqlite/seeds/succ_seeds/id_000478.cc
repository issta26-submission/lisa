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
//<ID> 478
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
    const void *filename = (const void *)L":memory:";
    const void *sql = (const void *)L"SELECT 1;";
    const void *pzTail = NULL;
    sqlite3_int64 last_rowid = 0;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_wal = 0;
    int rc_release = 0;
    int stmt_status = 0;
    const char *stmt_sql = NULL;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open16(filename, &db);
    rc_prepare = sqlite3_prepare16_v2(db, sql, -1, &stmt, &pzTail);

    // step 3: Operate / Validate
    stmt_sql = sqlite3_sql(stmt);
    stmt_status = sqlite3_stmt_status(stmt, 0, 0);
    rc_wal = sqlite3_wal_checkpoint(db, "main");
    sqlite3_set_last_insert_rowid(db, (sqlite3_int64)42);
    rc_release = sqlite3_db_release_memory(db);

    // step 4: Cleanup / Finalize
    sqlite3_finalize(stmt);
    db = NULL;
    stmt = NULL;

    (void)rc_open;
    (void)rc_prepare;
    (void)stmt_sql;
    (void)stmt_status;
    (void)rc_wal;
    (void)last_rowid;
    (void)rc_release;
    (void)pzTail;

    return 66;
}