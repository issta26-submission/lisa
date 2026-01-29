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
//<ID> 445
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
    const char *pzTail = NULL;
    const char *sql_create = "CREATE TABLE t(id INTEGER PRIMARY KEY, val TEXT);";
    const char *sql_insert = "INSERT INTO t(val) VALUES('hello');";
    const char *sql_select = "SELECT id, val FROM t;";
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int is_explain = 0;
    sqlite3_int64 prev_heap_limit = 0;
    char errmsg_buf[256];
    const char *errstr = NULL;
    unsigned int prepFlags = 0;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    prev_heap_limit = sqlite3_soft_heap_limit64((sqlite3_int64)(8 * 1024 * 1024)); // set soft heap limit to 8MB
    rc_prepare = sqlite3_prepare_v3(db, sql_create, -1, prepFlags, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prepare = sqlite3_prepare_v3(db, sql_insert, -1, prepFlags, &stmt, &pzTail);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 3: Operate / Validate
    rc_prepare = sqlite3_prepare_v3(db, sql_select, -1, prepFlags, &stmt, &pzTail);
    is_explain = sqlite3_stmt_isexplain(stmt);
    rc_step = sqlite3_step(stmt);
    errstr = sqlite3_errmsg(db);
    memset(errmsg_buf, 0, sizeof(errmsg_buf));
    sqlite3_snprintf(sizeof(errmsg_buf), errmsg_buf, "%s", errstr ? errstr : "no error");
    sqlite3_wal_autocheckpoint(db, 100);

    // step 4: Cleanup / Restore
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    prev_heap_limit = sqlite3_soft_heap_limit64(prev_heap_limit); // restore previous heap limit
    rc_close = sqlite3_close_v2(db);
    db = NULL;

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)is_explain;
    (void)errmsg_buf;
    (void)pzTail;
    (void)prepFlags;
    (void)prev_heap_limit;
    (void)errstr;
    // API sequence test completed successfully
    return 66;
}