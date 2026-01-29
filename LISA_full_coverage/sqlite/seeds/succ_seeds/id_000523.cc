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
//<ID> 523
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
    const char *sql_insert = "INSERT INTO t1(data) VALUES(?);";
    const char *sql_count = "SELECT count(*) FROM t1;";
    char zero_buf[32];
    sqlite3_int64 rowid = 1;
    int rc_open = 0;
    int rc_exec_create = 0;
    int rc_prep = 0;
    int rc_bind = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_blob = 0;
    int rc_blob_close = 0;
    int rc_test = 0;
    int rc_mutex_held = 0;
    int rc_close = 0;
    memset(zero_buf, 0, sizeof(zero_buf));

    // step 2: Setup / Configure
    rc_open = sqlite3_open(":memory:", &db);
    rc_exec_create = sqlite3_exec(db, sql_create, NULL, NULL, NULL);

    // step 3: Operate / Use APIs (data flows between calls)
    rc_prep = sqlite3_prepare_v3(db, sql_insert, -1, 0, &stmt, NULL);
    rc_bind = sqlite3_bind_text64(stmt, 1, "hello", (sqlite3_uint64)5, (void(*)(void *))NULL, (unsigned char)1);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    rc_test = sqlite3_test_control(0);
    rc_mutex_held = sqlite3_mutex_held(maybe_mutex);
    rc_blob = sqlite3_blob_open(db, "main", "t1", "data", rowid, 0, &blob);
    rc_blob_close = sqlite3_blob_close(blob);

    // step 4: Cleanup / Validate
    (void)sqlite3_prepare_v3(db, sql_count, -1, 0, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    rc_close = sqlite3_close(db);

    (void)zero_buf;
    (void)rc_open;
    (void)rc_exec_create;
    (void)rc_prep;
    (void)rc_bind;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_blob;
    (void)rc_blob_close;
    (void)rc_test;
    (void)rc_mutex_held;
    (void)rc_close;
    (void)rowid;
    // API sequence test completed successfully
    return 66;
}