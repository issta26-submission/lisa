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
//<ID> 364
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
    sqlite3_str *builder = NULL;
    const void *errmsg16 = NULL;
    char *str_result = NULL;
    const char *create_sql = "CREATE TABLE t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *insert_sql = "INSERT INTO t(data) VALUES(?);";
    const unsigned char blob_data[] = {0x01, 0x02, 0x03};
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    sqlite3_int64 rowid = 0;

    // step 2: Initialize / Configure
    rc_open = sqlite3_open(":memory:", &db);
    sqlite3_auto_extension((void (*)())0);
    builder = sqlite3_str_new(db);
    sqlite3_str_append(builder, "blob-inserted", (int)(sizeof("blob-inserted") - 1));

    // step 3: Operate / Validate
    rc_prepare = sqlite3_prepare_v2(db, create_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prepare = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    sqlite3_bind_blob(stmt, 1, blob_data, (int)sizeof(blob_data), (void(*)(void *))0);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    rowid = sqlite3_last_insert_rowid(db);
    sqlite3_blob_open(db, "main", "t", "data", rowid, 0, &blob);
    errmsg16 = sqlite3_errmsg16(db);

    // step 4: Cleanup
    sqlite3_blob_close(blob);
    str_result = sqlite3_str_finish(builder);
    sqlite3_free(str_result);
    sqlite3_reset_auto_extension();
    sqlite3_close(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rowid;
    (void)errmsg16;
    (void)builder;
    (void)blob_data;
    // API sequence test completed successfully
    return 66;
}