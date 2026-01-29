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
//<ID> 217
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Initialize and open an in-memory database
    sqlite3 *db = NULL;
    sqlite3_initialize();
    int rc_open = sqlite3_open(":memory:", &db);

    // step 2: Configure: inspect URI parameter, allocate a mutex, and query status
    const char *cache_param = sqlite3_uri_parameter(":memory:?cache=shared&mode=memory", "cache");
    sqlite3_mutex *mtx = sqlite3_mutex_alloc(0);
    sqlite3_int64 status_cur1 = 0;
    sqlite3_int64 status_hi1 = 0;
    int status_rc1 = sqlite3_status64(0, &status_cur1, &status_hi1, 0);

    // step 3: Operate: create a table, insert a row, prepare a statement and explain it
    char *sql_create = sqlite3_mprintf("CREATE TABLE t(a INTEGER);");
    int rc_exec_create = sqlite3_exec(db, sql_create, 0, 0, 0);
    char *sql_insert = sqlite3_mprintf("INSERT INTO t(a) VALUES (42);");
    int rc_exec_insert = sqlite3_exec(db, sql_insert, 0, 0, 0);
    sqlite3_stmt *stmt = NULL;
    int rc_prep = sqlite3_prepare_v2(db, "SELECT a FROM t;", -1, &stmt, 0);
    int explain_rc = sqlite3_stmt_explain(stmt, 0);
    int rc_step = sqlite3_step(stmt);
    int rc_fini = sqlite3_finalize(stmt);

    // step 4: Validate and cleanup: query status, get error code, free resources and close
    sqlite3_int64 status_cur2 = 0;
    sqlite3_int64 status_hi2 = 0;
    int status_rc2 = sqlite3_status64(0, &status_cur2, &status_hi2, 1);
    int err_code = sqlite3_errcode(db);
    int rc_close = sqlite3_close_v2(db);
    sqlite3_free(sql_create);
    sqlite3_free(sql_insert);
    sqlite3_mutex_free(mtx);

    // API sequence test completed successfully
    (void)rc_open;
    (void)cache_param;
    (void)status_rc1;
    (void)status_cur1;
    (void)status_hi1;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)rc_prep;
    (void)explain_rc;
    (void)rc_step;
    (void)rc_fini;
    (void)status_rc2;
    (void)status_cur2;
    (void)status_hi2;
    (void)err_code;
    (void)rc_close;

    return 66;
}