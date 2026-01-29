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
//<ID> 520
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_mutex *maybe_mutex = NULL;
    const char *sql_create = "CREATE TABLE t1(id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_insert = "INSERT INTO t1(data) VALUES(zeroblob(32));";
    const char *sql_select = "SELECT length(data) FROM t1 WHERE id = ?1;";
    const char *text = "bound-id-1";
    sqlite3_uint64 text_len = (sqlite3_uint64)strlen(text);
    sqlite3_int64 rowid = 1;
    int rc_open = 0;
    int rc_create = 0;
    int rc_insert = 0;
    int rc_prep = 0;
    int rc_bind = 0;
    int rc_blob_open = 0;
    int rc_blob_close = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int test_ctrl_res = 0;
    int mutex_held_res = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open(":memory:", &db);
    rc_create = sqlite3_exec(db, sql_create, 0, 0, 0);
    rc_insert = sqlite3_exec(db, sql_insert, 0, 0, 0);
    rc_prep = sqlite3_prepare_v2(db, sql_select, -1, &stmt, 0);

    // step 3: Operate / Validate
    rc_bind = sqlite3_bind_text64(stmt, 1, text, text_len, (void(*)(void *))NULL, (unsigned char)1);
    test_ctrl_res = sqlite3_test_control(1);
    mutex_held_res = sqlite3_mutex_held(maybe_mutex);
    rc_blob_open = sqlite3_blob_open(db, "main", "t1", "data", rowid, 0, &blob);

    // step 4: Cleanup
    rc_blob_close = sqlite3_blob_close(blob);
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_create;
    (void)rc_insert;
    (void)rc_prep;
    (void)rc_bind;
    (void)rc_blob_open;
    (void)rc_blob_close;
    (void)rc_finalize;
    (void)rc_close;
    (void)test_ctrl_res;
    (void)mutex_held_res;
    (void)maybe_mutex;
    (void)text_len;
    (void)rowid;
    (void)text;
    (void)sql_create;
    (void)sql_insert;
    (void)sql_select;

    // API sequence test completed successfully
    return 66;
}