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
//<ID> 525
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
    const char *sql_create = "CREATE TABLE t1(id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_insert = "INSERT INTO t1(data) VALUES(?);";
    const char *text_val = "hello blob";
    int rc_open = 0;
    int rc_exec = 0;
    int rc_prep = 0;
    int rc_bind = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_blob_close = 0;
    int rc_close = 0;
    int tcontrol = 0;
    int held = 0;
    sqlite3_int64 rowid = 1;
    sqlite3_uint64 text_len = (sqlite3_uint64)strlen(text_val);

    // step 2: Setup / Configure
    rc_open = sqlite3_open(":memory:", &db);
    rc_exec = sqlite3_exec(db, sql_create, NULL, NULL, NULL);

    // step 3: Operate / Validate (prepare -> bind_text64 -> step -> blob_open -> test_control -> mutex check)
    rc_prep = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, NULL);
    rc_bind = sqlite3_bind_text64(stmt, 1, text_val, text_len, (void(*)(void *))NULL, (unsigned char)1);
    rc_step = sqlite3_step(stmt);
    /* assume inserted row has rowid 1 in a fresh database */
    rc_blob_close = sqlite3_blob_open(db, "main", "t1", "data", rowid, 0, &blob);
    tcontrol = sqlite3_test_control(0);
    held = sqlite3_mutex_held(mutex);

    // step 4: Cleanup
    rc_finalize = sqlite3_finalize(stmt);
    rc_blob_close = sqlite3_blob_close(blob);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_exec;
    (void)rc_prep;
    (void)rc_bind;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_blob_close;
    (void)rc_close;
    (void)tcontrol;
    (void)held;
    (void)rowid;
    (void)text_len;
    (void)sql_create;
    (void)sql_insert;
    (void)text_val;
    (void)mutex;

    // API sequence test completed successfully
    return 66;
}