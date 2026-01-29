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
//<ID> 545
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
    sqlite3_value *colval = NULL;
    void *mem = NULL;
    const char *tbl_name = NULL;
    sqlite3_int64 total_changes = 0;
    sqlite3_uint64 alloc_size = (sqlite3_uint64)128;
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_step_create = 0;
    int rc_finalize_create = 0;
    int rc_prep_insert = 0;
    int rc_step_insert = 0;
    int rc_finalize_insert = 0;
    int rc_prep_select = 0;
    int rc_step_select = 0;
    int rc_finalize_select = 0;
    int rc_enable_ext_on = 0;
    int rc_enable_ext_off = 0;
    int col_type = 0;
    int rc_close = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep_create = sqlite3_prepare_v3(db, "CREATE TABLE t1(id INTEGER PRIMARY KEY, a TEXT);", -1, 0, &stmt_create, NULL);
    rc_step_create = sqlite3_step(stmt_create);
    rc_finalize_create = sqlite3_finalize(stmt_create);
    rc_prep_insert = sqlite3_prepare_v3(db, "INSERT INTO t1(a) VALUES('hello');", -1, 0, &stmt_insert, NULL);
    rc_step_insert = sqlite3_step(stmt_insert);
    rc_finalize_insert = sqlite3_finalize(stmt_insert);

    // step 3: Operate / Validate
    rc_prep_select = sqlite3_prepare_v3(db, "SELECT a FROM t1;", -1, 0, &stmt_select, NULL);
    rc_step_select = sqlite3_step(stmt_select);
    tbl_name = sqlite3_column_table_name(stmt_select, 0);
    colval = sqlite3_column_value(stmt_select, 0);
    col_type = sqlite3_value_type(colval);
    total_changes = sqlite3_total_changes64(db);
    rc_enable_ext_on = sqlite3_enable_load_extension(db, 1);
    mem = sqlite3_realloc64(NULL, alloc_size);
    mem = sqlite3_realloc64(mem, alloc_size * 2);

    // step 4: Cleanup
    rc_enable_ext_off = sqlite3_enable_load_extension(db, 0);
    rc_finalize_select = sqlite3_finalize(stmt_select);
    mem = sqlite3_realloc64(mem, 0);
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_finalize_create;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)rc_finalize_insert;
    (void)rc_prep_select;
    (void)rc_step_select;
    (void)tbl_name;
    (void)colval;
    (void)col_type;
    (void)total_changes;
    (void)rc_enable_ext_on;
    (void)rc_enable_ext_off;
    (void)alloc_size;
    (void)rc_finalize_select;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}