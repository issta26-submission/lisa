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
//<ID> 524
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations / Initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_blob *blob = NULL;
    const char *sql_create = "CREATE TABLE t1(id INTEGER PRIMARY KEY, data BLOB);";
    const char *sql_insert = "INSERT INTO t1(data) VALUES(?1);";
    const char *text_value = "HelloBlob";
    sqlite3_uint64 text_len = (sqlite3_uint64)9;
    sqlite3_int64 rowid = (sqlite3_int64)1;
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_step_create = 0;
    int rc_fini_create = 0;
    int rc_prep_insert = 0;
    int rc_bind = 0;
    int rc_step_insert = 0;
    int rc_fini_insert = 0;
    int rc_blob_open = 0;
    int rc_testctrl = 0;
    int rc_mutexheld = 0;
    int rc_blob_close = 0;
    int rc_close = 0;

    // step 2: Setup / Configure (open DB, create table)
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep_create = sqlite3_prepare_v2(db, sql_create, -1, &stmt_create, NULL);
    rc_step_create = sqlite3_step(stmt_create);
    rc_fini_create = sqlite3_finalize(stmt_create);

    // step 3: Operate / Bind, insert, open blob, test control, mutex check
    rc_prep_insert = sqlite3_prepare_v2(db, sql_insert, -1, &stmt_insert, NULL);
    rc_bind = sqlite3_bind_text64(stmt_insert, 1, text_value, text_len, NULL, (unsigned char)1);
    rc_step_insert = sqlite3_step(stmt_insert);
    rc_fini_insert = sqlite3_finalize(stmt_insert);
    rc_blob_open = sqlite3_blob_open(db, "main", "t1", "data", rowid, 0, &blob);
    rc_testctrl = sqlite3_test_control(0);
    rc_mutexheld = sqlite3_mutex_held((sqlite3_mutex *)NULL);

    // step 4: Cleanup / Finalize
    rc_blob_close = sqlite3_blob_close(blob);
    rc_close = sqlite3_close_v2(db);

    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_fini_create;
    (void)rc_prep_insert;
    (void)rc_bind;
    (void)rc_step_insert;
    (void)rc_fini_insert;
    (void)rc_blob_open;
    (void)rc_testctrl;
    (void)rc_mutexheld;
    (void)rc_blob_close;
    (void)rc_close;
    (void)text_value;
    (void)text_len;
    (void)rowid;
    (void)stmt_create;
    (void)stmt_insert;
    (void)blob;
    (void)db;

    // API sequence test completed successfully
    return 66;
}