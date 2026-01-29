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
//<ID> 355
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
    const char *create_sql = "CREATE TABLE IF NOT EXISTS demo(id INTEGER PRIMARY KEY, data BLOB);";
    const char *insert_sql = "INSERT INTO demo(id, data) VALUES(1, x'414243');";
    const char *select_sql = "SELECT data FROM demo WHERE id=1;";
    const char *errmsg = NULL;
    const void *selected_blob = NULL;
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_finalize = 0;
    int rc_enable_ext = 0;
    int rc_blob_open = 0;
    int rc_blob_close = 0;

    // step 2: Setup - open database and enable loadable extensions
    rc_open = sqlite3_open(":memory:", &db);
    rc_enable_ext = sqlite3_enable_load_extension(db, 1);

    // step 3: Core operations - create table, insert a blob, select it and reset scanstatus
    rc_prepare = sqlite3_prepare_v2(db, create_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prepare = sqlite3_prepare_v2(db, insert_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;
    rc_prepare = sqlite3_prepare_v2(db, select_sql, -1, &stmt, NULL);
    rc_step = sqlite3_step(stmt);
    sqlite3_stmt_scanstatus_reset(stmt);
    selected_blob = sqlite3_column_blob(stmt, 0);
    rc_finalize = sqlite3_finalize(stmt);
    stmt = NULL;

    // step 4: Cleanup - open and close a blob handle, capture error message, and close database
    rc_blob_open = sqlite3_blob_open(db, "main", "demo", "data", (sqlite3_int64)1, 0, &blob);
    rc_blob_close = sqlite3_blob_close(blob);
    blob = NULL;
    errmsg = sqlite3_errmsg(db);
    sqlite3_close(db);

    (void)selected_blob;
    (void)errmsg;
    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_finalize;
    (void)rc_enable_ext;
    (void)rc_blob_open;
    (void)rc_blob_close;
    (void)create_sql;
    (void)insert_sql;
    (void)select_sql;
    // API sequence test completed successfully
    return 66;
}