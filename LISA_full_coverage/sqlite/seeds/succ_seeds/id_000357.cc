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
//<ID> 357
//<Prompt> []
/*<Combination>: [
*/
//<score> 0, nr_unique_branch: 0
//<Quality> {"density":0,"unique_branches":{},"library_calls":[],"critical_calls":[],"visited":0}
/**/
int test_sqlite3_api_sequence() {
    // step 1: Declarations
    sqlite3 *db = NULL;
    sqlite3_stmt *stmt_create = NULL;
    sqlite3_stmt *stmt_insert = NULL;
    sqlite3_stmt *stmt_select = NULL;
    sqlite3_blob *blob = NULL;
    const char *errmsg = NULL;
    char buffer[16];
    int rc_open = 0;
    int rc_prepare = 0;
    int rc_step = 0;
    int rc_blob_open = 0;
    int bytes = 0;

    // step 2: Setup - open in-memory DB and enable loadable extensions
    rc_open = sqlite3_open(":memory:", &db);
    sqlite3_enable_load_extension(db, 1);

    // step 3: Core operations - create table, insert a zeroblob, prepare a select, operate on blob and inspect stmt state
    rc_prepare = sqlite3_prepare_v2(db, "CREATE TABLE demo(data BLOB);", -1, &stmt_create, NULL);
    rc_step = sqlite3_step(stmt_create);
    rc_prepare = sqlite3_finalize(stmt_create);
    rc_prepare = sqlite3_prepare_v2(db, "INSERT INTO demo(data) VALUES(zeroblob(10));", -1, &stmt_insert, NULL);
    rc_step = sqlite3_step(stmt_insert);
    rc_prepare = sqlite3_finalize(stmt_insert);
    rc_prepare = sqlite3_prepare_v2(db, "SELECT data FROM demo WHERE rowid = 1;", -1, &stmt_select, NULL);
    sqlite3_stmt_scanstatus_reset(stmt_select);
    rc_blob_open = sqlite3_blob_open(db, "main", "demo", "data", 1, 0, &blob);
    bytes = sqlite3_blob_bytes(blob);
    memset(buffer, 0, sizeof(buffer));
    sqlite3_blob_read(blob, buffer, bytes, 0);
    errmsg = sqlite3_errmsg(db);

    // step 4: Cleanup - close blob, finalize statement, close database
    sqlite3_blob_close(blob);
    sqlite3_finalize(stmt_select);
    sqlite3_close(db);

    (void)rc_open;
    (void)rc_prepare;
    (void)rc_step;
    (void)rc_blob_open;
    (void)bytes;
    (void)buffer;
    (void)errmsg;
    // API sequence test completed successfully
    return 66;
}