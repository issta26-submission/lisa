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
//<ID> 142
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    int rc_open = sqlite3_open16((const void *)u":memory:", &db);
    sqlite3_str *str = sqlite3_str_new(db);
    sqlite3_str_appendf(str, "initial-value");
    char *built = sqlite3_str_finish(str);
    sqlite3_str *str2 = sqlite3_str_new(db);
    sqlite3_str_appendf(str2, "temporary");
    sqlite3_str_reset(str2);
    const char *sql = "SELECT 1;";
    sqlite3_stmt *stmt = NULL;
    int rc_prep = sqlite3_prepare(db, sql, -1, &stmt, NULL);
    int rc_clear = sqlite3_clear_bindings(stmt);
    int rc_reset = sqlite3_reset(stmt);
    int rc_final = sqlite3_finalize(stmt);
    int errcode = sqlite3_errcode(db);
    int dbstat_cur = 0;
    int dbstat_hiwtr = 0;
    int rc_dbstatus = sqlite3_db_status(db, 0, &dbstat_cur, &dbstat_hiwtr, 0);
    sqlite3_int64 mem_used = sqlite3_memory_used();
    void *freed_built = sqlite3_realloc64(built, (sqlite3_uint64)0);
    void *freed_str2 = sqlite3_realloc64(str2, (sqlite3_uint64)0);
    int rc_close = sqlite3_close_v2(db);
    (void)rc_open;
    (void)built;
    (void)rc_prep;
    (void)rc_clear;
    (void)rc_reset;
    (void)rc_final;
    (void)errcode;
    (void)dbstat_cur;
    (void)dbstat_hiwtr;
    (void)rc_dbstatus;
    (void)mem_used;
    (void)freed_built;
    (void)freed_str2;
    (void)rc_close;
    return 66;
}