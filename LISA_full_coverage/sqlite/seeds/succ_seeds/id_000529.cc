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
//<ID> 529
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
    const char *sql_create = "CREATE TABLE t1(id INTEGER PRIMARY KEY, data BLOB, txt TEXT);";
    const char *sql_insert = "INSERT INTO t1(data, txt) VALUES(?, ?);";
    const unsigned char blob_bytes[] = { 0x01, 0x02, 0x03, 0x04 };
    const char text_value[] = "hello";
    sqlite3_int64 rowid = 1;
    sqlite3_uint64 text_len = (sqlite3_uint64)5;
    int rc_open = 0;
    int rc_prep_create = 0;
    int rc_step_create = 0;
    int rc_prep_insert = 0;
    int rc_bind_blob = 0;
    int rc_bind_text64 = 0;
    int rc_step_insert = 0;
    int rc_blob_open = 0;
    int rc_blob_close = 0;
    int rc_close = 0;
    int test_ctrl = 0;
    int mutex_held = 0;

    // step 2: Setup / Configure
    rc_open = sqlite3_open(":memory:", &db);
    rc_prep_create = sqlite3_prepare_v2(db, sql_create, -1, &stmt_create, NULL);
    rc_step_create = sqlite3_step(stmt_create);
    sqlite3_finalize(stmt_create);

    // step 3: Operate / Validate
    rc_prep_insert = sqlite3_prepare_v2(db, sql_insert, -1, &stmt_insert, NULL);
    rc_bind_blob = sqlite3_bind_blob(stmt_insert, 1, (const void *)blob_bytes, (int)sizeof(blob_bytes), NULL);
    rc_bind_text64 = sqlite3_bind_text64(stmt_insert, 2, text_value, text_len, NULL, (unsigned char)1);
    rc_step_insert = sqlite3_step(stmt_insert);
    sqlite3_finalize(stmt_insert);
    rc_blob_open = sqlite3_blob_open(db, "main", "t1", "data", rowid, 0, &blob);
    test_ctrl = sqlite3_test_control(0);
    mutex_held = sqlite3_mutex_held((sqlite3_mutex *)NULL);

    // step 4: Cleanup
    rc_blob_close = sqlite3_blob_close(blob);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_prep_create;
    (void)rc_step_create;
    (void)rc_prep_insert;
    (void)rc_bind_blob;
    (void)rc_bind_text64;
    (void)rc_step_insert;
    (void)rc_blob_open;
    (void)rc_blob_close;
    (void)rc_close;
    (void)test_ctrl;
    (void)mutex_held;
    (void)rowid;
    (void)text_len;
    (void)sql_create;
    (void)sql_insert;
    (void)blob_bytes;
    (void)text_value;

    // API sequence test completed successfully
    return 66;
}