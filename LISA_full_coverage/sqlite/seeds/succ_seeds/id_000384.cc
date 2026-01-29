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
//<ID> 384
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
    sqlite3_str *sqlstr = NULL;
    const char *sql_cstr = NULL;
    const char *tail = NULL;
    char *finished_sql = NULL;
    int rc_init = 0;
    int rc_open = 0;
    int rc_prepare1 = 0;
    int rc_step1 = 0;
    int rc_finalize1 = 0;
    int rc_prepare2 = 0;
    int rc_step2 = 0;
    int rc_finalize2 = 0;
    int rc_prepare3 = 0;
    int rc_step3 = 0;
    int rc_finalize3 = 0;
    int rc_collate = 0;
    int col0_int = 0;
    int scanstatus_out = 0;
    int scanstatus_rc = 0;

    // step 2: Initialize -> Configure
    rc_init = sqlite3_initialize();
    rc_open = sqlite3_open(":memory:", &db);
    rc_collate = sqlite3_create_collation_v2(db, "mycoll", 1, NULL, NULL, NULL);
    sqlstr = sqlite3_str_new(db);
    sqlite3_str_appendall(sqlstr, "CREATE TABLE t(id INTEGER PRIMARY KEY, a INT); INSERT INTO t(a) VALUES(42); SELECT a FROM t WHERE id=1;");
    sql_cstr = sqlite3_str_value(sqlstr);

    // step 3: Operate
    rc_prepare1 = sqlite3_prepare_v2(db, sql_cstr, -1, &stmt, &tail);
    rc_step1 = sqlite3_step(stmt);
    rc_finalize1 = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prepare2 = sqlite3_prepare_v2(db, tail, -1, &stmt, &tail);
    rc_step2 = sqlite3_step(stmt);
    rc_finalize2 = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prepare3 = sqlite3_prepare_v2(db, tail, -1, &stmt, &tail);
    rc_step3 = sqlite3_step(stmt);
    col0_int = sqlite3_column_int(stmt, 0);
    scanstatus_rc = sqlite3_stmt_scanstatus(stmt, 0, 0, &scanstatus_out);
    rc_finalize3 = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 4: Validate -> Cleanup
    finished_sql = sqlite3_str_finish(sqlstr);
    sqlite3_free(finished_sql);
    sqlite3_close(db);

    (void)rc_init;
    (void)rc_open;
    (void)rc_collate;
    (void)rc_prepare1;
    (void)rc_step1;
    (void)rc_finalize1;
    (void)rc_prepare2;
    (void)rc_step2;
    (void)rc_finalize2;
    (void)rc_prepare3;
    (void)rc_step3;
    (void)rc_finalize3;
    (void)col0_int;
    (void)scanstatus_out;
    (void)scanstatus_rc;

    // API sequence test completed successfully
    return 66;
}