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
//<ID> 102
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
    const void *sql_create = (const void *)"CREATE TABLE t(id INTEGER PRIMARY KEY, value TEXT);";
    const void *sql_insert = (const void *)"INSERT INTO t(value) VALUES('hello');";
    const void *sql_select = (const void *)"SELECT value FROM t;";
    char *errmsg = NULL;
    const unsigned char *selected_text = NULL;
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_step_create = 0;
    int rc_finalize_create = 0;
    int rc_prep_insert = 0;
    int rc_step_insert = 0;
    int rc_finalize_insert = 0;
    int rc_prep_select = 0;
    int rc_step_select = 0;
    int rc_isexplain = 0;
    int rc_finalize_select = 0;
    int rc_coll = 0;
    int rc_wal = 0;
    int rc_nochange = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open16((const void *)":memory:", &db);
    rc_coll = sqlite3_create_collation_v2(db, "mycoll", 1, NULL, NULL, NULL);
    rc_wal = sqlite3_wal_autocheckpoint(db, 10);

    // step 3: Operate / Validate
    rc_prep_create = sqlite3_prepare16(db, sql_create, -1, &stmt_create, NULL);
    rc_step_create = sqlite3_step(stmt_create);
    rc_finalize_create = sqlite3_finalize(stmt_create);
    rc_prep_insert = sqlite3_prepare16(db, sql_insert, -1, &stmt_insert, NULL);
    rc_step_insert = sqlite3_step(stmt_insert);
    rc_finalize_insert = sqlite3_finalize(stmt_insert);
    rc_prep_select = sqlite3_prepare16(db, sql_select, -1, &stmt_select, NULL);
    rc_step_select = sqlite3_step(stmt_select);
    selected_text = sqlite3_column_text(stmt_select, 0);
    rc_isexplain = sqlite3_stmt_isexplain(stmt_select);
    rc_nochange = sqlite3_vtab_nochange((sqlite3_context *)NULL);
    rc_finalize_select = sqlite3_finalize(stmt_select);

    // step 4: Cleanup
    int rc_close = sqlite3_close_v2(db);
    (void)errmsg;
    (void)selected_text;
    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_finalize_create;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)rc_finalize_insert;
    (void)rc_prep_select;
    (void)rc_step_select;
    (void)rc_isexplain;
    (void)rc_finalize_select;
    (void)rc_coll;
    (void)rc_wal;
    (void)rc_nochange;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}