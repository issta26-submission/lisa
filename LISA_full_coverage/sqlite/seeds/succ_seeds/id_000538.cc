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
//<ID> 538
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_stmt *stmt_select = NULL;
    sqlite3_mutex *mutex = NULL;
    const char *sql_create = "CREATE TABLE t1(id INTEGER PRIMARY KEY, value TEXT);";
    const char *sql_insert = "INSERT INTO t1(value) VALUES('hello');";
    const char *sql_select = "SELECT value FROM t1;";
    const char *pzTail = NULL;
    int rc_init = 0;
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_step_create = 0;
    int rc_finalize_create = 0;
    int rc_prep_insert = 0;
    int rc_step_insert = 0;
    int rc_finalize_insert = 0;
    int rc_prep_select = 0;
    int readonly_flag = 0;
    int rc_finalize_select = 0;
    int rc_close = 0;

    // step 2: Setup / Configure
    rc_init = sqlite3_initialize();
    mutex = sqlite3_mutex_alloc(0);
    rc_open = sqlite3_open(":memory:", &db);

    // step 3: Operate / Validate
    rc_prep_create = sqlite3_prepare_v2(db, sql_create, -1, &stmt_create, &pzTail);
    rc_step_create = sqlite3_step(stmt_create);
    rc_finalize_create = sqlite3_finalize(stmt_create);

    rc_prep_insert = sqlite3_prepare_v2(db, sql_insert, -1, &stmt_insert, &pzTail);
    rc_step_insert = sqlite3_step(stmt_insert);
    rc_finalize_insert = sqlite3_finalize(stmt_insert);

    rc_prep_select = sqlite3_prepare_v2(db, sql_select, -1, &stmt_select, &pzTail);
    readonly_flag = sqlite3_stmt_readonly(stmt_select);
    rc_finalize_select = sqlite3_finalize(stmt_select);

    // step 4: Cleanup
    rc_close = sqlite3_close_v2(db);
    sqlite3_mutex_free(mutex);

    (void)rc_init;
    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_finalize_create;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)rc_finalize_insert;
    (void)rc_prep_select;
    (void)readonly_flag;
    (void)rc_finalize_select;
    (void)rc_close;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;
    (void)pzTail;
    (void)db;
    (void)stmt_create;
    (void)stmt_insert;
    (void)stmt_select;

    // API sequence test completed successfully
    return 66;
}