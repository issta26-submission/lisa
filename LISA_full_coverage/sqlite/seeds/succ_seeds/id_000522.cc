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
//<ID> 522
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_stmt *stmt_select = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_mutex *mutex = NULL;
    const char *db_path = ":memory:";
    const char *sql_create = "CREATE TABLE t1(id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_insert = "INSERT INTO t1(data) VALUES(?);";
    const char *sql_select = "SELECT data FROM t1 WHERE id=1;";
    const char *text_value = "hello";
    sqlite3_uint64 text_len = (sqlite3_uint64)5;
    sqlite3_int64 target_row = (sqlite3_int64)1;
    int rc_open = 0;
    int rc_exec = 0;
    int rc_prep_insert = 0;
    int rc_prep_select = 0;
    int rc_bind = 0;
    int rc_transfer = 0;
    int rc_blob_open = 0;
    int rc_test = 0;
    int rc_blob_close = 0;
    int rc_finalize_insert = 0;
    int rc_finalize_select = 0;
    int rc_close = 0;
    int mutex_is_held = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open(db_path, &db);
    rc_exec = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    rc_prep_insert = sqlite3_prepare_v3(db, sql_insert, -1, 0, &stmt_insert, NULL);
    rc_prep_select = sqlite3_prepare_v3(db, sql_select, -1, 0, &stmt_select, NULL);

    // step 3: Operate / Validate (data flows between calls)
    rc_bind = sqlite3_bind_text64(stmt_insert, 1, text_value, text_len, NULL, (unsigned char)1);
    rc_transfer = sqlite3_transfer_bindings(stmt_insert, stmt_select);
    rc_blob_open = sqlite3_blob_open(db, "main", "t1", "data", target_row, 0, &blob);
    rc_test = sqlite3_test_control(0);
    mutex_is_held = sqlite3_mutex_held(mutex);

    // step 4: Cleanup
    rc_blob_close = sqlite3_blob_close(blob);
    rc_finalize_insert = sqlite3_finalize(stmt_insert);
    rc_finalize_select = sqlite3_finalize(stmt_select);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_exec;
    (void)rc_prep_insert;
    (void)rc_prep_select;
    (void)rc_bind;
    (void)rc_transfer;
    (void)rc_blob_open;
    (void)rc_test;
    (void)mutex_is_held;
    (void)rc_blob_close;
    (void)rc_finalize_insert;
    (void)rc_finalize_select;
    (void)rc_close;
    (void)db_path;
    (void)text_value;
    (void)text_len;
    (void)target_row;
    (void)mutex;
    (void)blob;
    (void)stmt_insert;
    (void)stmt_select;
    (void)db;

    // API sequence test completed successfully
    return 66;
}