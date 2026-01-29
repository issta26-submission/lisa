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
//<ID> 592
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_str *builder = NULL;
    char *create_sql = NULL;
    char sn_buf[128];
    char *sn_ret = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_stmt *stmt_select = NULL;
    const char *tail = NULL;
    const unsigned char *selected_text = NULL;
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_step_create = 0;
    int rc_finalize_create = 0;
    int rc_prep_insert = 0;
    int rc_bind = 0;
    int rc_step_insert = 0;
    int rc_finalize_insert = 0;
    int rc_prep_select = 0;
    int rc_step_select = 0;
    int rc_finalize_select = 0;
    int autocommit = 0;
    int rc_close = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open(":memory:", &db);

    // Allocate a small buffer for the CREATE TABLE SQL and fill it.
    create_sql = (char *)sqlite3_malloc64(128);
    if (create_sql) {
        sqlite3_snprintf(128, create_sql, "CREATE TABLE t1(id INTEGER PRIMARY KEY, info TEXT);");
    }

    // step 3: Operate / Validate
    rc_prep_create = sqlite3_prepare_v2(db, create_sql, -1, &stmt_create, &tail);
    rc_step_create = sqlite3_step(stmt_create);
    rc_finalize_create = sqlite3_finalize(stmt_create);
    rc_prep_insert = sqlite3_prepare_v2(db, "INSERT INTO t1(info) VALUES(?);", -1, &stmt_insert, NULL);
    rc_bind = sqlite3_bind_text(stmt_insert, 1, "hello", -1, (void(*)(void *))0);
    rc_step_insert = sqlite3_step(stmt_insert);
    rc_finalize_insert = sqlite3_finalize(stmt_insert);
    sn_ret = sqlite3_snprintf(sizeof(sn_buf), sn_buf, "SELECT info FROM t1 WHERE id=%d;", 1);
    rc_prep_select = sqlite3_prepare_v2(db, sn_buf, -1, &stmt_select, NULL);
    rc_step_select = sqlite3_step(stmt_select);
    selected_text = sqlite3_column_text(stmt_select, 0);
    rc_finalize_select = sqlite3_finalize(stmt_select);
    autocommit = sqlite3_get_autocommit(db);

    // step 4: Cleanup
    sqlite3_free(create_sql);
    create_sql = NULL;
    rc_close = sqlite3_close(db);
    db = NULL;

    (void)rc_open;
    (void)builder;
    (void)create_sql;
    (void)sn_ret;
    (void)sn_buf;
    (void)stmt_create;
    (void)stmt_insert;
    (void)stmt_select;
    (void)tail;
    (void)selected_text;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_finalize_create;
    (void)rc_prep_insert;
    (void)rc_bind;
    (void)rc_step_insert;
    (void)rc_finalize_insert;
    (void)rc_prep_select;
    (void)rc_step_select;
    (void)rc_finalize_select;
    (void)autocommit;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}