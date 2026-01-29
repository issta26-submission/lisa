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
//<ID> 547
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_stmt *stmt_select = NULL;
    sqlite3_value *colval = NULL;
    void *buffer = NULL;
    const char *table_name = NULL;
    sqlite3_int64 total_changes = 0;
    sqlite3_uint64 alloc_size = (sqlite3_uint64)16;
    const char *sql_create = "CREATE TABLE t1(id INTEGER PRIMARY KEY, a INTEGER, b TEXT);";
    const char *sql_insert = "INSERT INTO t1(a,b) VALUES(?,?);";
    const char *sql_select = "SELECT a, b FROM t1 WHERE id = ?;";
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_step_create = 0;
    int rc_fin_create = 0;
    int rc_prep_insert = 0;
    int rc_bind_insert_1 = 0;
    int rc_bind_insert_2 = 0;
    int rc_step_insert = 0;
    int rc_fin_insert = 0;
    int rc_enable_ext = 0;
    int rc_prep_select = 0;
    int rc_bind_select = 0;
    int rc_step_select = 0;
    int rc_fin_select = 0;
    int rc_close = 0;
    int col0_type = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep_create = sqlite3_prepare_v3(db, sql_create, -1, 0, &stmt_create, NULL);
    rc_step_create = sqlite3_step(stmt_create);
    rc_fin_create = sqlite3_finalize(stmt_create);

    // step 3: Operate / Validate
    rc_prep_insert = sqlite3_prepare_v3(db, sql_insert, -1, 0, &stmt_insert, NULL);
    rc_bind_insert_1 = sqlite3_bind_int(stmt_insert, 1, 123);
    rc_bind_insert_2 = sqlite3_bind_null(stmt_insert, 2);
    rc_step_insert = sqlite3_step(stmt_insert);
    rc_fin_insert = sqlite3_finalize(stmt_insert);
    rc_enable_ext = sqlite3_enable_load_extension(db, 1);
    rc_prep_select = sqlite3_prepare_v3(db, sql_select, -1, 0, &stmt_select, NULL);
    rc_bind_select = sqlite3_bind_int(stmt_select, 1, 1);
    rc_step_select = sqlite3_step(stmt_select);
    table_name = sqlite3_column_table_name(stmt_select, 0);
    colval = (sqlite3_value *)sqlite3_column_value(stmt_select, 0);
    col0_type = sqlite3_value_type(colval);
    total_changes = sqlite3_total_changes64(db);
    buffer = sqlite3_realloc64(NULL, alloc_size);
    memset(buffer, 0, (size_t)alloc_size);

    // step 4: Cleanup
    rc_fin_select = sqlite3_finalize(stmt_select);
    buffer = sqlite3_realloc64(buffer, 0); /* free via realloc with size 0 */
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_fin_create;
    (void)rc_prep_insert;
    (void)rc_bind_insert_1;
    (void)rc_bind_insert_2;
    (void)rc_step_insert;
    (void)rc_fin_insert;
    (void)rc_enable_ext;
    (void)rc_prep_select;
    (void)rc_bind_select;
    (void)rc_step_select;
    (void)table_name;
    (void)colval;
    (void)col0_type;
    (void)total_changes;
    (void)buffer;
    (void)alloc_size;
    (void)rc_fin_select;
    (void)rc_close;
    // API sequence test completed successfully
    return 66;
}