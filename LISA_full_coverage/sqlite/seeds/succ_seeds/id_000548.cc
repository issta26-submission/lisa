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
//<ID> 548
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
    const char *sql_create = "CREATE TABLE t1(id INTEGER PRIMARY KEY, txt TEXT);";
    const char *sql_insert = "INSERT INTO t1(txt) VALUES('hello');";
    const char *sql_select = "SELECT txt FROM t1;";
    int rc_open = 0;
    int rc_prep = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int rc_enable = 0;
    sqlite3_uint64 alloc_size = (sqlite3_uint64)16;
    char *dyn_buf = NULL;
    const char *tbl_name = NULL;
    sqlite3_int64 total_changes = 0;
    int val_type = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open(":memory:", &db);
    rc_enable = sqlite3_enable_load_extension(db, 1);
    rc_prep = sqlite3_prepare_v2(db, sql_create, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    rc_prep = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);

    // step 3: Operate / Validate
    rc_prep = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    tbl_name = sqlite3_column_table_name(stmt, 0);
    dyn_buf = (char *)sqlite3_malloc64(alloc_size);
    if (dyn_buf) {
        memset(dyn_buf, 0, (size_t)alloc_size);
        // Use a safe assignment; if tbl_name is NULL, write a NUL byte.
        dyn_buf[0] = (tbl_name ? tbl_name[0] : '\0');
    }
    total_changes = sqlite3_changes64(db);
    // Use sqlite3_changes as a safe integer-like value instead of calling sqlite3_value_type on NULL.
    val_type = sqlite3_changes(db);

    // step 4: Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    sqlite3_free(dyn_buf);
    rc_enable = sqlite3_enable_load_extension(db, 0);
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prep;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_close;
    (void)rc_enable;
    (void)alloc_size;
    (void)tbl_name;
    (void)total_changes;
    (void)val_type;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;

    // API sequence test completed successfully
    return 66;
}