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
//<ID> 350
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
    const char *dbfile = ":memory:";
    const char *create_sql = "CREATE TABLE IF NOT EXISTS t(id INTEGER PRIMARY KEY, data BLOB);";
    const char *insert_sql = "INSERT INTO t(id,data) VALUES(1, x'4142');";
    const char *select_sql = "SELECT data FROM t WHERE id=1;";
    const char *errmsg = NULL;
    unsigned char *serialized = NULL;
    void *buffer = NULL;
    int rc_open = 0;
    int rc_enable_ext_on = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_blob_open = 0;
    int blob_bytes = 0;
    int rc_blob_read = 0;
    int rc_blob_close = 0;
    int rc_close = 0;

    // step 2: Setup (Initialize and configure)
    rc_open = sqlite3_open(dbfile, &db);
    rc_enable_ext_on = sqlite3_enable_load_extension(db, 1);

    // step 3: Core operations / Operate and Validate
    rc_prepare = sqlite3_prepare_v2(db, create_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_stmt_scanstatus_reset(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prepare = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    rc_prepare = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    rc_blob_open = sqlite3_blob_open(db, "main", "t", "data", (sqlite3_int64)1, 0, &blob);
    blob_bytes = sqlite3_blob_bytes(blob);
    buffer = sqlite3_malloc((size_t)blob_bytes + 1);
    if (buffer) memset(buffer, 0, (size_t)blob_bytes + 1);
    rc_blob_read = sqlite3_blob_read(blob, buffer, blob_bytes, 0);
    rc_blob_close = sqlite3_blob_close(blob);
    blob = NULL;

    errmsg = sqlite3_errmsg(db);

    // step 4: Cleanup (final validation and resource release)
    sqlite3_enable_load_extension(db, 0);
    sqlite3_free(serialized);
    rc_close = sqlite3_close(db);

    (void)rc_open;
    (void)rc_enable_ext_on;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_blob_open;
    (void)blob_bytes;
    (void)rc_blob_read;
    (void)rc_blob_close;
    (void)rc_close;
    (void)errmsg;
    (void)dbfile;
    (void)create_sql;
    (void)insert_sql;
    (void)select_sql;
    (void)serialized;
    (void)buffer;

    // API sequence test completed successfully
    return 66;
}