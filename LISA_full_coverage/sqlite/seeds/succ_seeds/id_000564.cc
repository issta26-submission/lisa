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
//<ID> 564
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    int rc_os_init = 0;
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_stmt *stmt_select = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_str *builder = NULL;
    char *str_val = NULL;
    const char *sql_create = "CREATE TABLE t1(id INTEGER PRIMARY KEY, txt TEXT);";
    const char *sql_insert = "INSERT INTO t1(txt) VALUES('hello');";
    const char *sql_select = "SELECT txt FROM t1 WHERE id=?1;";
    const unsigned char *col_text = NULL;
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_step_create = 0;
    int rc_fin_create = 0;
    int rc_prep_insert = 0;
    int rc_step_insert = 0;
    int rc_fin_insert = 0;
    int rc_prep_select = 0;
    int rc_bind = 0;
    int rc_step_select = 0;
    int rc_fin_select = 0;
    int rc_blob_open = 0;
    int rc_blob_write = 0;
    int rc_blob_close = 0;
    int rc_close = 0;
    char blob_buffer[16];
    memset(blob_buffer, 0, sizeof(blob_buffer));

    // step 2: Setup / Configure
    rc_os_init = sqlite3_os_init();
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_prep_create = sqlite3_prepare_v3(db, sql_create, -1, 0, &stmt_create, NULL);
    rc_step_create = sqlite3_step(stmt_create);
    rc_fin_create = sqlite3_finalize(stmt_create);
    rc_prep_insert = sqlite3_prepare_v3(db, sql_insert, -1, 0, &stmt_insert, NULL);
    rc_step_insert = sqlite3_step(stmt_insert);
    rc_fin_insert = sqlite3_finalize(stmt_insert);
    rc_prep_select = sqlite3_prepare_v3(db, sql_select, -1, 0, &stmt_select, NULL);

    // step 3: Operate / Validate
    rc_bind = sqlite3_bind_text64(stmt_select, 1, "1", (sqlite3_uint64)1, NULL, (unsigned char)1);
    rc_step_select = sqlite3_step(stmt_select);
    col_text = sqlite3_column_text(stmt_select, 0);
    builder = sqlite3_str_new(db);
    str_val = sqlite3_str_value(builder);
    rc_blob_open = sqlite3_blob_open(db, "main", "t1", "txt", (sqlite3_int64)1, 1, &blob);
    rc_blob_write = sqlite3_blob_write(blob, (const void *)blob_buffer, (int)sizeof(blob_buffer), 0);
    rc_blob_close = sqlite3_blob_close(blob);

    // step 4: Cleanup
    rc_fin_select = sqlite3_finalize(stmt_select);
    char *finished = sqlite3_str_finish(builder);
    sqlite3_free(finished);
    rc_close = sqlite3_close(db);

    (void)rc_os_init;
    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_fin_create;
    (void)rc_prep_insert;
    (void)rc_step_insert;
    (void)rc_fin_insert;
    (void)rc_prep_select;
    (void)rc_bind;
    (void)rc_step_select;
    (void)col_text;
    (void)str_val;
    (void)rc_blob_open;
    (void)rc_blob_write;
    (void)rc_blob_close;
    (void)rc_fin_select;
    (void)rc_close;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;

    // API sequence test completed successfully
    return 66;
}