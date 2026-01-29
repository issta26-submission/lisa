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
//<ID> 526
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
    sqlite3_blob *blob = NULL;
    sqlite3_mutex *mx = NULL;
    const char *sql_create = "CREATE TABLE t1(id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_insert = "INSERT INTO t1(data) VALUES(?);";
    const char *mem_uri = ":memory:";
    int rc_init = 0;
    int rc_open = 0;
    int rc_exec = 0;
    int rc_prep = 0;
    int rc_bind = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_blob_open = 0;
    int rc_blob_close = 0;
    int rc_testctrl = 0;
    int rc_mutexheld = 0;
    int rc_close = 0;
    sqlite3_int64 rowid = 1;
    const char *payload = "payload";

    // step 2: Setup / Configure
    rc_init = sqlite3_initialize();
    rc_open = sqlite3_open(mem_uri, &db);
    rc_exec = sqlite3_exec(db, sql_create, 0, 0, 0);
    rc_prep = sqlite3_prepare_v2(db, sql_insert, -1, &stmt_insert, NULL);

    // step 3: Operate / Validate (data flows between calls)
    rc_bind = sqlite3_bind_text64(stmt_insert, 1, payload, (sqlite3_uint64)7, (void(*)(void *))NULL, (unsigned char)1);
    rc_step = sqlite3_step(stmt_insert);
    rc_finalize = sqlite3_finalize(stmt_insert);
    rc_blob_open = sqlite3_blob_open(db, "main", "t1", "data", rowid, 0, &blob);
    rc_testctrl = sqlite3_test_control(0);
    rc_mutexheld = sqlite3_mutex_held(mx);

    // step 4: Cleanup
    rc_blob_close = sqlite3_blob_close(blob);
    rc_close = sqlite3_close_v2(db);

    (void)rc_init;
    (void)rc_open;
    (void)rc_exec;
    (void)rc_prep;
    (void)rc_bind;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_blob_open;
    (void)rc_testctrl;
    (void)rc_mutexheld;
    (void)rc_blob_close;
    (void)rc_close;
    (void)rowid;
    (void)payload;
    (void)mx;

    // API sequence test completed successfully
    return 66;
}