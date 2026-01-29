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
//<ID> 105
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    const char *pzTail = NULL;
    char *errmsg = NULL;
    char *expanded_sql = NULL;
    const char *errstr = NULL;
    int rc_open = 0;
    int rc_create_coll = 0;
    int rc_wal = 0;
    int rc_exec_create = 0;
    int rc_exec_insert = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_isexplain = 0;
    int rc_vtab_nochange = 0;
    int rc_finalize = 0;
    int rc_close = 0;

    // step 2: Configure / Open and register collation + WAL checkpoint
    rc_open = sqlite3_open(":memory:", &db);
    rc_create_coll = sqlite3_create_collation_v2(db, "mycoll", 1, NULL, NULL, NULL);
    rc_wal = sqlite3_wal_autocheckpoint(db, 100);

    // step 3: Operate / Prepare/Execute statements and inspect statement properties
    rc_exec_create = sqlite3_exec(db, "CREATE TABLE t(a INTEGER);", NULL, NULL, &errmsg);
    rc_exec_insert = sqlite3_exec(db, "INSERT INTO t(a) VALUES(42);", NULL, NULL, &errmsg);
    rc_prepare = sqlite3_prepare_v2(db, "EXPLAIN SELECT a FROM t;", -1, &stmt, &pzTail);
    rc_isexplain = sqlite3_stmt_isexplain(stmt);
    rc_step = sqlite3_step(stmt);
    expanded_sql = sqlite3_expanded_sql(stmt);
    rc_vtab_nochange = sqlite3_vtab_nochange(NULL);
    rc_finalize = sqlite3_finalize(stmt);
    errstr = sqlite3_errmsg(db);

    // step 4: Cleanup / Free resources and close
    sqlite3_free(expanded_sql);
    sqlite3_free(errmsg);
    rc_close = sqlite3_close_v2(db);
    (void)rc_open;
    (void)rc_create_coll;
    (void)rc_wal;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)rc_prepare;
    (void)rc_isexplain;
    (void)rc_step;
    (void)rc_vtab_nochange;
    (void)rc_finalize;
    (void)rc_close;
    (void)pzTail;
    (void)errstr;
    (void)stmt;
    // API sequence test completed successfully
    return 66;
}