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
//<ID> 528
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
    const char *sql_insert = "INSERT INTO t1(data) VALUES(?);";
    const char *text = "hello sqlite3";
    sqlite3_int64 rowid = 1;
    int rc_open = 0;
    int rc_exec = 0;
    int rc_prep = 0;
    int rc_bind = 0;
    int rc_step = 0;
    int rc_blob = 0;
    int rc_finalize = 0;
    int rc_close = 0;
    int tc_result = 0;
    int mutex_held = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open(":memory:", &db);
    rc_exec = sqlite3_exec(db, sql_create, NULL, NULL, NULL);

    // step 3: Operate / Use APIs (prepare, bind, execute, test control, mutex check, open blob)
    rc_prep = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    rc_bind = sqlite3_bind_text64(stmt, 1, text, (sqlite3_uint64)strlen(text), (void(*)(void *))NULL, (unsigned char)1);
    rc_step = sqlite3_step(stmt);
    tc_result = sqlite3_test_control(1);
    mutex_held = sqlite3_mutex_held(mutex);
    rc_blob = sqlite3_blob_open(db, "main", "t1", "data", rowid, 0, &blob);

    // step 4: Cleanup / Finalize
    rc_finalize = sqlite3_finalize(stmt);
    rc_blob = sqlite3_blob_close(blob);
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_exec;
    (void)rc_prep;
    (void)rc_bind;
    (void)rc_step;
    (void)tc_result;
    (void)mutex_held;
    (void)rc_blob;
    (void)rc_finalize;
    (void)rc_close;
    (void)sql_create;
    (void)sql_insert;
    (void)text;
    (void)rowid;
    (void)mutex;

    // API sequence test completed successfully
    return 66;
}