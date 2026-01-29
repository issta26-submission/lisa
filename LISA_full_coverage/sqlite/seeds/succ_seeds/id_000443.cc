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
//<ID> 443
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
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT);";
    const char *sql_insert = "INSERT INTO t(id, val) VALUES(1, 'one');";
    const char *sql_select = "SELECT val FROM t WHERE id=?;";
    const char *pzTail = NULL;
    unsigned int prepFlags = 0;
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_prep_insert = 0;
    int rc_prep_select = 0;
    int rc_wal = 0;
    int rc_bind = 0;
    int is_explain = 0;
    sqlite3_int64 prev_heap_limit = 0;
    sqlite3_int64 new_heap_limit = (sqlite3_int64)(50 * 1024 * 1024); // 50MB
    int rc_finalize = 0;
    int rc_close = 0;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 6, NULL); // SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE == 6
    prev_heap_limit = sqlite3_soft_heap_limit64(new_heap_limit);
    rc_wal = sqlite3_wal_autocheckpoint(db, 100);

    // step 3: Operate / Validate
    rc_prep_create = sqlite3_prepare_v3(db, sql_create, -1, prepFlags, &stmt, &pzTail);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prep_insert = sqlite3_prepare_v3(db, sql_insert, -1, prepFlags, &stmt, &pzTail);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prep_select = sqlite3_prepare_v3(db, sql_select, -1, prepFlags, &stmt, &pzTail);
    is_explain = sqlite3_stmt_isexplain(stmt);
    rc_bind = sqlite3_bind_int(stmt, 1, 1);

    // step 4: Cleanup / Finalize
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;
    (void)pzTail;
    (void)prepFlags;
    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_prep_insert;
    (void)rc_prep_select;
    (void)rc_wal;
    (void)rc_bind;
    (void)is_explain;
    (void)prev_heap_limit;
    (void)new_heap_limit;
    (void)rc_finalize;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}