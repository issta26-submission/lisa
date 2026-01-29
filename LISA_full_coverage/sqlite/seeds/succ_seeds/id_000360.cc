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
//<ID> 360
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_str *pstr = NULL;
    const void *errmsg16 = NULL;
    char *blob_buf = NULL;
    const char *create_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *insert_sql = "INSERT INTO t(data) VALUES(x'010203');";
    int rc_init = 0;
    int rc_auto = 0;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_blob_open = 0;
    int rc_blob_read = 0;
    sqlite3_int64 last_row = 0;
    int str_len = 0;

    // step 2: Initialize and configure auto-extension
    rc_init = sqlite3_initialize();
    rc_auto = sqlite3_auto_extension((void (*)())sqlite3_initialize);

    // step 3: Operate - open DB, create table, insert a row, open blob, read it, and build a string
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    rc_prepare = sqlite3_prepare_v2(db, create_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prepare = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    last_row = sqlite3_last_insert_rowid(db);
    rc_blob_open = sqlite3_blob_open(db, "main", "t", "data", last_row, 0, &blob);
    blob_buf = (char *)sqlite3_malloc(3);
    memset(blob_buf, 0, 3);
    rc_blob_read = sqlite3_blob_read(blob, blob_buf, 3, 0);
    sqlite3_blob_close(blob);
    blob = NULL;
    pstr = sqlite3_str_new(db);
    sqlite3_str_append(pstr, "blob_len=", 9);
    str_len = sqlite3_str_length(pstr);
    errmsg16 = sqlite3_errmsg16(db);

    // step 4: Validate and Cleanup
    sqlite3_free(blob_buf);
    sqlite3_free(pstr);
    sqlite3_close(db);
    sqlite3_shutdown();

    (void)rc_init;
    (void)rc_auto;
    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_blob_open;
    (void)rc_blob_read;
    (void)last_row;
    (void)str_len;
    (void)errmsg16;
    // API sequence test completed successfully
    return 66;
}