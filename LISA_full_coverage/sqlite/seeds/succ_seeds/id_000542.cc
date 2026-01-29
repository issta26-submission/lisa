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
//<ID> 542
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_str *str = NULL;
    void *buffer = NULL;
    sqlite3_mutex *mx = NULL;
    char *msg = NULL;
    const char *sql = "CREATE TABLE t1(id INTEGER PRIMARY KEY, val INTEGER);";
    const char *stmt_sql = NULL;
    int rc_open = 0;
    int rc_prep = 0;
    int rc_final = 0;
    sqlite3_uint64 buf_size = 0;
    sqlite3_int64 hw = 0;

    rc_open = sqlite3_open16((const void *)L":memory:", &db);
    rc_prep = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    stmt_sql = sqlite3_sql(stmt);
    str = sqlite3_str_new(db);
    sqlite3_str_appendall(str, "built:");
    sqlite3_str_appendall(str, stmt_sql);
    sqlite3_str_appendf(str, " -done");
    msg = sqlite3_mprintf("lib=%s", sqlite3_libversion());
    buffer = sqlite3_realloc(NULL, 128);
    sqlite3_randomness(128, buffer);
    buf_size = sqlite3_msize(buffer);
    hw = sqlite3_memory_highwater(0);
    mx = sqlite3_mutex_alloc(0);
    rc_final = sqlite3_finalize(stmt);
    buffer = sqlite3_realloc(buffer, 0);

    (void)rc_open;
    (void)rc_prep;
    (void)rc_final;
    (void)db;
    (void)str;
    (void)msg;
    (void)buffer;
    (void)mx;
    (void)sql;
    (void)stmt_sql;
    (void)buf_size;
    (void)hw;

    return 66;
}