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
//<ID> 101
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
    const void *pzTail = NULL;
    const unsigned char *col0_text = NULL;
    sqlite3_int64 col0_int = 0;
    int rc_open16 = 0;
    int rc_prep_create = 0;
    int rc_step_create = 0;
    int rc_finalize_create = 0;
    int rc_prep_insert = 0;
    int rc_step_insert = 0;
    int rc_finalize_insert = 0;
    int rc_prep_select = 0;
    int rc_step_select = 0;
    int rc_finalize_select = 0;
    int rc_wal_autocp = 0;
    int rc_create_coll = 0;
    int is_explain = 0;
    int rc_vtab_nochange = 0;
    int rc_close = 0;

    // step 2: Configure
    rc_open16 = sqlite3_open16((const void*)u":memory:", &db);
    rc_wal_autocp = sqlite3_wal_autocheckpoint(db, 100);
    rc_create_coll = sqlite3_create_collation_v2(db, "mycoll", 1, NULL, NULL, NULL);

    // step 3: Operate / Validate
    rc_prep_create = sqlite3_prepare16(db, (const void*)u"CREATE TABLE t(id INTEGER PRIMARY KEY, val INTEGER);", -1, &stmt_create, &pzTail);
    rc_step_create = sqlite3_step(stmt_create);
    rc_finalize_create = sqlite3_finalize(stmt_create);

    rc_prep_insert = sqlite3_prepare16(db, (const void*)u"INSERT INTO t(val) VALUES(42);", -1, &stmt_insert, &pzTail);
    rc_step_insert = sqlite3_step(stmt_insert);
    rc_finalize_insert = sqlite3_finalize(stmt_insert);

    rc_prep_select = sqlite3_prepare16(db, (const void*)u"SELECT val FROM t LIMIT 1;", -1, &stmt_select, &pzTail);
    is_explain = sqlite3_stmt_isexplain(stmt_select);
    rc_step_select = sqlite3_step(stmt_select);
    col0_int = sqlite3_column_int64(stmt_select, 0);
    col0_text = sqlite3_column_text(stmt_select, 0);
    rc_vtab_nochange = sqlite3_vtab_nochange(NULL);
    rc_finalize_select = sqlite3_finalize(stmt_select);

    // step 4: Cleanup
    rc_close = sqlite3_close_v2(db);
    (void)rc_open16;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_finalize_create;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)rc_finalize_insert;
    (void)rc_prep_select;
    (void)rc_step_select;
    (void)rc_finalize_select;
    (void)rc_wal_autocp;
    (void)rc_create_coll;
    (void)is_explain;
    (void)rc_vtab_nochange;
    (void)col0_int;
    (void)col0_text;
    (void)pzTail;
    (void)stmt_create;
    (void)stmt_insert;
    (void)stmt_select;
    (void)db;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}