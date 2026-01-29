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
//<ID> 366
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations and initialize
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt = NULL;
    sqlite3_blob *blob = NULL;
    sqlite3_str *str = NULL;
    void *buffer = NULL;
    const void *errmsg16 = NULL;
    const char *create_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *insert_sql = "INSERT INTO t(data) VALUES(x'0102');";
    int rc_init = 0;
    int rc_auto = 0;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_blob_open = 0;
    int rc_blob_read = 0;

    rc_init = sqlite3_initialize();

    // step 2: Configure and open database
    rc_auto = sqlite3_auto_extension((void(*)())0);
    rc_open = sqlite3_open_v2(":memory:", &db, 0, NULL);
    str = sqlite3_str_new(db);
    sqlite3_str_append(str, create_sql, (int)strlen(create_sql));

    // step 3: Operate and validate - create table, insert row, open blob, read, and inspect error message (UTF-16)
    rc_prepare = sqlite3_prepare_v2(db, create_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prepare = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    rc_blob_open = sqlite3_blob_open(db, "main", "t", "data", (sqlite3_int64)1, 0, &blob);
    buffer = sqlite3_malloc(16);
    memset(buffer, 0, 16);
    rc_blob_read = sqlite3_blob_read(blob, buffer, 2, 0);
    sqlite3_blob_close(blob);
    blob = NULL;

    errmsg16 = sqlite3_errmsg16(db);

    // step 4: Cleanup
    sqlite3_free(str);
    sqlite3_free(buffer);
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
    (void)errmsg16;
    (void)create_sql;
    (void)insert_sql;

    // API sequence test completed successfully
    return 66;
}