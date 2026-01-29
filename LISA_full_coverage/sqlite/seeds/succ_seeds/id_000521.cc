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
//<ID> 521
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
    sqlite3_mutex *mutex = NULL;
    const char *sql_create = "CREATE TABLE t1(id INTEGER PRIMARY KEY, data TEXT);";
    const char *sql_insert_literal = "INSERT INTO t1(data) VALUES('hello');";
    const char *sql_insert_param = "INSERT INTO t1(data) VALUES(?);";
    int rc_open = 0;
    int rc_exec_create = 0;
    int rc_exec_insert = 0;
    int rc_prep = 0;
    int rc_bind = 0;
    int rc_finalize = 0;
    int rc_blob_open = 0;
    int rc_blob_close = 0;
    int rc_close = 0;
    int changes_before = 0;
    int changes_after = 0;
    int mutex_held_res = 0;
    int test_ctrl_res = 0;
    sqlite3_uint64 bind_len = (sqlite3_uint64)5;
    sqlite3_int64 rowid = 1;

    // step 2: Setup / Configure
    rc_open = sqlite3_open(":memory:", &db);
    rc_exec_create = sqlite3_exec(db, sql_create, NULL, NULL, NULL);
    rc_exec_insert = sqlite3_exec(db, sql_insert_literal, NULL, NULL, NULL);
    changes_before = sqlite3_changes(db);

    // step 3: Operate / Validate
    rc_prep = sqlite3_prepare_v2(db, sql_insert_param, -1, &stmt, NULL);
    rc_bind = sqlite3_bind_text64(stmt, 1, "hello", bind_len, NULL, (unsigned char)1);
    rc_finalize = sqlite3_finalize(stmt);
    rc_blob_open = sqlite3_blob_open(db, "main", "t1", "data", rowid, 0, &blob);
    mutex_held_res = sqlite3_mutex_held(mutex);
    test_ctrl_res = sqlite3_test_control(0);
    changes_after = sqlite3_changes(db);

    // step 4: Cleanup
    rc_blob_close = sqlite3_blob_close(blob);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_exec_create;
    (void)rc_exec_insert;
    (void)rc_prep;
    (void)rc_bind;
    (void)rc_finalize;
    (void)rc_blob_open;
    (void)rc_blob_close;
    (void)rc_close;
    (void)changes_before;
    (void)changes_after;
    (void)mutex_held_res;
    (void)test_ctrl_res;
    (void)bind_len;
    (void)rowid;
    (void)stmt;
    (void)mutex;

    // API sequence test completed successfully
    return 66;
}