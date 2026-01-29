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
//<ID> 560
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    int rc_os_init = sqlite3_os_init();
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_stmt *stmt_select = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_str *builder = NULL;
    char *str_value = NULL;
    char *finished = NULL;
    const char *create_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB, txt TEXT);";
    const char *insert_sql = "INSERT INTO t(txt, data) VALUES(? , zeroblob(4));";
    const char *select_sql = "SELECT id, txt FROM t;";
    int rc_open = sqlite3_open(":memory:", &db);
    int rc_exec_create = sqlite3_exec(db, create_sql, 0, 0, 0);

    // step 2: Setup / Configure
    int rc_prep_insert = sqlite3_prepare_v2(db, insert_sql, -1, &stmt_insert, 0);
    int rc_bind = sqlite3_bind_text64(stmt_insert, 1, "hello", (sqlite3_uint64)5, (void (*)(void *))0, (unsigned char)1);
    int rc_step_insert = sqlite3_step(stmt_insert);
    int rc_fin_insert = sqlite3_finalize(stmt_insert);
    int rc_blob_open = sqlite3_blob_open(db, "main", "t", "data", 1, 1, &blob);
    int rc_blob_write = sqlite3_blob_write(blob, "\x01\x02\x03\x04", 4, 0);
    int blob_bytes = sqlite3_blob_bytes(blob);

    // step 3: Operate / Validate
    int rc_blob_close = sqlite3_blob_close(blob);
    int rc_prep_select = sqlite3_prepare_v2(db, select_sql, -1, &stmt_select, 0);
    int rc_step_select = sqlite3_step(stmt_select);
    int col0_int = sqlite3_column_int(stmt_select, 0);
    const unsigned char *col1_text = sqlite3_column_text(stmt_select, 1);
    builder = sqlite3_str_new(db);
    sqlite3_str_appendf(builder, "id=%d txt=%s", col0_int, col1_text ? (const char*)col1_text : "");
    str_value = sqlite3_str_value(builder);
    finished = sqlite3_str_finish(builder);

    // step 4: Cleanup
    sqlite3_free(finished);
    int rc_fin_select = sqlite3_finalize(stmt_select);
    int rc_close = sqlite3_close(db);

    (void)rc_os_init;
    (void)rc_open;
    (void)rc_exec_create;
    (void)rc_prep_insert;
    (void)rc_bind;
    (void)rc_step_insert;
    (void)rc_fin_insert;
    (void)rc_blob_open;
    (void)rc_blob_write;
    (void)blob_bytes;
    (void)rc_blob_close;
    (void)rc_prep_select;
    (void)rc_step_select;
    (void)col0_int;
    (void)col1_text;
    (void)str_value;
    (void)rc_fin_select;
    (void)rc_close;

    // API sequence test completed successfully
    return 66;
}