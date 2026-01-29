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
//<ID> 92
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/

int test_sqlite3_api_sequence() {
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_stmt *stmt_select = NULL;
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_step_create = 0;
    int rc_prep_insert = 0;
    int rc_step_insert = 0;
    int rc_prep_select = 0;
    int rc_step_select = 0;
    int changes_after_insert = 0;
    double val_from_column = 0.0;
    int rc_close = 0;

    rc_open = sqlite3_open_v2(":memory:", &db, 6, NULL);
    sqlite3_wal_hook(db, NULL, NULL);

    rc_prep_create = sqlite3_prepare_v2(db, "CREATE TABLE t(a INTEGER);", -1, &stmt_create, NULL);
    rc_step_create = sqlite3_step(stmt_create);

    rc_prep_insert = sqlite3_prepare_v2(db, "INSERT INTO t VALUES(42);", -1, &stmt_insert, NULL);
    rc_step_insert = sqlite3_step(stmt_insert);

    changes_after_insert = sqlite3_changes(db);

    rc_prep_select = sqlite3_prepare_v2(db, "SELECT a FROM t;", -1, &stmt_select, NULL);
    rc_step_select = sqlite3_step(stmt_select);
    val_from_column = sqlite3_column_double(stmt_select, 0);

    rc_close = sqlite3_close(db);

    (void)stmt_create;
    (void)stmt_insert;
    (void)stmt_select;
    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)changes_after_insert;
    (void)rc_prep_select;
    (void)rc_step_select;
    (void)val_from_column;
    (void)rc_close;

    return 66;
}